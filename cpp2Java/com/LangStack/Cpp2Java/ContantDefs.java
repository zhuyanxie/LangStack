package com.LangStack.Cpp2Java;

import java.io.PrintStream;
import java.util.HashMap;
import java.util.Map;

public class ContantDefs {

    private Map<String, Integer> mIntConsts    = null;  ///< int常量定义
    private Map<String, Long>    mLongConsts   = null;  ///< long常量定义
    private Map<String, Double>  mDoubleConsts = null;  ///< double常量定义
    private Map<String, String>  mStringConsts = null;  ///< String常量定义
    /// TODO 其他类型常量定义
    
    public ContantDefs() {
        mIntConsts = new HashMap<String, Integer>();
        mLongConsts = new HashMap<String, Long>();
        mDoubleConsts = new HashMap<String, Double>();
        mStringConsts = new HashMap<String, String>();
    }
    
    /**
     * @brief       增加一个常量定义
     * @param       name        常量名
     * @param       obj         常量值
     * @param       comment     注释
     * @return      是否支持该种常量定义
     */
    public boolean addConst(String name, Object obj, String comment) {
        if (obj instanceof Integer) {
            mIntConsts.put(name, (Integer) obj);
        } else if (obj instanceof Long) {
            mLongConsts.put(name, (Long) obj);
        } else if (obj instanceof Double) {
            mDoubleConsts.put(name, (Double) obj);
        } else if (obj instanceof String) {
            mStringConsts.put(name, (String) obj);
        } else {
            return false;
        }

        return true;
    }
    
    /**
     * @brief       生成Java对应的常量定义
     * @param       p           输出流
     */
    public void genJava(PrintStream p) {
        for (String key : mIntConsts.keySet()) {
            p.println("    public static final int    " + key + " = "
                    + mIntConsts.get(key) + ";");
        }
        for (String key : mLongConsts.keySet()) {
            p.println("    public static final long   " + key + " = "
                    + mLongConsts.get(key) + ";");
        }
        for (String key : mDoubleConsts.keySet()) {
            p.println("    public static final double " + key + " = "
                    + mDoubleConsts.get(key) + ";");
        }
        for (String key : mStringConsts.keySet()) {
            p.println("    public static final String " + key + " = "
                    + mStringConsts.get(key) + ";");
        }
    }
}
