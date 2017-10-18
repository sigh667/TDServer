/*----------------- playerSkill.h
*
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ³ÂÁÁ
* Version      : V1.0
* Date         : 2016/1/20 16:43:51
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "gameLogic/gamePlayerFun/gamePlayerFun.h"
#include "memoryPool/memoryPool.h"
/*************************************************************/
class CScenePlayer;
class CGameSkillMgr;
class CPlayerSkill	: public CGamePlayerFun
{
public:
	CPlayerSkill(CScenePlayer& clPlayer);
	~CPlayerSkill();

public:
	virtual	void	initialize				();
	virtual	bool	loadFromProtobuf		(const PBPlayerData&pbPlayer);
	virtual	bool	saveToProtobuf			(PBPlayerData&pbPlayer);
	virtual	_emGameSystemType	getFunType	()const	{	return GameSystemType_Skill;	}
protected:
	bool	handle_UseSkill_Ask			(uint8 uEventID,CNetStream& clStream);
	bool	handle_BreakSkill_Ask		(uint8 uEventID,CNetStream& clStream);
	bool	handle_SkillUseCharge_Ask	(uint8 uEventID,CNetStream& clStream);

	bool	handle_AddSkill_Ask			(uint8 uEventID,CNetStream& clStream);
	bool	handle_UpgradeSkill_Ask		(uint8 uEventID,CNetStream& clStream);
};
