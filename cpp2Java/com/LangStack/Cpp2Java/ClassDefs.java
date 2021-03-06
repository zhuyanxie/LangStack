package com.LangStack.Cpp2Java;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import com.LangStack.Rpc.IRpcApi;

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
    private boolean                 mCallback      = false; ///<  是否为回调类

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
	    /// 先解析方法，方法中会带有javaname
        parseMethod();
        parsePackage();
	    parseDepend();
	    parseMember();
	}

    /**
	 * @throws FileNotFoundException 
     * @brief      生成java类
	 */
	public void genJavaClass(String path) throws FileNotFoundException {
	    PrintStream p = makeJavaFile(path);
	    genJavaPackage(p);
	    genJavaDepend(p);
	    genJavaClassHeader(p);
	    if (!isCallback()) {
	        genJavaConstant(p);
	        genJavaMember(p);
	        genJavaMethod(p);
	    } else {
	        genJavaAbstractMethod(p);
	    }
	    genJavaClassTail(p);
	    p.close();
	}


    private void genJavaAbstractMethod(PrintStream p) {
        for (MethodDefs method : mMethods) {
            method.genJavaAbstract(p);
        }
    }

    private void genJavaClassTail(PrintStream p) {
        if (!isCallback()) {
            p.printf("    protected void finalize()\r\n");
            p.printf("    {\r\n");
            p.printf("        call(\"delete\", this.getClass().getName(), this);\r\n");
            p.printf("    }\r\n\r\n");
        }
        p.printf("}\r\n\r\n");
    }

    private void genJavaPackage(PrintStream p) {
        p.printf("package %s;\r\n\r\n", mJavaPackage);
    }

    private PrintStream makeJavaFile(String path) throws FileNotFoundException {
        String dirs = path + "/" + mJavaPackage.replaceAll("\\.", "/");
        File f = new File(dirs);
        if (!f.exists()) {
            f.mkdirs();
        }
        String file = dirs + "/" + getJavaClassName() + ".java";
        File javaFile = new File(file);
        FileOutputStream fos = new FileOutputStream(javaFile);
        return new PrintStream(fos);
    }

    private void genJavaConstant(PrintStream p) {
        p.print("\r\n");
        mConsts.genJava(p);
    }
    
    private void genJavaClassHeader(PrintStream p) {
        p.print("\r\n");
        p.printf("public%sclass %s extends IRpcApi {\r\n", 
                isCallback() ? " abstract " : " ", getJavaClassName());
        
        if (isCallback()) return;
        
        for (MethodDefs method : mMethods) {
            if (method.getMethodType() == MethodDefs.CONSTRUCT) {
                return;
            }
        }
        p.printf("    public %s()\r\n", getJavaClassName());
        p.printf("    {\r\n");
        p.printf("        call(\"new\", this.getClass().getName(), this);\r\n");
        p.printf("    }\r\n\r\n");
    }
    
    private void genJavaDepend(PrintStream p) {
        for (String d : mDepends) {
            p.printf("import %s;\r\n", d);
        }
        p.printf("\r\n");
    }

    private void genJavaMember(PrintStream p) {
        for (MemberDefs member : mMembers.values()) {
            member.genJava(p);
        }
        
        p.printf("\r\n");
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
        mDepends.add("java.util.*");
        mDepends.add("com.LangStack.Rpc.*");
	}
	
	/**
	 * @brief      生成java包名以及类名，全局默认为（Global）
	 */
    private void parsePackage() {
        if (getJavaClassName() != null) {
            int pos = getJavaClassName().lastIndexOf(".");
            if (pos != -1) {
                setJavaPackage(getJavaClassName().substring(0, pos));
                setJavaClassName(getJavaClassName().substring(
                        pos + 1, getJavaClassName().length()));
            }
            else {
                setJavaPackage("com.Global");
            }
        } else {
            setJavaPackage("com." + ((getNamespace().equals("")) ? 
                    "Global" : getNamespace()));
            
            setJavaClassName(getClassName().equals("") ? 
                    "Global" : getClassName());
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

    public boolean isCallback() {
        return mCallback;
    }
    public void setCallback(boolean b) {
        mCallback = b;
    }

}
