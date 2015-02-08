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


#ifndef _LANGUAGE_STACK_META_FACTORY_H_
#define _LANGUAGE_STACK_META_FACTORY_H_

#include <map>
#include "MetaTable.h"

namespace ls {

class CMetaFactory
{
public:
    static CMetaFactory* instance();

    ///\brief           注册类
    ///\param[in]       className       类名（禁止为NULL）
    ///\param[in]       creator         创建回调函数
    void registerClass(const char* className, MetaClassCreator creator);

    ///\briedf          注册方法
    ///\param[in]       className       类名（""为注册函数，禁止为NULL）
    ///\param[in]       methodName      方法名
    ///\param[in]       params          参数
    ///\param[in]       invoke          方法执行对象
    void registerMethod(const char* className, const char* methodName,
            const ParamTypes &params, void* invoke);

    ///\brief           注册成员
    ///\param[in]       className       类名（""为注册全局变量，禁止为NULL）
    ///\param[in]       memberName      成员名
    ///\param[in]       type            类型
    ///\param[in]       setCall         set回调函数
    ///\param[in]       getCall         get回调函数
    void registerMember(const char* className, const char* memberName,
            MetaDataType type, MetaDataSet setCall, MetaDataGet getCall);

    ///\brief           获取类元表
    ///\param[in]       className       类名（""则获取全局元表）
    ///\param[out]      table           类元表
    ///\return          true/false
    bool getMetaTable(const char* className, CMetaTable*& table);

    ///\brief           创建类
    ///\return          类对象指针
    IReflection* createClassByName(const char* className);

private:
    std::map<std::string, CMetaTable*>      m_metaTables;   ///< 类元信息

private:
    CMetaFactory();
    ~CMetaFactory();
};

}


#endif /* _LANGUAGE_STACK_META_FACTORY_H_ */
