/*----------------- interactionMgr.h
*
* Copyright (C): 2011  Mokylin°§Mokyqi
* Author       : ’‘Œƒ‘¥
* Version      : V1.0
* Date         : 2012/4/3 15:17:07
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "interactionMgr/interactionStruct.h"
#include "memoryPool/mapPool.h"
#include "timer/timers.h"
#include "singleton/singleton.h"
/*************************************************************/
class CInteractionMgr	: public CTimerObject
						, _single_public(CInteractionMgr)
{
private:
	_single_friend(CInteractionMgr);

public:
	typedef CMapPool<uint32,_stInteraction>	MAP_OBJECT;

public:
	union _unInteractionid
	{
		struct
		{
			uint8	uEventID;
			uint8	uEventSubID;
			uint32	uRecvID;
		}_EVENT_S;
		uint64	uEventID;
	};
	struct _stObjectList
	{
		uint64		uEvent;
		MAP_OBJECT	mapObject;
		inline void initialize()
		{
			uEvent = 0;
			mapObject.clear();
		}
	};
private:
	typedef CMapPool<uint64,_stObjectList>	MAP_INTERACTION;

protected:
	MAP_INTERACTION	m_mapInteraction;

public:
private:
	CInteractionMgr();
	virtual~CInteractionMgr();

public:
	static uint64	interactionID(uint8 uEventID,uint8 uEventSubID,uint32 uRecvID);

public:
	inline	_stInteraction*	find		(uint8 uEventID,uint8 uEventSubID,uint32 uRecvID,uint32 uSendID);
	inline bool				remove		(uint8 uEventID,uint8 uEventSubID,uint32 uRecvID,uint32 uSendID,uint32*pDataID = NULL);

public:
	_stObjectList*	findObject	(uint8 uEventID,uint8 uEventSubID,uint32 uRecvID);
	bool			haveInteract(uint8 uEventID,uint8 uEventSubID,uint32 uSendID);
	_stInteraction*	find		(uint64 uEvent,uint32 uSendID);
	_stInteraction*	build		(uint8 uEventID,uint8 uEventSubID,uint32 uRecvID,uint32 uSendID,uint32 uDataID = 0,uint32 uTimeout = 60);
	bool			remove		(uint64 uEvent,uint32 uSendID,uint32*pDataID = NULL);

public:
	virtual void	timerProcess();
};
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
inline _stInteraction*	CInteractionMgr::find	(uint8 uEventID,uint8 uEventSubID,uint32 uRecvID,uint32 uSendID)
{
	return find(interactionID(uEventID,uEventSubID,uRecvID),uSendID);
}
//-------------------------------------------------------------
//------------------------------ 
inline	bool	CInteractionMgr::remove		(uint8 uEventID,uint8 uEventSubID,uint32 uRecvID,uint32 uSendID,uint32*pDataID)
{
	return remove(interactionID(uEventID,uEventSubID,uRecvID),uSendID,pDataID);
}
/*************************************************************/
extern CInteractionMgr* g_pInteractionMgr;
