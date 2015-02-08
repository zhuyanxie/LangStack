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

#ifndef _LANGUAGE_STACK_RPC_REQUEST_H_
#define _LANGUAGE_STACK_RPC_REQUEST_H_

#include <mutex>
#include <memory>
#include <condition_variable>

#include "Transport/TransSession.h"
#include "RpcCall.h"

namespace ls {

class CRpcRequest
{
public:
    CRpcRequest(RpcCallPtr call, SessionPtr session);
    ~CRpcRequest();

    ///\brief   阻塞带超时调用RpcRequest
    bool call(uint32_t timeout);

    ///\brief   接收到return
    void onReturn(RpcCallPtr call);

    ///\brief   获取本次请求ID，即callId
    int32_t getRequestId();

    ///\brief   获取返回值
    RpcCallPtr getCallReturn();

private:
    RpcCallPtr                      m_call;         ///< 调用
    RpcCallPtr                      m_return;       ///< 返回
    SessionPtr                      m_session;      ///< 会话
    std::condition_variable         m_cond;         ///< 条件变量
    std::mutex                      m_lock;         ///< 信号锁
};

typedef std::shared_ptr<CRpcRequest>     RpcRequestPtr;

}


#endif /* _LANGUAGE_STACK_RPC_REQUEST_H_ */
