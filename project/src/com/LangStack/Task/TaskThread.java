package com.LangStack.Task;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Queue;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class TaskThread extends Thread
{
    private int                       mIndex             = 0;       ///< 线程索引
    private TaskThreadPool            mParent            = null;    ///< 所属线程池
    private Map<String, Queue<ITask>> mTaskLists         = null;    ///< 任务队列    
    private int                       mTaskCount         = 0;       ///< 任务数
    private Lock                      mLock              = null;    ///< 任务锁
    private int                       mIdleTick          = 0;       ///< 空闲滴答
    private int                       mMaxTick           = -1;      ///< 最多空闲
    private boolean                   mLoop              = true;    ///< 线程标记
    private static final int          THREAD_SLEEP_SPACE = 10;      ///< 睡眠间隔
    
    /**
     * @brief       构造
     * @param       parent          所属线程池
     * @param       maxIdleTime     线程最大空闲时间
     * @param       index           线程索引号
     */
    public TaskThread(TaskThreadPool parent, int maxIdleTime, int index)
    {
        mIndex = index;
        mParent = parent;
        mMaxTick = maxIdleTime == -1 ? -1 : maxIdleTime / THREAD_SLEEP_SPACE;
        mTaskLists = new HashMap<String, Queue<ITask>>();
        mLock = new ReentrantLock();
    }
    
    /**
     * @brief       添加任务
     * @param       task            任务
     */
    public void addTask(ITask task)
    {
        mLock.lock();
        mTaskLists.get(task.getTaskId()).offer(task);
        ++mTaskCount;
        mLock.unlock();
    }
    
    /**
     * @brief       获取当前线程负载
     * @return      线程权重
     */
    public int getWeight()
    {
        mLock.lock();
        int count = mTaskCount;
        mLock.unlock();
        return count;
    }
    
    /**
     * @brief       获取当前线程索引
     * @return      索引
     */
    public int getIndex()
    {
        return mIndex;
    }
    
    /**
     * @brief       释放当前线程
     */
    public void release()
    {
        mLoop = false;
        try {
            this.join();
        } catch (InterruptedException e)  {
            e.printStackTrace();
        }
        
        clearTasks();
    }

    /**
     * @brief       线程体
     */
    @Override public void run()
    {
        while (mLoop)
        {
            ArrayList<ITask> tasks = getExcuteTasks();
            if (tasks.isEmpty())
            {
                try {
                    Thread.sleep(THREAD_SLEEP_SPACE);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                
                if (mMaxTick != -1 && ++mIdleTick > mMaxTick)
                {
                    mParent.onThreadIdle(mIndex);
                }
                continue;
            }

            for (ITask task : tasks)
            {
                task.run();
            }
            
            mIdleTick = 0;
            mLock.lock();
            mTaskCount -= tasks.size();
            mLock.unlock();
            tasks.clear();
        }
    }
    
    /**
     * @brief       获取下轮执行任务
     * @return      可执行任务
     */
    private ArrayList<ITask> getExcuteTasks()
    {
        ArrayList<ITask> tasks = new ArrayList<ITask>();
        mLock.lock();
        for (Queue<ITask> queue : mTaskLists.values()) 
        {
            if (queue.isEmpty())
            {
                continue;
            }
            tasks.add(queue.poll());
        }
        mLock.unlock();
        
        return tasks;
    }
    
    /**
     * @brief   清空任务,并通知线程池
     */
    private void clearTasks()
    {
        mLock.lock();
        for (String key : mTaskLists.keySet()) 
        {
            mParent.onTaskIdle(key, mIndex);
        }
        mTaskLists.clear();
        mLock.unlock(); 
    }
}
