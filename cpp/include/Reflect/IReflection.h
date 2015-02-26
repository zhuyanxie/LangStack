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


#ifndef _DAHUA_MOBILE_EVENT_IREFLECTION_H_
#define _DAHUA_MOBILE_EVENT_IREFLECTION_H_

#include "Reflect/MetaData.h"
#include "Reflect/MetaValue.h"
#include "Reflect/MetaFactory.h"

#include "Reflect/ReflectFunction.h"

namespace ls {

class IReflection
{
public:
    IReflection(){}
    virtual ~IReflection(){}

    ///\return      序列化类签名，使用java的反射类名
    virtual const char* getClassName() = 0;

    ///\brief       回调设置相关，必须从此接口实现
    virtual void attach(long long handler){}
    virtual void detach(long long handler){}
};

///\brief   声明反射类
#define REFLECTION_CLASS_DECLEAR(CLASS, JAVANAME)                           \
    public:                                                                 \
    inline static ::ls::IReflection* create##CLASS()      					\
    {                                                                       \
        return new CLASS();                                                 \
    }                                                                       \
    inline static const char* getJavaName() {return JAVANAME;}				\
    virtual const char* getClassName() {return JAVANAME;}                   \

///\brief   注册反射类信息到元表
#define REFLECTION_CLASS_IMPLEMENT(CLASS)                                   \
    namespace {                                                             \
    static struct CLASS##Register {                                         \
        CLASS##Register()                                                   \
        {                                                                   \
        ::ls::CMetaFactory::instance()->registerClass(    					\
        		CLASS::getJavaName(), CLASS::create##CLASS );      			\
        }                                                                   \
    } s_##CLASS##_ReflectionRegister;                                       \
    }

///\brief   反射成员变量声明，生成反射必须的get、set方法
#define REFLECTION_MEMBER_DECLEAR(CLASS, TYPE, NAME)                        \
    public:                                                                 \
    inline static void set##NAME(IReflection *obj, void *value)             \
    {                                                                       \
        ((CLASS*)(obj))->NAME = *(TYPE*)value;                              \
    }                                                                       \
    inline static void* get##NAME(IReflection *obj)                         \
    {                                                                       \
        return (void*)&(((CLASS*)(obj))->NAME);                             \
    }                                                                       \

///\brief   将反射参数信息注册到元表
#define REFLECTION_MEMBER_IMPLEMENT(CLASS, TYPE, NAME)                      \
    namespace {                                                             \
    static struct CLASS##NAME##Register                                     \
    {                                                                       \
        CLASS##NAME##Register()                                             \
        {                                                                   \
        ::ls::CMetaFactory::instance()->registerMember(   					\
        		CLASS::getJavaName(),                                       \
                #NAME,                                                      \
                ::ls::Type2MetaDataType< TYPE >()(),      					\
                CLASS::set##NAME,                                           \
                CLASS::get##NAME);                                          \
        }                                                                   \
    } s_##CLASS##NAME##_ReflectionRegister;                                 \
    }                                                                       \

///\brief   反射方法声明
#define REFLECTION_METHOD_DECLEAR_ARGS0(R, NAME)                                \
    R reflect_wrap_##NAME()                                                     \
{                                                                               \
    return NAME();                                                              \
}

#define REFLECTION_METHOD_DECLEAR_ARGS1(R, NAME)                                \
    R reflect_wrap_##NAME(const MetaValue &t1)                                  \
{                                                                               \
    return NAME(t1);                                                            \
}

#define REFLECTION_METHOD_DECLEAR_ARGS2(R, NAME)                                \
    R reflect_wrap_##NAME(const MetaValue &t1, const MetaValue &t2)             \
{                                                                               \
    return NAME(t1, t2);                                                        \
}

#define REFLECTION_METHOD_DECLEAR_ARGS3(R, NAME)                                \
    R reflect_wrap_##NAME(const MetaValue &t1, const MetaValue &t2,             \
            const MetaValue &t3)                                                \
{                                                                               \
    return NAME(t1, t2, t3);                                                    \
}

#define REFLECTION_METHOD_DECLEAR_ARGS4(R, NAME)                                \
    R reflect_wrap_##NAME(const MetaValue &t1, const MetaValue &t2,             \
            const MetaValue &t3, const MetaValue &t4)                           \
{                                                                               \
    return NAME(t1, t2, t3, t4);                                                \
}

