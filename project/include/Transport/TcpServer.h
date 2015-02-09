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

#ifndef _LANGUAGE_STACK_TCP_SERVER_H_
#define _LANGUAGE_STACK_TCP_SERVER_H_

#include <inttypes.h>
#include <thread>
#include <memory>

namespace ls {

///\brief   tcp server 单独起线程监听
class CTcpServer
{
public:
    CTcpServer(uint16_t port);
    ~CTcpServer();

private:
    ///\brief       监听接收函数
    void doAccept();

private:
    int             m_fd;       ///< 监听的服务器FD
    bool            m_loop;     ///< 线程循环控制
    std::thread     *m_thread;  ///< 独立的接收线程
};

typedef std::unique_ptr<CTcpServer>     TcpServerPtr;

}


#endif /* _LANGUAGE_STACK_TCP_SERVER_H_ */
