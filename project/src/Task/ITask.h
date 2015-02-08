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


#ifndef _LANGUAGE_STACK_TITASK_H_
#define _LANGUAGE_STACK_TITASK_H_

#include <string>

namespace ls {

enum TaskType
{
    TaskTypeRpcCall = 0x0,          ///< RPC相关任务
    TaskTypeRpcReturn,

    TaskTypeUnCare = 0xff,          ///< 不关注任务类型
};

class ITask
{
public:
    ITask(){}
    virtual ~ITask(){}
    virtual TaskType getTaskType()
    {
        return TaskTypeUnCare;
    }

    virtual void execute() = 0;
    virtual std::string getTaskQueueIdentify() = 0;
};

}

#endif //_LANGUAGE_STACK_TITASK_H_
