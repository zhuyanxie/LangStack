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

#include "Log/Log.h"


CEasyTypeClass::CEasyTypeClass() : m_int(0), m_longlong(0), m_double(0) {}
CEasyTypeClass::~CEasyTypeClass() {}

void CEasyTypeClass::dump()
{
	DEBUGF("test", "CEasyTypeClass dump called\n");
	VERBOSEF("test", "\tm_int: \t\t\t[%d]\n", m_int);
	VERBOSEF("test", "\tm_longlong:\t\t[%lld]\n", m_longlong);
	VERBOSEF("test", "\tm_double:\t\t[%lf]\n", m_double);
	VERBOSEF("test", "\tm_normal:\t\t[%s]\n", m_normal.c_str());
	VERBOSEF("test", "\tm_special:\t\t[%s]\n", m_special.c_str());
}

double CEasyTypeClass::foo(int arg)
{
	DEBUGF("test", "CEasyTypeClass foo called\n");
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
	DEBUGF("test", "global foo called\n");

    CEasyTypeClass *easy = dynamic_cast<CEasyTypeClass*>(obj);
    easy->dump();
    return { 1, 2, 3 };
}
