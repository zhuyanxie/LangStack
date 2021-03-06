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


#ifndef _LANGUAGE_STACK_META_DATA_H_
#define _LANGUAGE_STACK_META_DATA_H_

#include <map>
#include <string>

#include "Reflect/MetaDataTraits.h"
#include "Serial/SerializationConstant.h"
#include "Defs.h"

namespace ls {

///\brief   元表创建回调
typedef IReflection* (*MetaClassCreator)(void);

///\brief   元数据设置回调
typedef void (*MetaDataSet)(IReflection *obj, void *val);

///\brief   元数据获取回调
typedef void* (*MetaDataGet)(IReflection *obj);

class LS_EXPORT MetaData
{
public:
    ///\brief                   构造
    ///\param[in]   type        元数据类型
    ///\param[in]   metaName    元数据名称
    ///\param[in]   setCall     元数据设置回调
    ///\param[in]   getCall     元数据获取回调
    MetaData(MetaDataType type, const char *metaName, MetaDataSet setCall,
            MetaDataGet getCall);

    ~MetaData();

    ///\brief                   设置元数据值
    ///\param[in]       obj     对象地址
    ///\param[in]       value   值
    void setMetaData(IReflection *obj, void *val);

    ///\brief                   获取元数据值
    ///\param[in]       obj     对象地址
    ///\return          值
    void* getMetaData(IReflection *obj);

    ///\brief                   获取元类型
    std::string getMetaTypeString();

    ///\brief					inline gets
    inline MetaDataSet getSetCall() { return m_setCall; }
    inline MetaDataGet getGetCall() { return m_getCall; }
    inline std::string getMetaName() { return m_metaName; }
    inline MetaDataType getMetaType() { return m_metaType; }

private:
    MetaDataType    m_metaType;         ///< 元数据类型
    std::string     m_metaName;         ///< 元数据描述
    MetaDataSet     m_setCall;          ///< 元数据设置回调
    MetaDataGet     m_getCall;          ///< 元数据获取回调
};

typedef std::map<std::string, MetaData*>    MetaParamTable; ///< 元参数表

}


#endif /* _LANGUAGE_STACK_META_DATA_H_ */
