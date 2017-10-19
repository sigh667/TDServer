/*----------------- playerBuff.h
*
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ³ÂÁÁ
* Version      : V1.0
* Date         : 2016/1/26 15:23:55
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "gameLogic/gamePlayerFun/gamePlayerFun.h"
#include "memoryPool/memoryPool.h"
#include "protocol/CASProtocol.pb.h"
/*************************************************************/
class CScenePlayer;
class CGameBuffMgr;
class CPlayerBuff	: public CGamePlayerFun
{
public:
	CPlayerBuff(CScenePlayer& clPlayer);
	~CPlayerBuff();

public:
	virtual	void	initialize				();
	virtual	bool	loadFromProtobuf		(const PBPlayerData&pbPlayer);
	virtual	bool	saveToProtobuf			(PBPlayerData&pbPlayer);
	virtual	_emGameSystemType	getFunType	()const	{	return GameSystemType_Buff;	}
	virtual	void	onClientGo				();
protected:
	//--- É¾³ýbuff
	bool	handle_DelBuff_Ask		(uint8 uEventID,CNetStream& clStream);

};
