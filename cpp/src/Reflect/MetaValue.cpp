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

#include "Reflect/MetaValue.h"
#include "Reflect/IReflection.h"

namespace ls {

void MetaValue::release()
{
    switch(type)
    {
    case MetaDataTypeString:
    case MetaDataTypeMemory:
        if (nullptr != value.stringv)
        {
            delete value.stringv;
            value.stringv = nullptr;
        }
        break;
    case MetaDataTypeClass:
        if (nullptr != value.classv)
        {
            delete value.classv;
            value.classv = nullptr;
        }
        break;
    case MetaDataTypeIntList:
        if (nullptr != value.intlist)
        {
            delete value.intlist;
            value.intlist = nullptr;
        }
        break;
    case MetaDataTypeLonglongList:
        if (nullptr != value.llonglist)
        {
            delete value.llonglist;
            value.llonglist = nullptr;
        }
        break;
    case MetaDataTypeDoubleList:
        if (nullptr != value.doublelist)
        {
            delete value.doublelist;
            value.doublelist = nullptr;
        }
        break;
    case MetaDataTypeStringList:
        if (nullptr != value.stringlist)
        {
            delete value.stringlist;
            value.stringlist = nullptr;
        }
        break;
    case MetaDataTypeClassList:
        if (nullptr != value.classlist)
        {
            for (auto cls : *(value.classlist))
            {
                delete cls;
            }
            delete value.classlist;
            value.classlist = nullptr;
        }
        break;
    default:
        break;
    }
}

}

