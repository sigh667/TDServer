/*------------- clientNetwork.h
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.01
* Date         : 2011/2/24 9:44:31
*
*/ 
/***************************************************************
* 角色服务器外部角色通讯服务
***************************************************************/
#pragma once
#include "network/netService.h"
#include "memoryPool/mapPool.h"

#include "packet/clientPacket.h"
/*************************************************************/
class CClientNetwork	: public CNetworkBase
{
private:
	typedef CMapPool<tcp_context*,CClientPacket>	POOL_PACKET;

private:
	POOL_PACKET	m_poolPack;

public:
	message_queue_get_packet(m_poolPack);

private:
	virtual CMutex_*		connect_lock()						{	return &(m_poolPack.getLock());		}
	virtual CNetPacket*	connect_new	(tcp_context* pContext);
	virtual CNetPacket*	connect_find(tcp_context* pContext)	{	return m_poolPack.find(pContext);	}
	virtual void		connect_del	(tcp_context* pContext)	{	m_poolPack.release(pContext);		}
};
