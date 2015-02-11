package com.LangStack;

import java.util.HashMap;
import java.util.Map;

/**
 * @brief java类元表，保存类名映射关系，以及java泛型容器中的类型元信息
 * @author fxmqs
 */
public class MetaTable {
    
    public static final int JAVA_NAME_TYPE_NORMAL 	= 0;    ///< m开头首字母大写
    public static final int JAVA_NAME_TYPE_SPECIAL 	= 1;    ///< 首字母小写
    public static final int JAVA_NAME_TYPE_CPP 		= 2;    ///< C++风格 m_前缀

    private Map<String, String> mJava2Rpc;                  ///< 反射到序列化
    private Map<String, String> mRpc2Java; 	                ///< 序列化2反射
    private int mJavaNameType; 				                ///< java命名规范
    
    /**
     * @brief 单例
     * @return 元表句柄
     */
    public static MetaTable instance()
    {
        return SingletonHolder.instance;
    }

    /**
     * @brief 注册反射类名与序列化类名
     * @param cls java中的反射类名，带包名
     * @param serialName 序列化后的名称，(c++类名)
     */
    public void registerClass(String javaName, String rpcName)
    {
        mJava2Rpc.put(javaName, rpcName);
        mRpc2Java.put(rpcName, javaName);
    }

    public String getRpcClassName(String cls)
    {
        if (!mJava2Rpc.containsKey(cls))
        {
            return cls;
        }
        return mJava2Rpc.get(cls);
    }

    public String getJavaClassName(String serial)
    {
        if (!mRpc2Java.containsKey(serial))
        {
            return serial;
        }
        return mRpc2Java.get(serial);
    }

    /**
     * @brief 设置java中的命名规则
     * @param type
     */
    public void setJavaNameType(int type)
    {
        mJavaNameType = type;
    }

    public int getJavaNameType()
    {
        return mJavaNameType;
    }

    public String getRpcMemberName(String javaName)
    {
        if (mJavaNameType == JAVA_NAME_TYPE_NORMAL)
        {
            String preName = javaName.substring(1, 2);
            preName = preName.toLowerCase();
            String realName = javaName.substring(2);
            return "m_" + preName + realName;
        } 
        else if (mJavaNameType == JAVA_NAME_TYPE_SPECIAL)
        {
            return "m_" + javaName;
        } 
        else
        {
            return javaName;
        }
    }

    public String getJavaMemberName(String rpcName)
    {
        if (mJavaNameType == JAVA_NAME_TYPE_NORMAL)
        {
            String preName = rpcName.substring(2, 3);
            preName = preName.toUpperCase();
            String realName = rpcName.substring(3);
            return "m" + preName + realName;
        } 
        else if (mJavaNameType == JAVA_NAME_TYPE_SPECIAL)
        {
            return rpcName.substring(2);
        } 
        else
        {
            return rpcName;
        }
    }

    private MetaTable()
    {
        mJava2Rpc = new HashMap<String, String>();
        mRpc2Java = new HashMap<String, String>();
        mJavaNameType = MetaTable.JAVA_NAME_TYPE_NORMAL;
    }

    private static class SingletonHolder
    {
        static MetaTable instance = new MetaTable();
    }
}
