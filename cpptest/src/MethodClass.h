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
#ifndef METHOD_CLASS_H_
#define METHOD_CLASS_H_

#include <math.h>
#include <stdio.h>
#include "EasyTypeClass.h"
#include "ICallBack.h"

class DEMO_EXPORT CMethodClass : public IReflection
{
public:
    CMethodClass();
    CMethodClass(int seed);
    virtual ~CMethodClass();

    void dump();
    REFLECTION_METHOD_DECLEAR_ARGS0(void, dump)

    double test(int i, long long j, std::list<int> ilist, std::list<long long> llist);
    REFLECTION_METHOD_DECLEAR_ARGS4(double, test)

    virtual void attach(long long handler) override;
    REFLECTION_METHOD_DECLEAR_ARGS1(void, attach)

    virtual void detach(long long handler) override;
    REFLECTION_METHOD_DECLEAR_ARGS1(void, detach)

    bool operator==(const CMethodClass&rhs) const;

private:
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
    REFLECTION_CLASS_DECLEAR(CMethodClass, "com.LangStackTest.MethodClass");
    REFLECTION_MEMBER_DECLEAR(CMethodClass, int,                        m_int);
    REFLECTION_MEMBER_DECLEAR(CMethodClass, long long,                  m_longlong);
    REFLECTION_MEMBER_DECLEAR(CMethodClass, double,                     m_double);
    REFLECTION_MEMBER_DECLEAR(CMethodClass, std::string,                m_string);
    REFLECTION_MEMBER_DECLEAR(CMethodClass, IReflection*,               m_class);
    REFLECTION_MEMBER_DECLEAR(CMethodClass, IReflection*,               m_classNULL);

    REFLECTION_MEMBER_DECLEAR(CMethodClass, std::list<int>,             m_intList);
    REFLECTION_MEMBER_DECLEAR(CMethodClass, std::list<long long>,       m_longlongList);
    REFLECTION_MEMBER_DECLEAR(CMethodClass, std::list<double>,          m_doubleList);
    REFLECTION_MEMBER_DECLEAR(CMethodClass, std::list<std::string>,     m_stringList);
    REFLECTION_MEMBER_DECLEAR(CMethodClass, std::list<IReflection*>,    m_classList);

    REFLECTION_MEMBER_DECLEAR(CMethodClass, std::list<int>,             m_intListEmpty);
    REFLECTION_MEMBER_DECLEAR(CMethodClass, std::list<long long>,       m_longlongListEmpty);
    REFLECTION_MEMBER_DECLEAR(CMethodClass, std::list<double>,          m_doubleListEmpty);
    REFLECTION_MEMBER_DECLEAR(CMethodClass, std::list<std::string>,     m_stringListEmpty);
    REFLECTION_MEMBER_DECLEAR(CMethodClass, std::list<IReflection*>,    m_classListEmpty);
    REFLECTION_MEMBER_DECLEAR(CMethodClass, long long,                  m_handler);
};

REFLECTION_CLASS_IMPLEMENT(CMethodClass);

REFLECTION_MEMBER_IMPLEMENT(CMethodClass, int,                          m_int);
REFLECTION_MEMBER_IMPLEMENT(CMethodClass, long long,                    m_longlong);
REFLECTION_MEMBER_IMPLEMENT(CMethodClass, double,                       m_double);
REFLECTION_MEMBER_IMPLEMENT(CMethodClass, std::string,                  m_string);
REFLECTION_MEMBER_IMPLEMENT(CMethodClass, IReflection*,                 m_class);
REFLECTION_MEMBER_IMPLEMENT(CMethodClass, IReflection*,                 m_classNULL);

REFLECTION_MEMBER_IMPLEMENT(CMethodClass, std::list<int>,               m_intList);
REFLECTION_MEMBER_IMPLEMENT(CMethodClass, std::list<long long>,         m_longlongList);
REFLECTION_MEMBER_IMPLEMENT(CMethodClass, std::list<double>,            m_doubleList);
REFLECTION_MEMBER_IMPLEMENT(CMethodClass, std::list<std::string>,       m_stringList);
REFLECTION_MEMBER_IMPLEMENT(CMethodClass, std::list<IReflection*>,      m_classList);

REFLECTION_MEMBER_IMPLEMENT(CMethodClass, std::list<int>,               m_intListEmpty);
REFLECTION_MEMBER_IMPLEMENT(CMethodClass, std::list<long long>,         m_longlongListEmpty);
REFLECTION_MEMBER_IMPLEMENT(CMethodClass, std::list<double>,            m_doubleListEmpty);
REFLECTION_MEMBER_IMPLEMENT(CMethodClass, std::list<std::string>,       m_stringListEmpty);
REFLECTION_MEMBER_IMPLEMENT(CMethodClass, std::list<IReflection*>,      m_classListEmpty);
REFLECTION_MEMBER_IMPLEMENT(CMethodClass, long long,                    m_handler);

REFLECTION_METHOD_ARGS0(CMethodClass, dump, void);
REFLECTION_METHOD_ARGS4(CMethodClass, test, double, int, long long, std::list<int>, std::list<long long>);
REFLECTION_METHOD_ARGS1(CMethodClass, attach, void, long long);
REFLECTION_METHOD_ARGS1(CMethodClass, detach, void, long long);
#endif /* METHOD_CLASS_H_ */
