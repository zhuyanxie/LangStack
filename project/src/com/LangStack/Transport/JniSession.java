package com.LangStack.Transport;

import com.LangStack.Rpc.RpcCall;

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
        
    }
    
    public static void onCppReturn(String str)
    {
        
    }
    
    private native static void sendCall2Cpp(String str);
    private native static void sendReturn2Cpp(String str);
}