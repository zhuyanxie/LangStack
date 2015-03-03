package com.LangStackTest;

import java.util.ArrayList;

import com.LangStack.Rpc.IRpcApi;

public abstract class ICallBack extends IRpcApi
{
    public abstract void interface1();
    public abstract int interface2(int t0);
    public abstract double interface3(ArrayList<Long> t0, Object t1);
    public abstract String interface4();
}
