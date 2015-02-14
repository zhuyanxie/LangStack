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

#ifndef _LANGUAGE_STACK_REFLECT_FUNCTION_H_
#define _LANGUAGE_STACK_REFLECT_FUNCTION_H_

#include "Reflect/MetaValue.h"
#include <functional>

namespace ls {

#define REFLECT_ARGS0 ()
#define REFLECT_ARGS1 (const MetaValue &t0)
#define REFLECT_ARGS2 (const MetaValue &t0, const MetaValue &t1)
#define REFLECT_ARGS3 (const MetaValue &t0, const MetaValue &t1, const MetaValue &t2)
#define REFLECT_ARGS4 (const MetaValue &t0, const MetaValue &t1, const MetaValue &t2, const MetaValue &t3)
#define REFLECT_ARGS5 (const MetaValue &t0, const MetaValue &t1, const MetaValue &t2, const MetaValue &t3, const MetaValue &t4)

template<class R>
class ReflectFunciton
{
public:
    enum ReflectFuncitonType
    {
        ReflectFunciton0,
        ReflectFunciton1,
        ReflectFunciton2,
        ReflectFunciton3,
        ReflectFunciton4,
        ReflectFunciton5,

        ReflectMethod0,
        ReflectMethod1,
        ReflectMethod2,
        ReflectMethod3,
        ReflectMethod4,
        ReflectMethod5,
    };

    class X {};
    typedef R (X::*MEM_FUNCTION0)REFLECT_ARGS0;
    typedef R (X::*MEM_FUNCTION1)REFLECT_ARGS1;
    typedef R (X::*MEM_FUNCTION2)REFLECT_ARGS2;
    typedef R (X::*MEM_FUNCTION3)REFLECT_ARGS3;
    typedef R (X::*MEM_FUNCTION4)REFLECT_ARGS4;
    typedef R (X::*MEM_FUNCTION5)REFLECT_ARGS5;

    typedef R (*PTR_FUNCTION0)REFLECT_ARGS0;
    typedef R (*PTR_FUNCTION1)REFLECT_ARGS1;
    typedef R (*PTR_FUNCTION2)REFLECT_ARGS2;
    typedef R (*PTR_FUNCTION3)REFLECT_ARGS3;
    typedef R (*PTR_FUNCTION4)REFLECT_ARGS4;
    typedef R (*PTR_FUNCTION5)REFLECT_ARGS5;

    union
    {
        MEM_FUNCTION0 m0;
        MEM_FUNCTION1 m1;
        MEM_FUNCTION2 m2;
        MEM_FUNCTION3 m3;
        MEM_FUNCTION4 m4;
        MEM_FUNCTION5 m5;

        PTR_FUNCTION0 f0;
        PTR_FUNCTION1 f1;
        PTR_FUNCTION2 f2;
        PTR_FUNCTION3 f3;
        PTR_FUNCTION4 f4;
        PTR_FUNCTION5 f5;
    }m_func;

    X*                      m_obj;
    ReflectFuncitonType     m_type;

public:
    template<class T> ReflectFunciton(R(T::*f)REFLECT_ARGS0, const T* t)
    {
        m_type      = ReflectMethod0;
        m_func.m0   = horrible_cast<MEM_FUNCTION0>(f);
        m_obj       = horrible_cast<X*>(t);
    }
    template<class T> ReflectFunciton(R(T::*f)REFLECT_ARGS1, const T* t)
    {
        m_type      = ReflectMethod1;
        m_func.m1   = horrible_cast<MEM_FUNCTION1>(f);
        m_obj       = horrible_cast<X*>(t);
    }
    template<class T> ReflectFunciton(R(T::*f)REFLECT_ARGS2, const T* t)
    {
        m_type      = ReflectMethod2;
        m_func.m2   = horrible_cast<MEM_FUNCTION2>(f);
        m_obj       = horrible_cast<X*>(t);
    }
    template<class T> ReflectFunciton(R(T::*f)REFLECT_ARGS3, const T* t)
    {
        m_type      = ReflectMethod3;
        m_func.m3   = horrible_cast<MEM_FUNCTION3>(f);
        m_obj       = horrible_cast<X*>(t);
    }
    template<class T> ReflectFunciton(R(T::*f)REFLECT_ARGS4, const T* t)
    {
        m_type      = ReflectMethod4;
        m_func.m4   = horrible_cast<MEM_FUNCTION4>(f);
        m_obj       = horrible_cast<X*>(t);
    }
    template<class T> ReflectFunciton(R(T::*f)REFLECT_ARGS5, const T* t)
    {
        m_type      = ReflectMethod5;
        m_func.m5   = horrible_cast<MEM_FUNCTION5>(f);
        m_obj       = horrible_cast<X*>(t);
    }

