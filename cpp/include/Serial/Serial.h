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


#ifndef _LANGUAGE_STACK_SERIAL_H_
#define _LANGUAGE_STACK_SERIAL_H_

#include "Reflect/MetaValue.h"
#include "Serializion.h"
#include "Deserializion.h"

namespace ls {

///\brief   序列化函数
template<class T> bool serial(T obj, std::ostream &os)
{
    return Serializion<T>()(obj, os);
}

///\brief   memory序列化
bool LS_EXPORT serial(const char *buf, int len, std::ostream &os);

///\brief   反序列化函数
template<class T> bool deserial(const char *buf, T &val)
{
    return Deserializion<T>()(buf, val);
}

///\brief   内存反序列化
bool LS_EXPORT deserialMemory(const char *buf, MetaValue &val);

}

#endif /* SERIAL_H_ */
