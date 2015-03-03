package com.LangStackTest;

import java.util.ArrayList;

import com.LangStack.Rpc.IRpcApi;

public class NormalTypeClass extends IRpcApi {

	/// 构造对应new
    public NormalTypeClass()
    {
        call("new", this.getClass().getName(), this);
    }

    /// 析构对应delete
    protected void finalize()
    {
        call("delete", this.getClass().getName(), this);
    }
    
    /// 无参调用
    public void dump()
    {
        call("dump", this.getClass().getName(), this);
    }
    
    /// 带参数调用
    public double test(int i, long j, ArrayList<Integer> ilist, 
            ArrayList<Long> llist)
    {
        return (Double)call("test", this.getClass().getName(), this, 
                i, j, ilist, llist);
    }
    
    /// 注册回调
    @Override public void attach(IRpcApi o)
    {
        /// 先创建回调代理
        call("new", "CRealCallBackProxy", o);
        /// 再attach回调对象ID
        callbackSetting("attach", this.getClass().getName(), this, o);
    }
    
    /// 注销回调
    @Override public void detach(IRpcApi o)
    {
        /// 先detach回调对象ID
        callbackSetting("detach", this.getClass().getName(), this, o);
        /// 再删除远程回调代理
        call("delete", "CRealCallBackProxy", o);
    }

}
