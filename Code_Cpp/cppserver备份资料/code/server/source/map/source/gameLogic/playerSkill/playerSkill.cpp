/*----------------- gamePlayerSkill.cpp
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/1/20 17:31:59
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "gameLogic/playerSkill/playerSkill.h"
#include "gameLogic/sceneObject/scenePlayer.h"
#include "cfg_data/constantData/constantData.h"
#include "main/gameServer.h"
#include "protocol/CASMsg.pb.h"
#include "protocol/PlayerStruct.pb.h"
#include "protocol/MsgResult.pb.h"
#include "log/log.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
CPlayerSkill::CPlayerSkill(CScenePlayer& clPlayer):CGamePlayerFun(clPlayer)
{
	registerHandler(C2S_Skill_Use,(CGamePlayerFun::_handleSubMsg)&CPlayerSkill::handle_UseSkill_Ask	);
	registerHandler(C2S_Skill_UseCharge,(CGamePlayerFun::_handleSubMsg)&CPlayerSkill::handle_SkillUseCharge_Ask	);
	registerHandler(C2S_Skill_Break,(CGamePlayerFun::_handleSubMsg)&CPlayerSkill::handle_BreakSkill_Ask	);

	registerHandler(C2S_Skill_Add,(CGamePlayerFun::_handleSubMsg)&CPlayerSkill::handle_AddSkill_Ask	);
	registerHandler(C2S_Skill_Upgrade,(CGamePlayerFun::_handleSubMsg)&CPlayerSkill::handle_UpgradeSkill_Ask	);
}

//-------------------------------------------------------------
//------------------------------ 
CPlayerSkill::~CPlayerSkill()
{

}

//-------------------------------------------------------------
//------------------------------ 
void	CPlayerSkill::initialize			()
{
	CGamePlayerFun::initialize();
}
//-------------------------------------------------------------
//------------------------------ 
bool	CPlayerSkill::loadFromProtobuf	(const PBPlayerData&pbPlayer)
{
	return m_clPlayer.getGameSkillMgr().load(pbPlayer.playersystem().skill());
}
//-------------------------------------------------------------
//------------------------------ 
bool	CPlayerSkill::saveToProtobuf		(PBPlayerData&pbPlayer)
{
	return m_clPlayer.getGameSkillMgr().save(pbPlayer.mutable_playersystem()->mutable_skill());
}

//-------------------------------------------------------------
//------------------------------  技能中断 停止技能
bool	CPlayerSkill::handle_BreakSkill_Ask	(uint8 uEventID, CNetStream& clStream)
{
	PBC2GSkillBreak ask;
	if (!clStream.ParseFromStream(ask))
		return false;

	uint8 uCode = R_ResultSkill_Fail;
	if (!m_clPlayer.getGameSkillMgr().addSkillBreakCmd(ask.skillid(),uCode))
	{
		CGameSkillMgr::send_SkillBreak_Ack(ask.skillid(),0,uCode,false,&m_clPlayer);
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------  蓄力技能释放
bool	CPlayerSkill::handle_SkillUseCharge_Ask	(uint8 uEventID, CNetStream& clStream)
{
	PBC2SSkillUseCharge ask;
	if (!clStream.ParseFromStream(ask))
		return false;

	uint8 uCode = R_ResultSkill_Fail;
	if (!m_clPlayer.getGameSkillMgr().addSkillChargeCmd(ask.skillid(),uCode))
	{
		CGameSkillMgr::send_SkillUseCharge_Ack(ask.skillid(),uCode,false,&m_clPlayer);
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------  技能使用
bool	CPlayerSkill::handle_UseSkill_Ask	(uint8 uEventID, CNetStream& clStream)
{
	PBC2GUseSkill ask;
	if (!clStream.ParseFromStream(ask))
		return false;

	uint8 uCode = R_ResultSkill_Fail;
	_stPoint2F stTargetPoint(ask.pbtarget().posx(),ask.pbtarget().posy());
	_stPoint2F stSelfPoint(ask.pbposition().posx(),ask.pbposition().posy());
	
	//清除复活时所加的buff
	m_clPlayer.getGameBuffMgr().delBuff((uint32)g_clConstantData.Relive_Buff_ID, \
		DoingType_Relive);

	if (!m_clPlayer.getGameSkillMgr().addSkillUseCmd(ask.skillid(),ask.direction(),ask.targetid(),stSelfPoint,stTargetPoint,uCode))
	{
		//CGameSkillMgr::send_UseSkill_Ack(ask.skillid(),0,0,ask.targetid(),stTargetPoint,uCode,false,&m_clPlayer);
	}

	return true;
}


//-------------------------------------------------------------
//------------------------------  技能增加
bool	CPlayerSkill::handle_AddSkill_Ask	(uint8 uEventID, CNetStream& clStream)
{
	PBC2GSkillAdd ask;
	if (!clStream.ParseFromStream(ask))
		return false;

	uint8 uCode = R_ResultSkill_Fail;
	if (!m_clPlayer.getGameSkillMgr().addSkillAsk(ask.skillid(),ask.level(),uCode))
	{
		CGameSkillMgr::send_AddSkill_Ack(ask.skillid(),ask.level(),uCode,&m_clPlayer);
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------  技能升级
bool	CPlayerSkill::handle_UpgradeSkill_Ask	(uint8 uEventID, CNetStream& clStream)
{
	PBC2GSkillUpgrade ask;
	if (!clStream.ParseFromStream(ask))
		return false;

	uint8 uCode = R_ResultSkill_Fail;
	if (!m_clPlayer.getGameSkillMgr().upgradeSkillAsk(ask.skillid(),ask.level(),uCode))
	{
		CGameSkillMgr::send_UpgradeSkill_Ack(ask.skillid(),ask.level(),uCode,&m_clPlayer);
	}

	return true;
}
