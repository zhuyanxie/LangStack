package com.LangStack.Cpp2Java;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

public class ClassDefs 
{
    private String                  mNamespace;
    private String                  mClassName;

    private ContantDefs             mConsts  = null;
    private List<MethodDefs>        mMethods = null;
    private Map<String, MemberDefs> mMembers = null;
    private Set<String>             mDepends = null;

	/**
	 * @brief 		构造类定义
	 * @param 		namespace		类命名空间(""为全局类)
	 * @param 		className		类名(""为全局函数)
	 */
	public ClassDefs(String namespace, String className) {
	    mConsts  = new ContantDefs();
	    mMethods = new ArrayList<MethodDefs>();
	    mMembers = new HashMap<String, MemberDefs>();
	    mDepends = new HashSet<String>();
	    
		mNamespace = namespace;
		mClassName = className;
	}
	
	/**
	 * @brief      增加一个倚赖包
	 * @param      depend          依赖包名
	 */
	public void addDepends(String depend) {
	    mDepends.add(depend);
	}
	
	/**
	 * @brief		增加一个方法
	 * @param 		name			方法名
	 * @param       isStatic        是否为静态方法
	 * @return		方法定义
	 */
	public MethodDefs addMethod(String name, boolean isStatic) {
		MethodDefs method = new MethodDefs(mClassName, name, isStatic);
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
