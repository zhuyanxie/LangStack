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

#ifndef EASYTYPECLASS_H_
#define EASYTYPECLASS_H_

#include <cmath>
#include <iostream>

#include "Reflect/IReflection.h"

using namespace ls;

class CEasyTypeClass : public IReflection
{
public:
    CEasyTypeClass();
    virtual ~CEasyTypeClass();

    void dump();
    REFLECTION_METHOD_DECLEAR_ARGS0(void, dump)
    
    static double foo(int arg);
    REFLECTION_METHOD_DECLEAR_ARGS1(double, foo)

    bool operator==(const CEasyTypeClass &rhs) const;

public:
    int             m_int;
    long long       m_longlong;
    double          m_double;
    std::string     m_normal;
    std::string     m_special;

private:
    REFLECTION_CLASS_DECLEAR(CEasyTypeClass);

    REFLECTION_MEMBER_DECLEAR(CEasyTypeClass, int,            m_int);
    REFLECTION_MEMBER_DECLEAR(CEasyTypeClass, long long,      m_longlong);
    REFLECTION_MEMBER_DECLEAR(CEasyTypeClass, double,         m_double);
    REFLECTION_MEMBER_DECLEAR(CEasyTypeClass, std::string,    m_normal);
    REFLECTION_MEMBER_DECLEAR(CEasyTypeClass, std::string,    m_special);
};

/// class
REFLECTION_CLASS_IMPLEMENT(CEasyTypeClass);
/// member
REFLECTION_MEMBER_IMPLEMENT(CEasyTypeClass, int,            m_int);
REFLECTION_MEMBER_IMPLEMENT(CEasyTypeClass, long long,      m_longlong);
REFLECTION_MEMBER_IMPLEMENT(CEasyTypeClass, double,         m_double);
REFLECTION_MEMBER_IMPLEMENT(CEasyTypeClass, std::string,    m_normal);
REFLECTION_MEMBER_IMPLEMENT(CEasyTypeClass, std::string,    m_special);
/// method
REFLECTION_METHOD_ARGS0(CEasyTypeClass, dump, void);
REFLECTION_METHOD_ARGS1(CEasyTypeClass, foo, double, int);

std::list<int> foo(std::list<double> dl, IReflection* obj);
static REFLECTION_METHOD_DECLEAR_ARGS2(std::list<int>, foo);
REFLECTION_FUNCTION_ARGS2(foo, std::list<int>, std::list<double>, IReflection*);

template <class T> struct equalClass
{
    bool operator()(const T& lhs, const T& rhs) const
    {
        return lhs == rhs;
    }
};

template <> struct equalClass<double>
{
    bool operator()(double lhs, double rhs) const
    {
        return fabs(lhs - rhs) < 0.00001;
    }
};

template <class T> struct equalClass<std::list<T> >
{
    bool operator()(const std::list<T>& lhs, const std::list<T>& rhs) const
    {
        if (lhs.size() != rhs.size())
        {
            return false;
        }

        auto lit = lhs.begin();
        auto rit = rhs.begin();
        for ( ; lit != lhs.end() && rit != rhs.end(); ++lit, ++rit)
        {
            if (!equalClass<T>()(*lit, *rit))
            {
                return false;
            }
        }
        return true;
    }
};

template <> struct equalClass<IReflection* >
{
    bool operator()(const IReflection* lhs, const IReflection* rhs) const
    {
        if (lhs == NULL && rhs == NULL)
        {
            return true;
        }

        return *dynamic_cast<CEasyTypeClass*>(const_cast<IReflection*>(lhs)) ==
            *dynamic_cast<CEasyTypeClass*>(const_cast<IReflection*>(rhs));
    }
};

template<class T> bool isEqual(const T &lhs, const T& rhs)
{
    return equalClass<T>()(lhs, rhs);
}

#endif
