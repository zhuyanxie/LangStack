package com.LangStack.Cpp2Java;

import java.io.PrintStream;

public class Logger {
    
    public static void e(String errorInfo, PrintStream os, boolean terminal) {
        os.println(errorInfo);
        
        if (terminal) {
            System.exit(-1);
        }
    }
    
    
    public static void e(String errorInfo, PrintStream os, boolean terminal,
            String file, int line) {
        os.println(errorInfo);
        os.printf("\tfile[%s:%d]\r\n", file, line);
        
        if (terminal) {
            System.exit(-1);
        }
    }


    public static void e(String errorInfo, PrintStream os, boolean terminal,
            String file, int line, String cache) {
        os.println(errorInfo);
        os.printf("\tfile[%s:%d] [%s]\r\n", file, line, cache);
        
        if (terminal) {
            System.exit(-1);
        }
    }

}
