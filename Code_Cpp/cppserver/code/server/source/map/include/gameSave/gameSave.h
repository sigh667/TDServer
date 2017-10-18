/*----------------- gameSave.h
*
* Copyright (C): 2011  Mokylin¡¤Mokyqi
* Author       : ÕÔÎÄÔ´
* Version      : V1.0
* Date         : 2011/9/26 10:45:13
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "timer/timers.h"
#include "memoryPool/mapPool.h"
#include "singleton/singleton.h"
#include "protocol/PlayerStruct.pb.h"
#include "protocol/GameDefine.pb.h"
#include "basic/memoryFunctions.h"
#include "network/netStruct.h"
/*************************************************************/
class CScenePlayer;
class CGameSave	: public CTimerObject
				, _single_public(CGameSave)
{
private:
	_single_friend(CGameSave);

private:
	//´æ´¢Ä£°æ
	struct _stSaveData
	{
		uint8		uType;
		uint32		uObjectID;
		uint16		uLogicWorldID;
		uint32		uSaveOrder;
		uint64		uLastTime;
		uint32		uDataSize;
		char		szData[_IO_BUFFER_];

		inline void	initialize	()	{	memset(this,0,sizeof(*this));	}
		inline bool	writeData	(const void* pData,uint32 uLen)
		{
			if (!pData || uLen > sizeof(szData))
				return false;

			uDataSize	= uLen;
			dMemcpy(&szData,sizeof(szData),pData,uLen);
			return true;
		}
	};
protected:
	typedef	CMapPool<uint32,_stSaveData>	MAP_SAVE_DATA;

protected:
	MAP_SAVE_DATA	m_mapPlayerSave[_emPlayerType_ARRAYSIZE];
private:
	CGameSave();

public:
	void	initSave			();
	bool	isEmpty				();

public:
	bool	playerSave			(uint16 uLogicWorldID,uint32 uPlayerID,uint8 uType,uint32 uSaveOrder,const void* pData,uint32 uLen,bool bNow);
	void	onPlayerSaveDone	(uint32 uPlayerID,uint8 uType,uint32 uSaveOrder);
private:
	void	save				(_stSaveData&stData,uint64 uTime);
public:
	virtual void	timerProcess();
};
/*************************************************************/
extern CGameSave* g_pGameSave;
