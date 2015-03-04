package com.LangStack.Rpc;

import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import com.LangStack.Transport.TcpMessage;
import com.LangStack.Transport.TransSession;

public class RpcRequest
{
    private RpcCall      mCall    = null; ///< 调用
    private RpcCall      mReturn  = null; ///< 返回
    private TransSession mSession = null; ///< 会话
    private Lock         mLock    = null; ///< 条件变量锁
    private Condition    mCon     = null; ///< 条件变量
    
    public RpcRequest(RpcCall call, TransSession session)
    {
        mCall = call;
        mSession = session;
        mLock = new ReentrantLock();
        mCon  = mLock.newCondition();
    }
    
    /**
     * @brief   阻塞带超时调用RpcRequest
     * @param   超时时间
     * @return  是否正常响应
     */
    public boolean call(int timeout)
    {
        if (!mSession.send(mCall, TcpMessage.rpcTypeCall))
        {
            return false;
        }
        
        try {
            mLock.lock();
            mCon.await();
        } catch (InterruptedException e) {
            e.printStackTrace();
        } finally {
            mLock.unlock();
        }
        return (null != mReturn);
    }
    
    public void onReturn(RpcCall call)
    {
        mReturn = call;

        try {
            mLock.lock();
            mCon.signal();
        } finally {
            mLock.unlock();
        }
    }

    public RpcCall getCallReturn()
    {
        return mReturn;
    }

    public Integer getCallId()
    {
        return mCall.getCallId();
    }
    
    
}
