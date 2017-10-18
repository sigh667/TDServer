/*------------- sceneUnit.cpp
* Copyright (C): 2011  
* Author       : 赵文源
* Version      : V1.01
* Date         : 2011/3/23 13:07:45
*
*/ 
/***************************************************************
* 
***************************************************************/
#include "gameLogic/sceneObject/sceneUnit.h"
#include "gameScene/gameSceneCell.h"
#include "basic/basicFunctions.h"
#include "math/mathFunctions.h"
#include "log/log.h"
#include "gameScene/gameScene.h"
#include "gameScene/copymapBase.h"
#include "protocol/GhostStruct.pb.h"
#include "protocol/sceneStruct.pb.h"
#include "gameLogic/cooling/cooling.h"
#include "packet/clientPacket.h"
#include "cfg_data/battleData/battleData.h"
#include "cfg_data/constantData/constantData.h"
#include "cfg_data/campData/campData.h"
#include "cfg_data/jobData/jobData.h"
#include "gameLogic/sceneObject/sceneUnitAI.h"
#include "def_struct/teamStruct.h"
/*************************************************************/
#define TickMs      32
//-------------------------------------------------------------
//-------------------------------------------------------------
//------------------------------ 
CSceneUnit::CSceneUnit()
{
	m_stUnitID.uUnitType= GameObject_None;
	m_uGhostObject		= _BIT32(GameObject_Player);

	m_pSceneCell		= NULL;
	m_pSceneBlock		= NULL;
	m_pSceneHave		= NULL;
	m_pRegion			= NULL;
	m_pUnitAI			= NULL;
	m_pJobModel			= NULL;
	m_uCampID = 0;
	initialize();
}
//-------------------------------------------------------------
//------------------------------ 
CSceneUnit::~CSceneUnit()
{
	unregisterObject();
}