    ReflectFunciton(PTR_FUNCTION0 f, void *args = nullptr)
    {
        m_type      = ReflectFunciton0;
        m_func.f0   = f;
        m_obj       = (X*)args;
    }
    ReflectFunciton(PTR_FUNCTION1 f, void *args = nullptr)
    {
        m_type      = ReflectFunciton1;
        m_func.f1   = f;
        m_obj       = (X*)args;
    }
    ReflectFunciton(PTR_FUNCTION2 f, void *args = nullptr)
    {
        m_type      = ReflectFunciton2;
        m_func.f2   = f;
        m_obj       = (X*)args;
    }
    ReflectFunciton(PTR_FUNCTION3 f, void *args = nullptr)
    {
        m_type      = ReflectFunciton3;
        m_func.f3   = f;
        m_obj       = (X*)args;
    }
    ReflectFunciton(PTR_FUNCTION4 f, void *args = nullptr)
    {
        m_type      = ReflectFunciton4;
        m_func.f4   = f;
        m_obj       = (X*)args;
    }
    ReflectFunciton(PTR_FUNCTION5 f, void *args = nullptr)
    {
        m_type      = ReflectFunciton5;
        m_func.f5   = f;
        m_obj       = (X*)args;
    }

    void bind(void *obj)
    {
        m_obj = (X*) obj;
    }

    R invoke REFLECT_ARGS0
    {
        if (m_type == ReflectMethod0)
        {
            return (m_obj->*m_func.m0)();
        }
        else
        {
            return (*m_func.f0)();
        }
    }

    R invoke REFLECT_ARGS1
    {
        if (m_type == ReflectMethod1)
        {
            return (m_obj->*m_func.m1)(t0);
        }
        else
        {
            return (*m_func.f1)(t0);
        }
    }

    R invoke REFLECT_ARGS2
    {
        if (m_type == ReflectMethod2)
        {
            return (m_obj->*m_func.m2)(t0, t1);
        }
        else
        {
            return (*m_func.f2)(t0, t1);
        }
    }

    R invoke REFLECT_ARGS3
    {
        if (m_type == ReflectMethod3)
        {
            return (m_obj->*m_func.m3)(t0, t1, t2);
        }
        else
        {
            return (*m_func.f3)(t0, t1, t2);
        }
    }

    R invoke REFLECT_ARGS4
    {
        if (m_type == ReflectMethod4)
        {
            return (m_obj->*m_func.m4)(t0, t1, t2, t3);
        }
        else
        {
            return (*m_func.f4)(t0, t1, t2, t3);
        }
    }

    R invoke REFLECT_ARGS5
    {
        if (m_type == ReflectMethod5)
        {
            return (m_obj->*m_func.m5)(t0, t1, t2, t3, t4);
        }
        else
        {
            return (*m_func.f5)(t0, t1, t2, t3, t4);
        }
    }

private:
    template <class OutputClass, class InputClass>
    union horrible_union{
        OutputClass out;
        InputClass in;
    };

    template <class OutputClass, class InputClass>
    inline OutputClass horrible_cast(const InputClass input){
        horrible_union<OutputClass, InputClass> u;
        static_assert(sizeof(InputClass) == sizeof(u), "unsalf type cast");
        u.in = input;
        return u.out;
    }
};

#undef REFLECT_ARGS0
#undef REFLECT_ARGS1
#undef REFLECT_ARGS2
#undef REFLECT_ARGS3
#undef REFLECT_ARGS4
#undef REFLECT_ARGS5

}


#endif /* _LANGUAGE_STACK_REFLECT_FUNCTION_H_ */
