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

#include "TcpServer.h"

#include "Base/SocketHeader.h"
#include "Rpc/RpcCore.h"
#include "TcpSession.h"

namespace ls {

CTcpServer::CTcpServer(uint16_t port)
    : m_fd(-1)
    , m_loop(true)
    , m_thread(nullptr)
{
    struct sockaddr_in addr;
    addr.sin_len = sizeof(struct sockaddr_in);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bzero(&(addr.sin_zero),8);
    //创建socket
    m_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_fd != -1)
    {
        if (bind(m_fd, (struct sockaddr *)&addr, sizeof(addr)) != -1)
        {
            if (listen(m_fd, 32) != -1)
            {
                m_thread = new std::thread(std::bind(
                        &CTcpServer::doAccept, this));
            }
        }
    }
}

CTcpServer::~CTcpServer()
{
    ::close(m_fd);

    if (nullptr != m_thread)
    {
        m_loop = false;
        m_thread->join();
    }
}

void CTcpServer::doAccept()
{
    while (m_loop)
    {
        struct sockaddr_in addr;
        socklen_t len;
        int fd = accept(m_fd, (struct sockaddr *)&addr, &len);
        if (fd == -1)
        {
            continue;
        }

        CRpcCore::instance()->bindSession(SessionPtr(new CTcpSession(fd)));
    }
}



}


