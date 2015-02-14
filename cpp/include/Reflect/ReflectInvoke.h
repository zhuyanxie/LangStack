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


#ifndef _LANGUAGE_STACK_REFLECT_INVOKE_H_
#define _LANGUAGE_STACK_REFLECT_INVOKE_H_

#include "Reflect/MetaDataTraits.h"

namespace ls {

class ReflectMethodInvoker
{
public:
    ///\brief 展开参数列表并执行指定方法
    template<class R>
    static R invoke(const MetaMethodData &method, RpcCallPtr &call)
    {
        std::vector<MetaValue> params;
        for (uint32_t i = 1; i <= method.paramsCount; ++i)
        {
            params.push_back(getParams(method.params[i], call->m_values[i-1]));
        }
        switch (params.size())
        {
        case 0: return invoke<R>(method.method, (void*)call->m_object);
        case 1: return invoke<R>(params[0], method.method, (void*)call->m_object);
        case 2: return invoke<R>(params[0], params[1], method.method, (void*)call->m_object);
        case 3: return invoke<R>(params[0], params[1], params[2], method.method, (void*)call->m_object);
        case 4: return invoke<R>(params[0], params[1], params[2], params[3], method.method, (void*)call->m_object);
        case 5: return invoke<R>(params[0], params[1], params[2], params[3], params[4], method.method, (void*)call->m_object);
        }

        return  R();
    }

private:
    static MetaValue getParams(MetaDataType type, const std::string &value)
    {
        switch (type)
         {
         case MetaDataTypeInt:
             return MetaValue(Deserializion<int>()(value.c_str()));
         case MetaDataTypeLonglong:
             return MetaValue(Deserializion<long long>()(value.c_str()));
         case MetaDataTypeDouble:
             return MetaValue(Deserializion<double>()(value.c_str()));
         case MetaDataTypeString:
         {
             std::string *str = new std::string();
             Deserializion<std::string>()(value.c_str(), *str);
             return MetaValue(str);
         }
         case MetaDataTypeClass:
             return MetaValue(Deserializion<IReflection*>()(value.c_str()));
         case MetaDataTypeIntList:
         {
             std::list<int> *il = new std::list<int>();
             Deserializion<std::list<int> >()(value.c_str(), *il);
             return MetaValue(il);
         }
         case MetaDataTypeLonglongList:
         {
             std::list<long long> *lll = new std::list<long long>();
             Deserializion<std::list<long long> >()(value.c_str(), *lll);
             return MetaValue(lll);
         }
         case MetaDataTypeDoubleList:
         {
             std::list<double> *dl = new std::list<double>();
             Deserializion<std::list<double> >()(value.c_str(), *dl);
             return MetaValue(dl);
         }
         case MetaDataTypeStringList:
         {
             std::list<std::string> *sl = new std::list<std::string>();
             Deserializion<std::list<std::string> >()(value.c_str(), *sl);
             return MetaValue(sl);
         }
         case MetaDataTypeClassList:
         {
             std::list<IReflection*> *cl = new std::list<IReflection*>();
             Deserializion<std::list<IReflection*> >()(value.c_str(), *cl);
             return MetaValue(cl);
         }
         default:
             break;
         }

        return MetaValue();
    }

private:
    template<class R>
    static R invoke(void *method, void *obj)
    {
        ReflectFunciton<R> *tMethod = (ReflectFunciton<R> *)method;
        tMethod->bind(obj);
        return tMethod->invoke();
    }

    template<class R>
    static R invoke(const MetaValue& t1, void *method, void *obj)
    {
        ReflectFunciton<R> *tMethod = (ReflectFunciton<R> *)method;
        tMethod->bind(obj);
        return tMethod->invoke(t1);
    }

    template<class R>
    static R invoke(const MetaValue& t1, const MetaValue& t2, void *method,
            void *obj)
    {
        ReflectFunciton<R> *tMethod = (ReflectFunciton<R> *)method;
        tMethod->bind(obj);
        return tMethod->invoke(t1, t2);
    }

    template<class R>
    static R invoke(const MetaValue& t1, const MetaValue& t2,
            const MetaValue& t3, void *method, void *obj)
    {
        ReflectFunciton<R> *tMethod = (ReflectFunciton<R> *)method;
        tMethod->bind(obj);
        return tMethod->invoke(t1, t2, t3);
    }

    template<class R>
    static R invoke(const MetaValue& t1, const MetaValue& t2,
            const MetaValue& t3, const MetaValue& t4, void *method, void *obj)
    {
        ReflectFunciton<R> *tMethod = (ReflectFunciton<R> *)method;
        tMethod->bind(obj);
        return tMethod->invoke(t1, t2, t3, t4);
    }

    template<class R>
    static R invoke(const MetaValue& t1, const MetaValue& t2,
            const MetaValue& t3, const MetaValue& t4, const MetaValue& t5,
            void *method, void *obj)
    {
        ReflectFunciton<R> *tMethod = (ReflectFunciton<R> *)method;
        tMethod->bind(obj);
        return tMethod->invoke(t1, t2, t3, t4, t5);
    }
};

}


#endif /* _LANGUAGE_STACK_REFLECT_INVOKE_H_ */
