/*----------------- socket.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/3/5 17:03:09
*--------------------------------------------------------------
*********windows
*需要lib库【ws2_32.lib】
*********linux
*需要lib库【stdc libstdc.so】
*********
htonl()：把32位值从主机字节序转换成网络字节序
htons()：把16位值从主机字节序转换成网络字节序
ntohl()：把32位值从网络字节序转换成主机字节序
ntohs()：把16位值从网络字节序转换成主机字节序
*********
对已经关闭的soket调用send()
win32: 返回错误，没其他后果。
linux: 产生信号SIGPIPE，linux对该信号的默认操作是关闭进程
解决方法：预先注册SIGPIPE信号，
#include <signal.h>
void sig_pipe(int signal) { 处理}
在程序启动时，进行注册
signal(SIGPIPE, sig_pipe);
*------------------------------------------------------------*/
#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#ifdef WIN32
#include <winsock2.h>
#include <mswsock.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#else//WIN32
#include <netinet/in.h>//包含了<sys/socket.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>//系统类型定义
#include <arpa/inet.h>
#include <fcntl.h>//阻塞模式头文件
#include <netdb.h>
#include <unistd.h>//close函数
#endif//WIN32
#include <errno.h>//包含此文件就会设置errno变量【int geterror()】
#include "basic/basicTypes.h"
#include "basic/memoryFunctions.h"
#include "platform/platform.h"
#include "socketStartup.h"
/*************************************************************/
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus
//****************************************************************
#ifdef WIN32
typedef int	socklen_t;
#else//WIN32
typedef long SOCKET;
#define INVALID_SOCKET	(SOCKET)(~0)
#ifndef INADDR_NONE
#define INADDR_NONE		0xFFFFFFFF
#endif
#endif//WIN32
//#############################################################
//##############################
//#############################################################
class CSocket
{
private:
	SOCKET		m_socket;

public:
	CSocket(SOCKET _socket = INVALID_SOCKET);
	virtual~CSocket(){}

public:
	inline SOCKET	get_socket	()						{	return m_socket;										}
	inline bool		is_valid	()const					{	return (m_socket != INVALID_SOCKET && m_socket != 0);	}
	inline CSocket&	operator=	(const CSocket&_socket)
	{
		m_socket	= _socket.m_socket;
		return *this;
	}
	inline CSocket&	operator=	(const SOCKET&_socket)
	{
		m_socket	= _socket;
		return *this;
	}

public:
	//--- 中断socket事件
	int		shut_down	(int how);
	//--- 关闭socket
	void	close_socket();

#ifdef WIN32
public:
	//--- 创建win32_socket
	SOCKET	Socket		(int af,int type,int protocol,LPWSAPROTOCOL_INFOA lpProtocolInfo = NULL,GROUP g = 0,DWORD dwFlags = WSA_FLAG_OVERLAPPED);
	//--- 绑定select事件
	int		SelectEvent	(WSAEVENT hEventObject,long lNetworkEvents);
	//--- IO控制
	int		Ioctl		(DWORD dwIoControlCode,LPVOID lpvInBuffer,DWORD cbInBuffer,LPVOID lpvOutBuffer,DWORD cbOutBuffer,LPDWORD lpcbBytesReturned,LPWSAOVERLAPPED lpOverlapped,LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
	//--- 接受
	SOCKET	Accept		(struct sockaddr* addr,int*addrlen,LPCONDITIONPROC lpfnCondition,DWORD dwCallbackData);	
	//--- 链接
	int		Connect		(const struct sockaddr * name,int namelen,LPWSABUF lpCallerData, LPWSABUF lpCalleeData,LPQOS lpSQOS,LPQOS lpGQOS);
	//--- 发送消息
	int		Send		(LPWSABUF lpBuffers,DWORD dwBufferCount,LPDWORD lpNumberOfBytesSent,DWORD dwFlags,LPWSAOVERLAPPED lpOverlapped,LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
	//--- 接收消息
	int		Recv		(LPWSABUF lpBuffers,DWORD dwBufferCount,LPDWORD lpNumberOfBytesRecvd,LPDWORD lpFlags,LPWSAOVERLAPPED lpOverlapped,LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
	//--- 枚举网络事件
	int		EnumEvents	(WSAEVENT hEventObject,LPWSANETWORKEVENTS lpNetworkEvents);
#endif//WIN32

public:
	//--- 创建socket
	SOCKET	socket		(int af,int type,int protocol);
	//--- 绑定地址
	int		bind		(const struct sockaddr *addr,int namelen);
	int		bind		(uint16 sin_port,ulong ulAddr = INADDR_ANY);
	//--- 监听
	int		listen		(int backlog);
	//--- 接受
	int		accept		(struct sockaddr *addr, socklen_t *addrlen);
	//--- 选择模型
	int		select		(fd_set *readfds,fd_set*writefds,fd_set*exceptfds,struct timeval *timeout);
	//--- 链接
	int		connect		(const struct sockaddr *name,int namelen);
	int		connect		(uint16 sin_port,ulong ulAddr);
	//--- 是否接受错误
	bool	accept_error	();
	//--- 是否链接错误
	bool	connect_error	();
	//--- 是否拒绝链接
	bool	connect_refused	();
	//--- 是否需要重链
	bool	connect_reset	();
	//--- 是否读写错误
	bool	rw_error		();
	//--- 发送
	int		send		(const char * buf,int len);
	//--- 接收
	int		recv		(char * buf,int len);
	//--- udp发送
	int		sendto		(const char * buf,int len,int flags,const struct sockaddr *to,socklen_t tolen);
	//--- udp接收
	int		recv_from	(char * buf, int len, int flags, struct sockaddr * from,socklen_t* fromlen);

public:
	//--- io阻塞控制
	int		ioctl_block	(bool _block);
	//--- 是否阻塞模式
	bool	is_io_block	();
	//--- 设置socket选项
	int		set_sock_opt(int level,int optname,const char * optval, socklen_t optlen);
	//--- 获得socket选项
	int		get_sock_opt(int level,int optname,char * optval,socklen_t* optlen);

public:
	//--- 
	void	FD_set	(fd_set&_set);
	int		FD_isset(fd_set&_set);

public:
	//--- 获得socket地址
	int		get_socket_name	(struct sockaddr&_addr);
	//--- 获得socket链接地址
	int		get_peer_name	(struct sockaddr&_addr);
	//--- 获得远程链接地址
	ulong	get_remote_address	();
	//--- 获得远程链接IP
	const char*get_remote_ip	();

public:
	//--- 获得主机名
	static const char*	get_host_name	();
	//--- 获得主机名IP
	static const char*	get_host_ip		(const char* _name = NULL);
	//--- 获得主机名地址
	static ulong		get_host_address(const char* _name = NULL);
};
//****************************************************************
#ifdef __cplusplus
}
#endif//__cplusplus
