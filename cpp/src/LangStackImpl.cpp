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

#include "LangStackImpl.h"

#include <mutex>
#include "Rpc/RpcCore.h"
#include "Transport/JniSession.h"

namespace ls
{

static CLangStackImpl*  s_langStackInstance = nullptr;
static std::mutex       s_lock;

CLangStackImpl* CLangStackImpl::instance()
{
    if (nullptr == s_langStackInstance)
    {
        std::lock_guard<std::mutex> g(s_lock);
        if (nullptr == s_langStackInstance)
        {
            s_langStackInstance = new CLangStackImpl();
        }
    }

    return s_langStackInstance;
}

///\brief       启动协议栈,Jni传输
void CLangStackImpl::startJniMode()
{
    CRpcCore::instance()->bindSession(SessionPtr(new CJniSession()));
}

///\brief       启动协议栈,Tcp环回传输
void CLangStackImpl::startTcpMode(uint16_t port)
{
    m_server = TcpServerPtr(new CTcpServer(port));
}

CLangStackImpl::CLangStackImpl()
{
}

CLangStackImpl::~CLangStackImpl()
{
}

} /* namespace ls */
