package com.LangStack.Cpp2Java;

import java.util.HashMap;
import java.util.Map;

public class Symbols {

    private Map<String, ClassDefs> mClasses = null;     ///< 类定义
    private TypeDefs               mTypes   = null;     ///< 类型定义
	
	public Symbols() {
	    mClasses = new HashMap<String, ClassDefs>();
	    mTypes   = new TypeDefs();
	    
	    mClasses.put("", new ClassDefs("", ""));
	}

	public ClassDefs getClass(String namespace, String className) {
	    /// TODO namespace 与  classname的关系
		return mClasses.get(namespace + "::" + className);
	}
	
	public ClassDefs addClass(String namespace, String className) {
	    /// TODO namespace 与  classname的关系
		ClassDefs classDefs = new ClassDefs(namespace, className);
		mClasses.put(namespace + "::" + className, classDefs);
		return classDefs;
	}

	public Map<String, ClassDefs> getClasses() {
		return mClasses;
	}
	
	public TypeDefs getTypeDefs() {
	    return mTypes;
	}
	
}
