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


#ifndef _LANGUAGE_STACK_RPC_OBJECT_POOL_H_
#define _LANGUAGE_STACK_RPC_OBJECT_POOL_H_

#include <inttypes.h>
#include <map>
#include <mutex>

namespace ls {

struct Object
{
    void            *obj;           ///< 本地对象指针
    bool            autoDelete;     ///< 是否自动释放
};

class CRpcObjectPool
{
public:
    CRpcObjectPool();
    ~CRpcObjectPool();

    ///\brief   增加本地对象
    void addObject(int64_t object, void *localObject, bool autodelete = true);

    ///\brief   释放本地对象
    void delObject(int64_t object);

    ///\brief   释放本地对象
    void delObjectByLocalObject(void *localObject);

    ///\brief   获取本地对象
    void* getObject(int64_t object);

private:
    std::mutex                      m_lock;
    std::map<int64_t, Object>       m_objectPool;
};

}


#endif /* _LANGUAGE_STACK_RPC_OBJECT_POOL_H_ */
