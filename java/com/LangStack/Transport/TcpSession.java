package com.LangStack.Transport;

import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Arrays;

import com.LangStack.Rpc.RpcCall;
import com.LangStack.Rpc.RpcCore;

/**
 * @brief	java有且只有一个socket与c++通信，故直接使用阻塞socket
 */
public class TcpSession extends TransSession implements Runnable 
{
    public enum TcpSessionState
    {
        Connecting,                  ///< 连接中
        ConnectFail,                 ///< 连接失败
        Connected,                   ///< 连接成功
        Disconnected,                ///< 断开连接
        Unvalid,                     ///< 非法状态
    }

    private static final int     RPC_HEADER_LENGTH = 12;

    private int              mPort             = 16759;
    private Socket           mSocket           = null;
    private TcpSessionState  mState            = TcpSessionState.Unvalid;
    private byte[]           mBuf              = null;
    private int              mLen              = 0;
    private Thread           mThread           = null;

    /**
     * @brief       创建一个本地环回的tcp client
     * @param       port        端口
     */
    public TcpSession(int port)
    {
        mPort = port;
        mBuf = new byte[1024 * 1024];
        mLen = 0;
        reConnect();
        mThread = new Thread(this);
        mThread.start();
    }

    /**
     * @brief       线程体
     */
    public void run()
    {
        while (mThread.getState() == Thread.State.RUNNABLE)
        {
            switch (mState)
            {
            case Connecting:
                checkConnect();
                break;
            case ConnectFail:
                reConnect();
                break;
            case Connected:
                doRead();
                break;
            case Disconnected:
                reConnect();
                break;
            default:
                reConnect();
                break;
            }
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    /**
     * @brief       检查tcp链路连接
     */
    public void checkConnect()
    {
        if (mSocket.isConnected())
        {
            mState = TcpSessionState.Connected;
        }
        else if (mSocket.isClosed())
        {
            mState = TcpSessionState.Disconnected;
        }
    }

    /**
     * @brief       读取tcp数据（阻塞）
     */
    public void doRead()
    {
        while (true)
        {
            try {
                InputStream is = mSocket.getInputStream();
                int len = is.read(mBuf, mLen, 1024 * 1024 - mLen);
                if (len == -1)
                {
                    mState = TcpSessionState.Disconnected;
                    break;
                }
                mLen = mLen + len;

                /// 处理接收缓存
                dealMessage();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
    
    /**
     * @brief       重新连接
     */
    public void reConnect()
    {
        try {
            mSocket = new Socket("127.0.0.1", mPort);
            mSocket.setTcpNoDelay(true);
            mState = TcpSessionState.Connected;
        } catch (UnknownHostException e) {
            mState = TcpSessionState.ConnectFail;
            e.printStackTrace();
        } catch (IOException e) {
            mState = TcpSessionState.ConnectFail;
            e.printStackTrace();
        } catch (Exception e) {  
            mState = TcpSessionState.ConnectFail;
            e.printStackTrace();
        }
    }

    /**
     * @brief       检测消息合法性
     * @return      当前缓存消息是否合法
     */
    private boolean checkMessageValid()
    {
        if (mLen < RPC_HEADER_LENGTH)
        {
            return false;
        }

        int offset = 0;
        while (mBuf[offset] != 'L' || mBuf[offset + 1] != 'R'
                || mBuf[offset + 2] != 'P' || mBuf[offset + 3] != 'C')
        {
            ++offset;
            if (offset + 4 >= mLen)
            {
                break;
            }
        }

        if (offset != 0)
        {
            mBuf = Arrays.copyOfRange(mBuf, offset, mLen);
            mLen = mLen - offset;
        }
        if (mLen < RPC_HEADER_LENGTH)
        {
            return false;
        }

        return true;
    }

    /**
     * @brief       处理消息
     */
    private void dealMessage()
    {
	    while (checkMessageValid())
	    {
	        int length = TcpMessage.getLength(mBuf);
	        int type   = TcpMessage.getType(mBuf);
	        if (length + RPC_HEADER_LENGTH > mLen)
	        {
	            break;
	        }

	        String message = null;
            try {
                message = new String(Arrays.copyOfRange(
                        mBuf, RPC_HEADER_LENGTH, length + RPC_HEADER_LENGTH), 
                        "ISO-8859-1");
            } catch (UnsupportedEncodingException e) {
                e.printStackTrace();
            }
            
	        RpcCall call = RpcCall.deserial(message);
	        if (call != null)
	        {
	            if (type == TcpMessage.rpcTypeCall)
	            {
                    RpcCore.instance().onRpcCall(call);
	            }
	            else
	            {
	                RpcCore.instance().onRpcReturn(call);
	            }
	        }
	        for (int i = length + RPC_HEADER_LENGTH, j = 0; i < mLen; ++i, ++j)
	        {
	            mBuf[j] = mBuf[i];
	        }
	        mLen = mLen - length - RPC_HEADER_LENGTH;
	    }
    }

    /**
     * @brief       发送RPC调用
     * @param       call        RPC调用
     * @param       type        调用类型
     * @return
     */
    @Override public boolean send(RpcCall call, int type)
    {
        String str = call.serial();
        TcpHeader header = new TcpHeader(type, str.length());
        try {
            mSocket.getOutputStream().write(header.toBytes());
            mSocket.getOutputStream().write(str.getBytes("ISO-8859-1"));
            mSocket.getOutputStream().flush();
            
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
        return true;
    }
}

