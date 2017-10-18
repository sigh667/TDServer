/*----------------- skilling.cpp
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/1/26 9:57:51
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "gameLogic/skilling/skilling.h"
#include "basic/timeFunction.h"
#include "protocol/MsgResult.pb.h"
#include "protocol/GameDefine.pb.h"
#include "gameLogic/sceneObject/sceneUnit.h"
#include "gameScene/gameSceneCell.h"
//#include "gameLogic/playerQuest/playerQuestMgr.h"
//#include "gameLogic/playerColletion/playerColletion.h"
#include "cfg_data/monsterData/monsterModel.h"
#include "cfg_data/monsterData/monsterData.h"
#include "cfg_data/constantData/constantData.h"
#include "cfg_data/skillData/skillData.h"
#include "log/log.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
CSkilling::CSkilling()
{
	m_uLastCombatTick	= 0;	//上一段连击的时间
	m_uNextCombatSkill	= 0;	//下一段连击技能ID
	m_mapChildSkill.clear();
	m_pSkillModel		= NULL;
	m_uTarUnitID = 0;
	m_stOldPosition.zero();
	initialize(NULL,NULL);
}

//-------------------------------------------------------------
//------------------------------ 
void	CSkilling::initialize(CSceneUnit* pOwner, const _stSkillModel*	pSkillModel)
{
	m_pOwner			= pOwner;
	m_pSkillModel		= pSkillModel;
	m_pSkillAnimModel	= NULL;
	m_uCurStage			 = 0;
	m_uStartAnimTick	 = 0;
	m_uExpendCount		= 0;
	m_fChargeRate		= 0.0f;
	m_setChooseTarget.clear();
	m_setGatherTarget.clear();
	m_clSkillAnim.initialize(pOwner,m_uStartAnimTick,m_pSkillModel,m_pSkillAnimModel);
}

//-------------------------------------------------------------
//------------------------------ 
bool	CSkilling::canUse		(CSceneUnit* pOwner, const _stSkillModel* pSkillModel,uint8& uCode)
{
	if (!pSkillModel || !pOwner)
		return false;

	if (!pOwner->canUseSkill(NULL))
	{
		uCode = R_ResultSkill_ActForbid;
		return false;
	}

	//是否检查CD
	if (!pOwner->check_mark(SceneUnitMark_IgnoreSkillCD) || pSkillModel->uType == SkillType_NormalAtk)
	{
		//是否冷却中
		if (pSkillModel->uCoolTicket && pOwner->getCooling().isCooling(CoolingType_Skill,pSkillModel->uSkillID,SKILL_GOOLING_GROUP_ID))
		{
			uCode = R_ResultSkill_Cooling;
			return false;
		}
	}

	//检查消耗
	if (!m_pOwner->check_mark(SceneUnitMark_IgnoreSkillExpend) && !pSkillModel->mapAddExpend.empty())
	{
		int32 nReduce = pOwner->getGameBuffMgr().getReduceSkillExpendRate();
		MAP_U16_I64 mapAddExpend;
		CONST_ITER_MAP_U16_I64 _pos = pSkillModel->mapAddExpend.begin();
		for (; _pos != pSkillModel->mapAddExpend.end(); ++_pos)
		{
			int64 iValue = _pos->second;
			iValue -= (int64)(iValue * (nReduce / 10000.0f));
			if (iValue)
				mapAddExpend.insert_(_pos->first,iValue);
		}
		if (!mapAddExpend.empty() && !pOwner->canExpend(mapAddExpend))
		{
			uCode = R_ResultSkill_Consume;
			return false;
		}
	}

	//连击技能判断
	if (SkillCompositeType_Combat == pSkillModel->stCompositeData.uType)
	{
		if (pSkillModel->stCompositeData.uAtkPart > 1)
		{
			if ( pSkillModel->stCompositeData.uAtkPartCoolTicket && ( getSystemTick64() > m_uLastCombatTick + pSkillModel->stCompositeData.uAtkPartCoolTicket ) )
			{
				uCode = R_ResultSkill_Part;
				return false;
			}

			if (!m_uNextCombatSkill || m_uNextCombatSkill != pSkillModel->uSkillID)
			{
				uCode = R_ResultSkill_Part;
				return false;
			}
		}
	}
	else if (SkillCompositeType_Child == pSkillModel->stCompositeData.uType)
	{
		_stCompositeInfo* pInfo = getChildSkill(pSkillModel->uCompositeBaseSkillID);
		if (pInfo)
		{
			if (pInfo->m_uNextSkill != pSkillModel->uSkillID)
			{
				uCode = R_ResultSkill_Part;
				return false;
			}

			if (getSystemTick64() > pInfo->m_uExpireTick)
			{
				uCode = R_ResultSkill_Part;
				return false;
			}
		}
		else
		{
			if (pSkillModel->stCompositeData.uAtkPart > 1)
			{
				uCode = R_ResultSkill_Part;
				return false;
			}
		}
	}

	//暗杀技能
	if (SkillType_Thug == pSkillModel->uType && !useRewardSkill(uCode))
			return false;

	return true;
}
//-------------------------------------------------------------
//------------------------------ 获取技能时间
uint32 CSkilling::getSkillTimeByType(const _stSkillModel* pSkillModel,_emSkillTimeType emType)
{
	if (!m_pSkillModel || !m_pOwner)
		return 0;

	uint32 uTick = 0;
	switch (emType)
	{
	case SkillTimeType_CD:
		uTick = pSkillModel->uCoolTicket;
		break;
	case SkillTimeType_Charge:
		uTick = pSkillModel->uChargeTime;
		break;
	case SkillTimeType_Continue:
		uTick = pSkillModel->uChargeTime;
		break;
	default:
		break;
	}

	//计算buff
	uTick = m_pOwner->getGameBuffMgr().getSkillTimeByTime(emType,pSkillModel->uSkillID,uTick);
	return uTick;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CSkilling::useConsume		(bool bSinging,uint8& uCode)
{
	if (!canOperate())
		return false;

	//消耗
	if (!m_pOwner->check_mark(SceneUnitMark_IgnoreSkillExpend) && !m_pSkillModel->mapAddExpend.empty())
	{
		int32 nReduce = m_pOwner->getGameBuffMgr().getReduceSkillExpendRate();
		MAP_U16_I64 mapAddExpend;
		CONST_ITER_MAP_U16_I64 _pos = m_pSkillModel->mapAddExpend.begin();
		for (; _pos != m_pSkillModel->mapAddExpend.end(); ++_pos)
		{
			int64 iValue = _pos->second;
			if (iValue < 0)
				iValue -= (int64)(iValue * (nReduce / 10000.0f));

			if (iValue)
				mapAddExpend.insert_(_pos->first,iValue);

		}
		m_pOwner->addExpend(mapAddExpend, DoingType_Skill);
		m_uExpendCount++;
	}

	//消耗增加额外伤害
	if (m_pSkillModel->stExpendAddDamage.uExpendType)
	{
		if (m_pOwner->addExpend(m_pSkillModel->stExpendAddDamage.uExpendType,m_pSkillModel->stExpendAddDamage.iValue, DoingType_Skill))
		{
			m_pOwner->update_mark(SceneUnitMark_AddExtraDamage,m_pSkillModel->uSkillID);
		}
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 技能推进处理
bool	CSkilling::process		(uint64 uTick)
{
	processEndAnim(uTick);
	processExpend(uTick);
	m_clSkillAnim.process(uTick);

	{//父子技能等待输入时间过期
		MAP_COMPOSITEINFO::iterator _pos =  m_mapChildSkill.begin();
		for (; _pos != m_mapChildSkill.end();)
		{
			_stCompositeInfo& stInfo = _pos->second;
			if (!stInfo.m_uExpireTick || stInfo.m_uExpireTick > getSystemTick64())
			{
				 ++_pos;
				continue;
			}
			CGameSkill* pGameSkill = m_pOwner->getGameSkillMgr().findSkill(stInfo.m_uBaseSkill);
			if (!pGameSkill || !pGameSkill->getSkillModel())
			{
				++_pos;
				continue;
			}

			m_mapChildSkill.erase(_pos++);
			uint32 uCoolTime = getSkillTimeByType(pGameSkill->getSkillModel(),SkillTimeType_CD);
			m_pOwner->getCooling().addCooling(CoolingType_Skill,pGameSkill->getSkillModel()->uSkillID,uCoolTime,SKILL_GOOLING_GROUP_ID, pGameSkill->getSkillModel()->uGroupCoolTicket);
		}
	}
	
	//处理组合技能的CD
	if (m_pSkillModel && m_pSkillModel->stCompositeData.uNextSkillID > 0)
	{
		if (SkillCompositeType_Combat == m_pSkillModel->stCompositeData.uType && m_uLastCombatTick && m_pSkillModel->stCompositeData.uAtkPartCoolTicket &&
			getSystemTick64() >= m_uLastCombatTick + m_pSkillModel->stCompositeData.uAtkPartCoolTicket)
		{
			m_uLastCombatTick = 0;
			m_uNextCombatSkill = 0;
			CGameSkill* pBaseGameSkill = m_pOwner->getGameSkillMgr().findSkill(m_pSkillModel->uCompositeBaseSkillID);
			if (pBaseGameSkill && pBaseGameSkill->getSkillModel())
			{
				uint32 uCoolTime = getSkillTimeByType(pBaseGameSkill->getSkillModel(),SkillTimeType_CD);
				m_pOwner->getCooling().addCooling(CoolingType_Skill,m_pSkillModel->uCompositeBaseSkillID,uCoolTime,SKILL_GOOLING_GROUP_ID, pBaseGameSkill->getSkillModel()->uGroupCoolTicket);
			}
		}
	}

	//持续性锁定目标技能 需要中断
	if (m_uTarUnitID && m_pOwner->haveState(StateType_Skiling) && m_pSkillModel && m_pSkillModel->check_mark(SkillMark_Continue) && m_pSkillModel->check_mark(SkillMark_SingLockTarget) )
	{
		CSceneUnit* pTarget = m_pOwner->findCellObject(m_uTarUnitID);
		if (!pTarget || (pTarget && pTarget->isDead()))
		{
			uint8 uCode = 0;
			breakSkillAsk(m_pSkillModel->uSkillID,true,uCode);
		}
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 消耗持续
void	CSkilling::processExpend		(uint64 uTick)
{
	if (!canOperate() || !m_uStartAnimTick || !m_uExpendCount)
		return;

	const _stSkillSpareParam* pSpareParam = m_pSkillModel->findParam(SkillSpareParamType_ExpendInterval);
	if (!pSpareParam || pSpareParam->vecParams.empty())
		return;

	//消耗不够 直接停止技能
	if (uTick >= m_uStartAnimTick + pSpareParam->vecParams[0] * m_uExpendCount)
	{
		if (!m_pOwner->canExpend(m_pSkillModel->mapAddExpend))
		{
			uint8 uCode = 0;
			breakSkillAsk(m_pSkillModel->uSkillID,true,uCode);
			return;
		}

		m_pOwner->addExpend(m_pSkillModel->mapAddExpend, DoingType_Skill);
	}

}

//-------------------------------------------------------------
//------------------------------ 动作推进处理
bool	CSkilling::processEndAnim		(uint64 uTick)
{
	if (!canOperate() || !m_pSkillAnimModel)
		return false;

	//持续技能
	if (m_pSkillModel->check_mark(SkillMark_Continue) && !m_pSkillModel->uContinueTime)
		return true;

	if (!m_clSkillAnim.isBreak() && (!m_uStartAnimTick || m_uEndAnimTick > uTick))
		return true;

	m_uStartAnimTick = 0;
	m_uEndAnimTick = 0;
	// 	if (m_pOwner->asPlayer())
	// 		CLog::error("施法结束 当前时间:%lld,技能ID:%d",uTick,m_pSkillModel->uSkillID);

	//持续技能需要广播停止
	if (m_pSkillModel->check_mark(SkillMark_Continue))
	{
		CGameSkillMgr::send_SkillBreak_Ack(m_pSkillModel->uSkillID,1,0,true,m_pOwner);
	}

	onDoneSkill();
	return true;
}

//-------------------------------------------------------------
//------------------------------ 吟唱完成 自动使用技能
void	CSkilling::onDoneSinging(uint32 uSingingTick)
{
	if (!canOperate())
		return;

	uint8 uCode = R_ResultSkill_Fail;

	//存蓄力系数
	if (m_pSkillModel->check_mark(SkillMark_Charge))
	{
		uint32 uChargeTick = getSkillTimeByType(m_pSkillModel,SkillTimeType_Charge);
		if (uChargeTick)
		{
			float32 fTmpRate = (float32) MAX(uSingingTick,(uint32)1000) / uChargeTick;
			m_fChargeRate = MIN(fTmpRate,1.0f);
		}
	}

	//使用技能
	if (!readyUseSkill(getSingTriggerID(),uCode))
	{
		CGameSkillMgr::send_UseSkill_Ack(getSkillID(),1,m_uCurStage,getSingTriggerID(),getTargetPoint(),uCode,false,m_pOwner);
	}

	// 是否采集技能
	if(m_pSkillModel->uType == SkillType_MonsterCollletion)
	{
		if (!useColletionSkill(uCode))
		{
			CGameSkillMgr::send_UseSkill_Ack(getSkillID(),1,m_uCurStage,getSingTriggerID(),getTargetPoint(),uCode,false,m_pOwner);
		}
		return;
	}
	else if (m_pSkillModel->uType == SkillType_Arrest)
	{
		if (!arrestSkill(uCode))
		{
			CGameSkillMgr::send_UseSkill_Ack(getSkillID(),1,m_uCurStage,getSingTriggerID(),getTargetPoint(),uCode,false,m_pOwner);
		}
		return;
	}
	else if (m_pSkillModel->uType == SkillType_Active)
	{
		if (!activeSkill(uCode))
		{
			CGameSkillMgr::send_UseSkill_Ack(getSkillID(),1,m_uCurStage,getSingTriggerID(),getTargetPoint(),uCode,false,m_pOwner);
		}
		return;
	}
}

//-------------------------------------------------------------
//------------------------------ 
void	CSkilling::onDoneSkill(bool bBreak/* = false*/)
{
	if (!m_pOwner || !m_pSkillModel)
		return;
	m_pOwner->callbackUseSkillEnd(m_pSkillModel);
	m_pOwner->delState(StateType_Skiling);

	//技能完成 开始缉拿
	if (!bBreak && SkillType_Arrest == m_pSkillModel->uType)
	{
		uint8 uCode = 0;
		arrestSkill(uCode);
	}

	//技能完成开始CD (持续技能)
	if (!m_pOwner->check_mark(SceneUnitMark_IgnoreSkillCD) && m_pSkillModel->check_mark(SkillMark_Continue))
	{
		uint32 uCoolTime = getSkillTimeByType(m_pSkillModel,SkillTimeType_CD);
		m_pOwner->getCooling().addCooling(CoolingType_Skill,m_pSkillModel->uSkillID,uCoolTime,SKILL_GOOLING_GROUP_ID,m_pSkillModel->uGroupCoolTicket);
	}

	m_pOwner->delete_mark(SceneUnitMark_AddExtraDamage);

	//删除状态
	const _stSkillSpareParam* pSkillSpareParam = m_pSkillModel->findParam(SkillSpareParamType_AddState);
	if (pSkillSpareParam && pSkillSpareParam->vecParams.size() > 0)
		m_pOwner->delState((_emGameStateType)(uint32)pSkillSpareParam->vecParams[0]);

	m_uStartAnimTick = 0;
	m_clSkillAnim.end();
	m_uLastCombatTick	= 0;	//上一段连击的时间
	m_uNextCombatSkill	= 0;	//下一段连击技能ID
	m_fChargeRate = 0.0f;

	//技能完成删除buff
	const _stSkillSpareParam* pSpareParam = m_pSkillModel->findParam(SkillSpareParamType_doneDelBuff);
	if (pSpareParam && pSpareParam->vecParams.size() > 0)
	{
		m_pOwner->getGameBuffMgr().delBuff((uint32)pSpareParam->vecParams[0], DoingType_Skill);
	}

	//连续技能服务器自己释放
	if (SkillCompositeType_Continue == m_pSkillModel->stCompositeData.uType && !bBreak)
	{
		_stPoint2F stTargetPoint(getTargetPoint());
		if (m_pSkillModel->stCompositeData.uNextSkillID > 0)
		{
			//如果当前技能是预警技能,那么下一个技能要重新定义方向  而且是冲撞技能
			_stSkillModel* pNextModel = g_clSkillData.findSkill(m_pSkillModel->stCompositeData.uNextSkillID, m_pSkillModel->uLevel);
			if (m_pSkillModel->nRangeWarnID && m_pSkillModel->nRangeWarnType && pNextModel && pNextModel->findParam(SkillSpareParamType_Dash))
			{

				//更新实时坐标
				CGameSceneCell* pCell = m_pOwner->getSceneCell();
				if (pCell)
				{
					CSceneUnit* pUnit = pCell->findObject(m_pOwner->getSkilling().getCurentTargetUnitId());
					if (pUnit != NULL)
					{
						stTargetPoint = pUnit->getPosition();
					}
				}
				//更新朝向	
				float32 fDir	= get2PiRadian(m_pOwner->getPosition(), stTargetPoint);
				m_pOwner->setDirection(fDir);

				//检查障碍物
				_stPoint2F _stRealTargetPosition;
				if (pCell && !pCell->getTargetPoint(m_pOwner->getPosition(), stTargetPoint, _stRealTargetPosition))
				{
					stTargetPoint = _stRealTargetPosition;
					//需要向ai报告，技能撞墙了
					m_pOwner->callbackUseSkillFailedByBlock(m_pSkillModel->stCompositeData.uNextSkillID);
				}
			}
			m_pOwner->getGameSkillMgr().useSkill(m_pSkillModel->stCompositeData.uNextSkillID,m_pSkillModel->uLevel,m_pOwner->getDirection(),getCurentTargetUnitId(),stTargetPoint);
			return ;
		}
	}

	//连击
	if (SkillCompositeType_Combat == m_pSkillModel->stCompositeData.uType) 
	{
		m_uNextCombatSkill = m_pSkillModel->stCompositeData.uNextSkillID;
		m_uLastCombatTick = m_uNextCombatSkill ? getSystemTick64():0;
	}

	//父子技能 最后一段删除
	if (SkillCompositeType_Child == m_pSkillModel->stCompositeData.uType)
	{
		if (!m_pSkillModel->stCompositeData.uNextSkillID)
		{
			m_mapChildSkill.erase(m_pSkillModel->uCompositeBaseSkillID);
		}
		else
		{
			_stCompositeInfo* pInfo = getChildSkill(m_pSkillModel->uCompositeBaseSkillID);
			if (pInfo )
			{
				pInfo->m_uNextSkill = m_pSkillModel->stCompositeData.uNextSkillID;
				pInfo->m_uExpireTick = getSystemTick64() + m_pSkillModel->stCompositeData.uAtkPartCoolTicket;
				pInfo->m_uBaseSkill = m_pSkillModel->uCompositeBaseSkillID;
			}
			else
			{
				_stCompositeInfo& stInfo = m_mapChildSkill[m_pSkillModel->uCompositeBaseSkillID];
				stInfo.m_uNextSkill = m_pSkillModel->stCompositeData.uNextSkillID;
				stInfo.m_uExpireTick = getSystemTick64() + m_pSkillModel->stCompositeData.uAtkPartCoolTicket;
				stInfo.m_uBaseSkill = m_pSkillModel->uCompositeBaseSkillID;
			}
		}
	}

	///怪物散开规则 自动偏移
	do 
	{
		if (m_pOwner->asMonster() && !m_setGatherTarget.empty())
		{
			ITER_SET_U32 _pos =  m_setGatherTarget.begin();
			if (*_pos == m_pOwner->getUnitID())
				break;

			CSceneUnit* pOffsetTarget = m_pOwner->findCellObject(*_pos);
			if (!pOffsetTarget)
				break;

			MAP_GAME_UNIT	mapObject;
			float32 fAngle = get2PiRadian(pOffsetTarget->getPosition(),m_pOwner->getPosition());
			float32 fDistance = getDistance(pOffsetTarget->getPosition(),m_pOwner->getPosition());
			float32 fModelRadius = m_pOwner->getSkillModelRadius() > 0.1f ? m_pOwner->getSkillModelRadius() : 0.1f;
			float32 fTargetModelRadius = pOffsetTarget->getSkillModelRadius() > 0.1f ? pOffsetTarget->getSkillModelRadius() : 0.1f;

			//靠的太近 怪物自动沿着连线拉开距离
			if (pOffsetTarget->asPlayer(),fDistance < fModelRadius + fTargetModelRadius)
			{
				float32 fNewDistance = fModelRadius + fTargetModelRadius + 0.1f;
				_stPoint2F stNewPoint = pOffsetTarget->getPosition();
				stNewPoint.x += fNewDistance * ::sin(fAngle);
				stNewPoint.y += fNewDistance * ::cos(fAngle);
				//更新实时坐标
				CGameSceneCell* pCell = m_pOwner->getSceneCell();
				if (pCell && m_pOwner->canMove() && pCell->canMove(stNewPoint.x,stNewPoint.y))
				{
					m_pOwner->setPosition(stNewPoint,SetPositionReason_Move,true);
				}
			}
			else
			{
				m_pOwner->gatherSceneObject(m_pOwner->getPosition(),fModelRadius,mapObject,_BIT32(GameObject_Monster));

				float32 fOffsetRadian = fModelRadius * 3/fDistance;
				MAP_GAME_UNIT	mapTarObject;
				m_pOwner->gatherSceneObject(pOffsetTarget->getPosition(),fDistance,mapTarObject,_BIT32(GameObject_Monster));
				if (mapObject.size() > ceil(mapTarObject.size() * fOffsetRadian / M_2PI) && fDistance > FLOAT_EPSINON)
				{
					_stPoint2F stNewPoint = pOffsetTarget->getPosition();
					float32	fOffsetNewRadian= get2PiRadian(fOffsetRadian + fAngle);
					stNewPoint.x += fDistance * ::sin(fOffsetNewRadian);
					stNewPoint.y += fDistance * ::cos(fOffsetNewRadian);

					//更新实时坐标
					CGameSceneCell* pCell = m_pOwner->getSceneCell();
					if (pCell && m_pOwner->canMove() && pCell->canMove(stNewPoint.x,stNewPoint.y))
						m_pOwner->setPosition(stNewPoint,SetPositionReason_Move,true);
				}
			}
		}
	} while (false);


	if (!bBreak)
		m_pOwner->getGameSkillMgr().processNextCmd();
}

