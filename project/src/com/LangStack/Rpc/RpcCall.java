package com.LangStack.Rpc;

import java.util.ArrayList;

import com.LangStack.Serial.Deserial;
import com.LangStack.Serial.Serial;

public class RpcCall
{
    ///< 远程对象方法
    private String            mMethod    = new String();
    ///< 类名
    private String            mClassName = new String();
    ///< callID标记本次call
    private int               mCallId    = 0;
    ///< 远程对象句柄,句柄为空调用函数
    private long              mObject    = 0;
    ///< 入参、出参(return)列表
    private ArrayList<String> mValues    = new ArrayList<String>();

    ///< callId生成器
    private static int       id         = 0;       
       
    /**
     * @brief   默认构造
     */
    RpcCall()
    {        
    }

    /**
     * @brief   响应构造
     * @param callId    请求的callId
     */
    public RpcCall(int callId)
    {
        mCallId = callId;
    }
    
    /**
     * @brief   请求构造
     * @param method    远程调用的方法
     * @param cls       远程调用的类
     * @param obj       远程对象地址
     */
    public RpcCall(String method, String cls, long obj)
    {
        mMethod = method;
        mClassName  = cls;
        mObject = obj;
        mCallId = createCallId();
    }
    
    /**
     * @brief   增加远程调用参数
     * @param param     参数序列化后的值
     */
    public void pushParam(String param)
    {
        mValues.add(param);
    }
    
    /**
     * @brief   生成callId
     * @return  callId
     */
    private synchronized static int createCallId()
    {
        while (id++ == -1);
        return id;
    }
    
    /**
     * 序列化RPCCall
     * @return 序列化后的字符串
     */
    public String serial()
    {
        StringBuffer sb = new StringBuffer("RpcCall:");
        sb.append(Serial.serial(mMethod));
        sb.append(Serial.serial(mClassName));
        sb.append(Serial.serial(mCallId));
        sb.append(Serial.serial(mObject));
        for (String s : mValues)
        {
            sb.append(s);
        }
        sb.append(Serial.DETAIL_END);
        
        return sb.toString();
    }
    
    /**
     * @return  java中的参数的类型定义
     */
    public Class<?>[] getValueClassTypes()
    {
        Class<?>[] ca = new Class<?>[mValues.size()];
        for (int i = 0; i < mValues.size(); ++i)
        {
            ca[i] = Deserial.getType(mValues.get(i));
        }
        return ca;
    }
    
    /**
     * @brief   获取参数值
     * @return  值数组
     */
    public Object[] getValueObject()
    {
        Object[] oa = new Object[mValues.size()];
        for (int i = 0; i < mValues.size(); ++i)
        {
            oa[i] = Deserial.deserial(mValues.get(i));
        }
        return oa;
    }
    
    /**
     * 反序列化RPCCall
     * @param buf 输入的缓存
     * @return
     */
    public static RpcCall deserial(String str)
    {
        if (!str.startsWith("RpcCall:"))
        {
            return null;
        }
        
        RpcCall call = new RpcCall();
        str = str.substring("RpcCall:".length(), str.length());
        
        /// method
        int next = Deserial.getDetailLength(str);
        Object value = Deserial.deserial(str.substring(0, next + 1));
        call.mMethod = (String)value;
        str = str.substring(next + 1, str.length());  
        
        /// class
        next = Deserial.getDetailLength(str);
        value = Deserial.deserial(str.substring(0, next + 1));
        call.mClassName = (String)value;
        str = str.substring(next + 1, str.length());        
        
        /// callId
        next = Deserial.getDetailLength(str);
        value = Deserial.deserial(str.substring(0, next + 1));
        call.mCallId = (Integer)value;
        str = str.substring(next + 1, str.length());     
        
        /// object
        next = Deserial.getDetailLength(str);
        value = Deserial.deserial(str.substring(0, next + 1));
        call.mObject = (Long)value;
        str = str.substring(next + 1, str.length() - 1);    
        
        /// value
        while (str.length() > 0)
        {
            next = Deserial.getDetailLength(str);
            value = Deserial.deserial(str.substring(0, next + 1));
            call.mValues.add((String)str.substring(0, next + 1));
            str = str.substring(next + 1, str.length());
        } ;

        return call;
    }
    
    public String getMethod()
    {
        return mMethod;
    }

    public void setMethod(String mMethod)
    {
        this.mMethod = mMethod;
    }

    public String getClassName()
    {
        return mClassName;
    }

    public void setClassName(String mClass)
    {
        this.mClassName = mClass;
    }

    public int getCallId()
    {
        return mCallId;
    }

    public void setCallId(int mCallId)
    {
        this.mCallId = mCallId;
    }

    public long getObject()
    {
        return mObject;
    }

    public void setObject(long mObject)
    {
        this.mObject = mObject;
    }

    public ArrayList<String> getValues()
    {
        return mValues;
    }

    public void setValues(ArrayList<String> mValues)
    {
        this.mValues = mValues;
    }
}
