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

#include <ctime>
#include <sstream>
#include "Log/Log.h"


CEasyTypeClass::CEasyTypeClass() : m_char(0), m_short(0), m_int(0), m_longlong(0), m_double(0) {}
CEasyTypeClass::~CEasyTypeClass() {}

void CEasyTypeClass::dump()
{
	DEBUGF("test", "CEasyTypeClass dump called\n");
    VERBOSEF("test", "\tm_char: \t\t\t[%d]\n", m_char);
    VERBOSEF("test", "\tm_short: \t\t\t[%d]\n", m_short);
    VERBOSEF("test", "\tm_int: \t\t\t[%d]\n", m_int);
	VERBOSEF("test", "\tm_longlong:\t\t[%lld]\n", m_longlong);
	VERBOSEF("test", "\tm_double:\t\t[%lf]\n", m_double);
	VERBOSEF("test", "\tm_normal:\t\t[%s]\n", m_normal.c_str());
	VERBOSEF("test", "\tm_special:\t\t[%s]\n", m_special.c_str());
}

/// 随机内部值
void CEasyTypeClass::random()
{
    srand(time(NULL));
    m_char      = rand() % 256;
    m_short     = rand() % 65536;
    m_int       = ((int)rand() << 16) | (int) rand();
    m_longlong  = ((long long)rand() << 48) | ((long long)rand() << 32) |
                  ((long long)rand() << 16) | ((long long)rand());
    m_double    = rand() % 65536 * 0.1;

    int key = rand();
    std::ostringstream oss;
    oss << key << " " << key;
    m_normal = oss.str();

    oss << ":;?$%^**UI(*&  ()*@)(#*)@*#";
    m_special = oss.str();
}

double CEasyTypeClass::foo(int arg)
{
	DEBUGF("test", "CEasyTypeClass foo called\n");
    return 0.01;
}

bool CEasyTypeClass::operator==(const CEasyTypeClass &rhs) const
{
    return m_char       == rhs.m_char &&
            m_short     == rhs.m_short &&
            m_int       == rhs.m_int &&
            m_longlong  == rhs.m_longlong &&
            m_normal    == rhs.m_normal &&
            m_special   == rhs.m_special;
}

std::list<int> foo(std::list<double> dl, IReflection* obj)
{
	DEBUGF("test", "global foo called\n");

    CEasyTypeClass *easy = dynamic_cast<CEasyTypeClass*>(obj);
    easy->dump();
    return { 1, 2, 3 };
}
