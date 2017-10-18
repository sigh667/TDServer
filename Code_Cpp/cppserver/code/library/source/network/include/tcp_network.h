/*----------------- tcp_network.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
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
	CCondEvent			m_clExitEvent;		//退出循环事件

protected:
	event_base*			m_pListenerBase;	//监听根基
	event				m_evAccept;			//接受事件
	CSocket				m_clListenerSocket;	//监听socket
	CThread				m_clListenerThread;	//监听线程
	VECTOR_THREAD		m_vecWorkThread;	//工作线程池

protected:
	volatile bool		m_bStarted;			//是否已启动
	volatile bool		m_bShutdown;		//是否关闭
	volatile bool		m_bRejectRead;		//拒绝读取
	volatile bool		m_bRejectConnect;	//是否拒绝链接
	
protected:
	uint16				m_send_buffer_max;	//最大发送缓冲区数量

protected:
	timeval				m_timeout;
	uint64				m_timeout_connect;	//链接超时(秒)
	uint64				m_timeout_read;		//读取超时时间(秒)
	uint64				m_timeout_write;	//写入超时时间(秒)


#ifdef _NET_USE_TIME_WRITE
	timeval				m_time_write;
#endif//_NET_USE_TIME_WRITE

protected:
	CONTEXT_POOL		m_poolContext;		//客户上下文表

public:
	inline bool			isStarted	()	{	return m_bStarted;	}

public:
	tcp_network();
	virtual~tcp_network();

public:
	/*----->{ 拒绝连接 }*/ 
	virtual void		rejectConnect	(bool bReject);
	/*----->{ 拒绝读消息(不能再读) }*/ 
	virtual void		rejectRead		();
	virtual void		setSendBufferMax(uint16 _size);
	virtual void		set_timeout		(uint64 _timeout_connect,uint64 _timeout_read,uint64 _timeout_write);
	
#ifdef _NET_USE_TIME_WRITE
	virtual void		set_time_write	(long _ms);
#endif//_NET_USE_TIME_WRITE
	virtual size_t		getConnectCount	();

public:
	/*-----[ 启动服务(uOvertime = 0不启动超时线程,uPort == 0时启动链接服务) ]*/ 
	virtual bool		start			(uint16 uPort);
	/*-----[ 关闭服务 ]*/ 
	virtual void		shutdown		();
	/*-----[ 断开 ]*/ 
	virtual void		disconnect		(tcp_context* pContext);
	/*-----[ 断开所有连接 ]*/ 
	virtual void		disconnectAll	();
	/*-----[ 发送数据 ]*/ 
	virtual bool		send			(tcp_context* pContext,const char* pBuffer,int32 nSize);
	/*-----[ 连接 ]*/ 
	virtual tcp_context*connect			(uint16 uPort,const char* pAddress = NULL);
	/*-----[ 连接 ]*/ 
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
	/*----->{ 启动回调 }*/ 
	virtual void		onStart			();
	/*----->{ 关闭回调 }*/ 
	virtual void		onShutdown		();

public:
	/*-----[ 新连接通知 ]*/ 
	virtual void		onConnect		(tcp_context* pContext);
	/*-----[ 断开通知 ]*/ 
	virtual void		onDisconnect	(tcp_context* pContext);
	/*-----[ 超时通知 ]*/ 
	virtual void		onTimeoutRead	(tcp_context* pContext);
	/*----->{ 发送超时 }*/ 
	virtual void		onTimeoutWrite	(tcp_context* pContext);
	/*-----[ 数据包通知 ]*/ 
	virtual void		onPacket		(tcp_context* pContext,char* pBuffer,const uint16& nSize);

protected:
	/*-----[ 处理包 ]*/ 
	virtual bool		onProcessPackage(tcp_context* pContext,CNetBuffer* pBuffer);
	/*-----[ 发送完成回调 ]*/ 
	virtual	void		callback_write	(tcp_context* pContext,int nSize);

protected:
	//--- 获得一个工作线程
	virtual tcp_thread*	getWorkThread	();
	//--- 初始化上下文
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
	//--- 对线程使用次数进行排序
	static int32 _qsort_thread	(const void *p1, const void *p2);

private:
	static THREAD_RETURN	_listener_thread_	(void* pParam);
	static THREAD_RETURN	_working_thread_	(void* pParam);
};
