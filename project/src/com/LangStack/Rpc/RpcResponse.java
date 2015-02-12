package com.LangStack.Rpc;

import com.LangStack.Transport.TcpMessage;
import com.LangStack.Transport.TransSession;

public class RpcResponse
{
    private RpcCall      mCall    = null; ///< 待发送的响应
    private TransSession mSession = null; ///< 发送响应的session

    /**
     * @brief   创建RPC响应
     * @param   call        接收到的调用
     * @param   session     发送的会话
     */
    public RpcResponse(RpcCall call, TransSession session)
    {
        mCall = new RpcCall(call.mCallId);
        mSession = session;
    }

    /**
     * @brief   获取响应
     * @return  响应
     */
    public RpcCall getReturn()
    {
        return mCall;
    }

    /**
     * @brief   发送响应
     */
    public void sendResponse()
    {
        mSession.send(mCall, TcpMessage.rpcTypeReturn);
    }
}
