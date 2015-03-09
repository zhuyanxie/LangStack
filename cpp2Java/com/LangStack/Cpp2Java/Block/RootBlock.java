package com.LangStack.Cpp2Java.Block;

import com.LangStack.Cpp2Java.ClassDefs;
import com.LangStack.Cpp2Java.TypeDefs;

public class RootBlock extends Block {
    private String mComment;
    
    public RootBlock(String content, ClassDefs cls, TypeDefs types) {
        super(content, cls, types);
    }

    @Override
    public void parse() {
        
    }

}
