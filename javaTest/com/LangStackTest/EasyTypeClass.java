package com.LangStackTest;

import com.LangStack.Log.Logger;

public class EasyTypeClass {
	
	private int		m_int;
	private long	m_longlong;
	private double	m_double;
	private String	m_normal;
	private String	m_special;

    public EasyTypeClass()
    {
    	m_int = 0;
    	m_longlong = 0;
    	m_double = 0;
    	m_normal = null;
    	m_special = null;
    }

    @Override
    public boolean equals(Object o)
    {
        EasyTypeClass rhs = (EasyTypeClass) o;

        return m_int == rhs.m_int && m_longlong == rhs.m_longlong && 
        		m_double == rhs.m_double && m_normal.equals(rhs.m_normal) && 
        				m_special.equals(rhs.m_special);
    }
    
    public void dump()
    {
        Logger.v("LSJava", "dump :");
        Logger.v("LSJava", "\t\t\t\t mInt :" + m_int);
        Logger.v("LSJava", "\t\t\t\t mLonglong :" + m_longlong);
        Logger.v("LSJava", "\t\t\t\t mDouble :" + m_double);
        Logger.v("LSJava", "\t\t\t\t mNormal :" + m_normal);
        Logger.v("LSJava", "\t\t\t\t mSpecial :" + m_special);
    }
}
