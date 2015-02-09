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

#include "RpcSession.h"

#include <atomic>
#include <chrono>
#include "base/MemoryStream.h"

#include "RpcTask.h"
#include "RpcConstant.h"

#include <iostream>

using namespace std::chrono;

namespace ls {

std::atomic<uint32_t> CRpcSession::s_sessionId(0);
uint32_t CRpcSession::createSessionId()
{
    return ++CRpcSession::s_sessionId;
}

///\brief       构造主动连接的session
CRpcSession::CRpcSession(ReactorPtr reactor, const std::string ip, uint16_t port)
    : m_rAddress(ip.c_str(), port)
    , m_reactor(reactor)
    , m_socket(SocketPtr(new Socket(Sock_ipv4, SOCKET_TYPE_STREAM)))
    , m_state(RpcSessionStateConnecting)
    , m_time(duration_cast<milliseconds>(
            steady_clock::now().time_since_epoch()).count())
    , m_buf(1*1024*1024)
    , m_id(createSessionId())
{
    m_socket->setOption(RECVBUFSIZE, 512*1024);
    m_socket->setOption(SENDBUFSIZE, 512*1024);
    m_socket->connect(m_rAddress, 0);

    m_reactor->registFd(m_socket->handle(),
        std::bind(&CRpcSession::onRead, this, std::placeholders::_1));
}

///\brief       从已有连接构造session
CRpcSession::CRpcSession(ReactorPtr reactor, int fd, const Address &addr)
    : m_rAddress(addr)
    , m_reactor(reactor)
    , m_socket(SocketPtr(new Socket(fd, true)))
    , m_state(RpcSessionStateConnected)
    , m_time(duration_cast<milliseconds>(
            steady_clock::now().time_since_epoch()).count())
    , m_buf(1*1024*1024)
    , m_id(createSessionId())
{
    m_socket->setOption(RECVBUFSIZE, 512*1024);
    m_socket->setOption(SENDBUFSIZE, 512*1024);
    m_socket->setConnected(true);

    m_reactor->registFd(m_socket->handle(),
            std::bind(&CRpcSession::onRead, this, std::placeholders::_1));
}

CRpcSession::~CRpcSession()
{
    m_reactor->unregistFd(m_socket->handle());
    m_socket->close();
}

///\brief       RPC消息通知
bool CRpcSession::onRead(int fd)
{
    while (true)
    {
        int len = m_socket->recv(m_buf.getBufferEnd(), m_buf.getIdleSize(), 0);
        if (len < 0)
        {
//            WARNF("read recv [%d] disconnected !\n", len);
            setState(RpcSessionStateDisconnected);
            return false;
        }
        if (len == 0)
        {
            return true;
        }
        printf("do read : %d\n", len);
        m_buf.addSize(len);

        dealMessage();
    }

    return false;
}

///\brief       检测是否已经连接
void CRpcSession::heartbeat(uint64_t now)
{
    if (getState() == RpcSessionStateConnecting)
    {
        return checkConnect(now);
    }

    /// TODO
}

///\brief       发送RPC请求
bool CRpcSession::send(RpcCallPtr call, RpcType type)
{
    char sendBuffer[64 * 1024];
    MemoryOutputStream mos(
            sendBuffer + sizeof(RpcHeader), 64 * 1024 - sizeof(RpcHeader));

    ::new((void *)sendBuffer) RpcHeader(RpcHeader(type, 0));
    Serializion<RpcCallPtr>()(call, mos);
    RpcHeader *header = (RpcHeader *)sendBuffer;
    header->setLength((uint16_t)mos.charsWritten());

    /// TODO 本次未发送完
    uint32_t len = uint32_t(sizeof(RpcHeader)+mos.charsWritten());
    int res = m_socket->send(sendBuffer, len, 0);
    std::cout << "send :" << res << " fd : " << m_socket->handle() << std::endl;

    return true;
}

void CRpcSession::checkConnect(uint64_t now)
{
    struct timeval tv = { 0 };
    int ret = m_socket->poll(EV_WRITE_MASK, &tv);
    if (ret > 0)
    {
        m_socket->setConnected(true);
        setState(RpcSessionStateConnected);
    }
    if (now - m_time > 5000)
    {
        setState(RpcSessionStateDisconnected);
        return;
    }
}

///\brief       消息完整性校验
bool CRpcSession::checkMessageValid()
{
    if (m_buf.size() < RPC_HEADER_LENGTH)
    {
        return false;
    }

    uint32_t offset = 0;
    while (memcmp(m_buf.getBuffer() + offset, RPC_MESSAGE_TAG, 4) != 0)
    {
        ++offset;
        if (offset + 4 >= m_buf.size())
        {
            break;
        }
    }

    m_buf.popBuffer(offset);
    if (m_buf.size() < RPC_HEADER_LENGTH)
    {
        return false;
    }

    return true;
}

///\brief       消息处理
void CRpcSession::dealMessage()
{
    while (checkMessageValid())
    {
        RpcHeader *header = (RpcHeader *)m_buf.getBuffer();
        uint16_t length = header->getLength();
        RpcType type = (RpcType)header->getType();
        if (length + RPC_HEADER_LENGTH > m_buf.size())
        {
            break;
        }

        CRpcCall *call;
        std::string buf(m_buf.getBuffer() + RPC_HEADER_LENGTH, length);
        if (Event::deserial(buf.c_str(), call))
        {
            RpcCallPtr rpcCall(call);
            if (type == rpcTypeCall)
            {
                std::cout << buf << std::endl;
                CTaskThreadPool::instance()->addTask(
                        new CRpcCallTask(rpcCall, getSessionId()));
            }
            else
            {
                CRpcCore::instance()->onRpcReturn(rpcCall, getSessionId());
            }
        }
        m_buf.popBuffer(length + RPC_HEADER_LENGTH);
    }
}

}
