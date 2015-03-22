package com.LangStack.Cpp2Java;

import java.io.PrintStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

public class ClassDefs 
{
    private String                  mCppNamespace;          ///<  类命名空间
    private String                  mCppClassName;          ///<  类名
    private String                  mJavaClassName = null;  ///<  java类名
    private String                  mJavaPackage;           ///<  java包

    private ContantDefs             mConsts        = null;  ///<  类常量定义
    private List<MethodDefs>        mMethods       = null;  ///<  类方法
    private Map<String, MemberDefs> mMembers       = null;  ///<  类成员
    private Set<String>             mDepends       = null;  ///<  依赖包

    private TypeDefs                mTypes;                 ///<  类型定义引用

	/**
	 * @brief 		构造类定义
	 * @param 		namespace		类命名空间(""为全局类)
	 * @param 		className		类名(""为全局函数)
	 */
	public ClassDefs(String namespace, String className, TypeDefs types) {
	    mConsts  = new ContantDefs();
	    mMethods = new ArrayList<MethodDefs>();
	    mMembers = new HashMap<String, MemberDefs>();
	    mDepends = new HashSet<String>();
	    
        mCppNamespace = namespace;
        mCppClassName = className;
        mTypes        = types;
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
		MethodDefs method = new MethodDefs(mCppClassName, name, isStatic);
		mMethods.add(method);
		return method;
	}

    public void addMethod(MethodDefs method) {
        method.setTypes(mTypes);
        mMethods.add(method);
    }
	
	/**
	 * @brief		增加一个成员
	 * @param 		name			成员名
	 * @return		成员定义
	 */
	public MemberDefs addMember(String name, boolean isStatic) {
		MemberDefs member = new MemberDefs(mTypes, isStatic);
		mMembers.put(name, member);
		return member;
	}
    public void addMember(String name, MemberDefs member) {
        mMembers.put(name, member);
        member.setTypes(mTypes);
    }
	
	/**
	 * @brief      解析java类
	 */
	public void parseJavaClass() {
        parsePackage();
	    parseDepend();
	    parseMember();
	    parseMethod();
	}

    /**
	 * @brief      生成java类
	 */
	public void genJavaClass(PrintStream p) {
	    genJavaPackage(p);
	    genJavaDepend(p);
	    genJavaMember(p);
	    genJavaMethod(p);
	}

    private void genJavaPackage(PrintStream p) {
        p.printf("package %s;\r\n\r\n", mJavaPackage);
    }

    private void genJavaDepend(PrintStream p) {
        for (String d : mDepends) {
            p.printf("import %s;\r\n", d);
        }
        p.print("\r\n");
    }

    private void genJavaMember(PrintStream p) {
        for (MemberDefs member : mMembers.values()) {
            member.genJava(p);
        }
    }

    private void genJavaMethod(PrintStream p) {
        for (MethodDefs method : mMethods) {
            method.genJava(p);
        }
    }
    
    private void parseMember() {
        for (MemberDefs member : mMembers.values()) {
            member.parse();
        }
    }

    private void parseMethod() {
        for (MethodDefs method : mMethods) {
            method.parse();
        }
    }
    
	private void parseDepend() {
	    /// TODO 目前只使用java.util.*
	    mDepends.add("java.util.*;");
	}
	
    private void parsePackage() {
        if (getJavaClassName() != null) {
            int pos = getJavaClassName().lastIndexOf(".");
            if (pos != -1) {
                setJavaPackage(getJavaClassName().substring(0, pos));
                setJavaClassName(getJavaClassName().substring(
                        pos + 1, getJavaClassName().length()));
            }
            else {
                String pack = "com";
                String []names = getNamespace().split(".");
                for (String name : names) {
                    pack = pack + "." + name;
                }
                setJavaPackage("pack");
            }
        } else {
            setJavaClassName(getClassName());
        }
    }

	public String getNamespace() {
		return mCppNamespace;
	}

	public void setNamespace(String cppNamespace) {
		this.mCppNamespace = cppNamespace;
	}
	
	public String getClassName() {
		return mCppClassName;
	}

	public void setClassName(String mClassName) {
		this.mCppClassName = mClassName;
	}

	public List<MethodDefs> getMethods() {
		return mMethods;
	}

	public Map<String, MemberDefs> getMembers() {
		return mMembers;
	}
	
    public String getJavaClassName() {
        return mJavaClassName;
    }

    public void setJavaClassName(String javaClassName) {
        this.mJavaClassName = javaClassName;
    }

    public String getJavaPackage() {
        return mJavaPackage;
    }

    public void setJavaPackage(String javaPackage) {
        this.mJavaPackage = javaPackage;
    }

    public ContantDefs getConsts() {
        return mConsts;
    }

    public void setConsts(ContantDefs consts) {
        this.mConsts = consts;
    }

}
