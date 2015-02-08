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



#ifndef _LANGUAGE_STACK_TRANS_SESSION_H_
#define _LANGUAGE_STACK_TRANS_SESSION_H_

#include <inttypes.h>
#include <memory>
#include <atomic>

#include "Rpc/RpcCall.h"

namespace ls {

class ITransSession
{
public:
    ITransSession();
    virtual ~ITransSession();

    ///\brief       发送RPC请求
    ///\param[in]   type        RPC消息类型定义
    ///\return      true/false协议栈缓存不足
    virtual bool send(RpcCallPtr call, RpcType type) = 0;

    ///\brief       ID get/set
    inline uint32_t getSessionId()
    {
        return m_id;
    }
    inline void setSessionId(uint32_t id)
    {
        m_id = id;
    }

private:
    uint32_t                        m_id;       ///< session id

private:
    static uint32_t createSessionId();
    static std::atomic<uint32_t> s_sessionId;
};

typedef std::shared_ptr<ITransSession>  SessionPtr;

}


#endif /* _LANGUAGE_STACK_TRANS_SESSION_H_ */
