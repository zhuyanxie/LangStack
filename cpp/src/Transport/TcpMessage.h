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

#ifndef _LANGUAGE_STACK_TCP_MESSAGE_H_
#define _LANGUAGE_STACK_TCP_MESSAGE_H_

#ifndef WIN32
#include <arpa/inet.h>
#else
#include <stdlib.h>
#endif

#include <inttypes.h>

#if defined(WIN32) || defined(__mac__) || defined(IOS)
#define __BYTE_ORDER __LITTLE_ENDIAN
#else
#include <endian.h>
#endif
#include <string>

#include "Rpc/RpcConstant.h"

namespace ls {

///\brief   RPC包头
struct RpcHeader
{
    uint32_t    flag;                       ///< RPC包标记
    uint16_t    type;                       ///< 包类型，见RpcType
    uint16_t    length;                     ///< 不包括包头的包数据长度
    uint16_t    checkSum;                   ///< 校验和(备用)
    uint16_t    reversed;                   ///< 备用字段(备用)
#if 1
    RpcHeader(RpcType ty, uint16_t len)
        : flag(0)
        , type(htons(uint16_t(ty)))
        , length(htons(uint16_t(len)))
        , checkSum(0)
        , reversed(0)
    {
        memcpy(&flag, RPC_MESSAGE_TAG, 4);
    }

    inline void setLength(uint16_t len) {length = htons(uint16_t(len));}

    inline uint16_t     getType()       {return ntohs(type);}
    inline uint16_t     getLength()     {return ntohs(length);}
    inline uint16_t     getCs()         {return ntohs(checkSum);}
    inline uint16_t     getReversed()   {return ntohs(reversed);}
#else

    RpcHeader(RpcType ty, uint16_t len)
        : flag(0)
        , type((uint16_t(ty)))
        , length((uint16_t(len)))
        , checkSum(0)
        , reversed(0)
    {
        memcpy(&flag, RPC_MESSAGE_TAG, 4);
    }

    inline void setLength(uint16_t len) { length = (uint16_t(len)); }

    inline uint16_t     getType()       { return (type); }
    inline uint16_t     getLength()     { return (length); }
    inline uint16_t     getCs()         { return (checkSum); }
    inline uint16_t     getReversed()   { return (reversed); }
#endif
};

}


#endif /* _LANGUAGE_STACK_TCP_MESSAGE_H_ */
