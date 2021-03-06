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

#include "TestSerial.h"

#include "Serial/Serial.h"
#include "Base/MemoryStream.h"
#include "EasyTypeClass.h"

TEST_F(CTestSerial, EasyClass)
{
    CEasyTypeClass easy;
    easy.random();
    std::ostringstream oss;
    ls::serial((ls::IReflection*)&easy, oss);
    std::string str = oss.str();

    ls::IReflection* p = nullptr;
    ls::deserial(str.c_str(), p);

    ASSERT_TRUE(!!dynamic_cast<CEasyTypeClass*>(p));
    CEasyTypeClass *easy2 = (CEasyTypeClass*)p;
    ASSERT_TRUE(easy == *easy2);
}

TEST_F(CTestSerial, Memory)
{
    char buf[2048];
    for (int i = 0; i < 2048; ++i) 
    {
        buf[i] = i % 256;
    }
    std::string str(buf, 2048);
    ASSERT_TRUE(str.size() == 2048);
    ASSERT_TRUE(memcmp(str.c_str(), buf, 2048) == 0);

    char buf2[4096];
    ls::MemoryOutputStream mos(buf2, 4096);
    ls::serial(str.c_str(), str.size(), mos);
    std::string str1(buf2, mos.charsWritten());

    ls::MetaValue val;
    ls::deserialMemory(str1.c_str(), val);

    std::string str2 = val;
    int len(val);
    ASSERT_TRUE(len == 2048);
    ASSERT_TRUE(memcmp(str2.c_str(), buf, 2048) == 0);
}

TEST_F(CTestSerial, Dead)
{

}



