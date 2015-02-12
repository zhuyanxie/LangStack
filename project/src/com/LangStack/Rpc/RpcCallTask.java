package com.LangStack.Rpc;

import com.LangStack.Task.ITask;

public class RpcCallTask implements ITask
{
    private RpcCall mCall = null;
    private String mId = null;

    public RpcCallTask(RpcCall call)
    {
        mCall = call;
        mId = "call : " + call.getCallId();
    }
    
    @Override public void run()
    {
        RpcCore.instance().onRpcCall(mCall);
    }

    @Override public String getTaskId()
    {
        return mId;
    }

}
