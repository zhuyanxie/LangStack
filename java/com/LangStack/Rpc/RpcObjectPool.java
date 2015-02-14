package com.LangStack.Rpc;

import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class RpcObjectPool
{
    private Lock              mLock = null;
    private Map<Long, Object> mPool = null;

    public RpcObjectPool()
    {
        mLock = new ReentrantLock();
        mPool = new HashMap<Long, Object>();
    }

    /**
     * @brief   增加本地对象
     */
    public void addObject(long id, Object obj)
    {
        mLock.lock();
        if (mPool.containsKey(id))
        {
            mPool.remove(id);
        }
        mPool.put(id, obj);
        mLock.unlock();
    }
    
    /**
     * @brief   删除本地对象
     */
    public void delObject(long id)
    {
        mLock.lock();
        mPool.remove(id);
        mLock.unlock();
    }
    
    /**
     * @brief   删除本地对象
     */
    public void delObjectByLocalObject(Object o)
    {
        mLock.lock();

        for (Entry<Long, Object> entry: mPool.entrySet()) 
        {
            if (entry.getValue().equals(o))
            {
                mPool.remove(entry.getKey());
            }
        }
        mLock.unlock();
    }
    
    /**
     * @brief   获取本地对象
     */
    public Object getObject(long id)
    {
        mLock.lock();
        Object o = null;
        if (mPool.containsKey(id))
        {
            o = mPool.get(id);
        }
        mLock.unlock();
        return o;
    }
}
