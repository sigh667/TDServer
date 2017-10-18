/*----------------- gameBuff.cpp
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/1/26 14:49:37
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "gameLogic/gameBuff/gameBuff.h"
#include "basic/basicFunctions.h"
#include "basic/timeFunction.h"
#include "gameLogic/sceneObject/sceneUnit.h"
#include "gameLogic/sceneObject/sceneTrap.h"
#include "log/log.h"
#include "protocol/GameDefine.pb.h"
#include "cfg_data/dazuoguajiData/dazuoguajiData.h"
#include "gameLogic/playerCommon/playerCommon.h"
//#include "gameLogic/playerTeam/playerTeam.h"
#include "gameLogic/sceneObject/scenePlayer.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
CGameBuff::CGameBuff()
{
	initialize();
}

//-------------------------------------------------------------
//------------------------------ 
CGameBuff::~CGameBuff()
{
}

//-------------------------------------------------------------
//------------------------------ 
void CGameBuff::initialize(uint32 uSourceID,CSceneUnit* pOwner,const _stBuffModel* pModel,uint64 uLeftTick)
{
	m_uSourceID = uSourceID;
	m_pOwner = pOwner;

	m_pBuffModel = pModel;
	if (pModel && !pModel->uContinueTime)
		uLeftTick = 0;

	m_uContinueTick = uLeftTick;
	m_uNextTick = 0;
	m_uBeginTick = 0;
	m_uLayer = 0;
	m_bDelete = false;
	if (pOwner && pModel)
	{
		start(uSourceID,uLeftTick,1);
	}
}

//-------------------------------------------------------------
//------------------------------ 
void CGameBuff::start(uint32 uSourceID,uint64 uLeftTick,int8 iAddLayer/* = 0*/)
{
	if (!canOperate())
		return;

	//CLog::error(" CGameBuff::start 开始执行buff:%d,uLeftTick:%lld,iAddLayer:%d;",getBuffID(),uLeftTick,iAddLayer);
	m_uContinueTick = uLeftTick;
	m_uBeginTick = getSystemTick64();

	CSceneUnit* pSource = m_pOwner->findCellObject(uSourceID);
	if (m_pBuffModel->uBuffType == BuffType_ContiueAddExpend)
	{
		m_uNextTick = m_uBeginTick + m_pBuffModel->stContiueAddExpend.uEffectTime;
	}
	else if (m_pBuffModel->uBuffType == BuffType_ContiueAddBuff)
	{
		m_uNextTick = m_uBeginTick + m_pBuffModel->stContiueAddBuff.uEffectTime;
	}
	else if (m_pBuffModel->uBuffType == BuffType_ContiueUseSkill)
	{
		m_uNextTick = m_uBeginTick + m_pBuffModel->stContiueUseSkill.uEffectTime;
	}
	else if (m_pBuffModel->uBuffType == BuffType_Attri)
	{
		
	}
	else if (m_pBuffModel->uBuffType == BuffType_AddState)
	{
	}
	else if (m_pBuffModel->uBuffType == BuffType_Move && pSource)
	{
		_stPoint2F stTargetPoint = pSource->getSkilling().getTargetPoint();
		//设置buff时间
		if(BuffMoveType_Back == m_pBuffModel->stAddMove.uBuffMoveType || BuffMoveType_BackFly == m_pBuffModel->stAddMove.uBuffMoveType)
		{
			switch (m_pBuffModel->stAddMove.uTargetType)
			{
			case BuffMoveTargetType_Back:
				//用buff配置的时间
				break;
			case BuffMoveTargetType_BackFly:
				//重新计算时间
				m_uContinueTick = (uint64)((stTargetPoint-m_pOwner->getPosition()).len()/m_pBuffModel->stAddMove.fSpeed*1000);
				break;
			case BuffMoveTargetType_TrapEnd:
				{//如果是trap释放的此类技能,需要直接按照trap的终点来设置位移终点.
					CSceneTrap *pTrap = pSource->asTrap();
					if (pTrap)
						stTargetPoint = pTrap->getEndPoint();

					m_uContinueTick = (uint64)((stTargetPoint-m_pOwner->getPosition()).len()/m_pBuffModel->stAddMove.fSpeed*1000);
				}
			default:
				break;
			}
		}

		if (BuffMoveType_Fly == m_pBuffModel->stAddMove.uBuffMoveType)
		{
			m_uContinueTick = m_pOwner->getFlyTotalTick();
		}
		else
		{
			if (BuffMoveType_BackFly ==  m_pBuffModel->stAddMove.uBuffMoveType
				||BuffMoveTargetType_TrapEnd ==  m_pBuffModel->stAddMove.uBuffMoveType
				)
			{
				m_uContinueTick += m_pOwner->getBakFlayTick();
			}
			addMove(stTargetPoint,pSource->getDirection());
			//CLog::warn("buff   %d, set move ", getBuffID());
		}
		m_uContinueTick = MAX((uint64)1,m_uContinueTick);
	}
	else if (m_pBuffModel->uBuffType == BuffType_HatredLevel && pSource)
	{
		//更改仇恨等级.添加
		m_pOwner->getHatredMgr().addHatredLevel(pSource, m_pBuffModel->nHatredLevel);

	}
	else if (m_pBuffModel->uBuffType == BuffType_Dazuoguaji)
	{
		m_uNextTick = m_uBeginTick + m_pBuffModel->stContiueAddBuff.uEffectTime;
	}

	if (m_pBuffModel->uStateType)
	{
	}
	if (m_pBuffModel->uMaxLayer)
		addLayer(iAddLayer);
}
 
