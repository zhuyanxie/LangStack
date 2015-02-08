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


#ifndef _LANGUAGE_STACK_DESERIAL_TYPE_H_
#define _LANGUAGE_STACK_DESERIAL_TYPE_H_

#include "Deserializion.h"

namespace ls {

MetaDataType deserialMetaType(const char *buf)
{
    if (deserialCStringMatch(buf, TAG_INT))
        return MetaDataTypeInt;
    else if (deserialCStringMatch(buf, TAG_LONGLONG))
        return MetaDataTypeLonglong;
    else if (deserialCStringMatch(buf, TAG_DOUBLE))
        return MetaDataTypeDouble;
    else if (deserialCStringMatch(buf, TAG_STRING))
        return MetaDataTypeString;
    else if (deserialCStringMatch(buf, TAG_CLASS))
        return MetaDataTypeClass;
    else if (deserialCStringMatch(buf, "List:Int:"))
        return MetaDataTypeIntList;
    else if (deserialCStringMatch(buf, "List:LLong:"))
        return MetaDataTypeLonglongList;
    else if (deserialCStringMatch(buf, "List:Double:"))
        return MetaDataTypeDoubleList;
    else if (deserialCStringMatch(buf, "List:String:"))
        return MetaDataTypeStringList;
    else if (deserialCStringMatch(buf, "List:Class:"))
        return MetaDataTypeClassList;
    return MetaDataTypeUnkown;
}

}


#endif /* _LANGUAGE_STACK_DESERIAL_TYPE_H_ */
