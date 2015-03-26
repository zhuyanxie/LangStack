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

#ifndef _LANGUAGE_STACK_META_VALUE_H_
#define _LANGUAGE_STACK_META_VALUE_H_

#include <list>
#include <string>

#include "Reflect/MetadataTraits.h"
#include "Defs.h"

namespace ls {

struct MetaValue
{
    union
    {
        char                        charv;
        short                       shortv;
        int                         intv;
        long long                   llongv;
        double                      doublev;
        std::string*                stringv;
        IReflection*                classv;

        std::list<char>*            charlist;
        std::list<short>*           shortlist;
        std::list<int>*             intlist;
        std::list<long long>*       llonglist;
        std::list<double>*          doublelist;
        std::list<std::string>*     stringlist;
        std::list<IReflection*>*    classlist;
    }value;
    MetaDataType    type;

    MetaValue(const MetaValue &rhs) : value(rhs.value), type(rhs.type) {}

    MetaValue() :
        type(MetaDataTypeVoid)
    {
        value.llongv = 0;
    }
    MetaValue(char v) :
            type(MetaDataTypeChar)
    {
        value.charv = v;
    }
    MetaValue(short v) :
            type(MetaDataTypeShort)
    {
        value.shortv = v;
    }
    MetaValue(int v) :
            type(MetaDataTypeInt)
    {
        value.intv = v;
    }
    MetaValue(long long v) :
            type(MetaDataTypeLonglong)
    {
        value.llongv = v;
    }
    MetaValue(double v) :
            type(MetaDataTypeDouble)
    {
        value.doublev = v;
    }
    MetaValue(std::string* v) :
            type(MetaDataTypeString)
    {
        value.stringv = v;
    }
    MetaValue(const char* v) :
            type(MetaDataTypeString)
    {
        value.stringv = new std::string(v);
    }
    MetaValue(const char* v, int len) :
            type(MetaDataTypeMemory)
    {
        value.stringv = new std::string(v, len);
    }
    MetaValue(IReflection* v) :
            type(MetaDataTypeClass)
    {
        value.classv = v;
    }
    MetaValue(std::list<char>* v) :
        type(MetaDataTypeCharList)
    {
        value.charlist = v;
    }
    MetaValue(std::list<short>* v) :
        type(MetaDataTypeShortList)
    {
        value.shortlist = v;
    }
    MetaValue(std::list<int>* v) :
        type(MetaDataTypeIntList)
    {
        value.intlist = v;
    }
    MetaValue(std::list<long long>* v) :
            type(MetaDataTypeLonglongList)
    {
        value.llonglist = v;
    }
    MetaValue(std::list<double>* v) :
            type(MetaDataTypeDoubleList)
    {
        value.doublelist = v;
    }
    MetaValue(std::list<std::string>* v) :
            type(MetaDataTypeStringList)
    {
        value.stringlist = v;
    }
    MetaValue(std::list<IReflection*>* v) :
            type(MetaDataTypeClassList)
    {
        value.classlist = v;
    }
    ~MetaValue()
    {
    }

    operator char() const                       {return value.charv;}
    operator short() const                      {return value.shortv;}
    operator int() const
    {
        /// memory时候特殊处理
        if (MetaDataTypeMemory == type) {
            return (int)(*value.stringv).size();
        }
        return value.intv;
    }
    operator long long() const                  {return value.llongv;}
    operator double() const                     {return value.doublev;}
    operator std::string() const                {return *value.stringv;}
    operator IReflection*() const               {return value.classv;}
    operator std::list<char>() const            {return *value.charlist;}
    operator std::list<short>() const           {return *value.shortlist;}
    operator std::list<int>() const             {return *value.intlist;}
    operator std::list<long long>() const       {return *value.llonglist;}
    operator std::list<double>() const          {return *value.doublelist;}
    operator std::list<std::string>() const     {return *value.stringlist;}
    operator std::list<IReflection*>() const    {return *value.classlist;}

    const MetaValue& operator=(const MetaValue& rhs)
    {
        this->value     = rhs.value;
        this->type      = rhs.type;
        return *this;
    }

    ///\brief       回收内存用
    void release();
};

}


#endif /* _LANGUAGE_STACK_META_VALUE_H_ */
