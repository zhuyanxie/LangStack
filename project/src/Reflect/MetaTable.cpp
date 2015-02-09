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


#include "MetaTable.h"

namespace ls {

///\brief           默认构造
CMetaTable::CMetaTable()
    : m_className("")
    , m_metaCreator(NULL)
{
}

///\brief           元表
CMetaTable::CMetaTable(const char *className, MetaClassCreator creator)
    : m_className(className)
    , m_metaCreator(creator)
{
}

CMetaTable::~CMetaTable()
{
    MetaParamTable::iterator it = m_members.begin();
    for (; it != m_members.end(); ++it)
    {
        delete it->second;
    }
    m_members.clear();
}

///\brief           更新创建回调
void CMetaTable::updateCreator(MetaClassCreator creator)
{
    m_metaCreator = creator;
}

///\briedf          注册方法
void CMetaTable::registerMethod(const char* methodName,
        const ParamTypes &params, void *invoke)
{
    if (!m_methods.count(methodName))
    {
        m_methods[methodName] = new CMetaMethod(methodName);
    }
    m_methods[methodName]->addMethod(params, invoke);
}


///\brief           获取所有同名重载方法
bool CMetaTable::getMethods(const char* methodName, OverLoadMethods*& methods)
{
    if (!m_methods.count(methodName))
    {
        return false;
    }

    methods = m_methods[methodName]->getMethods();
    return true;
}

///\brief           注册变量
void CMetaTable::registerMember(const char* memberName, MetaDataType type,
        MetaDataSet setCall, MetaDataGet getCall)
{
    MetaParamTable::iterator it = m_members.find(memberName);

    if (it != m_members.end())
    {
        delete it->second;
    }

    m_members[memberName] = new MetaData(type, memberName, setCall, getCall);
}

///\brief           获取类元表
bool CMetaTable::getMetaData(const char* memberName, MetaData*& data)
{
    MetaParamTable::iterator it = m_members.find(memberName);

    if (it == m_members.end())
    {
        return false;
    }

    data = it->second;
    return true;
}

///\brief           获取数据元表
MetaParamTable& CMetaTable::getMetaTable()
{
    return m_members;
}

///\brief           创建类
IReflection* CMetaTable::createClass()
{
    return m_metaCreator();
}

}
