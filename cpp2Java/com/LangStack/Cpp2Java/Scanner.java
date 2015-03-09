package com.LangStack.Cpp2Java;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
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
    	for (String file : mFiles) {
    	    try {
                scanFile(file);
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            } catch (IOException ioe) {
                ioe.printStackTrace();
            }
    	}
    }

    private void scanFile(String file) throws IOException {
        FileInputStream fis     = new FileInputStream(new File(file));
        InputStreamReader isr   = new InputStreamReader(fis, "UTF-8");
        BufferedReader br       = new BufferedReader(isr);
        String buff             = "";
        String comment          = "";
        int line = 0;
        while ((buff = br.readLine()) != null) {
            ++line;
            
        }

        fis.close();
        br.close();
        isr.close();
    }

    
    
}
