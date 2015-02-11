package com.LangStack;

import com.LangStack.Rpc.RpcCore;

public class LangStack
{
    /**
     * @brief       启动Jni传输模式
     * @param       port        端口
     */
    public static void startJniTransMode(int transPort, int port)
    {
        startJniMode();
    }
    
    /**
     * @brief       启动Tcp传输模式
     * @param       port        端口
     */
    public static void startTcpTransMode(int port)
    {
        startTcpMode(port);
    }

    private native static void startJniMode();
    private native static void startTcpMode(int port);
}
