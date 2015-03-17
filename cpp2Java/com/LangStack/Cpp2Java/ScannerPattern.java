package com.LangStack.Cpp2Java;

import java.util.regex.Pattern;

/* test */

public class ScannerPattern {
  
    public static Pattern BLOCK_COMMENT_START = Pattern.compile(
            "^\\s*/\\*.*");
    public static Pattern BLOCK_COMMENT = Pattern.compile(
            "^\\s*/\\*(.*)\\*/(.*)");
    
    public static Pattern LINE_COMMENT_START = Pattern.compile(
            "^\\s*//.*");
    public static Pattern LINE_COMMENT = Pattern.compile(
            "^\\s*//(.*)[^\\\\]$");

    public static Pattern INCLUDE_START = Pattern.compile(
            "^\\s*#include.*");
    public static Pattern INCLUDE = Pattern.compile(
            "^\\s*#include\\s*(<.*>|\\\".*\\\")\\s*");
    
    public static Pattern TYPEDEF_START = Pattern.compile(
            "^\\s*typedef.*");  
    public static Pattern TYPEDEF = Pattern.compile(
            "^\\s*typedef(.*);(.*)");  
    
    public static Pattern DEFINE_START = Pattern.compile(
            "^\\s*#(define|ifdef|ifndef).*");  
    public static Pattern DEFINE = Pattern.compile(
            "^\\s*#(define|ifdef|ifndef)(.*[^\\\\]$)");  
    
    public static Pattern USING_START = Pattern.compile(
            "^\\s*using.*");  
    public static Pattern USING = Pattern.compile(
            "^\\s*using(.*);(.*)");  
    
    public static Pattern NAMESPACE_START = Pattern.compile(
            "^\\s*namespace.*");      
    public static Pattern NAMESPACE = Pattern.compile(
            "^\\s*namespace(.*)\\{\\s*(.*)");  
    
    
    public static Pattern CLASS_START = Pattern.compile(
            "^\\s*(class|struct).*");
    public static Pattern CLASS = Pattern.compile(
            "^\\s*(class|struct)\\s*(.*)\\s*\\{(.*)"); 
    public static Pattern CLASS_TAG = Pattern.compile(
            "^\\s*(class).*");   
    
    public static Pattern MEMBER = Pattern.compile(
            "^\\s*(\\S+)\\s+(\\S+);(.*)");     
    
    public static Pattern PERMISSION = Pattern.compile(
            "^\\s*(public|protected|private)\\s*:(.*)"); 
    
    public static Pattern END_OF_BLOCK = Pattern.compile(
            "^\\s*\\}\\s*(.*)");
    
    public static Pattern SEMICOLON_BLOCK = Pattern.compile(
            "^\\s*;.*");    
    public static Pattern DROP_SEMICOLON_BLOCK = Pattern.compile(
                    "^[^;].*");
    
    public static Pattern TEMPLATE = Pattern.compile(
            "^\\s*template\\s*<.*>\\s*(.*)");
    
    public static Pattern BLOCK_SEMICOLON = Pattern.compile(
            "^\\s*(.*)\\s*;(.*)");
    
    public static Pattern METHOD_BLOCK = Pattern.compile(
            "^\\s*(.*)\\s*\\((.*)\\)");
}
