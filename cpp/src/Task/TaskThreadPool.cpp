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


#include "Task/TaskThreadPool.h"

#include "DeleteThreadTask.h"
#include "Log/Log.h"
#include "LangStackConstant.h"

namespace ls {

std::mutex CTaskThreadPool::s_initLock;
CTaskThreadPool* CTaskThreadPool::s_instance = NULL;

CTaskThreadPool* CTaskThreadPool::instance()
{
    if (NULL == s_instance)
    {
        std::lock_guard<std::mutex> lck(s_initLock);
        if (NULL == s_instance)
        {
            s_instance = new CTaskThreadPool();
        }
    }

    return s_instance;
}

CTaskThreadPool::CTaskThreadPool(int minThreadCount, int maxThreadCount,
        int maxIdleTime)
    : m_index(0)
    , m_minCount(minThreadCount)
    , m_maxCount(maxThreadCount)
    , m_idleTime(maxIdleTime)
{
    for (int i = 0; i < minThreadCount; ++i)
    {
        addThread(-1);
    }
}

CTaskThreadPool::~CTaskThreadPool()
{
    m_taskThreads.clear();
}

///\brief			设置新创建的线程的空闲超时时间
void CTaskThreadPool::setThreadIdleTime(int idleTime)
{
	m_idleTime = idleTime <= 0 ? 5000 : idleTime;
}

///\brief           添加一个任务到线程池
void CTaskThreadPool::addTask(ITask* task)
{
    std::lock_guard<std::mutex> lck(m_threadLock);
    getTheLightThread(task->getTaskId())->addTask(task);
}

///\brief           获取负载较轻的线程
TaskThreadPtr CTaskThreadPool::getTheLightThread(const std::string &id)
{
    std::map<std::string, int>::iterator itmap = mIdentifyMap.find(id);
    if (mIdentifyMap.end() != itmap && m_taskThreads.count(itmap->second))
    {
    	VERBOSEF(LS_TAG, "get thread sucess repeat id [%s->%d]\n",
    			id.c_str(), itmap->second);
        return m_taskThreads[itmap->second];
    }

    auto res = m_taskThreads.begin();
    int weight = res->second->getWeight();
    for (auto it = m_taskThreads.begin(); it != m_taskThreads.end(); ++it)
    {
        if (weight == 0)
        {
            break;
        }
        if (weight > it->second->getWeight())
        {
            res = it;
            weight = it->second->getWeight();
        }
    }

    if (weight != 0 && m_taskThreads.size() < size_t(m_maxCount))
    {
        int index = addThread(m_idleTime);
        res = m_taskThreads.find(index);
    }
	DEBUGF(LS_TAG, "get thread sucess [%s->%d]\n", id.c_str(), res->first);
    mIdentifyMap[id] = res->first;
    return res->second;
}

///\brief           在线程池中增加一个线程
int CTaskThreadPool::addThread(int maxIdleTime)
{
    m_taskThreads[m_index] = TaskThreadPtr(
        new TaskThread(this, maxIdleTime, m_index));
   return m_index++;
}

///\brief           线程空闲回调
void CTaskThreadPool::onThreadIdle(int index)
{
    std::lock_guard<std::mutex> lck(m_threadLock);
    auto it = m_taskThreads.find(index);
    if (it != m_taskThreads.end())
    {
        ITask *task = new CDeleteThreadTask(it->second);
        m_taskThreads.erase(index);
        getTheLightThread(task->getTaskId())->addTask(task);
    }
}

///\brief           任务队列完成回调
void CTaskThreadPool::onTaskIdle(const std::string &taskId, int index)
{
    std::lock_guard<std::mutex> lck(m_threadLock);
    auto it = mIdentifyMap.find(taskId);
    if (it->second == index)
    {
        mIdentifyMap.erase(it);
    }
}

}
