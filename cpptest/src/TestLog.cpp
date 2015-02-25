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


#include "TestLog.h"

#include "Log/Log.h"

TEST_F(CTestLog, TestLoglevelFULL)
{
	setLangStackLogLevel(LogLevelFULL);
	VERBOSEF("Test", "VERBOSEF\n");
	DEBUGF("Test", "DEBUGF\n");
	INFOF("Test", "INFOF\n");
	WARNF("Test", "WARNF\n");
	ERRORF("Test", "ERRORF\n");
	FATALF("Test", "FATALF\n");

	setLangStackLogLevel(LogLevelFULL);
}
TEST_F(CTestLog, TestLoglevelVERBOSE)
{
	setLangStackLogLevel(LogLevelVERBOSE);
	VERBOSEF("Test", "VERBOSEF\n");
	DEBUGF("Test", "DEBUGF\n");
	INFOF("Test", "INFOF\n");
	WARNF("Test", "WARNF\n");
	ERRORF("Test", "ERRORF\n");
	FATALF("Test", "FATALF\n");

	setLangStackLogLevel(LogLevelFULL);
}
TEST_F(CTestLog, TestLogleveldebug)
{
	setLangStackLogLevel(LogLevelDEBUG);
	VERBOSEF("Test", "VERBOSEF\n");
	DEBUGF("Test", "DEBUGF\n");
	INFOF("Test", "INFOF\n");
	WARNF("Test", "WARNF\n");
	ERRORF("Test", "ERRORF\n");
	FATALF("Test", "FATALF\n");

	setLangStackLogLevel(LogLevelFULL);
}
TEST_F(CTestLog, TestLoglevelINFOF)
{
	setLangStackLogLevel(LogLevelINFO);
	VERBOSEF("Test", "VERBOSEF\n");
	DEBUGF("Test", "DEBUGF\n");
	INFOF("Test", "INFOF\n");
	WARNF("Test", "WARNF\n");
	ERRORF("Test", "ERRORF\n");
	FATALF("Test", "FATALF\n");

	setLangStackLogLevel(LogLevelFULL);
}
TEST_F(CTestLog, TestLoglevelWARN)
{
	setLangStackLogLevel(LogLevelWARN);
	VERBOSEF("Test", "VERBOSEF\n");
	DEBUGF("Test", "DEBUGF\n");
	INFOF("Test", "INFOF\n");
	WARNF("Test", "WARNF\n");
	ERRORF("Test", "ERRORF\n");
	FATALF("Test", "FATALF\n");

	setLangStackLogLevel(LogLevelFULL);
}
TEST_F(CTestLog, TestLoglevelERROR)
{
	setLangStackLogLevel(LogLevelERROR);
	VERBOSEF("Test", "VERBOSEF\n");
	DEBUGF("Test", "DEBUGF\n");
	INFOF("Test", "INFOF\n");
	WARNF("Test", "WARNF\n");
	ERRORF("Test", "ERRORF\n");
	FATALF("Test", "FATALF\n");
	setLangStackLogLevel(LogLevelFULL);
}
TEST_F(CTestLog, TestLoglevelFATAL)
{
	setLangStackLogLevel(LogLevelFATAL);
	VERBOSEF("Test", "VERBOSEF\n");
	DEBUGF("Test", "DEBUGF\n");
	INFOF("Test", "INFOF\n");
	WARNF("Test", "WARNF\n");
	ERRORF("Test", "ERRORF\n");
	FATALF("Test", "FATALF\n");

	setLangStackLogLevel(LogLevelFULL);
}
TEST_F(CTestLog, TestLoglevelFULLEND)
{
	VERBOSEF("Test", "VERBOSEF\n");
	DEBUGF("Test", "DEBUGF\n");
	INFOF("Test", "INFOF\n");
	WARNF("Test", "WARNF\n");
	ERRORF("Test", "ERRORF\n");
	FATALF("Test", "FATALF\n");
}

TEST_F(CTestLog, Dead)
{
}




