package com.LangStack.Cpp2Java;

import java.util.HashMap;
import java.util.Map;

public class Symbols {
	
	private Map<String, ClassDefs> mClasses = new HashMap<String, ClassDefs>();
	
	public Symbols() {
		
	}
	
	public ClassDefs getClass(String className) {
		return mClasses.get(className);
	}
	
	public ClassDefs addClass(String namespace, String className) {
		ClassDefs classDefs = new ClassDefs(namespace, className);
		mClasses.put(className, classDefs);
		return classDefs;
	}
	
	public Map<String, ClassDefs> getClasses() {
		return mClasses;
	}
}
