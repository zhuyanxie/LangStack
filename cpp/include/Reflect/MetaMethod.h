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

#ifndef _LANGUAGE_STACK_META_METHOD_H_
#define _LANGUAGE_STACK_META_METHOD_H_

#include <inttypes.h>

#include <map>
#include <vector>

#include "Reflect/MetaDataTraits.h"
#include "Defs.h"

namespace ls {

/**
 * note:    首个参数类型为返回类型， 反射方法目前不支持出参
 *
 * eg:
 *  参数列表类型： R, T1, T2, T3, T4(五个参数类型， R为返回类型)
 */
typedef std::vector<MetaDataType>               ParamTypes;     ///< 方法参数类型列表

///\brief       方法元信息
struct MetaMethodData
{
    ParamTypes              params;                             ///< 参数类型列表
    size_t                  paramsCount;                        ///< 参数个数
    void*                   method;                             ///< 执行function

    MetaMethodData(const ParamTypes &p, void* m)
        : params(p)
        , paramsCount(p.size()-1)
        , method(m)
    {
    }
};
typedef std::vector<MetaMethodData>         OverLoadMethods;    ///< 同名重载方法列表


///\brief       方法元
class CMetaMethod
{
public:
    CMetaMethod(const char *methodName);
    ~CMetaMethod();

    ///\brief       增加方法
    ///\param[in]   data        参数类型列表
    ///\param[in]   invoke      方法执行对象
    void addMethod(const ParamTypes &data, void* invoke);

    ///\brief       获取所有同名的方法
    OverLoadMethods* getMethods();

private:
    std::string             m_name;                             ///< 方法名
    OverLoadMethods         m_method;                           ///< 方法元信息
};
typedef std::map<std::string, CMetaMethod*>      MetaMethods;   ///< 方法元列表

}


#endif /* _LANGUAGE_STACK_META_METHOD_H_ */
