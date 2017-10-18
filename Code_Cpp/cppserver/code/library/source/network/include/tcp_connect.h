/*----------------- tcp_connect.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2012/3/26 18:53:46
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "socket.h"
#include "socketStartup.h"
#include "socketBuffer.h"
#include "threads/thread.h"
#include "threads/condEvent.h"
#include "libevent.h"
/*************************************************************/
class tcp_connect	: public CSocketBuffer
{
private:
	event_base* 	m_event_base;	//�����¼�
	event			m_event_read;
	event			m_event_write;
	event			m_event_timeout;

private:
	ulong			m_uConnectAddr;	//���ӵ�ַ
	uint16			m_uConnectPort;	//���Ӷ˿�

private:
	volatile bool	m_bConnected;
	volatile bool	m_bDisconnect;
	volatile bool	m_bShutdown;

private:
	timeval			m_timeout;
	uint64			m_uTimeoutConnect;	//���ӳ�ʱ(��)
	uint64			m_uTimeoutRead;		//��ȡ��ʱʱ��(��)
	uint64			m_uTimeoutWrite;	//д�볬ʱʱ��(��)
	uint64			m_uConnectTime;		//����ʱ��
	uint64			m_uLastReadTime;	//����ȡʱ��
	uint64			m_uLastWriteTime;	//���дʱ��
#ifdef _NET_USE_TIME_WRITE
	timeval			m_time_write;
#endif//_NET_USE_TIME_WRITE

private:
	CSocketStartup	m_clStartup;
	CSocket			m_clSocket;
	CThread			m_clThread;		//�̶߳���
	CCondEvent		m_clExitEvent;	//�˳�ѭ���¼�

public:
	inline bool	connected	()const	{	return m_bConnected;											}//���ӳɹ�
	inline bool	disconnected()const	{	return (!m_clSocket.is_valid());								}//���ӶϿ�
	inline bool	connecting	()const	{	return((m_clSocket.is_valid()&&!m_bConnected) ? true : false);	}//��������

public:
	tcp_connect();
	virtual~tcp_connect();

private:
	bool	start	();

public:
	void	shutdown();

public:
	void	set_timeout	(uint64 _timeout_connect,uint64 _timeout_read,uint64 _timeout_write);
#ifdef _NET_USE_TIME_WRITE
	void	set_time_write(long _ms);
#endif//_NET_USE_TIME_WRITE
	bool	connect		(uint16 uPort,char* pAddress = NULL);
	bool	connect		(uint16 uPort,ulong uAddr = INADDR_NONE);
	bool	reconnect	();
	void	disconnect	();
	bool	send		(const char* pData,int nSize);

protected:
	/*-----[ ����֪ͨ ]*/ 
	virtual void	onConnect		(bool bConnect);
	/*-----[ �Ͽ�֪ͨ ]*/ 
	virtual void	onDisconnect	();
	/*-----[ ���ݰ�֪ͨ ]*/ 
	virtual void	onPacket		(char* pBuffer,const uint16& nSize);
	/*-----[ ����� ]*/ 
	virtual bool	onProcessPackage();
	/*-----[ ��ʱ֪ͨ ]*/ 
	virtual void	onTimeoutWrite	(){}
	virtual void	onTimeoutRead	(){}

private:
	bool	set_read	();
	bool	set_write	();
	bool	set_close	();
	bool	set_timeout	();
	void	del_read	();
	void	del_timeout	();
	bool	loop_break	();

private:
	void	onRead	();
	void	onWrite	();
	void	onTimeout();
	void	onClose	();

private:
	bool	on_read	();
	bool	on_write();

private:
	static void		on_read		(evutil_socket_t fd, short events, void *arg);
	static void		on_write	(evutil_socket_t fd, short events, void *arg);
	static void		on_timeout	(evutil_socket_t fd, short events, void *arg);

private:
	static THREAD_RETURN	__thread_proc(void* pParam);
};
