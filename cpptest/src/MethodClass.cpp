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
#include "MethodClass.h"

#include "ICallback.h"
#include "Log/Log.h"

CMethodClass::CMethodClass() : CMethodClass(5) {}

CMethodClass::CMethodClass(int seed) : m_int(seed),
        m_longlong(seed * 10001000LL), m_double(seed * 0.001001),
        m_class(0), m_classNULL(0), m_handler(0)
{
    char buf[256];
    sprintf(buf, "%d-%d", seed, seed);
    m_string  = buf;
    CEasyTypeClass* temp = new CEasyTypeClass();
    temp->m_double      = m_double;
    temp->m_int         = m_int;
    temp->m_longlong    = m_longlong;
    temp->m_special     = m_string + ":::%0%1%;;;";
    temp->m_normal      = m_string;
    m_class             = temp;

    for (int i = 1; i < 4; ++i)
    {
        m_intList.push_back(m_int + i);
        m_longlongList.push_back(m_longlong + i);
        m_doubleList.push_back(m_double + i);
        m_stringList.push_back(m_string);

        temp = new CEasyTypeClass();
        temp->m_double   = m_double;
        temp->m_int      = m_int + i;
        temp->m_longlong = m_longlong;
        temp->m_special  = m_string + ":::%%%;;;";
        temp->m_normal   = m_string;
        m_classList.push_back(temp);
    }
}

CMethodClass::~CMethodClass()
{
    if (m_class)
    {
        delete m_class;
        m_class = NULL;
    }

    for (auto it = m_classList.begin(); it != m_classList.end(); ++it)
    {
        delete *it;
    }
    m_classList.clear();
}

void CMethodClass::dump()
{
    VERBOSEF("test", "dump start:\n");
    dynamic_cast<CEasyTypeClass*>(m_class)->dump();

    for (auto it = m_classList.begin(); it != m_classList.end(); ++it)
    {
        dynamic_cast<CEasyTypeClass*>(*it)->dump();
    }

    if (0 != m_handler)
    {
        ICallBack *callback = (ICallBack *)m_handler;
        callback->interface1();
        VERBOSEF("test", "call back inerface2 return[%d]\n", callback->interface2(33));
        auto p = new CEasyTypeClass();
        VERBOSEF("test", "call back inerface3 return[%lf]\n", callback->interface3({1,2}, p));
        VERBOSEF("test", "call back inerface4 return[%s]\n", callback->interface4().c_str());
    }
    VERBOSEF("test", "dump over:\n");
}

double CMethodClass::test(int i, long long j, std::list<int> ilist, std::list<long long> llist)
{
	VERBOSEF("test", "cpp test called\n");
    return 0.01;
}

void CMethodClass::attach(long long handler)
{
    m_handler = handler;
}

void CMethodClass::detach(long long handler)
{
    if (m_handler == handler)
    {
        m_handler = 0;
    }
}

bool CMethodClass::operator==(const CMethodClass&rhs) const
{
    bool ret = isEqual(m_int,               rhs.m_int);
    ret     &= isEqual(m_longlong,          rhs.m_longlong);
    ret     &= isEqual(m_double,            rhs.m_double);
    ret     &= isEqual(m_string,            rhs.m_string);
    ret     &= isEqual(m_class,             rhs.m_class);
    ret     &= isEqual(m_classNULL,         rhs.m_classNULL);
    ret     &= isEqual(m_intList,           rhs.m_intList);
    ret     &= isEqual(m_longlongList,      rhs.m_longlongList);
    ret     &= isEqual(m_doubleList,        rhs.m_doubleList);
    ret     &= isEqual(m_stringList,        rhs.m_stringList);
    ret     &= isEqual(m_classList,         rhs.m_classList);
    ret     &= isEqual(m_intListEmpty,      rhs.m_intListEmpty);
    ret     &= isEqual(m_longlongListEmpty, rhs.m_longlongListEmpty);
    ret     &= isEqual(m_doubleListEmpty,   rhs.m_doubleListEmpty);
    ret     &= isEqual(m_stringListEmpty,   rhs.m_stringListEmpty);
    ret     &= isEqual(m_classListEmpty,    rhs.m_classListEmpty);

    return ret;
}
