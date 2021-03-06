package com.LangStack.Transport;


public class TcpMessage
{
    public static final int rpcTypeCall     = 0;       ///< RPC调用包
    public static final int rpcTypeReturn   = 1;       ///< RPC调用返回包
    
    static int getLength(byte[] buf)
    {
        return ByteOrder.bytesToUshort(buf, 6);
    }
    
    static int getType(byte[] buf)
    {
        return ByteOrder.bytesToUshort(buf, 4);
    }
    
}
