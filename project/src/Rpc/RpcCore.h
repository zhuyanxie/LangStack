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

#include "Transport/TransSession.h"
#include "Transport/TcpServer.h"

#include "RpcCall.h"
#include "RpcRequest.h"
#include "RpcResponse.h"
#include "RpcObjectPool.h"

namespace ls {

typedef std::map<int32_t, RpcRequestPtr>    RequestPool;

class CRpcCore
{
public:
    static CRpcCore* instance();

    ///\brief           绑定新的Rpc session
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
    ///\brief           调用
    bool call(const RpcCallPtr &call, RpcCallPtr &retCall, uint32_t timeout);

    ///\brief           记录一个请求
    void insertRequest(const RpcRequestPtr &request);

    ///\brief           删除一个请求
    void deleteRequest(int32_t requestId);

    ///\brief           获取一个请求
    RpcRequestPtr getRequest(int32_t requestId);

    ///\brief           接收到远程RPC调用，调用本地方法/函数
    bool onRpcCall(const MetaMethodData &metaMethod, RpcCallPtr &call,
            CRpcResponse &resp);

    ///\brief           处理对象
    ///\param[in]       call        RPC调用对象
    void createObject(RpcCallPtr &call, CRpcResponse &resp);

    ///\brief           处理回调对象
    ///\param[in]       call        RPC调用对象
    void attachObject(RpcCallPtr &call, CRpcResponse &resp);

private:
    CRpcCore();
    ~CRpcCore();

private:
    CRpcObjectPool                          m_objects;      ///< ROC对象池
    SessionPtr                              m_session;      ///< RPC会话

    std::mutex                              m_lock;         ///< 请求锁
    RequestPool                             m_requests;     ///< 当前请求池
};

}

#endif /* _LANGUAGE_STACK_RPC_CORE_H_ */
