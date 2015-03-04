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

#ifndef ICALLBACKTEST_H_
#define ICALLBACKTEST_H_

#include <inttypes.h>
#include <string>
#include <iostream>

#include "Reflect/IReflection.h"
#include "Rpc/IRpcApi.h"

using namespace ls;

class DEMO_EXPORT ICallBack : public IReflection
{
public:
    virtual ~ICallBack() {}
    virtual void interface1() = 0;

    virtual int interface2(int) = 0;

    virtual double interface3(std::list<long long>, IReflection *) = 0;

    virtual std::string interface4() = 0;

};

///\brief       回调代理
class DEMO_EXPORT CCallBackProxy : ICallBack
{
public:
    CCallBackProxy(){}
    virtual ~CCallBackProxy(){}
    virtual void interface1()
    {
        callRemoteInterface<void>("interface1", getClassName(), this);
    }
    REFLECTION_METHOD_DECLEAR_ARGS0(void, interface1)

    virtual int interface2(int t0)
    {
        return callRemoteInterface<int>(t0, "interface2", getClassName(), this);
    }
    REFLECTION_METHOD_DECLEAR_ARGS1(int, interface2)

    virtual double interface3(std::list<long long> t0, IReflection *t1)
    {
        return callRemoteInterface<int>(t0, t1, "interface3", getClassName(), this);
    }
    REFLECTION_METHOD_DECLEAR_ARGS2(double, interface3)

    virtual std::string interface4()
    {
        return callRemoteInterface<std::string>("interface4", getClassName(), this);
    }
    REFLECTION_METHOD_DECLEAR_ARGS0(std::string, interface4)

private:
    REFLECTION_CLASS_DECLEAR(CCallBackProxy, "com.LangStackTest.RealCallback");
};
REFLECTION_CLASS_IMPLEMENT(CCallBackProxy);
REFLECTION_METHOD_ARGS0(CCallBackProxy, interface1, void);
REFLECTION_METHOD_ARGS1(CCallBackProxy, interface2, int, int);
REFLECTION_METHOD_ARGS2(CCallBackProxy, interface3, double, std::list<long long>, IReflection*);
REFLECTION_METHOD_ARGS0(CCallBackProxy, interface4, std::string);

#endif /* ICALLBACKTEST_H_ */
