package com.LangStack.Transport;

import java.io.UnsupportedEncodingException;
import java.nio.ByteBuffer;


/***
 * @brief   Rpc消息头定义
 */
public class TcpHeader
{
/* @see c++ 定义 struct RpcHeader 
    uint32_t    flag;                       ///< RPC包标记
    uint16_t    type;                       ///< 包类型，见RpcType
    uint16_t    length;                     ///< 不包括包头的包数据长度
    uint16_t    checkSum;                   ///< 校验和(备用)
    uint16_t    reversed;                   ///< 备用字段(备用)
*/
    ByteBuffer bb = null;
    
    public TcpHeader(int type, int len)
    {
        bb = ByteBuffer.allocate(12);
        try {
            bb.put("LRPC".getBytes("ISO-8859-1"));
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }
        bb.put(ByteOrder.shortToBytes((short)type));
        bb.put(ByteOrder.shortToBytes((short)len));
        bb.put(ByteOrder.shortToBytes((short)0));
        bb.put(ByteOrder.shortToBytes((short)0));
    }
    
    public byte[] toBytes()
    {
        return bb.array();
    }
}