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


#include "Rpc/RpcCore.h"

#include "RpcCoreImpl.h"

namespace ls
{

static CRpcCore* s_coreInstance = NULL;
static std::mutex s_lock;

CRpcCore* CRpcCore::instance()
{
    if (NULL == s_coreInstance)
    {
        std::lock_guard<std::mutex> g(s_lock);
        if (NULL == s_coreInstance)
        {
            s_coreInstance = new CRpcCore();
        }
    }

    return s_coreInstance;
}

///\brief           绑定新的session
bool CRpcCore::bindSession(const SessionPtr &session)
{
    return m_impl->bindSession(session);
}

///\brief           调用一个RPC调用
bool CRpcCore::call(RpcCallPtr call, RpcCallPtr &retCall, uint32_t timeout)
{
    return m_impl->call(call, retCall, timeout);
}

///\brief           接收到远程RPC调用
void CRpcCore::onRpcCall(RpcCallPtr rpcCall)
{
    return m_impl->onRpcCall(rpcCall);
}

///\brief           接收到远程RPC的回调
void CRpcCore::onRpcReturn(RpcCallPtr retCall)
{
    return m_impl->onRpcReturn(retCall);
}

CRpcCore::CRpcCore()
{
    m_impl = new CRpcCoreImpl();
}

CRpcCore::~CRpcCore()
{
    if (nullptr != m_impl)
    {
        delete m_impl;
        m_impl = nullptr;
    }
}



}
