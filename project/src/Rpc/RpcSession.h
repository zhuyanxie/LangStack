/******************************************************************************
Copyright (C) 2015 zhuyanxie

　　Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished
to do so, subject to the following conditions:

　　The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#ifndef _LANGUAGE_STACK_RPC_SESSION_H_
#define _LANGUAGE_STACK_RPC_SESSION_H_

#include <memory>

#include "Net/Socket.h"
#include "Net/TcpBuffer.h"
#include "Net/Reactor.h"

#include "RpcCall.h"
#include "RpcMessage.h"

namespace ls {

enum RpcSessionState
{
    RpcSessionStateConnecting,                  ///< 连接中
    RpcSessionStateConnectFail,                 ///< 连接失败
    RpcSessionStateConnected,                   ///< 连接成功
    RpcSessionStateDisconnected,                ///< 断开连接
    RpcSessionStateUnvalid,                     ///< 非法状态
};

class CRpcSession
{
public:
    ///\brief       构造主动连接的session
    ///\param[in]   reactor     事件反应堆
    ///\param[in]   ip          目标IP
    ///\param[in]   port        目标端口
    CRpcSession(ReactorPtr reactor, const std::string ip, uint16_t port);

    ///\brief       从已有连接构造session
    ///\param[in]   fd          已连接的fd字
    CRpcSession(ReactorPtr reactor, int fd, const Address &addr);
    ~CRpcSession();

    ///\brief       fd消息通知
    ///\return      是否需要继续关注fd事件
    bool onRead(int fd);

    ///\brief       检测是否已经连接
    void heartbeat(uint64_t now);

    ///\brief       发送RPC请求
    ///\param[in]   type        RPC消息类型定义
    ///\return      true/false协议栈缓存不足
    bool send(RpcCallPtr call, RpcType type = rpcTypeCall);

    ///\brief       获取session状态
    inline RpcSessionState getState()
    {
        return m_state;
    }
    inline void setState(RpcSessionState state)
    {
        m_state = state;
    }

    ///\brief       ID get/set
    uint32_t getSessionId()
    {
        return m_id;
    }
    void setSessionId(uint32_t id)
    {
        m_id = id;
    }

    ///\brief       获取远程地址
    Address getRemoteAddress()
    {
        return m_rAddress;
    }

private:
    ///\brief       连接检测
    void checkConnect(uint64_t now);

    ///\brief       消息完整性校验
    bool checkMessageValid();

    ///\brief       消息处理
    void dealMessage();

private:
    Address                         m_rAddress; ///< 远程地址
    ReactorPtr                      m_reactor;  ///< reactor
    SocketPtr                       m_socket;   ///< socket对象
    RpcSessionState                 m_state;    ///< session对象
    int64_t                         m_time;     ///< 状态切换时间记录
    CTcpBuffer                      m_buf;      ///< Tcp缓存
    uint32_t                        m_id;       ///< session id

private:
    static uint32_t createSessionId();
    static std::atomic<uint32_t> s_sessionId;
};

typedef std::shared_ptr<CRpcSession>     RpcSessionPtr;

}

#endif /* _LANGUAGE_STACK_RPC_SESSION_H_ */
