package com.LangStackTest.Cpp2Java;

import static org.junit.Assert.*;

import org.junit.Test;

import com.LangStack.Cpp2Java.Scanner;

public class ScannerTest {

    @Test
    public void testGetFilePaths() {
        Scanner scanner = new Scanner("./");
        scanner.scan();
        
    }

}
