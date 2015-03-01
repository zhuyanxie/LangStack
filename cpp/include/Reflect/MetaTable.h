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

#ifndef _LANGUAGE_STACK_META_TABLE_H_
#define _LANGUAGE_STACK_META_TABLE_H_

#include "Reflect/MetaData.h"
#include "Reflect/MetaMethod.h"
#include "Defs.h"

namespace ls {

class LS_EXPORT CMetaTable
{
public:
    ///\brief           默认构造
    CMetaTable();

    ///\brief           元表
    ///\param[in]       className       类名
    ///\param[in]       creator         创建回调
    CMetaTable(const char *className, MetaClassCreator creator = nullptr);
    ~CMetaTable();

    ///\brief           更新创建回调
    void updateCreator(MetaClassCreator creator);

    ///\brief           注册方法
    ///\param[in]       methodName      方法名
    ///\param[in]       params          参数
    ///\param[in]       invoke          方法执行对象
    void registerMethod(const char* methodName, const ParamTypes &params,
            void *invoke);

    ///\brief           获取所有同名重载方法
    ///\param[in]       methodName      方法名
    ///\param[out]      methods         所有重载方法元信息
    bool getMethods(const char* methodName, OverLoadMethods*& methods);

    ///\brief           注册成员
    ///\param[in]       memberName      成员名
    ///\param[in]       setCall         set回调函数
    ///\param[in]       getCall         get回调函数
    void registerMember(const char* memberName, MetaDataType type,
            MetaDataSet setCall, MetaDataGet getCall);

    ///\brief           获取类元表
    ///\param[in]       memberName      成员名
    ///\param[out]      data            数据元信息
    ///\return          true/false
    bool getMetaData(const char* memberName, MetaData*& data);

    ///\brief           获取数据元表
    MetaParamTable& getMetaTable();

    ///\brief           创建类
    IReflection* createClass();

private:
    std::string                     m_className;        ///< 类名
    MetaClassCreator                m_metaCreator;      ///< 元类型创建
    MetaParamTable                  m_members;          ///< 元参数数据表
    MetaMethods                     m_methods;          ///< 元方法表
};

}


#endif /* _LANGUAGE_STACK_META_TABLE_H_ */
