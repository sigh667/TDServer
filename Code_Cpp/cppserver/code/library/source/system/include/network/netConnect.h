/*----------------- netConnect.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/9/6 12:02:34
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "netPacket.h"
#include "network/include/network.h"
#include "msg_queue/message_queue.h"
/*************************************************************/
class CNetConnect : public tcp_connect
{
private:
	typedef tcp_connect	Parent;

protected:
	message_queue*		m_pMessageQueue;//消息队列
	CMutex_*			m_pLogicLock;
	CNetPacket*			m_pNetPacket;

protected:
	message_queue	m_clMessageQueue;//消息队列

public:
	CNetConnect();
	virtual~CNetConnect();

public:
	inline void		bindPacket		(CNetPacket* pPacket)		{	m_pNetPacket = pPacket;				}
	inline void		bindLogicLock	(CMutex_* pLock)			{	m_pLogicLock = pLock;				}
	inline void		bindMsgQueue	(message_queue* _queue)		{	m_pMessageQueue = _queue;			}
	inline void		selfMsgQueue	()							{	m_pMessageQueue = &m_clMessageQueue;}

protected:
	virtual void	log				(char* pString,...)	{}
	virtual void	warn			(char* pString,...)	{}
	virtual void	error			(char* pString,...);
	virtual void	onConnect		(bool bConnect);
	/*-----[ 断开通知 ]*/ 
	virtual void	onDisconnect	();
	/*-----[ 处理包 ]*/ 
	virtual bool	onProcessPackage();
	virtual void	onPacket		(char* pBuffer,const uint16& nSize);
	virtual void	onTimeoutWrite	();
	virtual void	onTimeoutRead	();

public:
	virtual	void	processMsgQueue	();
};
