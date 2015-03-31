package com.LangStack.Cpp2Java;

import java.io.PrintStream;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;

import com.LangStack.Rpc.IRpcApi;
import com.LangStackTest.RealCallback;

public class MethodDefs {
    
    public static final int CONSTRUCT   = 0;            ///< 构造
    public static final int DECONSTRUCT = 1;            ///< 析构
    public static final int METHOD      = 2;            ///< 一般方法、函数
    public static final int ATTACH      = 3;            ///< 注册回调
    public static final int DETACH      = 4;            ///< 注销回调
    public static final int NAME_DEF    = 5;            ///< JAVA类名定义
    
    private ClassDefs   mClass;                          ///< 所属的类
    private ClassDefs   mCallbackClass = null;           ///< 回调类
    private String      mCppClassName;                   ///< 类名
    private String      mMethodName;                     ///< 方法名
    private int         mMethodType = METHOD;            ///< 方法类型
    private boolean     mIsStatic = false;               ///< 静态方法标记
    private boolean     mIsVirual = false;               ///< 虚方法标记
    private List<Param> mParams = new ArrayList<Param>();///< 首参返回值
    private TypeDefs    mTypes  = null;                  ///< 类型定义引用
    private String      mComment;                        ///< 方法注释
    private String      mDetail;                         ///< 方法内容
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
        mFile = file;
        mLine = line;
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
                System.out.printf("ERROR: prase param[%s] fail, form:\r\n"
                        + "\t[%s:%d]\r\n[%s]\r\n",
                        method.mCppType, mFile, mLine, mBlock);
                System.exit(-1);
            }
        }
        
        /// char* + int 被解析为memory
        for (int i = 1; i < mParams.size(); ++i) {
            Param method = mParams.get(i);
            if (i + 1 < mParams.size()) {
                Param methodNext = mParams.get(i + 1);
                if (method.mCppType.indexOf("string") == -1 &&
                        method.mEnumType == TypeDefs.TYPE_STRING && 
                        methodNext.mEnumType == TypeDefs.TYPE_INT32) {
                    method.mEnumType = TypeDefs.TYPE_MEMORY;
                }
            }
        }
    }
    
    private void parseBlock() {
        mBlock = mBlock.replaceAll("\\s+", " ").trim();

        int idx = mBlock.indexOf(mMethodName);
        if (idx == -1) {
            Logger.e("MethodDefs::parseBlock Unkown method format", 
                    System.err, true, mFile, mLine);
        }
        String retVal = mBlock.substring(0, idx);
        
        int l   = mBlock.indexOf("(");
        int r   = mBlock.indexOf(")");
        String params = mBlock.substring(l + 1, r);
        parseMethodType();
        if (mMethodType == NAME_DEF) {
            parseClassJavaName();
        } else {
            parseReturn(retVal);
            parseParams(params);
        }
    }

    private void parseClassJavaName() {
        int returnIndex = mDetail.indexOf("return");
        int l = mDetail.indexOf("\"", returnIndex);
        int r = mDetail.indexOf("\"", l + 1);
        String javaname = mDetail.substring(l, r);
        mClass.setJavaClassName(javaname);
    }

    private void parseMethodType() {
        if (mMethodName.equals("attach")) {
            mMethodType = ATTACH;
        } else if (mMethodName.equals("detach")) {
            mMethodType = DETACH;
        } else if (mMethodName.equals("getClassName")) {
            mMethodType = NAME_DEF;
        } else if (mMethodName.contains("~")) {
            mMethodType = DECONSTRUCT;
        } else {
            mMethodType = METHOD;
        }
    }

    private void parseParams(String paramsBlock) {
        if (paramsBlock.isEmpty()) {
            return;
        }
        
        if (mMethodType == ATTACH ||
                mMethodType == DETACH) {
            parseCallBackParam(paramsBlock);
            return;
        }
        
        String []params = null;
        if (paramsBlock.indexOf(",") != -1) {
            params = paramsBlock.split(",");
        } else {
            params = new String[1];
            params[0] = paramsBlock;
        }
        
        for (String param : params) {
            int idx = param.lastIndexOf(" ");
            int pointIdx = param.lastIndexOf("*");
            int refIdx = param.lastIndexOf("&");
            if (pointIdx > idx) idx = pointIdx;
            if (idx != -1) {
                String type = param.substring(0, idx + 1).trim();
                int constIdx = type.indexOf("const");
                if (constIdx != -1) {
                    type = type.substring(constIdx + "const".length(), 
                            type.length()).trim();
                }
                if (refIdx > idx) idx = refIdx;
                String name = param.substring(idx + 1, param.length()).trim();
                addParam(type, name);
            } else {
                Logger.e("MethodDefs::parseBlock Unkown method params format", 
                        System.err, true, mFile, mLine);
            }
        }
    }

    private void parseCallBackParam(String paramsBlock) {
        int idx = paramsBlock.lastIndexOf(" ");
        int pidx = paramsBlock.lastIndexOf("*");
        
        if (pidx > idx) idx = pidx;
        String paramName = paramsBlock.substring(0, idx).trim();
        ClassDefs cls = mTypes.getClassDefs(paramName);
        if (null == cls) {
            Logger.e("find callback class : " + paramsBlock + " defs fail", 
                    System.err, true, mFile, mLine, mBlock);
        }
        mCallbackClass = cls;
        cls.setCallback(true);
    }

    private void parseReturn(String retVal) {
        if (mMethodType == METHOD && retVal.isEmpty()) {
            mMethodType = CONSTRUCT;
            return;
        }
        if (mMethodType == DECONSTRUCT || 
                mMethodType == ATTACH ||
                mMethodType == DETACH) {
            return;
        }
        
        int index = 0;
        int staticIndex = mBlock.indexOf("static");
        if (staticIndex != -1) {
            mIsStatic = true;
            index = staticIndex + "static".length(); 
        } 
        
        int virtualIndex = mBlock.indexOf("virtual");
        if (virtualIndex != -1) {
            mIsVirual = true;
            index = virtualIndex + "virtual".length(); 
        } else {
            /// TODO 如果还有其他特殊标记需要处理
        }
        
        addParam(retVal.substring(index, retVal.length()).trim(), "");
    }

    /**
     * @brief       生成java部分代码
     * @param       p               输出流
     */
    public void genJava(PrintStream p) {
        if (mMethodType == ATTACH) {
            genJavaAttach(p);
            return;
        } else if (mMethodType == DETACH) {
            genJavaDetach(p);
            return;
        } else if (mMethodType == NAME_DEF ||
                mMethodType == DECONSTRUCT) {
            return;
        } else if (mMethodType == CONSTRUCT) {
            genJavaConstruct(p);
            return;
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

            if (mParams.get(i).mEnumType == TypeDefs.TYPE_MEMORY) {
                ++i;
            }
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
        p.println("}\r\n");
    }
    
    private void genJavaConstruct(PrintStream p) {
        p.printf("    public %s (", mClass.getJavaClassName());
        for (int i = 0; i < mParams.size(); ++i) {
            if (i != 0) p.print(", ");
            p.print(mTypes.getJavaType(mParams.get(i).mEnumType) + " ");
            p.print(mParams.get(i).mName);

            if (mParams.get(i).mEnumType == TypeDefs.TYPE_MEMORY) {
                ++i;
            }
        }
        p.printf(")\r\n    {\r\n");        
        p.printf("    {\r\n");
        p.printf("        call(\"new\", this.getClass().getName(), this");
        for (Param param : mParams) {
            p.printf(", " + param.mName);
        }
        p.printf(");\r\n");
        p.printf("    }\r\n\r\n");
    }

    private void genJavaAttach(PrintStream p) {
        p.printf("    @Override public void attach(IRpcApi o)\r\n");
        p.printf("    {\r\n");
        p.printf("        call(\"new\", %s, o);\r\n", 
                mCallbackClass.getJavaClassName() + "proxy");
        p.printf("        callbackSetting(\"attach\", "
                + "this.getClass().getName(), this, o);\r\n");
        p.printf("    }\r\n\r\n");
    }
    
    private void genJavaDetach(PrintStream p) {
        p.printf("    @Override public void detach(IRpcApi o)\r\n");
        p.printf("    {\r\n");
        p.printf("        callbackSetting(\"detach\", "
                + "this.getClass().getName(), this, o);\r\n");
        p.printf("        call(\"delete\", %s, o);\r\n", 
                mCallbackClass.getJavaClassName() + "proxy");
        p.printf("    }\r\n\r\n");
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

    public void setTypes(TypeDefs types) {
        mTypes = types;
    }

    public void setDetail(String detail) {
        mDetail = detail;
    }

    public void setClass(ClassDefs cls) {
        mClass = cls;
    }

    public int getMethodType() {
        return mMethodType;
    }
}
