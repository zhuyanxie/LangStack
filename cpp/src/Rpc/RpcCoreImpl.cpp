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

#include "RpcCoreImpl.h"

#include <mutex>
#include <iostream>

#include "Reflect/MetaDataTraits.h"
#include "Reflect/ReflectInvoke.h"
#include "Serial/Deserializion.h"
#include "Serial/DeserialType.h"
#include "LangStackConstant.h"
#include "Log/Log.h"

namespace ls {

CRpcCoreImpl::CRpcCoreImpl()
{
}

CRpcCoreImpl::~CRpcCoreImpl()
{
}


///\brief           绑定session
bool CRpcCoreImpl::bindSession(const SessionPtr &session)
{
    m_session = session;
    return true;
}

///\brief       调用
bool CRpcCoreImpl::call(RpcCallPtr call, RpcCallPtr &retCall, uint32_t timeout)
{
    if (!m_session)
    {
        return false;
    }

    RpcRequestPtr request(new CRpcRequest(call, m_session));
    insertRequest(request);

    if (!request->call(timeout))
    {
        deleteRequest(request->getRequestId());
        return false;
    }

    retCall = request->getCallReturn();
    if (call->m_method == "attach")
    {
        m_objects.addObject(
                Deserializion<long long>()(retCall->m_values[0].c_str()),
                (void*)Deserializion<long long>()(call->m_values[0].c_str()),
                false);
    }
    else if (call->m_method == "detach")
    {
        m_objects.delObject(Deserializion<long long>()(retCall->m_values[0].c_str()));
    }

    deleteRequest(request->getRequestId());
    return true;
}

///\brief           接收到远程RPC调用
void CRpcCoreImpl::onRpcCall(RpcCallPtr rpcCall)
{
    /// 响应
    CRpcResponse resp(rpcCall, m_session);

    /// 类
    CMetaTable *table = nullptr;
    CMetaFactory::instance()->getMetaTable(rpcCall->m_cls.c_str(), table);
    if (nullptr == table)
    {
        return;
    }

    /// 特殊操作对象处理
    if (rpcCall->m_method == "new" || rpcCall->m_method == "delete")
    {
        return createObject(rpcCall, resp);
    }

    /// 方法
    OverLoadMethods *methods = nullptr;
    table->getMethods(rpcCall->m_method.c_str(), methods);
    if (nullptr == methods)
    {
        return;
    }

    /// attach的话先改变对象
    if (rpcCall->m_method == "attach" || rpcCall->m_method == "detach")
    {
        attachObject(rpcCall, resp);
    }

    /// 调用
    rpcCall->m_object = (int64_t) m_objects.getObject(rpcCall->getObject());
    for (auto it = methods->begin(); it != methods->end(); ++it)
    {
        if (onRpcCall(*it, rpcCall, resp))
        {
            break;
        }
    }
}

///\brief           接收到远程RPC的回调
void CRpcCoreImpl::onRpcReturn(RpcCallPtr retCall)
{
    RpcRequestPtr request = getRequest(retCall->getCallId());
    if (request)
    {
        request->onReturn(retCall);
    }
}

///\brief       记录一个请求
void CRpcCoreImpl::insertRequest(const RpcRequestPtr &request)
{
    std::lock_guard<std::mutex> g(m_lock);
    m_requests[request->getRequestId()] = request;
}

///\brief       删除一个请求
void CRpcCoreImpl::deleteRequest(int32_t requestId)
{
    std::lock_guard<std::mutex> g(m_lock);
    m_requests.erase(requestId);
}

///\brief       获取一个请求
RpcRequestPtr CRpcCoreImpl::getRequest(int32_t requestId)
{
    std::lock_guard<std::mutex> g(m_lock);
    return m_requests.count(requestId) ? m_requests[requestId] : RpcRequestPtr();
}

///\brief       接收到远程RPC调用，调用本地方法/函数
bool CRpcCoreImpl::onRpcCall(const MetaMethodData &metaMethod, RpcCallPtr &call,
        CRpcResponse &resp)
{
    if (metaMethod.params.empty())
    {
        return false;
    }

    /// 重载校验
    auto it     = metaMethod.params.begin();
    auto itv    = call->m_values.begin();
    for (++it; it != metaMethod.params.end() && itv != call->m_values.end();
            ++it, ++itv)
    {
        if (*it != deserialMetaType((*itv).c_str()))
        {
            break;
        }
    }
    if (it != metaMethod.params.end() || itv != call->m_values.end())
    {
        return false;
    }

    /// 函数调用
    RpcCallPtr &returnCall = resp.getReturn();

    MetaDataType returnType = *metaMethod.params.begin();
    switch (returnType)
    {
    case MetaDataTypeVoid:
        ReflectMethodInvoker::invoke<void>(metaMethod, call);
        break;
    case MetaDataTypeInt:
        returnCall->pushParams(ReflectMethodInvoker::invoke<
                int>(metaMethod, call));
        break;
    case MetaDataTypeLonglong:
        returnCall->pushParams(ReflectMethodInvoker::invoke<
                long long>(metaMethod, call));
        break;
    case MetaDataTypeDouble:
        returnCall->pushParams(ReflectMethodInvoker::invoke<
                double>(metaMethod, call));
        break;
    case MetaDataTypeString:
        returnCall->pushParams(ReflectMethodInvoker::invoke<
                std::string>(metaMethod, call));
        break;
    case MetaDataTypeClass:
        returnCall->pushParams(ReflectMethodInvoker::invoke<
                IReflection*>(metaMethod, call));
        break;
    case MetaDataTypeIntList:
        returnCall->pushParams(ReflectMethodInvoker::invoke<
                std::list<int> >(metaMethod, call));
        break;
    case MetaDataTypeLonglongList:
        returnCall->pushParams(ReflectMethodInvoker::invoke<
                std::list<long long> >(metaMethod, call));
        break;
    case MetaDataTypeDoubleList:
        returnCall->pushParams(ReflectMethodInvoker::invoke<
                std::list<double> >(metaMethod, call));
        break;
    case MetaDataTypeStringList:
        returnCall->pushParams(ReflectMethodInvoker::invoke<
                std::list<std::string> >(metaMethod, call));
        break;
    case MetaDataTypeClassList:
        returnCall->pushParams(ReflectMethodInvoker::invoke<
                std::list<IReflection*> >(metaMethod, call));
        break;
    default:
        break;
    }

    return true;
}

///\brief           处理对象
void CRpcCoreImpl::createObject(RpcCallPtr &call, CRpcResponse &resp)
{
    int64_t id = call->getObject();
    void *object = nullptr;
    if (call->m_method == "new")
    {
        object = CMetaFactory::instance()->createClassByName(
                call->m_cls.c_str());

        if (nullptr != object)
        {
            m_objects.addObject(id, object);
        }
        else
        {
            /// FIXME error info;
        }

        DEBUGF(LS_TAG, "new rpc object, id[lld] local[%p]\n", id, object);
    }
    else
    {
        object = m_objects.getObject(id);
        if (nullptr != object)
        {
            delete (IReflection* )object;
            m_objects.delObject(id);
        }
        else
        {
            /// FIXME error info;
        }

        DEBUGF(LS_TAG, "del rpc object, id[lld] local[%p]\n", id, object);
    }
}

///\brief           处理回调对象
void CRpcCoreImpl::attachObject(RpcCallPtr &call, CRpcResponse &resp)
{
    int64_t id = Deserializion<long long>()(call->m_values[0].c_str());
    void *object = (IReflection*)m_objects.getObject(id);

    call->m_values.clear();
    call->pushParams<long long>((long long)(object));

    resp.getReturn()->pushParams<long long>((long long)(object));

    DEBUGF(LS_TAG, "attach rpc object, id[lld] local[%p] retrun:[%s]\n",
            id, object, resp.getReturn()->m_values[0].c_str());
}


}
