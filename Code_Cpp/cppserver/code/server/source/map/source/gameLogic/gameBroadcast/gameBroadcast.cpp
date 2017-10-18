/*----------------- gameBroadcast.cpp
*
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ÕÔÎÄÔ´
* Version      : V1.0
* Date         : 2016/8/25 14:58:37
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "gameLogic/gameBroadcast/gameBroadcast.h"
#include "protocol/Broadcast.pb.h"
#include "main/gameServer.h"
#include "gameLogin/gamePlayerMgr.h"
/*************************************************************/
CGameBroadcast* g_pGameBroadcast = NULL;
//-------------------------------------------------------------
//------------------------------ 
CGameBroadcast::CGameBroadcast()
{
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameBroadcast::broadCastMsg			(uint16 uLogicWorldID,_emBroadcast_Channel eChannel,uint64 uID,uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent)
{
	CNetStream clData;
	if ( msg )
		clData.SerializeToStream(*msg);

	broadCastStream(uLogicWorldID,eChannel,uID,uMainProtocol,uProtocol,clData.getBuffer(),clData.getPosition(),uEvent);
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameBroadcast::broadCastStream		(uint16 uLogicWorldID,_emBroadcast_Channel eChannel,uint64 uID,uint8 uMainProtocol, uint8 uProtocol, void* pBuffer,uint32 uSize, uint8 uEvent)
{
	if (!g_pServer->isWorldConneted(uLogicWorldID))
		return ;

	static char szStream[_IO_BUFFER_];
	CNetStream clBuffer(szStream,_IO_BUFFER_);
	clBuffer.write(uID);
	clBuffer.write(uMainProtocol);
	clBuffer.write(uProtocol);
	if (pBuffer&&uSize)
		clBuffer.write(uSize,pBuffer);
	g_pServer->sendBufferToWorld(uLogicWorldID,P_G2W_Protocol_Broadcast,eChannel,clBuffer.getBuffer(),clBuffer.getPosition(),uEvent);
}
//-------------------------------------------------------------
//------------------------------
void	CGameBroadcast::broadCastMsg_World		(uint16 uLogicWorldID,uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent)
{
	broadCastMsg(uLogicWorldID,BroadcastChannel_World,uLogicWorldID,uMainProtocol,uProtocol,msg,uEvent);
}
//-------------------------------------------------------------
//------------------------------
void	CGameBroadcast::broadCastStream_World	(uint16 uLogicWorldID,uint8 uMainProtocol, uint8 uProtocol, void* pBuffer,uint32 uSize, uint8 uEvent)
{
	broadCastStream(uLogicWorldID,BroadcastChannel_World,uLogicWorldID,uMainProtocol,uProtocol,pBuffer,uSize,uEvent);
}
//-------------------------------------------------------------
//------------------------------
void	CGameBroadcast::broadCastMsg_Team		(uint16 uLogicWorldID,uint64 uTeamID,uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent)
{
	broadCastMsg(uLogicWorldID,BroadcastChannel_Team,uTeamID,uMainProtocol,uProtocol,msg,uEvent);
}
//-------------------------------------------------------------
//------------------------------
void	CGameBroadcast::broadCastStream_Team	(uint16 uLogicWorldID,uint64 uTeamID,uint8 uMainProtocol, uint8 uProtocol, void* pBuffer,uint32 uSize, uint8 uEvent)
{
	broadCastStream(uLogicWorldID,BroadcastChannel_Team,uTeamID,uMainProtocol,uProtocol,pBuffer,uSize,uEvent);
}
//-------------------------------------------------------------
//------------------------------
void	CGameBroadcast::broadCastMsg_Faction		(uint16 uLogicWorldID,uint32 uFactionID,uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent)
{
	broadCastMsg(uLogicWorldID,BroadcastChannel_Faction,uFactionID,uMainProtocol,uProtocol,msg,uEvent);
}
//-------------------------------------------------------------
//------------------------------
void	CGameBroadcast::broadCastStream_Faction	(uint16 uLogicWorldID,uint32 uFactionID,uint8 uMainProtocol, uint8 uProtocol, void* pBuffer,uint32 uSize, uint8 uEvent)
{
	broadCastStream(uLogicWorldID,BroadcastChannel_Faction,uFactionID,uMainProtocol,uProtocol,pBuffer,uSize,uEvent);
}
//-------------------------------------------------------------
//------------------------------
void	CGameBroadcast::broadCastMsg_Player		(uint16 uLogicWorldID,uint32 uPlayerID,uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent)
{
	broadCastMsg(uLogicWorldID,BroadcastChannel_Player,uPlayerID,uMainProtocol,uProtocol,msg,uEvent);
}
//-------------------------------------------------------------
//------------------------------
void	CGameBroadcast::broadCastStream_Player	(uint16 uLogicWorldID,uint32 uPlayerID,uint8 uMainProtocol, uint8 uProtocol, void* pBuffer,uint32 uSize, uint8 uEvent)
{
	broadCastStream(uLogicWorldID,BroadcastChannel_Player,uPlayerID,uMainProtocol,uProtocol,pBuffer,uSize,uEvent);
}
//-------------------------------------------------------------
//------------------------------
void	CGameBroadcast::on_world_broadCast		(uint16 uLogicWorldID,_emBroadcast_Channel eChannel,uint64 uID,uint8 uMainProtocol, uint8 uProtocol, void* pBuffer,uint32 uSize,uint8 uEvent)
{
	if ( eChannel == BroadcastChannel_Player )
	{
		CScenePlayer* pPlayer = g_pGamePlayerMgr->find((uint32)uID);
		if (pPlayer)
		{
			pPlayer->sendBuffer(uMainProtocol,uProtocol,pBuffer,uSize,uEvent);
		}
		return ;
	}

	CGamePlayerMgr::iterator _pos = g_pGamePlayerMgr->begin();
	CGamePlayerMgr::iterator _end = g_pGamePlayerMgr->end();
	for ( ; _pos != _end ; ++_pos )
	{
		CScenePlayer* pPlayer = _pos->second;
		if ( !pPlayer || !pPlayer->isClientConneted() )
			continue;

		bool bSend = false;
		switch (eChannel)
		{
		case BroadcastChannel_World:
			{
				bSend = ( pPlayer->getLogicWorldID() == uLogicWorldID );
			}
			break;
		case BroadcastChannel_Team:
			{
				bSend = ( pPlayer->getTeamID() == uID );
			}
			break;
		case BroadcastChannel_Faction:
			{
				bSend = ( pPlayer->getFactionID() == (uint32)uID );
			}
			break;
		default:
			break;
		}

		if ( bSend )
		{
			pPlayer->sendBuffer(uMainProtocol,uProtocol,pBuffer,uSize,uEvent);
		}
	}
}