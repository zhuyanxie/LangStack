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


#ifndef _LANGUAGE_STACK_SERIALIZION_H_
#define _LANGUAGE_STACK_SERIALIZION_H_

#include <map>
#include <list>
#include <string>
#include <ostream>
#include <functional>

#include "Reflect/MetaFactory.h"
#include "SerializationConstant.h"
#include "Reflect/IReflection.h"

namespace ls {

///\brief   序列化
template <class T> class Serializion
{
public:
    ///\brief           序列化
    ///\param[in]       obj             需要序列化对象
    ///\param[in/out]   os              序列化输出流
    ///\return          序列化成功/失败
    bool operator()(T obj, std::ostream &os)
    {
//        DAHUA_STATIC_ASSERT(false && "Serializion NOT support type!");
        return false;
    }
};

///\brief   int序列化
template <> class Serializion<int>
{
public:
    bool operator()(int obj, std::ostream &os)
    {
        os << TAG_INT << obj << DETAIL_END;
        return true;
    }
};

///\brief   long long序列化
template <> class Serializion<long long>
{
public:
    bool operator()(long long obj, std::ostream &os)
    {
        os << TAG_LONGLONG << obj << DETAIL_END;
        return true;
    }
};

///\brief   double序列化
template <> class Serializion<double>
{
public:
    bool operator()(double obj, std::ostream &os)
    {
        os << TAG_DOUBLE << obj << DETAIL_END;
        return true;
    }
};

///\brief   字符串关键字符转码
static std::string serializionEncode(const char *buf)
{
    std::string res;
    while (buf && *buf)
    {
        if (*buf == '%') res += "%%";
        else if (*buf == TAG_END[0]) res += "%0";
        else if (*buf == DETAIL_END[0]) res += "%1";
        else res += *buf;
        ++buf;
    }
    return res;
}

///\brief   cstr序列化
template <> class Serializion<const char *>
{
public:
    bool operator()(const char *obj, std::ostream &os)
    {
        /// 允许空串"", 禁止NULL
        if (obj == NULL)
        {
            return false;
        }
        os << TAG_STRING << serializionEncode(obj) << DETAIL_END;
        return true;
    }
};

///\brief   string序列化
template <> class Serializion<std::string>
{
public:
    bool operator()(const std::string &obj, std::ostream &os)
    {
        return Serializion<const char *>()(obj.c_str(), os);
    }
};

///\brief   list相关
template <class T> class Serializion<std::list<T> >
{
public:
    bool operator()(const std::list<T> &obj, std::ostream &os)
    {
        bool ret = true;
        if (obj.empty())
        {
            os << TAG_EMPTY_LIST;
            return ret;
        }

        os << TAG_LIST;
        for (auto it = obj.begin(); it != obj.end(); ++it)
        {
            ret &= Serializion<T>()(*it, os);
        }
        os << DETAIL_END;

        return ret;
    }
};

///\brief   IReflection*序列化
template <> class Serializion<IReflection*>
{
public:
    bool operator()(IReflection* obj, std::ostream &os)
    {
        if (obj == NULL)
        {
            os << TAG_EMPTY_CLASS;
            return true;
        }

        CMetaTable *metaTable = NULL;
        CMetaFactory *s_meta = CMetaFactory::instance();
        if (!s_meta->getMetaTable(obj->getClassName(), metaTable))
        {
            return false;
        }

        bool ret = true;
        os << TAG_CLASS << obj->getClassName() << TAG_END;
        MetaParamTable &table = metaTable->getMetaTable();
        MetaParamTable::iterator it = table.begin();
        for (; it != table.end(); ++it)
        {
            MetaData *data = it->second;
            os << data->m_metaName << TAG_PARAM_SPLIT;
            switch(data->m_metaType)
            {
            case MetaDataTypeInt:
                ret &= Serializion<int>()(
                        *(int*)data->getMetaData(obj), os);
                break;
            case MetaDataTypeLonglong:
                ret &= Serializion<long long>()(
                        *(long long*)data->getMetaData(obj), os);
                break;
            case MetaDataTypeDouble:
                ret &= Serializion<double>()(
                        *(double*)data->getMetaData(obj), os);
                break;
            case MetaDataTypeString:
                ret &= Serializion<std::string>()(
                        *(std::string*)data->getMetaData(obj), os);
                break;
            case MetaDataTypeClass:
                ret &= Serializion<IReflection*>()(
                        *(IReflection**)data->getMetaData(obj), os);
                break;
            case MetaDataTypeIntList:
                ret &= Serializion<std::list<int> >()(
                        *(std::list<int>*)data->getMetaData(obj), os);
                break;
            case MetaDataTypeLonglongList:
                ret &= Serializion<std::list<long long> >()(
                        *(std::list<long long>*)data->getMetaData(obj), os);
                break;
            case MetaDataTypeDoubleList:
                ret &= Serializion<std::list<double> >()(
                        *(std::list<double>*)data->getMetaData(obj), os);
                break;
            case MetaDataTypeStringList:
                ret &= Serializion<std::list<std::string> >()(
                        *(std::list<std::string>*)data->getMetaData(obj), os);
                break;
            case MetaDataTypeClassList:
                ret &= Serializion<std::list<IReflection*> >()(
                        *(std::list<IReflection*>*)data->getMetaData(obj), os);
                break;
            case MetaDataTypeUnkown:
            default:
                ret = false;
                break;
            }
        }
        os << DETAIL_END;
        return ret;
    }
};


}


#endif /* _LANGUAGE_STACK_Serializion_H_ */
