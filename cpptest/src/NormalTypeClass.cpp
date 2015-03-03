/*************************************************************************
 ** 版权保留(C), 2001-2014, 浙江大华技术股份有限公司.
 ** 版权所有.
 **
 ** $Id$
 **
 ** 功能描述   :
 **
 ** 修改历史     : 2015年1月13日 zhu_long Modification
*************************************************************************/

#include "NormalTypeClass.h"

#include <iostream>
#include "ICallback.h"

CNormalTypeClass::CNormalTypeClass() : CNormalTypeClass(5) {}

CNormalTypeClass::CNormalTypeClass(int seed) : m_int(seed),
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

CNormalTypeClass::~CNormalTypeClass()
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

void CNormalTypeClass::dump()
{
    std::cout << "m_int:\t\t\t" << m_int << std::endl;
    std::cout << "m_longlong:\t\t" << m_longlong << std::endl;
    std::cout << "m_double:\t\t" << m_double << std::endl;
    std::cout << "m_string:\t\t" << m_string << std::endl;

    std::cout << "CEasyTypeClass:\t\t" << std::endl;
    dynamic_cast<CEasyTypeClass*>(m_class)->dump();

    for (auto it = m_classList.begin(); it != m_classList.end(); ++it)
    {
        std::cout << "CEasyTypeClass:\t\t" << std::endl;
        dynamic_cast<CEasyTypeClass*>(*it)->dump();
    }

    if (0 != m_handler)
    {
        ICallBack *callback = (ICallBack *)m_handler;
        callback->interface1();
        std::cout << __FILE__ << "-" << __LINE__ << ":" << callback->interface2(33) << std::endl;
        std::cout << __FILE__ << "-" << __LINE__ << ":" << callback->interface3({1,2}, nullptr) << std::endl;
        std::cout << __FILE__ << "-" << __LINE__ << ":" << callback->interface4() << std::endl;

        std::cout << __FILE__ << "-" << __LINE__ << ":" <<
                callback->interface3({1,2}, new CEasyTypeClass()) << std::endl;
    }
}

double CNormalTypeClass::test(int i, long long j, std::list<int> ilist, std::list<long long> llist)
{
    std::cout << "i:\t\t\t" << i << std::endl;
    std::cout << "j:\t\t\t" << j << std::endl;

    std::cout << "m_int:\t\t\t" << m_int << std::endl;
    std::cout << "m_longlong:\t\t" << m_longlong << std::endl;
    std::cout << "m_double:\t\t" << m_double << std::endl;
    std::cout << "m_string:\t\t" << m_string << std::endl;

    for (auto &ii : ilist) std::cout << "ii:\t\t\t\t" << ii << std::endl;
    for (auto &ll : llist) std::cout << "ll:\t\t\t\t" << ll << std::endl;

    return 0.01;
}

void CNormalTypeClass::attach(long long handler)
{
    m_handler = handler;
}

void CNormalTypeClass::detach(long long handler)
{
    if (m_handler == handler)
    {
        m_handler = 0;
    }
}

bool CNormalTypeClass::operator==(const CNormalTypeClass&rhs) const
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
