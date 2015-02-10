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

#include "Transport/TcpSession.h"

#include <iostream>
#include "Base/MemoryStream.h"
#include "Rpc/RpcTask.h"
#include "Rpc/RpcCore.h"
#include "TcpMessage.h"

namespace ls {

CTcpSession::CTcpSession(int fd)
    : m_fd(fd)
    , m_buf(1*1024*1024)
    , m_loop(true)
    , m_thread(std::bind(&CTcpSession::readTcpMessage, this))
{
}

CTcpSession::~CTcpSession()
{
    m_loop = false;
    m_thread.join();
}

bool CTcpSession::send(RpcCallPtr call, RpcType type)
{
    char sendBuffer[64 * 1024];
    MemoryOutputStream mos(
            sendBuffer + sizeof(RpcHeader), 64 * 1024 - sizeof(RpcHeader));

    ::new((void *)sendBuffer) RpcHeader(RpcHeader(type, 0));
    Serializion<RpcCallPtr>()(call, mos);
    RpcHeader *header = (RpcHeader *)sendBuffer;
    header->setLength((uint16_t)mos.charsWritten());

    /// 本次未发送完,环回不做处理
    uint32_t len = uint32_t(sizeof(RpcHeader)+mos.charsWritten());
    size_t res = ::send(m_fd, sendBuffer, len, 0);
    std::cout << "send :" << res << " fd : " << m_fd << std::endl;

    return true;
}

///\brief       阻塞读取tcp消息
void CTcpSession::readTcpMessage()
{
    while (m_loop)
    {
        long len = (long)::recv(m_fd, m_buf.getBufferEnd(), m_buf.getIdleSize(), 0);
        if (len == 0 || (len < 0 && !IS_RECV_IGNORABLE(GET_NET_ERROR())))
        {
            break;
        }
        printf("do read : %ld\n", len);
        m_buf.addSize((uint32_t)len);

        dealMessage();
    }
}

///\brief       消息完整性校验
bool CTcpSession::checkMessageValid()
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
void CTcpSession::dealMessage()
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
        if (ls::deserial(buf.c_str(), call))
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
                CRpcCore::instance()->onRpcReturn(rpcCall);
            }
        }
        m_buf.popBuffer(length + RPC_HEADER_LENGTH);
    }
}

}




