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

#ifndef _LANGUAGE_STACK_SERIALIZATION_CONSTANT_H_
#define _LANGUAGE_STACK_SERIALIZATION_CONSTANT_H_

#include <string>
#include "Reflect/MetaDataTraits.h"

namespace ls {

extern const char TAG_INT[];									///< Int:
extern const char TAG_LONGLONG[];								///< LLong:
extern const char TAG_DOUBLE[];									///< Double:
extern const char TAG_STRING[];									///< String:
extern const char TAG_LIST[];									///< List:
extern const char TAG_CLASS[];									///< Class
extern const char TAG_END[];									///< :
extern const char TAG_PARAM_SPLIT[];							///< &
extern const char DETAIL_END[];                                 ///< ;

extern const char TAG_EMPTY_CLASS[];                            ///< NULL
extern const char TAG_EMPTY_LIST[];                             ///< 空List

extern const std::string META_TYPE_STRING[];		            ///< 元数据字符串描述

}


#endif /* _LANGUAGE_STACK_SERIALIZATION_CONSTANT_H_ */
