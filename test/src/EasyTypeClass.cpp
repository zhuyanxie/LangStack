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

#include "EasyTypeClass.h"

#include <iostream>

CEasyTypeClass::CEasyTypeClass() : m_int(0), m_longlong(0), m_double(0) {}
CEasyTypeClass::~CEasyTypeClass() {}

void CEasyTypeClass::dump()
{
    std::cout << "\tm_int:\t\t\t" << m_int << std::endl;
    std::cout << "\tm_longlong:\t\t" << m_longlong << std::endl;
    std::cout << "\tm_double:\t\t" << m_double << std::endl;
    std::cout << "\tm_normal:\t\t" << m_normal << std::endl;
    std::cout << "\tm_special:\t\t" << m_special << std::endl;
}

double CEasyTypeClass::foo(int arg)
{
    std::cout << "\tfoo:i" << std::endl;
    return 0.01;
}

bool CEasyTypeClass::operator==(const CEasyTypeClass &rhs) const
{
    return m_int        == rhs.m_int &&
            m_longlong  == rhs.m_longlong &&
            m_double    == rhs.m_double &&
            m_normal    == rhs.m_normal &&
            m_special   == rhs.m_special;
}

std::list<int> foo(std::list<double> dl, IReflection* obj)
{
    CEasyTypeClass *easy = dynamic_cast<CEasyTypeClass*>(obj);
    easy->dump();
    for (auto d : dl) std::cout << "\tdouble:\t\t" << d << std::endl;
    return { 1, 2, 3 };
}
