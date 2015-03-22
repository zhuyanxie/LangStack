package com.LangStack.Cpp2Java;

import java.io.PrintStream;

public class MemberDefs {
    private String mName;           ///< 成员名
    private String mCppClassName;   ///< 成员所属类名
    private String mCppType;        ///< 成员类型
    private String mComment;        ///< 成员注释
    private boolean mIsPublic;      ///< 是否public
    private boolean mIsStatic;      ///< 是否为静态成员
    private int mMemType;           ///< 解析成员类型
    private TypeDefs mTypes;        ///< 类型定义引用
    private String mBlock;          ///< 一个待解析的成员字符串
    private String mFile;           ///< 定义文件
    private int mLine;              ///< 对应行数

    public MemberDefs(String block, String file, int line) {
        mBlock = block;
        mFile = file;
        mLine = line;
    }

    public MemberDefs(TypeDefs types, boolean isPublic) {
        mTypes = types;
        mIsPublic = isPublic;
    }
    
    public void parse() {
        parseBlock();
        mMemType = mTypes.getCppType(mCppType);
    }
    
    /**
     * @brief       简单的解析，不支持特殊宏定义
     */
    private void parseBlock() {
        mBlock = mBlock.replaceAll("\\s+", " ").trim();
        int staticIndex = mBlock.indexOf("static");
        if (staticIndex != -1) {
            mIsStatic = true;
            staticIndex = staticIndex + "static ".length();
            mBlock = mBlock.substring(staticIndex,  mBlock.length());
        } else {
            staticIndex = 0;
        }
        
        String []blocks = mBlock.split("\\s+");
        mName = blocks[blocks.length - 1];
        mCppType = "";
        for (int i = 0; i < blocks.length - 1; ++i) {
            mCppType = mCppType + ((i == 0) ? "" : " ") + blocks[i];
        }
        mCppType.trim();
    }
    
    public void genJava(PrintStream p) {
        if (mComment == null) {
            p.println("    ///< " + mComment);
        }
        p.print("    " + (isPublic() ? "public" : "private"));
        p.print(isStatic() ? " static " : " ");
        p.println(mTypes.getJavaType(mMemType) + " " + mName + ";");
    }
    
    public void genCppGetSet(PrintStream p) {
        p.printf("    inline static void set%s(ls::IReflection *obj, void *value)\r\n", mName);
        p.println("    {");
        p.printf("        ((%s*)(obj))->NAME = *(%s*)value;\r\n", mCppClassName, mCppType);
        p.println("    }");
        
        p.printf("    inline static void get%s(ls::IReflection *obj)\r\n", mName);
        p.println("    {");
        p.printf("        return (void*)&(((%s*)(obj))->NAME)\r\n", mCppClassName);
        p.println("    }");
    }
    
    public void genCppReflectRegist(PrintStream p) {
        p.printf("    ::ls::CMetaFactory::instance()->registerMember("
                + "%s::getJavaName(), %s, ::ls::Type2MetaDataType< %s >()(), "
                + "%s::set%s, %s::get%s\r\n", 
                mCppClassName, mName, mCppType, mName, mName);;
    }
    
    public String getName() {
        return mName;
    }
    public void setName(String name) {
        this.mName = name;
    }
    public String getCppType() {
        return mCppType;
    }    
    public boolean isPublic() {
        return mIsPublic;
    }
    public void setPublic(boolean isPublic) {
        this.mIsPublic = isPublic;
    }
    public boolean isStatic() {
        return mIsStatic;
    }
    public void setStatic(boolean isStatic) {
        this.mIsStatic = isStatic;
    }
    public void setCppType(String cppType) {
        this.mCppType = cppType;
    }
    public String getComment() {
        return mComment;
    }
    public void setComment(String comment) {
        this.mComment = comment;
    }
    public int getType() {
        return mMemType;
    }
    public void setType(int mTokenType) {
        this.mMemType = mTokenType;
    }

    public void setTypes(TypeDefs types) {
        mTypes = types;
    }

}
