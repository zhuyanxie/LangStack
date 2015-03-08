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
    public static final int           TYPE_VOID        = 99;

    private Set<String>               mCppTypeSet      = new HashSet<String>();
    private Map<String, DefineSource> mCppTypes        = new HashMap<String, DefineSource>();
    private Map<String, Integer>      mCppType2Enum    = new HashMap<String, Integer>();
    private Map<Integer, String>      mEnum2JavaType   = new HashMap<Integer, String>();
    
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
        mCppType2Enum.put("int", TYPE_INT32);
        mCppType2Enum.put("INT", TYPE_INT32);
        mCppType2Enum.put("DWORD", TYPE_INT32);
        mCppType2Enum.put("int32_t", TYPE_INT32);
        mCppType2Enum.put("uint32_t", TYPE_INT32);

        mCppType2Enum.put("long", TYPE_INT64);
        mCppType2Enum.put("long long", TYPE_INT64);
        mCppType2Enum.put("int64_t", TYPE_INT64);
        mCppType2Enum.put("uint64_t", TYPE_INT64);

        mCppType2Enum.put("float", TYPE_DOUBLE);
        mCppType2Enum.put("double", TYPE_DOUBLE);
        
        mCppType2Enum.put("string", TYPE_STRING);
        mCppType2Enum.put("std::string", TYPE_STRING);
        
        mCppType2Enum.put("std::list<int>", TYPE_LIST_INT32);
        mCppType2Enum.put("std::list<INT>", TYPE_LIST_INT32);
        mCppType2Enum.put("std::list<DWORD>", TYPE_LIST_INT32);
        mCppType2Enum.put("std::list<int32_t>", TYPE_LIST_INT32);
        mCppType2Enum.put("std::list<uint32_t>", TYPE_LIST_INT32);         
        mCppType2Enum.put("list<int>", TYPE_LIST_INT32);
        mCppType2Enum.put("list<INT>", TYPE_LIST_INT32);
        mCppType2Enum.put("list<DWORD>", TYPE_LIST_INT32);
        mCppType2Enum.put("list<int32_t>", TYPE_LIST_INT32);
        mCppType2Enum.put("list<uint32_t>", TYPE_LIST_INT32); 

        mCppType2Enum.put("std::list<long>", TYPE_LIST_INT64);
        mCppType2Enum.put("std::list<long long>", TYPE_LIST_INT64);
        mCppType2Enum.put("std::list<int64_t>", TYPE_LIST_INT64);
        mCppType2Enum.put("std::list<uint64_t>", TYPE_LIST_INT64);
        mCppType2Enum.put("list<long>", TYPE_LIST_INT64);
        mCppType2Enum.put("list<long long>", TYPE_LIST_INT64);
        mCppType2Enum.put("list<int64_t>", TYPE_LIST_INT64);
        mCppType2Enum.put("list<uint64_t>", TYPE_LIST_INT64);

        mCppType2Enum.put("std::list<float>", TYPE_LIST_DOUBLE);
        mCppType2Enum.put("std::list<double>", TYPE_LIST_DOUBLE);
        mCppType2Enum.put("list<float>", TYPE_LIST_DOUBLE);
        mCppType2Enum.put("list<double>", TYPE_LIST_DOUBLE);

        mCppType2Enum.put("std::list<string>", TYPE_LIST_STRING);
        mCppType2Enum.put("std::list<std::string>", TYPE_LIST_STRING);
        mCppType2Enum.put("list<string>", TYPE_LIST_STRING);
        mCppType2Enum.put("list<std::string>", TYPE_LIST_STRING);

        mCppType2Enum.put("void", TYPE_VOID);
        mCppType2Enum.put("VOID", TYPE_VOID);


        mEnum2JavaType.put(TYPE_INT32, "Integer");
        mEnum2JavaType.put(TYPE_INT64, "Long");
        mEnum2JavaType.put(TYPE_DOUBLE, "Double");
        mEnum2JavaType.put(TYPE_STRING, "String");
        
        mEnum2JavaType.put(TYPE_LIST_INT32, "ArrayList<Integer>");
        mEnum2JavaType.put(TYPE_LIST_INT64, "ArrayList<Long>");
        mEnum2JavaType.put(TYPE_LIST_DOUBLE, "ArrayList<Double>");
        mEnum2JavaType.put(TYPE_LIST_STRING, "ArrayList<String>");
        
        mEnum2JavaType.put(TYPE_OBJECT, "IRpcApi");
        mEnum2JavaType.put(TYPE_LIST_OBJECT, "ArrayList<IRpcApi>");
        mEnum2JavaType.put(TYPE_VOID, "void");
    }
    
    /**
     * @brief       增加类型定义
     * @param       file        定义的文件
     * @param       line        文件中行数
     * @param       detail      详细定义内容
     * @param       type        原类型
     * @param       newType     新类型
     */
    public void addTypeDefine(String file, int line, String detail,
            String type, String newType) {
        if (!mCppTypes.containsKey(newType)) {
            mCppTypes.put(newType, new DefineSource(type));
        }
        mCppTypeSet.add(newType);
        mCppTypes.get(newType).addSouce(file, line, detail);
    }

    /**
     * @brief       类型解析
     */
    public void parseType() {
        for (String key : mCppTypeSet) {
            int newType = getUnkownCppTypes(key);
            int oldType = getUnkownCppTypes(mCppTypes.get(key).mType);
            if (TYPE_UNKOWN == newType && TYPE_UNKOWN == oldType) {
                continue;
            }
            
            if (newType == oldType) {
                mCppType2Enum.put(key, newType);
                mCppTypes.remove(key);
            } else {
                parseTypeError(key, newType, oldType);
            }
        }
    }

    /**
     * @brief       类型解析错误提示
     * @param       key         string类型
     * @param       newType     新类型
     * @param       oldType     原类型
     */
    private void parseTypeError(String key, int newType, int oldType) {
        DefineSource source = mCppTypes.get(key);
        System.out.println("type link error new Type:" + 
            mEnum2JavaType.get(newType) + " oldType: " + mEnum2JavaType.get(oldType));
        
        for (Source s : source.mSouce) {
            System.out.println("\tfile:" + s.getFile() + " line:"
                    + s.getLine() + " detail:" + s.mDetail);
        }
        System.out.print("\n");
    }

    /**
     * @brief       获取位置的类型
     * @param       key         键
     * @return
     */
    private int getUnkownCppTypes(String key) {
        if (mCppType2Enum.containsKey(key)) {
            return mCppType2Enum.get(key);
        }
        
        if (mCppTypes.containsKey(key)) {
            return getUnkownCppTypes(mCppTypes.get(key).mType);
        }
        return TYPE_UNKOWN;
    }

    /**
     * @param       param       cpp类型定义
     * @return      类型枚举定义
     */
    public int getCppType(String param) {
        if (mCppType2Enum.containsKey(param)) {
            return mCppType2Enum.get(param);
        }
        return TYPE_UNKOWN;
    }

    /**
     * @param       type        类型枚举值
     * @return      java中的类型定义
     */
    public String getJavaType(int type) {
        return mEnum2JavaType.get(type);
    }
    
}
