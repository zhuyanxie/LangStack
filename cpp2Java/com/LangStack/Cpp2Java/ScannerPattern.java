package com.LangStack.Cpp2Java;

import java.util.regex.Pattern;

public class ScannerPattern {
  
    public static Pattern BLOCK_COMMENT_START = Pattern.compile(
            "^\\s*/\\*");
    public static Pattern BLOCK_COMMENT = Pattern.compile(
            "^\\s*/\\*(.*)\\*/(.*)");
    
    public static Pattern LINE_COMMENT_START = Pattern.compile(
            "^\\s*//");
    public static Pattern LINE_COMMENT = Pattern.compile(
            "^\\s*//(.*)[^\\]$");

    public static Pattern INCLUDE_START = Pattern.compile(
            "^\\s*#include.*");
    public static Pattern INCLUDE = Pattern.compile(
            "^\\s*#include\\s*<[_a-zA-Z]\\w+\\.h>\\s*");
    
    public static Pattern TYPEDEF_START = Pattern.compile(
            "^\\s*typedef.*");  
    public static Pattern TYPEDEF = Pattern.compile(
            "^\\s*typedef(.*);(.*)");  
    
    public static Pattern DEFINE_START = Pattern.compile(
            "^\\s*#define.*");  
    public static Pattern DEFINE = Pattern.compile(
            "^\\s*#define(.*[^\\\\]$)");  
    
    public static Pattern USING_START = Pattern.compile(
            "^\\s*using.*");  
    public static Pattern USING = Pattern.compile(
            "^\\s*using(.*);(.*)");  
    
    public static Pattern NAMESPACE_START = Pattern.compile(
            "^\\s*namespace.*");      
    public static Pattern NAMESPACE = Pattern.compile(
            "^\\s*namespace(.*){\\s*(.*)");  
    
    public static Pattern CLASS_START = Pattern.compile(
            "^\\s*(class|struct).*");
    public static Pattern CLASS = Pattern.compile(
            "^\\s*(class|struct)\\s*(.*)\\s*{(.*)");  
}
