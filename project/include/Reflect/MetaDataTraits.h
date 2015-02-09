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


#ifndef _LANGUAGE_STACK_METADATA_TRAITS_H_
#define _LANGUAGE_STACK_METADATA_TRAITS_H_

#include <list>
#include <vector>
#include <string>

#include <type_traits>

namespace ls {

class IReflection;

///\brief   元数据类型
enum MetaDataType
{
    MetaDataTypeVoid,           ///< void类型，仅用于表示参数类型

    MetaDataTypeInt,            ///< int
    MetaDataTypeLonglong,       ///< long long
    MetaDataTypeDouble,         ///< double
    MetaDataTypeString,         ///< std::string
    MetaDataTypeClass,          ///< IReflection*

    MetaDataTypeIntList,        ///< std::list<int>
    MetaDataTypeLonglongList,   ///< std::list<long long>
    MetaDataTypeDoubleList,     ///< std::list<double>
    MetaDataTypeStringList,     ///< std::list<std::string>
    MetaDataTypeClassList,      ///< std::list<IReflection*>

    MetaDataTypeUnkown,
};

///\brief   实际类型转换为元数据类型
template <class T> struct Type2MetaDataType
{
    MetaDataType operator()() const
    {
        return MetaDataTypeUnkown;
    }
};

template <> struct Type2MetaDataType<void>
{
    MetaDataType operator()() const { return MetaDataTypeVoid; }
};

template <> struct Type2MetaDataType<int>
{
    MetaDataType operator()() const { return MetaDataTypeInt; }
};

template <> struct Type2MetaDataType<long long>
{
    MetaDataType operator()() const { return MetaDataTypeLonglong; }
};

template <> struct Type2MetaDataType<double>
{
    MetaDataType operator()() const { return MetaDataTypeDouble; }
};

template <> struct Type2MetaDataType<std::string>
{
    MetaDataType operator()() const { return MetaDataTypeString; }
};

template <> struct Type2MetaDataType<IReflection*>
{
    MetaDataType operator()() const { return MetaDataTypeClass; }
};

template <> struct Type2MetaDataType<std::list<int> >
{
    MetaDataType operator()() const { return MetaDataTypeIntList; }
};

template <> struct Type2MetaDataType<std::list<long long> >
{
    MetaDataType operator()() const { return MetaDataTypeLonglongList; }
};

template <> struct Type2MetaDataType<std::list<double> >
{
    MetaDataType operator()() const { return MetaDataTypeDoubleList; }
};

template <> struct Type2MetaDataType<std::list<std::string> >
{
    MetaDataType operator()() const { return MetaDataTypeStringList; }
};

template <> struct Type2MetaDataType<std::list<IReflection*> >
{
    MetaDataType operator()() const { return MetaDataTypeClassList; }
};

///\brief		获取参数数据类型集合
template<class R>
static std::vector<MetaDataType> types2MetaDataTypes()
{
	std::vector<MetaDataType> types;
	types.push_back(Type2MetaDataType<R>()());
	return types;
}

template<class R, class... Args>
static std::vector<MetaDataType> types2MetaDataTypes()
{
	std::vector<MetaDataType> types;
	types.push_back(Type2MetaDataType<R>()());
	return types;
}

///\brief       获取数据类型
template<MetaDataType T> struct MetaType2RealType
{
    typedef int real_type;
};
template <> struct MetaType2RealType<MetaDataTypeInt>
{
    typedef int real_type;
};
template <> struct MetaType2RealType<MetaDataTypeLonglong>
{
    typedef long long real_type;
};
template <> struct MetaType2RealType<MetaDataTypeDouble>
{
    typedef double real_type;
};
template <> struct MetaType2RealType<MetaDataTypeString>
{
    typedef std::string real_type;
};
template <> struct MetaType2RealType<MetaDataTypeClass>
{
    typedef IReflection* real_type;
};
template <> struct MetaType2RealType<MetaDataTypeIntList>
{
    typedef std::list<int> real_type;
};
template <> struct MetaType2RealType<MetaDataTypeLonglongList>
{
    typedef std::list<long long> real_type;
};
template <> struct MetaType2RealType<MetaDataTypeDoubleList>
{
    typedef std::list<double> real_type;
};
template <> struct MetaType2RealType<MetaDataTypeStringList>
{
    typedef std::list<std::string> real_type;
};
template <> struct MetaType2RealType<MetaDataTypeClassList>
{
    typedef std::list<IReflection*> real_type;
};



}


#endif /* METADATATRAITS_H_ */
