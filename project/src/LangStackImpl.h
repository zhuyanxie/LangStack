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

#ifndef _LANGUAGE_STACK_CLANGSTACK_IMPL_H_
#define _LANGUAGE_STACK_CLANGSTACK_IMPL_H_

#include "Transport/TcpServer.h"

namespace ls
{

class CLangStackImpl
{
public:
    ///\brief       单例
    static CLangStackImpl* instance();

    ///\brief       启动协议栈,Jni传输
    void startJniMode();

    ///\brief       启动协议栈,Tcp环回传输
    ///\param[in]   port        tcp端口
    void startTcpMode(uint16_t port);

private:
    TcpServerPtr            m_server;       ///< tcp服务

private:
    CLangStackImpl();
    ~CLangStackImpl();
};

} /* namespace ls */
#endif /* _LANGUAGE_STACK_CLANGSTACK_IMPL_H_ */
