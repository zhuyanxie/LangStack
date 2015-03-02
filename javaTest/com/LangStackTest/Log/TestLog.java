package com.LangStackTest.Log;

import static org.junit.Assert.*;

import org.junit.Test;

import com.LangStack.Log.Logger;

public class TestLog {
    
    static {
    	System.loadLibrary("LangStack");
    }
    
	@Test
	public void test() 
	{
		for (int i = Logger.LogLevelFATAL; i <= Logger.LogLevelFULL; ++i)
		{
			Logger.setLoggerLevel(i);
			Logger.f("TestLog", "Test : " + i + "\n");
			Logger.e("TestLog", "Test : " + i + "\n");
			Logger.w("TestLog", "Test : " + i + "\n");
			Logger.i("TestLog", "Test : " + i + "\n");
			Logger.d("TestLog", "Test : " + i + "\n");
			Logger.v("TestLog", "Test : " + i + "\n");
		}
	}

}
