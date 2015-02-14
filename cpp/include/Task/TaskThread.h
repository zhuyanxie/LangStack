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


#ifndef _LANGUAGE_STACK_TTASK_THREAD_H_
#define _LANGUAGE_STACK_TTASK_THREAD_H_

#include <map>
#include <list>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <memory>

#include "Task/ITask.h"

namespace ls {

typedef std::list<ITask*>                   TaskList;
typedef std::map<std::string, TaskList>     TaskLists;

class CTaskThreadPool;
class TaskThread
{
public:
    static const int s_idleTickSpace = 10;              ///< 空闲线程sleep时间

public:
    ///\brief           构造
    ///\param[in]       param       持有线程池对象指针
    ///\param[in]       maxIdleTime 线程最大空闲时间
    ///\param[in]       index       当前线程索引
    TaskThread(CTaskThreadPool *parent, int maxIdleTime, int index);
    ~TaskThread();

    ///\brief           线程体
    void threadProc();

    ///\brief           添加线程任务
    ///\param[in]       task        任务
    void addTask(ITask *task);

    ///\brief           获取当前线程权重
    ///\return          权重
    int getWeight();

    ///\brief           获取当前线程ID
    int getIndex();

private:
    ///\brief           任务过滤策略
    void adjuestTask(ITask *task);

    ///\brief           获取下次执行任务
    ///\param[in,out]   vec         任务列表
    void getExcuteTasks(std::vector<ITask*> &vec);

    ///\brief           清空所有待执行任务
    void clearTasks();

private:
    int                             m_index;                ///< 线程索引
    CTaskThreadPool                 *m_parent;              ///< threadpool
    int                             m_idleTick;             ///< 空闲tick数
    int                             m_maxTick;              ///< 最多空闲tick数
    int                             m_taskCount;            ///< 任务数量
    std::mutex                      m_taskLock;             ///< 任务锁
    TaskLists                       m_taskLists;            ///< 任务列表
    bool                            m_loop;                 ///< 线程循环标记
    std::unique_ptr<std::thread>    m_thread;               ///< 线程体
};

typedef std::shared_ptr<TaskThread>     TaskThreadPtr;

}

#endif /* _LANGUAGE_STACK_TTASK_THREAD_H_ */
