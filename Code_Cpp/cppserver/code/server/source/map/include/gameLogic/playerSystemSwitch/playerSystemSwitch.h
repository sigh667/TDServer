/*------------- plyaerSystemSwitch.h
 * Copyright (C): 2016  Mokylin¡¤Mokyqi
 * Author       : ·½Éù»Ô
 * Version      : V1.01
 * Date         : 2016/09/30
 * 
 */
/***************************************************************
* 
***************************************************************/
#pragma once
#include "gameLogic/gamePlayerFun/gamePlayerFun.h"
#include "protocol/PlayerStruct.pb.h"
/*************************************************************/
class CSceneUnit;
class CPlayerSystemSwitch	: public CGamePlayerFun
{
private:
	SET_U32					m_setSystemSwith;

public:
	CPlayerSystemSwitch(CScenePlayer& clPlayer);
	virtual~CPlayerSystemSwitch();
	
public:
	inline	bool checkSwitchStateOpen		(uint32 uID)	{		return m_setSystemSwith.is_exist(uID);			}

public:
	virtual	void	initialize				();
	virtual	bool	loadFromProtobuf		(const PBPlayerData&pbPlayer);
	virtual	bool	saveToProtobuf			(PBPlayerData&pbPlayer);
	virtual	_emGameSystemType	getFunType	()const					{	return GameSystemType_Switch ; }
	virtual	void	newOnline				();
public:
	bool isExit						(uint32 ID);
	bool callback_completeQuest		(uint32 uQuestID);
	bool callback_upLevel			(uint16 uLevel);
	bool callback_upVip				(uint16 uVipLevel);
	bool openSystemSwitch			(uint32 uID);
		
public:
	bool	send_SystemSwitchOpen_Ack(uint32 uID);

};
/*************************************************************/