//-------------------------------------------------------------
//------------------------------ 
void CGameBuff::act	()
{
	if (!canOperate() || isDelete())
		return;

	if (BuffType_AttackAddExpend == m_pBuffModel->uBuffType)
	{
		if (m_pBuffModel->stAttackAddExpend.uExpendType)
		{
			CSceneUnit* pSource = m_pOwner->findCellObject(m_uSourceID);
			m_pOwner->addExpend(m_pBuffModel->stAttackAddExpend.uExpendType,m_pBuffModel->stAttackAddExpend.iValue,DoingType_Buff,pSource);
			if (m_pOwner->isExpendFull(m_pBuffModel->stAttackAddExpend.uExpendType))
				setDelete(true);
		}
	}
}
//-------------------------------------------------------------
//------------------------------ 
void CGameBuff::onUseSkill	(uint32 uSkillID)
{
	if (!canOperate() || !uSkillID)
		return;
	CSceneUnit* pSource = m_pOwner->findCellObject(m_uSourceID);
	if (pSource)
	{
		_stPoint2F stTargetPoint = m_pOwner->getPosition();
		pSource->getGameSkillMgr().useSkill(uSkillID,m_pBuffModel->uLevel,pSource->getDirection(),m_pOwner->getUnitID(),stTargetPoint);
	}
}
//-------------------------------------------------------------
//------------------------------ 
void CGameBuff::beSkillHit	(CSceneUnit* pSource,const _stSkillModel*pSkillModel)
{
	if (!canOperate() || !pSource || !pSkillModel)
		return;
	CSceneUnit* m_pSource = m_pOwner->findCellObject(m_uSourceID);
	if (BuffType_BeSkillHitSUseSkill == m_pBuffModel->uBuffType)
	{
		if (m_pBuffModel->stBeSkillHitUseSkill.uBeSkillID == pSkillModel->uSkillID && m_pSource)
		{
			_stPoint2F stTargetPoint = m_pOwner->getPosition();
			m_pSource->getGameSkillMgr().useSkill(m_pBuffModel->stBeSkillHitUseSkill.uUseSkillID,m_pBuffModel->uLevel,m_pSource->getDirection(),m_pOwner->getUnitID(),stTargetPoint);
		}
	}
	else if (BuffType_BeSkillHitDelBuff == m_pBuffModel->uBuffType)
	{
		if (0 == m_pBuffModel->stBeSkillHitDelBuff.uBeSkillID  || m_pBuffModel->stBeSkillHitDelBuff.uBeSkillID == pSkillModel->uSkillID)
		{
			setDelete(true);
		}
	}
	if (BuffType_BeSkillHitTUseSkill == m_pBuffModel->uBuffType)
	{
		if ((!m_pBuffModel->stBeSkillHitTarUseSkill.uBeSkillID || m_pBuffModel->stBeSkillHitTarUseSkill.uBeSkillID == pSkillModel->uSkillID )&& m_pSource)
		{
			_stPoint2F stTargetPoint = pSource->getPosition();
			m_pSource->getGameSkillMgr().useSkill(m_pBuffModel->stBeSkillHitTarUseSkill.uUseSkillID,m_pBuffModel->uLevel,m_pSource->getDirection(),pSource->getUnitID(),stTargetPoint);
		}
	}

	if (m_pBuffModel->check_mark(BuffMark_BeSkillHitDelete))
	{
		setDelete(true);
	}
}
//-------------------------------------------------------------
//------------------------------ 
void CGameBuff::repeat(const _stBuffModel* pModel,uint64 uLeftTick)
{
	if (!canOperate() || !pModel)
		return;

	m_pBuffModel = pModel;
	//处理叠加
	if (BuffSameRepeatType_Drop == m_pBuffModel->uRepeatType)
	{
		return;
	}
	else if (BuffSameRepeatType_Refresh == m_pBuffModel->uRepeatType)
	{
		m_uContinueTick = uLeftTick;
		m_uBeginTick = getSystemTick64();

		//start(m_pSource,uLeftTick,0);
	}
	else if (BuffSameRepeatType_Time == m_pBuffModel->uRepeatType)
	{
		m_uContinueTick += uLeftTick;
		m_uContinueTick = MIN(m_uContinueTick,(uint64)m_pBuffModel->uMaxContinueTime);
	}

	//层数叠加
	if (m_pBuffModel->uMaxLayer)
		addLayer();
}
//-------------------------------------------------------------
//------------------------------
void	 CGameBuff::addLayer(uint8 uAddNum/* = 1*/)
{
	if (!canOperate() || isDelete())
		return;

	if (!m_pBuffModel->uMaxLayer)
		return;

	m_uLayer += uAddNum;
	m_uLayer = MIN(m_uLayer,m_pBuffModel->uMaxLayer);

	//层数加buff
	const uint32 * uBuffID = m_pBuffModel->mapBuff.find_(m_uLayer);
	if (uBuffID )
	{
		m_pOwner->getGameBuffMgr().addBuff(*uBuffID, getLevel(), m_uSourceID,0, DoingType_Buff);
	}

	//层数使用技能，达到最大层数，并且要清除此buff
	const uint32 * uSkillID = m_pBuffModel->mapSkillID.find_(m_uLayer);
	if (uSkillID)
	{
		onUseSkill(*uSkillID);
	}
}

