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

#include "Serial/Serial.h"

namespace ls {

bool serial(const char *buf, int len, std::ostream &os)
{
    os << TAG_MEMORY << len << TAG_END;
    os.write(buf, len);
    return true;
}

bool deserialMemory(const char *buf, MetaValue &val)
{
    if (*buf && *buf == TAG_MEMORY[0])
    {
        /// memory 使用tlv格式
        int len = 0;
        const char* tag = strstr(buf, ":");
        std::string length = std::string(buf + 1, tag);
        sscanf(length.c_str(), "%d", &len);
        val =  MetaValue(tag + 1, len);
        return true;
    }

    return false;
}

}



