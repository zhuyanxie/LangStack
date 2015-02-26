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

#include "TestTask.h"
#include <thread>
#include "Task/TaskThreadPool.h"
#include "Log/Log.h"

static int g_delayTaskId = 0;
class CDelayTask : public ls::ITask
{
public:
	CDelayTask(int id)
	{
		m_id = (id == 0) ? g_delayTaskId++ : id;
    	char buf[1024];
    	snprintf(buf, 1024, "thread:%d", m_id);
    	m_sid = buf;
	}

	virtual ~CDelayTask()
	{
	}


    virtual void execute()
    {
        //WARNF("Delay", "[%ld]excute delay task [%s]\n", 
        //    std::hash<std::thread::id>()(std::this_thread::get_id()), m_sid.c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    virtual std::string getTaskId()
    {
    	return m_sid;
    }

private:
    int m_id;
    std::string m_sid;
};


TEST_F(CTestTask, testSameThreadId)
{
	ls::CTaskThreadPool::instance()->setThreadIdleTime(5000);
	ASSERT_EQ(ls::CTaskThreadPool::instance()->getThreadCount(), 4);
    ERRORF("TEST", "\n");
	for (int i = 0; i < 28; ++i)
	{
		ls::CTaskThreadPool::instance()->addTask(new CDelayTask((i%4) + 1));
	}
	ASSERT_EQ(ls::CTaskThreadPool::instance()->getThreadCount(), 4);
    ERRORF("TEST", "\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(15 * 1000));
    ERRORF("TEST", "\n");
	ASSERT_EQ(ls::CTaskThreadPool::instance()->getThreadCount(), 4);
    ERRORF("TEST", "\n");
}

TEST_F(CTestTask, testDiffThreadId)
{
    ERRORF("TEST", "\n");
	ASSERT_EQ(ls::CTaskThreadPool::instance()->getThreadCount(), 4);
	ls::CTaskThreadPool::instance()->setThreadIdleTime(5000);

	for (int i = 0; i < 256; ++i)
	{
		ls::CTaskThreadPool::instance()->addTask(new CDelayTask(0));
	}
	ASSERT_EQ(ls::CTaskThreadPool::instance()->getThreadCount(), 64);

    ERRORF("TEST", "\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(15 * 1000));

	ASSERT_EQ(ls::CTaskThreadPool::instance()->getThreadCount(), 4);
}

TEST_F(CTestTask, Dead)
{

}

