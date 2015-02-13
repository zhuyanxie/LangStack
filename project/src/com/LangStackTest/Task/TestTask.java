package com.LangStackTest.Task;

import static org.junit.Assert.*;

import org.junit.Test;

import com.LangStack.Task.TaskThreadPool;

public class TestTask {

	@Test
	public void testTask() 
	{
		assertEquals(TaskThreadPool.instance().getThreadCount(), 4);

		for (int i = 0; i < 4 * 8; ++i)
		{
			TaskThreadPool.instance().addTask(new DelayTask((i % 4) + 1));
		}
		assertEquals(TaskThreadPool.instance().getThreadCount(), 4);
		try {
			Thread.sleep(30 * 1000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		assertEquals(TaskThreadPool.instance().getThreadCount(), 4);
		
		
		
		for (int i = 0; i < 4 * 64; ++i)
		{
			TaskThreadPool.instance().addTask(new DelayTask(0));
		}
		assertEquals(TaskThreadPool.instance().getThreadCount(), 64);
		try {
			Thread.sleep(30 * 1000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		assertEquals(TaskThreadPool.instance().getThreadCount(), 4);
	}

}
