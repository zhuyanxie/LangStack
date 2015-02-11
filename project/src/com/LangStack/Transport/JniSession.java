package com.LangStack.Transport;

import com.LangStack.Rpc.RpcCall;

public class JniSession extends TransSession
{
    @Override public boolean send(RpcCall call, int type)
    {
        // TODO Auto-generated method stub
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