#define REFLECTION_METHOD_DECLEAR_ARGS5(R, NAME)                                \
    R reflect_wrap_##NAME(const MetaValue &t1, const MetaValue &t2,             \
            const MetaValue &t3, const MetaValue &t4, const MetaValue &t5)      \
{                                                                               \
    return NAME(t1, t2, t3, t4, t5);                                            \
}

///\brief   反射方法注册类
class ReflectionMethodRegister
{
public:
    ReflectionMethodRegister(const char *cls, const char *method,
        const ParamTypes &types, void *invoke)
    {
        ::ls::CMetaFactory::instance()->registerMethod(
            cls, method, types, invoke);
    }
};

#if 0 /// TODO实现失败。
///\brief   反射方法注册到元表
#define REFLECTION_METHOD_ARGS(CLASS, NAME, R, ...)                         \
    namespace {                                                             \
    static ReflectionMethodRegister s_##CLASS##NAME##Register(              \
                CLASS::getJavaName(),                                       \
                #NAME,                                                      \
                Types2MetaDataTypes<R(##__VA_ARGS__)>()(),                  \
                new ::ls::ReflectFunciton<R>(                               \
                        &CLASS::reflect_wrap_##NAME, (CLASS*)0));}
#endif
///\brief   反射方法注册到元表
#define REFLECTION_METHOD_ARGS0(CLASS, NAME, R)                             \
    namespace {                                                             \
    static ReflectionMethodRegister s_##CLASS##NAME##Register(              \
    			CLASS::getJavaName(),                                       \
                #NAME,                                                      \
                {                                                           \
                ::ls::Type2MetaDataType< R >()()          					\
                },                                                          \
                new ::ls::ReflectFunciton<R>(             					\
                        &CLASS::reflect_wrap_##NAME, (CLASS*)0));}

#define REFLECTION_METHOD_ARGS1(CLASS, NAME, R, T1)                         \
    namespace {                                                             \
    static ReflectionMethodRegister s_##CLASS##NAME##Register(              \
				CLASS::getJavaName(),                                       \
                #NAME,                                                      \
                {                                                           \
                ::ls::Type2MetaDataType< R >()(),         					\
                ::ls::Type2MetaDataType< T1 >()(),        					\
                },                                                          \
                new ::ls::ReflectFunciton<R>(             					\
                        &CLASS::reflect_wrap_##NAME, (CLASS*)0));}

#define REFLECTION_METHOD_ARGS2(CLASS, NAME, R, T1, T2)                     \
    namespace {                                                             \
    static ReflectionMethodRegister s_##CLASS##NAME##Register(              \
                CLASS::getJavaName(),                                       \
                #NAME,                                                      \
                {                                                           \
                ::ls::Type2MetaDataType< R >()(),         					\
                ::ls::Type2MetaDataType< T1 >()(),        					\
                ::ls::Type2MetaDataType< T2 >()(),        					\
                },                                                          \
                new ::ls::ReflectFunciton<R>(             					\
                        &CLASS::reflect_wrap_##NAME, (CLASS*)0));}

#define REFLECTION_METHOD_ARGS3(CLASS, NAME, R, T1, T2, T3)                 \
    namespace {                                                             \
    static ReflectionMethodRegister s_##CLASS##NAME##Register(              \
                CLASS::getJavaName(),                                       \
                #NAME,                                                      \
                {                                                           \
                ::ls::Type2MetaDataType< R >()(),         					\
                ::ls::Type2MetaDataType< T1 >()(),        					\
                ::ls::Type2MetaDataType< T2 >()(),        					\
                ::ls::Type2MetaDataType< T3 >()(),        					\
                },                                                          \
                new ::ls::ReflectFunciton<R>(             					\
                        &CLASS::reflect_wrap_##NAME, (CLASS*)0));}


#define REFLECTION_METHOD_ARGS4(CLASS, NAME, R, T1, T2, T3, T4)             \
    namespace {                                                             \
    static ReflectionMethodRegister s_##CLASS##NAME##Register(              \
                CLASS::getJavaName(),                                       \
                #NAME,                                                      \
                {                                                           \
                ::ls::Type2MetaDataType< R >()(),         					\
                ::ls::Type2MetaDataType< T1 >()(),        					\
                ::ls::Type2MetaDataType< T2 >()(),        					\
                ::ls::Type2MetaDataType< T3 >()(),        					\
                ::ls::Type2MetaDataType< T4 >()(),        					\
                },                                                          \
                new ::ls::ReflectFunciton<R>(             					\
                        &CLASS::reflect_wrap_##NAME, (CLASS*)0));}

#define REFLECTION_METHOD_ARGS5(CLASS, NAME, R, T1, T2, T3, T4, T5)         \
    namespace {                                                             \
    static ReflectionMethodRegister s_##CLASS##NAME##Register(              \
                CLASS::getJavaName(),                                       \
                #NAME,                                                      \
                {                                                           \
                ::ls::Type2MetaDataType< R >()(),         					\
                ::ls::Type2MetaDataType< T1 >()(),        					\
                ::ls::Type2MetaDataType< T2 >()(),        					\
                ::ls::Type2MetaDataType< T3 >()(),        					\
                ::ls::Type2MetaDataType< T4 >()(),        					\
                ::ls::Type2MetaDataType< T5 >()(),        					\
                },                                                          \
                new ::ls::ReflectFunciton<R>(             					\
                        &CLASS::reflect_wrap_##NAME, (CLASS*)0));}


///\brief   反射函数注册到元表
#define REFLECTION_FUNCTION_ARGS0(NAME, R)                                      \
    namespace {                                                                 \
    static ReflectionMethodRegister s_##NAME##Register(                         \
                "",                                                             \
                #NAME,                                                          \
                {                                                               \
                ::ls::Type2MetaDataType< R >()()              					\
                },                                                              \
                new ::ls::ReflectFunciton<R>(reflect_wrap_##NAME));}

#define REFLECTION_FUNCTION_ARGS1(NAME, R, T1)                                  \
    namespace {                                                                 \
    static ReflectionMethodRegister s_##NAME##Register(                         \
                "",                                                             \
                #NAME,                                                          \
                {                                                               \
                ::ls::Type2MetaDataType< R >()(),             					\
                ::ls::Type2MetaDataType< T1 >()(),            					\
                },                                                              \
                new ::ls::ReflectFunciton<R>(reflect_wrap_##NAME));}

#define REFLECTION_FUNCTION_ARGS2(NAME, R, T1, T2)                              \
    namespace {                                                                 \
    static ReflectionMethodRegister s_##NAME##Register(                         \
                "",                                                             \
                #NAME,                                                          \
                {                                                               \
                ::ls::Type2MetaDataType< R >()(),             					\
                ::ls::Type2MetaDataType< T1 >()(),            					\
                ::ls::Type2MetaDataType< T2 >()(),            					\
                },                                                              \
                new ::ls::ReflectFunciton<R>(reflect_wrap_##NAME));}

#define REFLECTION_FUNCTION_ARGS3(NAME, R, T1, T2, T3)                          \
    namespace {                                                                 \
    static ReflectionMethodRegister s_##NAME##Register(                         \
                "",                                                             \
                #NAME,                                                          \
                {                                                               \
                ::ls::Type2MetaDataType< R >()(),             					\
                ::ls::Type2MetaDataType< T1 >()(),            					\
                ::ls::Type2MetaDataType< T2 >()(),            					\
                ::ls::Type2MetaDataType< T3 >()(),            					\
                },                                                              \
                new ::ls::ReflectFunciton<R>(reflect_wrap_##NAME));}


#define REFLECTION_FUNCTION_ARGS4(NAME, R, T1, T2, T3, T4)                      \
    namespace {                                                                 \
    static ReflectionMethodRegister s_##NAME##Register(                         \
                "",                                                             \
                #NAME,                                                          \
                {                                                               \
                ::ls::Type2MetaDataType< R >()(),             					\
                ::ls::Type2MetaDataType< T1 >()(),            					\
                ::ls::Type2MetaDataType< T2 >()(),            					\
                ::ls::Type2MetaDataType< T3 >()(),            					\
                ::ls::Type2MetaDataType< T4 >()(),            					\
                },                                                              \
                new ::ls::ReflectFunciton<R>(reflect_wrap_##NAME));}

#define REFLECTION_FUNCTION_ARGS5(NAME, R, T1, T2, T3, T4, T5)                  \
    namespace {                                                                 \
    static ReflectionMethodRegister s_##NAME##Register(                         \
                "",                                                             \
                #NAME,                                                          \
                {                                                               \
                ::ls::Type2MetaDataType< R >()(),             					\
                ::ls::Type2MetaDataType< T1 >()(),            					\
                ::ls::Type2MetaDataType< T2 >()(),            					\
                ::ls::Type2MetaDataType< T3 >()(),            					\
                ::ls::Type2MetaDataType< T4 >()(),            					\
                ::ls::Type2MetaDataType< T5 >()(),            					\
                },                                                              \
                new ::ls::ReflectFunciton<R>(reflect_wrap_##NAME));}

}

#endif /* _DAHUA_MOBILE_EVENT_ISERIALIZATION_H_ */
