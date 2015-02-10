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

#include "TestReflect.h"

#include <string>
#include <sstream>
#include <iostream>

#include "Serial/Serial.h"

#include "EasyTypeClass.h"

/// 类创建反射
TEST_F(CTestSerial, EasyClassReflect)
{
    std::stringstream ss;

    CMetaTable *table = nullptr;
    CMetaFactory::instance()->getMetaTable("CEasyTypeClass", table);
    auto foo = CMetaFactory::instance()->createClassByName("CEasyTypeClass");
    auto etc = dynamic_cast<CEasyTypeClass*>(foo);
    ASSERT_TRUE(etc != nullptr);
    ASSERT_DOUBLE_EQ(etc->foo(1), 0.01);

    delete foo;
}

/// 成员反射
TEST_F(CTestSerial, EasyMemberReflect)
{
    std::stringstream ss;

    CMetaTable *table = nullptr;
    CMetaFactory::instance()->getMetaTable("CEasyTypeClass", table);
    auto foo = CMetaFactory::instance()->createClassByName("CEasyTypeClass");
    auto etc = dynamic_cast<CEasyTypeClass*>(foo);
    ASSERT_TRUE(etc != nullptr);
    ASSERT_DOUBLE_EQ(etc->foo(1), 0.01);

    delete foo;
}

/// 最直接的函数反射调用
TEST_F(CTestSerial, EasyFunctionReflectTest)
{
    CEasyTypeClass easy;
    easy.m_int = 1024;
    easy.m_longlong = 10101010;
    easy.m_double = 10.10;
    easy.m_normal = "10.10.10.10";
    easy.m_special = "!@#$%^&*()_+-=[]\\;',./`~<>?:\"{}|::;;%0%1%%";
    easy.dump();

    /// 类
    CMetaTable *table = nullptr;
    CMetaFactory::instance()->getMetaTable("CEasyTypeClass", table);

    /// 成员方法 dump
    OverLoadMethods *methods = nullptr;
    table->getMethods("dump", methods);
    ReflectFunciton<void> *dump = (ReflectFunciton<void>*)(*methods)[0].method;
    dump->bind(&easy);
    dump->invoke();

    /// 静态方法 foo
    methods = nullptr;
    table->getMethods("foo", methods);
    ReflectFunciton<double> *foo = (ReflectFunciton<double>*)(*methods)[0].method;
    std::cout << foo->invoke(1) << std::endl;

    /// 全局函数 foo
    CMetaFactory::instance()->getMetaTable("", table);
    table->getMethods("foo", methods);
    ReflectFunciton<std::list<int> > *gfoo = (ReflectFunciton<std::list<int> >*)(*methods)[0].method;
    std::list<double> ld = {0.1, 0.2, 0.3};
    for (auto i : gfoo->invoke(&ld, &easy))
        std::cout << "\t\t\t\t i: " << i << std::endl;

}

TEST_F(CTestSerial, Dead)
{
}

// The following lines pull in the real gtest *.cc files.
#include "src/gtest.cc"
#include "src/gtest-death-test.cc"
#include "src/gtest-filepath.cc"
#include "src/gtest-port.cc"
#include "src/gtest-printers.cc"
#include "src/gtest-test-part.cc"
#include "src/gtest-typed-test.cc"

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();

    char inputchar;
    while ((inputchar = getchar()) != 'q')
    {
        break;
    }

    return 0;
}