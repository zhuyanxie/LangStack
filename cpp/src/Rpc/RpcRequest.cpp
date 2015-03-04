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


#include "RpcRequest.h"

#include "LangStackConstant.h"
#include "Log/Log.h"

namespace ls {

CRpcRequest::CRpcRequest(RpcCallPtr call, SessionPtr session)
    : m_call(call)
    , m_session(session)
{
}

CRpcRequest::~CRpcRequest()
{
}

///\brief   阻塞带超时调用RpcRequest
bool CRpcRequest::call(uint32_t timeout)
{
    if (!m_session->send(m_call, rpcTypeCall))
    {
        return false;
    }
    if (!m_return)
    {
        std::unique_lock<std::mutex> lck(m_lock);
        m_cond.wait(lck);
    }
    return !!m_return;
}

///\brief   接收到return
void CRpcRequest::onReturn(RpcCallPtr call)
{
    m_return = call;
    std::unique_lock<std::mutex> lck(m_lock);
    m_cond.notify_one();
}

///\brief   获取sessionIed
int32_t CRpcRequest::getRequestId()
{
    return m_call->m_callId;
}

///\brief   获取返回值
RpcCallPtr CRpcRequest::getCallReturn()
{
    return m_return;
}

}