//-------------------------------------------------------------
//------------------------------ 
bool CSkilling::useSkillAsk(CSceneUnit*	pOwner, const _stSkillModel* pSkillModel, uint32 uTarUnitID, _stPoint2F&stTargetPoint, uint8& uCode)
{
	if (!pOwner || !pSkillModel || !canUse(pOwner,pSkillModel,uCode) || !m_pOwner->getSceneCell())
		return false;

	//无目标不放技能
	if (pSkillModel->check_mark(SkillMark_NoTarNoSkill))
	{
		if (!uTarUnitID)
			return false;
	}

	uint32 uOldTarUnitID = uTarUnitID;
	float32 fNewDir = pOwner->getDirection();
	CSceneUnit* pTarget = m_pOwner->findCellObject(uTarUnitID);
	if (pTarget)
		fNewDir = get2PiRadian(pOwner->getPosition(),pTarget->getPosition());

	//修正目标
	switch (pSkillModel->uDirStrategy)
	{
	case SkillDirStrategy_NoDir:			//无方向，原地施放
	case SkillDirStrategy_ButtonDir:		//技能按钮指定方向
	case SkillDirStrategy_PoleDir: 			//摇杆指定方向
		{
			stTargetPoint = pOwner->getPosition();
			uTarUnitID = 0;
		}
		break;
	case SkillDirStrategy_TarDir:			//按目标确定方向
	case SkillDirStrategy_ApoointTargetDir:
		{
			stTargetPoint = pOwner->getPosition();
			pOwner->setDirection(fNewDir);
			if (!pSkillModel->check_mark(SkillMark_Charge) && !pSkillModel->check_mark(SkillMark_Sing))
			{
				uTarUnitID = 0;
			}
		}
		break;
	case SkillDirStrategy_ReleasePos:		//技能按钮指定射程位置
		{
			stTargetPoint = pOwner->getPosition();
			uTarUnitID = 0;
		}
		break;
	case SkillDirStrategy_ButtonPos: 		//技能按钮指定位置
	case SkillDirStrategy_PoleReleasePos:	//摇杆指定射程位置（翻滚）
	case SkillDirStrategy_DevelopPos:		//程序设置指定位置
		{
			uTarUnitID = 0;
		}
		break;
	case SkillDirStrategy_TargetPos:		//按目标确定位置（如果没有目标取射程距离）
	case SkillDirStrategy_ApoointTargetPos:
		{
			//最大射程
			_stPoint2F stMaxTargetPoint(stTargetPoint);
			if (pOwner->getSceneCell())
				pOwner->getSceneCell()->getTargetPoint(m_pOwner->getPosition(), m_pOwner->getDirection(), (float32)pSkillModel->uReleaseRange, stMaxTargetPoint);

			stTargetPoint = pTarget ? pTarget->getPosition() : pOwner->getPosition();
 			if (pTarget)
 			{
 				stTargetPoint = pTarget->getPosition();
				if (pOwner->getSceneCell())
					pOwner->getSceneCell()->getTargetPoint(m_pOwner->getPosition(),pTarget->getPosition(),stTargetPoint);
 			}
 			else
			{
				stTargetPoint = stMaxTargetPoint;
			}
		}
		break;
	case SkillDirStrategy_Target:
		{
			//目标不存在 返回
			if (!pTarget)
				return false;

			stTargetPoint = pTarget ? pTarget->getPosition() : pOwner->getPosition();
			stTargetPoint = pTarget->getPosition();
			float32 fNewDir = ::get2PiRadian(m_pOwner->getPosition(),pTarget->getPosition());
			m_pOwner->setDirection(fNewDir);
		}
		break;
	default:
		break;
	}

	//冲撞修正目标点
	if (pSkillModel->findParam(SkillSpareParamType_Dash))
	{
		stTargetPoint = pOwner->getPosition();
		//最大射程
		if (pOwner->getSceneCell())
			pOwner->getSceneCell()->getTargetPoint(m_pOwner->getPosition(), m_pOwner->getDirection(), (float32)pSkillModel->uReleaseRange, stTargetPoint);
	}

	//使用子技能位置 需要修正目标点
	if (pSkillModel->findParam(SkillSpareParamType_InheritTargetPos))
	{
		if (1 == pSkillModel->stCompositeData.uAtkPart)
		{
			m_stOldPosition = pOwner->getPosition();
		}
		else
		{
			stTargetPoint = m_stOldPosition;
			m_stOldPosition.zero();
		}
	}

	//特殊锁定目标的
	if (pSkillModel->check_mark(SkillMark_SingLockTarget))
	{
		uTarUnitID = uOldTarUnitID;
	}

	return useSkillInnerAsk(pOwner,pSkillModel,uTarUnitID,stTargetPoint,uCode);
}
//-------------------------------------------------------------
//------------------------------ 
bool CSkilling::useSkillInnerAsk(CSceneUnit* pOwner, const _stSkillModel* pSkillModel, uint32 uTarUnitID, _stPoint2F&stTargetPoint, uint8& uCode)
{
	if (pSkillModel->check_mark(SkillMark_ChargeEnableMove)
		|| pSkillModel->check_mark(SkillMark_SkillEnableMove)
		|| pSkillModel->findParam(SkillSpareParamType_Dash)
		|| pOwner->asMonster() != NULL || pOwner->asTrap() != NULL)
	{
	}
	else
	{
		//不能移动释放的技能,后台要主动停住.
		pOwner->endMove();
	}

	//无吟唱无动作技能直接释放
	bool bSinging = pSkillModel->check_mark(SkillMark_Charge) || pSkillModel->check_mark(SkillMark_Sing);
	if (!bSinging && SkillBehavior_NoAnim == pSkillModel->uBehavior)
	{
		setTargetPoint(stTargetPoint);
		bool bRet = CSkilling::useNoAnimSkill(m_pOwner,pSkillModel,uTarUnitID,stTargetPoint);
		if (bRet)
		{
			if (pSkillModel->uCoolTicket)
				m_pOwner->getCooling().addCooling(CoolingType_Skill,pSkillModel->uSkillID,pSkillModel->uCoolTicket);
		}
		return bRet;
	}

	initialize(pOwner,pSkillModel);

	//下马
	if (pOwner->asPlayer())
	{
		//uint8 uCode = 0;
		pOwner->asPlayer()->mount(false);
	}

	//吟唱蓄力技能 先吟唱蓄力
	if (bSinging && pOwner->getSinging().getSingState() == SingState_Null)
		return singing(uTarUnitID,stTargetPoint,uCode);

	//判断吟唱蓄力是否已经完成
	if (bSinging && (pOwner->getSinging().getSingState() != SingState_End))
	{
		uCode = R_ResultSkill_Singing;
		return false;
	}

	//消耗
	if (!useConsume(false,uCode))
		return false;

	setTargetPoint(stTargetPoint);
	return readyUseSkill(uTarUnitID,uCode);
}

