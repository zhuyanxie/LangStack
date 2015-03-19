package com.LangStack.Cpp2Java;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Scanner {

    private String              mRoot     = ".";
    private List<String>        mPaths    = new ArrayList<String>();
    private List<String>        mFiles    = new ArrayList<String>();
    private Map<String, String> mScanInfo = new HashMap<String, String>();
    private Symbols             mSymbols  = null;
    
    private String              mFile;      ///< 当前扫描文件
    private int                 mLine;      ///< 当前行
    private String              mCache;     ///< 上次解析缓存
    private String              mComment;   ///< 本次读取的注释
    private int                 mPermission;///< 访问权限0:public 1:protected 2:private

    public static final int CLASS_SCOPE = 0;
    public static final int EMPTY_SCOPE = 1;
    public static final int NAMES_SCOPE = 2;
    ∂
    BlockingQueue<String>   mNameSpaces = new ArrayBlockingQueue<String>(128);
    BlockingQueue<String>   mClasses    = new ArrayBlockingQueue<String>(128);
    BlockingQueue<Integer>  mScopes     = new ArrayBlockingQueue<Integer>(128);
    
    public Scanner(String root, Symbols symbols) {
        mPermission = 0;
        mRoot       = root;
        mSymbols    = symbols;
    }

    public void scan()
    {
        scanPaths(new File(mRoot));
        scanFiles();  
    }

    public String getRoot() {
        return mRoot;
    }

    public List<String> getPaths() {
        return mPaths;
    }

    public List<String> getFiles() {
        return mFiles;
    }

    public Map<String, String> getScanInfo() {
        return mScanInfo;
    }

    /**
     * @brief       递归指定路径下的所有.h为后缀的文件
     * @param       root
     */
    private void scanPaths(File root) {
        if (!root.isDirectory()) {
            /// 仅支持.h
            if (root.getName().endsWith(".h")) {
                mFiles.add(root.getAbsolutePath());
            }
            return;
        }
        else if (!root.isDirectory()) {
            return;
        }
        
        mPaths.add(root.getAbsolutePath());
        for (File file : root.listFiles()) {
            scanPaths(file);
        }
    }
    
    /**
     * @brief       扫描所有文件
     */
    private void scanFiles() {
    	for (String file : mFiles) {
    	    try {
    	        mFile = file;
    	        System.out.println("scaning " + file);
                scanFile(file);
            } catch (FileNotFoundException fe) {
                fe.printStackTrace();
            } catch (IOException ioe) {
                ioe.printStackTrace();
            } catch (Exception e) {
                e.printStackTrace();
                errorLog("", System.err, true);
            } 
    	}
    }

    private void scanFile(String file) throws IOException {
        FileInputStream fis     = new FileInputStream(new File(file));
        InputStreamReader isr   = new InputStreamReader(fis, "UTF-8");
        BufferedReader br       = new BufferedReader(isr);
        String buff             = "";

        mCache                  = "";
        mLine                   = 0;
        while ((buff = br.readLine()) != null) {
            ++mLine;
            mCache = mCache + buff;
            while (praseCache(br));
        }

        fis.close();
        br.close();
        isr.close();
    }

    /**
     * @brief       解析当前已读缓存
     * @param       br          输入流
     * @return      是否能够继续解析
     * @throws IOException 
     */
    private boolean praseCache(BufferedReader br) throws IOException {
        if (mCache.isEmpty()) {
            return false;
        }
        
        /// TODO 构造有限状态机来处理
        if (matchBlockComment()) {
            return readBlockComment(br);
        } else if (matchLineComment()) {
            return readLineComment(br);
        } else if (matchInclude()) {
            return readInclude(br);
        } else if (matchTypedef()) {
            return readTypedef(br);
        } else if (matchDefine()) {
            return readDefine(br);
        } else if (matchUsing()) {
            return readUsing(br);
        } else if (matchExtern()) {
            return readExtern(br);
        } else if (matchNamespace()) {
            return readNamespace(br);
        } else if (matchClass()) {
            return readClass(br);
        } else if (matchPermission()) {
            return readPermission(br);
        } else if (matchScope()) {
            return readScope(br);
        } else if (matchPrefixSemicolon()) {
            return readPrefixSemicolon(br);
        } else if (matchTemplate()) {
            errorLog("WARN: now not support template drop- -!!!", 
                    System.out, false);
            dropCurrentBlock(br);
        } else if (matchBlock()) {
            return parseBlock(br);
        }
        
        return false;
    }

    private boolean readPrefixSemicolon(BufferedReader br) {
        do {
            Matcher m = ScannerPattern.DROP_SEMICOLON_SCOPE.matcher(mCache);
            if (m.find()) mCache = m.group(0);
            else mCache = "";
        } while (matchPrefixSemicolon());
        
        return !mCache.equals("");
    }

    private boolean matchPrefixSemicolon() {
        return ScannerPattern.SEMICOLON_SCOPE.matcher(mCache).find();
    }


    class BlockPosition {
        public int semicolonPos;    ///< ";"的位置
        public int lBraceCount;     ///< "{"的数量
        public int rBraceCount;     ///< "}"的数量
        public int left;            ///< "{.....}"中的"{"位置
        public int right;           ///< "{.....}"中的"}"位置
        
        public BlockPosition() {
            semicolonPos    = 0;
            lBraceCount     = 0;
            rBraceCount     = 0;
            left            = 0;
            right           = 0;
        }
    }

    private void dropCurrentBlock(BufferedReader br) throws IOException  {
        BlockPosition pos = new BlockPosition();
        if (readNextBlock(br, pos)) {
            mCache = mCache.substring(pos.semicolonPos + 1, mCache.length()).trim();
        } else if (pos.lBraceCount == pos.rBraceCount) {
            mCache = mCache.substring(pos.right + 1, mCache.length()).trim();
        } else {
            mCache = "";
        }
    }

    private boolean parseBlock(BufferedReader br) throws IOException {
        BlockPosition pos = new BlockPosition();
        if (readNextBlock(br, pos)) {
            return parseSemicolon(pos);
        } else {
            return parseBrace(pos);
        }
    }
    
    private boolean matchBlock() {
        return mCache.indexOf(";") != -1 || mCache.indexOf("{") != -1;
    }
    
    private boolean parseBrace(BlockPosition pos) {
        if (pos.lBraceCount == pos.rBraceCount) {
            parseMethod(mCache.substring(0, pos.left).trim());
            mCache = mCache.substring(pos.right + 1, mCache.length()).trim();
        } else {
            mCache = "";
        }
        
        return !mCache.equals("");
    }

    /**
     * @brief 读取下一块以;结尾或者以{}的区域
     * 1 ;  结尾
     * 2 {} 花括号域
     * @param       pos       域位置信息
     * @return true";"域 | false "{}"域
     */
    private boolean readNextBlock(BufferedReader br, BlockPosition pos) throws IOException {
        boolean isBraceBlock = cacheIsBraceBlock(br, pos);
        if (isBraceBlock) {
            readBrace(br, pos);
            return false;
        } else {
            return true;
        }
    }

    /**
     * @brief       解析 ";"域
     * @param       pos         ";"域位置信息
     * @return
     */
    private boolean parseSemicolon(BlockPosition pos) {
        String block = mCache.substring(0, pos.semicolonPos);
        parseBlock(block);
        mCache = mCache.substring(pos.semicolonPos + 1, mCache.length()).trim();
        return !mCache.equals("");
    }

    /**
     * @brief       解析指定域
     */
    private void parseBlock(String block) {
        if (block.indexOf("(") != -1) {
            parseMethod(block);
        } else {
            parseMember(block);
        }
    }

    /**
     * @brief       成员变量解析
     * @param       block       成员变量字符串
     */
    private void parseMember(String block) {
        /// 获取指定类，增加成员
        ClassDefs cls = mSymbols.getClassDef(getNamespace(), getCurrentClass());
        Map<String, MemberDefs> members = cls.getMembers();
        int nameIndex = block.lastIndexOf(" ");
        String name = block.substring(nameIndex, block.length()).trim();
        
        if (members.containsKey(name)) {
            errorLog("ERROR: repeat member : " + name + "in class : " + members, 
                    System.err, true);
        }
        
        MemberDefs member = new MemberDefs(block, mFile, mLine);
        member.setPublic(mPermission == 0);
    }

    /**
     * @brief       解析方法
     * @param       block       方法字符串
     */
    private void parseMethod(String block) {
        int nameIndex = block.indexOf("(");
        String name = block.substring(0, nameIndex).trim();
        int spaceIndex = name.lastIndexOf(" ", nameIndex);
        if (spaceIndex != -1) {
            name = name.substring(spaceIndex + 1, name.length());
            /// inline 方法直接忽略
            if (name.contains("::")) return;
        }
        
        ClassDefs cls = mSymbols.getClassDef(getNamespace(), getCurrentClass());
        List<MethodDefs> methods = cls.getMethods();
        methods.add(
                new MethodDefs(cls.getClassName(), name, block, mFile, mLine));
    }

    /**
     * @param       bracePos        {}域位置信息
     * @brief       读取包含{}的域
     */
    private void readBrace(BufferedReader br, BlockPosition bracePos) throws IOException {        
        int next = 0;
        do {
            int lb = mCache.indexOf("{", next);
            int rb = mCache.indexOf("}", next);
            if (rb != -1) {
                if (rb < lb || lb == -1) {
                    next = rb + 1;
                    ++bracePos.rBraceCount;
                    bracePos.right = rb;
                }
            } else if (lb != -1) {
                next = lb + 1;
                if (bracePos.left == 0) bracePos.left = lb;
                ++bracePos.lBraceCount;
            }
            
            if (bracePos.lBraceCount == bracePos.rBraceCount) break;
            if (lb != -1 || rb != -1) continue;
            
            String buff = br.readLine();
            ++mLine;
            if (buff == null) {
                mCache = "";
                return;
            }
            mCache = mCache + buff;
        } while (true);
    }

    private boolean cacheIsBraceBlock(BufferedReader br, BlockPosition pos) throws IOException {
        while (true) {
            int semicolon = mCache.indexOf(";");
            int brace = mCache.indexOf("{");
            
            /// 找到分号，未找到左括号
            if (brace == -1) {
                if (semicolon != -1) {
                    pos.semicolonPos = semicolon;
                    return false;
                }
            } else {
                if (semicolon != -1 && semicolon < brace) {
                    return false;
                } else {
                    return true;
                }
            }
            
            String buff = br.readLine();
            ++mLine;
            if (buff == null) return false;
            mCache = mCache + buff;
        }
        
    }

    private boolean matchTemplate() {
        return ScannerPattern.TEMPLATE.matcher(mCache).find();
    }

    private boolean readScope(BufferedReader br) {
        int start   = mCache.indexOf("{");
        int end     = mCache.indexOf("}");
        int idx     = -1;
        if (end == -1 || (start != -1 && start < end)) {
            mScopes.put(EMPTY_SCOPE);
            idx = start;
        } else {
            int scope = mScopes.poll();
            switch (scope) {
            case CLASS_SCOPE: 
                mClasses.poll();
                break;
            case NAMES_SCOPE: 
                mNameSpaces.poll();
                break;
            default:
                break;
            }
            idx = end;
        }
        mCache = mCache.substring(idx + 1, mCache.length()).trim();
        return !mCache.equals("");
    }

    private boolean matchScope() {
        return ScannerPattern.SCOPE.matcher(mCache).find();
    }

    private boolean matchPermission() {
        return ScannerPattern.PERMISSION.matcher(mCache).find();
    }

    private boolean readPermission(BufferedReader br) {
        if (mCache.indexOf("public") != -1) mPermission = 0;
        else if (mCache.indexOf("protected") != -1) mPermission = 1;
        else mPermission = 2;
        
        mCache = mCache.substring(mCache.indexOf(":") + 1, mCache.length()).trim();
        return !mCache.equals("");
    }

    private boolean readMember(BufferedReader br) {
        Matcher m = ScannerPattern.MEMBER.matcher(mCache);
        String type = m.group(0);
        String name = m.group(1);
        if (mPermission == 0) {
            /// FIXME只保留public的成员
            MemberDefs member = mSymbols.getClassDef(
                    getNamespace(), getCurrentClass()).addMember(name, false);
            member.setCppType(type);
        }
        
        mCache = m.group(2) == null ? "" : m.group(2);
        return !mCache.equals("");
    }

    private String getCurrentClass() {
        if (mClasses.isEmpty()) return "";
        return mClasses.peek();
    }
    
    private String getNamespace() {
        String namespace = "";
        for (String name : mNameSpaces) {
            if (!namespace.equals("")) namespace = namespace + ".";
            namespace = namespace + name;
        }
        
        return namespace;
    }

    private boolean matchMember() {
        return ScannerPattern.MEMBER.matcher(mCache).find();
    }
    
    private boolean readClass(BufferedReader br) throws IOException {
        if (!readCache(ScannerPattern.CLASS, br, " ")) return false;
        if (ScannerPattern.CLASS_TAG.matcher(mCache).find()) {
            mPermission = 1;
        } else {
            mPermission = 0;
        }
        
        int classDefsStart  = mCache.indexOf(":");
        int classDefsEnd    = mCache.indexOf("{");
        String classdefine  = mCache.substring(0, 
                classDefsStart == -1 ? classDefsEnd : classDefsStart);
        
        String[] classes    = classdefine.split(" ");
        if (classes != null && classes.length != 0) {
            String classname = classes[classes.length - 1];
            try {
                mSymbols.addClass(getNamespace(), classname);
                mClasses.put(classname);
                mScopes.put(CLASS_SCOPE);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        } else {
            System.err.println("\tERROR: unspport class prase from : ");
            System.err.printf("\t\tfile[%s:%d], %s\r\n", mFile, mLine, mCache);
            System.exit(-1);
        }
        
        /// TODO
        mCache = mCache.substring(classDefsEnd + 1, mCache.length()).trim();
        return !mCache.equals("");
    }

    private boolean matchClass() {
        return ScannerPattern.CLASS_START.matcher(mCache).find();
    }

    private boolean readNamespace(BufferedReader br) throws IOException {
        if (!readCache(ScannerPattern.NAMESPACE, br)) return false;
        String []names = mCache.split(" ");
        String namespace = names[1];
        mCache = "";
        try {
            mNameSpaces.put(namespace);
            mScopes.put(NAMES_SCOPE);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        
        return false;
    }

    private boolean matchNamespace() {
        return ScannerPattern.NAMESPACE_START.matcher(mCache).find();
    }
    
    private boolean matchExtern() {
        return ScannerPattern.EXTERN_START.matcher(mCache).find();
    }

    private boolean readExtern(BufferedReader br) throws IOException {
        int scope = -1;
        int semicolon = -1;
        while (true) {        
            scope = mCache.indexOf("{");
            semicolon = mCache.indexOf(";");
            if (scope != -1 || semicolon != -1) {
                break;
            }
            
            String buff = br.readLine();
            ++mLine;
            if (buff == null) return false;
            mCache = mCache + buff;
        }
        int idx = -1;
        if (scope != -1) {
            idx = scope;
            mScopes.put(EMPTY_SCOPE);
        } else {
            idx = semicolon;
        }
        mCache = mCache.substring(idx + 1, mCache.length());
        return !mCache.equals("");
    }

    private boolean readUsing(BufferedReader br) throws IOException {
        readCache(ScannerPattern.USING, br);
        return false;
    }

    private boolean matchUsing() {
        return ScannerPattern.USING_START.matcher(mCache).find();
    }

    private boolean readDefine(BufferedReader br) throws IOException {
        if (!readCache(ScannerPattern.DEFINE, br)) return false;
        System.out.printf("WARN: unspport define parse ingore :\r\n");
        System.out.printf("\t\"%s\"\r\n", mCache);
        System.out.printf("\tfile[%s:%d]\r\n", mFile, mLine);
        mCache = "";
        
        return false;
    }

    private boolean matchDefine() {
        return ScannerPattern.DEFINE_START.matcher(mCache).find();
    }

    private boolean readCache(Pattern p, BufferedReader br) throws IOException {
        return readCache(p, br, "\r\n");
    }
    
    private boolean readCache(Pattern p, BufferedReader br, String flag)
            throws IOException {
        while (true) {        
            Matcher m = p.matcher(mCache);
            if (m.find()) {
                break;
            }
            
            String buff = br.readLine();
            ++mLine;
            if (buff == null) return false;
            mCache = mCache + buff + flag;
        } 
        return true;
    }
    
    private boolean readTypedef(BufferedReader br) throws IOException {
        String typedefs = null;
        Matcher m = null;
        while (true) {        
            m = ScannerPattern.TYPEDEF.matcher(mCache);
            if (m.find()) {
                typedefs = mCache.substring(0, mCache.indexOf(";"));
                break;
            }
            
            String buff = br.readLine();
            ++mLine;
            if (buff == null) return false;
            mCache = mCache + buff;
        }
        int idx = typedefs.lastIndexOf(" ");
        if (idx != -1) {
            int typedefIndex = typedefs.indexOf("typedef") + "typedef".length();
            String type = typedefs.substring(typedefIndex, idx).trim();
            String alias = typedefs.substring(idx, typedefs.length()).trim();
            mSymbols.getTypeDefs().addTypeDefine(mFile, mLine, mCache, type, alias);
        } else {
            System.out.println("WARN unmatch include format from:");
            System.out.printf("\tfile[%s:%d], %s\r\n", mFile, mLine, mCache);
        }
        
        mCache = mCache.substring(mCache.indexOf(";") + 1, mCache.length()).trim();
        return !mCache.equals("");
    }

    private boolean matchTypedef() {
        return ScannerPattern.TYPEDEF_START.matcher(mCache).find();
    }

    private boolean readInclude(BufferedReader br) {
        if (!ScannerPattern.INCLUDE.matcher(mCache).find()) {
            System.out.println("WARN unmatch include format from:");
            System.out.printf("\tfile[%s:%d], %s\r\n", mFile, mLine, mCache);
        }
        mCache = "";
        return false;
    }

    private boolean matchInclude() {
        return ScannerPattern.INCLUDE_START.matcher(mCache).find();
    }
    
    private boolean readComment(Pattern commentPattern, BufferedReader br) 
            throws IOException {
        while (true) {        
            Matcher m = commentPattern.matcher(mCache);
            if (m.find()) {
                mComment    = m.group(0);
                return true;
            }
            
            String buff = br.readLine();
            ++mLine;
            if (buff == null) return false;
            mCache = mCache + "\r\n" + buff;
        } 
    }

    private boolean readLineComment(BufferedReader br) throws IOException {
        readComment(ScannerPattern.LINE_COMMENT, br);
        mCache = "";
        return false;
    }

    private boolean matchLineComment() {
        return ScannerPattern.LINE_COMMENT_START.matcher(mCache).find();
    }

    private boolean readBlockComment(BufferedReader br) throws IOException {
        int idx = -1;
        while (true) {
            idx = mCache.indexOf("*/");
            if (idx != -1) {
                break;
            }
            
            String buff = br.readLine();
            ++mLine;
            if (buff == null) return false;
            mCache = mCache + "\r\n" + buff;
        }
        idx = idx + "/*".length();
        mComment = mCache.substring(0, idx);
        mCache = mCache.substring(idx, mCache.length()).trim();
        return false;
    }

    private boolean matchBlockComment() {
        return ScannerPattern.BLOCK_COMMENT_START.matcher(mCache).find();
    }
    
    private void errorLog(String errorInfo, PrintStream os, boolean terminal) {
        Logger.e(errorInfo, os, terminal, mFile, mLine, mCache);
    }
    
}

