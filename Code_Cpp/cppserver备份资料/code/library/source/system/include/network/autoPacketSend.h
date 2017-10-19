/*----------------- autoPacketSend.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2011/8/21 13:34:47
*--------------------------------------------------------------
*���ݰ��Զ�����
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
	/*--->[ ���ð��¼� ]*/
	inline void	setPacketEvent		(uint8 uEvent)
	{
		FUN_STACK_CHECK();

		if(m_pPacket)
			m_pPacket->setPacketEvent(uEvent);
	}
	
	/*--->[ ������С ]*/
	inline void packetSize ()
	{
		FUN_STACK_CHECK();

		if(m_pPacket)
		{
			m_pPacket->skipPosition(getPosition());
			m_pPacket->packetSize();
		}
	}
	/*--->[ ȡ�����ͼ����ִ��� ]*/
	inline void	cancell()
	{
		FUN_STACK_CHECK();

		if(m_pPacket)
		{
			m_pPacket->initPosition();
			m_pPacket = NULL;
		}
	}
	/*--->[ ������Ϣ ]*/
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
