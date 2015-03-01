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

#include "Reflect/MetaData.h"

namespace ls {

///\brief                   构造
///\param[in]   type        元数据类型
///\param[in]   metaName    元数据名称
///\param[in]   setCall     元数据设置回调
///\param[in]   getCall     元数据获取回调
MetaData::MetaData(MetaDataType type, const char *metaName, MetaDataSet setCall,
        MetaDataGet getCall)
    : m_metaType(type)
    , m_metaName(metaName)
    , m_setCall(setCall)
    , m_getCall(getCall)
{
}

MetaData::~MetaData()
{
}

///\brief                   设置元数据值
///\param[in]       obj     对象地址
///\param[in]       value   值
void MetaData::setMetaData(IReflection *obj, void *val)
{
    return m_setCall(obj, val);
}

///\brief                   获取元数据值
///\param[in]       obj     对象地址
///\return          值
void* MetaData::getMetaData(IReflection *obj)
{
    return m_getCall(obj);
}

///\brief                   获取元类型
std::string MetaData::getMetaTypeString()
{
    return META_TYPE_STRING[m_metaType];
}

}


