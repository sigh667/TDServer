/*----------------- autoPacketSendClient.h
*
* Copyright (C): 2015  Mokylin¡¤Mokyqi
* Author       : ÕÔÎÄÔ´
* Version      : V1.0
* Date         : 2015/12/21 16:03:45
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "autoPacketSend.h"
#include "netPacketClient.h"
#include "stackChecker/stackChecker.h"
#include "protocol/MsgResult.pb.h"
#include "protocol/CAGProtocol.pb.h"
#include "protocol/CAGMsg.pb.h"
/*************************************************************/
class CAutoPacketSendClient	: public CAutoPacketSend
{
private:
	typedef CBufferStream Parent;

protected:
	CNetPacketClient*	m_pPacketClient;

public:
	CAutoPacketSendClient(CNetPacketClient* pPacket):CAutoPacketSend(pPacket)
	{
		m_pPacketClient = pPacket;
	}

public:
	inline void	buildPackets	(uint8 uMainProtocol,uint8 uProtocol,uint8 uEvent = 0)
	{
		FUN_STACK_CHECK();

		if(!m_pPacketClient)
			return;
		m_pPacketClient->buildPacketHeads(uMainProtocol,uProtocol,uEvent);
		initBufferStream(m_pPacket->getSpareBuffer(),(uint32)m_pPacket->getSpareSize());
	}
};
