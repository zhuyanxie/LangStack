package com.LangStackTest.Serial;

import static org.junit.Assert.*;

import org.junit.Test;

import com.LangStack.Serial.Deserial;
import com.LangStack.Serial.Serial;
import com.LangStackTest.EasyTypeClass;

public class TestSerial {

    @Test
    public void testSerial()
    {
        byte c = -28;
        short s = 1024;
        int i = 0;
        long l = 1000100012345l;
        double d = 0.100100;
        String normal = "ok";
        String special = ";;;:::%0%1%";
        byte[] memory = new byte[1024];
        for (int x = 0; x < 1024; ++x)
        {
            memory[x] = (byte)(x % 256);
        }

        assertEquals(Serial.serial(i), "I32:0;");
        assertEquals(Serial.serial(l), "I64:1000100012345;");
        assertEquals(Serial.serial(d), "D:0.1001;");
        assertEquals(Serial.serial(normal), "S:ok;");
        assertEquals(Serial.serial(special), "S:%1%1%1%0%0%0%%0%%1%%;");

        byte dc = (byte) Deserial.deserial(Serial.serial(c));
        short ds = (short) Deserial.deserial(Serial.serial(s));
        int di = (Integer) Deserial.deserial(Serial.serial(i));
        long dl = (Long) Deserial.deserial(Serial.serial(l));
        double dd = (Double) Deserial.deserial(Serial.serial(d));
        String dsn = (String) Deserial.deserial(Serial.serial(normal));
        String dss = (String) Deserial.deserial(Serial.serial(special));
        byte[] dm  = (byte[]) Deserial.deserial(Serial.serial(memory));

        assertEquals(dc, c);
        assertEquals(ds, s);
        assertEquals(di, i);
        assertEquals(dl, l);
        assertEquals(dd, d, 0.0001);
        assertEquals(dsn, normal);
        assertEquals(dss, special);
        
        for (int x = 0; x < 1024; ++x) 
        {
            assertEquals(memory[x], dm[x]);
        }
    }

    @Test
    public void testSerialEasyClass()
    {
        EasyTypeClass easy = new EasyTypeClass();
        String str = Serial.serial(easy);
        EasyTypeClass de = (EasyTypeClass) Deserial.deserial(str);
        assertTrue(de.equals(easy));
    }

}
