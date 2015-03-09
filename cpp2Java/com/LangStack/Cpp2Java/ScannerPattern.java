package com.LangStack.Cpp2Java;

import java.util.regex.Pattern;

public class ScannerPattern {
    public static Pattern DEFINE_PATTERN = Pattern.compile(
            "\\s*#include\\s*<[_a-zA-Z]\\w+\\.h>\\s*");
    
    public static Pattern TYPEDEFS_PATTERN = Pattern.compile(
            "\\s*typedef\\s+([_a-zA-Z]\\w+)\\s+([_a-zA-Z]\\w+)\\s*");
}
