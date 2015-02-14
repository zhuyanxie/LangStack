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


#include "RpcObjectPool.h"

#include "Reflect/IReflection.h"

namespace ls {

CRpcObjectPool::CRpcObjectPool()
{
}

CRpcObjectPool::~CRpcObjectPool()
{
    for (auto &oPair : m_objectPool)
    {
        if (oPair.second.autoDelete)
        {
            delete (IReflection*)oPair.second.obj;
        }
    }
    m_objectPool.clear();
}

///\brief   增加本地对象
void CRpcObjectPool::addObject(int64_t object, void *localObject, bool autodelete)
{
    Object o;
    o.obj = localObject;
    o.autoDelete = autodelete;

    std::lock_guard<std::mutex> g(m_lock);
    if (m_objectPool.count(object))
    {
        delete (IReflection*)m_objectPool[object].obj;
    }

    m_objectPool[object] = o;
}

///\brief   释放本地对象
void CRpcObjectPool::delObject(int64_t object)
{
    std::lock_guard<std::mutex> g(m_lock);
    m_objectPool.erase(object);
}

///\brief   释放本地对象
void CRpcObjectPool::delObjectByLocalObject(void *localObject)
{
    for (auto &oPair : m_objectPool)
    {
        if (oPair.second.obj == localObject)
        {
            m_objectPool.erase(oPair.first);
            return;
        }
    }
}

///\brief   获取本地对象
void* CRpcObjectPool::getObject(int64_t object)
{
    std::lock_guard<std::mutex> g(m_lock);
    if (m_objectPool.count(object))
    {
        return m_objectPool[object].obj;
    }
    return nullptr;
}

}


