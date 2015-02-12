package com.LangStack.Rpc;

import com.LangStack.MetaTable;
import com.LangStack.Serial.Deserial;
import com.LangStack.Serial.Serial;

public class IRpcApi
{    
    /// RPC调用超时时间
    public static final int          RPC_CALL_TIMEOUT    = 3000;
    /// 远程调用对象ID
    private long                     mRpcObjectId        = 0;      
    /// 静态ID生成器
    private static long              sRpcObjectIdCreator = 0;                          

    public IRpcApi()
    {
        mRpcObjectId = createId();
    }
    
    /**
     * @brief       JAVA对象不能直接转换为long，因此增加id
     * @return      当前对象的唯一ID
     */
    public long toId()
    {
        return mRpcObjectId;
    }
    
    /**
     * @brief       注册回调
     * @param       o               回调对象
     */
    public void attach(IRpcApi o)
    {
    }
    
    /**
     * @brief       注销回调
     * @param       o               回调对象
     */
    public void detach(IRpcApi o)
    {
    }
    
    /**
     * @brief       调用远程对象
     * @param       method          方法
     * @param       className       类名
     * @param       rpcObject       远程调用对象
     * @return      返回值
     */
    public static Object call(String method, String className, IRpcApi rpcObject, 
            Object... os)
    {
        String cls = MetaTable.instance().getRpcClassName(className);
        RpcCall rpcCall = rpcObject != null ? 
                new RpcCall(method, cls, rpcObject.toId()) :
                new RpcCall(method, cls, 0);
                
        for (Object o : os)
        {
            rpcCall.pushParam(Serial.serial(o));
        }
        
        RpcCall rpcReturn = RpcCore.instance().call(
                rpcCall, RPC_CALL_TIMEOUT, rpcObject);
        
        Object  res = null;
        if (rpcReturn != null && !rpcReturn.mValues.isEmpty())
        {
            return Deserial.deserial(rpcReturn.mValues.get(0));
        }
        return res;
    }
    
    /**
     * @brief       调用远程对象
     * @param       method          方法
     * @param       className       类名
     * @param       rpcObject       远程调用对象
     * @return      返回值
     */
    public static void callbackSetting(String method, String className, 
            IRpcApi rpcObject, IRpcApi callbackObject)
    {
        String cls = MetaTable.instance().getRpcClassName(className);
        RpcCall rpcCall = rpcObject != null ? 
                new RpcCall(method, cls, rpcObject.toId()) :
                new RpcCall(method, cls, 0);
                
        rpcCall.pushParam(Serial.serial(callbackObject.toId()));
        RpcCore.instance().call(rpcCall, RPC_CALL_TIMEOUT, callbackObject);
    }
    
    
    /**
     * @return      生成的全局唯一对象ID
     */
    private synchronized static long createId()
    {
        return sRpcObjectIdCreator++;
    }
    
}
