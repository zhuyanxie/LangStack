package com.LangStack.Cpp2Java;

import java.io.FileNotFoundException;
import java.util.HashMap;
import java.util.Map;

public class Symbols {

    private Map<String, ClassDefs> mClasses = null;     ///< 类定义
    private TypeDefs               mTypes   = null;     ///< 类型定义
    private String                 mGenPath = ".";      ///< 设置生成目录
	
	public Symbols(String genPath) {
	    mClasses = new HashMap<String, ClassDefs>();
	    mTypes   = new TypeDefs();
	    mGenPath = genPath;
	    
	    mClasses.put("", new ClassDefs("", "", mTypes));
	}

    public ClassDefs getClassDef(String namespace, String className) {
	    /// TODO namespace 与  classname的关系
		return mClasses.get(namespace + "::" + className);
	}
	
	public ClassDefs addClass(String namespace, String className, String file, 
	        int line, String detail) {
	    /// TODO namespace 与  classname的关系
		ClassDefs classDefs = new ClassDefs(namespace, className, mTypes);
		classDefs.setNamespace(namespace);
		mClasses.put(namespace + "::" + className, classDefs);
        mTypes.addClassDefine(file, line, detail, classDefs, 
                classDefs.getClassName());
        
        if (namespace.isEmpty()) {
            return classDefs;
        }
        
        if (namespace.indexOf(".") == -1) {
            mTypes.addClassDefine(file, line, detail, classDefs, 
                    namespace.trim() + "::" + className);
            return classDefs;
        }
		
		String []names = namespace.split(".");
		for (int i = names.length - 1; i >= 0; --i) {
		    String name = "";
		    for (int j = 0; j <= i; ++j) {
		        name = name + names[j] + "::";
		    }
            mTypes.addClassDefine(file, line, detail, classDefs, 
                    name + className);
		}
		
		/// TODO class也是一个类型
		return classDefs;
	}

	public Map<String, ClassDefs> getClasses() {
		return mClasses;
	}
	
	public TypeDefs getTypeDefs() {
	    return mTypes;
	}
	
	public void parse() {
	    /// 先解析类型定义
	    mTypes.parseType();
	    
	    /// 类解析
	    for (ClassDefs cls : mClasses.values()) {
	        cls.parseJavaClass();
	    }
	}
	
	public void genJavaCode() {
	       /// 类解析
        for (ClassDefs cls : mClasses.values()) {
            try {
                cls.genJavaClass(mGenPath);
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            }
        }
	}
	
	public void genCppCode() {
	    
	}
}
