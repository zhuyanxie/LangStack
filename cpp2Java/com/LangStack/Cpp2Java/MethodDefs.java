package com.LangStack.Cpp2Java;

import java.io.FileOutputStream;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.List;

public class MethodDefs {
    private String        mClassName;                            ///< 类名
    private String        mMethodName;                           ///< 方法名
    private boolean       mIsStatic;                             ///< 静态方法标记
    private List<Method>  mParams     = new ArrayList<Method>(); ///< 首参返回值
    private TypeDefs      mTypes      = null;                    ///< 类型定义引用
    private String        mComment;                              ///< 方法注释

    class Method 
    {
        private String  mParam;     ///< 参数类型字符串
        private int     mType;      ///< 类型定义@see TypeDefs
        private String  mName;      ///< 参数名
        
        public String getParam() {
            return mParam;
        }
        public void setParam(String param) {
            this.mParam = param;
        }
        public int getType() {
            return mType;
        }
        public String getName() {
            return mName;
        }
        public void setName(String name) {
            this.mName = name;
        }
        public boolean paramToType() {
            mType = mTypes.getType(mParam);
            return mType != TypeDefs.TYPE_UNKOWN;
        }
    }
   
    public MethodDefs(String className, String methodName, boolean isStatic) {
        mClassName = className;
        mMethodName = methodName;
        mIsStatic = isStatic;
    }
    
    public void addParam(String param, String name) {
        Method method = new Method();
        method.setParam(param);
        method.setName(name);
    }
    
    public boolean paramToType() {
        for (Method method : mParams) {
            if (!method.paramToType()) {
                return false;
            }
        }
        
        return true;
    }
    
    /**
     * @brief       生成java部分代码
     * @param       fout            文件输出流
     */
    public void genJava(FileOutputStream fout) {
        PrintStream p = new PrintStream(fout);
        
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


    /// gets/sets
    public String getComment() {
        return mComment;
    }

    public void setComment(String mComment) {
        this.mComment = mComment;
    }

    public String getClassName() {
        return mClassName;
    }

    public String getMethodName() {
        return mMethodName;
    }

    public boolean isStatic() {
        return mIsStatic;
    }

    public List<Method> getParams() {
        return mParams;
    }

    public TypeDefs getTypes() {
        return mTypes;
    }
}
