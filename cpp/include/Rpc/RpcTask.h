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

#ifndef _LANGUAGE_STACK_RPC_TASK_H_
#define _LANGUAGE_STACK_RPC_TASK_H_

#include "Task/ITask.h"
#include "Task/TaskThreadPool.h"

#include "Rpc/RpcCore.h"
#include "Defs.h"

namespace ls {

class LS_EXPORT CRpcCallTask : public ITask
{
public:
    CRpcCallTask(RpcCallPtr rpcCall)
        : m_call(rpcCall)
    {
        char buf[128];
        sprintf(buf, "java call : %d", m_call->m_callId);
        m_taskId = buf;
    }
    virtual ~CRpcCallTask() {}
    virtual void execute()
    {
        CRpcCore::instance()->onRpcCall(m_call);
    }
    virtual std::string getTaskId()
    {
        return m_taskId;
    }

private:
    RpcCallPtr          m_call;
    std::string         m_taskId;
};


}


#endif /* _LANGUAGE_STACK_RPC_TASK_H_ */
