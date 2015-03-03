/*************************************************************************
 ** 版权保留(C), 2001-2014, 浙江大华技术股份有限公司.
 ** 版权所有.
 **
 ** $Id$
 **
 ** 功能描述   :
 **
 ** 修改历史     : 2014年12月11日 zhu_long Modification
*************************************************************************/

#ifndef NORMALTYPECLASS_H_
#define NORMALTYPECLASS_H_

#include <math.h>
#include <stdio.h>
#include "EasyTypeClass.h"
#include "ICallBack.h"

class CNormalTypeClass : public IReflection
{
public:
    CNormalTypeClass();
    CNormalTypeClass(int seed);
    virtual ~CNormalTypeClass();

    void dump();
    REFLECTION_METHOD_DECLEAR_ARGS0(void, dump)

    double test(int i, long long j, std::list<int> ilist, std::list<long long> llist);
    REFLECTION_METHOD_DECLEAR_ARGS4(double, test)

    virtual void attach(long long handler) override;
    REFLECTION_METHOD_DECLEAR_ARGS1(void, attach)

    virtual void detach(long long handler) override;
    REFLECTION_METHOD_DECLEAR_ARGS1(void, detach)

    bool operator==(const CNormalTypeClass&rhs) const;

public:
    int                             m_int;
    long long                       m_longlong;
    double                          m_double;
    std::string                     m_string;
    IReflection*                    m_class;
    IReflection*                    m_classNULL;

    std::list<int>                  m_intList;
    std::list<long long>            m_longlongList;
    std::list<double>               m_doubleList;
    std::list<std::string>          m_stringList;
    std::list<IReflection*>         m_classList;

    std::list<int>                  m_intListEmpty;
    std::list<long long>            m_longlongListEmpty;
    std::list<double>               m_doubleListEmpty;
    std::list<std::string>          m_stringListEmpty;
    std::list<IReflection*>         m_classListEmpty;

    long long                       m_handler;

private:
    REFLECTION_CLASS_DECLEAR(CNormalTypeClass);
    REFLECTION_MEMBER_DECLEAR(CNormalTypeClass, int,                        m_int);
    REFLECTION_MEMBER_DECLEAR(CNormalTypeClass, long long,                  m_longlong);
    REFLECTION_MEMBER_DECLEAR(CNormalTypeClass, double,                     m_double);
    REFLECTION_MEMBER_DECLEAR(CNormalTypeClass, std::string,                m_string);
    REFLECTION_MEMBER_DECLEAR(CNormalTypeClass, IReflection*,               m_class);
    REFLECTION_MEMBER_DECLEAR(CNormalTypeClass, IReflection*,               m_classNULL);

    REFLECTION_MEMBER_DECLEAR(CNormalTypeClass, std::list<int>,             m_intList);
    REFLECTION_MEMBER_DECLEAR(CNormalTypeClass, std::list<long long>,       m_longlongList);
    REFLECTION_MEMBER_DECLEAR(CNormalTypeClass, std::list<double>,          m_doubleList);
    REFLECTION_MEMBER_DECLEAR(CNormalTypeClass, std::list<std::string>,     m_stringList);
    REFLECTION_MEMBER_DECLEAR(CNormalTypeClass, std::list<IReflection*>,    m_classList);

    REFLECTION_MEMBER_DECLEAR(CNormalTypeClass, std::list<int>,             m_intListEmpty);
    REFLECTION_MEMBER_DECLEAR(CNormalTypeClass, std::list<long long>,       m_longlongListEmpty);
    REFLECTION_MEMBER_DECLEAR(CNormalTypeClass, std::list<double>,          m_doubleListEmpty);
    REFLECTION_MEMBER_DECLEAR(CNormalTypeClass, std::list<std::string>,     m_stringListEmpty);
    REFLECTION_MEMBER_DECLEAR(CNormalTypeClass, std::list<IReflection*>,    m_classListEmpty);
    REFLECTION_MEMBER_DECLEAR(CNormalTypeClass, long long,                  m_handler);
};

REFLECTION_CLASS_IMPLEMENT(CNormalTypeClass);

REFLECTION_MEMBER_IMPLEMENT(CNormalTypeClass, int,                          m_int);
REFLECTION_MEMBER_IMPLEMENT(CNormalTypeClass, long long,                    m_longlong);
REFLECTION_MEMBER_IMPLEMENT(CNormalTypeClass, double,                       m_double);
REFLECTION_MEMBER_IMPLEMENT(CNormalTypeClass, std::string,                  m_string);
REFLECTION_MEMBER_IMPLEMENT(CNormalTypeClass, IReflection*,                 m_class);
REFLECTION_MEMBER_IMPLEMENT(CNormalTypeClass, IReflection*,                 m_classNULL);

REFLECTION_MEMBER_IMPLEMENT(CNormalTypeClass, std::list<int>,               m_intList);
REFLECTION_MEMBER_IMPLEMENT(CNormalTypeClass, std::list<long long>,         m_longlongList);
REFLECTION_MEMBER_IMPLEMENT(CNormalTypeClass, std::list<double>,            m_doubleList);
REFLECTION_MEMBER_IMPLEMENT(CNormalTypeClass, std::list<std::string>,       m_stringList);
REFLECTION_MEMBER_IMPLEMENT(CNormalTypeClass, std::list<IReflection*>,      m_classList);

REFLECTION_MEMBER_IMPLEMENT(CNormalTypeClass, std::list<int>,               m_intListEmpty);
REFLECTION_MEMBER_IMPLEMENT(CNormalTypeClass, std::list<long long>,         m_longlongListEmpty);
REFLECTION_MEMBER_IMPLEMENT(CNormalTypeClass, std::list<double>,            m_doubleListEmpty);
REFLECTION_MEMBER_IMPLEMENT(CNormalTypeClass, std::list<std::string>,       m_stringListEmpty);
REFLECTION_MEMBER_IMPLEMENT(CNormalTypeClass, std::list<IReflection*>,      m_classListEmpty);
REFLECTION_MEMBER_IMPLEMENT(CNormalTypeClass, long long,                    m_handler);

REFLECTION_METHOD_ARGS0(CNormalTypeClass, dump, void);
REFLECTION_METHOD_ARGS4(CNormalTypeClass, test, double, int, long long, std::list<int>, std::list<long long>);
REFLECTION_METHOD_ARGS1(CNormalTypeClass, attach, void, long long);
REFLECTION_METHOD_ARGS1(CNormalTypeClass, detach, void, long long);
#endif /* NORMALTYPECLASS_H_ */
