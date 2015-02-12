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

#ifndef _LANGUAGE_STACK_TASK_THREAD_POOL_H_
#define _LANGUAGE_STACK_TASK_THREAD_POOL_H_

#include <map>
#include <deque>
#include <string>
#include "Task/TaskThread.h"

namespace ls {

class CTaskThreadPool
{
public:
    ///\brief           单例
    static CTaskThreadPool* instance();

    ///\brief           添加一个任务到线程池
    void addTask(ITask* task);

    ///\brief           获取当前线程数量
    ///\return          线程总数
    inline int getThreadCount();

    ///\brief           线程空闲回调
    ///\param[in]       index           闲置线程ID
    void onThreadIdle(int index);

    ///\brief           任务队列空闲回调
    ///\param[in]       taskId          闲置任务队列ID
    void onTaskIdle(const std::string &taskId, int index);

private:
    ///\brief           获取负载较轻的线程
    TaskThreadPtr getTheLightThread(const std::string &id);

    ///\brief           在线程池中增加一个线程
    ///\return          增加的线程索引
    ///\param[in]       maxIdleTime     线程最大空闲时间
    int addThread(int maxIdleTime);

private:
    int                             m_index;            ///< 内部索引
    int                             m_minCount;         ///< 线程池最少线程数
    int                             m_maxCount;         ///< 线程池最大线程数
    int                             m_idleTime;         ///< 单个线程空闲时间
    std::mutex                      m_threadLock;       ///< 线程锁
    std::map<int, TaskThreadPtr>    m_taskThreads;      ///< 线程池
    std::map<std::string, int>      mIdentifyMap;       ///< 任务-线程索引表

private:
    ///\brief           构造
    ///\param[in]       最少线程数
    ///\param[in]       最大线程数
    ///\param[in]       单个线程空闲时间
    CTaskThreadPool(int minThreadCount = 4, int maxThreadCount = 64,
            int maxIdleTime = 15000);

    ~CTaskThreadPool();

private:
    static std::mutex               s_initLock;
    static CTaskThreadPool          *s_instance;
};


/// inlines

///\brief               获取当前线程数量
int CTaskThreadPool::getThreadCount()
{
    std::lock_guard<std::mutex> lck(m_threadLock);
    return (int)m_taskThreads.size();
}

}

#endif /* _PLAYER_COMPONENT_TASK_THREAD_POOL_H_ */
