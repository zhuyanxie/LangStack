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

#include "Task/TaskThread.h"
#include "Task/TaskThreadPool.h"

#include "Log/Log.h"
#include "LangStackConstant.h"

namespace ls {

///\brief           构造
TaskThread::TaskThread(CTaskThreadPool *parent, int maxIdleTime, int index)
    : m_index(index)
    , m_parent(parent)
    , m_idleTick(0)
    , m_maxTick(maxIdleTime == -1 ? -1 : maxIdleTime / s_idleTickSpace)
	, m_taskCount(0)
    , m_loop(true)
{
    m_thread = std::unique_ptr<std::thread>(new std::thread(
            std::bind(&TaskThread::threadProc, this)));

    DEBUGF(LS_TAG, "thread [%d] create index[%d] sucess\n", 
        getCurrentThreadId(), m_index);
}

TaskThread::~TaskThread()
{
    m_loop = false;
    if (std::this_thread::get_id() != m_thread->get_id())
    {
        m_thread->join();
    }
    else
    {
        /// TODO error!!!
    }

    DEBUGF(LS_TAG, "release index[%d], thread id[%d] sucess\n", 
        m_index, getCurrentThreadId());
    clearTasks();
}

///\brief           线程体
void TaskThread::threadProc()
{
    DEBUGF(LS_TAG, " index[%d], thread id[%d] enter\n",
        m_index, getCurrentThreadId());

    std::vector<ITask*> tempVec;
    while (m_loop)
    {
        getExcuteTasks(tempVec);
        if (tempVec.empty())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            m_idleTick += (m_maxTick != -1);

            if (m_idleTick > m_maxTick && m_maxTick != -1)
            {
                /// 线程空闲处理
                m_parent->onThreadIdle(m_index);
            }
            continue;
        }

        /// 非空线程
        for (auto task : tempVec)
        {
            task->execute();
            if (task != nullptr)
            {
                delete task;
            }
        }

        {
            std::lock_guard<std::mutex> lck(m_taskLock);
            m_taskCount -= tempVec.size();
        }

        m_idleTick = 0;
        tempVec.clear();
    }

    DEBUGF(LS_TAG, " index[%d], thread id[%d] leave\n",
        m_index, getCurrentThreadId());
}

///\brief           添加线程任务
void TaskThread::addTask(ITask *task)
{
    std::lock_guard<std::mutex> lck(m_taskLock);
    adjuestTask(task);
    m_taskLists[task->getTaskId()].push_back(task);
    ++m_taskCount;
}

///\brief           获取当前线程权重
int TaskThread::getWeight()
{
    std::lock_guard<std::mutex> lck(m_taskLock);
    return m_taskCount;
}

///\brief           获取当前线程ID
int TaskThread::getIndex()
{
    return m_index;
}

///\brief           任务过滤策略
void TaskThread::adjuestTask(ITask *task)
{
    /// TODO 增加任务过滤策略。
}

///\brief           获取下次执行任务
void TaskThread::getExcuteTasks(std::vector<ITask*> &vec)
{
    /// 拷贝任务，防止线程执行时加锁
    std::lock_guard<std::mutex> lck(m_taskLock);
    for (TaskLists::iterator it = m_taskLists.begin();
            it != m_taskLists.end(); ++it)
    {
        if (it->second.empty())
        {
            continue;
        }
        vec.push_back(it->second.front());
        it->second.pop_front();
    }
}

///\brief           清空所有待执行任务
void TaskThread::clearTasks()
{
    std::lock_guard<std::mutex> lck(m_taskLock);
    for (TaskLists::iterator it = m_taskLists.begin();
            it != m_taskLists.end(); ++it)
    {
        for (TaskList::iterator itx = it->second.begin();
                itx != it->second.end(); ++itx)
        {
            if (*itx != nullptr)
            {
            	ERRORF(LS_TAG, "clear undone task[%s]\n", (*itx)->getTaskId().c_str());
                delete *itx;
                *itx = nullptr;
            }
        }

        m_parent->onTaskIdle(it->first, m_index);
        it->second.clear();
    }

    m_taskLists.clear();
}

}
