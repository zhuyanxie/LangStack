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


#ifndef _LANGUAGE_STACK_DESERIALIZION_H_
#define _LANGUAGE_STACK_DESERIALIZION_H_

#include <list>
#include <string>
#include <istream>

#include "Reflect/IReflection.h"
#include "Defs.h"

namespace ls {

///\brief   序列化
template <class T> class Deserializion
{
public:
    ///\brief           反序列化
    ///\param[in]       obj             需要序列化对象
    ///\param[in]       len             缓存长度
    ///\param[out]      val             值
    ///\return          序列化成功/失败
    bool operator()(const char *buf, T &val) const
    {
        return false;
    }
};

static bool deserialCStringMatch(const char *buf, const char *tag)
{
    for (; *buf && *tag && *buf == *tag; ++buf, ++tag);
    return *tag == 0;
}

static const char* deserialFindCString(const char *buf, const char *tag)
{
    while (*buf && *tag && !deserialCStringMatch(buf, tag)) ++buf;
    return *buf ? buf : nullptr;
}

static size_t deserialGetDetailLength(const char *buf)
{
    int endflag = 0;

    const char *p = buf;
    for (; *p ; ++p)
    {
        if (deserialCStringMatch(p, DETAIL_END))
        {
            if (--endflag <= 0)
            {
                break;
            }
        }
        else if (deserialCStringMatch(p, TAG_END))
        {
            ++endflag;
        }
    }

    return p - buf;
}

static bool deserialSampleValueFromString(const char *buf, const char *checkTag,
        const char *fmt, void *val)
{
    if (!deserialCStringMatch(buf, checkTag))
    {
        return false;
    }
    size_t offset = strlen(checkTag);
    sscanf(buf + offset, fmt, val);
    return true;
}

///\brief   int的反序列化
template <> class Deserializion<int>
{
public:
    bool operator()(const char *buf, int &val) const
    {
        return deserialSampleValueFromString(buf, TAG_INT, "%d", &val);
    }
    int operator()(const char *buf) const
    {
        int res(0);
        this->operator ()(buf, res);
        return res;
    }
};

///\brief   long long的反序列化
template <> class Deserializion<long long>
{
public:
    bool operator()(const char *buf, long long &val) const
    {
        return deserialSampleValueFromString(buf, TAG_LONGLONG, "%lld", &val);
    }
    long long operator()(const char *buf) const
    {
        long long res(0);
        this->operator ()(buf, res);
        return res;
    }
};

///\brief   double的反序列化
template <> class Deserializion<double>
{
public:
    bool operator()(const char *buf, double &val) const
    {
        return deserialSampleValueFromString(buf, TAG_DOUBLE, "%lf", &val);
    }
    double operator()(const char *buf) const
    {
        double res(0);
        this->operator ()(buf, res);
        return res;
    }
};

///\brief   字符串关键字符解码
static std::string serializionDecode(const char *buf, size_t len)
{
    std::string res;
    for (size_t i = 0; i < len; ++i)
    {
        if (buf[i] == '%' && i + 1 < len)
        {
            if (buf[i + 1] == '%') res += '%';
            else if (buf[i + 1] == '0') res += TAG_END[0];
            else if (buf[i + 1] == '1') res += DETAIL_END[0];
            else
            {
                /// drop this code
            }
            ++i;
        }
        else
        {
            res += buf[i];
        }
    }

    return res;
}

///\brief   std::string的反序列化
template <> class Deserializion<std::string>
{
public:
    bool operator()(const char *buf, std::string &val) const
    {
        if (!deserialCStringMatch(buf, TAG_STRING))
        {
            return false;
        }
        size_t offset = strlen(TAG_STRING);

        const char *stringEnd = deserialFindCString(buf + offset, DETAIL_END);
        if (NULL == stringEnd)
        {
            return false;
        }
        val = serializionDecode(buf + offset, stringEnd - buf - offset);
        return true;
    }
    std::string operator()(const char *buf) const
    {
        std::string res;
        this->operator ()(buf, res);
        return res;
    }
};

///\brief   list相关
template <class T> class Deserializion<std::list<T> >
{
public:
    bool operator()(const char *buf, std::list<T> &val) const
    {
        val.clear();
        if (deserialCStringMatch(buf, TAG_EMPTY_LIST))
        {
            return true;
        }

        if (!deserialCStringMatch(buf, TAG_LIST))
        {
            return false;
        }
        size_t offset = strlen(TAG_LIST);
        while (true)
        {
            size_t next = deserialGetDetailLength(buf + offset);
            if (next <= 1) break;
            T temp;
            Deserializion<T>()(buf + offset, temp);
            val.push_back(temp);
            offset += next + strlen(DETAIL_END);
        }

        return true;
    }
    std::list<T> operator()(const char *buf) const
    {
        std::list<T> res;
        this->operator ()(buf, res);
        return res;
    }
};

///\brief   反序列化class
template <class T> class DeserializionClass
{
public:
    bool operator()(MetaData *data, IReflection* val, const char* buf)
    {
        T temp;
        bool ret = Deserializion<T>()(buf, temp);
        data->setMetaData(val, &temp);
        return ret;
    }
};

///\brief   std::string的反序列化
template <> class Deserializion<IReflection*>
{
public:
    bool operator()(const char *buf, IReflection *&val) const
    {
        if (deserialCStringMatch(buf, TAG_EMPTY_CLASS))
        {
            val = NULL;
            return true;
        }

        /// 检测tag
        if (!deserialCStringMatch(buf, TAG_CLASS))
        {
            return false;
        }
        size_t offset = strlen(TAG_CLASS);

        /// 获取类名
        const char *className = buf + offset;
        const char *classNameOffset = deserialFindCString(buf + offset, TAG_END);
        if (classNameOffset == NULL)
        {
            return NULL;
        }
        *const_cast<char *>(classNameOffset) = 0;
        offset = classNameOffset - buf + strlen(TAG_END);

        /// 创建类
        CMetaFactory* s_meta = CMetaFactory::instance();
        val = s_meta->createClassByName(className);
        if (val == NULL)
        {
            return false;
        }

        /// 获取元表
        CMetaTable *metaTable = NULL;
        if (!s_meta->getMetaTable(className, metaTable))
        {
            delete val;
            val = NULL;
            return false;
        }

        /// 反射设置参数
        bool ret = true;
        while (ret)
        {
            size_t next = deserialGetDetailLength(buf + offset);
            if (next <= 1) break;
            size_t metaOffset = offset;
            offset += next + strlen(DETAIL_END);

            /// 获取成员名
            const char *metaName = buf + metaOffset;
            const char *nameOffset = deserialFindCString(metaName, TAG_PARAM_SPLIT);
            if (nameOffset == NULL)
            {
                /// 无法获取成员名
                ret = false;
                break;
            }
            *const_cast<char *>(nameOffset) = 0;
            metaOffset = nameOffset - buf + strlen(TAG_PARAM_SPLIT);

            /// 获取成员类型
            const char *metaType = buf + metaOffset;
            const char *typeOffset = deserialFindCString(metaType, TAG_END);
            if (typeOffset == NULL)
            {
                /// 无法获取成员类型
                ret = false;
                break;
            }
            std::string memberType(metaType, typeOffset - metaType);

            /// 获取元数据,校验与本地信息的差别，异常项略过
            MetaData *data = NULL;
            if (!metaTable->getMetaData(metaName, data))
            {
                /// 找不到类成员
                continue;
            }

            if (memberType.find(data->getMetaType()) != 0)
            {
                /// 成员类型改变
                continue;
            }

            /// 反序列化值
            ret &= getValue(data, val, buf + metaOffset);
        }
        if (!ret)
        {
            delete val;
            val = NULL;
        }
        return ret;
    }

    IReflection * operator()(const char *buf) const
    {
        IReflection *res = nullptr;
        this->operator ()(buf, res);
        return res;
    }

private:
    bool getValue(MetaData *data, IReflection* val, const char* buf) const
    {
        bool ret = true;
        /// 反序列化值
        switch (data->getMetaType())
        {
        case MetaDataTypeInt:
            ret &= DeserializionClass<int>()(data, val, buf);
            break;
        case MetaDataTypeLonglong:
            ret &= DeserializionClass<long long>()(data, val, buf);
            break;
        case MetaDataTypeDouble:
            ret &= DeserializionClass<double>()(data, val, buf);
            break;
        case MetaDataTypeString:
            ret &= DeserializionClass<std::string>()(data, val, buf);
            break;
        case MetaDataTypeClass:
            ret &= DeserializionClass<IReflection*>()(data, val, buf);
            break;
        case MetaDataTypeIntList:
            ret &= DeserializionClass<std::list<int> >()(data, val, buf);
            break;
        case MetaDataTypeLonglongList:
            ret &= DeserializionClass<std::list<long long> >()(data, val, buf);
            break;
        case MetaDataTypeDoubleList:
            ret &= DeserializionClass<std::list<double> >()(data, val, buf);
            break;
        case MetaDataTypeStringList:
            ret &= DeserializionClass<std::list<std::string> >()(data, val, buf);
            break;
        case MetaDataTypeClassList:
            ret &= DeserializionClass<std::list<IReflection*> >()(data, val, buf);
            break;
        case MetaDataTypeUnkown:
        default:
            ret = false;
            break;
        }

        return ret;
    }
};

}


#endif /* _LANGUAGE_STACK_DESERIALIZION_H_ */
