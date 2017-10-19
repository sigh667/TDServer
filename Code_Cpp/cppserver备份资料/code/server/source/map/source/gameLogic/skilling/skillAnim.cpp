/*----------------- skillAnim.cpp
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/1/27 19:58:49
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "gameLogic/skilling/skillAnim.h"
#include "basic/timeFunction.h"
#include "protocol/MsgResult.pb.h"
#include "protocol/GameDefine.pb.h"
#include "gameLogic/sceneObject/sceneUnit.h"
#include "cfg_data/trapData/trapData.h"
#include "gameScene/gameSceneCell.h"
#include "cfg_data/monsterData/monsterData.h"
#include "cfg_data/constantData/constantData.h"
#include "log/log.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
CSkillAnim::CSkillAnim()
{
	initialize(NULL,0,NULL,NULL);
}
//-------------------------------------------------------------
//------------------------------ 
void	CSkillAnim::initialize(CSceneUnit* pOwner,uint64 uStartTick,const _stSkillModel* pSkillModel,const _stSkillAnimModel* pSkillAnimModel)
{
	m_pOwner		= pOwner;
	m_uStartAnimTick= uStartTick;
	m_uEndAnimTick	= 0;
	if (pSkillModel)
		m_uEndAnimTick = uStartTick + pSkillModel->uContinueTime;
	m_stAttEffect.init();
	m_uCurEffect	= 0;
	m_pSkillModel = pSkillModel;
	m_pSkillAnimModel = pSkillAnimModel;
	m_bBreak = false;
}

//-------------------------------------------------------------
//------------------------------ 
void	CSkillAnim::start()
{
	if (!canOperate()  )
		return;

	if ( m_pSkillAnimModel->vecAttEffect.empty() )
	{
		end();
		return ;
	}

	m_uCurEffect++;
	if (m_uCurEffect <= m_pSkillAnimModel->vecAttEffect.size())
	{
		m_stAttEffect = m_pSkillAnimModel->vecAttEffect[m_uCurEffect - 1];
	}
	else
	{
		//持续技能需要重新播放动作
		if (m_pSkillModel->check_mark(SkillMark_Continue) && (!m_pSkillModel->uContinueTime || m_uEndAnimTick >= getSystemTick64()))
		{
			m_uCurEffect = 1;//直接执行第一阶段
			m_stAttEffect = m_pSkillAnimModel->vecAttEffect[0];
			m_uStartAnimTick += m_pSkillAnimModel->uTotalTick;
			//广播
			uint8 uLevel = m_pOwner->getGameSkillMgr().getSkillLevel(m_pSkillModel->uSkillID);
			CGameSkillMgr::send_UseSkill_Ack(m_pSkillModel->uSkillID,uLevel,1,0,m_pOwner->getSkilling().getTargetPoint(),R_ResultSkill_Succeed,true,m_pOwner);
		}
		else
		{
			end();
		}
	}
}

//-------------------------------------------------------------
//------------------------------ 结束
void	CSkillAnim::end()
{
	m_uCurEffect = 0;
	m_uStartAnimTick = 0;
	m_stAttEffect.init();
}

//-------------------------------------------------------------
//------------------------------ 
bool	CSkillAnim::process		(uint64 uTick)
{
	processAttack(uTick);
	return true;
}

//-------------------------------------------------------------
//------------------------------ 攻击帧推进
bool	CSkillAnim::processAttack	(uint64 uTick)
{
	if (!canOperate() || !m_stAttEffect.pEffectModel || !m_pOwner)
		return false;

	if (!m_uStartAnimTick || m_uStartAnimTick > uTick || m_uStartAnimTick +  m_stAttEffect.uTick > uTick)
		return true;

	//if (m_pOwner->asPlayer())
		//CLog::error("作用帧:%d,开始时间:%lld,当前时间:%lld,技能ID:%d,效果ID:%d",m_uCurEffect,m_uStartAnimTick,uTick,m_pSkillModel->uSkillID,m_stAttEffect.pEffectModel->uEffectID);

	//需要去到最新的坐标
	_stPoint2F target = m_pOwner->getSkilling().getTargetPoint();
	//位移
	{
		//配置的位移
		if (!isFloatEqual(m_stAttEffect.fDistance,0.0f))
			m_pOwner->getUnitTargetPoint(m_stAttEffect.fDir + m_pOwner->getDirection(),m_stAttEffect.fDistance,target);

		//冲撞的位移
		const _stSkillSpareParam* pParam = m_pSkillModel->findParam(SkillSpareParamType_Dash);
		if (pParam && pParam->vecParams.size() > 0 && m_pOwner->getPosition() != m_pOwner->getSkilling().getTargetPoint())
		{
			uint32 uLastTick = m_uCurEffect > 1 ? m_pSkillAnimModel->vecAttEffect[m_uCurEffect -2].uTick : 0;
			float32 fTmpDis = (m_stAttEffect.uTick - uLastTick) * pParam->vecParams[0] / 1000.0f;
			float32 fMaxDis = getDistance(m_pOwner->getPosition(),m_pOwner->getSkilling().getTargetPoint());
			if (fTmpDis < fMaxDis)
			{
				m_pOwner->getUnitTargetPoint(m_pOwner->getDirection(),fTmpDis,target);
			}
			else
			{
				target = m_pOwner->getSkilling().getTargetPoint();

				//到最远距离技能停止
				m_bBreak = true;
			}
		}
	}

	//移动中不设置位置 有技能边移动边释放
	if(!m_pOwner->isMoving() && m_pSkillModel->uDirStrategy !=SkillDirStrategy_Target)
	{
		m_pOwner->setPosition(target,SetPositionReason_Skill,false);
	}
	//if (m_pOwner->asPlayer())
		//CLog::error("processAttack position(%f,%f),dir(%f)",m_pOwner->getPosition().x,m_pOwner->getPosition().y,m_pOwner->getDirection());

	float32 fMoveDis = getDistance(target,m_pOwner->getPosition());
	if (!m_stAttEffect.uUseRange)
		fMoveDis = 0;

	//施法位置 默认取客户端发的目标位置 冲撞默认取自身位置
	_stPoint2F skillTarget = m_pOwner->getPosition();

	//伤害
	CSceneUnit* pTarget = m_pOwner->findCellObject(m_pOwner->getSkilling().getCurentTargetUnitId());
	if (pTarget)
		skillTarget = pTarget->getPosition();

	beginDamage(m_pOwner,pTarget,skillTarget,m_pSkillModel,m_stAttEffect.pEffectModel,fMoveDis);

	//计算下一帧
	start();
	return true;
}

//-------------------------------------------------------------
//------------------------------ 能够选取目标
bool	CSkillAnim::canChooseTo	(CSceneUnit* pOwner,CSceneUnit*pTarget,const _stSkillModel* pModel)
{
	if (!pOwner || !pTarget || !pModel)
		return false;

	if (!pOwner->canUseSkill(pTarget))
		return false;

	if (!pTarget->canByUseSkill(pOwner) || pTarget->isDead())
		return false;

	if(pModel->uTagetCategory != TargetCategory_All)
	{
		uint8 uRelation = pOwner->getRelation(pTarget);
		if (TargetCategory_FriendAndSelf == pModel->uTagetCategory)
		{
			if (uRelation != TargetCategory_Self && uRelation != TargetCategory_Friend )
			{
				return false;
			}
		}
		else if (uRelation != pModel->uTagetCategory)
		{
			return false;
		}
	}

	CSceneUnit* pTargetOwner = pTarget->getOwner();
	if ( pTargetOwner )
	{
		//-todo
		CScenePlayer* pTargetPlayer = pTargetOwner->asPlayer();
		if (pTargetPlayer&&pTargetPlayer->testRegion(RegionMark_PlayerCntBeHarm))
			return true;
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CSkillAnim::gatherObject(CSceneUnit* pOwner,CSceneUnit*pTarget,const _stPoint2F&stTargetPoint,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel,MAP_GAME_UNIT&mapTarget,float32 fMoveDis)
{
	if (!pOwner || !pSkillEffectModel || !pSkillModel || !pSkillEffectModel->uMaxAffectNum)
		return false;

	CGameScene*pScene = pOwner->getSceneMap();
	if(!pScene)
		return false;

	//对列表排序
	static VECTOR_RANGE_UNIT vecRangeUnit;
	vecRangeUnit.clear();

	if(!CSkillAnim::gatherObject(pOwner,stTargetPoint,pSkillModel,pSkillEffectModel,vecRangeUnit,fMoveDis))
		return false;

	static VECTOR_RANGE_UNIT vecEffectUnit;
	vecEffectUnit.clear();
	for (uint32 i = 0;i < vecRangeUnit.size();++i)
	{
		_stRangeUnit&stUnit = vecRangeUnit[i];
		//客户端给的目标一定要打到
		if(stUnit.pUnit && canChooseTo(pOwner,stUnit.pUnit,pSkillModel))
		{
			if (stUnit.pUnit == pTarget)
			{
				mapTarget.insert_(pTarget->getUnitID(),pTarget);
			}
			else
			{
				vecEffectUnit.push_back(stUnit);
			}
		}
	}

	//选择目标
	switch (pSkillEffectModel->uTargetSelectType)
	{
	case TargetSelectType_Near:
		{
			for (uint32 i = 0; i < vecEffectUnit.size(); ++i)
			{
				if (vecEffectUnit[i].pUnit && mapTarget.size() < pSkillEffectModel->uMaxAffectNum)
					mapTarget.insert_(vecEffectUnit[i].pUnit->getUnitID(),vecEffectUnit[i].pUnit);
			}
		}
		break;
	case TargetSelectType_Rand:
		{
			VECTOR_RANGE_UNIT vecOut;
			getRandVecDiff(vecEffectUnit,vecOut,pSkillEffectModel->uMaxAffectNum);
			for (uint32 i = 0; i < vecOut.size(); ++i)
			{
				if (vecOut[i].pUnit && mapTarget.size() < pSkillEffectModel->uMaxAffectNum)
					mapTarget.insert_(vecOut[i].pUnit->getUnitID(),vecOut[i].pUnit);
			}
		}
		break;
	case TargetSelectType_Long:
		{
			for (int32 i = (int32)vecEffectUnit.size() - 1; i >= 0 ; --i)
			{
				if (vecEffectUnit[i].pUnit && mapTarget.size() < pSkillEffectModel->uMaxAffectNum)
					mapTarget.insert_(vecEffectUnit[i].pUnit->getUnitID(),vecEffectUnit[i].pUnit);
			}
		}
		break;
	default:
		break;
	}


	//过滤目标
	if (pSkillModel->check_mark(SkillMark_TargetOnce))
	{
		MAP_GAME_UNIT::iterator _pos = mapTarget.begin();
		for (; _pos != mapTarget.end();)
		{
			if (pOwner->getSkilling().haveChoose(_pos->first))
			{
				mapTarget.erase(_pos++);
			}
			else
			{
				 pOwner->getSkilling().addChooseTarget(_pos->first);
				 ++_pos;
			}
		}
	}

	//存已选目标
	MAP_GAME_UNIT::iterator _pos = mapTarget.begin();
	for (; _pos != mapTarget.end();_pos++)
	{
		pOwner->getSkilling().addGatherTarget(_pos->first);
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CSkillAnim::gatherObject	(CSceneUnit* pOwner,const _stPoint2F&stTargetPoint,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel,VECTOR_RANGE_UNIT&vecRangeUnit,float32 fMoveDis)
{
	if(!pSkillModel || !pSkillEffectModel)
		return false;

	CGameSceneCell* pScene = pOwner->getSceneCell();
	if(!pScene)
		return false;

	//查找攻击范围内的所有对像
	static MAP_GAME_UNIT	mapObject;
	static MAP_GAME_UNIT	mapUseObject;
	static VECTOR_GAME_UNIT vecObject;
	mapObject.clear();
	vecObject.clear();
	mapUseObject.clear();

	float fRadius = 0;
	_stSkillEffectModel::MapSkillRangeData::const_iterator _pos = pSkillEffectModel->mapRangeData.begin();
	for (; _pos != pSkillEffectModel->mapRangeData.end(); ++_pos)
	{
		const _stSkillRangeData& stData = _pos->second;
		fRadius = MAX(fRadius,stData.fLength + stData.fOffset);
		if (stData.uRangeType == RangeType_Rect)
			fRadius = MAX(fRadius,stData.fWidth);
	}
	fRadius = MAX(fRadius,fMoveDis) + pScene->getMaxModelRadius();
	//搜集最大的圆范围目标
	pOwner->gatherSceneObject(stTargetPoint,fRadius,mapObject,g_uBattleObject);

	//搜集自身模型圆的范围
	pOwner->gatherObjectRound(stTargetPoint,pOwner->getModelRadius(),mapObject,mapUseObject);

	//搜集位移矩形的范围 方向相反
	if (fMoveDis)
		pOwner->gatherObjectLine(stTargetPoint,pOwner->getDirection() + M_PI_F,fMoveDis,pOwner->getModelRadius(),mapObject,mapUseObject);

	_pos = pSkillEffectModel->mapRangeData.begin();
	for (; _pos != pSkillEffectModel->mapRangeData.end(); ++_pos)
	{
		const _stSkillRangeData& stData = _pos->second;
		float32 fOffsetDir = pOwner->getDirection() + getAngleToRadian(stData.fOffsetAngle);
		//计算偏移目标 烈炎爆破
		_stPoint2F stPoint;
		getTargetPoint(stTargetPoint,fOffsetDir,stData.fOffset,stPoint);
		//线 矩形
		if(stData.uRangeType == RangeType_Rect)
			pOwner->gatherObjectLine(stPoint,fOffsetDir,stData.fLength,stData.fWidth,mapObject,mapUseObject);
		//圆 扇形
		if(stData.uRangeType == RangeType_Round)
			pOwner->gatherObjectArc(stPoint,fOffsetDir,stData.fLength,stData.fWidth,mapObject,mapUseObject);
		//圆 环形
		if(stData.uRangeType == RangeType_Ring)
			pOwner->gatherObjectRound(stPoint,stData.fLength,mapObject,mapUseObject);
	}

	MAP_GAME_UNIT::iterator iterUseObject = mapUseObject.begin();
	for (; iterUseObject != mapUseObject.end(); ++iterUseObject)
	{
		CSceneUnit* pUnit = iterUseObject->second;
		if(!pUnit)
			continue;
		
		//外环
		const _stSkillRangeData* pRangeData = pSkillEffectModel->mapRangeData.find_(RangeType_Ring);
		float32 fDis = getDistance(stTargetPoint,pUnit->getPosition()) - pUnit->getSkillModelRadius();
		if (fDis < 0.0f)
			fDis = fDis * (-1);
		if (pRangeData && pRangeData->fWidth && fDis >= pRangeData->fWidth)
			pUnit->update_mark(SceneUnitMark_OutSideDamage);

		//加入多人位面判断
		if (!pUnit->haveState(StateType_Plane))
		{
			vecRangeUnit.push_back(_stRangeUnit(pUnit,fDis));
		}
	}

	//排序
	if(!vecRangeUnit.empty())
		qsort(&vecRangeUnit[0],vecRangeUnit.size(),sizeof(_stRangeUnit),CGhostObject::_rangeUnit_distance);

	return true;
}
//-------------------------------------------------------------
//------------------------------ 延时攻击
bool	CSkillAnim::delayDamage	(CSceneUnit* pOwner,CSceneUnit* pTarget,const _stPoint2F&stTargetPoint,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel)
{
	if (!pOwner || !pTarget || !pSkillModel || !pSkillEffectModel)
		return false;

	MAP_GAME_UNIT mapTarget;
	mapTarget.insert_(pTarget->getUnitID(),pTarget);
	return realDamage(pOwner,mapTarget,stTargetPoint,pSkillModel,pSkillEffectModel);
}
//-------------------------------------------------------------
//------------------------------ 开始攻击
bool	CSkillAnim::beginDamage(CSceneUnit* pOwner,CSceneUnit* pTarget,const _stPoint2F&stTargetPoint,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel,float32 fMoveDis/* = 0*/)
{
	if (!pOwner || !pSkillEffectModel || !pSkillModel)
		return false;

	MAP_GAME_UNIT mapTarget;
	if (!CSkillAnim::gatherObject(pOwner,pTarget,stTargetPoint,pSkillModel,pSkillEffectModel,mapTarget,fMoveDis))
		return false;

	//无目标中断技能
	if (pSkillModel->check_mark(SkillMark_NoTargeStop) && mapTarget.empty())
	{
		pOwner->breakUseSkill();
		return true;
	}
/* 	if (pOwner->asPlayer())
 		CLog::error("beginDamage 当前位置(%f,%f),目标位置(%f,%f),目标个数(%d),效果ID(%d)",pOwner->getPosition().x,pOwner->getPosition().y,\
 					stTargetPoint.x,stTargetPoint.y,mapTarget.size(),pSkillEffectModel->uEffectID);*/

	//算延时攻击
	const _stSkillSpareParam* pSpareParam = pSkillModel->findParam(SkillSpareParamType_DelaySkillEffect);
	if (pSpareParam && pSpareParam->vecParams.size() > 0)
	{
		float32 fSpeed = pSpareParam->vecParams[0];
		if (isFloatEqual(fSpeed,0.0f))
			return false;

		//无目标 要给自己算buff
		if (mapTarget.empty())
			return CSkillAnim::skillAllEffect(pOwner,mapTarget,stTargetPoint,pSkillModel,pSkillEffectModel);

		uint16 uTargetPos = 0;//目标的顺序
		MAP_GAME_UNIT::iterator _pos = mapTarget.begin();
		MAP_GAME_UNIT::iterator _end = mapTarget.end();
		for (; _pos != _end; ++_pos)
		{
			CSceneUnit* pTarget = _pos->second;
			if (!pTarget)
				continue;

			uTargetPos++;
			float32 fDis = getDistance(pOwner->getPosition(),pTarget->getPosition());
			uint32 uTick = (uint32)(fDis * 1000 / fSpeed);
			pTarget->byAttack(pOwner,stTargetPoint,pSkillModel,pSkillEffectModel,uTick);
		}	
		//冲撞设置位置
		if (pSkillModel->findParam(SkillSpareParamType_Dash))
		{
			//需要实时位置
			_stPoint2F stP = pOwner->getSkilling().getTargetPoint();
			pOwner->setPosition(stP,SetPositionReason_Skill, false);
		}

		return true;
	}

	//真正伤害
	return realDamage(pOwner,mapTarget,stTargetPoint,pSkillModel,pSkillEffectModel);
}
//-------------------------------------------------------------
//------------------------------ 真正伤害
bool	CSkillAnim::realDamage	(CSceneUnit* pOwner,MAP_GAME_UNIT&mapTarget,const _stPoint2F&stTargetPoint,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel)
{
	if (!pOwner || !pSkillEffectModel || !pSkillModel)
		return false;

	
	uint8 uHitNum = 0;//算命中
	uint16 uTargetPos = 0;//目标的顺序
	MAP_GAME_UNIT::iterator _pos =  mapTarget.begin();
	for (; _pos != mapTarget.end();)
	{
		CSceneUnit* pUnit = _pos->second;
		if (!pUnit)
		{
			++_pos;
			continue;
		}
		uTargetPos++;
		pUnit->delete_mark(SceneUnitMark_SkillMiss);
		if (pOwner->isAttackHit(pUnit,pSkillModel,pSkillEffectModel))
		{
			uHitNum++;
			 ++_pos;
			 pUnit->callbackBeSkillEffect(pOwner,pSkillModel,pSkillEffectModel);		
			 pOwner->callbackSkillHitTarget(pUnit,pSkillModel,pSkillEffectModel);		

		}
		else
		{//闪避通知客户端
			pUnit->update_mark(SceneUnitMark_SkillMiss);
			CGameSkillMgr::send_AttackNotice_Ack(pOwner,pUnit,SkillNoticeType_Miss,false);
			mapTarget.erase(_pos++);
		}
	}

	//命中敌方回调
	if (uHitNum > 0)
		pOwner->callbackSkillHit(pSkillModel,pSkillEffectModel);
	else
		pOwner->callbackSkillNotHit(pSkillModel,pSkillEffectModel);

	//伤害效果
	CSkillAnim::skillAllEffect(pOwner,mapTarget,stTargetPoint,pSkillModel,pSkillEffectModel);

	//技能伤害完成
	skillDamageOver(pOwner,mapTarget,pSkillModel,pSkillEffectModel);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 所有效果
bool CSkillAnim::skillAllEffect(CSceneUnit* pOwner,MAP_GAME_UNIT&mapTarget,const _stPoint2F&stTargetPoint,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel)
{
	if(!pOwner || !pSkillModel || !pSkillEffectModel)
		return false;

	for (uint8 i = 0; i <pSkillEffectModel->vecEffectType.size(); ++i)
	{
		const _stSkillEffectType& stSkillEffectType = pSkillEffectModel->vecEffectType[i];
		if (SkillEffectType_Damage == stSkillEffectType.uEffectType)//算伤害
		{
			skillAttack(pOwner,mapTarget,stTargetPoint,pSkillModel,pSkillEffectModel);
		}
		else if (SkillEffectType_Cure == stSkillEffectType.uEffectType)//治疗
		{
			skillCure(pOwner,mapTarget,stTargetPoint,pSkillModel,pSkillEffectModel);
		}
		else if (SkillEffectType_AddBuff == stSkillEffectType.uEffectType)//加buff
		{
			skillAddBuff(pOwner,mapTarget,pSkillModel,pSkillEffectModel,stSkillEffectType);
		}
		else if (SkillEffectType_AddExpend == stSkillEffectType.uEffectType)//增加消耗值
		{
			skillAddExpend(pOwner,mapTarget,pSkillModel,pSkillEffectModel,stSkillEffectType);
		}
		else if (SkillEffectType_AddObject == stSkillEffectType.uEffectType)//陷阱
		{
			skillAddSceneObject(pOwner,mapTarget,stTargetPoint,pSkillModel,pSkillEffectModel);
		}
		else if (SkillEffectType_ClearUsefulBuff == stSkillEffectType.uEffectType)//清除正面buff
		{
			skillClearBuff(pOwner,mapTarget,pSkillModel,pSkillEffectModel,stSkillEffectType);
		}
		else if (SkillEffectType_ClearHarmfulBuff == stSkillEffectType.uEffectType)//清除负面buff
		{
			skillClearBuff(pOwner,mapTarget,pSkillModel,pSkillEffectModel,stSkillEffectType);
		}
		else if (SkillEffectType_ClearSkillCD == stSkillEffectType.uEffectType)//清除CD
		{
			pOwner->getGameSkillMgr().clearSkillCD(pSkillModel->uSkillID);
		}
		else if (SkillEffectType_Move == stSkillEffectType.uEffectType)//增加位移
		{
			skillAddMove(pOwner,stTargetPoint,pSkillModel,pSkillEffectModel,stSkillEffectType);
		}
		else if (SkillEffectType_AddSkill == stSkillEffectType.uEffectType)//增加技能
		{
			skillUseSkill(pOwner,mapTarget,pSkillModel,pSkillEffectModel,stSkillEffectType);
		}
		else if (SkillEffectType_OutRingAddBuff == stSkillEffectType.uEffectType)//增加技能
		{
			skillOutRingAddBuff(pOwner,mapTarget,pSkillModel,pSkillEffectModel,stSkillEffectType);
		}
		else if (SkillEffectType_ResetBuffTime == stSkillEffectType.uEffectType)//增加技能
		{
			skillResetBuffTime(pOwner,mapTarget,pSkillModel,pSkillEffectModel,stSkillEffectType);
		}
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------ 增加buff
bool	CSkillAnim::skillAddBuff(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel,const _stSkillEffectType& stSkillEffectType)
{
	if(!pOwner || !pSkillModel || !pSkillEffectModel || SkillEffectType_AddBuff != stSkillEffectType.uEffectType)
		return false;

	const _stSkillEffectModel::DMAP_SKILLADDBUFF::_sub_map* pSumap = pSkillEffectModel->dmapAddBuff.find_(stSkillEffectType.uEffectTargetType);
	if (!pSumap)
		return false;

	//给自己加不依赖命中的消耗
	if (EffectTargetType_Self == stSkillEffectType.uEffectTargetType && !stSkillEffectType.bSkillHit)
	{
		_stSkillEffectModel::DMAP_SKILLADDBUFF::_sub_map::const_iterator _pos = pSumap->begin();
		for (; _pos != pSumap->end(); ++_pos)
		{
			const _stSkillAddBuff& stAddBuff = _pos->second;
			if (!stAddBuff.uExpendType || pOwner->addExpend(stAddBuff.uExpendType,stAddBuff.iValue, DoingType_Skill))
				pOwner->getGameBuffMgr().addBuff(stAddBuff.uBuffID,pSkillModel->uLevel,pOwner->getUnitID(),pSkillModel->uSkillID, DoingType_Skill);
		}
		
		return true;
	}

	MAP_GAME_UNIT::iterator _pos =  mapObject.begin();
	uint8 uHitNum = 0;
	for (; _pos != mapObject.end(); ++_pos)
	{
		//CLog::warn("----has target skillAddBuff target  is ");

		CSceneUnit* pUnit = _pos->second;
		if (!pUnit)
			continue;

		if (stSkillEffectType.bSkillHit && pUnit->check_mark(SceneUnitMark_SkillMiss))
			continue;

		uHitNum++;
		//目标直接加
		if (EffectTargetType_Other == stSkillEffectType.uEffectTargetType)
		{
			_stSkillEffectModel::DMAP_SKILLADDBUFF::_sub_map::const_iterator _pos = pSumap->begin();
			for (; _pos != pSumap->end(); ++_pos)
			{
				const _stSkillAddBuff& stAddBuff = _pos->second;
				if (!stAddBuff.uExpendType || pOwner->addExpend(stAddBuff.uExpendType,stAddBuff.iValue, DoingType_Skill))
				{
					pUnit->getGameBuffMgr().addBuff(stAddBuff.uBuffID,pSkillModel->uLevel,pOwner->getUnitID(),pSkillModel->uSkillID, DoingType_Skill);
					//CLog::warn("skillAddBuff target %d add buff %d ", pUnit->getUnitID(), stAddBuff.uBuffID);

				}
			}
		}
		else if (EffectTargetType_Self == stSkillEffectType.uEffectTargetType)
		{
			//命中 只加一次的消耗
			if (stSkillEffectType.bAddOneCount && uHitNum == 1)
			{
				_stSkillEffectModel::DMAP_SKILLADDBUFF::_sub_map::const_iterator _pos = pSumap->begin();
				for (; _pos != pSumap->end(); ++_pos)
				{
					const _stSkillAddBuff& stAddBuff = _pos->second;
					if (!stAddBuff.uExpendType || pOwner->addExpend(stAddBuff.uExpendType,stAddBuff.iValue, DoingType_Skill))
						pOwner->getGameBuffMgr().addBuff(stAddBuff.uBuffID,pSkillModel->uLevel,pOwner->getUnitID(),pSkillModel->uSkillID, DoingType_Skill);
				}
				return	true;
			}

			_stSkillEffectModel::DMAP_SKILLADDBUFF::_sub_map::const_iterator _pos = pSumap->begin();
			for (; _pos != pSumap->end(); ++_pos)
			{
				const _stSkillAddBuff& stAddBuff = _pos->second;
				if (!stAddBuff.uExpendType || pOwner->addExpend(stAddBuff.uExpendType,stAddBuff.iValue, DoingType_Skill))
					pOwner->getGameBuffMgr().addBuff(stAddBuff.uBuffID,pSkillModel->uLevel,pOwner->getUnitID(),pSkillModel->uSkillID, DoingType_Skill);
			}
		}
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CSkillAnim::skillAddExpend	(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel,const _stSkillEffectType& stSkillEffectType)
{
	if(!pOwner || !pSkillModel || !pSkillEffectModel|| mapObject.empty()|| SkillEffectType_AddExpend != stSkillEffectType.uEffectType)
		return false;

	//给自己加不依赖命中的消耗
	if (EffectTargetType_Self == stSkillEffectType.uEffectTargetType && !stSkillEffectType.bSkillHit)
		return pOwner->addExpend(pSkillEffectModel->mapAddSelfExpend, DoingType_Skill);

	MAP_GAME_UNIT::iterator _pos =  mapObject.begin();
	uint8 uHitNum = 0;
	for (; _pos != mapObject.end(); ++_pos)
	{
		CSceneUnit* pUnit = _pos->second;
		if (!pUnit)
			continue;

		if (stSkillEffectType.bSkillHit && pUnit->check_mark(SceneUnitMark_SkillMiss))
			continue;

		uHitNum++;
		//目标直接加
		if (EffectTargetType_Other == stSkillEffectType.uEffectTargetType)
		{
			pUnit->addExpend(pSkillEffectModel->mapAddExpend, DoingType_Skill);
		}
		else if (EffectTargetType_Self == stSkillEffectType.uEffectTargetType)
		{
			//命中 只加一次的消耗
			if (stSkillEffectType.bAddOneCount && uHitNum == 1)
				return	pOwner->addExpend(pSkillEffectModel->mapAddSelfExpend, DoingType_Skill);
			
			pOwner->addExpend(pSkillEffectModel->mapAddSelfExpend, DoingType_Skill);
		}
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 清除buff
bool	CSkillAnim::skillClearBuff	(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel,const _stSkillEffectType& stSkillEffectType)
{
	if(!pOwner || !pSkillModel || !pSkillEffectModel|| mapObject.empty())
		return false;

	_emEffectType emEffectType;
	if (SkillEffectType_ClearHarmfulBuff == stSkillEffectType.uEffectType)
	{
		emEffectType = EffectType_Harmful;
	}
	else
	{
		emEffectType = EffectType_Useful;
	}
	//目标是自己，只能清除负面buff
	if (SkillEffectType_ClearHarmfulBuff == stSkillEffectType.uEffectType && EffectTargetType_Self == stSkillEffectType.uEffectTargetType)
		return pOwner->getGameBuffMgr().clearBuff(emEffectType, DoingType_Skill);

	MAP_GAME_UNIT::iterator _pos =  mapObject.begin();
	for (; _pos != mapObject.end(); ++_pos)
	{
		CSceneUnit* pUnit = _pos->second;
		if (!pUnit)
			continue;

		if (stSkillEffectType.bSkillHit && pUnit->check_mark(SceneUnitMark_SkillMiss))
			continue;

		//目标直接加
		if (EffectTargetType_Other == stSkillEffectType.uEffectTargetType)
			pUnit->getGameBuffMgr().clearBuff(emEffectType, DoingType_Skill);
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 增加位移
bool	CSkillAnim::skillAddMove	(CSceneUnit* pOwner,const _stPoint2F&stTargetPoint,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel,const _stSkillEffectType& stSkillEffectType)
{
	if(!pOwner || !pSkillModel || !pSkillEffectModel || SkillEffectType_Move != stSkillEffectType.uEffectType)
		return false;

	_stPoint2F stNewTarget = pOwner->getPosition();
	if (EffectAddMoveType_UseRelease == pSkillEffectModel->stAddMove.uAddMoveType)
	{
		pOwner->getUnitTargetPoint(pOwner->getDirection(),(float32)pSkillModel->uReleaseRange,stNewTarget);
	}
	else if (EffectAddMoveType_UseDistance == pSkillEffectModel->stAddMove.uAddMoveType)
	{
		pOwner->getUnitTargetPoint(pOwner->getDirection(),(float32)pSkillEffectModel->stAddMove.iDistance,stNewTarget);
		//强制修改目标位置
		pOwner->getSkilling().setTargetPoint(stNewTarget);
	}

	pOwner->setPosition(stNewTarget,SetPositionReason_Skill,false);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSkillAnim::skillUseSkill(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel,const _stSkillEffectType& stSkillEffectType)
{
	if(!pOwner || !pSkillModel || !pSkillEffectModel|| mapObject.empty()|| SkillEffectType_AddSkill != stSkillEffectType.uEffectType)
		return false;

	MAP_GAME_UNIT::iterator _pos =  mapObject.begin();
	for (; _pos != mapObject.end(); ++_pos)
	{
		CSceneUnit* pUnit = _pos->second;
		if (!pUnit)
			continue;

		if (stSkillEffectType.bSkillHit && pUnit->check_mark(SceneUnitMark_SkillMiss))
			continue;

		if (EffectTargetType_Other == stSkillEffectType.uEffectTargetType)
		{
			_stPoint2F stTargetPoint(pUnit->getPosition());
			pOwner->getGameSkillMgr().useSkill(pSkillEffectModel->uHitUseSkill,pSkillEffectModel->uLevel,pOwner->getDirection(),
				pUnit->getUnitID(),stTargetPoint);
		}
		else if (EffectTargetType_Self == stSkillEffectType.uEffectTargetType)
		{
			_stPoint2F stTargetPoint(pOwner->getPosition());
			pOwner->getGameSkillMgr().useSkill(pSkillEffectModel->uHitUseSkill,pSkillEffectModel->uLevel,pOwner->getDirection(),
				pOwner->getUnitID(),stTargetPoint);
		}
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSkillAnim::skillOutRingAddBuff(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel,const _stSkillEffectType& stSkillEffectType)
{
	if(!pOwner || !pSkillModel || !pSkillEffectModel|| !pSkillEffectModel->uOutRingAddBuff || mapObject.empty()|| SkillEffectType_OutRingAddBuff != stSkillEffectType.uEffectType)
		return false;

	MAP_GAME_UNIT::iterator _pos =  mapObject.begin();
	for (; _pos != mapObject.end(); ++_pos)
	{
		CSceneUnit* pUnit = _pos->second;
		if (!pUnit || !pUnit->check_mark(SceneUnitMark_OutSideDamage))
			continue;

		if (stSkillEffectType.bSkillHit && pUnit->check_mark(SceneUnitMark_SkillMiss))
			continue;

		if (EffectTargetType_Other == stSkillEffectType.uEffectTargetType)
		{
			pUnit->getGameBuffMgr().addBuff(pSkillEffectModel->uOutRingAddBuff,pSkillEffectModel->uLevel,pOwner->getUnitID(),pSkillModel->uSkillID, DoingType_Skill);
		}
		else if (EffectTargetType_Self == stSkillEffectType.uEffectTargetType)
		{
			pOwner->getGameBuffMgr().addBuff(pSkillEffectModel->uOutRingAddBuff,pSkillEffectModel->uLevel,pOwner->getUnitID(),pSkillModel->uSkillID, DoingType_Skill);
		}
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------ 重置buff时间
bool	CSkillAnim::skillResetBuffTime(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel,const _stSkillEffectType& stSkillEffectType)
{
	if(!pOwner || !pSkillModel || !pSkillEffectModel || mapObject.empty()|| SkillEffectType_ResetBuffTime != stSkillEffectType.uEffectType)
		return false;

	MAP_GAME_UNIT::iterator _pos =  mapObject.begin();
	for (; _pos != mapObject.end(); ++_pos)
	{
		CSceneUnit* pUnit = _pos->second;
		if (!pUnit )
			continue;
		pUnit->getGameBuffMgr().resetBuffTime(pOwner,pSkillEffectModel->uResetBuffID);
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------ 技能命中完成
bool	CSkillAnim::skillDamageOver(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel)
{
	if(!pOwner || !pSkillModel || !pSkillEffectModel || mapObject.empty())
		return false;

	MAP_GAME_UNIT::iterator _pos =  mapObject.begin();
	for (; _pos != mapObject.end(); ++_pos)
	{
		CSceneUnit* pUnit = _pos->second;
		if (!pUnit)
			continue;
		pUnit->delete_mark(SceneUnitMark_OutSideDamage);
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CSkillAnim::skillCure	(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stPoint2F&stTargetPoint,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel)
{
	if(!pOwner || !pSkillModel || !pSkillEffectModel|| mapObject.empty())
		return false;

	MAP_GAME_UNIT::iterator _pos =  mapObject.begin();
	for (; _pos != mapObject.end(); ++_pos)
	{
		CSceneUnit* pUnit = _pos->second;
		if (!pUnit)
			continue;
		uint64 uAddHp = (uint64)pUnit->getHPMax() * pSkillEffectModel->uExtraCureRadio / 10000 + pSkillEffectModel->uExtraCure;

		//治疗加深
		uAddHp += (uint64)(uAddHp * pOwner->getGameBuffMgr().getAddCureRate());

		pUnit->updateHP((int32)uAddHp,pOwner, DoingType_Skill);

		pUnit->getHatredMgr().onByCure_Skill(pOwner,(int32)uAddHp,pSkillModel,pSkillEffectModel);
		CGameSkillMgr::send_AttackDamage_Ack(pOwner,pSkillModel->uSkillID,10,(int32)uAddHp,true,pUnit);
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CSkillAnim::skillAttack	(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stPoint2F&stTargetPoint,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel)
{
	if(!pOwner || !pSkillModel || !pSkillEffectModel|| mapObject.empty())
		return false;

	//广播受击动作
	PBS2CSkillEffectBroad pbBroad;
	pbBroad.set_sourceid(pOwner->getUnitID());
	pbBroad.set_skillid(pSkillModel->uSkillID);

	//单独伤害
	MAP_GAME_UNIT::iterator _pos = mapObject.begin();
	MAP_GAME_UNIT::iterator _end = mapObject.end();
	uint8 uAttackNum = 1;
	for (;_pos != _end;++_pos)
	{
		CSceneUnit*pTarget = _pos->second;
		if (!pTarget)
			continue;

		if (pTarget->checkStateBuffAttr(StateBuffAttr_ForbidDamage))
			continue;

		attackDamage(pOwner,pTarget,stTargetPoint,pSkillModel,pSkillEffectModel,uAttackNum++,mapObject.size());

		//怪物才能被击退
		//if (pTarget->asMonster())
		{
			PBS2CSkillEffect* pbEffect = pbBroad.add_skilleffect();
			if (pbEffect)
			{
				PBPoint* pbPos = pbEffect->mutable_newposion();
				if ( pbPos )
				{
					pbPos->set_posx(pTarget->getPosition().x);
					pbPos->set_posy(pTarget->getPosition().y);
				}
				pbEffect->set_targetid(pTarget->getUnitID());
			}
		}
	}
	
	if (pbBroad.skilleffect_size() >= 1 )
	{
		//不广播给自己
		pOwner->sendBroadcast(P_S2C_Protocol_Skill,S2C_Skill_EffectBroad,&pbBroad,R_ResultSkill_Succeed,true);
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 计算伤害
bool	CSkillAnim::attackDamage(CSceneUnit* pOwner,CSceneUnit*pTarget,const _stPoint2F&stTargetPoint,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel,uint8 uAttackNum,uint8 uTargetNum)
{
	if(!pOwner || !pTarget || !pSkillModel || !pSkillEffectModel || !uTargetNum)
		return false;
	
	//环形伤害外环伤害值不一样 外网加buff只加一次(金轮阵)
	uint32 uExtraDamageRadio = pSkillEffectModel->uExtraDamageRadio ;
	if (pTarget->check_mark(SceneUnitMark_OutSideDamage))
		uExtraDamageRadio = pSkillEffectModel->uParamDamageRadio;

	//MAX(((MAX((a攻击-b防御)*(a技能放大比+a伤害加深-b伤害减免)+a附加伤害,a攻击*0.1))*MAX(a暴击率*b失心率-b暴击免疫*a失心率,1.1)+a重击-b格挡)*(1-b豁免),1)
	float32 fDamageRadio = MAX(0.0f,(uExtraDamageRadio+ pOwner->getAttribute(BattleAttribute_DamageImprove) - pTarget->getAttribute(BattleAttribute_DamageRemit)) / FLOAT_MAX_PROBABILITY);
	int32 iDamage = MAX(0,(int32)((pOwner->getAttribute(BattleAttribute_Attack) - pTarget->getAttribute(BattleAttribute_Defense)) * fDamageRadio));
	iDamage = MAX(int32(iDamage + pSkillEffectModel->uExtraDamage),int32(pOwner->getAttribute(BattleAttribute_Attack) * 0.1));
	
	//计算蓄力
	if (pSkillModel->check_mark(SkillMark_Charge))
	{
		float32 fChargeRate = pOwner->getSkilling().getChargeRate();
		iDamage = int32(iDamage * fChargeRate);
	}

	//计算暴击率 默认扩大十倍
	uint32 uCritical = pOwner->getCritcal(pTarget,pSkillEffectModel,pSkillModel);
	uint32 uEndCritical = uint32((uint64)uCritical * pSkillEffectModel->uAverageDamageRadio / 10000);

	//计算平均伤害
	if(pSkillModel->check_mark(SkillMark_Average))
		uEndCritical = uEndCritical / uTargetNum;
	//最终输出结果：暴击伤害/正常伤害+重击值-对方格挡值
	iDamage = iDamage * uEndCritical / 10 + pOwner->getAttribute(BattleAttribute_Thump) -pTarget->getAttribute(BattleAttribute_Parry);
	iDamage = int32(iDamage * (1.0f- pTarget->getAttribute(BattleAttribute_Exempt) / FLOAT_MAX_PROBABILITY));
	iDamage = MAX(1,iDamage);

	//怪物受击 无霸体才能被击飞
	CSceneMonster* pTargetMonster = pTarget->asMonster();
	if (pTargetMonster && !pTargetMonster->getSuperArmor() && pTargetMonster->getMonsterModel() && pTargetMonster->getMonsterModel()->uBeAttackMoveTick)
	{
		//先增加硬直
		uint32 uBeAttackTick = pTargetMonster->getMonsterModel()->uBeAttackMoveTick;
		uint32 uBuffID = g_clConstantData.Skill_BeAttackAddBuff;
		pTarget->getGameBuffMgr().addBuff(uBuffID,pSkillEffectModel->uLevel,uBeAttackTick,pOwner->getUnitID(),pSkillModel->uSkillID,false, DoingType_Skill);

		//增加受击位移
		if (pSkillEffectModel->stBeAttackMove.uDirectionType)
		{
			pTarget->setNewPosition(pOwner,pSkillEffectModel->stBeAttackMove.fBeAttackMove,pSkillEffectModel->stBeAttackMove.uDirectionType);
		}
	}

	int32 iTotalDamage = iDamage;

	//伤害倍数增加
	const _stSkillSpareParam* pSpareParam = pSkillModel->findParam(SkillSpareParamType_DamageUpByNum);
	if (pSpareParam && uAttackNum > 1 && pSpareParam->vecParams.size() > 0)
	{
		uint32 uValue = uint32((uAttackNum - 1) * pSpareParam->vecParams[0]);
		iTotalDamage += int32(iDamage * (uValue / FLOAT_MAX_PROBABILITY));
	}

	//buff额外伤害
	iTotalDamage += pTarget->getGameBuffMgr().getExtraDamage(pOwner->getUnitID(),pSkillModel->uCompositeBaseSkillID,iDamage);

	CSceneMonster* pMonster = pTarget->asMonster();
	if ( pMonster )
	{
		iTotalDamage += (int32)(iTotalDamage*pOwner->getGameBuffMgr().getMonsterExtraDamage());
	}

	//消耗加伤害百分比
	if (pSkillEffectModel->stExpendAddDamage.uExpendType
		&& pOwner->addExpend(pSkillEffectModel->stExpendAddDamage.uExpendType,pSkillEffectModel->stExpendAddDamage.iValue, DoingType_Skill))
	{
		iTotalDamage += int32(iDamage * (pSkillEffectModel->stExpendAddDamage.uRate / 10000.0f)) + pSkillEffectModel->stExpendAddDamage.uDamage;
	}

	//技能
	if (pOwner->check_mark(SceneUnitMark_AddExtraDamage,pSkillModel->uSkillID))
	{
		iTotalDamage += int32(iDamage * (pSkillModel->stExpendAddDamage.uRate / 10000.0f)) + pSkillModel->stExpendAddDamage.uDamage;
	}

	// 攻击者有增加伤害的buff(血量越低伤害越高)
	int32	nAddHarmValue	= 0;
	float32	fAddHarmRate	= 0.f;
	pOwner->getGameBuffMgr().getAddTargetHarm(nAddHarmValue,fAddHarmRate);
	iTotalDamage		= (int32)((1.0f + fAddHarmRate)*iTotalDamage);
	iTotalDamage		+= nAddHarmValue;

	//被击者伤害加深
	iTotalDamage += (int32)(iTotalDamage * pTarget->getGameBuffMgr().getAddHarmRate(pSkillModel->uSkillID));

	//上下浮动5%
	iTotalDamage = (int32)(iTotalDamage*((float32)randRange(g_clConstantData.Game_FinalDamageRand_High, g_clConstantData.Game_FinalDamageRand_Low)/100));

	//被击及伤害
	pTarget->byDamage(pOwner,iTotalDamage,pSkillModel,pSkillEffectModel,uCritical);	
	return true;
}

//-------------------------------------------------------------
//------------------------------ 增加技能
bool	CSkillAnim::skillAddSceneObject(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stPoint2F&stTargetPoint,const _stSkillModel* pSkillModel,const _stSkillEffectModel* pSkillEffectModel)
{
	if(!pOwner || !pSkillModel || !pSkillEffectModel)
		return false;

	uint32 uTargetUnitID = 0;
	if (!mapObject.empty() && mapObject.begin()->second)
		uTargetUnitID = mapObject.begin()->second->getUnitID();

	CGameSceneCell* pSceneCell = pOwner->getSceneCell();
	if (!pSceneCell)
		return false;

	_stPoint2F stTmpTarget = stTargetPoint;
	if ( EffectTargetType_Self == pSkillEffectModel->stAddObject.uTargetPositonType)
		stTmpTarget = pOwner->getPosition();

	//计算偏移
	_stPoint2F stRealTarget = stTmpTarget;
	getTargetPoint(stTmpTarget,pOwner->getDirection(),pSkillEffectModel->stAddObject.fDistance,stRealTarget);
	_stPoint2F stRandTarget = stRealTarget;
	CGameSceneCell* pCell = pOwner->getSceneCell();
	if (!pCell)
		return false;

	if (pSkillEffectModel->stAddObject.fRandDis > FLOAT_EPSINON)
	{
		pCell->randGetPoint(stRandTarget,stRealTarget.x,stRealTarget.y,pSkillEffectModel->stAddObject.fRandDis);
	}

	if (GameObject_Trap == pSkillEffectModel->stAddObject.uObjectType)
	{
		const _stTrapModel*pTrapModel = g_clTrapData.findTrap(pSkillEffectModel->stAddObject.uOjbectID,pSkillEffectModel->uLevel);
		if (!pTrapModel)
			return false;

		//无个数 表明是静态陷阱 取技能目标点
		uint8 uObjectNum = pSkillEffectModel->stAddObject.uObjectNum;
		if (pSkillEffectModel->stAddObject.uExpendType &&
			pOwner->addExpend(pSkillEffectModel->stAddObject.uExpendType,pSkillEffectModel->stAddObject.iValue, DoingType_Skill))
		{
			uObjectNum += pSkillEffectModel->stAddObject.uAddNum;
		}

		if (TrapAngleType_OneDir == pSkillEffectModel->stAddObject.uParam1)
		{
			for (uint8 i = 0; i < uObjectNum; ++i)
			{
				pSceneCell->newTrap(pOwner,pTrapModel,stRandTarget.x,stRandTarget.y,pOwner->getDirection(),pSkillEffectModel->stAddObject.fHigh,uTargetUnitID);
			}

		}
		else if (TrapAngleType_Equal == pSkillEffectModel->stAddObject.uParam1)
		{
			float32 fBeginDir = pOwner->getDirection() - getAngleToRadian((float32)pSkillEffectModel->stAddObject.uParam2 / 2);
			float32 fEndDir = pOwner->getDirection() + getAngleToRadian((float32)pSkillEffectModel->stAddObject.uParam2 / 2);
			if (1 == uObjectNum)
			{
				pSceneCell->newTrap(pOwner,pTrapModel,stRandTarget.x,stRandTarget.y,pOwner->getDirection(),pSkillEffectModel->stAddObject.fHigh,uTargetUnitID);
			}
			else if (2 == uObjectNum)
			{
				//起始跟终点都要算(360度的时候，起始点重叠 取左右90度)
				if (pSkillEffectModel->stAddObject.uParam2 == 360)
				{
					pSceneCell->newTrap(pOwner,pTrapModel,stRandTarget.x,stRandTarget.y,pOwner->getDirection() - getAngleToRadian(90),pSkillEffectModel->stAddObject.fHigh,uTargetUnitID);
					pSceneCell->newTrap(pOwner,pTrapModel,stRandTarget.x,stRandTarget.y,pOwner->getDirection() + getAngleToRadian(90),pSkillEffectModel->stAddObject.fHigh,uTargetUnitID);
				}
				else
				{
					pSceneCell->newTrap(pOwner,pTrapModel,stRandTarget.x,stRandTarget.y,fBeginDir,pSkillEffectModel->stAddObject.fHigh,uTargetUnitID);
					pSceneCell->newTrap(pOwner,pTrapModel,stRandTarget.x,stRandTarget.y,pOwner->getDirection() + fEndDir,pSkillEffectModel->stAddObject.fHigh,uTargetUnitID);
				}
			}
			else
			{
				//起始跟终点都要算 (360度的时候，起始点重叠 多分一等份)
				float32 fInterval = getAngleToRadian((float32)pSkillEffectModel->stAddObject.uParam2 / (uObjectNum - 1)); 
				if (pSkillEffectModel->stAddObject.uParam2 == 360)
					fInterval = getAngleToRadian((float32)pSkillEffectModel->stAddObject.uParam2 / uObjectNum);

				for (uint8 i = 0; i < uObjectNum; ++i)
				{
					pSceneCell->newTrap(pOwner,pTrapModel,stRandTarget.x,stRandTarget.y,fBeginDir,pSkillEffectModel->stAddObject.fHigh,uTargetUnitID);
					fBeginDir += fInterval;
				}
			}
		}
		else if (TrapAngleType_Rand == pSkillEffectModel->stAddObject.uParam1)
		{
			for (uint8 i = 0; i < uObjectNum; ++i)
			{
				uint16 uRandAngle = randRange(pSkillEffectModel->stAddObject.uParam2) - pSkillEffectModel->stAddObject.uParam2 / 2;
				float32 fDir = pOwner->getDirection() + getAngleToRadian((float32)uRandAngle);
				pSceneCell->newTrap(pOwner,pTrapModel,stRandTarget.x,stRandTarget.y,fDir,pSkillEffectModel->stAddObject.fHigh,uTargetUnitID);
			}
		}
		else if (TrapAngleType_Static == pSkillEffectModel->stAddObject.uParam1)
		{
			for (uint8 i = 0; i < uObjectNum; ++i)
			{
				pSceneCell->newTrap(pOwner,pTrapModel,stRandTarget.x,stRandTarget.y,pOwner->getDirection(),pSkillEffectModel->stAddObject.fHigh,uTargetUnitID);
				if (pSkillEffectModel->stAddObject.fRandDis > FLOAT_EPSINON)
					pCell->randGetPoint(stRandTarget,stRealTarget.x,stRealTarget.y,pSkillEffectModel->stAddObject.fRandDis);
			}
		}
		else if (TrapAngleType_RandNoRepeat == pSkillEffectModel->stAddObject.uParam1)
		{
			stl_vector<_stPoint2F> vecOut;
			pSceneCell->randGetPoint(vecOut,stRandTarget,pSkillEffectModel->stAddObject.fRandDis,pTrapModel->fRadius,uObjectNum);
			for (uint32 i = 0; i < vecOut.size(); ++i)
			{
				pSceneCell->newTrap(pOwner,pTrapModel,vecOut[i].x,vecOut[i].y,pOwner->getDirection(),pSkillEffectModel->stAddObject.fHigh,uTargetUnitID);
			}
		}
	}
	else if (GameObject_Monster == pSkillEffectModel->stAddObject.uObjectType)
	{
		const _stMonsterModel*pMonsterModel = g_clMonsterData.find(pSkillEffectModel->stAddObject.uOjbectID);
		if (!pMonsterModel)
			return false;

		_stPoint2F stOut;
		for ( uint32 i = 0 ; i < pSkillEffectModel->stAddObject.uObjectNum ;++i )
		{
			CSceneMonster*  pSceneMonster = pCell->createMonster(pMonsterModel,stRandTarget.x,stRandTarget.y,getRandDir(),0,true);
			if (!pSceneMonster)
				continue;
			pSceneMonster->setOwner(pOwner);
			pCell->randGetPoint(stRandTarget,stRealTarget.x,stRealTarget.y,pSkillEffectModel->stAddObject.fRandDis);

		}
	}

	return true;
}