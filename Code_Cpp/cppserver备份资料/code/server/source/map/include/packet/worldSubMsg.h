/*------------- worldSubMsg.h
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈腾
* Version      : V1.01
* Date         : 2016/03/11
*
*/ 
/***************************************************************
* 世界服子协议基类
***************************************************************/
#pragma once
#include "network/netStruct.h"
#include "stream/netStream.h"
#include "worldPacket.h"
/*************************************************************/
class CWorldSubMsg
{
protected:
	typedef bool (CWorldSubMsg::*_HandleSrvSubMsg)	(_stPacketHead_Svr& pHead, CNetStream& clStream);

private:
	CWorldPacket&	m_stWorldPacket;

protected:
	CWorldSubMsg(CWorldPacket& stPacket) : m_stWorldPacket(stPacket) { }
	inline bool		sendMsg(_stSvrInfo& stFromSrvInfo,_stSvrInfo& stToSrvInfo,uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent = 0, bool bCheck = true)
	{
		return m_stWorldPacket.sendMsg(stFromSrvInfo, stToSrvInfo, uMainProtocol, uProtocol, msg, uEvent, bCheck);
	}
	
public:
	virtual bool	onPacketSubMsg	(_stPacketHead_Svr* pHead, CNetStream& clStream) = 0;
};
/*************************************************************/
