package com.LangStack.Serial;

import java.util.ArrayList;
import java.lang.reflect.Field;
import java.nio.charset.Charset;

/**
 * 序列化 API
 */
public class Serial {

    public static final String TAG_CHAR     = "I8:";
    public static final String TAG_SHORT    = "I16:";
    public static final String TAG_INT      = "I32:";
    public static final String TAG_LONG		= "I64:";
    public static final String TAG_DOUBLE	= "D:";
    public static final String TAG_STRING	= "S:";
    public static final String TAG_LIST		= "L:";
    public static final String TAG_MEMORY   = "M";
    public static final String TAG_CLASS    = "C";
    public static final String TAG_END		= ":";
    public static final String TAG_PARAM_SPLIT	= "&";
    public static final String DETAIL_END   = ";";

    public static final String TAG_EMPTY_CLASS 	= "C:E;";
    public static final String TAG_EMPTY_LIST  	= "L:E;";
    
    public static String serial(Object obj)
    {
        if (obj instanceof Charset)
        {
            return serial((Charset)obj);
        } 
        else if(obj instanceof Short)
        {
            return serial((Short)obj);
        } 
        else if(obj instanceof Integer)
        {
            return serial((Integer)obj);
        } 
        else if (obj instanceof Long)
        {
            return serial((Long)obj);
        } 
        else if (obj instanceof Double)
        {
            return serial((Double)obj);
        } 
        else if (obj instanceof String)
        {
            return serial((String)obj);
        }         
        else if (obj instanceof ArrayList<?>)
        {
            return serial((ArrayList<?>)obj);
        } 
        else
        {
            return serialClass(obj);
        }  
    }
    
    public static String serial(byte[] memory)
    {
        return TAG_MEMORY + memory.length + TAG_END +
                memory.toString() + DETAIL_END;
    }
    
    public static String serial(Charset o)
    {
        return TAG_CHAR + o + DETAIL_END;
    }    
    
    public static String serial(Short o)
    {
        return TAG_SHORT + o + DETAIL_END;
    }    
    
    public static String serial(Integer o)
    {
        return TAG_INT + o + DETAIL_END;
    }

    public static String serial(Long o)
    {
        return TAG_LONG + o + DETAIL_END;
    }

    public static String serial(Double o)
    {
        return TAG_DOUBLE + o + DETAIL_END;
    }

    public static String serial(String o)
    {
        /// TODO String改成tlv模型
        if (null != o)
        {
            o = o.replaceAll("%", "%%");
            o = o.replaceAll(":", "%0");
            o = o.replaceAll(";", "%1");
        }
        return TAG_STRING + o + DETAIL_END;
    }

    public static String serial(ArrayList<?> o)
    {
        if (null == o || o.isEmpty())
        {
            return TAG_EMPTY_LIST;
        }
        
        String str = TAG_LIST;
        for (Object obj : o)
        {
            str = str + serial(obj);
        }
        str = str + DETAIL_END;
        
        return str;
    }

    public static String serialClass(Object o)
    {
        if (null == o)
        {
            return TAG_EMPTY_CLASS;
        }

        String serialName = o.getClass().getName();
        if (null == serialName)
        {
            return null;
        }
        String str = TAG_CLASS + serialName + TAG_END;

        /// 获取所有属性域
        Field[] fields = o.getClass().getDeclaredFields();
        for (int i = 0; i < fields.length; ++i)
        {
            /// 获取类型、属性名、值
            String varName = fields[i].getName() + TAG_PARAM_SPLIT;

            fields[i].setAccessible(true);
            String valueSerialString = null;
            try {
                Object obj = fields[i].get(o);
                valueSerialString = serial(obj);
            } catch (IllegalArgumentException e) {
                e.printStackTrace();
            } catch (IllegalAccessException e) {
                e.printStackTrace();
            }

            if (null == valueSerialString)
            {
                return null;
            }
            str = str + varName + valueSerialString;
        }

        str = str + DETAIL_END;
        return str;
    }
}