//-------------------------------------------------------------
//------------------------------
uint64	 CGameBuff::getLeftTick()
{
	uint64 uTick = getSystemTick64();
	return m_uContinueTick + m_uBeginTick  > uTick ? m_uContinueTick + m_uBeginTick  - uTick : 0;
}

//-------------------------------------------------------------
//------------------------------
void	 CGameBuff::setDelete(bool bSet/* = true*/)
{
	if (!canOperate())
		return;

	if ( m_pBuffModel->uBuffCat == BuffCat_PassiveSkill )
		return ;

	m_bDelete = bSet;
}

//-------------------------------------------------------------
//------------------------------
void	 CGameBuff::onDelete()
{
	if (!canOperate())
		return;

	if (m_pBuffModel->uBuffType == BuffType_Attri)
	{
		
	}
	else if (m_pBuffModel->uBuffType == BuffType_AddState)
	{
	}
	else if (m_pBuffModel->uBuffType == BuffType_Move)
	{

	}
	else if (m_pBuffModel->uBuffType == BuffType_HatredLevel)
	{
		//恢复仇恨等级
		CSceneUnit* m_pSource = m_pOwner->findCellObject(m_uSourceID);
		m_pOwner->getHatredMgr().addHatredLevel(m_pSource, m_pBuffModel->nHatredLevel * -1);
	}

	//删除buff触发事件
	CONST_ITER_MAP_U8_U32 _pos = m_pBuffModel->mapDeleteTriggerEvent.begin();
	for (; _pos != m_pBuffModel->mapDeleteTriggerEvent.end(); ++_pos)
	{
		uint8 uType = _pos->first;
		switch (uType)
		{
		case BuffDeleteTriggerEventType_Skill:
			this->onUseSkill(_pos->second);
			break;
		case BuffDeleteTriggerEventType_Buff:
			m_pOwner->getGameBuffMgr().addBuff(_pos->second,m_pBuffModel->uLevel,m_pOwner->getUnitID(),0,DoingType_Buff);
			break;
		default:
			break;
		}
	}
}

