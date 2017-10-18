/*----------------- gameBroadcast.h
*
* Copyright (C): 2016  Mokylin°§Mokyqi
* Author       : ’‘Œƒ‘¥
* Version      : V1.0
* Date         : 2016/8/25 14:58:31
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "timer/timers.h"
#include "singleton/singleton.h"
#include "google/protobuf/message.h"
#include "stream/netStream.h"
#include "stl/std_set.h"
#include "protocol/Broadcast.pb.h"
//------------------------------ 
/*************************************************************/
class CGameBroadcast : public CTimerObject
					, _single_public(CGameBroadcast)
{
private:
	_single_friend(CGameBroadcast);
public:
	CGameBroadcast();
public:
	void	broadCastMsg			(uint16 uLogicWorldID,_emBroadcast_Channel eChannel,uint64 uID,uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent = 0);
	void	broadCastStream			(uint16 uLogicWorldID,_emBroadcast_Channel eChannel,uint64 uID,uint8 uMainProtocol, uint8 uProtocol, void* pBuffer,uint32 uSize,uint8 uEvent = 0);

public:
	void	broadCastMsg_World		(uint16 uLogicWorldID,uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent = 0);
	void	broadCastStream_World	(uint16 uLogicWorldID,uint8 uMainProtocol, uint8 uProtocol, void* pBuffer,uint32 uSize, uint8 uEvent = 0);

	void	broadCastMsg_Team		(uint16 uLogicWorldID,uint64 uTeamID,uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent = 0);
	void	broadCastStream_Team	(uint16 uLogicWorldID,uint64 uTeamID,uint8 uMainProtocol, uint8 uProtocol, void* pBuffer,uint32 uSize, uint8 uEvent = 0);

	void	broadCastMsg_Faction	(uint16 uLogicWorldID,uint32 uFactionID,uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent = 0);
	void	broadCastStream_Faction	(uint16 uLogicWorldID,uint32 uFactionID,uint8 uMainProtocol, uint8 uProtocol, void* pBuffer,uint32 uSize, uint8 uEvent = 0);

	void	broadCastMsg_Player		(uint16 uLogicWorldID,uint32 uPlayerID,uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent = 0);
	void	broadCastStream_Player	(uint16 uLogicWorldID,uint32 uPlayerID,uint8 uMainProtocol, uint8 uProtocol, void* pBuffer,uint32 uSize, uint8 uEvent = 0);
public:
	void	on_world_broadCast		(uint16 uLogicWorldID,_emBroadcast_Channel eChannel,uint64 uID,uint8 uMainProtocol, uint8 uProtocol, void* pBuffer,uint32 uSize,uint8 uEvent);
};
/*************************************************************/
extern CGameBroadcast* g_pGameBroadcast;
