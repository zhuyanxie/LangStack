package com.LangStackTest;

public class EasyTypeClass {
	
    public int    mInt;
    public long   mLonglong;
    public double mDouble;
    public String mNormal;
    public String mSpecial;

    public EasyTypeClass()
    {
        mInt = 0;
        mLonglong = 0;
        mDouble = 0;
        mNormal = null;
        mSpecial = null;
    }

    @Override
    public boolean equals(Object o)
    {
        EasyTypeClass rhs = (EasyTypeClass) o;

        return mInt == rhs.mInt && mLonglong == rhs.mLonglong && 
        		mDouble == rhs.mDouble && mNormal.equals(rhs.mNormal) && 
        		mSpecial.equals(rhs.mSpecial);
    }
    
    public void dump()
    {
        System.out.println("dump :");
        System.out.println("\t\t\t\t mInt :" + mInt);
        System.out.println("\t\t\t\t mLonglong :" + mLonglong);
        System.out.println("\t\t\t\t mDouble :" + mDouble);
        System.out.println("\t\t\t\t mNormal :" + mNormal);
        System.out.println("\t\t\t\t mSpecial :" + mSpecial);
    }
}