//-------------------------------------------------------------
//------------------------------
bool	CGameBuff::processContinue	(uint64 uTick,uint64 uTime,uint32 uSecond)
{
	if (!canOperate() || isDelete() || !m_uNextTick || uTick < m_uNextTick)
		return false;

	CSceneUnit* pSource = m_pOwner->findCellObject(m_uSourceID);
	if (m_pBuffModel->uBuffType == BuffType_ContiueAddExpend)
	{
		//删除消耗值 失败后删除buff
		if (m_pBuffModel->stContiueAddExpend.uDelExpendType)
		{
			int32 iDelValue = m_pBuffModel->stContiueAddExpend.iDelValue;
			if (iDelValue > 0 &&!m_pOwner->canExpend(m_pBuffModel->stContiueAddExpend.uDelExpendType,iDelValue))
			{
				setDelete();
				return true;
			}
			m_pOwner->addExpend(m_pBuffModel->stContiueAddExpend.uDelExpendType,iDelValue * -1,DoingType_Buff,pSource);
		}

		int32 iValue = m_pBuffModel->stContiueAddExpend.iValue;
		
		if (!iValue && m_pBuffModel->stContiueAddExpend.uAttrType < _emBattleAttribute_ARRAYSIZE && m_pBuffModel->stContiueAddExpend.uAttrType > 0)
		{
			if (EffectTargetType_Self == m_pBuffModel->stContiueAddExpend.uTargetCategory)
			{
				iValue = m_pOwner->getGameAttribute().iAttribute[m_pBuffModel->stContiueAddExpend.uAttrType] * m_pBuffModel->stContiueAddExpend.iRadio / 10000;
			}
			else if (pSource)
			{
				iValue = pSource->getGameAttribute().iAttribute[m_pBuffModel->stContiueAddExpend.uAttrType] * m_pBuffModel->stContiueAddExpend.iRadio / 10000;
			}
		}

		//每叠加1层则效果增加，叠到N层时效果为初始状态的N倍
		if (m_uLayer)
			iValue *= m_uLayer;

		m_pOwner->addExpend(m_pBuffModel->stContiueAddExpend.uExpendType,iValue,DoingType_Buff,pSource);

		m_uNextTick = m_uNextTick + m_pBuffModel->stContiueAddExpend.uStepTime;
	}
	else if (m_pBuffModel->uBuffType == BuffType_ContiueAddBuff)
	{
		m_pOwner->getGameBuffMgr().addBuff(m_pBuffModel->stContiueAddBuff.uBuffID,getLevel(),m_uSourceID,0, DoingType_Buff);
		m_uNextTick = m_uNextTick + m_pBuffModel->stContiueAddBuff.uStepTime;
	}
	else if (m_pBuffModel->uBuffType == BuffType_ContiueUseSkill)
	{

		uint32 uSkillID =m_pBuffModel->stContiueUseSkill.uSkillID;
		const uint32 *ptr = m_pBuffModel->mapSkillID.find_(m_uLayer);
		if (m_pBuffModel->mapSkillID.is_exist(m_uLayer) && ptr)
			uSkillID = *ptr;

		onUseSkill(uSkillID);

		m_uNextTick = m_uNextTick + m_pBuffModel->stContiueUseSkill.uStepTime;
	}
	else if (m_pBuffModel->uBuffType == BuffType_Dazuoguaji)
	{
		m_uNextTick = m_uNextTick + m_pBuffModel->stContiueAddBuff.uStepTime;
		if (m_pOwner->asPlayer())
		{
			//根据每日上限增加经验以及韬略
			const _stDazuoguajiModel* pDazuoguaji = g_clDazuoguajiData.findDazuoguaji(m_pOwner->getLevel());
			if (pDazuoguaji)
			{
				////判断队员在身边
				//bool isTeamMemberAroundTakeSeat = m_pOwner->asPlayer()->getPlayerTeam()->isMemberAroundTakeSeat();


				////增加经验
				//uint32 uAddExp = pDazuoguaji->uSingleExp;
				//if (isTeamMemberAroundTakeSeat)
				//{
				//	uAddExp += pDazuoguaji->uTeamAdditionExp;
				//}
				//uint32 uTotalExpAdded = m_pOwner->asPlayer()->getPlayerCommon()->getDailyDazuoguajiExp() ;
				//if (uAddExp + uTotalExpAdded > pDazuoguaji->uDailyTotalExp)
				//{
				//	uAddExp = pDazuoguaji->uDailyTotalExp - uTotalExpAdded;
				//}
				//m_pOwner->asPlayer()->addExp(uAddExp, DoingType_Dazuoguaji);
				//m_pOwner->asPlayer()->getPlayerCommon()->setDailyDazuoguajiExp(uAddExp + uTotalExpAdded);

				////增加韬略
				//uint32 uAddWisdom = pDazuoguaji->uSingleTaolue;
				//if (isTeamMemberAroundTakeSeat)
				//{
				//	uAddWisdom += pDazuoguaji->uTeamAdditionTaolue;
				//}
				//uint32 uTotalTaolueAdded = m_pOwner->asPlayer()->getPlayerCommon()->getDailyDazuoguajiTaolue() ;
				//if (uAddWisdom + uTotalTaolueAdded > pDazuoguaji->uDailyTotalTaolue)
				//{
				//	uAddWisdom = pDazuoguaji->uDailyTotalTaolue - uTotalTaolueAdded;
				//}
				//m_pOwner->asPlayer()->addExpend(ExpendType_Wisdom, uAddWisdom, DoingType_Dazuoguaji);
				//m_pOwner->asPlayer()->getPlayerCommon()->setDailyDazuoguajiTaolue(uAddWisdom + uTotalTaolueAdded);

				////返回剩余属性
				//if (uAddExp > 0 || uAddWisdom > 0)
				//{
				//	uTotalExpAdded = m_pOwner->asPlayer()->getPlayerCommon()->getDailyDazuoguajiExp() ;
				//	uTotalTaolueAdded = m_pOwner->asPlayer()->getPlayerCommon()->getDailyDazuoguajiTaolue() ;
				//	m_pOwner->asPlayer()->getPlayerCommon()->sendDazuoguajiNtf(pDazuoguaji->uDailyTotalExp - uTotalExpAdded
				//		, pDazuoguaji->uDailyTotalTaolue - uTotalTaolueAdded
				//		, uAddExp, uAddWisdom
				//		, false, false);
				//}
			}
		}
		
		
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------
bool	CGameBuff::process	(uint64 uTick,uint64 uTime,uint32 uSecond)
{
	if (!canOperate() || isDelete() || !m_uBeginTick)
		return false;

	processContinue(uTick,uTime,uSecond);

	//Buff的层数减1，然后将buff时间重置为初始状态，如果Buff层数减为0则移除 减层数暂时不处理 有问题
	if (m_uBeginTick && m_uBeginTick + m_uContinueTick <= uTick)
	{
		setDelete(true);
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 技能位移
bool	CGameBuff::addMove(_stPoint2F& stTarget,float32 fSkillDir)
{
	if (!canOperate())
		return false;
	CSceneUnit* pSource = m_pOwner->findCellObject(m_uSourceID);
	if (!pSource)
		return false;

	float32 fdis = 0;
	switch (m_pBuffModel->stAddMove.uTargetType)
	{
	case BuffMoveTargetType_Back:
		fdis = m_pBuffModel->stAddMove.fSpeed * m_pBuffModel->uContinueTime / 1000;
		break;
	case BuffMoveTargetType_BackFly:
	case BuffMoveTargetType_TrapEnd:
		fdis = (stTarget-m_pOwner->getPosition()).len();
		break;
	default:
		break;
	}
	//float32 fdis = m_pBuffModel->stAddMove.fSpeed * m_uContinueTick / 1000;
	m_pOwner->setNewPosition(pSource,fdis,m_pBuffModel->stAddMove.uDirectionType);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 技能位移
bool	CGameBuff::updateLeftTick(uint64 uContinueTick,bool bSend)
{
	if (!canOperate())
		return false;

	repeat(m_pBuffModel,uContinueTick);
	if (bSend)
	{
		CGameBuffMgr::send_UpdateBuffTickAck(m_pBuffModel->uBuffID,m_uSourceID,m_uContinueTick,m_pOwner);
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------ 增加属性
void	CGameBuff::calcAttr		()
{
	if (!canOperate())
		return;

	if (m_pBuffModel->uBuffType != BuffType_Attri)
		return;

	uint8 uLayer = MAX(m_uLayer,(uint8)1);
	for (uint32 i = 0; i < m_pBuffModel->vecAddBattleAttr.size(); ++i)
	{
		const _stAddBattleAttri&	stAddBattleAttri = m_pBuffModel->vecAddBattleAttr[i];
		int32 iValue = stAddBattleAttri.iValue;
		if (stAddBattleAttri.uTransAttrType > 0)
		{
			iValue += m_pOwner->getAttribute((uint8)stAddBattleAttri.uTransAttrType) * stAddBattleAttri.iTransRadio / MAX_PROBABILITY;
		}
		m_pOwner->addAttribute(GameSystemType_Buff,stAddBattleAttri.uAttrType,iValue * uLayer,stAddBattleAttri.iRadio * uLayer);
	}
}
//-------------------------------------------------------------
//------------------------------ 获取额外伤害
uint32	CGameBuff::getExtraDamage(int32 iDamage)
{
	if (!canOperate() || iDamage < 0)
		return 0;
	 uint32 uValue = uint32(iDamage * (m_pBuffModel->stSkillExtraDamage.uRadio / FLOAT_MAX_PROBABILITY) + m_pBuffModel->stSkillExtraDamage.uValue);
	 if (m_uLayer)
		uValue *=  m_uLayer;
	 return uValue;
}
