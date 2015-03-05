package com.LangStack.Cpp2Java;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class ClassDefs 
{
    private String                  mNamespace;
    private String                  mClassName;
    private List<MethodDefs>        mMethods = new ArrayList<MethodDefs>();
    private Map<String, MemberDefs> mMembers = new HashMap<String, MemberDefs>();

	/**
	 * @brief 		构造类定义
	 * @param 		namespace		类命名空间(""为全局类)
	 * @param 		className		类名(""为全局函数)
	 */
	public ClassDefs(String namespace, String className) {
		mNamespace = namespace;
		mClassName = className;
	}
	
	/**
	 * @brief		增加一个方法
	 * @param 		name			方法名
	 * @return		方法定义
	 */
	public MethodDefs addMethod(String name) {
		MethodDefs method = new MethodDefs();
		mMethods.add(method);
		return method;
	}
	
	/**
	 * @brief		增加一个成员
	 * @param 		name			成员名
	 * @return		成员定义
	 */
	public MemberDefs addMember(String name) {
		MemberDefs member = new MemberDefs();
		mMembers.put(name, member);
		return member;
	}
	
	
	public String getNamespace() {
		return mNamespace;
	}

	public void setNamespace(String mNamespace) {
		this.mNamespace = mNamespace;
	}
	
	public String getClassName() {
		return mClassName;
	}

	public void setClassName(String mClassName) {
		this.mClassName = mClassName;
	}

	public List<MethodDefs> getMethods() {
		return mMethods;
	}

	public Map<String, MemberDefs> getMembers() {
		return mMembers;
	}

}
