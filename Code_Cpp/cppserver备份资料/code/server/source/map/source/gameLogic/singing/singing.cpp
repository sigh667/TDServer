/*----------------- singing.cpp
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/1/21 18:55:20
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "gameLogic/singing/singing.h"
#include "basic/timeFunction.h"
#include "gameLogic/singing/singing.h"
#include "protocol/CASMsg.pb.h"
#include "protocol/CASProtocol.pb.h"
#include "protocol/MsgResult.pb.h"
#include "gameLogic/sceneObject/sceneUnit.h"
#include "gameLogic/sceneObject/sceneMonster.h"
#include "cfg_data/skillData/skillData.h"
#include "cfg_data/monsterData/monsterData.h"
/*************************************************************/
//#############################################################
//############################## 
//#############################################################
//-------------------------------------------------------------
//------------------------------ 
CSingObject::CSingObject()
{
	initSingObject(0,0,0,0,NULL);
}
//-------------------------------------------------------------
//------------------------------ 
void	CSingObject::initSingObject	(uint32 uSkillID,uint8 uSkillLevel,uint32 uTargetID,uint32 uTick,const _stPoint2F*pPoint)
{
	m_uSingTick		= uTick;
	m_uTriggerID	= uTargetID;
	m_uSkillID		= uSkillID;
	m_uSkillLevel	= uSkillLevel;
	if(pPoint)
		m_stTargetPoint	= *pPoint;
	else
		m_stTargetPoint.zero();
}

//#############################################################
//############################## 
//#############################################################
//-------------------------------------------------------------
//------------------------------ 
CSinging::CSinging()
{
	initialize(NULL);
}

//-------------------------------------------------------------
//------------------------------ 
void	CSinging::initialize(CSceneUnit* pOwner)
{
	m_uBeginTick	= 0;
	m_uEndTick		= 0;
	m_uInteTick		= 0;
	m_pObject		= NULL;
	m_pOwner		= pOwner;
	m_uSingState	= SingState_Null;
}
//-------------------------------------------------------------
//------------------------------ 
uint32	CSinging::getSingTick		()const
{
	if (!m_pOwner)
		return 0;

	uint64 uTick = 0;
	if(m_uEndTick > uTick)
		return uint32(m_uEndTick - uTick);

	return 0;
}

//-------------------------------------------------------------
//------------------------------ 吟唱推进处理
bool	CSinging::process			(uint64 uTick)
{
	if(!m_pObject || SingState_End == m_uSingState) 
		return false;

	if(uTick < m_uEndTick)
		return false;

	doneSinging();
	return true;
}

//-------------------------------------------------------------
//------------------------------ 开始吟唱
bool	CSinging::beginSinging	(CSingObject*pSing)
{
	if(m_pObject || !m_pOwner || !pSing)
		return false;

	m_pObject		= pSing;
	m_uBeginTick	= getSystemTick64();
	m_uEndTick		= m_uBeginTick + pSing->m_uSingTick;
	m_uSingState	= SingState_Ing;
	m_pOwner->addState(StateType_Singing);
	
	//给目标加状态
	CSceneUnit* pTargetUnit = m_pOwner->findCellObject(pSing->m_uTriggerID);
	if (pTargetUnit && pSing->getSkillID())
	{
		_stSkillModel* pModel = g_clSkillData.findSkill(pSing->getSkillID(),pSing->getSkillLevel());
		if (pModel && pModel->uType == SkillType_Arrest)
		{
			pTargetUnit->addState(StateType_Hold);
		}

		CSceneMonster* pMonster = pTargetUnit->asMonster();
		if (pMonster && pMonster->getMonsterModel() && pMonster->getMonsterModel()->check_mark(MonsterMark_PvpWarBanner))
		{
			pMonster->setActivePlayer(m_pOwner->asPlayer());
		}
	}

	CGameSkillMgr::send_UseSkill_Ack(pSing->getSkillID(),pSing->getSkillLevel(),0,pSing->getSingTriggerID(),pSing->getTargetPoint(),R_ResultSkill_Succeed,true,m_pOwner);
	return true;
}

//-------------------------------------------------------------
//------------------------------ 吟唱完成 使用技能
void	CSinging::doneSinging		()
{
	if (!m_pObject)
		return;

	//使用技能
	m_uSingState = SingState_End;
	uint32 uSingingTick = (uint32)(getSystemTick64() - m_uBeginTick);
	m_pObject->onDoneSinging(uSingingTick);
	endSinging();
}

//-------------------------------------------------------------
//------------------------------ 结束吟唱
void	CSinging::endSinging		()
{
	//取消状态
	m_pOwner->delState(StateType_Singing);

	//给目标删除加状态
	CSceneUnit* pTargetUnit = NULL;
	if (m_pObject)
	{
		pTargetUnit = m_pOwner->findCellObject(m_pObject->m_uTriggerID);
	}
	if (pTargetUnit && m_pObject && m_pObject->getSkillID())
	{
		_stSkillModel* pModel = g_clSkillData.findSkill(m_pObject->getSkillID(),m_pObject->getSkillLevel());
		if (pModel && pModel->uType == SkillType_Arrest)
		{
			pTargetUnit->delState(StateType_Hold);
		}

		CSceneMonster* pMonster = pTargetUnit->asMonster();
		if (pMonster && pMonster->getMonsterModel() && pMonster->getMonsterModel()->check_mark(MonsterMark_PvpWarBanner))
		{
			pMonster->setActivePlayer(NULL);
		}
	}

	initialize(m_pOwner);
}

//-------------------------------------------------------------
//------------------------------ 中断取消吟唱
bool	CSinging::breakSinging	(bool bInter)
{
	if(!m_pObject)
		return false;

	//打断广播
	CGameSkillMgr::send_SkillBreak_Ack(m_pObject->getSkillID(),0,R_ResultSkill_Succeed,true,m_pOwner);

	endSinging();
	return true;
}

