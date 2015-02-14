package com.LangStack.Transport;

import com.LangStack.Rpc.RpcCall;
import com.LangStack.Rpc.RpcCallTask;
import com.LangStack.Rpc.RpcCore;
import com.LangStack.Task.TaskThreadPool;

public class JniSession extends TransSession
{
    @Override public boolean send(RpcCall call, int type)
    {
        String str = call.serial();
        if (type == TcpMessage.rpcTypeCall)
        {
            sendCall2Cpp(str);
        }
        else
        {
            sendReturn2Cpp(str);
        }
        return true;
    }

    public static void onCppCall(String str)
    {
    	RpcCall call = RpcCall.deserial(str);
    	if (call != null)
    	{
    		TaskThreadPool.instance().addTask(new RpcCallTask(call));
    	}
    }
    
    public static void onCppReturn(String str)
    {
    	RpcCall call = RpcCall.deserial(str);
    	if (call != null)
    	{
    		RpcCore.instance().onRpcReturn(call);;
    	}  
    }
    
    private native static void sendCall2Cpp(String str);
    private native static void sendReturn2Cpp(String str);
}
