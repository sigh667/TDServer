/*----------------- socket.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2012/3/5 17:03:09
*--------------------------------------------------------------
*********windows
*��Ҫlib�⡾ws2_32.lib��
*********linux
*��Ҫlib�⡾stdc libstdc.so��
*********
htonl()����32λֵ�������ֽ���ת���������ֽ���
htons()����16λֵ�������ֽ���ת���������ֽ���
ntohl()����32λֵ�������ֽ���ת���������ֽ���
ntohs()����16λֵ�������ֽ���ת���������ֽ���
*********
���Ѿ��رյ�soket����send()
win32: ���ش���û���������
linux: �����ź�SIGPIPE��linux�Ը��źŵ�Ĭ�ϲ����ǹرս���
���������Ԥ��ע��SIGPIPE�źţ�
#include <signal.h>
void sig_pipe(int signal) { ����}
�ڳ�������ʱ������ע��
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
#include <netinet/in.h>//������<sys/socket.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>//ϵͳ���Ͷ���
#include <arpa/inet.h>
#include <fcntl.h>//����ģʽͷ�ļ�
#include <netdb.h>
#include <unistd.h>//close����
#endif//WIN32
#include <errno.h>//�������ļ��ͻ�����errno������int geterror()��
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
	//--- �ж�socket�¼�
	int		shut_down	(int how);
	//--- �ر�socket
	void	close_socket();

#ifdef WIN32
public:
	//--- ����win32_socket
	SOCKET	Socket		(int af,int type,int protocol,LPWSAPROTOCOL_INFOA lpProtocolInfo = NULL,GROUP g = 0,DWORD dwFlags = WSA_FLAG_OVERLAPPED);
	//--- ��select�¼�
	int		SelectEvent	(WSAEVENT hEventObject,long lNetworkEvents);
	//--- IO����
	int		Ioctl		(DWORD dwIoControlCode,LPVOID lpvInBuffer,DWORD cbInBuffer,LPVOID lpvOutBuffer,DWORD cbOutBuffer,LPDWORD lpcbBytesReturned,LPWSAOVERLAPPED lpOverlapped,LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
	//--- ����
	SOCKET	Accept		(struct sockaddr* addr,int*addrlen,LPCONDITIONPROC lpfnCondition,DWORD dwCallbackData);	
	//--- ����
	int		Connect		(const struct sockaddr * name,int namelen,LPWSABUF lpCallerData, LPWSABUF lpCalleeData,LPQOS lpSQOS,LPQOS lpGQOS);
	//--- ������Ϣ
	int		Send		(LPWSABUF lpBuffers,DWORD dwBufferCount,LPDWORD lpNumberOfBytesSent,DWORD dwFlags,LPWSAOVERLAPPED lpOverlapped,LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
	//--- ������Ϣ
	int		Recv		(LPWSABUF lpBuffers,DWORD dwBufferCount,LPDWORD lpNumberOfBytesRecvd,LPDWORD lpFlags,LPWSAOVERLAPPED lpOverlapped,LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
	//--- ö�������¼�
	int		EnumEvents	(WSAEVENT hEventObject,LPWSANETWORKEVENTS lpNetworkEvents);
#endif//WIN32

public:
	//--- ����socket
	SOCKET	socket		(int af,int type,int protocol);
	//--- �󶨵�ַ
	int		bind		(const struct sockaddr *addr,int namelen);
	int		bind		(uint16 sin_port,ulong ulAddr = INADDR_ANY);
	//--- ����
	int		listen		(int backlog);
	//--- ����
	int		accept		(struct sockaddr *addr, socklen_t *addrlen);
	//--- ѡ��ģ��
	int		select		(fd_set *readfds,fd_set*writefds,fd_set*exceptfds,struct timeval *timeout);
	//--- ����
	int		connect		(const struct sockaddr *name,int namelen);
	int		connect		(uint16 sin_port,ulong ulAddr);
	//--- �Ƿ���ܴ���
	bool	accept_error	();
	//--- �Ƿ����Ӵ���
	bool	connect_error	();
	//--- �Ƿ�ܾ�����
	bool	connect_refused	();
	//--- �Ƿ���Ҫ����
	bool	connect_reset	();
	//--- �Ƿ��д����
	bool	rw_error		();
	//--- ����
	int		send		(const char * buf,int len);
	//--- ����
	int		recv		(char * buf,int len);
	//--- udp����
	int		sendto		(const char * buf,int len,int flags,const struct sockaddr *to,socklen_t tolen);
	//--- udp����
	int		recv_from	(char * buf, int len, int flags, struct sockaddr * from,socklen_t* fromlen);

public:
	//--- io��������
	int		ioctl_block	(bool _block);
	//--- �Ƿ�����ģʽ
	bool	is_io_block	();
	//--- ����socketѡ��
	int		set_sock_opt(int level,int optname,const char * optval, socklen_t optlen);
	//--- ���socketѡ��
	int		get_sock_opt(int level,int optname,char * optval,socklen_t* optlen);

public:
	//--- 
	void	FD_set	(fd_set&_set);
	int		FD_isset(fd_set&_set);

public:
	//--- ���socket��ַ
	int		get_socket_name	(struct sockaddr&_addr);
	//--- ���socket���ӵ�ַ
	int		get_peer_name	(struct sockaddr&_addr);
	//--- ���Զ�����ӵ�ַ
	ulong	get_remote_address	();
	//--- ���Զ������IP
	const char*get_remote_ip	();

public:
	//--- ���������
	static const char*	get_host_name	();
	//--- ���������IP
	static const char*	get_host_ip		(const char* _name = NULL);
	//--- �����������ַ
	static ulong		get_host_address(const char* _name = NULL);
};
//****************************************************************
#ifdef __cplusplus
}
#endif//__cplusplus
