/*------------- playerRelive.h
 * Copyright (C): 2016  Mokylin·Mokyqi
 * Author       : 方声辉
 * Version      : V1.01
 * Date         : 2017/05/31
 * 
 */
/***************************************************************
* 
***************************************************************/
#pragma once
#include "gameLogic/sceneObject/sceneUnit.h"
#include "gameLogic/gamePlayerFun/gamePlayerFun.h"
#include "protocol/PlayerStruct.pb.h"
#include "basic/memoryFunctions.h"

/*************************************************************/


class CPlayerRelive	: public CGamePlayerFun
{

private:
	uint32  m_FreeCount	;			// 免费复活次数
	uint32  m_CostCount	;			// 消耗道具复次数

public:
	CPlayerRelive(CScenePlayer& clPlayer);
	virtual~CPlayerRelive();

public:
	virtual	void	initialize				()override;
	virtual	bool	loadFromProtobuf		(const PBPlayerData&pbPlayer) override;
	virtual	bool	saveToProtobuf			(PBPlayerData&pbPlayer) override;
	virtual void	newOnline				() override ;
	virtual void	cleanupDaily			() override ;
	
	virtual	_emGameSystemType	getFunType	()const override	{return GameSystemType_Relives ; }
	
private:
	bool	origin_relive					(uint8 &uCode);
	bool	safepoint_relive				(uint8 &uCode);
	uint16	getRelivePoint					();
private:
	bool			handle_Relive(uint8 uEventID,CNetStream& clStream);
	bool			send_Relive_Ack();
public:
	bool			relive(uint32 uType , uint8 & uCode);
};


/*************************************************************/