package com.LangStackTest.Serial;

import static org.junit.Assert.*;

import org.junit.Test;

import com.LangStack.Serial.Deserial;
import com.LangStack.Serial.Serial;
import com.LangStackTest.EasyTypeClass;
import com.LangStackTest.NormalTypeClass;

public class TestSerial {

    @Test
    public void testSerial()
    {
        int i = 0;
        long l = 1000100012345l;
        double d = 0.100100;
        String normal = "ok";
        String special = ";;;:::%0%1%";

        assertEquals(Serial.serial(i), "Int:0;");
        assertEquals(Serial.serial(l), "LLong:1000100012345;");
        assertEquals(Serial.serial(d), "Double:0.1001;");
        assertEquals(Serial.serial(normal), "String:ok;");
        assertEquals(Serial.serial(special), "String:%1%1%1%0%0%0%%0%%1%%;");

        int di = (Integer) Deserial.deserial(Serial.serial(i));
        long dl = (Long) Deserial.deserial(Serial.serial(l));
        double dd = (Double) Deserial.deserial(Serial.serial(d));
        String dsn = (String) Deserial.deserial(Serial.serial(normal));
        String dss = (String) Deserial.deserial(Serial.serial(special));

        assertEquals(di, i);
        assertEquals(dl, l);
        assertEquals(dd, d, 0.0001);
        assertEquals(dsn, normal);
        assertEquals(dss, special);
    }

    @Test
    public void testSerialEasyClass()
    {
        EasyTypeClass easy = new EasyTypeClass();
        easy.mInt = 10;
        easy.mLonglong = 1010202030304040L;
        easy.mDouble = 10.10;
        easy.mNormal = "10.10.10.10";
        easy.mSpecial = "!@#$%^&*()_+-=[]\\;',./`~<>?:\"{}|;;;:::%0%1%";
        String str = Serial.serial(easy);
        EasyTypeClass de = (EasyTypeClass) Deserial.deserial(str);
        assertTrue(de.equals(easy));
    }

    @Test
    public void testSerialNormalClass()
    {
        NormalTypeClass normal = new NormalTypeClass(3);
        String serialStr = Serial.serial(normal);
        NormalTypeClass dn = (NormalTypeClass) Deserial.deserial(serialStr);
        assertTrue(dn.equals(normal));
    }

}
