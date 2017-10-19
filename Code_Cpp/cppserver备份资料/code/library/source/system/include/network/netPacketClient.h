/*----------------- netPacketClient.h
*
* Copyright (C): 2015  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2015/12/21 11:51:24
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "network/netPacket.h"
/*************************************************************/
class CNetPacketClient	: public CNetPacket
{
protected:
	typedef CNetPacket Parent;

public:
	typedef bool (CNetPacketClient::*_HandleClientMsg)	 (_stPacketHead_Client &pHead, CNetStream &clStream);
	typedef bool (CNetPacketClient::*_HandleClientSubMsg)(_stPacketHead_Client &pHead, CNetStream &clStream);

protected:
	/*--->[ 构建带结构类型包头 ]*/
public:
	CNetPacketClient();
	virtual ~CNetPacketClient();

	bool				sendMsg					(uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent = 0, bool bCheck = true);
	bool				sendBuffer				(uint8 uMainProtocol, uint8 uProtocol, void* pData, size_t uSize, uint8 uEvent = 0, bool bCheck = true);

public:
	/*----->{ 初始化 }*/ 
	virtual void		initialize				();
protected:
	/*----->{ 数据包处理 }*/ 
	virtual bool		onPacket				(_stPacketHead &pHead,CNetStream &clStream);
protected:
	/*----->{ 数据包处理 }*/ 
	virtual bool		onPacketClient			(_stPacketHead_Client &pHead,CNetStream &clStream) = 0 ;
};
