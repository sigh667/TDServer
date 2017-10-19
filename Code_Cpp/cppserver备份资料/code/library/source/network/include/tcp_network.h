/*----------------- tcp_network.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2012/3/26 18:54:46
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <vector>
#include "socket.h"
#include "socketStartup.h"
#include "socketBuffer.h"
#include "tcp_context.h"
/*************************************************************/
//#############################################################
//##############################
//#############################################################
class tcp_network
{
protected:
	typedef CMemoryPool<tcp_context>	CONTEXT_POOL;
	typedef CONTEXT_POOL::iterator		CONTEXT_ITER;
	typedef std::vector<tcp_thread*>	VECTOR_THREAD;

protected:
	CSocketStartup		m_clStartup;
	CCondEvent			m_clExitEvent;		//�˳�ѭ���¼�

protected:
	event_base*			m_pListenerBase;	//��������
	event				m_evAccept;			//�����¼�
	CSocket				m_clListenerSocket;	//����socket
	CThread				m_clListenerThread;	//�����߳�
	VECTOR_THREAD		m_vecWorkThread;	//�����̳߳�

protected:
	volatile bool		m_bStarted;			//�Ƿ�������
	volatile bool		m_bShutdown;		//�Ƿ�ر�
	volatile bool		m_bRejectRead;		//�ܾ���ȡ
	volatile bool		m_bRejectConnect;	//�Ƿ�ܾ�����
	
protected:
	uint16				m_send_buffer_max;	//����ͻ���������

protected:
	timeval				m_timeout;
	uint64				m_timeout_connect;	//���ӳ�ʱ(��)
	uint64				m_timeout_read;		//��ȡ��ʱʱ��(��)
	uint64				m_timeout_write;	//д�볬ʱʱ��(��)


#ifdef _NET_USE_TIME_WRITE
	timeval				m_time_write;
#endif//_NET_USE_TIME_WRITE

protected:
	CONTEXT_POOL		m_poolContext;		//�ͻ������ı�

public:
	inline bool			isStarted	()	{	return m_bStarted;	}

public:
	tcp_network();
	virtual~tcp_network();

public:
	/*----->{ �ܾ����� }*/ 
	virtual void		rejectConnect	(bool bReject);
	/*----->{ �ܾ�����Ϣ(�����ٶ�) }*/ 
	virtual void		rejectRead		();
	virtual void		setSendBufferMax(uint16 _size);
	virtual void		set_timeout		(uint64 _timeout_connect,uint64 _timeout_read,uint64 _timeout_write);
	
#ifdef _NET_USE_TIME_WRITE
	virtual void		set_time_write	(long _ms);
#endif//_NET_USE_TIME_WRITE
	virtual size_t		getConnectCount	();

public:
	/*-----[ ��������(uOvertime = 0��������ʱ�߳�,uPort == 0ʱ�������ӷ���) ]*/ 
	virtual bool		start			(uint16 uPort);
	/*-----[ �رշ��� ]*/ 
	virtual void		shutdown		();
	/*-----[ �Ͽ� ]*/ 
	virtual void		disconnect		(tcp_context* pContext);
	/*-----[ �Ͽ��������� ]*/ 
	virtual void		disconnectAll	();
	/*-----[ �������� ]*/ 
	virtual bool		send			(tcp_context* pContext,const char* pBuffer,int32 nSize);
	/*-----[ ���� ]*/ 
	virtual tcp_context*connect			(uint16 uPort,const char* pAddress = NULL);
	/*-----[ ���� ]*/ 
	virtual tcp_context*connect			(uint16 uPort,ulong uAddr = INADDR_NONE);

private:
	bool	create_event	(uint16 uPort);
	bool	setup_listner	(uint16 uPort);
	bool	setup_workers	();
	bool	setup_timeout	();

private:
	void	shutdown_timeout();
	void	shutdown_listner();
	void	shutdown_workers();
	void	close_event		();

public:
	/*----->{ �����ص� }*/ 
	virtual void		onStart			();
	/*----->{ �رջص� }*/ 
	virtual void		onShutdown		();

public:
	/*-----[ ������֪ͨ ]*/ 
	virtual void		onConnect		(tcp_context* pContext);
	/*-----[ �Ͽ�֪ͨ ]*/ 
	virtual void		onDisconnect	(tcp_context* pContext);
	/*-----[ ��ʱ֪ͨ ]*/ 
	virtual void		onTimeoutRead	(tcp_context* pContext);
	/*----->{ ���ͳ�ʱ }*/ 
	virtual void		onTimeoutWrite	(tcp_context* pContext);
	/*-----[ ���ݰ�֪ͨ ]*/ 
	virtual void		onPacket		(tcp_context* pContext,char* pBuffer,const uint16& nSize);

protected:
	/*-----[ ����� ]*/ 
	virtual bool		onProcessPackage(tcp_context* pContext,CNetBuffer* pBuffer);
	/*-----[ ������ɻص� ]*/ 
	virtual	void		callback_write	(tcp_context* pContext,int nSize);

protected:
	//--- ���һ�������߳�
	virtual tcp_thread*	getWorkThread	();
	//--- ��ʼ��������
	virtual void		initContext		();

private:
	bool	init_connect(tcp_context* pContext);
	bool	set_read	(tcp_context* pContext);
	bool	set_write	(tcp_context* pContext);
	bool	set_timeout	(tcp_context* pContext);
	bool	set_close	(tcp_context* pContext);
	void	del_read	(tcp_context* pContext);
	void	del_timeout	(tcp_context* pContext);
	bool	loop_break	(event_base*_event);

protected:
	virtual bool	onAccept	(SOCKET fd);
	virtual void	onRead		(tcp_context* pContext);
	virtual void	onWrite		(tcp_context* pContext);
	virtual void	onClose		(tcp_context* pContext);
	virtual void	onTimeout	(tcp_context* pContext);

private:
	virtual bool	on_read		(tcp_context* pContext);
	virtual bool	on_write	(tcp_context* pContext);

private:
	static void	on_accept	(evutil_socket_t fd,short events,void * arg);
	static void	on_read		(evutil_socket_t fd,short events,void * arg);
	static void	on_write	(evutil_socket_t fd,short events,void * arg);
	static void	on_timeout	(evutil_socket_t fd,short events,void * arg);

private:
	//--- ���߳�ʹ�ô�����������
	static int32 _qsort_thread	(const void *p1, const void *p2);

private:
	static THREAD_RETURN	_listener_thread_	(void* pParam);
	static THREAD_RETURN	_working_thread_	(void* pParam);
};
