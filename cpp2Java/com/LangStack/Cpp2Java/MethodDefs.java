package com.LangStack.Cpp2Java;

import java.io.FileOutputStream;
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
        private String  mCppType;       ///< c++参数类型
        private int     mEnumType;      ///< 类型定义@see TypeDefs
        private String  mName;          ///< 参数名
        
        public String getCppType() {
            return mCppType;
        }
        public void setCppType(String param) {
            this.mCppType = param;
        }
        public int getType() {
            return mEnumType;
        }
        public String getName() {
            return mName;
        }
        public void setName(String name) {
            this.mName = name;
        }
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
    
    public void addParam(String param, String name) {
        Param method = new Param();
        method.setCppType(param);
        method.setName(name);
    }
    
    public boolean paramToType() {
        for (Param method : mParams) {
            if (!method.cpp2Enum()) {
                return false;
            }
        }
        
        return true;
    }
    
    /**
     * @brief       生成java部分代码
     * @param       fout            文件输出流
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
        p.print(mTypes.getJavaType(mParams.get(0).getType()) + " ");
        p.print(mMethodName + "(");
        for (int i = 1; i < mParams.size(); ++i) {
            if (i != 1) p.print(", ");
            p.print(mTypes.getJavaType(mParams.get(i).getType()) + " ");
            p.print(mParams.get(i).getName());
        }
        p.println(") {");

        p.print("        ");
        if (mParams.get(0).getType() != TypeDefs.TYPE_VOID) {
            p.print("return ");
        }
        p.print("call(\"" + mMethodName + "\", this.getClass().getName(), this");
        for (int i = 1; i < mParams.size(); ++i) {
            p.print(", " + mParams.get(i).getName());
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
        p.printf("%s reflect_proxy_%s(", retval.getCppType(), retval.getName());
        for (int i = 1; i < mParams.size(); ++i) {
            if (i != 1) p.print(',');
            p.printf("const ls::MetaValue t%d", i);
        }
        p.println("{");
        if (retval.getType() == TypeDefs.TYPE_VOID) p.print("    ");
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
                    mParams.get(i).getCppType());
        }
        Param retval = mParams.get(0);
        p.printf("}, new ::ls::ReflectFunciton<%s>(\r\n"
                + "        &%s::reflect_proxy_%s,(%s*)0))\r\n", 
                retval.getCppType(), getCppClassName(), 
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
