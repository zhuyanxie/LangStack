package com.LangStackTest;

import java.util.ArrayList;

public class NormalTypeClass {
    private int                      mInt;
    private long                     mLonglong;
    private double                   mDouble;
    private String                   mString;
    private EasyTypeClass            mClass;
    private EasyTypeClass            mClassNULL;

    private ArrayList<Integer>       mIntList;
    private ArrayList<Long>          mLonglongList;
    private ArrayList<Double>        mDoubleList;
    private ArrayList<String>        mStringList;
    private ArrayList<EasyTypeClass> mClassList;

    private ArrayList<Integer>       mIntListEmpty;
    private ArrayList<Long>          mLongListEmpty;
    private ArrayList<Long>          mLonglongListEmpty;
    private ArrayList<Double>        mDoubleListEmpty;
    private ArrayList<String>        mStringListEmpty;
    private ArrayList<EasyTypeClass> mClassListEmpty;

    public NormalTypeClass()
    {
        mInt = 0;
        mLonglong = 0;
        mDouble = 0;
        mString = new String();
        mClass = null;
        mClassNULL = null;
        mIntList = new ArrayList<Integer>();
        mLonglongList = new ArrayList<Long>();
        mDoubleList = new ArrayList<Double>();
        mStringList = new ArrayList<String>();
        mClassList = new ArrayList<EasyTypeClass>();
        mIntListEmpty = new ArrayList<Integer>();
        mLongListEmpty = new ArrayList<Long>();
        mLonglongListEmpty = new ArrayList<Long>();
        mDoubleListEmpty = new ArrayList<Double>();
        mStringListEmpty = new ArrayList<String>();
        mClassListEmpty = new ArrayList<EasyTypeClass>();
    }

    public NormalTypeClass(int seed)
    {
        mInt = seed;
        mLonglong = seed * 10001000L;
        mDouble = seed * 0.001001;
        mClass = null;
        mClassNULL = null;
        mIntList = new ArrayList<Integer>();
        mLonglongList = new ArrayList<Long>();
        mDoubleList = new ArrayList<Double>();
        mStringList = new ArrayList<String>();
        mClassList = new ArrayList<EasyTypeClass>();
        mIntListEmpty = new ArrayList<Integer>();
        mLongListEmpty = new ArrayList<Long>();
        mLonglongListEmpty = new ArrayList<Long>();
        mDoubleListEmpty = new ArrayList<Double>();
        mStringListEmpty = new ArrayList<String>();
        mClassListEmpty = new ArrayList<EasyTypeClass>();

        mString = new String();
        mString = String.format("%d-%d", seed, seed);
        EasyTypeClass temp = new EasyTypeClass();
        temp.mDouble = mDouble;
        temp.mInt = mInt;
        temp.mLonglong = mLonglong;
        temp.mSpecial = mString + ":::%0%1%;;;";
        temp.mNormal = mString;
        mClass = temp;

        for (int i = 1; i < 4; ++i)
        {
            mIntList.add(mInt + i);
            mLonglongList.add(mLonglong + i);
            mDoubleList.add(mDouble + i);
            mStringList.add(mString);

            temp = new EasyTypeClass();
            temp.mDouble = mDouble;
            temp.mInt = mInt + i;
            temp.mLonglong = mLonglong;
            temp.mSpecial = mString + ":::%%%;;;";
            temp.mNormal = mString;
            mClassList.add(temp);
        }
    }

    public boolean equals(NormalTypeClass rhs)
    {
		boolean ret = mInt == rhs.mInt && mLonglong == rhs.mLonglong
				&& mDouble == rhs.mDouble && mString.equals(rhs.mString)
				&& mClass.equals(rhs.mClass) && mClassNULL == rhs.mClassNULL;

        ret &= equals(mIntList, rhs.mIntList);
        ret &= equals(mDoubleList, rhs.mDoubleList);
        ret &= equals(mStringList, rhs.mStringList);
        ret &= equals(mClassList, rhs.mClassList);
        ret &= equals(mIntListEmpty, rhs.mIntListEmpty);
        ret &= equals(mLongListEmpty, rhs.mLongListEmpty);
        ret &= equals(mLonglongListEmpty, rhs.mLonglongListEmpty);
        ret &= equals(mDoubleListEmpty, rhs.mDoubleListEmpty);
        ret &= equals(mStringListEmpty, rhs.mStringListEmpty);
        ret &= equals(mClassListEmpty, rhs.mClassListEmpty);

        return ret;
    }

    private boolean equals(ArrayList<?> lhs, ArrayList<?> rhs)
    {
        if (lhs.size() != rhs.size())
        {
            return false;
        }

        for (int i = 0; i < lhs.size(); ++i)
        {
            if (!lhs.get(i).equals(rhs.get(i)))
            {
                return false;
            }
        }

        return true;
    }
}
