/*----------------- netService.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2012/9/13 15:04:59
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "netPacket.h"
#include "network/include/network.h"
#include "msg_queue/message_queue.h"
/*************************************************************/
//#############################################################
//############################## CNetworkBase
//#############################################################
class CNetworkBase	: public tcp_network
{
protected:
	typedef tcp_network	Parent;

protected:
	CMutex_*			m_pMainMutex;	//���߳���
	message_queue*		m_pMessageQueue;//��Ϣ����

protected:
	message_queue		m_clMessageQueue;//��Ϣ����

public:
	inline void		setMainMutex	(CMutex_* _mutex)			{	m_pMainMutex = _mutex;				}
	inline void		bindMsgQueue	(message_queue* _queue)		{	m_pMessageQueue = _queue;			}
	inline void		selfMsgQueue	()							{	m_pMessageQueue = &m_clMessageQueue;}

public:
	CNetworkBase();
	virtual~CNetworkBase(){}

protected:
	virtual CMutex_*	connect_lock()						= 0;
	virtual CNetPacket*	connect_new	(tcp_context* pContext) = 0;
	virtual CNetPacket*	connect_find(tcp_context* pContext) = 0;
	virtual void		connect_del	(tcp_context* pContext) = 0;

public:
	virtual CNetPacket*	getPacket	(tcp_context* _context,uint32 _key);

protected:
	/*-----[ ������֪ͨ ]*/ 
	virtual void	onConnect		(tcp_context* pContext);
	/*-----[ �Ͽ�֪ͨ ]*/ 
	virtual void	onDisconnect	(tcp_context* pContext);
	/*-----[ ��ʱ֪ͨ ]*/ 
	virtual void	onTimeoutWrite	(tcp_context* pContext);
	virtual void	onTimeoutRead	(tcp_context* pContext);
	/*-----[ ����� ]*/ 
	virtual bool	onProcessPackage(tcp_context* pContext,CNetBuffer* pBuffer);

public:
	virtual	void	processMsgQueue	();
};

//#############################################################
//############################## CHttpNetworkBase
//#############################################################
class CHttpNetworkBase	: public CNetworkBase
{
protected:
	typedef CNetworkBase	Parent;

public:
	CHttpNetworkBase(){}
	virtual~CHttpNetworkBase(){}

protected:
	/*-----[ ����� ]*/ 
	virtual bool	onProcessPackage(tcp_context* pContext,CNetBuffer* pBuffer);
};
