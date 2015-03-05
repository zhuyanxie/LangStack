package com.LangStack.Cpp2Java;

import java.util.ArrayList;
import java.util.List;

public class MethodDefs {
    private String        mClassName;
    private String        mMethodName;
    private boolean       mIsStatic;
    private List<Method>  mParams     = new ArrayList<Method>();
    
    class Method 
    {
        public String getParam() {
            return mParam;
        }
        public void setParam(String param) {
            this.mParam = mParam;
        }
        public int getType() {
            return mType;
        }
        public String getName() {
            return mName;
        }
        public void setName(String name) {
            this.mName = mName;
        }
        
        private String mParam;
        private int mType;
        private String mName;
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
    
    /// TODO 方法翻译
}
