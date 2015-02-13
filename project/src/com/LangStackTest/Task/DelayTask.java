package com.LangStackTest.Task;

import com.LangStack.Task.ITask;

public class DelayTask implements ITask {
	
	private static int id = 1;
	private String mId =null;
	
	public DelayTask(int i)
	{
		if (i == 0)
		{
			mId = "delay : " + ++id;
		}
		else 
		{
			mId = "delay : " + i;
		}
		
		System.out.println(mId);
	}

	@Override
	public void run() 
	{
		try {
			Thread.sleep(1000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

	@Override
	public String getTaskId() 
	{
		return mId;
	}

}
