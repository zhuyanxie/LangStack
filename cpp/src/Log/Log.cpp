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

#include "Log/Log.h"
#include <stdarg.h>
#include <ctime>
#include <chrono>
#include <mutex>

using namespace std;
using namespace std::chrono;

static mutex 	s_lock;
static LogLevel s_globalLevel = LogLevel::LogLevelVERBOSE;

void setLangStackLogLevel(LogLevel limitLevel)
{
	s_globalLevel = limitLevel;
}

static const char* getFileName(const char *file)
{
	const char *p = file;
	for (; file && *file; ++file)
		if (*file == '/' || *file == '\\') p = file + 1;
	return p;
}

#ifdef android
#include <android/log.h>
void langstackLogPrintFull(LogLevel lev, const char *file, int line,
		const char *tag, const char *fmt, ...)
{
	if (lev >= s_globalLevel)
	{
		return;
	}

	char format[256];
	snprintf(format, sizeof(format), "<%s:%d>%s", getFileName(file), line, fmt);
    va_list args;
	va_start(args, fmt);

	char buf[256*1024];
	vsnprintf(buf, sizeof(buf), format, args);
	switch (lev)
	{
	case LogLevelFATAL:
		__android_log_print(ANDROID_LOG_FATAL, tag, "%s", buf);
		break;
	case LogLevelERROR:
		__android_log_print(ANDROID_LOG_ERROR, tag, "%s", buf);
		break;
	case LogLevelWARN:
		__android_log_print(ANDROID_LOG_WARN, tag, "%s", buf);
		break;
	case LogLevelINFO:
		__android_log_print(ANDROID_LOG_INFO, tag, "%s", buf);
		break;
	case LogLevelDEBUG:
		__android_log_print(ANDROID_LOG_DEBUG, tag, "%s", buf);
	case LogLevelVERBOSE:
		__android_log_print(ANDROID_LOG_VERBOSE, tag, "%s", buf);
		break;
	default:
		break;
	}
	va_end(args);
}
#else

static const char *s_logLevelStrings[] =
	{"FATAL", "ERROR", "WARNF", "INFOF", "DEBUG", "VERBOSE"};

void langstackLogPrintFull(LogLevel lev, const char *file, int line,
		const char *tag, const char *fmt, ...)
{
	if (lev >= s_globalLevel)
	{
		return;
	}

	auto now = system_clock::to_time_t(system_clock::now());
	tm *date = std::localtime(&now);
    va_list args;
	va_start(args, fmt);
	s_lock.lock();
	printf("%s|%s|%02d-%02d %02d:%02d:%02d<%s:%d>",
			tag, s_logLevelStrings[lev], date->tm_mon, date->tm_mday,
			date->tm_hour, date->tm_min, date->tm_sec, getFileName(file), line);
	vfprintf(stdout, fmt, args);
	s_lock.unlock();
	va_end(args);
	fflush(stdout);
}
#endif

