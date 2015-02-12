package com.LangStack.Rpc;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import com.LangStack.Serial.Deserial;
import com.LangStack.Serial.Serial;
import com.LangStack.Transport.TransSession;

public class RpcCore
{
    private static RpcCore           sCore        = null;
    private TransSession             mSession     = null;
    private Map<Integer, RpcRequest> mRequestPool = null;
    private Lock                     mLock        = null;
    private RpcObjectPool            mObjects     = null;

    /**
     * @return 单例
     */
    public synchronized static RpcCore instance()
    {
        if (sCore == null)
        {
            sCore = new RpcCore();
        }
        return sCore;
    }

    /**
     * @brief   绑定传输会话
     * @param   session     传输会话
     * @return  true/false是否已经启动
     */
    public synchronized boolean bindSession(TransSession session)
    {
        mSession = session;
        return true;
    }

    /**
     * @brief   调用一个RPC调用
     * @param   rpcCall         rpc调用
     * @param   timeout         超时时间
     * @param   o               本地调用对象（attach/detach用）
     * @return  rpc调用返回值
     */
    RpcCall call(RpcCall call, int timeout, Object o)
    {
        /// FIXME 多个session的情况
        RpcRequest req = new RpcRequest(call, mSession);
        insertRequest(req);
        if (!req.call(timeout))
        {
            deleteRequest(req);
            return null;
        }

        RpcCall retCall = req.getCallReturn();
        if (call.mMethod.equals("attach"))
        {
            mObjects.addObject(
                    (Long)Deserial.deserial(retCall.mValues.get(0)), o);
            
            System.out.println("attach : " + (Long)Deserial.deserial(retCall.mValues.get(0)));
        }
        else if (call.mMethod.equals("detach"))
        {
            mObjects.delObject(
                    (Long)Deserial.deserial(retCall.mValues.get(0)));
            
            System.out.println("detach : " + (Long)Deserial.deserial(retCall.mValues.get(0)));
        }

        deleteRequest(req);
        return retCall;
    }

    /**
     * @brief   删除一个请求
     * @param   req   请求
     */
    private void deleteRequest(RpcRequest req)
    {
        mLock.lock();
        mRequestPool.remove(req.getCallId());
        mLock.unlock();
    }


    /**
     * @brief   接收到远程RPC调用
     * @param   call            rpc调用
     */
    public void onRpcCall(RpcCall call)
    {
        RpcResponse resp = new RpcResponse(call, mSession);
        do {
            /// 对象创建销毁特殊处理
            if (call.mMethod.equals("new") || 
                    call.mMethod.equals("delete"))
            {
                createObject(call, resp);
                break;
            }
            
            /// 回调特殊处理
            if (call.mMethod.equals("attach") ||
                    call.mMethod.equals("detach"))
            {
                /// TODO c++一般不会需要java回调
                break;
            }
            
            /// 获取类
            Class<?> c = null;
            try {
                c = Class.forName(call.mClass);
            } catch (ClassNotFoundException e) {
                e.printStackTrace();
                break;
            }
            
            /// 执行方法
            if (!onRpcCall(c, call, resp))
            {
                /// TODO set error code
            }
            
        } while (false);
        
        resp.sendResponse();
    }
    
    /**
     * @brief   执行RPC方法
     * @param   c           java反射类
     * @param   call        RPC调用对象
     * @param   resp        RPC调用返回
     * @return  true/falseRPC调用的非本方法
     */
    private boolean onRpcCall(Class<?> c, RpcCall call, RpcResponse resp)
    {
        try
        {
            Method method = c.getDeclaredMethod(call.mMethod, 
                    call.getValueClassTypes());
            Object o = mObjects.getObject(call.mObject);
            
            try
            {
                Object res = method.invoke(o, call.getValueObject());
                if (method.getReturnType() != void.class)
                {
                    resp.getReturn().pushParam(Serial.serial(res));
                }
                return true;
            } catch (IllegalAccessException e) {
                e.printStackTrace();
            } catch (IllegalArgumentException e) {
                e.printStackTrace();
            } catch (InvocationTargetException e) {
                e.printStackTrace();
            }
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (SecurityException e) {
            e.printStackTrace();
        }
        return false;
    }

    /**
     * @brief   创建或销毁本地对象
     * @param       call    远程调用
     * @param       resp    响应
     */
    private void createObject(RpcCall call, RpcResponse resp)
    {
        long id = call.mObject;
        Object o = null;
        
        if (call.mMethod.equals("new"))
        {
            try {
                o = Class.forName(call.mClass).newInstance();
                mObjects.addObject(id, o);
            } catch (InstantiationException e1) {
                e1.printStackTrace();
            } catch (IllegalAccessException e1) {
                e1.printStackTrace();
            } catch (ClassNotFoundException e1) {
                e1.printStackTrace();
            }
        }
        else
        {
            mObjects.delObject(id);
        }
    }

    /**
     * @brief   接收到远程RPC返回
     * @param   rpcCall         rpc调用
     */
    public void onRpcReturn(RpcCall retCall)
    {
        RpcRequest req = getRequest(retCall.mCallId);
        if (req != null)
        {
            req.onReturn(retCall);
        }
    }
    
    /**
     * 构造
     */
    private RpcCore()
    {
        mRequestPool    = new HashMap<Integer, RpcRequest>();
        mLock           = new ReentrantLock();
        mObjects        = new RpcObjectPool();
    }

    /**
     * @brief 记录一个请求
     * @param req   请求
     */
    private void insertRequest(RpcRequest req)
    {
        mLock.lock();
        if (mRequestPool.containsKey(req.getCallId()))
        {
            mRequestPool.remove(req.getCallId());
        }
        mRequestPool.put(req.getCallId(), req);
        mLock.unlock();
    }
    
    /**
     * @brief 获取一个请求
     * @param callId    请求的唯一ID
     * @return
     */
    private RpcRequest getRequest(int callId)
    {
        RpcRequest req = null;
        mLock.lock();
        if (mRequestPool.containsKey(callId))
        {
            req = mRequestPool.get(callId);
        }
        mLock.unlock();
        return req;
    }
}
