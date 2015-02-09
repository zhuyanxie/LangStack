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


#ifndef _LANGUAGE_STACK_IRPC_API_H_
#define _LANGUAGE_STACK_IRPC_API_H_

#include "RpcCore.h"

namespace ls {

///\brief       内部远程接口调用封装结构
template<class R> struct RemoteInerfaceCallerInternal
{
    static R caller(RpcCallPtr &call)
    {
        RpcCallPtr ret;
        if (CRpcCore::instance()->call(call, ret))
        {
            return Deserializion<R>()(ret->m_values[0].c_str());
        }
        return R();
    }
};
template<> struct RemoteInerfaceCallerInternal<void>
{
    static void caller(RpcCallPtr &call)
    {
        RpcCallPtr ret;
        CRpcCore::instance()->call(call, ret);
    }
};

///\brief       远程接口调用封装结构
template<class R> struct RemoteInerfaceCaller
{
    static R caller(const char *method, const char *className, void *handler)
    {
        RpcCallPtr call(new CRpcCall(method, className, handler));
        return RemoteInerfaceCallerInternal<R>::caller(call);
    }

    template<class T0>
    static R caller(T0 t0, const char *method, const char *className, void *handler)
    {
        RpcCallPtr call(new CRpcCall(method, className, handler));
        call->pushParams(t0);
        return RemoteInerfaceCallerInternal<R>::caller(call);
    }

    template<class T0, class T1>
    static R caller(T0 t0, T1 t1, const char *method, const char *className, void *handler)
    {
        RpcCallPtr call(new CRpcCall(method, className, handler));
        call->pushParams(t0);
        call->pushParams(t1);
        return RemoteInerfaceCallerInternal<R>::caller(call);
    }

    template<class T0, class T1, class T2>
    static R caller(T0 t0, T1 t1, T2 t2, const char *method, const char *className, void *handler)
    {
        RpcCallPtr call(new CRpcCall(method, className, handler));
        call->pushParams(t0);
        call->pushParams(t1);
        call->pushParams(t2);
        return RemoteInerfaceCallerInternal<R>::caller(call);
    }

    template<class T0, class T1, class T2, class T3>
    static R caller(T0 t0, T1 t1, T2 t2, T3 t3, const char *method, const char *className, void *handler)
    {
        RpcCallPtr call(new CRpcCall(method, className, handler));
        call->pushParams(t0);
        call->pushParams(t1);
        call->pushParams(t2);
        call->pushParams(t3);
        return RemoteInerfaceCallerInternal<R>::caller(call);
    }

    template<class T0, class T1, class T2, class T3, class T4>
    static R caller(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, const char *method, const char *className, void *handler)
    {
        RpcCallPtr call(new CRpcCall(method, className, handler));
        call->pushParams(t0);
        call->pushParams(t1);
        call->pushParams(t2);
        call->pushParams(t3);
        call->pushParams(t4);
        return RemoteInerfaceCallerInternal<R>::caller(call);
    }
};

///\brief       远程接口调用封装
template<class R>
R callRemoteInterface(const char *method, const char *className, void *handler)
{
    return RemoteInerfaceCaller<R>::caller(method, className, handler);
}

template<class R, class T0>
R callRemoteInterface(T0 t0, const char *method, const char *className, void *handler)
{
    return RemoteInerfaceCaller<R>::caller(t0, method, className, handler);
}

template<class R, class T0, class T1>
R callRemoteInterface(T0 t0, T1 t1, const char *method, const char *className, void *handler)
{
    return RemoteInerfaceCaller<R>::caller(t0, t1, method, className, handler);
}

template<class R, class T0, class T1, class T2>
R callRemoteInterface(T0 t0, T1 t1,T2 t2, const char *method, const char *className, void *handler)
{
    return RemoteInerfaceCaller<R>::caller(t0, t1, t2, method, className, handler);
}

template<class R, class T0, class T1, class T2, class T3>
R callRemoteInterface(T0 t0, T1 t1, T2 t2, T3 t3, const char *method, const char *className, void *handler)
{
    return RemoteInerfaceCaller<R>::caller(t0, t1, t2, t3, method, className, handler);
}

template<class R, class T0, class T1, class T2, class T3, class T4>
R callRemoteInterface(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, const char *method, const char *className, void *handler)
{
    return RemoteInerfaceCaller<R>::caller(t0, t1, t2, t3, t4, method, className, handler);
}


}


#endif /* IRPCAPI_H_ */
