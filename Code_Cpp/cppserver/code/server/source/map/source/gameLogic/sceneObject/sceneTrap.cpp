/*----------------- sceneTrap.cpp
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2016/2/23 11:17:41
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "gameLogic/sceneObject/sceneTrap.h"
#include "protocol/GhostStruct.pb.h"
#include "gameLogic/sceneObject/sceneUnit.h"
#include "gameScene/gameSceneCell.h"
#include "cfg_data/trapData/trapModel.h"
#include "cfg_data/constantData/constantData.h"
#include "log/log.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
CSceneTrap::CSceneTrap()
{
	initialize();
}

//-------------------------------------------------------------
//------------------------------ 
CSceneTrap::~CSceneTrap()
{
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneTrap::packetGhost(uint64 uTime,bool bSelf,uint32& updateMark,PBGhostInfo&pbInfo)
{
	if ( !m_pTrapModel)
		return false;

	if(_CHECK_BIT(updateMark,_BIT32(Ghost_Remove)))
		return false;

	bool bSend = Parent::packetGhost(uTime,bSelf,updateMark,pbInfo);
	PBGhostTrap& pbTrap= *pbInfo.mutable_trap();

	//基本信息
	if(_CHECK_BIT(updateMark,_BIT32(Ghost_Base)))
	{
		bSend = true;
		pbTrap.set_buildtick(m_uBuildTick);
		pbTrap.set_ownerid(getOwnerID());
		pbTrap.set_trapid(m_pTrapModel->uTrapID);
		pbTrap.set_startposx(m_stStartPoint.x);
		pbTrap.set_startposy(m_stStartPoint.y);	
		pbTrap.set_startdir(m_fStartDir);
		pbTrap.set_distance(m_fDistance);
		pbTrap.set_high(m_fHigh);
	}
	_SET_BIT32(updateMark,Ghost_Base,false);

	return bSend;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneTrap::initialize	()
{
	Parent::initialize();
	m_clSinging.initialize(this);
	m_stUnitID.initialize();
	m_stUnitID.uUnitType= GameObject_Trap;

	m_uBuildTick		= 0;
	m_uExitTick			= 0;
	m_uDestroyTick		= 0;

	m_bDelete			= false;
	m_pTrapModel		= NULL;

	m_vecTrapSkill.clear();
	m_setUnitID.clear();

	// 移动相关
	m_uLastMoveTick		= 0;
	m_uStopTick			= 0;
	m_stStartPoint.zero();
	m_stEndPoint.zero();
	m_stPrePoint.zero();
	m_uStartTick		= 0;
	m_fStartDir			= 0.0f;
	m_fDistance			= 0.0f;
	m_fHigh				= 0.0f;
	m_uTarUnitID		= 0;
	m_eMoveState		= CurMoveState_None;
	m_eMoveMethod		= CurMoveMethod_None;
	m_uMoveStage		= 0;
	m_bNoUsekill		= false;
	return true;
}
//-------------------------------------------------------------
//------------------------------ 构建
bool	CSceneTrap::build			(CSceneUnit* pOwner,const _stTrapModel*pModel,const _stPoint2F&stBuildPos,float32 fDir,float32 fHigh,_stPoint2F*const pTargetPos,uint32 uTarUnitID)
{
	if(!pModel)
		return false;

	m_pTrapModel = pModel;

	setPosition(stBuildPos.x,stBuildPos.y,SetPositionReason_Build,false);
	setDirection(fDir);

	initAttribute();

	// 基础属性
	_stGameAttribute*pAtrr = getAttribute(GameSystemType_Common);
	if ( pAtrr )
	{
		pAtrr->initGameAttribute();

		if ( pOwner )
		{
			for ( uint8 i = 0 ; i < _emBattleAttribute_ARRAYSIZE ; ++i)
			{
				pAtrr->iAttribute[i] = pModel->stAttribute.iAttribute[i];
				pAtrr->iAttribute[i] += (int32) (pModel->stAttribute.iRadio[i]/10000.f*pOwner->getAttribute(i));
			}
		}
		else
		{
			*pAtrr = pModel->stAttribute;
		}
	}

	calculateAttributeBase();

	//生命
	setHP(getHPMax());

	if ( pOwner)
		setOwner(pOwner);

	//增加技能
	if ( !pModel->mapSkillModel.empty() )
	{
		uint64 uCurTick = getSystemTick64();
		m_vecTrapSkill.resize(pModel->mapSkillModel.size());
		MAP_TRAP_SKILL_MODEL::const_iterator _pos = pModel->mapSkillModel.begin();
		MAP_TRAP_SKILL_MODEL::const_iterator _end = pModel->mapSkillModel.end();
		for ( int i = 0; _pos != _end ; ++_pos,++i )
		{
			_stTrapSkill& stSkill	= m_vecTrapSkill[i];
			const _stTrapSkillModel& stSkillModel = _pos->second;
			stSkill.pModel			= &stSkillModel;
			stSkill.uNextTrigerTick =  uCurTick +  stSkillModel.uFirstAffectMS;
		}
	}

	m_uBuildTick		= getSystemTick64();
	m_uExitTick			= getSystemTick64() + (m_pTrapModel->uExistTimeMS?m_pTrapModel->uExistTimeMS:1000*60);

	m_stStartPoint		= stBuildPos;
	m_stPrePoint		= stBuildPos;
	m_fHigh				= fHigh;
	m_fStartDir			= fDir;
	m_uStartTick		= getSystemTick64() + m_pTrapModel->uDelayMoveTick;
	m_uTarUnitID		= uTarUnitID;
	m_eMoveMethod		= CurMoveMethod_None;
	m_eMoveState		= CurMoveState_None;
	m_uMoveStage		= 1;
	m_fDistance			= pModel->fDistance;
	setCampID(pOwner != NULL ? pOwner->getCampID() : m_pTrapModel->uCampID);
	if ( pTargetPos && (!isFloatEqual(pTargetPos->x,0) || !isFloatEqual(pTargetPos->y, 0)) )
		m_fDistance = getDistance(m_stStartPoint,*pTargetPos);


	switch (m_pTrapModel->uMoveType)
	{
	case TrapMoveType_Straight:
		{			
			getTargetPoint(m_stStartPoint, m_fStartDir, m_fDistance, m_stEndPoint);
		}
	case TrapMoveType_Parabola:
	case TrapMoveType_Boomerang:
		{
			m_eMoveMethod = CurMoveMethod_Straight;
		}
		break;
	case TrapMoveType_Track:
		{
			m_eMoveMethod = CurMoveMethod_Track;
		}
		break;
	case TrapMoveType_SpiralClockWise:
		{
			m_eMoveMethod = CurMoveMethod_Spiral;
		}
	case TrapMoveType_SpiralCounterClockWise:
		{
			m_eMoveMethod = CurMoveMethod_SpiralRevert;
		}
		break;
	default:
		break;
	}
	if ( m_eMoveMethod != CurMoveMethod_None )
	{
		m_uLastMoveTick	= getSystemTick64();
		m_eMoveState	= CurMoveState_Move;
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneTrap::processLogic	(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay)
{
	//是否销毁
	if (m_uDestroyTick)
	{
		if (m_uDestroyTick <=uTick)
		{
			destroy();
		}

		return;
	}

	Parent::processLogic(uTick,uTime,uSecond,bNewDay);

	if ( !m_pTrapModel || !m_pSceneCell )
		return ;

	// 主人消失就消失 或者时间到了
	CSceneUnit* pOwner = getOwner();
	if ( !pOwner || m_uExitTick < uTick || m_bDelete)
	{
		onExit();
		return;
	}

	// 使用技能
	VEC_TRAP_SKILL::iterator _pos = m_vecTrapSkill.begin();
	VEC_TRAP_SKILL::iterator _end = m_vecTrapSkill.end();
	for ( ; _pos != _end ; ++_pos )
	{
		if (m_bNoUsekill)
			break;

		_stTrapSkill& stSkill	= *_pos;
		if ( !stSkill.pModel )
			continue;

		if (stSkill.uNextTrigerTick > uTick )
			continue;
		_stPoint2F stTargetPoint = getPosition();
		
		//使用技能
		getGameSkillMgr().useSkill(stSkill.pModel->uSkillID,(uint8)m_pTrapModel->uTapLevel,getDirection(),m_uTarUnitID,stTargetPoint);
		//CLog::warn("trap  use skill %d, ", stSkill.pModel->uSkillID);
		stSkill.uNextTrigerTick += stSkill.pModel->uIntervalMS;
	}

	processMove(uTick,uTime,uSecond);
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneTrap::processMove			(uint64 uTick,uint64 uTime,uint32 uSecond)
{
	if ( !m_pTrapModel || !m_pSceneCell )
		return ;

	//延迟移动
	if (m_pTrapModel->uDelayMoveTick && m_uStartTick > uTick)
		return;

	switch ( m_eMoveState )
	{
	case CurMoveState_Move:
		{
			processMove_Move(uTick,uTime,uSecond);
		}
		break;
	case CurMoveState_Stop:
		{
			processMove_Stop(uTick,uTime,uSecond);
		}
		break;
	default:
		break;
	}
}
//-------------------------------------------------------------
//------------------------------  // 位置变动要自动控制 
void	CSceneTrap::process_move		(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay)
{

}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneTrap::processMove_Move			(uint64 uTick,uint64 uTime,uint32 uSecond)
{
	if ( !m_pTrapModel || !m_pSceneCell )
		return ;

	_stPoint2F	target = m_stStartPoint;//穿透的目标点
	float32		fDir	= getDirection();
	bool		bStop	= false;
	switch (m_eMoveMethod)
	{
	case CurMoveMethod_Straight:
		{
			float32		fDist	= getMoveSpeed(MoveType_Run) * (float32)(uTick - m_uStartTick);
			getTargetPoint(m_stStartPoint,fDir,fDist,target);
			bStop = ( getDistance(m_stStartPoint,target) >= getFlyDistance() );
		}
		break;
	case CurMoveMethod_Track:
		{
			CSceneUnit* pTarget = NULL;
			if ( m_uTarUnitID )
				pTarget = m_pSceneCell->findObject(m_uTarUnitID);

			// 目标没有了 就消失
			if ( !pTarget )
			{
				onExit();
				return;
			}

			float32		fDist	= getMoveSpeed(MoveType_Run) * (float32)(uTick - m_uLastMoveTick);
			fDir = get2PiRadian(m_stPrePoint,pTarget->getPosition());
			float32 fTmpDist = getDistance(m_stPrePoint,pTarget->getPosition()) - pTarget->getModelRadius();
			fDist = MIN(fTmpDist,fDist);

			getTargetPoint(m_stPrePoint,fDir,fDist,target);

			fDir = get2PiRadian(target,pTarget->getPosition());
			bStop = ( getDistance(target,pTarget->getPosition()) <= (pTarget->getModelRadius()+0.1f) );
			m_stPrePoint = target;
		}
		break;
	case CurMoveMethod_Spiral:
		{
			// 每秒转180度
			float32 fTarDir = m_fStartDir + (float32)(uTick - m_uStartTick) * Float_Pi /1000.f;
			float32	fDist	= getMoveSpeed(MoveType_Run) * (float32)(uTick - m_uStartTick);
			getTargetPoint(m_stStartPoint,fTarDir,fDist,target);
			fDir	= fTarDir + Float_HalfPi; // 大概的切线
			fDir	= get2PiRadian(fDir);
			bStop = ( getDistance(m_stStartPoint,target) >= getFlyDistance() );
		}
		break;
	case CurMoveMethod_SpiralRevert:
		{
			// 每秒转180度
			uint32 uAngleSpeed = g_clConstantData.Skill_TrapAngleSpeed;
			float32 fTarDir = m_fStartDir - (float32)(uTick - m_uStartTick) * (uAngleSpeed / 360.f) * Float_2Pi /1000.f;
			float32	fDist	= getMoveSpeed(MoveType_Run) * (float32)(uTick - m_uStartTick);
			getTargetPoint(m_stStartPoint,fTarDir,fDist,target);
			fDir	= fTarDir - Float_HalfPi; // 大概的切线
			fDir	= get2PiRadian(fDir);
			bStop = ( getDistance(m_stStartPoint,target) >= getFlyDistance() );
		}
		break;
	default:
		break;
	}

	//碰到障碍后停止
	_stPoint2F	realTarget = target;//实际目标点
	if (TrapStopType_Through == m_pTrapModel->uStopType && !m_pSceneCell->getTargetPoint(m_stStartPoint,target,realTarget))
	{
		bStop = true;
		if (CurMoveMethod_Track == m_eMoveMethod)
		{
			m_stPrePoint = realTarget;
		}
	}

	m_uLastMoveTick	= uTick;

	if ( bStop )
	{
		if ( (m_pTrapModel->uMoveType == TrapMoveType_Boomerang ) &&  (m_uMoveStage>1) )
		{
			onExit();
			return;
		}
		m_eMoveState	= CurMoveState_Stop;
		m_uStopTick		= uTick;
	}
	setMoveState( bStop ? MoveType_Idle : MoveType_Run,realTarget.x,realTarget.y,fDir);
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneTrap::processMove_Stop			(uint64 uTick,uint64 uTime,uint32 uSecond)
{
	if ( !m_pTrapModel || !m_pSceneCell )
		return ;

	if (uTick < m_uStopTick + m_pTrapModel->uStayTime )
		return;

	CSceneUnit* pOwner = getOwner();
	if ( !pOwner )
		return ;

	switch (m_pTrapModel->uMoveType)
	{
	case TrapMoveType_Boomerang:
		{
			m_eMoveMethod	= CurMoveMethod_Track;
			m_uTarUnitID	= getOwnerID();
			m_setUnitID.clear();
			m_uMoveStage++;
			m_eMoveState	= CurMoveState_Move;
			m_stStartPoint	= getPosition();

			float32		fDir	= get2PiRadian(getPosition(),pOwner->getPosition());
			setMoveState( MoveType_Run,getPosition().x,getPosition().y,fDir);
			m_uLastMoveTick	= uTick;
			m_stPrePoint	= getPosition();
		}
		break;
	case TrapMoveType_Track:
		{
			if (m_uMoveStage >= m_pTrapModel->uPenNumber)
			{
				onExit();
				return;
			}
			
			//重新找目标 找最近的
			uint32 uNewTargetID = 0;
			{
				uint32 uDistance = m_pTrapModel->findSpareParam(TrapSpareParamType_FindTarDistance);

				MAP_GAME_UNIT	mapObject;
				mapObject.clear();
				//搜集最大的圆范围目标
				pOwner->gatherSceneObject(getPosition(),(float32)uDistance,mapObject,g_uBattleObject);
				float32 fMinDis = 0.0f;
				MAP_GAME_UNIT::iterator iterUseObject = mapObject.begin();
				for (; iterUseObject != mapObject.end(); ++iterUseObject)
				{
					CSceneUnit* pUnit = iterUseObject->second;
					if(!pUnit || !pUnit->canByUseSkill(this) || !this->canUseSkill(pUnit) || TargetCategory_Enemy != getRelation(pUnit))
						continue;

					if (m_setUnitID.is_exist(pUnit->getUnitID()))
						continue;

					float32 fDis = getDistance(getPosition(),pUnit->getPosition());
					if (fDis <= fMinDis || fMinDis < FLOAT_EPSINON)
					{
						fMinDis = fDis;
						uNewTargetID = pUnit->getUnitID();
					}
				}
			}

			if (!uNewTargetID)
			{
				onExit();
				return;
			}
	
			m_eMoveMethod	= CurMoveMethod_Track;
			m_uTarUnitID	= uNewTargetID;
			m_uMoveStage++;
			m_eMoveState	= CurMoveState_Move;
			m_stStartPoint	= getPosition();

			float32		fDir	= get2PiRadian(getPosition(),pOwner->getPosition());
			setMoveState( MoveType_Run,getPosition().x,getPosition().y,fDir);
			m_uLastMoveTick	= uTick;
			m_stPrePoint	= getPosition();
		}
		break;
	default:
		{
			onExit();
			return;
		}
		break;
	}
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneTrap::canUseSkill	(CSceneUnit*pTarget)
{
	if ( !m_pTrapModel)
		return false;

	if ( !Parent::canUseSkill(pTarget) )
		return false;

	CSceneUnit* pOwner = getOwner();
	if ( pOwner && pOwner->asMonster() &&  pTarget &&  pOwner->asMonster()->isQuestLimit(pTarget->getOwnerPlayer()))
		return false;

	if (m_pTrapModel->check_mark(TrapMark_MultiDmg))
		return true;

	if ( pTarget && m_setUnitID.end() !=  m_setUnitID.find(pTarget->getUnitID()) )
		return false;

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneTrap::canByUseSkill	(CSceneUnit*pAtker)
{
	if ( !m_pTrapModel)
		return false;

	CSceneUnit* pOwner = getOwner();
	if ( pOwner && pOwner->asMonster() && pAtker && pOwner->asMonster()->isQuestLimit(pAtker->getOwnerPlayer()))
		return false;

	return m_pTrapModel->check_mark(TrapMark_IsBeAtkable);
}
//-------------------------------------------------------------
//------------------------------
bool	CSceneTrap::isVisible				(CSceneUnit*pObject)
{
	if (!pObject)
		return false;

	CSceneUnit* pOwner = getOwner();
	if ( pOwner && pOwner->asMonster() &&  pObject &&  pOwner->asMonster()->isQuestLimit(pObject->getOwnerPlayer()))
		return false;

	return Parent::isVisible(pObject);
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneTrap::callbackAttack		(CSceneUnit* pUnit,int32 iDamage,bool bCritical)
{
	if ( !m_pTrapModel )
		return ;

	Parent::callbackAttack(pUnit,iDamage,bCritical);
	if ( pUnit )
	{
		//对第一个目标加buff
		if (m_setUnitID.empty())
		{
			uint32 uBuffID = m_pTrapModel->findSpareParam(TrapSpareParamType_FirstAddBuff);
			if (uBuffID)
				pUnit->getGameBuffMgr().addBuff(uBuffID,1,this->getUnitID(),0, DoingType_Trap);
		}

		m_setUnitID.insert(pUnit->getUnitID());
	}
	

	//可以伤害几个角色
	if (m_pTrapModel->uPenNumber && m_setUnitID.size() >= m_pTrapModel->uPenNumber)
	{
		m_bNoUsekill = true;

		//是否停止
		if (TrapStopType_Pen == m_pTrapModel->uStopType)
		{
			m_eMoveState	= CurMoveState_Stop;
			m_uStopTick		= getSystemTick64();
		}
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneTrap::callbackSkillHitTarget		(CSceneUnit* pUnit,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel)
{
	if ( !pSkillModel || !pSkillEffectModel || !pUnit)
		return ;

	Parent::callbackSkillHitTarget(pUnit,pSkillModel,pSkillEffectModel);

	//是否命中改变阵营
	if (m_pTrapModel->check_mark(TrapMark_IsHitChangeCamp))
	{
		setCampID(pUnit->getCampID());
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneTrap::callbackSkillHit		(const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel)
{
	if ( !pSkillModel || !pSkillEffectModel)
		return ;

	Parent::callbackSkillHit(pSkillModel,pSkillEffectModel);
	
	

	//是否命中消失
	if (m_pTrapModel->check_mark(TrapMark_IsHitDelete))
	{
		m_bDelete = true;
	}
}
//-------------------------------------------------------------
//------------------------------ 
float32	CSceneTrap::getFlyDistance	()
{
	return m_fDistance;
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneTrap::onExit	()
{
	if (!m_pTrapModel || !m_pSceneCell || m_uDestroyTick)
		return;

	//消失放技能
	if (!m_pTrapModel->mapDestroyAction.empty())
	{
		_stTrapModel::MAP_DESTROY::const_iterator _pos = 	m_pTrapModel->mapDestroyAction.begin();
		for (; _pos != m_pTrapModel->mapDestroyAction.end(); ++_pos)
		{
			if (TrapDestroyType_UseSkill == _pos->first && _pos->second.vecParam.size() > 0)
			{
				uint32 uSkillID = _pos->second.vecParam[0];
				_stPoint2F stTargetPoint = getPosition();

				//使用技能
				getGameSkillMgr().useSkill(uSkillID,(uint8)m_pTrapModel->uTapLevel,getDirection(),m_uTarUnitID,stTargetPoint);
			}
		}
	}

	//无延迟直接销毁
	if (!m_pTrapModel->uDelayDestroyTick)
	{
		destroy();
	}
	else
	{
		m_uDestroyTick = getSystemTick64() + m_pTrapModel->uDelayDestroyTick;

		//设置隐藏
		addState(StateType_Hide);
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneTrap::destroy	()
{
	if (m_pSceneCell)
		m_pSceneCell->leaveScene(this);
}
