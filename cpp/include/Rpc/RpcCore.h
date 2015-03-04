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

#ifndef _LANGUAGE_STACK_RPC_CORE_H_
#define _LANGUAGE_STACK_RPC_CORE_H_

#include <map>
#include <mutex>
#include <functional>

#include "Rpc/RpcCall.h"
#include "Transport/TransSession.h"
#include "Defs.h"

namespace ls {

class CRpcCoreImpl;
class LS_EXPORT CRpcCore
{
public:
    static CRpcCore* instance();

    ///\brief           绑定新的session
    ///\param[in]       session     Rpc会话
    ///\return          true
    bool bindSession(const SessionPtr &session);

    ///\brief           调用一个RPC调用
    ///\param[in]       rpcCall     rpc调用
    ///\param[out]      retCall     rpc调用返回值
    ///\param[in]       sessionId   rpc调用的会话,0则调用最早创建的RPCSession
    ///\param[in]       timeout     超时时间
    ///\return          true/false超时or调用失败
    bool call(RpcCallPtr call, RpcCallPtr &retCall, uint32_t timeout = 3000);

    ///\brief           接收到远程RPC调用
    ///\param[in]       rpcCall     rpc调用
    void onRpcCall(RpcCallPtr rpcCall);

    ///\brief           接收到远程RPC的回调
    ///\param[in]       retCall     rpc返回
    void onRpcReturn(RpcCallPtr retCall);

private:
    CRpcCoreImpl *m_impl;

private:
    CRpcCore();
    ~CRpcCore();
};

}

#endif /* _LANGUAGE_STACK_RPC_CORE_H_ */
