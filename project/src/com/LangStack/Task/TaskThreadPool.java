package com.LangStack.Task;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class TaskThreadPool
{
    private static final int         MIN_THREAD_COUNT = 4;      ///< 线程池最少线程
    private static final int         MAX_THREAD_COUNT = 64;     ///< 线程池最大线程
    private static final int         THREAD_IDLE_TIME = 15000;  ///< 线程最大空闲时间

    private static TaskThreadPool    sTaskThreadPool  = null;   ///< 单例
    
    private Map<Integer, TaskThread> mThreadPool      = null;   ///< 线程池
    private Map<String, Integer>     mIdentifyMap     = null;   ///< 任务ID索引
    private Lock                     mLock            = null;   ///< 锁
    private int                      mIndex           = 0;      ///< 内部线程索引
    

    /**
     * @return 单例
     */
    public synchronized static TaskThreadPool instance()
    {
        if (sTaskThreadPool == null)
        {
            sTaskThreadPool = new TaskThreadPool();
        }
        return sTaskThreadPool;
    }
    
    /**
     * @brief       添加任务
     * @param       task        任务
     */
    public void addTask(ITask task)
    {
        mLock.lock();
        getTheLightThread(task.getTaskId()).addTask(task);
        mLock.unlock();
    }
    
    /**
     * @brief       线程空闲回调
     * @param       index       线程索引
     */
    public void onThreadIdle(int index)
    {
        mLock.lock();
        if (mThreadPool.containsKey(index))
        {
            TaskThread thread = mThreadPool.get(index);
            if (thread.getWeight() == 0)
            {
                ITask task = new DeleteThreadTask(thread);
                mThreadPool.remove(index);
                getTheLightThread(task.getTaskId()).addTask(task);
            }
        }
        mLock.unlock();    
    }
    
    /**
     * @biref       线程任务队列空闲回调
     * @param       taskId      任务队列标识
     * @param       index       线程ID
     */
    public void onTaskIdle(String taskId, int index)
    {
        mLock.lock();
        if (mIdentifyMap.containsKey(taskId))
        {
            if (mIdentifyMap.get(taskId) == index)
            {
                mIdentifyMap.remove(taskId);
            }
        }
        mLock.unlock();     
    }
    
    public int getThreadCount()
    {
        mLock.lock();
        int res = mThreadPool.size();
        mLock.unlock();  
        return res;
    }
    
    /**
     * @brief       获取负载最轻的线程
     * @param       taskId          任务ID(相同任务ID会分配同一个线程)
     * @return      线程
     */
    private TaskThread getTheLightThread(String taskId)
    {
        if (mIdentifyMap.containsKey(taskId))
        {
            int key = mIdentifyMap.get(taskId);
            if (mThreadPool.containsKey(key))
            {
                return mThreadPool.get(mIdentifyMap.get(taskId));
            }
        }
        
        int weight = Integer.MAX_VALUE;
        TaskThread lightThread = null;
        for (TaskThread thread : mThreadPool.values())
        {
            if (thread.getWeight() < weight)
            {
                weight = thread.getWeight();
                lightThread = thread;
                if (weight == 0)
                {
                	break;
                }
                
            }
        }
        
        if (weight != 0 && mThreadPool.size() < MAX_THREAD_COUNT)
        {
            lightThread = new TaskThread(this, THREAD_IDLE_TIME, mIndex);
            mThreadPool.put(mIndex, lightThread);
            while (mThreadPool.containsKey(mIndex)) 
            {
                ++mIndex;
            }
        }
        
        mIdentifyMap.put(taskId, lightThread.getIndex());
        return lightThread;
    }

    /**
     * @brief       构造，创建默认的最少线程
     */
    private TaskThreadPool()
    {
        mThreadPool = new HashMap<Integer, TaskThread>();
        mIdentifyMap = new HashMap<String, Integer>();
        mLock = new ReentrantLock();
        for (int i = 0; i < MIN_THREAD_COUNT; ++i)
        {
        	/// 最低线程永不超时
            mThreadPool.put(mIndex, new TaskThread(this, -1, mIndex));
            while (mThreadPool.containsKey(mIndex)) 
            {
                ++mIndex;
            }
        }
    }
}
