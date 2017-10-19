/*----------------- gameSkillMgr.cpp
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/1/20 17:31:35
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "gameLogic/gameSkill/gameSkillMgr.h"
#include "gameLogic/sceneObject/sceneUnit.h"
#include "main/gameServer.h"
#include "protocol/MsgResult.pb.h"
#include "cfg_data/skillData/skillData.h"
#include "protocol/CASMsg.pb.h"
#include "protocol/CASProtocol.pb.h"
#include "gameLogic/sceneObject/scenePlayer.h"
#include "gameLogic/sceneObject/sceneMonster.h"
#include "log/log.h"
//#include "gameLogic/playerWuxue/playerWuxue.h"
#include "gameLogic/playerCommon/playerCommon.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
CGameSkillMgr::CGameSkillMgr()
{
	initialize(NULL);
}

//-------------------------------------------------------------
//------------------------------ 
CGameSkillMgr::~CGameSkillMgr()
{
}

//-------------------------------------------------------------
//------------------------------ 
void CGameSkillMgr::initialize(CSceneUnit* pOwner)
{
	m_uContinueNotHit	= 0;
	m_pOwner = pOwner;
	m_uProcessTick	= 0;
	m_queueCmd.clear();
	clear();
}

//-------------------------------------------------------------
//------------------------------ 
void	CGameSkillMgr::clear()
{
	m_mapSkill.clear();
	for ( uint32 i = 0 ; i < _emSkillUseType_MAX ; ++i )
	{
		m_vecSkill[i].clear();
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameSkillMgr::updateTempSkill	()
{// 重新收集新的临时技能
	if ( !m_pOwner )
		return ;

	{// 删除原先的 临时技能
		POOL_SKILL::iterator _pos;
		m_mapSkill.getHead(_pos);
		while (!m_mapSkill.isTail(_pos))
		{
			CGameSkill* pGameSkill = m_mapSkill.getNext(_pos);
			if (!pGameSkill || !pGameSkill->isTempSkill())
				continue;

			m_mapSkill.release(pGameSkill->getSkillID());
		}
	}

	{// 增加新的 临时技能
		MAP_U16_U8 mapSkill;
		m_pOwner->collectTempSkill(mapSkill);
		MAP_U16_U8::iterator _pos = mapSkill.begin();
		MAP_U16_U8::iterator _end = mapSkill.end();
		for ( ; _pos != _end ; ++_pos )
		{
			addSkill(_pos->first,_pos->second,true,false, DoingType_Others);
		}
	}

	m_pOwner->setUpdateValue(_UV_PassiveSkillBuff);
	sortSkill();
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameSkillMgr::load(const PBSystemSkill& pbSkill)
{
	this->clear();
	for (uint16 i = 0; i < pbSkill.pbskill_size(); ++i)
	{
		this->addSkill(pbSkill.pbskill(i).skillid(), pbSkill.pbskill(i).level(),false,false, DoingType_Others);
	}
	
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameSkillMgr::save(PBSystemSkill* pbSkill)
{
	pbSkill->Clear();
	POOL_SKILL::iterator _pos = m_mapSkill.begin();
	while (!m_mapSkill.isTail(_pos))
	{
		CGameSkill* pGameSkill = m_mapSkill.getNext(_pos);
		if (!pGameSkill || pGameSkill->isTempSkill() )
			continue;
	
		PBSkill* _pbSkill = pbSkill->add_pbskill();
		if (!_pbSkill)
			return false;

		_pbSkill->set_skillid(pGameSkill->getSkillID());
		_pbSkill->set_level(pGameSkill->getLevel());
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
uint8	CGameSkillMgr::getSkillLevel(uint32 uSkillID)
{
	CGameSkill* pGameSkill = findSkill(uSkillID);
	if (pGameSkill)
		return pGameSkill->getLevel();

	return 0;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CGameSkillMgr::canUse	(uint32 uSkillID,uint8& uCode)
{
	if (!canOperate())
		return false;

	CSceneUnit* pOwner = m_pOwner->getOwner();
	if ( pOwner )
	{
		CScenePlayer* pPlayer = pOwner->asPlayer();
		if (pPlayer&&pPlayer->testRegion(RegionMark_PlayerCntUseSkill))
		{
			uCode = R_ResultSkill_Region;
			//return false;
		}
		//检查行为状态
		if (!pOwner->canUseSkill(NULL))
		{
			uCode = R_ResultSkill_ActForbid;
			return false;
		}
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CGameSkillMgr::canAdd	(uint32 uSkillID,uint8& uCode)
{
	if (!canOperate())
		return false;

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CGameSkillMgr::canUpgrade	(uint32 uSkillID,uint8 uLevel,uint8& uCode)
{
	if (!canOperate())
		return false;

	return true;
}
//-------------------------------------------------------------
//------------------------------ 使用技能请求
bool	CGameSkillMgr::useSkillAsk(uint32 uSkillID,float32 fDirection,uint32 uTarUnitID,_stPoint2F&stTargetPoint,uint8& uCode)
{
	//取消打坐
	if (m_pOwner->asPlayer()
		&&m_pOwner->asPlayer()->getPlayerCommon())
	{
		m_pOwner->asPlayer()->getPlayerCommon()->cancelTakeASeat();
	}

	uCode = R_ResultSkill_Fail;
	CGameSkill* pGameSkill = findSkill(uSkillID);
	if (!pGameSkill)
	{
		uCode = R_ResultSkill_NoSkill;
		return false;
	}
	
	const _stSkillModel* pSkillModel = pGameSkill->getSkillModel();
	if (!pSkillModel ||(pSkillModel->uUseType != SkillUseType_Proactive))
		return false;

	CScenePlayer* pScenePlayer = m_pOwner->asPlayer();
	//if (pScenePlayer && pScenePlayer->getPlayerWuxue() && pSkillModel->uType != SkillType_RollSkill)
	//{
	//	if (!pScenePlayer->getPlayerWuxue()->canUseSkill(uSkillID))
	//	{
	//		uCode = R_ResultSkill_NoSkill;
	//		//return false;
	//	}
	//}
// 
// 	//重新计算目标点
// 	if (pSkillModel->findParam(SkillSpareParamType_DashSpeed))
// 	{
// 		_stPoint2F stTarget = stTargetPoint;
// 		m_pOwner->getUnitTargetPoint(stTargetPoint,stTarget);
// 		stTargetPoint = stTarget;
// 	}

	m_pOwner->setDirection(fDirection);
	return m_pOwner->getSkilling().useSkillAsk(m_pOwner,pSkillModel,uTarUnitID,stTargetPoint,uCode);
}

//-------------------------------------------------------------
//------------------------------ 请求添加技能
bool	CGameSkillMgr::addSkillAsk	(uint32 uSkillID,uint8 uLevel,uint8& uCode)
{
	uCode = R_ResultSkill_Fail;
	if (!canAdd(uSkillID,uCode))
		return false;

	return this->addSkill(uSkillID,uLevel,false,true, DoingType_Others);
}

//-------------------------------------------------------------
//------------------------------ 请求升级技能
bool	CGameSkillMgr::upgradeSkillAsk	(uint32 uSkillID,uint8 uLevel,uint8& uCode)
{
	uCode = R_ResultSkill_Fail;
	if (!canUpgrade(uSkillID,uLevel,uCode))
		return false;

	//消耗
	return this->upgradeSkill(uSkillID,uLevel,true, DoingType_Others);
}

//-------------------------------------------------------------
//------------------------------ 添加技能
bool	CGameSkillMgr::addSkill(uint32 uSkillID,uint8 uLevel,bool bTmp,bool bSend,_emDoingType emDoing)
{
	if ( !m_pOwner )
		return false;

	const _stSkillModel* pModel = g_clSkillData.findSkill(uSkillID,uLevel);
	if (!pModel)
		return false;

	//检查身份
	if (!pModel->setNeedJob.empty()&&!pModel->setNeedJob.is_exist(m_pOwner->getJob()))
		return false;

	//是否已有技能
	CGameSkill* pGameSkill = m_mapSkill.find(uSkillID);
	if (!pGameSkill)
	{
		pGameSkill = m_mapSkill.allocate(uSkillID);
		if (!pGameSkill)
			return false;
		pGameSkill->initialize(pModel, bTmp);
	}
	else
	{// 留等级最高的
		if (pGameSkill->getLevel() >= uLevel)
			return true;

		pGameSkill->upgrade(pModel);
	}

	//通知客户端增加技能
	if( bSend )
	{
		CGameSkillMgr::send_AddSkill_Ack(uSkillID,uLevel,R_ResultSkill_Succeed,m_pOwner);
	}

	sortSkill();

	// 被动技能
	if ( pModel->uUseType ==  SkillUseType_Passive )
		m_pOwner->setUpdateValue(_UV_PassiveSkillBuff);

	return true;
}
//-------------------------------------------------------------
//------------------------------删除技能
bool	CGameSkillMgr::delSkill	(uint32 uSkillID,bool bSend,_emDoingType emDoing)
{
	CGameSkill* pGameSkill = m_mapSkill.find(uSkillID);
	if (!pGameSkill || !pGameSkill->isTempSkill())
		return false;
	const _stSkillModel* pModel = pGameSkill->getSkillModel();
	if (!pModel)
		return false;

	m_mapSkill.release(pGameSkill->getSkillID());

	sortSkill();

	if (bSend)
		CGameSkillMgr::send_DelSkill_Ack(uSkillID,R_ResultSkill_Succeed,m_pOwner);

	// 被动技能
	if ( pModel->uUseType ==  SkillUseType_Passive )
		m_pOwner->setUpdateValue(_UV_PassiveSkillBuff);

	return true;
}
//-------------------------------------------------------------
//------------------------------ 添加技能
bool	CGameSkillMgr::upgradeSkill(uint32 uSkillID,uint8 uLevel,bool bSend/* = false*/,_emDoingType emDoing)
{
	CGameSkill* pGameSkill = findSkill(uSkillID);
	if (!pGameSkill || pGameSkill->isTempSkill() )
		return false;

	_stSkillModel* pModel = g_clSkillData.findSkill(uSkillID,uLevel);
	if (!pModel)
		return false;

	pGameSkill->upgrade(pModel);
	//通知客户端增加技能
	if(bSend)
	{
		CGameSkillMgr::send_UpgradeSkill_Ack(uSkillID,uLevel,R_ResultSkill_Succeed,m_pOwner);
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 使用技能
bool	CGameSkillMgr::useSkill(uint32 uSkillID,uint8 uLevel,float32 fDirection,uint32 uTarUnitID,_stPoint2F&stTargetPoint)
{
	const _stSkillModel* pModel = g_clSkillData.findSkill(uSkillID,uLevel);
	if (!pModel)
		return false;

	m_pOwner->setDirection(fDirection);
	uint8 uCode = 0;
	return m_pOwner->getSkilling().useSkillAsk(m_pOwner,pModel,uTarUnitID,stTargetPoint,uCode);
}

//-------------------------------------------------------------
//------------------------------ 清除所有的技能CD
void	CGameSkillMgr::clearSkillCD(uint32 uSkillID)
{
	POOL_SKILL::iterator _pos = m_mapSkill.begin();
	while (!m_mapSkill.isTail(_pos))
	{
		CGameSkill* pGameSkill = m_mapSkill.getNext(_pos);
		if (!pGameSkill || pGameSkill->getSkillID() == uSkillID)
			continue;

		m_pOwner->getCooling().clearCooling(CoolingType_Skill,pGameSkill->getSkillID());
	}
}

//-------------------------------------------------------------
//------------------------------
bool	CGameSkillMgr::process	(uint64 uTick)
{
	processNextCmd();
	return true;
}

//-------------------------------------------------------------
//------------------------------ 使用技能
bool	CGameSkillMgr::send_UseSkill_Ack(uint32 uSkillID,uint8 uLevel,uint8 uStage,uint32 uTarUnitID,_stPoint2F&stTargetPoint,uint8 uCode,bool bBroad,CSceneUnit* pOwner,_stPoint2F* pStartPoint/* = NULL*/)
{
	if (!pOwner)
		return false;
	if (uLevel < 1)
		uLevel = 1;
	
	PBG2CUseSkill ack;
	ack.mutable_skill()->set_skillid(uSkillID);
	ack.mutable_skill()->set_level(uLevel);
	ack.set_direction(pOwner->getDirection());
	if (pStartPoint)
	{
		ack.mutable_pbposition()->set_posx(pStartPoint->x);
		ack.mutable_pbposition()->set_posy(pStartPoint->y);
	}
	else
	{
		ack.mutable_pbposition()->set_posx(pOwner->getPosition().x);
		ack.mutable_pbposition()->set_posy(pOwner->getPosition().y);
	}
	
	ack.set_stage(uStage);
	ack.set_targetid(uTarUnitID);
	ack.set_unitid(pOwner->getUnitID());
	ack.mutable_pbtarget()->set_posx(stTargetPoint.x);
	ack.mutable_pbtarget()->set_posy(stTargetPoint.y);
	CSceneUnit* pUnit = pOwner->getOwner();
	if (!pUnit)
		return true;

	if (bBroad)
	{
		pUnit->sendBroadcast(P_S2C_Protocol_Skill,S2C_Skill_Use,&ack,uCode);
	}
	else
	{
		CScenePlayer* pPlayer = pUnit->asPlayer();
		if (pPlayer)
			pPlayer->sendMsg(P_S2C_Protocol_Skill,S2C_Skill_Use,&ack,uCode);
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------ 中断技能广播
bool	CGameSkillMgr::send_SkillBreak_Ack(uint32 uSkillID,uint8 uStage,uint8 uCode,bool bBroad,CSceneUnit* pOwner)
{
	if(!pOwner)
		return false;

	PBG2CSkillBreak ack;
	ack.set_skillid(uSkillID);
	ack.set_stage(uStage);
	ack.set_unitid(pOwner->getUnitID());

	if (bBroad)
	{
		pOwner->sendBroadcast(P_S2C_Protocol_Skill,S2C_Skill_SkillBreak,&ack,R_ResultSkill_Succeed);
	}
	else
	{
		CScenePlayer* pPlayer = pOwner->asPlayer();
		if (pPlayer)
			pPlayer->sendMsg(P_S2C_Protocol_Skill,S2C_Skill_SkillBreak,&ack,R_ResultSkill_Succeed);
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------ 蓄力技能释放
bool	CGameSkillMgr::send_SkillUseCharge_Ack(uint32 uSkillID,uint8 uCode,bool bBroad,CSceneUnit* pOwner)
{
	if(!pOwner)
		return false;

	PBG2CSkillUseCharge ack;
	ack.set_skillid(uSkillID);
	ack.set_unitid(pOwner->getUnitID());

	if (bBroad)
	{
		pOwner->sendBroadcast(P_S2C_Protocol_Skill,S2C_Skill_UseCharge,&ack,R_ResultSkill_Succeed);
	}
	else
	{
		CScenePlayer* pPlayer = pOwner->asPlayer();
		if (pPlayer)
			pPlayer->sendMsg(P_S2C_Protocol_Skill,S2C_Skill_UseCharge,&ack,R_ResultSkill_Succeed);
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------  增加技能返回
bool	CGameSkillMgr::send_AddSkill_Ack	(uint32 uSkillID,uint8 uLevel,uint8 uCode,CSceneUnit* pOwner)
{
	if (!pOwner)
		return false;

	PBG2CAddSkill ack;
	ack.set_skillid(uSkillID);
	ack.set_level(uLevel);
	CScenePlayer* pPlayer = pOwner->asPlayer();
	if (pPlayer)
		pPlayer->sendMsg(P_S2C_Protocol_Skill,S2C_Skill_Add,&ack,uCode);
	return true;
}
//-------------------------------------------------------------
//------------------------------  删除技能返回
bool	CGameSkillMgr::send_DelSkill_Ack	(uint32 uSkillID,uint8 uCode,CSceneUnit* pOwner)
{
	if (!pOwner)
		return false;

	PBG2CDelSkill ack;
	ack.set_skillid(uSkillID);
	CScenePlayer* pPlayer = pOwner->asPlayer();
	if (pPlayer)
		pPlayer->sendMsg(P_S2C_Protocol_Skill,S2C_Skill_Del,&ack,uCode);
	return true;
}
//-------------------------------------------------------------
//------------------------------  升级技能返回
bool	CGameSkillMgr::send_UpgradeSkill_Ack	(uint32 uSkillID,uint8 uLevel,uint8 uCode,CSceneUnit* pOwner)
{
	if (!pOwner)
		return false;

	PBG2CUpgradeSkill ack;
	ack.set_skillid(uSkillID);
	ack.set_level(uLevel);
	CScenePlayer* pPlayer = pOwner->asPlayer();
	if (pPlayer)
		pPlayer->sendMsg(P_S2C_Protocol_Skill,S2C_Skill_Upgrade,&ack,uCode);
	return true;
}

//-------------------------------------------------------------
//------------------------------  攻击闪避返回 通知施法者和目标
bool	CGameSkillMgr::send_AttackNotice_Ack		(CSceneUnit* pSource,CSceneUnit* pTarget,uint8 uNoticeType,bool bBroad/* = false*/)
{
	if (!pTarget)
		return false;

	PBG2CSkillNotice ack;
	ack.set_targetid(pTarget->getUnitID());
	ack.set_type(uNoticeType);
	CScenePlayer* pTarPlayer = pTarget->asPlayer();
	if (pTarPlayer)
		pTarPlayer->sendMsg(P_S2C_Protocol_Skill,S2C_Skill_Notice,&ack,R_ResultSkill_Succeed);

	if (pSource)
	{
		CScenePlayer* pSorPlayer = pSource->asPlayer();
		if (pSorPlayer)
			pSorPlayer->sendMsg(P_S2C_Protocol_Skill,S2C_Skill_Notice,&ack,R_ResultSkill_Succeed);
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------  攻击伤害返回
bool	CGameSkillMgr::send_AttackDamage_Ack		(CSceneUnit* pSource,uint32 uSkillID,uint8 uCritical,int32 iDamage,bool bBroad,CSceneUnit* pTarget)
{
	if (!pTarget || !pSource)
		return false;

	//CLog::error("[%s]受到[%d]的伤害值[%d],暴击倍数:%d",pOwner->getName(),uAttackID,iDamage,uCritical/10);
	PBG2CSkillDamage ack;
	ack.set_sourceid(pSource->getUnitID());
	ack.set_targetid(pTarget->getUnitID());
	ack.set_critical(uCritical);
	ack.set_value(iDamage);
	ack.set_skillid(uSkillID);
	PBPoint* pbPos = ack.mutable_newposion();
	if ( pbPos )
	{
		pbPos->set_posx(pTarget->getPosition().x);
		pbPos->set_posy(pTarget->getPosition().y);
	}
	if (bBroad)
	{
		pTarget->sendBroadcast(P_S2C_Protocol_Skill,S2C_Skill_Damage,&ack,R_ResultSkill_Succeed);
	}
	else
	{
		CSceneUnit* pTargetOwner = pTarget->getOwner();
		if (pTargetOwner)
		{
			CScenePlayer* pPlayer = pTarget->asPlayer();
			if (pPlayer)
				pPlayer->sendMsg(P_S2C_Protocol_Skill,S2C_Skill_Damage,&ack,R_ResultSkill_Succeed);
		}
		
		CSceneUnit* pSourceOwner = pSource->getOwner();
		if (pSourceOwner)
		{
			CScenePlayer* pPlayer = pSourceOwner->asPlayer();
			if (pPlayer)
				pPlayer->sendMsg(P_S2C_Protocol_Skill,S2C_Skill_Damage,&ack,R_ResultSkill_Succeed);
		}
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------  
void CGameSkillMgr::gatherHaloVec(VEC_U16& haloVec)
{
	if (!m_pOwner)
		return;

	//普通技能
	/*{
		POOL_SKILL::iterator pos = m_mapSkill.begin();
		POOL_SKILL::iterator end = m_mapSkill.end();
		for (; pos != end; ++pos)
		{
			if (!pos->second)
				continue;

			_stSkillModel*pModel = pos->second->getSkillModel();
			if (pModel && pModel->uHaloID)
				haloVec.push_back(pModel->uHaloID);
		}
	}*/
}
//-------------------------------------------------------------
//------------------------------  指令预存
bool	CGameSkillMgr::addSkillUseCmd	(uint32 uSkillID,float32 fDirection,uint32 uTarUnitID,_stPoint2F&stSelfPoint,_stPoint2F&stTargetPoint,uint8& uCode)
{
	uCode = R_ResultSkill_Fail;
	if (m_pOwner->isDead())
		return false;

	CGameSkill* pSkill = m_mapSkill.find(uSkillID);
	if (!pSkill)
		return false;

	if ( !m_queueCmd.empty() )
	{
		_stSkillCmd* pPreCmd = m_queueCmd.back();
		if (pPreCmd&&pPreCmd->eCmdType==SkillCmdType_Charge)
		{
			return false;
		}
		else
		{
			// 使用技能命令会把其他命令都互斥掉
			m_queueCmd.clear();
		}
	}

	_stSkillCmd* pNewCmd = m_queueCmd.push();
	if (!pNewCmd)
		return false;

	pNewCmd->eCmdType			= SkillCmdType_Use;
	pNewCmd->pSkillModel		= pSkill->getSkillModel();
	pNewCmd->fDirection			= fDirection;
	pNewCmd->uTarUnitID			= uTarUnitID;
	pNewCmd->stSelfPoint		= stSelfPoint;
	pNewCmd->stTargetPoint		= stTargetPoint;

	uCode = R_ResultSkill_Succeed;
	return true;
}
//-------------------------------------------------------------
//------------------------------释放蓄力技能 指令预存
bool	CGameSkillMgr::addSkillChargeCmd(uint32 uSkillID,uint8& uCode)
{
	uCode = R_ResultSkill_Fail;
	CGameSkill* pSkill = m_mapSkill.find(uSkillID);
	if (!pSkill)
		return false;

	if ( !m_queueCmd.empty() )
	{
		_stSkillCmd* pPreCmd = m_queueCmd.back();
		if (pPreCmd&&pPreCmd->eCmdType!=SkillCmdType_Use)
		{
			return false;
		}
		else
		{
			// 使用技能命令会把其他命令都互斥掉
			m_queueCmd.clear();
		}
	}
	_stSkillCmd* pNewCmd = m_queueCmd.push();
	if (!pNewCmd)
		return false;
	pNewCmd->eCmdType			= SkillCmdType_Charge;
	pNewCmd->pSkillModel		= pSkill->getSkillModel();
	uCode = R_ResultSkill_Succeed;
	return true;
}
//-------------------------------------------------------------
//------------------------------打断技能 指令预存
bool	CGameSkillMgr::addSkillBreakCmd(uint32 uSkillID,uint8& uCode)
{
	uCode = R_ResultSkill_Fail;
	if (!m_pOwner)
		return false;

	CGameSkill* pSkill = m_mapSkill.find(uSkillID);
	if (!pSkill)
		return false;
	
	// 清除命令队列
	m_queueCmd.clear();
	m_pOwner->getSkilling().breakSkillAsk(uSkillID,true,uCode);
	uCode = R_ResultSkill_Succeed;
	return true;
}
//-------------------------------------------------------------
//------------------------------
void	CGameSkillMgr::clearSkillCmd	()
{
	m_queueCmd.clear();
}
//-------------------------------------------------------------
//------------------------------
void	CGameSkillMgr::processNextCmd	()
{
	if ( !m_pOwner )
		return ;

	//眩晕死亡状态下 直接清空
	if (!m_pOwner->canUseSkill(NULL))
	{
		clearSkillCmd();
		return;
	}

	_stSkillCmd* pNextCmd = m_queueCmd.front();
	if (!pNextCmd )
		return ;

	if (!pNextCmd->pSkillModel)
	{// 为了逻辑正确
		m_queueCmd.pop();
		return ;
	}

	switch (pNextCmd->eCmdType)
	{
	case SkillCmdType_Use:
		{
			if ( pNextCmd->pSkillModel->uGroupCoolTicket && m_pOwner->getCooling().isCoolGroup(CoolingType_Skill, pNextCmd->pSkillModel->uSkillID, SKILL_GOOLING_GROUP_ID) )
				return;

			if ( pNextCmd->pSkillModel->uCoolTicket && m_pOwner->getCooling().isCooling(CoolingType_Skill, pNextCmd->pSkillModel->uSkillID, SKILL_GOOLING_GROUP_ID) )
				break;

			if( m_pOwner->haveState(StateType_Singing) || m_pOwner->haveState(StateType_Skiling) )
			{
				//中断优先级
				const _stSkillModel* pOldSkillModel = m_pOwner->getSkilling().getSkillModel();
				if (pOldSkillModel->uInterruptValue > 0 && pNextCmd->pSkillModel->uInterruptValue > pOldSkillModel->uInterruptValue)
				{
					uint8 uCode = 0;
					if (!m_pOwner->getSkilling().breakSkillAsk(pOldSkillModel->uSkillID,true,uCode))
						return;
				}
				else
				{
					return ;
				}
			}

			uint8 uCode = R_ResultSkill_Fail;
			m_pOwner->setPosition(pNextCmd->stSelfPoint.x,pNextCmd->stSelfPoint.y,SetPositionReason_Skill,false);
			if (!useSkillAsk(pNextCmd->pSkillModel->uSkillID,pNextCmd->fDirection,pNextCmd->uTarUnitID,pNextCmd->stTargetPoint,uCode))
			{
				//CGameSkillMgr::send_UseSkill_Ack(pNextCmd->pSkillModel->uSkillID,0,0,pNextCmd->uTarUnitID,pNextCmd->stTargetPoint,uCode,false,m_pOwner);
			}
		}
		break;
	case SkillCmdType_Charge:
		{
			uint8 uCode;
			m_pOwner->getSkilling().useChargeAsk(pNextCmd->pSkillModel->uSkillID,uCode);
		}
		break;
	default:
		break;
	}

	m_queueCmd.pop();
}
//-------------------------------------------------------------
//------------------------------
bool	CGameSkillMgr::trigerSkill					(const _stSkillModel* pSkillModel,uint32 uTarUnitID,uint32 uTrigerParam)
{
	if ( !pSkillModel || !m_pOwner )
		return false;

	//是否冷却中
	if (pSkillModel->uCoolTicket && m_pOwner->getCooling().isCooling(CoolingType_Skill,pSkillModel->uSkillID))
		return false;

	switch ( pSkillModel->uUseType )
	{
	case SkillUseType_AtkTriger:
	case SkillUseType_BeAtkTriger:
	case SkillUseType_BeKillTriger:
		{
			if ((int32)randRange(10000) > pSkillModel->nUseParam )
				return false;
		}
		break;
	case SkillUseType_HpLimitTriger:
		{
			int32 nMaxHp = m_pOwner->getHPMax();
			int32 nCurHp = m_pOwner->getHP();
			if ( nCurHp <= 0 || nMaxHp <= 0 )
				return false;

			float32	fPercent = (float32)nCurHp/(float32)nMaxHp;
			if ( pSkillModel->nUseParam > 0 )
			{
				if ( fPercent < (float32)pSkillModel->nUseParam/10000.f)
					return false;
			}
			else
			{
				if ( fPercent > (float32)(pSkillModel->nUseParam*-1)/10000.f)
					return false;
			}
		}
		break;
	case SkillUseType_UseItem:
		{
			if ( (uint32)pSkillModel->nUseParam != uTrigerParam )
				return false;
		}
		break;
	case SkillUseType_ContinueNotHit:
		{
			if ( m_uContinueNotHit < (uint32)pSkillModel->nUseParam )
				return false;
		}
		break;
	default:
		return false;
	}

	_stPoint2F stTargetPoint= m_pOwner->getPosition();
	bool bRet = CSkilling::useNoAnimSkill(m_pOwner,pSkillModel,uTarUnitID,stTargetPoint);
	if (bRet)
	{
		if (pSkillModel->uCoolTicket)
		{
			m_pOwner->getCooling().addCooling(CoolingType_Skill,pSkillModel->uSkillID,pSkillModel->uCoolTicket);
		}
	}
	return bRet;
}
//-------------------------------------------------------------
//------------------------------
bool	CGameSkillMgr::tryTrigerSkill					(uint8 uSkillUseType,uint32 uTarUnitID,uint32 uTrigerParam )
{
	if ( uSkillUseType >= _emSkillUseType_MAX )
		return false;

	bool bRet = false;
	VEC_SKILL& vecSkill = m_vecSkill[uSkillUseType];
	VEC_SKILL::iterator _pos1 = vecSkill.begin();
	VEC_SKILL::iterator _end1 = vecSkill.end();
	for ( ; _pos1 != _end1 ; ++_pos1 )
	{
		CGameSkill* pSkill = *_pos1;
		if ( !pSkill )
			continue;

		bool bSucc = trigerSkill(pSkill->getSkillModel(),uTarUnitID,uTrigerParam);
		if ( bSucc )
			bRet = true;
	}
	return bRet;
}
//-------------------------------------------------------------
//------------------------------
void	CGameSkillMgr::sortSkill					()
{
	for ( uint32 i = 0 ; i < _emSkillUseType_ARRAYSIZE ; ++i )
	{
		m_vecSkill[i].clear();
	}

	POOL_SKILL::iterator _pos;
	m_mapSkill.getHead(_pos);
	while (!m_mapSkill.isTail(_pos))
	{
		CGameSkill* pGameSkill = m_mapSkill.getNext(_pos);
		if (!pGameSkill )
			continue;
		 
		const _stSkillModel* pModel = pGameSkill->getSkillModel();
		if (!pModel || pModel->uUseType >= _emSkillUseType_ARRAYSIZE )
			continue;

		m_vecSkill[pModel->uUseType].push_back(pGameSkill);
	}
}
//-------------------------------------------------------------
//------------------------------
void	CGameSkillMgr::callbackUseSkill			(uint32 uTarUnitID)
{
	tryTrigerSkill(SkillUseType_AtkTriger,uTarUnitID);
}
//-------------------------------------------------------------
//------------------------------  
void CGameSkillMgr::callbackBeSkillHit	(CSceneUnit* pUnit,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel)
{
	tryTrigerSkill(SkillUseType_BeAtkTriger,pUnit?pUnit->getUnitID():0);
}
//-------------------------------------------------------------
//------------------------------  
void	CGameSkillMgr::callbackBeKillHarm			(CSceneUnit* pUnit,int32& iDamage)
{
	if ( !m_pOwner || m_pOwner->getHP() <= 0)
		return ;

	tryTrigerSkill(SkillUseType_BeKillTriger,pUnit?pUnit->getUnitID():0);
}
//-------------------------------------------------------------
//------------------------------
void	CGameSkillMgr::callbackUpdateHP			()
{
	tryTrigerSkill(SkillUseType_HpLimitTriger,0);
}
//-------------------------------------------------------------
//------------------------------
void	CGameSkillMgr::callbackUseItem				(const _stItemModel* pItemModel)
{
	if ( !pItemModel )
		return;

	tryTrigerSkill(SkillUseType_UseItem,0,pItemModel->uType);
}
//-------------------------------------------------------------
//------------------------------
void	CGameSkillMgr::callbackSkillNotHit			()
{
	++m_uContinueNotHit;
	tryTrigerSkill(SkillUseType_ContinueNotHit,0);
}
//-------------------------------------------------------------
//------------------------------
void	CGameSkillMgr::callbackSkillHit			()
{
	m_uContinueNotHit = 0;
}