/******************************************************************************
Copyright (C) 2015 zhuyanxie

　　Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished
to do so, subject to the following conditions:

　　The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#ifndef _LANGUAGE_STACK_SOCKET_HEADER_H__
#define _LANGUAGE_STACK_SOCKET_HEADER_H__


#ifdef WIN32 /// windows

#include <ws2tcpip.h>
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")

typedef int socklen_t;

#define GET_NET_ERROR() WSAGetLastError()
#define IS_CONNECT_INPROGRESS(code) (code==WSAEWOULDBLOCK)
#define IS_ACCEPT_IGNORABLE(code) ((code==WSAEWOULDBLOCK)||(code==WSAECONNRESET)||(code==WSAEINTR)||(code==WSAEINPROGRESS))
#define IS_DISCONECTED(code) ((code==WSAENETRESET)||(code==WSAECONNABORTED)||(code==WSAETIMEDOUT)||(code==WSAECONNRESET))
#define IS_IGNORABLE_ERROR(code) (code==WSAEWOULDBLOCK)
#define IS_SEND_IGNORABLE(code) ((code==WSAEWOULDBLOCK)||(code==WSAEINTR))
#define IS_RECV_IGNORABLE(code) ((code==WSAEWOULDBLOCK)||(code==WSAEINTR))
#define IS_RECVFROM_IGNORABLE(code) ((code==WSAEWOULDBLOCK)||(code==WSAECONNRESET))
#define IS_SENDTO_IGNORABLE(code) ((code==WSAEWOULDBLOCK)||(code==WSAECONNRESET))
#define IS_INVALID_SOCKET(code) (code==WSAENOTSOCK)
#define IS_INTERRUPTED(code) (code==WSAEINTR)

#else /// *nux
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <netinet/in.h>
//#ifdef __mac__
//#else
//#include <linux/sockios.h>
//#endif
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <fcntl.h>
#include <string.h>
#include <net/if.h>

#define GET_NET_ERROR() errno
#define IS_CONNECT_INPROGRESS(code) (code==EINPROGRESS)
#define IS_ACCEPT_IGNORABLE(code) ((code==EAGAIN)||(code==ECONNABORTED)||(code==EINTR))
#define IS_DISCONECTED(code) ((code==ECONNREFUSED)||(code==ECONNRESET)||(code==EPIPE)||(code==ENOTCONN))
#define IS_IGNORABLE_ERROR(code) ((code == EAGAIN) || (code == EINTR))
#define IS_SEND_IGNORABLE(code) ((code==EAGAIN)||(code==EWOULDBLOCK)||(code == EINTR))
#define IS_RECV_IGNORABLE(code) ((code==EAGAIN)||(code==EWOULDBLOCK)||(code == EINTR))
#define IS_RECVFROM_IGNORABLE(code) ((code==EAGAIN)||(code==EINTR)||(code==ECONNREFUSED))
#define IS_SENDTO_IGNORABLE(code) ((code==EAGAIN)||(code==EWOULDBLOCK)||(code==EINTR)||(code==EPIPE)||(code==ECONNRESET))
#define IS_INVALID_SOCKET(code) (code==EBADF)
#define IS_INTERRUPTED(code) (code==EINTR)

#endif // WIN32 


#endif /*_LANGUAGE_STACK_SOCKET_HEADER_H__*/


