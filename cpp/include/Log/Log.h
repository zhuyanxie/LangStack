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

#ifndef _LANGUAGE_STACK_LOG_H_
#define _LANGUAGE_STACK_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	LogLevelFATAl,		///< 不输出任何日志
	LogLevelERROR,		///< 只输出fatal等级日志
	LogLevelWARN,		///< 输出error以上等级日志
	LogLevelINFO,		///< 输出info以上等级日志
	LogLevelDEBUG,		///< 输出debug以上等级日志
	LogLevelVERBOSE, 	///< verbose以上等级(默认)
	LogLevelFULL,		///< 所有等级日志
} LogLevel;

///\brief		设置日志输出等级
void setLangStackLogLevel(LogLevel limitLevel);

///\brief		不直接调用
void langstackLogPrintFull(LogLevel lev, const char *file, int line,
		const char *tag, const char *fmt, ...);


#define VERBOSEF(TAG, FORMAT, ...) \
		langstackLogPrintFull(LogLevelVERBOSE, __FILE__, __LINE__, TAG, FORMAT, ##__VA_ARGS__)
#define DEBUGF(TAG, FORMAT, ...) \
		langstackLogPrintFull(LogLevelDEBUG, __FILE__, __LINE__,  TAG, FORMAT, ##__VA_ARGS__)
#define INFOF(TAG, FORMAT, ...) \
		langstackLogPrintFull(LogLevelINFO, __FILE__, __LINE__,  TAG, FORMAT, ##__VA_ARGS__)
#define WARNF(TAG, FORMAT, ...) \
		langstackLogPrintFull(LogLevelWARN, __FILE__, __LINE__,  TAG, FORMAT, ##__VA_ARGS__)
#define ERRORF(TAG, FORMAT, ...) \
		langstackLogPrintFull(LogLevelERROR, __FILE__, __LINE__,  TAG, FORMAT, ##__VA_ARGS__)
#define FATALF(TAG, FORMAT, ...) \
		langstackLogPrintFull(LogLevelFATAl, __FILE__, __LINE__,  TAG, FORMAT, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* _LANGUAGE_STACK_LOG_H_ */
