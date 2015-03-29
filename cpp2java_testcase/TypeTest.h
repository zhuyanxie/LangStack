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


#ifndef TYPETEST_H_
#define TYPETEST_H_

namespace ls {
namespace test {

/**
 * 没有指定javaname，默认生成的com.ls.test.TypeTest
 * 规则： com + [namespace...] + classname
 */
struct TypeTest
{
    char                    m_char;
    short                   m_short;
    int                     m_int;
    long long               m_longlong;
    double                  m_double;
    std::string             m_string;

    std::list<char>         m_i8l;
    std::list<short>        m_i16l;
    std::list<int>          m_i32l;
    std::list<long long>    m_i64l;
    std::list<double>       m_dl;
    std::list<std::string>  m_sl;

    TypeTest();
    TypeTest(int seed);
    ~TypeTest();
};

}
}


#endif /* TYPETEST_H_ */