//-------------------------------------------------------------
//------------------------------ 
bool CSkilling::breakSkillAsk(uint32 uSkillID,bool bForce,uint8& uCode)
{
	if (!canOperate() /*|| m_pSkillModel->uSkillID != uSkillID*/)
		return false;

	//吟唱是否能被打断
	if (!bForce && m_pSkillModel->check_mark(SkillMark_NoBeBreak))
		return false;

	//吟唱中
	if (SingState_Ing == m_pOwner->getSinging().getSingState())
	{
		m_pOwner->getSinging().breakSinging(true);
	}
	else
	{
		if (!m_pOwner->haveState(StateType_Skiling))
			return false;

		//打断广播
		CGameSkillMgr::send_SkillBreak_Ack(m_pSkillModel->uSkillID,m_uCurStage,R_ResultSkill_Succeed,true,m_pOwner);

		onDoneSkill(true);
	
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool CSkilling::useChargeAsk(uint32 uSkillID,uint8& uCode)
{
	if (!canOperate() || m_pSkillModel->uSkillID != uSkillID || !m_pSkillModel->check_mark(SkillMark_Charge))
		return false;

	//是否在蓄力中
	if (SingState_Ing != m_pOwner->getSinging().getSingState())
		return false;

	//完成蓄力
	m_pOwner->getSinging().doneSinging();
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool CSkilling::canSinging(uint32 uTarUnitID,uint8& uCode)
{
	if (!m_pSkillModel)
		return false;

	//判断激活
	if (SkillType_Active == m_pSkillModel->uType || SkillType_MonsterCollletion == m_pSkillModel->uType)
	{
		CSceneUnit* pTarget = m_pOwner->findCellObject(uTarUnitID);
		if (!pTarget)
			return false;

		CSceneMonster* pMonster = pTarget->asMonster();
		if (!pMonster)
			return false;

		//有人在激活 不能用技能
		if (pMonster->getActivePlayer())
		{
			return false;
		}
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool CSkilling::singing(uint32 uTarUnitID,_stPoint2F&stTargetPoint,uint8& uCode)
{
	if (!canOperate())
		return false;

	if (!canSinging(uTarUnitID,uCode))
	{
		return false;
	}
	//检查消耗
	if (!useConsume(true,uCode))
		return true;

	//开始吟唱
	uint32 uTime = 0;
	if (m_pSkillModel->check_mark(SkillMark_Charge))
	{
		uTime = getSkillTimeByType(m_pSkillModel,SkillTimeType_Charge);
	}
	else
	{
		uTime = getSkillTimeByType(m_pSkillModel,SkillTimeType_Continue);
	}
	this->initSingObject(m_pSkillModel->uSkillID,m_pSkillModel->uLevel,uTarUnitID,uTime,&stTargetPoint);
	m_pOwner->getSinging().beginSinging(this);
	return true;
}

//-------------------------------------------------------------
//------------------------------ 时间开始
void CSkilling::beginTime()
{
	if (!canOperate() || !m_pSkillAnimModel || m_pSkillModel == NULL || m_pOwner == NULL)
		return;

	m_uStartAnimTick = getSystemTick64();
	//需要处理持续技能
	uint32 uMaxTime = MAX(m_pSkillModel->uContinueTime,m_pSkillAnimModel->uTotalTick);
	m_uEndAnimTick	= m_uStartAnimTick + uMaxTime;
}

//-------------------------------------------------------------
//------------------------------ 准备施法
bool CSkilling::readyUseSkill(uint32 uTarUnitID,uint8& uCode)
{
	if (!canOperate())
		return false;

	m_uTarUnitID = uTarUnitID;
	_stPoint2F stSkillStart(m_pOwner->getPosition());

	//特殊技能锁定目标 更改方向
	if (m_pSkillModel->check_mark(SkillMark_SingLockTarget))
	{
		CSceneUnit* pTargetUnit = m_pOwner->findCellObject(uTarUnitID);
		if (pTargetUnit)
		{
			float32 fNewDir = ::get2PiRadian(m_pOwner->getPosition(),pTargetUnit->getPosition());
			m_pOwner->setDirection(fNewDir);
		}
	}

	//回调
	m_pOwner->callbackUseSkill(m_pSkillModel,uTarUnitID);

	//设置方向和位置
	if (m_pSkillModel->findParam(SkillSpareParamType_InheritTargetPos))
		m_pOwner->setPosition(getTargetPoint(),SetPositionReason_Skill,false);

	//无动作，有效果直接伤害
	if (!m_pSkillModel->vecEffectModel.empty() && !m_pSkillModel->pAnimModel)
		return useNoAnimSkill(m_pOwner,m_pSkillModel,uTarUnitID,getTargetPoint());

	//动作只会有一次
	CHECKF(NULL == m_pSkillAnimModel);

	//进入行为
	m_pOwner->addState(StateType_Skiling);
	m_pSkillAnimModel = m_pSkillModel->pAnimModel;
	m_uCurStage++;

	//加状态
	const _stSkillSpareParam* pSkillSpareParam = m_pSkillModel->findParam(SkillSpareParamType_AddState);
	if (pSkillSpareParam && pSkillSpareParam->vecParams.size() > 0)
		m_pOwner->addState((_emGameStateType)(uint32)pSkillSpareParam->vecParams[0]);

	//开始时间
	beginTime();

	m_clSkillAnim.initialize(m_pOwner,m_uStartAnimTick,m_pSkillModel,m_pSkillAnimModel);
	m_clSkillAnim.start();
	//广播
	CGameSkillMgr::send_UseSkill_Ack(m_pSkillModel->uSkillID,m_pSkillModel->uLevel,m_uCurStage,uTarUnitID,getTargetPoint(),R_ResultSkill_Succeed,true,m_pOwner,&stSkillStart);
	
	//技能完成开始CD (非持续技能) 组合技能在最后加组合技能的第一个技能CD
	if ((!m_pOwner->check_mark(SceneUnitMark_IgnoreSkillCD) && !m_pSkillModel->check_mark(SkillMark_Continue) && !m_pSkillModel->stCompositeData.uType)
		|| (m_pSkillModel->stCompositeData.uType && !m_pSkillModel->stCompositeData.uNextSkillID))
	{
		uint32 uCoolTime = 0;
		uint32 uTmpSkillID = m_pSkillModel->uSkillID;
		uint32 uGroupCoolTicket = m_pSkillModel->uGroupCoolTicket;
		if (m_pSkillModel->stCompositeData.uType)
		{
			uTmpSkillID = m_pSkillModel->uCompositeBaseSkillID;
			const _stSkillModel* pCompoSkillModel =g_clSkillData.findSkill(m_pSkillModel->uCompositeBaseSkillID,m_pSkillModel->uLevel);
			if (pCompoSkillModel)
			{
				uCoolTime = getSkillTimeByType(pCompoSkillModel,SkillTimeType_CD);
				uGroupCoolTicket = pCompoSkillModel->uGroupCoolTicket;
			}
		}
		else
		{
			uCoolTime = getSkillTimeByType(m_pSkillModel,SkillTimeType_CD);
		}
		m_pOwner->getCooling().addCooling(CoolingType_Skill,uTmpSkillID,uCoolTime,SKILL_GOOLING_GROUP_ID,uGroupCoolTicket);
	}
	
// 	if (m_pOwner->asPlayer())
// 		CLog::error("开始施法 当前时间:%lld,技能ID:%d",m_uStartAnimTick,m_pSkillModel->uSkillID);
	m_pOwner->energyUseSkill(m_pSkillModel);
	return true;
}

//-------------------------------------------------------------
//------------------------------ 无动作技能
bool	CSkilling::useNoAnimSkill(CSceneUnit* pOwner,const _stSkillModel* pSkillModel,uint32 uTarUnitID,_stPoint2F&stTargetPoint)
{
	if (!pOwner || !pSkillModel || SkillBehavior_HaveAnim == pSkillModel->uBehavior || pSkillModel->vecEffectModel.empty())
		return false;

	//广播
	uint8 uLevel = pOwner->getGameSkillMgr().getSkillLevel(pSkillModel->uSkillID);
	CGameSkillMgr::send_UseSkill_Ack(pSkillModel->uSkillID,uLevel,1,uTarUnitID,stTargetPoint,R_ResultSkill_Succeed,true,pOwner);
	CSceneUnit* pTarget = pOwner->findCellObject(uTarUnitID);
	for (uint32 i = 0; i < pSkillModel->vecEffectModel.size(); ++i)
	{
		const _stSkillEffectModel* pEffect = pSkillModel->vecEffectModel[i];
		if (!pEffect)
			return false;

		//伤害
		if (!CSkillAnim::beginDamage(pOwner,pTarget,stTargetPoint,pSkillModel,pEffect))
			return false;
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------ 采集技能
bool CSkilling::useColletionSkill(uint8& uCode)
{
	uCode = R_ResultSkill_Fail;
	CScenePlayer* pPlayer = m_pOwner->asPlayer();
	if (!pPlayer) 
		return false;

	CSceneUnit* pUnit = m_pOwner->findCellObject(getSingTriggerID());
	if (!pUnit) 
		return false;

	//CPlayerColletion *pPlayerColleton = pPlayer->getPlayerColletion();
	//if (pPlayerColleton)
	//{
	//	if(!pPlayerColleton->colletionMonster(pUnit, uCode))
	//	{
	//		return false;
	//	}
	//}

	return true;
}
//-------------------------------------------------------------
//------------------------------ 抓捕技能
bool CSkilling::arrestSkill(uint8& uCode)
{
	uCode = R_ResultSkill_Fail;

	CGameSceneCell* pGameCell = m_pOwner->getSceneCell();
	if (!pGameCell)
		 return false;

	CSceneUnit* pTargeUnit = m_pOwner->findCellObject(getCurentTargetUnitId());
	if (!pTargeUnit || !pTargeUnit->asMonster() || pTargeUnit->asMonster()->getMonsterType() != MonsterType_Capture)
	{
		uCode = R_ResultSkill_ArrestNoPlayer;
		return false;
	}

	pGameCell->arrestmonster(pTargeUnit,m_pOwner->asPlayer());
	return true;
}
//-------------------------------------------------------------
//------------------------------ 激活技能
bool CSkilling::activeSkill(uint8& uCode)
{
	uCode = R_ResultSkill_Fail;

	CGameSceneCell* pGameCell = m_pOwner->getSceneCell();
	if (!pGameCell)
		return false;

	CSceneUnit* pTargeUnit = m_pOwner->findCellObject(getSingTriggerID());
	CSceneMonster* pMonster = pTargeUnit ? pTargeUnit->asMonster() :NULL;
	if (!pMonster)
	{
		uCode = R_ResultSkill_NoActive;
		return false;
	}
	uint8 uType = pMonster->getMonsterType();
	if (uType != MonsterType_Active)
	{
		uCode = R_ResultSkill_NoActive;
		return false;
	}

	pMonster->setActivePlayer(NULL);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 刺客暗杀技能
bool CSkilling::useRewardSkill(uint8& uCode)
{
	uCode = R_ResultSkill_Fail;

	CGameSceneCell* pGameCell = m_pOwner->getSceneCell();
	if (!pGameCell)
		return false;

	CSceneUnit* pTargeUnit = m_pOwner->findCellObject(getSingTriggerID());
	CSceneMonster* pMonster = pTargeUnit ? pTargeUnit->asMonster() :NULL;
	if (!pMonster)
	{
		uCode = R_ResultSkill_NoThug;
		return false;
	}
	if (pMonster->getMonsterType() != MonsterType_RewardThugElite)
	{
		uCode = R_ResultSkill_NoThug;
		return false;
	}

	//暗杀
	pGameCell->thugmonster(pTargeUnit,m_pOwner->asPlayer());

	return true;
}
//-------------------------------------------------------------
//------------------------------ 杀人回调
void CSkilling::callbackKillTarget		(CSceneUnit* pTarget)
{
	if (!m_pSkillModel || !pTarget)
		return;

	//持续性锁定目标技能 需要中断
	if (m_pSkillModel->check_mark(SkillMark_Continue) && m_pSkillModel->check_mark(SkillMark_SingLockTarget) )
	{
		uint8 uCode = 0;
		breakSkillAsk(m_pSkillModel->uSkillID,true,uCode);
	}
}
