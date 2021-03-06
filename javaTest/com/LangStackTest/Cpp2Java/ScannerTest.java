package com.LangStackTest.Cpp2Java;

import static org.junit.Assert.*;

import org.junit.Test;

import com.LangStack.Cpp2Java.Scanner;
import com.LangStack.Cpp2Java.Symbols;

public class ScannerTest {

    @Test
    public void testGetFilePaths() {
        Symbols symbols = new Symbols("./cpp2java_testcase");
        Scanner scanner = new Scanner("./cpp2java_testcase", symbols);
        scanner.scan();
        symbols.parse();
        symbols.genJavaCode();
    }

}
