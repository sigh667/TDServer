/*----------------- autoPacketSend.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/8/21 13:34:47
*--------------------------------------------------------------
*数据包自动发送
*------------------------------------------------------------*/
#pragma once
#include "netPacket.h"
#include "stackChecker/stackChecker.h"
#include "google/protobuf/message.h"
/*************************************************************/
class CAutoPacketSend	: public CNetStream
{
private:
	typedef CNetStream Parent;

protected:
	CNetPacket*	m_pPacket;

protected:
	CAutoPacketSend(CNetPacket* pPacket){	m_pPacket = pPacket;	}
	virtual~CAutoPacketSend()			{	send();					}

public:
	/*--->[ 设置包事件 ]*/
	inline void	setPacketEvent		(uint8 uEvent)
	{
		FUN_STACK_CHECK();

		if(m_pPacket)
			m_pPacket->setPacketEvent(uEvent);
	}
	
	/*--->[ 检测包大小 ]*/
	inline void packetSize ()
	{
		FUN_STACK_CHECK();

		if(m_pPacket)
		{
			m_pPacket->skipPosition(getPosition());
			m_pPacket->packetSize();
		}
	}
	/*--->[ 取消载送及各种处理 ]*/
	inline void	cancell()
	{
		FUN_STACK_CHECK();

		if(m_pPacket)
		{
			m_pPacket->initPosition();
			m_pPacket = NULL;
		}
	}
	/*--->[ 发送消息 ]*/
	inline bool	send()
	{
		FUN_STACK_CHECK();

		if(m_pPacket)
		{
			m_pPacket->skipPosition(getPosition());
			return m_pPacket->send();
		}
		return false;
	}
};
