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

#include "Reflect/MetaFactory.h"
#include "Log/Log.h"
#include "LangStackConstant.h"

namespace ls {

CMetaFactory* CMetaFactory::instance()
{
    static CMetaFactory s_metaFactory;
    return &s_metaFactory;
}

///\brief           注册类
void CMetaFactory::registerClass(const char* className,
        MetaClassCreator creator)
{
    if (m_metaTables.count(className))
    {
    	WARNF(LS_TAG, "registerClass overwrite class [%s]\n", className);
        m_metaTables[className]->updateCreator(creator);
    }
    else
    {
        m_metaTables[className] = new CMetaTable(className, creator);
    }
}

///\briedf          注册方法
void CMetaFactory::registerMethod(const char* className, const char* methodName,
        const ParamTypes &params, void* invoke)
{
    if (!m_metaTables.count(className))
    {
    	WARNF(LS_TAG, "registerMethod hava no class [%s]\n", className);
        m_metaTables[className] = new CMetaTable(className);
    }

    m_metaTables[className]->registerMethod(methodName, params, invoke);
}

///\brief           注册成员
void CMetaFactory::registerMember(const char* className,
        const char* memberName, MetaDataType type,
        MetaDataSet setCall, MetaDataGet getCall)
{
    if (!m_metaTables.count(className))
    {
    	WARNF(LS_TAG, "registerMember hava no class [%s]\n", className);
        m_metaTables[className] = new CMetaTable(className);
    }

    m_metaTables[className]->registerMember(memberName, type, setCall, getCall);
}

///\brief           获取类元表
bool CMetaFactory::getMetaTable(const char* className, CMetaTable*& table)
{
    std::map<std::string, CMetaTable*>::iterator it =
            m_metaTables.find(className);

    if (it == m_metaTables.end())
    {
    	ERRORF(LS_TAG, "can't find class [%s]\n", className);
        return false;
    }

    table = it->second;
    return true;
}

///\brief           创建类
IReflection* CMetaFactory::createClassByName(const char* className)
{
    CMetaTable *table = NULL;
    if (!getMetaTable(className, table))
    {
        return NULL;
    }

    return table->createClass();
}

CMetaFactory::CMetaFactory()
{
}

CMetaFactory::~CMetaFactory()
{
    std::map<std::string, CMetaTable*>::iterator it = m_metaTables.begin();
    for (; it != m_metaTables.end(); ++it)
    {
        delete it->second;
    }
    m_metaTables.clear();
}

}