//-------------------------------------------------------------
//------------------------------ 
uint32	CSceneUnit::registerObjectID	()
{
	static uint32	g_uGameObjectID = 1;

	return ++g_uGameObjectID;
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::registerObject		(uint32 uUnitID)
{
	if(!m_bRegister || !m_stUnitID.uUnitID)
		m_stUnitID.uUnitID = ((uUnitID != 0) ? uUnitID : registerObjectID());

	m_bRegister	= true;
}

//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::unregisterObject	()
{
	m_bRegister	= false;
	m_stUnitID.uUnitID	= 0;

	m_pRegion	= NULL;
	m_pSceneCell	= NULL;
	m_pSceneHave	= NULL;
	m_pSceneBlock	= NULL;
}

//-------------------------------------------------------------
//------------------------------ 
uint16	CSceneUnit::getSceneID()
{
	if(m_pSceneCell)
		return m_pSceneCell->getSceneID();

	return 0;
}

//-------------------------------------------------------------
//------------------------------ 
uint32	CSceneUnit::getSceneCellID()
{
	if(m_pSceneCell)
		return m_pSceneCell->getCellID();

	return 0;
}
//-------------------------------------------------------------
//------------------------------ 
uint64	CSceneUnit::getSceneDynID()
{
	if(m_pSceneCell)
		return m_pSceneCell->getDynID();

	return 0;
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::refreshStateBuffAttr()
{
	bool bHide = checkStateBuffAttr(StateBuffAttr_Hide);
	m_uStateBuffAttrMark = m_clGameBuffMgr.getStateBuffAttr();

	if ( bHide != checkStateBuffAttr(StateBuffAttr_Hide) )
	{
		refreshVisible(!bHide);
	}

	if ( checkStateBuffAttr(StateBuffAttr_ForbidMove) )
		breakMove();

	if ( checkStateBuffAttr(StateBuffAttr_ForbidSkill) )
		breakUseSkill();

	if ( checkStateBuffAttr(StateBuffAttr_ForbidMount) )
	{
		if (asPlayer())//下马
			asPlayer()->mount(false);
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::refreshBuffAttr()
{
	m_stGameAttributeBuff.initGameAttribute();
	_stGameAttribute* pGameAttribute = m_mapSystemAttribute.find_(GameSystemType_Buff);
	if (!pGameAttribute)
		return;

	pGameAttribute->initGameAttribute();

	m_clGameBuffMgr.refreshAttr();
	calculateAttributeBuff();
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnit::checkStateBuffAttr	(_emStateBuffAttr eAttr)
{
	return _CHECK_BIT(m_uStateBuffAttrMark,_BIT32(eAttr));
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnit::initialize			()
{
	m_bRegister			= false;

	m_stUnitID.uUnitID	= 0;
	m_uJob				= GameJob_All;
	m_pJobModel			= g_clJobData.findJobModel(m_uJob);
	m_uOwnerID			= 0;
	m_uOwnerPlayerID	= 0;
	m_strOwnerPlayerName.clear();

	m_fDirection		= 0;	//当前朝向 
	m_stPosition.zero();		//当前坐标点

	m_iCurrHP			= 0;
	m_iMaxHP			= 0;
	m_uWalkSpeedAttr	= 0;
	m_uRunSpeedAttr		= 0;
	m_uFightingEndTime	= 0;
	m_uFightingBeginTime= 0;
	m_eMoveType			= MoveType_Idle;
	m_uBeginMoveTick	= 0;
	m_stBeginMovePosition.zero();
	m_fMoveDirection	= 0.0f;
	m_fMoveSpeed		= 0.0f;

	m_uStateBuffAttrMark= 0;
	CSceneUnitAI::destroySceneUnitAI(m_pUnitAI);
	m_uRouteID			= 0;

	m_pSceneCell	= NULL;
	m_pSceneBlock	= NULL;
	m_pSceneHave	= NULL;
	m_pRegion		= NULL;


	CGhostObject::initialize();

	m_clGameSkillMgr.initialize(this);
	m_clGameBuffMgr.initialize(this);
	m_clSinging.initialize(this);
	m_clSkilling.initialize(this,NULL);
	m_clHatredMgr.initialize(this);
	m_clhurtMgr.initialize(this);
	m_clCooling.initialize(this);

	m_mapMark.clear();
	m_mapEnergy.clear();
	m_mapEnergyRecovery.clear();
	m_listAttack.clear();

	//复位属性
	initAttribute();

	dMemset(m_uStateFlag,0,sizeof(m_uStateFlag));

	for (uint8 i = 0;i < _UV_Max;++i)
		m_bUpdateValue[i] = false;

	m_uLastEnergyTick	= 0;
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::refreshAttribute	()
{
}

//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnit::onSetPoint(bool bMove)
{
	//区域ID更新
	updateRange(bMove);

	if(!m_pSceneCell)
		return false;

	if ( !bMove && isMoving() )
	{
		m_eMoveType = MoveType_Idle;
		setGhost(_BIT32(Ghost_Move));
	}

	if(m_pSceneCell->insertObject(this,m_stPosition.x,m_stPosition.y))
		return true;

	//暂不启用
	//removeViewRange(m_stPosition);
	return false;
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::updateSceneBlock(CGameSceneBlock*pBlock)
{
	if(m_pSceneBlock == pBlock)
		return;

	if(m_pSceneBlock)			//从上一个场景块中移除
		m_pSceneBlock->erase(this);

	if(pBlock)
		pBlock->insert(this);

	m_pSceneBlock = pBlock;

	updateViewRange();
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::updateSceneHave		(CGameSceneHave*pHave)
{
	if(m_pSceneHave == pHave)
		return;

	sceneHave(false);

	m_pSceneHave = pHave;
	sceneHave(true);
}
//-------------------------------------------------------------
//------------------------------ 
_stTeamInfo* CSceneUnit::getTeam				()
{
	return NULL;
}
//-------------------------------------------------------------
//------------------------------ 
uint64		CSceneUnit::getTeamID			()
{
	_stTeamInfo* pTeamInfo = getTeam();
	if ( !pTeamInfo )
		return 0;
	return pTeamInfo->uTeamID;
}
//-------------------------------------------------------------
//------------------------------
uint32		CSceneUnit::getFactionID		()
{
	return 0;
}
//-------------------------------------------------------------
//------------------------------
uint32		CSceneUnit::getUnionID	()
{
	return 0;
}
//-------------------------------------------------------------
//------------------------------
uint32		CSceneUnit::getCorpsID			()
{
	return 0;
}
//-------------------------------------------------------------
//------------------------------ 
void		CSceneUnit::updateViewRange()
{
	/*收集新单元范围对像列表*/ 
	static MAP_GAME_UNIT	mapObject;
	mapObject.clear();
	if(m_pSceneCell)	/*pCell=NULL时为退出不需要收集*/ 
		m_pSceneCell->gatherObject(getPosition(),ViewRange,m_uGhostObject,mapObject);

	//当自己在队伍中时
	_stTeamInfo*pTeam = getTeam();
	//把所有当前场景队友加入ghost列表中
	if(pTeam)
	{
		stl_map<uint32,_stTeamMember>::iterator _pos = pTeam->mapMember.begin();
		stl_map<uint32,_stTeamMember>::iterator _end = pTeam->mapMember.end();
		for (; _pos != _end ;++_pos)
		{
			_stTeamMember& stMember = _pos->second;
			CScenePlayer*pPlayer = m_pSceneCell->findPlayer(stMember.stAvatar.stDisplay.uPlayerID);
			if(pPlayer && pPlayer != this && pPlayer->isClientConneted())
				mapObject.insert_(pPlayer->getUnitID(),pPlayer);
		}
	}

	updateGhostList(mapObject);
}
//-------------------------------------------------------------
//------------------------------ 
bool CSceneUnit::isInViewRange		(CSceneUnit* pUnit,float32 iRadius)
{
	if ( pUnit == NULL )
		return false;
	return inRange( getPosition(), pUnit->getPosition(), iRadius );
}

//-------------------------------------------------------------
//------------------------------ 
CGameScene*CSceneUnit::getSceneMap()
{
	return m_pSceneCell ? m_pSceneCell->getSceneMap() : NULL;
}

//-------------------------------------------------------------
//------------------------------ 
CCopymapBase*	CSceneUnit::getGameCopymap	()
{
	return m_pSceneCell ? m_pSceneCell->getGameCopymap() : NULL;
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::updateGhostList	(MAP_GAME_UNIT& _map)
{
	CGhostObject::updateGhostList(_map);
}
//-------------------------------------------------------------
//------------------------------ 自己能否看到目标
bool	CSceneUnit::isVisible(CSceneUnit*pObject)
{
	if ( !pObject )
		return false;

	//位面必不可见
	if (haveState(StateType_Plane) || pObject->haveState(StateType_Plane))
		return false;

	//如果目标隐身
	if (pObject->haveState(StateType_Hide) || pObject->checkStateBuffAttr(StateBuffAttr_Hide))
	{
		//敌对不可见
		if (TargetCategory_Enemy == getRelation(pObject))
		{
			return false;
		}
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool	CSceneUnit::isHide				()
{
	return haveState(StateType_Plane) || haveState(StateType_Hide) || checkStateBuffAttr(StateBuffAttr_Hide);
}
//-------------------------------------------------------------
//------------------------------ 
CScenePlayer*	CSceneUnit::findCellPlayer		(uint32 uID)
{
	if(!m_pSceneCell)
		return NULL;

	return m_pSceneCell->findPlayer(uID);
}

//-------------------------------------------------------------
//------------------------------ 
CSceneUnit*	CSceneUnit::findCellObject		(uint32 uID)
{
	if(!m_pSceneCell)
		return NULL;

	return m_pSceneCell->findObject(uID);
}

//-------------------------------------------------------------
//------------------------------ 
CSceneUnit*	CSceneUnit::findRangeObject(uint32 uID)
{
	if(!m_pSceneCell)
		return NULL;

	CSceneUnit*pUnit = m_pSceneCell->findObject(uID,m_stPosition.x,m_stPosition.y,ViewRange);
	if(pUnit && inRange(m_stPosition,pUnit->getPosition(),(float32)ViewRange))
		return pUnit;

	return NULL;
}

//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::gatherSceneObject	(const _stPoint2F&stPoint,float32 uRadius,MAP_GAME_UNIT& _map,uint32 uTypeMark,bool bClear)
{
	if(bClear)
		_map.clear();
	if(m_pSceneCell)
		m_pSceneCell->gatherObject(stPoint,uRadius,uTypeMark,_map);
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::gatherSceneObject	(const _stPoint2F&stPoint,float32 uRadius,VECTOR_RANGE_UNIT& vec,uint32 uTypeMark)
{
	static MAP_GAME_UNIT _map;
	_map.clear();
	gatherSceneObject(stPoint,uRadius,_map,uTypeMark);

	_stPoint2F stPointF = m_stPosition;
	MAP_GAME_UNIT::iterator _pos = _map.begin();
	MAP_GAME_UNIT::iterator _end = _map.end();
	for (;_pos != _end;++_pos)
	{
		CSceneUnit* pUnit = _pos->second;
		if(!pUnit)
			continue;

		float32 fDis = getDistance(pUnit->getPosition(),stPointF) - pUnit->getSkillModelRadius();
		if (fDis < 0.0f)
			fDis = fDis * (-1);

		vec.push_back(_stRangeUnit(pUnit,fDis));
	}
	if(!vec.empty())
		qsort(&vec[0],vec.size(),sizeof(_stRangeUnit),CGhostObject::_rangeUnit_distance);
}

//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::gatherObjectPoint	(const _stPoint2F&stPoint,MAP_GAME_UNIT&mapObject,VECTOR_GAME_UNIT& vec)
{
	CSceneUnit*		pObject = NULL;
	MAP_GAME_UNIT::iterator pos	= mapObject.begin();
	MAP_GAME_UNIT::iterator end	= mapObject.end();
	for (;pos != end;++pos)
	{
		pObject = pos->second;
		if(!pObject || pObject->getPosition() != stPoint)
			continue;

		vec.push_back(pObject);
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::gatherObjectLine	(const _stPoint2F&stPoint,float32 fDirection,float32 uLineLength,float32 uLineWidth,MAP_GAME_UNIT&mapObject,MAP_GAME_UNIT& mapUseObject,bool bOnlyMove/* = false*/)
{
	//计算面向角度
	float32		fRotation	= fDirection;
	//目标在自己面像90度以内
	float32		fRadianRange= getAngleToRadian(90);
	_stPoint2F	stPointF = m_stPosition;
	_stPoint2F	stEnd		= stPointF;

	//斜线距离
	float32 fLienght = 1.0f;

	stEnd.x += ::sin(fRotation) * uLineLength * fLienght;
	stEnd.y += ::cos(fRotation) * uLineLength * fLienght;

	float32 fDistance = fLienght * uLineWidth / 2.0f;

	CSceneUnit*		pObject = NULL;
	MAP_GAME_UNIT::iterator pos	= mapObject.begin();
	MAP_GAME_UNIT::iterator end	= mapObject.end();
	for (;pos != end;++pos)
	{
		pObject = pos->second;
		if(!pObject)
			continue;

		if((fLienght * uLineLength) + pObject->getSkillModelRadius() < getDistance(stPointF,pObject->getPosition()))
			continue;

		if(pObject->getPosition() != stPoint)
		{
			//计算角度
			float32 fRadian = get2PiRadian(stPointF,pObject->getPosition());
			if(!inRadianRange(fRotation,fRadianRange,fRadian))
				continue;
		}

		//是否在矩形宽度范围内
		if(fDistance + pObject->getSkillModelRadius() < getPointToLineDistance(stPointF,stEnd,pObject->getPosition()))
			continue;

		mapUseObject.insert_(pObject->getUnitID(),pObject);
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::gatherObjectArc(const _stPoint2F&stPoint,float32 fDir,float32 uArcRadius,float32 fArcAngle,MAP_GAME_UNIT&mapObject,MAP_GAME_UNIT& mapUseObject,bool bOnlyMove/* = false*/)
{
	if (fArcAngle >= 360.0f)
		return gatherObjectRound(stPoint,uArcRadius,mapObject,mapUseObject,bOnlyMove);

	_stPoint2F	stPointF = m_stPosition;
	//攻击弧度范围
	float32	fRotation	= fDir;
	float32	fRadianRange= getAngleToRadian(fArcAngle) / 2;
	float32 fDistance = uArcRadius;

	CSceneUnit*		pObject = NULL;
	MAP_GAME_UNIT::iterator pos	= mapObject.begin();
	MAP_GAME_UNIT::iterator end	= mapObject.end();
	for (;pos != end;++pos)
	{
		pObject = pos->second;
		if(!pObject)
			continue;

		if(fDistance + pObject->getSkillModelRadius() < getDistance(stPointF,pObject->getPosition()))
			continue;

		if(pObject->getPosition() != stPointF)
		{
			float32 fRadian = get2PiRadian(stPointF,pObject->getPosition());
			if(!inRadianRange(fRotation,fRadianRange,fRadian))
				continue;
		}

		mapUseObject.insert_(pObject->getUnitID(),pObject);
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::gatherObjectRound	(const _stPoint2F&stPoint,float32 uRoundRadius,MAP_GAME_UNIT&mapObject,MAP_GAME_UNIT& mapUseObject,bool bOnlyMove/* = false*/)
{
	//_stPoint2F	stPointF = m_stPosition;
	if (isFloatEqual(uRoundRadius,0.0f))
		return;

	_stPoint2F	stPointF = stPoint;
	float32 fDistance	= uRoundRadius;

	CSceneUnit*		pObject = NULL;
	MAP_GAME_UNIT::iterator pos	= mapObject.begin();
	MAP_GAME_UNIT::iterator end	= mapObject.end();
	for (;pos != end;++pos)
	{
		pObject = pos->second;
		if(!pObject)
			continue;

		//可以打目标模型边缘
		if(fDistance + pObject->getSkillModelRadius() < getDistance(stPointF,pObject->getPosition()))
			continue;

		mapUseObject.insert_(pObject->getUnitID(),pObject);
	}
}
//-------------------------------------------------------------
//------------------------------
void	CSceneUnit::gatherBroadcast		(QUEUE_PLAYER&vectorPacket)
{
	CScenePlayer* pPlayer = NULL;
	MAP_GHOST::iterator _pos = m_mapGhost.begin();
	MAP_GHOST::iterator _end = m_mapGhost.end();
	for(;_pos != _end;++_pos)
	{
		_stGhost&stGhost = (_pos->second);
		/*无可更新内容*/ 
		if(stGhost.testMark(Ghost_Remove) || !stGhost.pObject)
			continue;

		pPlayer = stGhost.pObject->asPlayer();
		if(!pPlayer)
			continue;

		vectorPacket.push_back(pPlayer);
	}
}

//-------------------------------------------------------------
//------------------------------ 根据距离，查找目标点
bool	CSceneUnit::getUnitTargetPoint	(float32 fdir,float32 fdis,_stPoint2F& target)
{
	if (!m_pSceneCell)
		return false;
	return m_pSceneCell->getTargetPoint(getPosition(),fdir,fdis,target);
}

//-------------------------------------------------------------
//------------------------------ 查找两点之间的有效点
bool	CSceneUnit::getUnitTargetPoint	(const _stPoint2F& target,_stPoint2F& outTarget)
{
	if (!m_pSceneCell)
		return false;
	return m_pSceneCell->getTargetPoint(getPosition(),target,outTarget);
}

//-------------------------------------------------------------
//------------------------------ 九宫格广播
void	CSceneUnit::sendBroadcast	(uint8 uMainProtocol,uint8 uSubProtocol,const google::protobuf::Message* pMsg,uint8 uEventID,bool bIgnore/* = false*/)
{
	FUNCTION_BEGIN_CHECK();
	static QUEUE_PLAYER vectorPlayer;
	vectorPlayer.clear();
	if(!bIgnore)
		vectorPlayer.push_back(asPlayer());
	gatherBroadcast(vectorPlayer);

	QUEUE_PLAYER::iterator _pos = vectorPlayer.begin();
	QUEUE_PLAYER::iterator _end = vectorPlayer.end();
	for(;_pos != _end;++_pos)
	{
		CScenePlayer* pPlayer = (*_pos);
		/*无可更新内容*/ 
		if(!pPlayer)
			continue;

		//陷阱的所有的消息都不发送给它的主人
		if (this->asTrap() && getOwner() && getOwner()->getUnitID() == pPlayer->getUnitID())
			continue;

		if (pPlayer->haveState(StateType_Plane) || pPlayer->haveState(StateType_WaitReady))
			continue;

		//发送包
		pPlayer->sendMsg(uMainProtocol,uSubProtocol,pMsg,uEventID);
	}
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnit::canUseSkill	(CSceneUnit*pTarget)
{
	if ( haveState(StateType_Plane) 
		|| haveState(StateType_WaitReady)  || haveState(StateType_Hold)
		 || haveState(StateType_Dart) || haveState(StateType_Qinggong)
		)
		return false;

	if ( checkStateBuffAttr(StateBuffAttr_ForbidSkill) )
		return false;

	if (isDead() && getOwnerPlayer())
		return false;

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnit::canByUseSkill	(CSceneUnit*pAtker)
{
	if ( haveState(StateType_Dead) || haveState(StateType_Plane)
		|| haveState(StateType_Dart) || haveState(StateType_Qinggong)
		)
		return false;
	return true;
}

//-------------------------------------------------------------
//------------------------------ 获取关系
uint8	CSceneUnit::getRelation	(CSceneUnit* pUnit)
{
	uint8 uRelation = TargetCategory_Friend;
	if (!pUnit || pUnit == this)
		return TargetCategory_Self;

	//主人一样 一定为友方
	if ( getOwner() == pUnit->getOwner() )
		return TargetCategory_Friend;

	if (m_uOwnerID)
	{// 有主人 走主人的关系
		CSceneUnit* pOwner = getOwner();
		if (pOwner && pOwner!=this )
			return pOwner->getRelation(pUnit);
	}
	//判断阵营
	uRelation = g_clCampData.findCamp(getCampID(),pUnit->getCampID());
	return uRelation;
}

//-------------------------------------------------------------
//------------------------------ 获取技能叠加后的属性值
int32 CSceneUnit::getSkillAttribute(uint16 uAttrType,const _stSkillEffectModel*pSkillEffectModel)
{
	if (uAttrType < _emBattleAttribute_MIN || uAttrType > _emBattleAttribute_MAX)
		return 0;

	if ( !pSkillEffectModel || pSkillEffectModel->mapAddSelfAttr.empty() )
	{
		return m_stGameAttribute.iAttribute[uAttrType];
	}

	_stGameAttribute stSkillAttr;
	stl_map<uint16,_stAddBattleAttri>::const_iterator _pos = pSkillEffectModel->mapAddSelfAttr.begin();
	stl_map<uint16,_stAddBattleAttri>::const_iterator _end = pSkillEffectModel->mapAddSelfAttr.end();
	for (; _pos != _end; ++_pos)
	{
		const _stAddBattleAttri& stAttri = _pos->second;
		stSkillAttr.iAttribute[stAttri.uAttrType] = stAttri.iValue;
		stSkillAttr.iRadio[stAttri.uAttrType] = stAttri.iRadio;
	}

	this->attributeConvert(m_stGameAttribute,stSkillAttr);
	return stSkillAttr.iAttribute[uAttrType];
}

//-------------------------------------------------------------
//------------------------------ 是否命中
bool	CSceneUnit::isAttackHit	(CSceneUnit* pTarget,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel)
{
	if (!pTarget || !pSkillModel || !pSkillEffectModel)
		return false;

	if (pSkillModel->check_mark(SkillMark_IgnoreDodge))
		return true;

	//是否有攻击丢失状态
	if (checkStateBuffAttr(StateBuffAttr_AttackMiss))
		return false;

	int32 iHitRate = getAttribute(BattleAttribute_HitRate) + MAX_PROBABILITY;

	//MAX(攻方命中*(1-防方威慑) - 防方闪避*(1-攻方威慑)，保底值) 保底值:怪物0,玩家0.15
	float32 fAttHitAttri = (iHitRate / FLOAT_MAX_PROBABILITY) * (1.0f - pTarget->getAttribute(BattleAttribute_OveraweRate) / FLOAT_MAX_PROBABILITY);
	float32 fDefHitAttri = (pTarget->getAttribute(BattleAttribute_DodgeRate) / FLOAT_MAX_PROBABILITY) * (1.0f - getAttribute(BattleAttribute_OveraweRate) / FLOAT_MAX_PROBABILITY);
	//获取技能叠加属性
	float32 fSkillAdd= getSkillAttribute(BattleAttribute_Hit,pSkillEffectModel) / FLOAT_MAX_PROBABILITY;
	float32 fHit = MAX(fAttHitAttri + fSkillAdd - fDefHitAttri,(float32)0.15 + fSkillAdd);
	if (asMonster())
		fHit = MAX(fAttHitAttri + fSkillAdd - fDefHitAttri,0.0f);

	//命中率最低为:技能基础命中率，最高为10000.
	if (!randRatio(uint32(fHit * MAX_PROBABILITY)))
		return false;

	return true;
}

//-------------------------------------------------------------
//------------------------------ 获取暴击数 扩大十倍
uint32	CSceneUnit::getCritcal	(CSceneUnit* pTarget,const _stSkillEffectModel*pSkillEffectModel,const _stSkillModel* pSkillModel)
{
	if (!pTarget || !pSkillEffectModel || !pSkillModel)
		return false;


	//MAX(a暴击率*(1-b失心率) - b暴击免疫*(1-a失心率)，0.15）
	float32 fAttCrit= (getAttribute(BattleAttribute_CritRate) / FLOAT_MAX_PROBABILITY) * (1.0f - pTarget->getAttribute(BattleAttribute_AfraidRate) / FLOAT_MAX_PROBABILITY);
	float32 fCritImmune	= (pTarget->getAttribute(BattleAttribute_CritImmune)/ FLOAT_MAX_PROBABILITY) * (1.0f - getAttribute(BattleAttribute_AfraidRate)/ FLOAT_MAX_PROBABILITY);

	//技能基础暴击率
	float32 fSkillAddCritical = getSkillAttribute(BattleAttribute_CritRate,pSkillEffectModel) / FLOAT_MAX_PROBABILITY;
	float32 fCriticalRate = MAX(fAttCrit - fCritImmune + fSkillAddCritical,(float32)0.15 + fSkillAddCritical);

	//判断是否暴击
	if (!randRatio(uint32(fCriticalRate * MAX_PROBABILITY)))
		return 10;
	
	//暴击回调
	callbackCritical();

	//暴击倍率
	//MAX(a暴击倍数*(1-b失心率) -b暴击免疫*(1-a失心率)，1.1）
	float32 fAttCritical = (getAttribute(BattleAttribute_CritMultiple) / FLOAT_MAX_PROBABILITY) * (1.0f - pTarget->getAttribute(BattleAttribute_AfraidRate) / FLOAT_MAX_PROBABILITY);
	float32 fDefCritical = (pTarget->getAttribute(BattleAttribute_CritImmune) / FLOAT_MAX_PROBABILITY) * (1.0f - getAttribute(BattleAttribute_AfraidRate) / FLOAT_MAX_PROBABILITY);
	float32 fCritical = MAX(fAttCritical - fDefCritical,float32(1.1));
	uint32 uCritical = uint32(fCritical * 10); 
	return MAX((uint32)10,uCritical);
}
//-------------------------------------------------------------
//------------------------------ 被攻击 怪物取拥有者
bool	CSceneUnit::byDamage	(CSceneUnit*pSource,int32 iDamage,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel,uint32 uCritical,uint32 uDelayTime/* = 0*/)
{
	if (!pSource || !iDamage || !pSkillModel || !pSkillEffectModel )
		return false;

	//攻击回调
	callbackByAttack(pSource, iDamage);
	pSource->callbackAttack(this,iDamage,uCritical>10);

	if (!canUpdateHp(-iDamage))
		return false;
	//只发给自己和目标
	CGameSkillMgr::send_AttackDamage_Ack(pSource,pSkillModel->uSkillID,uCritical,iDamage*-1,false,this);
	//伤血
	if ( iDamage >= getHP() )
	{
		callbackBeKillHarm(pSource,iDamage);
	}
	updateHP(iDamage * -1,pSource,DoingType_Skill);

	getHatredMgr().onByDamage_Skill(pSource,iDamage,pSkillModel,pSkillEffectModel);

	//存助攻

	return true;
}
//-------------------------------------------------------------
//------------------------------ 延时伤害
bool	CSceneUnit::byAttack (CSceneUnit* pOwner,const _stPoint2I&stTargetPoint,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel,uint32 uDelayTick)
{
	if (!pOwner || !pSkillModel || !pSkillEffectModel)
		return false;

	static _stSkillAttack stAttack;
	stAttack.pSkillEffectModel = pSkillEffectModel;
	stAttack.pSkillModel = pSkillModel;
	stAttack.uSouceID = pOwner->getUnitID();
	stAttack.stTargetPoint = stTargetPoint;
	stAttack.uDelayTick = uDelayTick + getSystemTick64();

	m_listAttack.push_back(stAttack);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::processAttack		(uint64 uTick)
{
	if(m_listAttack.empty() || !m_pSceneCell)
		return;

	{//计算伤害
		LIST_ATTACK::iterator _pos = m_listAttack.begin();
		while (_pos != m_listAttack.end())
		{
			//死亡清除所有的伤害信息
			if(isDead())
			{
				m_listAttack.clear();
				break;
			}

			//计算时间
			_stSkillAttack&stAttack = (*_pos);
			if(stAttack.uDelayTick > uTick)
			{
				++_pos;
				continue;
			}

			//计算目标是否存在
			CSceneUnit* pSource = m_pSceneCell->findObject(stAttack.uSouceID);
			if (!pSource)
			{
				_pos = m_listAttack.erase(_pos);
				continue;
			}

			CSkillAnim::delayDamage(pSource,this,stAttack.stTargetPoint,stAttack.pSkillModel,stAttack.pSkillEffectModel);
			_pos = m_listAttack.erase(_pos);
		}
	}
}

//-------------------------------------------------------------
//------------------------------ 属性转换公式
int32 CSceneUnit::getConverValue(int32 iAttribute, uint8 uType, float32 fValue)
{
	int32 nUpdate = 0;
	switch (uType)
	{
	case BattleAttrAffectType_Multiply:
		{
			nUpdate = (int32)(iAttribute * fValue + 0.5f);
		}
		break;
	case BattleAttrAffectType_Divide:
		{
			if ( fValue != 0.0f )
				nUpdate = (int32)(iAttribute / fValue + 0.5f);
		}
		break;
	case BattleAttrAffectType_DivideSum:
		{
			if ( fValue + iAttribute != 0.0f )
				nUpdate = (int32)(iAttribute / (fValue + iAttribute) + 0.5f);
		}
		break;
	default:
		CLog::error("getConverValue is error, uType=%d", uType);
		break;
	}

	return nUpdate;
}
//-------------------------------------------------------------
//------------------------------ 转换属性到另外属性
void CSceneUnit::attributeConvert(_stGameAttribute& stAttrBase,_stGameAttribute& stAttr)
{
	//属性转换 低转高
	for (uint8 uLayer = g_clBattleData.getMinLayer(); uLayer < g_clBattleData.getMaxLayer(); ++uLayer)
	{
		for (uint8 uType = _emBattleAttribute_MIN; uType <= _emBattleAttribute_MAX; ++uType)
		{
			const _stBattleAttrModel* pModel = g_clBattleData.find(uType);
			if (!pModel || pModel->uLayer != uLayer)
				continue;

			//增加万分比
			stAttr.updateAttribute(uType, int32(stAttrBase.getAttribute(uType) * (stAttr.getRadio(uType) / FLOAT_MAX_PROBABILITY)));
			stAttr.iRadio[uType] = 0;

			//该属性影响的其他属性
			for (uint32 i = 0; i < pModel->vecAffect.size(); ++i)
			{
				const _stBattleAttrAffect& affect = pModel->vecAffect[i];
				if (affect.uAttrType > _emBattleAttribute_MAX || affect.uAttrType < _emBattleAttribute_MIN)
					continue;

				int32 nUpdate = getConverValue(stAttr.iAttribute[uType], affect.nAffectType, affect.fAffectValue);
				stAttr.updateAttribute(affect.uAttrType, nUpdate);
			}
		}
	}
}

//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::calculateAttributeBase()
{
	m_stGameAttributeBase.initGameAttribute();
	MAP_ATTR::iterator _pos = m_mapSystemAttribute.begin();
	MAP_ATTR::iterator _end = m_mapSystemAttribute.end();
	for (; _pos != _end; ++_pos)
	{
		//buff 不属于基础属性
		if (_pos->first == GameSystemType_Buff)
			continue;

		_stGameAttribute& attr = _pos->second;
		m_stGameAttributeBase += attr;
	}

	attributeConvert(m_stGameAttributeBase,m_stGameAttributeBase);

	//先转换 再计算计算战斗力
	callbackFightPower();
	calculateAttribute();
}

//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::calculateAttributeBuff()
{
	m_stGameAttributeBuff.initGameAttribute();
	_stGameAttribute* pGameAttribute = m_mapSystemAttribute.find_(GameSystemType_Buff);
	if (!pGameAttribute)
		return;
	
	m_stGameAttributeBuff = pGameAttribute->getGameAttribute();

	attributeConvert(m_stGameAttributeBase,m_stGameAttributeBuff);
	calculateAttribute();
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::calculateAttribute()
{
	m_stGameAttribute.initGameAttribute();
	m_stGameAttribute += m_stGameAttributeBase;
	m_stGameAttribute += m_stGameAttributeBuff;

	if (m_iMaxHP != m_stGameAttribute.getAttribute(BattleAttribute_HPMax))
	{
		m_iMaxHP = m_stGameAttribute.getAttribute(BattleAttribute_HPMax);
		setGhost(_BIT32(Ghost_HPMP));
	}

	if (m_uWalkSpeedAttr != getAttribute(BattleAttribute_WalkSpeed) || 
		m_uRunSpeedAttr != getAttribute(BattleAttribute_RunSpeed) )
	{
		m_uWalkSpeedAttr = getAttribute(BattleAttribute_WalkSpeed);
		m_uRunSpeedAttr	 = getAttribute(BattleAttribute_RunSpeed);
		setGhost(_BIT32(Ghost_Speed));
	}
	setGhost(_BIT32(Ghost_Property),true,true);
}

//-------------------------------------------------------------
//------------------------------ 获得移动速度（毫秒）
float32	CSceneUnit::getMoveSpeed(uint8 uMoveType)const
{
	// 每秒的移动速度 = 移动速度属性 / 1000;
	float32 fSpeed = float32(getMoveSpeedAttr(uMoveType)/1000.f);

	// 转换成毫秒的速度
	fSpeed = float32(fSpeed / 1000.f);
	return fSpeed;
}

//-------------------------------------------------------------
//------------------------------
int32	CSceneUnit::getMoveSpeedAttr	(uint8 uMoveType)const
{
	int32 nMoveSpeedAttr = 0;
	switch (uMoveType)
	{
	case MoveType_Walk:
		{
			nMoveSpeedAttr = getAttribute(BattleAttribute_WalkSpeed);
		}
		break;
	case MoveType_Run:
		{
			nMoveSpeedAttr = getAttribute(BattleAttribute_RunSpeed);
		}
		break;
	case MoveType_Rush:
		{
			nMoveSpeedAttr = g_clConstantData.Game_RushSpeed;
		}
		break;
	default:
		break;
	}
	return nMoveSpeedAttr;
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::enterScene(CGameSceneCell*pCell)
{
	m_pSceneCell = pCell;
	CGhostObject::clearGhost();
}

//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::leaveScene()
{
	getHatredMgr().onLeaveScene();
	//清除ghost
	CGhostObject::clearGhost(true);

	sceneHave(false);

	m_pSceneHave	= NULL;

	//从场景中移除
	if(m_pSceneBlock)//从场景块中移除
		m_pSceneBlock->erase(this);
	m_pSceneCell	= NULL;
	m_pSceneBlock	= NULL;

	m_bRegister	= false;
}

//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::sceneHave	(bool _increase)
{
	if(!m_pSceneHave)
		return;

	if(_increase)
		m_pSceneHave->increase(getUnitType());
	else
		m_pSceneHave->reduce(getUnitType());
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::process_move(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay)
{
	if(!m_pSceneCell || m_eMoveType == MoveType_Idle )
		return;

	float32	fDist	= m_fMoveSpeed * (float32)(uTick - m_uBeginMoveTick);
	if (fDist<=0.1)
		return ;

	m_fDirection	= m_fMoveDirection;
	_stPoint2F target;
	getTargetPoint(m_stBeginMovePosition,m_fMoveDirection,fDist,target);
	if ( m_pSceneCell->isPassable(m_stBeginMovePosition,target))
	{
		callbackPostionChange();
		setPosition(target.x,target.y,SetPositionReason_Move,false);
		float32 fCurMoveSpeed = getMoveSpeed(m_eMoveType);
		if (!isFloatEqual(m_fMoveSpeed,fCurMoveSpeed))
		{
			m_fMoveSpeed			= fCurMoveSpeed;
			m_stBeginMovePosition	= getPosition();
			m_uBeginMoveTick		= uTick;
			setGhost(_BIT32(Ghost_Move));
		}
	}else if (m_pUnitAI)
	{
		endMove();
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::processAI			(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay)
{
	if (!m_pUnitAI )
		return ;

	m_pUnitAI->process(uTick, uTime, uSecond);
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::processFight		(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay)
{
	if ( getHatredMgr().hasHatred() )
		enterFight(EnterFightReason_Hatred);
	if ( getHatredMgr().hasBeHatred() )
		enterFight(EnterFightReason_BeHatred);

	if ( isFighting() )
	{
		if (asMonster())
		{
			if ( !getHatredMgr().hasHatred() && !getHatredMgr().hasBeHatred() )
				leaveFight();
		}
		else if(m_uFightingEndTime < uTime)
		{
			leaveFight();
		}
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::process(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay)
{
	if(!m_pSceneCell)
		return;


	m_clGameSkillMgr.process(uTick);
	process_move(uTick,uTime,uSecond,bNewDay);
	processUpdateValue(uTime);
	processEnergy(uTick,uTime,uSecond,bNewDay);
	processAttack(uTick);
	processLogic(uTick,uTime,uSecond,bNewDay);
	processAI(uTick,uTime,uSecond,bNewDay);
	processFight(uTick,uTime,uSecond,bNewDay);
	//ghost对像移除更新
	clearRemove();
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::collectTempSkill	(MAP_U16_U8& mapSkill)
{

}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::processUpdateValue	(uint64 uTime)
{
	//更新临时技能
	if(m_bUpdateValue[_UV_TempSkill])
	{
		m_bUpdateValue[_UV_TempSkill] = false;
		m_clGameSkillMgr.updateTempSkill();
	}

	//更新被动技能增加的buff
	if(m_bUpdateValue[_UV_PassiveSkillBuff])
	{
		m_bUpdateValue[_UV_PassiveSkillBuff] = false;
		m_clGameBuffMgr.updatePassiveSkillBuff();
	}

	//更新计算属性
	if(m_bUpdateValue[_UV_AttributeBase])
	{
		m_bUpdateValue[_UV_AttributeBase] = false;
		calculateAttributeBase();
	}

	//更新计算属性
	if(m_bUpdateValue[_UV_AttributeBuff])
	{
		m_bUpdateValue[_UV_AttributeBuff] = false;
		calculateAttributeBuff();
	}
}
//-------------------------------------------------------------
//------------------------------
bool	CSceneUnit::canRunAI			()
{
	return true;
}
//-------------------------------------------------------------
//------------------------------
void CSceneUnit::processEnergy(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay)
{
	if (uTick < m_uLastEnergyTick + 300)
		return;

	m_uLastEnergyTick = uTick;
	CHECK(m_pJobModel != NULL);
	const MAP_I32_I32* pMap = isFighting() ? &m_pJobModel->mapFighting : &m_pJobModel->mapNotFighting;
	if (pMap && !pMap->empty())
	{
		CONST_ITER_MAP_I32_I32 _pos = pMap->begin();
		CONST_ITER_MAP_I32_I32 _end = pMap->end();
		for (; _pos != _end; ++_pos)
		{
			updateEnergy(_pos->first, _pos->second, DoingType_UpdateEnergy);
		}
	}

	{//定时恢复能量
		_stJobModel::ITER_MAP_ENERGY_RECOVERY _pos = m_pJobModel->mapTimeAddEnegy.begin();
		_stJobModel::ITER_MAP_ENERGY_RECOVERY _end = m_pJobModel->mapTimeAddEnegy.end();
		for (; _pos != _end; ++_pos)
		{
			_stEnergyRecovery& stRecovery = m_mapEnergyRecovery[_pos->first];
			if (stRecovery.uTick + _pos->second.uStepTick < uTick)
			{
				stRecovery.uTick = uTick;
				updateEnergy(_pos->first, _pos->second.nAddValue, DoingType_UpdateEnergy);
			}
		}
	}
}

//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::processLogic(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay)
{
	//吟唱处理
	m_clSinging.process(uTick);
	//使用技能处理
	m_clSkilling.process(uTick);
	//buffer处理
	m_clGameBuffMgr.process(uTick,uTime,uSecond);
}

//-------------------------------------------------------------
//------------------------------ 更新区域
void	CSceneUnit::updateRange(bool bMove)
{
	CGameScene*pScenemap = getSceneMap();
	if (!pScenemap)
		return;

	_stGridRegionInfo* pRegion	= pScenemap->findGridRegion(getPosition());
	if(pRegion != m_pRegion)
	{
		//离开区域回调
		triggerRegionEvent(RegionTriggerType_Leave);
		//设置区域
		m_pRegion = pRegion;

		uint32 uNotTrigger = 0;
		if ( !bMove )
		{
			uNotTrigger |= _BIT32(RegionEventType_Transfer);
			uNotTrigger |= _BIT32(RegionEventType_Move);
		}
		///进入区域回调
		if (!haveState(StateType_WaitReady))
			triggerRegionEvent(RegionTriggerType_Enter,uNotTrigger);
	}
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnit::packetGhost(uint64 uTime,bool bSelf,uint32& updateMark,PBGhostInfo&pbInfo)
{
	if(_CHECK_BIT(updateMark,_BIT32(Ghost_Remove)))
		return false;

	PBGhostUnit& pbUnit= *pbInfo.mutable_unit();
	pbUnit.set_unittype(m_stUnitID.uUnitType);
	pbUnit.set_unitid(m_stUnitID.uUnitID);

	bool bSend = false;
	/*基础属性*/
	if(_CHECK_BIT(updateMark,_BIT32(Ghost_Property)) && bSelf)
	{
		bSend = true;
		PBGhostGameAttr* pInfo = pbUnit.mutable_attr();
		if (pInfo)
		{
			for ( int32 i = 0 ; i < _emBattleAttribute_ARRAYSIZE ; ++i )
			{
				pInfo->add_attr(m_stGameAttributeBase.iAttribute[i]);
			}
		}
	}
	_SET_BIT32(updateMark,Ghost_Property,false);

	/*生命与魔法*/
	if(_CHECK_BIT(updateMark,_BIT32(Ghost_HPMP)))
	{
		bSend = true;

		PBGhostHPMP* pInfo = pbUnit.mutable_hpmp();
		if (pInfo)
		{
			pInfo->set_curhp(m_iCurrHP);
			pInfo->set_maxhp(getHPMax());

			MAP_ENERGY::const_iterator _pos = m_mapEnergy.begin();
			MAP_ENERGY::const_iterator _end = m_mapEnergy.end();
			for(; _pos != _end; ++_pos)
			{
				PBEnergy* pEnergy = pInfo->add_curenergy();
				if (!pEnergy)
					continue;

				pEnergy->set_type(_pos->first);
				pEnergy->set_value(_pos->second);
			}
		}
	}
	_SET_BIT32(updateMark,Ghost_HPMP,false);

	/*位移*/
	if(_CHECK_BIT(updateMark,_BIT32(Ghost_Move)) && !bSelf )
	{
		bSend = true;

		PBGhostMove* pInfo = pbUnit.mutable_move();
		if (pInfo)
		{
			packetMove(*pInfo);
		}
	}
	_SET_BIT32(updateMark,Ghost_Move,false);

	/*状态*/
	if(_CHECK_BIT(updateMark,_BIT32(Ghost_State)))
	{
		bSend = true;

		uint32 stateFlag = 0;
		for (uint16 i = 0; i < _emGameStateType_ARRAYSIZE; i++)
		{
			if (m_uStateFlag[i])
				_SET_BIT32(stateFlag,i,m_uStateFlag[i]);	
		}
		pbUnit.set_state(stateFlag);

	}
	_SET_BIT32(updateMark,Ghost_State,false);

	//轻功
	if (_CHECK_BIT(updateMark,_BIT32(Ghost_Qinggong)))
	{
		if (haveState(StateType_Qinggong))
		{
			//
			float32 fX;
			float32 fY;
			uint32  uNextIndex;
			uint32	uMs2NextPoint;
			if (getSceneMap()
				&& getSceneMap()->findQinggongPoint(m_uQinggongID, (uint32)(getSystemTick64()-m_uQinggongStartTick), uNextIndex, uMs2NextPoint, fX, fY))
			{
				PBGhostQinggong * pbQinggong = pbUnit.mutable_qinggong();
				if (pbQinggong )
				{
					pbQinggong->set_id(m_uQinggongID);
					pbQinggong->set_nextindex(uNextIndex);
					pbQinggong->set_ms2jump(uMs2NextPoint);
				}
				setPosition(fX,fY,SetPositionReason_Qinggong,false);
			}
		}
	}
	_SET_BIT32(updateMark,Ghost_Qinggong,false);

	/*阵营*/
	if(_CHECK_BIT(updateMark,_BIT32(Ghost_Camp)))
	{
		bSend = true;
		pbUnit.set_campid(m_uCampID);
	}
	_SET_BIT32(updateMark,Ghost_Move,false);

	/*速度*/
	if(_CHECK_BIT(updateMark,_BIT32(Ghost_Speed)) && bSelf )
	{
		bSend = true;
		PBGhostSpeed* pInfo = pbUnit.mutable_speed();
		if (pInfo)
		{
			pInfo->set_walkspeed(m_uWalkSpeedAttr);
			pInfo->set_runspeed(m_uRunSpeedAttr);
		}
	}
	_SET_BIT32(updateMark,Ghost_Speed,false);

	return bSend;
}
//-------------------------------------------------------------
//------------------------------ 
bool			CSceneUnit::canMove				()
{
	if ( checkStateBuffAttr(StateBuffAttr_ForbidMove) )
			return false;

	if ( haveState(StateType_Dead) || haveState(StateType_Plane) 
		|| haveState(StateType_WaitReady) || haveState(StateType_Hold)
		|| haveState(StateType_Dart) || haveState(StateType_Qinggong)
		)
		return false;

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool	CSceneUnit::checkMove			(float32 fPosX,float32 fPosY,float32 fDir)
{
	if ( !canMove() )
		return false;

	if (!m_pSceneCell/* || !m_pSceneCell->canMove(fPosX,fPosY)*/)//移动不限制可行区域限制
		return false;

	_stPoint2F tar;
	tar.x = fPosX;
	tar.y = fPosY;
	if (getDistance(getPosition(),tar) >= 10.0)
		return false;

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnit::beginMove			(uint8 uMoveType,float32 fPosX,float32 fPosY,float32 fDir)
{
	if ( !checkMove(fPosX,fPosY,fDir) )
	{
		breakMove();
		return false;
	}

	setMoveState(uMoveType,fPosX,fPosY,fDir);

	//打断非蓄力技能技能
	if (SingState_Ing == getSinging().getSingState() && getSkilling().getSkillModel() && !getSkilling().getSkillModel()->check_mark(SkillMark_Charge))
		getSinging().breakSinging(false);

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnit::beginMove			(uint8 uMoveType,float32 fDir)
{
	return beginMove(uMoveType,getPosition().x,getPosition().y,fDir);
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnit::endMove				(float32 fPosX,float32 fPosY,float32 fDir)
{
	if ( !checkMove(fPosX,fPosY,fDir) )
	{
		breakMove();
		return false;
	}
	setMoveState(MoveType_Idle,fPosX,fPosY,fDir);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool			CSceneUnit::endMove				()
{
	if (!isMoving())
		return true;
	const _stPoint2F& ctCurPos = getPosition();
	return endMove(ctCurPos.x,ctCurPos.y,getDirection());
}
//-------------------------------------------------------------
//------------------------------
bool			CSceneUnit::canDirection		()
{
	if ( checkStateBuffAttr(StateBuffAttr_ForbidDirection) )
		return false;

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnit::changeDiretion				(float32 fPosX,float32 fPosY,float32 fDir)
{
	if (!canDirection())
		return false;

	setDirection(fDir);
	setGhost(_BIT32(Ghost_Move));
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnit::packetMove			(PBGhostMove& move)
{
	move.set_movetype(getMoveType());
	move.set_movespeed( getMoveSpeedAttr(getMoveType()) );
	move.set_posx(getPosition().x);
	move.set_posy(getPosition().y);
	move.set_dir(getDirection());

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::setMoveState				(uint8 uMoveType,float32 fPosX,float32 fPosY,float32 fDir)
{
	setDirection(fDir);
	setPosition(fPosX,fPosY,SetPositionReason_Move,false);

	m_eMoveType				= (_emMoveType)uMoveType;
	m_uBeginMoveTick		= getSystemTick64();
	m_fMoveDirection		= fDir;
	m_fMoveSpeed			= getMoveSpeed(uMoveType);
	m_stBeginMovePosition	= getPosition();
	setGhost(_BIT32(Ghost_Move));
}

//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::setState		(uint16 uStateType,bool bFlag)
{
	if (uStateType >= _emGameStateType_ARRAYSIZE)
		return;
	m_uStateFlag[uStateType] = bFlag;

	if ( uStateType == StateType_Dead || uStateType == StateType_Fight  
		|| uStateType == StateType_Hold || uStateType == StateType_Dart
		|| uStateType == StateType_Dartcar_Rob || uStateType == StateType_Rest || uStateType == StateType_DrinkWine
		)
	{
		setGhost(_BIT32(Ghost_State));
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::setCampID (uint8 uCampID)
{
	if (uCampID == m_uCampID)
		return;
	m_uCampID = uCampID;
	setGhost(_BIT32(Ghost_Camp));
}
//-------------------------------------------------------------
//------------------------------ 是否有状态
bool	CSceneUnit::haveState		(_emGameStateType uStateType)
{
	if (uStateType >= _emGameStateType_ARRAYSIZE)
		return false;

	return m_uStateFlag[uStateType];
}

//-------------------------------------------------------------
//------------------------------ 删除状态
bool	CSceneUnit::delState		(_emGameStateType uStateType,bool bInterupt)
{
	if (uStateType >= _emGameStateType_ARRAYSIZE)
		return false;

	if ( m_uStateFlag[uStateType] )
	{
		setState(uStateType,false);
		onLeaveState(uStateType,bInterupt);
		return true;
	}
	return false;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnit::addState		(_emGameStateType uStateType)
{
	if (uStateType >= _emGameStateType_ARRAYSIZE)
		return false;

	if ( !m_uStateFlag[uStateType] )
	{
		setState(uStateType,true);
		onEnterState(uStateType);
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::breakMove			()
{
	if ( isMoving() )
	{//打断移动
		m_eMoveType = MoveType_Idle;
		setGhost(_BIT32(Ghost_Move));
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::breakUseSkill		()
{
	if (haveState(StateType_Skiling) || haveState(StateType_Singing))
	{
		const _stSkillModel* pModel = m_clSkilling.getSkillModel();
		if ( pModel )
		{
			uint8 uCode = 0;
			m_clSkilling.breakSkillAsk(pModel->uSkillID,false,uCode);
		}
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::onEnterState		(_emGameStateType uStateType)
{
	switch (uStateType)
	{
	case StateType_Plane:
		{
			refreshVisible();
			breakMove();
			breakUseSkill();
		}
		break;
	case StateType_Hide:
		{
			refreshVisible();
			breakUseSkill();
		}
		break;
	case StateType_Dead:
		{
			breakMove();
			breakUseSkill();
		}
		break;
	case StateType_Hold:
		{
			breakMove();
			breakUseSkill();
		}
		break;
	case StateType_Dart:
		{
		}
		break;
	case StateType_Dartcar_Rob:
		{

		}
		break;
	case StateType_Qinggong:
		{
			//先不发
			//setGhost(_BIT32(Ghost_Qinggong));
		}
		break;
	default:
		break;
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::onLeaveState		(_emGameStateType uStateType,bool bInterupt)
{
	switch (uStateType)
	{
	case StateType_Plane:
		{
			refreshVisible(false);
		}
		break;
	case StateType_Hide:
		{
			refreshVisible(false);
		}
		break;
	case StateType_Dart:
		{
		}
		break;
	default:
		break;
	}
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnit::testRegion(uint8 uAttrType)const
{
	if(!m_pRegion)
		return false;

	if (m_pRegion->testMark(uAttrType))
		return true;

	// 地图的默认属性
	return m_pSceneCell && m_pSceneCell->testRegion(uAttrType);
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnit::isInRegion				(uint16 uRegionID)
{
	if(!m_pRegion)
		return false;

	auto _pos = m_pRegion->vecRegion.begin();
	auto _end = m_pRegion->vecRegion.end();
	for ( ;_pos != _end;++_pos )
	{
		const _stGameRegion* pData = *_pos;
		if (pData && pData->uRegionID == uRegionID )
			return true;
	}

	return m_pSceneCell && m_pSceneCell->getDefaultRegion() == uRegionID;
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::onEnterRegion			(const _stGameRegion* pGameRegion)
{

}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::onLeaveRegion			(const _stGameRegion* pGameRegion)
{

}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::triggerGameEvent		(const _stGameRegion* pGameRegion,_emRegionTriggerType uTriggerType,uint32 uNotTrigger,uint32& uTriggeredEventMark)
{
	if ( uTriggerType == RegionTriggerType_Enter )
	{
		onEnterRegion(pGameRegion);
	}
	else if ( uTriggerType == RegionTriggerType_Leave )
	{
		onLeaveRegion(pGameRegion);
	}

	//触发当前区域事件
	for (uint32 i = 0;i < pGameRegion->vecEvent.size();i++)
	{
		const _stRegionEvent&stEvent = pGameRegion->vecEvent[i];
		if(stEvent.uTriggerType != uTriggerType || _CHECK_BIT(uNotTrigger,_BIT32(stEvent.uEventType)))
			continue;

		if(!canTriggerEvent(&stEvent))
			continue;

		if ( triggerEvent(&stEvent) )
		{
			_SET_BIT32(uTriggeredEventMark,stEvent.uEventType,true);  // 设置成已经触发了该类事件
		}
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::triggerRegionEvent		(_emRegionTriggerType uTriggerType,uint32 uNotTrigger)
{
	triggerGridEvent(m_pRegion,uTriggerType,uNotTrigger);
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::triggerGridEvent		(_stGridRegionInfo* pGridRegion,_emRegionTriggerType uTriggerType,uint32 uNotTrigger)
{
	if(!pGridRegion)
		return;

	// 从最顶层区域 向  下层区域触发
	uint32 uTriggeredEventMark = 0; // 已经触发了得事件列表
	VECTOR_GAME_REGION::reverse_iterator _pos = pGridRegion->vecRegion.rbegin();
	VECTOR_GAME_REGION::reverse_iterator _end = pGridRegion->vecRegion.rend();
	for (  ; _pos != _end ; ++_pos )
	{
		const _stGameRegion* pGameRegion = *_pos;
		triggerGameEvent(pGameRegion,uTriggerType,uNotTrigger,uTriggeredEventMark);

		// 在上层已经触发的事件 不会再下层触发
		uNotTrigger |= uTriggeredEventMark;
	}
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnit::canTriggerEvent	(const _stRegionEvent*pEvent)
{
	if ( !pEvent)
		return false;

	switch ( pEvent->uEventType )
	{
	case RegionEventType_Transfer:
		{

		}
		break;
	default:
		break;
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnit::triggerEvent		(const _stRegionEvent*pEvent)
{
	return true;
}

//-------------------------------------------------------------
//------------------------------ 注册属性模块
_stGameAttribute* CSceneUnit::registerAttribute(_emGameSystemType systemType)
{
	MAP_ATTR::iterator pos = m_mapSystemAttribute.find(systemType);
	if (pos != m_mapSystemAttribute.end())
		return &pos->second;

	pos = m_mapSystemAttribute.insert_(systemType, _stGameAttribute()).first;
	return &pos->second;
}
//-------------------------------------------------------------
//------------------------------ 
_stGameAttribute* CSceneUnit::getAttribute(_emGameSystemType systemType)
{
	MAP_ATTR::iterator pos = m_mapSystemAttribute.find(systemType);
	if (pos != m_mapSystemAttribute.end())
		return &pos->second;

	return NULL;
}
//-------------------------------------------------------------
//------------------------------ 
int32 CSceneUnit::getAttributeSingle	(_emGameSystemType systemType, uint8 uAttrType)
{
	_stGameAttribute* pAttr = getAttribute(systemType);
	if (pAttr)
	{
		return pAttr->getAttribute(uAttrType);
	}
	return 0;
}
//-------------------------------------------------------------
//------------------------------ 
bool CSceneUnit::addAttribute(_emGameSystemType systemType,uint16 uAttrType,int32 iValue,int32 iRadio,_emDoingType emDoing)
{
	CHECKF(uAttrType >= _emBattleAttribute_MIN && uAttrType <= _emBattleAttribute_MAX);
	_stGameAttribute* pGameAttribute = getAttribute(systemType);
	if (!pGameAttribute)
		return false;

	pGameAttribute->iAttribute[uAttrType] += iValue;
	pGameAttribute->iRadio[uAttrType] += iRadio;

	if (GameSystemType_Buff == systemType)
	{
		setUpdateValue(_UV_AttributeBuff);
	}
	else
	{
		setUpdateValue(_UV_AttributeBase);
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool CSceneUnit::isExpendFull(uint16 uExpendType)
{
	CHECKF(uExpendType < _emExpendType_ARRAYSIZE && uExpendType > 0);
	bool bFull = false;
	switch (uExpendType)
	{
	case ExpendType_HP:
		bFull = getHP() >= getHPMax();
		break;
	}

	return bFull;
}
//-------------------------------------------------------------
//------------------------------ 
uint64 CSceneUnit::getExpend(uint16 uExpendType)
{
	CHECKF(uExpendType < _emExpendType_ARRAYSIZE && uExpendType > 0);
	uint64 uValue = 0;
	switch (uExpendType)
	{
	case ExpendType_Level:
		uValue = getLevel();
		break;
	case ExpendType_HP:
		uValue = getHP();
		break;
	case ExpendType_EnergyAnger:
	case ExpendType_EnergyVigor:
	case ExpendType_EnergyPower:
	case ExpendType_EnergyHit:
	case ExpendType_EnergyDark:
	case ExpendType_EnergyCrystal:
	case ExpendType_EnergyPsych:
	case ExpendType_Roll:
		uValue = getEnergy(getEnergyByExpend(uExpendType));
		break;
	default:
		break;
	}

	return uValue;
}
//-------------------------------------------------------------
//------------------------------ 
bool CSceneUnit::canExpend(uint16 uExpendType,uint64 uValue)
{
	CHECKF(uExpendType < _emExpendType_ARRAYSIZE && uExpendType > 0 && uValue != 0);
	return getExpend(uExpendType) >= uValue;
}
//-------------------------------------------------------------
//------------------------------ 
bool CSceneUnit::canExpend(const MAP_U16_I64& mapExpend)
{
	bool bRet = true;
	CONST_ITER_MAP_U16_I64 _pos = mapExpend.begin();
	CONST_ITER_MAP_U16_I64 _end = mapExpend.end();
	for (; _pos != _end; ++_pos)
	{
		if (_pos->second >= 0)
			continue;
		if (!canExpend(_pos->first,uint64(_pos->second * -1)))
		{
			bRet = false;
			break; 
		}
	}

	return bRet;
}

//-------------------------------------------------------------
//------------------------------ 
bool CSceneUnit::addExpend(const MAP_U16_I64& mapExpend,_emDoingType emDoing,CSceneUnit* pSource/* = NULL*/)
{
	if (mapExpend.empty())
		return true;

	bool bRet = true;
	CONST_ITER_MAP_U16_I64 _pos = mapExpend.begin();
	CONST_ITER_MAP_U16_I64 _end = mapExpend.end();
	for (; _pos != _end; ++_pos)
	{
		bool bTmp = addExpend(_pos->first,_pos->second,emDoing,pSource);
		bRet &= bTmp;
	}

	return bRet;
}

//-------------------------------------------------------------
//------------------------------ 
bool CSceneUnit::addExpend(uint16 uExpendType,int64 nValue,_emDoingType emDoing,CSceneUnit* pSource/* = NULL*/)
{
	CHECKF(uExpendType < _emExpendType_ARRAYSIZE && uExpendType > 0 && nValue != 0);
	bool bRet = false;
	uint64 uOldValue = getExpend(uExpendType);
	switch (uExpendType)
	{
	case ExpendType_HP:
		bRet = updateHP((int32)nValue,pSource,emDoing);
		break;
	case ExpendType_EnergyAnger:
	case ExpendType_EnergyVigor:
	case ExpendType_EnergyPower:
	case ExpendType_EnergyHit:
	case ExpendType_EnergyDark:
	case ExpendType_EnergyCrystal:
	case ExpendType_EnergyPsych:
	case ExpendType_Roll:
		bRet = updateEnergy(getEnergyByExpend(uExpendType),(int32)nValue,emDoing);
		break;
	default:
		break;
	}

	uint64 uNowValue = getExpend(uExpendType);
	callbackChangeExpend(uExpendType,int32(uNowValue - uOldValue));
	return bRet;
}
//-------------------------------------------------------------
//------------------------------ 获取能量上限
uint32 CSceneUnit::getMaxEnergy		(uint8 uType)
{
	uint8 uMaxType = 0;
	switch (uType)
	{
	case BattleAttribute_EnergyAnger:
		uMaxType = BattleAttribute_MaxEnergyAnger;
		break;
	case BattleAttribute_EnergyVigor:
		uMaxType = BattleAttribute_MaxEnergyVigor;
		break;
	case BattleAttribute_EnergyPower:
		uMaxType = BattleAttribute_MaxEnergyPower;
		break;
	case BattleAttribute_EnergyHit	:
		uMaxType = BattleAttribute_MaxEnergyHit;
		break;
	case BattleAttribute_EnergyDark :
		uMaxType = BattleAttribute_MaxEnergyDark;
		break;
	case BattleAttribute_EnergyCrystal:
		uMaxType = BattleAttribute_MaxEnergyCrystal;
		break;
	case BattleAttribute_EnergyPsych:
		uMaxType = BattleAttribute_MaxEnergyPsych;
		break;
	case BattleAttribute_EnergyRoll:
		uMaxType = BattleAttribute_MaxEnergyRoll;
		break;
	default:
		break;
	}
	uint32 uMax = g_clBattleData.getAttributeRange(uMaxType,false);
	uMax = MIN((uint32)getAttribute(uMaxType),uMax);
	return uMax;
}
//-------------------------------------------------------------
//------------------------------ 能量更新
bool CSceneUnit::updateEnergy(uint8 uType, int32 nValue,_emDoingType emDoing)
{
	//职业特例
	if (getJob() == GameJob_All || !nValue)
		return true;

	MAP_ENERGY::iterator _pos = m_mapEnergy.find(uType);
	if (_pos == m_mapEnergy.end())
		return false;

	int32 iOldValue = _pos->second;
	//怒气恢复每秒限制
	if (uType == BattleAttribute_EnergyAnger && nValue > 0)
	{
		uint64 uTick = (uint64)getTime();
		_stEnergyRecovery& stRecovery = m_mapEnergyRecovery[uType];
		if (stRecovery.uTick != uTick)
		{
			stRecovery.uTick	= uTick;
			stRecovery.iValue	= 0;
		}

		nValue = MIN(nValue, 100 - stRecovery.iValue);
		stRecovery.iValue += nValue;
	}

	if (!nValue)
		return true;
	

	//达到最大值直接返回
	uint32 uMaxValue = getMaxEnergy(uType);
	if (nValue + _pos->second < 0)
	{
		_pos->second = 0;
	}
	else
	{
		_pos->second = MIN((int32)uMaxValue,_pos->second + nValue);
	}

	if (iOldValue != _pos->second)
		setGhost(_BIT32(Ghost_HPMP));
	return true;
}

//-------------------------------------------------------------
//------------------------------ 注册能量
void CSceneUnit::registerEnergy(uint8 uType, int32 nCurValue)
{
	m_mapEnergy[uType] = nCurValue;
	setGhost(_BIT32(Ghost_HPMP));
}

//-------------------------------------------------------------
//------------------------------ 
void CSceneUnit::energyUseSkill(const _stSkillModel* pSkill)
{
	CHECK(pSkill && m_pJobModel);
	CONST_ITER_MAP_I32_I32 _pos = m_pJobModel->mapByAttack.begin();
	CONST_ITER_MAP_I32_I32 _end = m_pJobModel->mapByAttack.end();
	for (; _pos != _end; ++_pos)
	{
		updateEnergy(_pos->first, _pos->second,DoingType_Others);
	}
}

//-------------------------------------------------------------
//------------------------------ 
void CSceneUnit::energyByAttack()
{
	CHECK(m_pJobModel != NULL);

	CONST_ITER_MAP_I32_I32 _pos = m_pJobModel->mapByAttack.begin();
	CONST_ITER_MAP_I32_I32 _end = m_pJobModel->mapByAttack.end();
	for (; _pos != _end; ++_pos)
	{
		updateEnergy(_pos->first, _pos->second, DoingType_EnergyByAttack);
	}
}

//-------------------------------------------------------------
//------------------------------ 
void CSceneUnit::energyByHitEnermy(const _stSkillEffectModel* pSkillEffectModel)
{
	CHECK(m_pJobModel != NULL && pSkillEffectModel != NULL);

	CONST_ITER_MAP_U16_I32 _pos = m_pJobModel->mapHitAddEnergy.begin();
	CONST_ITER_MAP_U16_I32 _end = m_pJobModel->mapHitAddEnergy.end();
	for (; _pos != _end; ++_pos)
	{
		updateEnergy((uint8)_pos->first, _pos->second, DoingType_EnergyByHitEnermy);
	}
}

//-------------------------------------------------------------
//------------------------------ 
_stPoint2F CSceneUnit::getBrithPoint()
{
	return getPosition();
}
//-------------------------------------------------------------
//------------------------------ 
bool CSceneUnit::insertGhostObject(CSceneUnit*pObject,bool bRecursive /*= true*/)
{
	bool bResult = super::insertGhostObject(pObject, bRecursive);
	return bResult;
}

//-------------------------------------------------------------
//------------------------------ 
CSceneUnit*	CSceneUnit::getOwner()
{
	if (!m_pSceneCell || !m_uOwnerID)
		return this;

	return m_pSceneCell->findObject(m_uOwnerID);
}
//-------------------------------------------------------------
//------------------------------ 
CScenePlayer*	CSceneUnit::getOwnerPlayer	()
{
	CSceneUnit* pOwner = getOwner();
	if (!pOwner)
		return NULL;

	return pOwner->asPlayer();
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::setOwner(CSceneUnit* pOwner)
{
	m_uOwnerID	= 0;
	m_uOwnerPlayerID = 0;
	m_strOwnerPlayerName.clear();
	if ( pOwner && pOwner->getUnitID() != getUnitID() )
	{
		m_uOwnerID = pOwner->getUnitID();
		if (pOwner->asPlayer())
		{
			m_uOwnerPlayerID = pOwner->getPlayerID();
			m_strOwnerPlayerName = pOwner->getName();
			setGhost(_BIT64(Ghost_Base));
		}
	}
}
//-------------------------------------------------------------
//------------------------------
void	CSceneUnit::checkRefreshKillTime(CSceneUnit* pBeAtkUnit)
{
	if ( !pBeAtkUnit )
		return ;

	CSceneUnit* pAtkOwner = getOwner();
	if ( !pAtkOwner )
		return ;

	CScenePlayer* pAtker = pAtkOwner->asPlayer();
	if ( !pAtker )
		return ;

	CSceneUnit* pBeAtkOwner = pBeAtkUnit->getOwner();
	if ( !pBeAtkOwner )
		return ;

	CScenePlayer* pBeAtker = pBeAtkOwner->asPlayer();
	if ( !pBeAtker )
		return ;

	if (!pAtker->canAddPKValue(pBeAtker))
		return ;

	pAtker->refreshKillTime();
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::callbackHurt			(CSceneUnit* pUnit,int32 iHurt)
{
	if (!pUnit)
		return;

	CScenePlayer* pPlayer = pUnit->asPlayer();
	if (pUnit->getOwner())
		pPlayer = pUnit->getOwner()->asPlayer();

	m_clhurtMgr.updateHurt(pPlayer,iHurt);

	if (m_pSceneCell)
	{
		CCopymapBase* pCopymap = m_pSceneCell->getGameCopymap();
		if (pCopymap)
		{
			pCopymap->callback_player_gethurt(asPlayer(), iHurt, pPlayer);
		}
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::callbackBeKillHarm		(CSceneUnit* pUnit,int32& iDamage)
{
	m_clGameSkillMgr.callbackBeKillHarm		(pUnit,iDamage);
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::callbackAttack		(CSceneUnit* pUnit,int32 iDamage,bool bCritical)
{
	if (!pUnit || pUnit == this)
		return;

	enterFight(pUnit->getOwnerPlayer() != NULL ? EnterFightReason_AttackPlayer : EnterFightReason_AttackMonster);

	checkRefreshKillTime(pUnit);

	if ( iDamage > 0 && getHP() > 0  )
	{
		int32 nSuckHP = (int32) (iDamage * m_clGameBuffMgr.getSuckHPRate(bCritical));
		updateHP(nSuckHP,pUnit, DoingType_Others);
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::callbackByAttack		(CSceneUnit* pUnit,int32 iDamage)
{
	if(!pUnit || pUnit == this)
		return;

	enterFight(pUnit->getOwnerPlayer() != NULL ? EnterFightReason_BeAttackPlayer : EnterFightReason_BeAttackMonster);
	energyByAttack();
	if (m_pUnitAI)
		m_pUnitAI->callbackByAttack(pUnit,iDamage);
}

//-------------------------------------------------------------
//------------------------------
void	CSceneUnit::callbackCritical	()
{
	m_clGameBuffMgr.callbackCritical();

	//判断是否有暴击
	const _stSkillModel* pSkillModel = getSkilling().getSkillModel();
	if (pSkillModel)
	{
		const _stSkillSpareParam* pParam = pSkillModel->findParam(SkillSpareParamType_criticalAddBuff);
		if (pParam && pParam->vecParams.size() > 2)
		{
			uint32 uRate = (uint32)pParam->vecParams[0];
			uint32 uBuffID = (uint32)pParam->vecParams[1];
			if ((uint32)randRange(10000) <= uRate)
			{
				m_clGameBuffMgr.addBuff(uBuffID,pSkillModel->uLevel,this->getUnitID(),pSkillModel->uSkillID, DoingType_Others);
			}
		}
	}


}

//-------------------------------------------------------------
//------------------------------
void	CSceneUnit::callbackUseSkill	(const _stSkillModel*pSkillModel,uint32 uTarUnitID)
{
	if (!pSkillModel)
		return;

	if ( pSkillModel->uUseType != SkillUseType_Proactive )
		return ;

	getGameBuffMgr().callbackUseSkill();
	getGameSkillMgr().callbackUseSkill(uTarUnitID);
	if(pSkillModel->uType != SkillType_MonsterCollletion)
	{
		enterFight(EnterFightReason_UseSkill);
	}
}
//-------------------------------------------------------------
//------------------------------
void	CSceneUnit::callbackUseSkillEnd	(const _stSkillModel*pSkillModel)
{
	if (!pSkillModel)
		return;

	getGameBuffMgr().callbackUseSkillEnd();
}
//-------------------------------------------------------------
//------------------------------
void	CSceneUnit::callbackSkillHit	(const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel)
{
	if (!pSkillModel || !pSkillEffectModel)
		return;

	//命中敌人
	if (TargetCategory_Enemy == pSkillModel->uTagetCategory)
	{
		//buff效果
		getGameBuffMgr().callbackSkillHit();
		getGameSkillMgr().callbackSkillHit();
		energyByHitEnermy(pSkillEffectModel);
	}
}
//-------------------------------------------------------------
//------------------------------
void	CSceneUnit::callbackSkillNotHit	(const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel)
{
	if (!pSkillModel || !pSkillEffectModel)
		return;

	getGameSkillMgr().callbackSkillNotHit();
}
//-------------------------------------------------------------
//------------------------------被技能效果命中
void	CSceneUnit::callbackBeSkillEffect	(CSceneUnit* pUnit,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel)
{
	if (!pUnit|| !pSkillModel || !pSkillEffectModel)
		return;
	
	//命中敌人
	if (TargetCategory_Enemy == pSkillModel->uTagetCategory)
	{
		//是否命中
		if (!this->check_mark(SceneUnitMark_SkillMiss))
		{
			getGameSkillMgr().callbackBeSkillHit(pUnit,pSkillModel,pSkillEffectModel);
			getGameBuffMgr().callbackBeSkillHit(pUnit,pSkillModel,pSkillEffectModel);
			
			//命中加仇恨
			if (pSkillEffectModel->nHatredValue)
				getHatredMgr().updateHatred(pUnit,pSkillEffectModel->nHatredValue);
		}
	}
}
//-------------------------------------------------------------
//------------------------------命中敌人
void	CSceneUnit::callbackSkillHitTarget	(CSceneUnit* pUnit,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel)
{
	
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::callbackDead		(CSceneUnit* pSource)
{
	//加死亡状态
	addState(StateType_Dead);
	getHatredMgr().onDead();
	getGameBuffMgr().callbackDead();
	delState(StateType_Hide);
	//中断
	if (pSource)
		pSource->callbackKillTarget(this);
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::callbackKillTarget		(CSceneUnit* pTarget)
{
	getSkilling().callbackKillTarget(pTarget);
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::callbackLife		()
{
	//取消死亡状态
	delState(StateType_Dead);
}

//-------------------------------------------------------------
//------------------------------ 
void CSceneUnit::gatherHaloVec(VEC_U16& haloVec)
{
	m_clGameSkillMgr.gatherHaloVec(haloVec);
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnit::isDead			()
{
	return haveState(StateType_Dead);
}

//-------------------------------------------------------------
//------------------------------ 
void CSceneUnit::callbackUpdateHP(int32 iHP)
{
	m_clGameSkillMgr.callbackUpdateHP();
}
//-------------------------------------------------------------
//------------------------------ 隐身回调,
//隐身是一种单向看不见的关系,进入隐身以后,对方看不见我,我可以看见对方
//如果进入隐身,那么要给  对方玩家发送ghost消失,      给对方怪物发送仇恨消除
//如果取消隐身,那么要给  对方玩家发送完整ghost消息,  给对方怪物增加仇恨列表
//void	CSceneUnit::callbackHide(bool bHide)

void	CSceneUnit::refreshVisible(bool bHide/* = true*/)
{
	MAP_GHOST::iterator _pos = m_mapGhost.begin();
	MAP_GHOST::iterator _end = m_mapGhost.end();
	for (;_pos != _end;++_pos)
	{
		_stGhost&ghost = _pos->second;
		CSceneUnit* pUnit = ghost.pObject;
		if ( !pUnit )
			continue;

		bool bRealHide	= ! (pUnit->isVisible(this) );
		if ( bRealHide )
		{
			CScenePlayer* pPlayer = pUnit->asPlayer();
			if ( !pPlayer )
				continue;

			PBG2CCommonUnitRemove ack;
			ack.set_unittype(getUnitType());
			ack.set_unitid(getUnitID());
			pPlayer->sendMsg(P_S2C_Protocol_Common,S2C_Common_Unit_Remove,&ack);
		}
		else
		{
			_stGhost* pSelfGhost = pUnit->findGhost(getUnitID());
			if (!bHide && pSelfGhost && !_CHECK_BIT(pSelfGhost->updateMark,_BIT32(Ghost_Remove)))
			{
				pSelfGhost->updateMark = 0xffffffff;
				_SET_BIT32(pSelfGhost->updateMark,Ghost_Remove,false);
			}
		}
	}

	getHatredMgr().onRefreshVisible();
}
//-------------------------------------------------------------
//------------------------------ 战斗间隔时间
void	CSceneUnit::callbackFight()
{
	
}
//-------------------------------------------------------------
//------------------------------
bool		CSceneUnit::canUpdateHp			(int32 iHP)
{
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnit::updateHP	(int32 iHP,CSceneUnit* pSource,_emDoingType emDoing)
{
	if (!pSource)
		pSource = this;

	if (iHP == 0 || !canUpdateHp(iHP) )
		return false;

	int iOldHP	= m_iCurrHP;
	int iCurrHP = m_iCurrHP;
	if (emDoing == DoingType_Buff)
	{
		if ( iHP > 0 )
		{
			iHP += (int32) (iHP * m_clGameBuffMgr.getAddCureRate());
			getHatredMgr().onByCure_Buff(pSource,iHP);
		}
		else
		{
			if (checkStateBuffAttr(StateBuffAttr_ForbidDamage))
				return true;

			iHP += (int32) (iHP * m_clGameBuffMgr.getAddHarmRate(0));
			if ( iHP == 0 )
				iHP = -1;

			getHatredMgr().onByDamage_Buff(pSource,-iHP);
		}
	}

	if(!updateInt(iCurrHP,iHP,getHPMax()))
		return false;

	if (checkStateBuffAttr(StateBuffAttr_ForbidDead) && iCurrHP <= 0)
	{
		iCurrHP = 1;
		if (iOldHP == iCurrHP)
			return false;
	}

	setHP(iCurrHP);
	callbackUpdateHP(iCurrHP - iOldHP);
	if (iHP<0)
		callbackHurt(pSource,iHP * -1);
	///无生命死亡
	if(m_iCurrHP <= 0 && iOldHP > 0)
	{
		callbackDead(pSource->getOwner());
	}
	else if(iOldHP<= 0 && m_iCurrHP > 0)
	{
		callbackLife();
	}

	//同步
	if (emDoing == DoingType_Buff 
		&& iOldHP == iCurrHP)
	{
		//buff加血如果已经满了, 不加.
	}
	else
	{
		CGameBuffMgr::send_BuffHpAck(iHP,this,pSource,emDoing);
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::setNewPosition(CSceneUnit* pSource,float32 fDis,uint8 uDirType)
{
	if (!pSource || uDirType >= _emDirectionType_ARRAYSIZE)
		return;

	float32 fNewDir = getDirection();
	switch (uDirType)
	{
	case DirectionType_SkillSame:
		{
			fNewDir = pSource->getDirection();
		}
		break;
	case DirectionType_SkillDif:
		{
			fNewDir = pSource->getDirection() + M_PI_F;
		}
		break;
	case DirectionType_SkillTarSame://技能到目标的方向
		{
			fNewDir = get2PiRadian(pSource->getPosition(),getPosition());
		}
		break;
	case DirectionType_SkillTarDif://目标到技能的方向
	case DirectionType_SkillSource://移动到施法者点
		{
			fNewDir = get2PiRadian(getPosition(),pSource->getPosition());
		}
		break;
	case DirectionType_SkillVertical://远离技能垂直方向
		{
			float32 fRadian = get2PiRadian(pSource->getPosition(),getPosition());

			//弧度小于方向在左边
			float32 fSpare = get2PiRadian(get2PiRadian(fRadian) - get2PiRadian(pSource->getDirection()));
			if (fSpare > 0 && fSpare < Float_Pi)
			{
				fNewDir = pSource->getDirection() - Float_Pi/2;
			}
			else
			{
				fNewDir = pSource->getDirection() + Float_Pi/2;		
			}
		}
		break;
	default:
		break;
	}

	///都不改方向
	if (DirectionType_SkillSource == uDirType)
	{
		setPosition(pSource->getPosition(),SetPositionReason_Skill,false);
	}
	else
	{
		_stPoint2F stPoint(getPosition());
		getUnitTargetPoint(fNewDir,fDis,stPoint);
		setPosition(stPoint,SetPositionReason_Skill,false);
	}
	
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnit::isFighting			()
{
	return haveState(StateType_Fight);
}
//-------------------------------------------------------------
//------------------------------ 
void		CSceneUnit::enterFight			(_emEnterFightReason eReason)
{
	uint32 uEffectFightTime = 0;
	switch (eReason)
	{
	case EnterFightReason_UseSkill:
		uEffectFightTime = g_clConstantData.Game_FightTimeUseSkill;
		break;
	case EnterFightReason_AttackPlayer:
		uEffectFightTime = g_clConstantData.Game_FightTimeAttackPlayer;
		break;
	case EnterFightReason_BeAttackPlayer:
		uEffectFightTime = g_clConstantData.Game_FightTimeBeAttackPlayer;
		break;
	case EnterFightReason_AttackMonster:
		uEffectFightTime = g_clConstantData.Game_FightTimeAttackMonster;
		break;
	case EnterFightReason_BeAttackMonster:
		uEffectFightTime = g_clConstantData.Game_FightTimeBeAttackMonster;
		break;
	case EnterFightReason_Hatred:
		uEffectFightTime = g_clConstantData.Game_FightTimeHatred;
		break;
	case EnterFightReason_BeHatred:
		uEffectFightTime = g_clConstantData.Game_FightTimeBeHatred;
		break;
	default:
		break;
	}
	uEffectFightTime = MAX((uint32)1,uEffectFightTime);
	uint32 uCurTime = getTime();
	if(!m_uFightingEndTime || m_uFightingEndTime < uCurTime+uEffectFightTime )
		m_uFightingEndTime = uCurTime+uEffectFightTime;

	if (!isFighting())
	{
		m_uFightingBeginTime = uCurTime;
		addState(StateType_Fight);
		callbackFight();
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::leaveFight			()
{
	if (!isFighting())
		return ;

	delState(StateType_Fight);
	m_uFightingEndTime		= 0;
	m_uFightingBeginTime	= 0;
}
//-------------------------------------------------------------
//------------------------------ 
uint32	CSceneUnit::getFightTime	()
{
	if (!isFighting())
		return 0;
	return getTime() - m_uFightingBeginTime;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnit::isInCopymap			()
{
	if ( !m_pSceneCell )
		return false;

	return m_pSceneCell->isCopymap();
}
//-------------------------------------------------------------
//------------------------------ 
void CSceneUnit::callbackUseSkillFailedByBlock		(uint32 uSkillID)   //因为阻挡点的问题，导致技能使用失败
{
	if (m_pUnitAI)
	{
		m_pUnitAI->callbackUseSkillFailedByBlock(uSkillID);
	}
}
//-------------------------------------------------------------
//------------------------------ 
void CSceneUnit::callbackChangeExpend		(uint16 uExpendType,int32 iValue)
{
	m_clGameBuffMgr.callbackChangeExpend(uExpendType,iValue);
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnit::check_mark	(_emSceneUnitMark emMark,uint32 uValue/* = 0*/)
{
	uint32* pValue = m_mapMark.find_((uint16)emMark);
	if (!pValue || *pValue != uValue)
		return false;
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::update_mark	(_emSceneUnitMark emMark,uint32 uValue/* = 0*/)
{
	m_mapMark[emMark] = uValue;
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::delete_mark	(_emSceneUnitMark emMark,uint32 uValue/* = 0*/)
{
	m_mapMark.erase(emMark);
}
//-------------------------------------------------------------
//------------------------------ 
uint32	CSceneUnit::find_mark	(_emSceneUnitMark emMark)
{
	uint32* pValue = m_mapMark.find_((uint16)emMark);
	return pValue != NULL ? *pValue : 0;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnit::setPosition(const float32 x,const float32 y,_emSetPositionReason eReason,bool bSyn)
{
	_stPoint2F stPosition(x,y);
	return setPosition(stPosition,eReason,bSyn);
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneUnit::setPosition(const _stPoint2F&stPoint,_emSetPositionReason eReason,bool bSyn)
{
	m_stPosition = stPoint ;
	if (SetPositionReason_Build == eReason)
		return true;

	if ( bSyn )
	{
		PBG2CCommon_SetPosition msg;
		msg.set_unitid(getUnitID());
		msg.set_posx(m_stPosition.x);
		msg.set_posy(m_stPosition.y);
		msg.set_dir(getDirection());
		sendBroadcast(P_S2C_Protocol_Common,S2C_Common_SetPosition,&msg,0,false);
	}
	onSetPoint(eReason==SetPositionReason_Move);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::speak				(uint32 uSpeakID)
{

}
