package com.LangStack;

import com.LangStack.Rpc.RpcCore;
import com.LangStack.Transport.JniSession;
import com.LangStack.Transport.TcpSession;

public class LangStack
{
    /**
     * @brief       启动Jni传输模式
     */
    public static void startJniTransMode()
    {
        startJniMode();
        RpcCore.instance().bindSession(new JniSession());
    }
    
    /**
     * @brief       启动Tcp传输模式
     * @param       port        端口
     */
    public static void startTcpTransMode(int port)
    {
        startTcpMode(port);
        RpcCore.instance().bindSession(new TcpSession(port));
    }

    private native static void startJniMode();
    private native static void startTcpMode(int port);
    
    static {
    	System.loadLibrary("LangStack");
    }
}
