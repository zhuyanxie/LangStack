package com.LangStack.Cpp2Java;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import com.LangStack.Cpp2Java.TypeDefs.DefineSource.Source;

public class TypeDefs {

    public static final int           TYPE_UNKOWN      = 0;
    
    public static final int           TYPE_INT32       = 1;
    public static final int           TYPE_INT64       = 2;
    public static final int           TYPE_DOUBLE      = 3;
    public static final int           TYPE_STRING      = 4;

    public static final int           TYPE_LIST_INT32  = 11;
    public static final int           TYPE_LIST_INT64  = 12;
    public static final int           TYPE_LIST_DOUBLE = 13;
    public static final int           TYPE_LIST_STRING = 14;

    public static final int           TYPE_OBJECT      = 20;
    public static final int           TYPE_LIST_OBJECT = 30;

    private Set<String>               mUnkownKeys      = new HashSet<String>();
    private Map<String, DefineSource> mUnkownTypes     = new HashMap<String, DefineSource>();
    private Map<String, Integer>      mType2Key        = new HashMap<String, Integer>();
    private Map<Integer, String>      mKey2Type        = new HashMap<Integer, String>();
    
    class DefineSource
    {
        class Source
        {
            private String mFile;        ///< 定义的文件
            private int    mLine;        ///< 定义的行数
            private String mDetail;      ///< 定义的整行内容
            
            public Source(String file, int line, String detail) {
                mFile = file;
                mLine = line;
                mDetail = detail;
            }

            public String getFile() {
                return mFile;
            }

            public int getLine() {
                return mLine;
            }

            public String getDetail() {
                return mDetail;
            }
        }

        
        public List<Source> mSouce = new ArrayList<Source>(); ///< 定义的位置
        public String       mType;                            ///< 定义的类型

        public DefineSource(String type) {
            mType   = type;
        }
        
        public void addSouce(String file, int line, String detail) {
            mSouce.add(new Source(file, line, detail));
        }
    }
    
    public TypeDefs()
    {
        mType2Key.put("int", TYPE_INT32);
        mType2Key.put("INT", TYPE_INT32);
        mType2Key.put("DWORD", TYPE_INT32);
        mType2Key.put("int32_t", TYPE_INT32);
        mType2Key.put("uint32_t", TYPE_INT32);

        mType2Key.put("long", TYPE_INT64);
        mType2Key.put("long long", TYPE_INT64);
        mType2Key.put("int64_t", TYPE_INT64);
        mType2Key.put("uint64_t", TYPE_INT64);

        mType2Key.put("float", TYPE_DOUBLE);
        mType2Key.put("double", TYPE_DOUBLE);
        
        mType2Key.put("string", TYPE_STRING);
        mType2Key.put("std::string", TYPE_STRING);
        
        mType2Key.put("std::list<int>", TYPE_LIST_INT32);
        mType2Key.put("std::list<INT>", TYPE_LIST_INT32);
        mType2Key.put("std::list<DWORD>", TYPE_LIST_INT32);
        mType2Key.put("std::list<int32_t>", TYPE_LIST_INT32);
        mType2Key.put("std::list<uint32_t>", TYPE_LIST_INT32);         
        mType2Key.put("list<int>", TYPE_LIST_INT32);
        mType2Key.put("list<INT>", TYPE_LIST_INT32);
        mType2Key.put("list<DWORD>", TYPE_LIST_INT32);
        mType2Key.put("list<int32_t>", TYPE_LIST_INT32);
        mType2Key.put("list<uint32_t>", TYPE_LIST_INT32); 

        mType2Key.put("std::list<long>", TYPE_LIST_INT64);
        mType2Key.put("std::list<long long>", TYPE_LIST_INT64);
        mType2Key.put("std::list<int64_t>", TYPE_LIST_INT64);
        mType2Key.put("std::list<uint64_t>", TYPE_LIST_INT64);
        mType2Key.put("list<long>", TYPE_LIST_INT64);
        mType2Key.put("list<long long>", TYPE_LIST_INT64);
        mType2Key.put("list<int64_t>", TYPE_LIST_INT64);
        mType2Key.put("list<uint64_t>", TYPE_LIST_INT64);

        mType2Key.put("std::list<float>", TYPE_LIST_DOUBLE);
        mType2Key.put("std::list<double>", TYPE_LIST_DOUBLE);
        mType2Key.put("list<float>", TYPE_LIST_DOUBLE);
        mType2Key.put("list<double>", TYPE_LIST_DOUBLE);

        mType2Key.put("std::list<string>", TYPE_LIST_STRING);
        mType2Key.put("std::list<std::string>", TYPE_LIST_STRING);
        mType2Key.put("list<string>", TYPE_LIST_STRING);
        mType2Key.put("list<std::string>", TYPE_LIST_STRING);
    }
    
    /**
     * @breief      增加类型定义
     * @param       file        定义的文件
     * @param       line        文件中行数
     * @param       detail      详细定义内容
     * @param       type        原类型
     * @param       newType     新类型
     */
    public void addTypeDefine(String file, int line, String detail,
            String type, String newType) {
        if (!mUnkownTypes.containsKey(newType)) {
            mUnkownTypes.put(newType, new DefineSource(type));
        }
        mUnkownKeys.add(newType);
        mUnkownTypes.get(newType).addSouce(file, line, detail);
    }

    /**
     * @brief       类型连接
     */
    public void linkTypes() {
        for (String key : mUnkownKeys) {
            int newType = getTypes(key);
            int oldType = getTypes(mUnkownTypes.get(key).mType);
            if (TYPE_UNKOWN == newType && TYPE_UNKOWN == oldType) {
                continue;
            }
            
            if (newType == oldType) {
                mType2Key.put(key, newType);
                mUnkownTypes.remove(key);
            } else {
                linkTypeError(key, newType, oldType);
            }
        }
    }

    /**
     * @brief       类型连接错误提示
     * @param       key         string类型
     * @param       newType     新类型
     * @param       oldType     原类型
     */
    private void linkTypeError(String key, int newType, int oldType) {
        DefineSource source = mUnkownTypes.get(key);
        System.out.println("typedefine error new Type:" + 
            mKey2Type.get(newType) + " oldType: " + mKey2Type.get(oldType));
        
        for (Source s : source.mSouce) {
            System.out.println("\tfile:" + s.getFile() + " line:"
                    + s.getLine() + " detail:" + s.mDetail);
        }
        System.out.print("\n");
    }

    private int getTypes(String key) {
        if (mType2Key.containsKey(key)) {
            return mType2Key.get(key);
        }
        
        return getTypes(mUnkownTypes.get(key).mType);
    }
    
}
