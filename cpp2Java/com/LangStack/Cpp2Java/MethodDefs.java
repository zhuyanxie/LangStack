package com.LangStack.Cpp2Java;

import java.io.PrintStream;
import java.util.ArrayList;
import java.util.List;

public class MethodDefs {
    private String        mCppClassName;                         ///< 类名
    private String        mMethodName;                           ///< 方法名
    private boolean       mIsStatic;                             ///< 静态方法标记
    private List<Param>   mParams     = new ArrayList<Param>();  ///< 首参返回值
    private TypeDefs      mTypes      = null;                    ///< 类型定义引用
    private String        mComment;                              ///< 方法注释

    class Param 
    {
        public String  mCppType;       ///< c++参数类型
        public int     mEnumType;      ///< 类型定义@see TypeDefs
        public String  mName;          ///< 参数名
        public String  mFile;          ///< 来源文件
        public String  mLine;          ///< 来源行
        
        public boolean cpp2Enum() {
            mEnumType = mTypes.getCppType(mCppType);
            return mEnumType != TypeDefs.TYPE_UNKOWN;
        }
    }
   
    public MethodDefs(String className, String methodName, boolean isStatic) {
        mCppClassName = className;
        mMethodName = methodName;
        mIsStatic = isStatic;
    }
    
    public void addParam(String cppType, String name) {
        Param method = new Param();
        method.mCppType = cppType;
        method.mName = name;
    }
    
    public void parse() {
        for (Param method : mParams) {
            if (!method.cpp2Enum()) {
                System.out.printf("ERROR: prase param[%s] fail, form [%s:%d]\n",
                        method.mCppType, method.mFile, method.mLine);
                System.exit(-1);
            }
        }
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
