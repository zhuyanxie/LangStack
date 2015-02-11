package com.LangStack.Transport;

import java.util.concurrent.atomic.AtomicInteger;

import com.LangStack.Rpc.RpcCall;

public abstract class TransSession
{
    private int                  mId = 0;
    
    private static AtomicInteger sId = new AtomicInteger();

    public TransSession()
    {
        mId = createSessionId();
    }
    
    /**
     * @brief       发送RPC调用
     * @param       call        RPC调用
     * @param       type        调用类型
     * @return
     */
    public abstract boolean send(RpcCall call, int type);
    
    /**
     * @brief       创建sessionId
     * @return      独立的sessionId
     */
    private int createSessionId()
    {
        return sId.addAndGet(1);
    }
    
    public int getId()
    {
        return mId;
    }

    public void setId(int id)
    {
        this.mId = id;
    }
}

