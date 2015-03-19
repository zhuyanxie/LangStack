package com.LangStack.Cpp2Java;

import java.io.PrintStream;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;

public class MethodDefs {
    private String      mCppClassName;                   ///< 类名
    private String      mMethodName;                     ///< 方法名
    private boolean     mIsStatic = false;               ///< 静态方法标记
    private List<Param> mParams = new ArrayList<Param>();///< 首参返回值
    private TypeDefs    mTypes  = null;                  ///< 类型定义引用
    private String      mComment;                        ///< 方法注释
    private String      mBlock;                          ///< 带解析字符串
    private String      mFile;                           ///< 来源文件
    private int         mLine;                           ///< 来源行

    class Param 
    {
        public String  mCppType;       ///< c++参数类型
        public int     mEnumType;      ///< 类型定义@see TypeDefs
        public String  mName;          ///< 参数名
        
        public boolean cpp2Enum() {
            mEnumType = mTypes.getCppType(mCppType);
            return mEnumType != TypeDefs.TYPE_UNKOWN;
        }
    }
    
    public MethodDefs(String className, String methodName, String block, 
            String file, int line) {
        mCppClassName = className;
        mMethodName = methodName;
        mBlock = block;
    }
   
    public MethodDefs(String className, String methodName, boolean isStatic) {
        mCppClassName = className;
        mMethodName = methodName;
        mIsStatic = isStatic;
    }
    
    public void addParam(String cppType, String name) {
        Param param = new Param();
        param.mCppType = cppType;
        param.mName = name;
        mParams.add(param);
    }
    
    public void parse() {
        parseBlock();
        for (Param method : mParams) {
            if (!method.cpp2Enum()) {
                System.out.printf("ERROR: prase param[%s] fail, form [%s:%d]\n",
                        method.mCppType, mFile, mLine);
                System.exit(-1);
            }
        }
    }
    
    private void parseBlock() {
        Matcher m = ScannerPattern.METHOD_SCOPE.matcher(mBlock);
        if (!m.find()) {
            Logger.e("MethodDefs::parseBlock Unkown method format", 
                    System.err, true, mFile, mLine);
        }
        String retVal = m.group(0);
        String params = m.group(1);
        parseReturn(retVal);
        parseParams(params);
    }

    private void parseParams(String paramsBlock) {
        String []params = paramsBlock.split(",");
        
        for (String param : params) {
            int idx = param.lastIndexOf(" ");
            if (idx != -1) {
                addParam(param.substring(0, idx).trim(), 
                         param.substring(param.length(), idx).trim());
            } else {
                Logger.e("MethodDefs::parseBlock Unkown method params format", 
                        System.err, true, mFile, mLine);
            }
        }
    }

    private void parseReturn(String retVal) {
        int staticIndex = mBlock.indexOf("static");
        if (staticIndex != -1) {
            mIsStatic = true;
            staticIndex = staticIndex + "static ".length();
        } else {
            staticIndex = 0;
        }
        
        addParam(retVal.substring(staticIndex, retVal.length()), "");
    }

    /**
     * @brief       生成java部分代码
     * @param       p               输出流
     */
    public void genJava(PrintStream p) {
        if (mMethodName.equals("attach")) {
            genJavaAttach(p);
        } else if (mMethodName.equals("detach")) {
            genJavaDetach(p);
        }
        p.println(mComment);
        
        p.print("    public");
        p.print(isStatic() ? " static " : " ");
        p.print(mTypes.getJavaType(mParams.get(0).mEnumType) + " ");
        p.print(mMethodName + "(");
        for (int i = 1; i < mParams.size(); ++i) {
            if (i != 1) p.print(", ");
            p.print(mTypes.getJavaType(mParams.get(i).mEnumType) + " ");
            p.print(mParams.get(i).mName);
        }
        p.println(") {");

        p.print("        ");
        if (mParams.get(0).mEnumType != TypeDefs.TYPE_VOID) {
            p.print("return ");
        }
        p.print("call(\"" + mMethodName + "\", this.getClass().getName(), this");
        for (int i = 1; i < mParams.size(); ++i) {
            p.print(", " + mParams.get(i).mName);
        }
        p.println(");");
        p.println("}");
    }
    
    private void genJavaAttach(PrintStream p) {
        /// TODO attach
    }
    
    private void genJavaDetach(PrintStream p) {
        /// TODO detach
    }
    
    public void genCppProxy(PrintStream p) {
        if (!mCppClassName.equals(""))
            p.print("        ");
        Param retval = mParams.get(0);
        p.printf("%s reflect_proxy_%s(", retval.mCppType, retval.mName);
        for (int i = 1; i < mParams.size(); ++i) {
            if (i != 1) p.print(',');
            p.printf("const ls::MetaValue t%d", i);
        }
        p.println("{");
        if (retval.mEnumType == TypeDefs.TYPE_VOID) p.print("    ");
        else p.print("    return");
        p.printf("%s(", mMethodName);
        for (int i = 1; i < mParams.size(); ++i) {
            if (i != 1) p.print(',');
            p.printf("t%d", i);
        }
        p.println(");\r\n}");
    }
    
    public void genCppReflectRegist(PrintStream p) {
        p.printf("    ::ls::CMetaFactory::instance()->registerMethod("
                + "%s::getJavaName(), %s, {\r\n", mCppClassName, mMethodName);
        for (int i = 0; i < mParams.size(); ++i) {
            p.printf("        ::ls::Type2MetaDataType< %s >()(), \r\n", 
                    mParams.get(i).mCppType);
        }
        Param retval = mParams.get(0);
        p.printf("}, new ::ls::ReflectFunciton<%s>(\r\n"
                + "        &%s::reflect_proxy_%s,(%s*)0))\r\n", 
                retval.mCppType, getCppClassName(), 
                getMethodName(), getCppClassName());
    }


    public String getComment() {
        return mComment;
    }

    public void setComment(String comment) {
        this.mComment = comment;
    }

    public String getCppClassName() {
        return mCppClassName;
    }

    public String getMethodName() {
        return mMethodName;
    }

    public boolean isStatic() {
        return mIsStatic;
    }

    public List<Param> getParams() {
        return mParams;
    }

    public TypeDefs getTypes() {
        return mTypes;
    }
}
