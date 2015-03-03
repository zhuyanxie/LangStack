package com.LangStackTest.Rpc;

import java.util.ArrayList;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import com.LangStack.LangStack;
import com.LangStack.Log.Logger;
import com.LangStackTest.NormalTypeClass;
import com.LangStackTest.RealCallback;

public class TestRpc
{
    @Before
    public void setUp() throws Exception
    {
        Logger.d("javarpc", "Rpc Test setUp");
    }

    @After
    public void tearDown() throws Exception
    {
        Logger.d("javarpc", "Rpc Test tearDown");
    }

    @Test
    public void TestRpcClientCall()
    {
        /// 全局启动一次
    	LangStack.startJniTransMode();
        
        NormalTypeClass ntc = new NormalTypeClass();
        Logger.d("javarpc", "Rpc call dump start");
        ntc.dump();
        Logger.d("javarpc", "Rpc call dump end");

        ArrayList<Integer> il   = new ArrayList<Integer>();
        ArrayList<Long> ll      = new ArrayList<Long>();
        for (Long i = 0L; i < 5L; ++i)
        {
            il.add(i.intValue());
            ll.add(i * 1010101020202020L);
        }

        Logger.d("javarpc", "Rpc call test start");
        double res = ntc.test(1, 222333444555666777L, il, ll);        
        Logger.d("javarpc", "Rpc call test end");

        Logger.d("javarpc", "the ntc test return : " + res);
    }

    @Test
    public void TestRpcClientCallback()
    {
        /// 已经启动过了  不需要再启动
        /// LangStack.start(16759);
        
        NormalTypeClass ntc = new NormalTypeClass();
        RealCallback    rcb = new RealCallback();
        
        for (int i = 0; i < 100; ++i)
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
