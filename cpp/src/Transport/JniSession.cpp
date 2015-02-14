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

#include "Transport/JniSession.h"

#include "jni/com_LangStack_LangStack.h"
#include "Rpc/RpcTask.h"
#include "Rpc/RpcCore.h"
#include "Task/TaskThreadPool.h"

namespace ls {

CJniSession::CJniSession()
{
}

CJniSession::~CJniSession()
{
}

///\brief       消息发送
bool CJniSession::send(RpcCallPtr call, RpcType type)
{
    char buffer[64 * 1024];
    MemoryOutputStream mos(buffer, 64 * 1024);
    Serializion<RpcCallPtr>()(call, mos);
    buffer[mos.charsWritten()] = 0;

    if (type == rpcTypeCall)
    {
        sendCall2Java(buffer);
    }
    else
    {
        sendReturn2Java(buffer);
    }
    return true;
}


///\brief		接收到java的调用
void CJniSession::onJavaCall(const char *str)
{
	CRpcCall *call;
    if (ls::deserial(str, call))
    {
        RpcCallPtr rpcCall(call);
		CTaskThreadPool::instance()->addTask(new CRpcCallTask(rpcCall));
    }
}

///\brief		接收到java的返回
void CJniSession::onJavaReturn(const char *str)
{
	CRpcCall *call;
    if (ls::deserial(str, call))
    {
        RpcCallPtr rpcCall(call);
        CRpcCore::instance()->onRpcReturn(rpcCall);
    }
}

}




