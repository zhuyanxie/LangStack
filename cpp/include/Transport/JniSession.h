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

#ifndef _LANGUAGE_STACK_JNI_SESSION_H_
#define _LANGUAGE_STACK_JNI_SESSION_H_

#include <jni.h>

#include "TransSession.h"
#include "Defs.h"

namespace ls {

///\brief       一个tcp session对应一个线程
class CJniSession : public ITransSession
{
public:
    CJniSession();
    virtual ~CJniSession();

    ///\brief       消息发送
    virtual bool send(RpcCallPtr call, RpcType type);

    ///\brief		接收到java的调用
    static void onJavaCall(const char *str);

    ///\brief		接收到java的返回
    static void onJavaReturn(const char *str);
};

}



#endif /* _LANGUAGE_STACK_JNI_SESSION_H_ */
