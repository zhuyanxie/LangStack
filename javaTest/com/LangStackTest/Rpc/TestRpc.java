package com.LangStackTest.Rpc;

import java.util.ArrayList;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import com.LangStack.LangStack;
import com.LangStack.Log.Logger;
import com.LangStackTest.MethodClass;
import com.LangStackTest.RealCallback;

public class TestRpc
{    
    static {
        System.loadLibrary("LangStack");
        System.loadLibrary("LSCppDemo");
    }
    
    @Before
    public void setUp() throws Exception
    {
        Logger.d("javarpc", "Rpc Test setUp\n");
    }

    @After
    public void tearDown() throws Exception
    {
        Logger.d("javarpc", "Rpc Test tearDown\n");
    }

    @Test
    public void TestRpcClientCall()
    {
        /// 启动
    	LangStack.startJniTransMode();
        
        MethodClass ntc = new MethodClass();
        Logger.d("javarpc", "Rpc call dump start\n");
        ntc.dump();
        Logger.d("javarpc", "Rpc call dump end\n");

        ArrayList<Integer> il   = new ArrayList<Integer>();
        ArrayList<Long> ll      = new ArrayList<Long>();
        for (Long i = 0L; i < 5L; ++i)
        {
            il.add(i.intValue());
            ll.add(i * 1010101020202020L);
        }

        Logger.d("javarpc", "Rpc call test start\n");
        double res = ntc.test(1, 222333444555666777L, il, ll);        
        Logger.d("javarpc", "Rpc call test end\n");

        Logger.d("javarpc", "the ntc test return : " + res + "\n");
    }

    @Test
    public void TestRpcClientCallback()
    {
        /// 启动
        LangStack.startJniTransMode();
        
        MethodClass     ntc = new MethodClass();
        RealCallback    rcb = new RealCallback();
        
        for (int i = 0; i < 1; ++i)
        {
            ntc.attach(rcb);
            /// dump方法会触发回调
            ntc.dump();
            
            ntc.detach(rcb);
            /// dump方法不触发回调
            ntc.dump();
        }
    }
}
