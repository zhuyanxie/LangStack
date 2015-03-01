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


#ifndef _LANGUAGE_STACK_RPC_CALL_H_
#define _LANGUAGE_STACK_RPC_CALL_H_

#include <inttypes.h>

#include <atomic>
#include <memory>
#include <vector>
#include <sstream>

#include "Serial/Serial.h"
#include "Defs.h"

namespace ls {

typedef std::vector<std::string>        RpcValues;

///\brief   RPC类型定义
enum RpcType
{
    rpcTypeCall = 0,                        ///< RPC调用包
    rpcTypeReturn = 1,                      ///< RPC调用返回包
    rpcTypeUnvalid = 0xff,                  ///< 非法包
};

///\brief   RPC调用抽象
struct CRpcCall
{
    std::string                         m_method;   ///< 远程对象方法
    std::string                         m_cls;      ///< 类名
    int32_t                             m_callId;   ///< callID标记本次call
    int64_t                             m_object;   ///< 远程对象句柄, 句柄为空调用函数
    RpcValues                           m_values;   ///< 入参、出参(return)列表

    CRpcCall() : m_callId(-1) , m_object(0) {}

    ///\brief           用于回复的构造
    CRpcCall(int32_t callId) : m_callId(callId) , m_object(0) {}

    ///\brief           用于请求的构造
    ///\param[in]       method      方法名
    ///\param[in]       cls         类名，为空调用函数
    ///\param[in]       obj         对象句柄，为空调用静态方法
    CRpcCall(const char* method, const char *cls = "", void* obj = nullptr)
        : m_method(method)
        , m_cls(cls)
        , m_callId(createCallId())
        , m_object((int64_t)obj)
    {
    }

    ~CRpcCall()
    {
        m_values.clear();
    }

    ///\brief   加参数
    template<class T>
    void pushParams(const T &t)
    {
        std::stringstream ss;
        if (ls::Serializion<T>()(t, ss))
        {
            m_values.push_back(ss.str());
        }
    }

    template<class T, class ... Args>
    void pushParams(const T &t, const Args& ... args)
    {
        pushParams<T>(t);
        pushParams<Args...>(args...);
    }

    int32_t getCallId()
    {
        return m_callId;
    }

    int64_t getObject()
    {
        return m_object;
    }
private:
    ///\brief   生成一个call id
    int32_t createCallId()
    {
        static std::atomic<int32_t> s_id;
        while (++s_id == -1);
        return s_id;
    }
};

typedef std::shared_ptr<CRpcCall>    RpcCallPtr;

///\brief   CRpcCall序列化
template <> class Serializion<CRpcCall>
{
public:
    bool operator()(const CRpcCall &obj, std::ostream &os)
    {
        os << "RpcCall:";
        ls::serial(obj.m_method, os);
        ls::serial(obj.m_cls,    os);
        ls::serial(obj.m_callId, os);
        ls::serial(obj.m_object, os);

        for (auto param : obj.m_values)
        {
            os << param;
        }
        os << DETAIL_END;
        return true;
    }
};

template <> class Serializion<CRpcCall*>
{
public:
    bool operator()(CRpcCall *obj, std::ostream &os)
    {
        return Serializion<CRpcCall>()(*obj, os);
    }
};

template <> class Serializion<RpcCallPtr>
{
public:
    bool operator()(const RpcCallPtr &obj, std::ostream &os)
    {
        return Serializion<CRpcCall*>()(obj.get(), os);
    }
};


///\brief   CRpcCall反序列化
template <> class Deserializion<CRpcCall *>
{
public:
    bool operator()(const char *buf, CRpcCall *&val) const
    {
        if (!deserialCStringMatch(buf, "RpcCall:"))
        {
            return false;
        }

        val = new CRpcCall();
        int offset = strlen("RpcCall:");

        if (!deserial(buf, offset, val->m_method))
        {
            return false;
        }
        if (!deserial(buf, offset, val->m_cls))
        {
            return false;
        }
        if (!deserial(buf, offset, val->m_callId))
        {
            return false;
        }
        if (!deserial(buf, offset, val->m_object))
        {
            return false;
        }

        while (true)
        {
            size_t next = deserialGetDetailLength(buf + offset);
            if (next <= 1) break;
            val->m_values.push_back(std::string(buf + offset, next + 1));
            offset += next + strlen(DETAIL_END);
        }
        return true;
    }

private:
    template <class T> bool deserial(const char *buf, int &offset, T &val) const
    {
        size_t next = deserialGetDetailLength(buf + offset);
        if (!ls::deserial(buf + offset, val))
        {
            return false;
        }
        offset += next + strlen(DETAIL_END);
        return true;
    }
};

}


#endif /* _LANGUAGE_STACK_RPC_CALL_H_ */
