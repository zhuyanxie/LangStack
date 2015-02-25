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


#ifndef _LANGUAGE_STACK_THREAD_TASK_H_
#define _LANGUAGE_STACK_THREAD_TASK_H_

#include "Task/ITask.h"
#include "Task/TaskThread.h"

namespace ls {

class CDeleteThreadTask : public ITask
{
public:
    CDeleteThreadTask(TaskThreadPtr thread) : m_thread(thread)
    {
        char buf[64];
        sprintf(buf, "thread:%d", m_thread->getIndex());
        m_id = buf;
    }
    virtual ~CDeleteThreadTask() {}
    virtual void execute()
    {
        m_thread.reset();
    }
    virtual std::string getTaskId()
    {
        return m_id;
    }

private:
    std::string                 m_id;
    TaskThreadPtr               m_thread;
};


}


#endif /* _LANGUAGE_STACK_THREAD_TASK_H_ */
