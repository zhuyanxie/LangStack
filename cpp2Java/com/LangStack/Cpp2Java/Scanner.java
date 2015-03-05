package com.LangStack.Cpp2Java;

import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Scanner {

    private String              mRoot     = ".";
    private List<String>        mPaths    = new ArrayList<String>();
    private List<String>        mFiles    = new ArrayList<String>();
    private Map<String, String> mScanInfo = new HashMap<String, String>();
    
    public Scanner(String root) {
        mRoot = root;
    }

    public void scan()
    {
        scanPaths(new File(mRoot));
        scanFiles();  
    }

    public String getRoot() {
        return mRoot;
    }

    public List<String> getPaths() {
        return mPaths;
    }

    public List<String> getFiles() {
        return mFiles;
    }

    public Map<String, String> getScanInfo() {
        return mScanInfo;
    }

    /**
     * @brief       递归指定路径下的所有.h为后缀的文件
     * @param       root
     */
    private void scanPaths(File root) {
        if (!root.isDirectory()) {
            mFiles.add(root.getAbsolutePath() + root.getName());
            return;
        }
        else if (!root.isDirectory()) {
            return;
        }
        
        mPaths.add(root.getAbsolutePath());
        for (File file : root.listFiles()) {
            scanPaths(file);
        }
    }
    
    /**
     * @brief       扫描所有文件
     */
    private void scanFiles() {
    	
    }

    
    
}
