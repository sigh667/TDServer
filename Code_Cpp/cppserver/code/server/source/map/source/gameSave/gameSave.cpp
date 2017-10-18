/*----------------- gameSave.cpp
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/9/26 10:53:31
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "gameSave/gameSave.h"
#include "log/log.h"
//------------------------------ 
#include "packet/worldPacket.h"
#include "packet/clientPacket.h"
#include "gameLogic/sceneObject/scenePlayer.h"
#include "main/gameServer.h"
#include "gameLogin/gamePlayerMgr.h"
/*************************************************************/
CGameSave* g_pGameSave = NULL;
//-------------------------------------------------------------
//------------------------------ 
CGameSave::CGameSave()
{
	initSave();
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameSave::initSave	()
{
	for (uint8 i = 0;i < _emPlayerType_ARRAYSIZE;++i)
		m_mapPlayerSave[i].clear();
}

//-------------------------------------------------------------
//------------------------------ 
bool	CGameSave::isEmpty			()
{
	for (uint8 i = 0;i < _emPlayerType_ARRAYSIZE;++i)
	{
		if(!m_mapPlayerSave[i].empty())
			return false;
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameSave::playerSave			(uint16 uLogicWorldID,uint32 uPlayerID,uint8 uType,uint32 uSaveOrder,const void* pData,uint32 uLen,bool bNow)
{
	if(!uPlayerID || uType >= _emPlayerType_ARRAYSIZE)
		return false;

	MAP_SAVE_DATA&mapData = m_mapPlayerSave[uType];
	_stSaveData*pSaveData = mapData.allocate(uPlayerID);
	if(!pSaveData)
		return false;
	pSaveData->uType		= uType;
	pSaveData->uObjectID	= uPlayerID;
	pSaveData->uSaveOrder	= uSaveOrder;
	pSaveData->uLogicWorldID= uLogicWorldID;

	if (pData && uLen)
		pSaveData->writeData(pData,uLen);

	uint64 uTime = (uint64)getTime();
	if ( bNow )
	{
		save(*pSaveData,uTime);
	}
	else
	{
		pSaveData->uLastTime	= uTime + 3;
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------ 
void	CGameSave::onPlayerSaveDone		(uint32 uPlayerID,uint8 uType,uint32 uSaveOrder)
{
	if(uType < _emPlayerType_ARRAYSIZE)
	{
		MAP_SAVE_DATA&mapData = m_mapPlayerSave[uType];

		_stSaveData*pSaveData = mapData.find(uPlayerID);
		if(!pSaveData || pSaveData->uSaveOrder != uSaveOrder)
			return;

		mapData.release(uPlayerID);
	}

	/*全部保存完成*/
	for (uint8 i = 0;i < _emPlayerType_ARRAYSIZE;++i)
	{
		if(m_mapPlayerSave[i].exist(uPlayerID))
			return;
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameSave::timerProcess()
{
	uint64	uTime = (uint64)getTime();
	for (uint8 i = 0;i < _emPlayerType_ARRAYSIZE;++i)
	{
		MAP_SAVE_DATA&mapData = m_mapPlayerSave[i];

		MAP_SAVE_DATA::iterator _pos = mapData.begin();
		while (!mapData.isTail(_pos))
		{
			_stSaveData*pSaveData = mapData.getNext(_pos);
			if(!pSaveData)
				continue;

			if(pSaveData->uLastTime && pSaveData->uLastTime + 20 > uTime)
				continue;

			save(*pSaveData,uTime);
		}
 	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameSave::save	(_stSaveData&stData,uint64 uTime)
{
	if ( !g_pServer->isWorldConneted(stData.uLogicWorldID) )
		return ;

	static char szStream[_IO_BUFFER_];
	CNetStream clBuffer(szStream,_IO_BUFFER_);

	clBuffer.write(uint32(stData.uObjectID));
	clBuffer.write(uint8(stData.uType));
	clBuffer.write(stData.uSaveOrder);
	clBuffer.write(stData.uDataSize,stData.szData);
	g_pServer->sendBufferToWorld(stData.uLogicWorldID,P_G2W_Protocol_Player,G2W_Player_Save,clBuffer.getBuffer(),clBuffer.getPosition(),stData.uLastTime?0:1);

	stData.uLastTime = uTime;
}