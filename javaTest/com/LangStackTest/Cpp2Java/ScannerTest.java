package com.LangStackTest.Cpp2Java;

import static org.junit.Assert.*;

import org.junit.Test;

import com.LangStack.Cpp2Java.Scanner;
import com.LangStack.Cpp2Java.Symbols;

public class ScannerTest {

    @Test
    public void testGetFilePaths() {
        Symbols symbols = new Symbols();
        Scanner scanner = new Scanner("./", symbols);
        scanner.scan();
        
    }

}
