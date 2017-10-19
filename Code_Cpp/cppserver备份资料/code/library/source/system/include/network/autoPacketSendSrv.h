/*----------------- autoPacketSendSrv.h
*
* Copyright (C): 2015  Mokylin¡¤Mokyqi
* Author       : ÕÔÎÄÔ´
* Version      : V1.0
* Date         : 2015/12/21 16:13:17
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "autoPacketSend.h"
#include "netPacketSrv.h"
#include "stackChecker/stackChecker.h"
/*************************************************************/
class CAutoPacketSendSrv	: public CAutoPacketSend
{
private:
	typedef CBufferStream Parent;

protected:
	CNetPacketSrv*	m_pPacketSrv;

public:
	CAutoPacketSendSrv(CNetPacketSrv* pPacket):CAutoPacketSend(pPacket)
	{
		m_pPacketSrv = pPacket;
	}

public:
	inline void	buildPackets	(_stSvrInfo& stFromSrvInfo,_stSvrInfo& stToSrvInfo,uint8 uMainProtocol,uint8 uProtocol,uint8 uEvent = 0)
	{
		FUN_STACK_CHECK();

		if(!m_pPacketSrv)
			return;
		m_pPacketSrv->buildPacketHeadsSrv(stFromSrvInfo,stToSrvInfo,uMainProtocol,uProtocol,uEvent);
		initBufferStream(m_pPacket->getSpareBuffer(),(uint32)m_pPacket->getSpareSize());
	}
};
