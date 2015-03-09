package com.LangStack.Cpp2Java.Block;

import java.util.ArrayList;
import java.util.List;

import com.LangStack.Cpp2Java.ClassDefs;
import com.LangStack.Cpp2Java.TypeDefs;

/**
 * @brief   Block解析中间件，辅助解析各个C++块
 */
public abstract class Block {
    private String      mContent   = null;
    private Block       mParent    = null;
    private List<Block> mSons      = new ArrayList<Block>();
    private ClassDefs   mClassDefs = null;
    private TypeDefs    mTypeDefs  = null;

    public Block(String content, ClassDefs cls, TypeDefs types) {
        mContent    = content;
        mClassDefs  = cls;
        mTypeDefs   = types;
    }
    
    public abstract void parse();

    public String getContent() {
        return mContent;
    }

    public void setContent(String mContent) {
        this.mContent = mContent;
    }

    public ClassDefs getClassDefs() {
        return mClassDefs;
    }

    public void setClassDefs(ClassDefs mClassDefs) {
        this.mClassDefs = mClassDefs;
    }

    public TypeDefs getTypeDefs() {
        return mTypeDefs;
    }

    public void setTypeDefs(TypeDefs mTypeDefs) {
        this.mTypeDefs = mTypeDefs;
    }
    
    public Block getParent() {
        return mParent;
    }

    public void setParent(Block parent) {
        this.mParent = parent;
    }

    public List<Block> getSons() {
        return mSons;
    }

    public void addSon(Block Son) {
        mSons.add(Son);
    }
    
    public void setSons(List<Block> Sons) {
        this.mSons = Sons;
    }

}
