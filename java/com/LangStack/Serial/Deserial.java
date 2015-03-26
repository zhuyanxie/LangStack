package com.LangStack.Serial;

import java.lang.reflect.Field;
import java.nio.charset.Charset;
import java.util.ArrayList;

public class Deserial {

    /**
     * @brief   反序列化
     * @param   str     输入的字符串
     * @return  反序列化后的对象
     */
    public static Object deserial(String str)
    {
        if (!str.endsWith(Serial.DETAIL_END))
        {
            return null;
        }

        if (str.startsWith(Serial.TAG_CHAR))
        {
            int val = Integer.parseInt(str.substring(Serial.TAG_CHAR.length(),
                    str.length() - 1));
            return (char) val;
        }
        else if (str.startsWith(Serial.TAG_SHORT))
        {
            int val =  Integer.parseInt(str.substring(Serial.TAG_SHORT.length(),
                    str.length() - 1));
            return (short) val;
        }
        else if (str.startsWith(Serial.TAG_INT))
        {
            return Integer.parseInt(str.substring(Serial.TAG_INT.length(),
                    str.length() - 1));
        }
        else if (str.startsWith(Serial.TAG_LONG))
        {
            return Long.parseLong(str.substring(Serial.TAG_LONG.length(),
                    str.length() - 1));
        }
        else if (str.startsWith(Serial.TAG_DOUBLE))
        {
            return Double.parseDouble(str.substring(Serial.TAG_DOUBLE.length(),
                    str.length() - 1));
        } 
        else if (str.startsWith(Serial.TAG_STRING))
        {
            return DecodeString(str.substring(Serial.TAG_STRING.length(),
                    str.length() - 1));
        } 
        else if (str.startsWith(Serial.TAG_LIST))
        {
            return deserialList(str);
        } 
        else if (str.startsWith(Serial.TAG_CLASS))
        {
            return deserialObject(str);
        }

        return null;
    }
    
    /**
     * @brief   获取反序列化类型
     * @param   str     输入的字符串
     * @return  类型
     */
    public static Class<?> getType(String str)
    {
        if (!str.endsWith(Serial.DETAIL_END))
        {
            return Object.class;
        }

        if (str.startsWith(Serial.TAG_CHAR))
        {
            return char.class;
        } 
        else if (str.startsWith(Serial.TAG_SHORT))
        {
            return short.class;
        } 
        else if (str.startsWith(Serial.TAG_INT))
        {
            return int.class;
        } 
        else if (str.startsWith(Serial.TAG_LONG))
        {
            return long.class;
        } 
        else if (str.startsWith(Serial.TAG_DOUBLE))
        {
            return double.class;
        } 
        else if (str.startsWith(Serial.TAG_STRING))
        {
            return String.class;
        } 
        else if (str.startsWith(Serial.TAG_LIST))
        {
            return ArrayList.class;
        }
        else if (str.startsWith(Serial.TAG_MEMORY))
        {
            return byte[].class;
        }

        return Object.class;
    }

    private static ArrayList<Object> deserialList(String str)
    {
        ArrayList<Object> oList = new ArrayList<Object>();
        if (str.equals("List:Empty;"))
        {
            return oList;
        }
        str = str.substring(Serial.TAG_LIST.length(), str.length() - 1);
        while (str.length() > 0)
        {
            int next = getDetailLength(str);
            if (next == 0)
            {
                break;
            }
            Object value = deserial(str.substring(0, next + 1));
            oList.add(value);
            str = str.substring(next + 1, str.length());
        }

        return oList;
    }

    private static Object deserialObject(String str)
    {
        Object o = null;
        if (str.equals("Class:Empty;"))
        {
            return o;
        }

        String clsName = str.substring(Serial.TAG_CLASS.length(),
                str.indexOf(Serial.TAG_END));

        str = str.substring(str.indexOf(Serial.TAG_END) + 1, str.length() - 1);
        try {
            o = Class.forName(clsName).newInstance();
        } catch (InstantiationException e1) {
            e1.printStackTrace();
        } catch (IllegalAccessException e1) {
            e1.printStackTrace();
        } catch (ClassNotFoundException e1) {
            e1.printStackTrace();
        }

        while (o != null && str.length() > 0)
        {
            int paramSplit = str.indexOf(Serial.TAG_PARAM_SPLIT);
            if (paramSplit == -1)
            {
                break;
            }
            String memberName = str.substring(0, paramSplit);

            int next = getDetailLength(str);
            Object value = deserial(str.substring(
                    str.indexOf(Serial.TAG_PARAM_SPLIT) + 1, next + 1));

            try {
                Field field = o.getClass().getDeclaredField(memberName);
                field.setAccessible(true);
                field.set(o, value);
            } catch (IllegalArgumentException e) {
                e.printStackTrace();
            } catch (IllegalAccessException e) {
                e.printStackTrace();
            } catch (NoSuchFieldException e) {
                e.printStackTrace();
            } catch (SecurityException e) {
                e.printStackTrace();
            }

            str = str.substring(next + 1, str.length());
        }
        return o;
    }

    /**
     * @brief 解析字符串，注意%%1等串
     * @param   str       序列化后的串
     * @return  原串
     */
    private static String DecodeString(String str)
    {
        int i = str.indexOf('%');
        if (i == -1)
        {
            return str;
        }
        String realStr = str.substring(0, i);
        int len = str.length();
        for (; i < len; ++i)
        {
            if (str.charAt(i) == '%' && i + 1 < len)
            {
                if (str.charAt(i + 1) == '%') realStr = realStr + "%";
                else if (str.charAt(i + 1) == '0') realStr = realStr + ":";
                else if (str.charAt(i + 1) == '1') realStr = realStr + ";";

                ++i;
            } 
            else
            {
                realStr = realStr + str.charAt(i);
            }
        }
        return realStr;
    }

    /**
     * @param str 序列化后的串
     * @return 下一次解析内容的长度
     */
    public static int getDetailLength(String str)
    {
        int pos = 0;
        int endflag = 0;
        for (; pos < str.length(); ++pos)
        {
            if (str.charAt(pos) == Serial.DETAIL_END.charAt(0))
            {
                if (--endflag <= 0)
                {
                    break;
                }
            }

            if (str.charAt(pos) == Serial.TAG_END.charAt(0))
            {
                ++endflag;
            }
        }

        return pos;
    }
}
