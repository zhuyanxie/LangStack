package com.LangStackTest;

import java.util.ArrayList;

import com.LangStack.Log.Logger;

public class RealCallback extends ICallBack
{
    @Override public void interface1()
    {
        Logger.d("test", "interface1 called!\n");
    }

    @Override public int interface2(int t0)
    {
    	Logger.d("test", "interface2 called! arg0:" + t0 + "\n");
        return 2222222;
    }

    @Override public double interface3(ArrayList<Long> t0, Object t1)
    {
    	Logger.d("test", "interface3 called!\n");
        for (Long l : t0)
        	Logger.d("test", "" + l + "\n");
        
        if (t1 != null)
        {
        	Logger.d("test", "" + t1.toString() + "\n");
            if (t1 instanceof EasyTypeClass)
            {
                EasyTypeClass etc = (EasyTypeClass) t1;
                etc.dump();
            }
        }

        return 0.3333333;
    }

    @Override public String interface4()
    {
    	Logger.d("test", "interface4 called!\n");
        return "Java interface4 called!";
    }

}
