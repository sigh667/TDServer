/*----------------- sceneTrap.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2016/2/23 10:30:19
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "sceneUnit.h"
#include "cfg_data/trapData/trapModel.h"
/*************************************************************/
class CSceneTrap	: public CSceneUnit
{
	enum _emCurMoveState
	{
		CurMoveState_None,

		CurMoveState_Move,			// 移动
		CurMoveState_Stop,			// 停止
	};
	enum _emCurMoveMethod
	{
		CurMoveMethod_None,
		CurMoveMethod_Straight,		// 直线
		CurMoveMethod_Track,		// 追踪
		CurMoveMethod_Spiral,		// 螺旋顺时针
		CurMoveMethod_SpiralRevert,	// 螺旋逆时针

	};
	struct _stTrapSkill
	{
		_stTrapSkill()
		{
			initTrap();
		}
		void	initTrap()				{	memset(this,0,sizeof(*this));	}


		const _stTrapSkillModel* pModel;// 技能模板
		uint64	uNextTrigerTick;		// 下一次触发技能时间
		uint32	uTrigerCount;			// 已经触发的次数
	};
	typedef	stl_vector<_stTrapSkill>	VEC_TRAP_SKILL;
	typedef	stl_set	<uint32>			SET_UNIT_ID;
private:
	typedef CSceneUnit Parent;
private:
	uint64				m_uBuildTick;	// 创建时间
	uint64				m_uExitTick;	// 消失时间点
	uint64				m_uDestroyTick;	// 销毁时间点 真正离开场景

	bool				m_bDelete;		// 是否删除
	const _stTrapModel*	m_pTrapModel;	// 陷阱模板

	// 技能相关
	VEC_TRAP_SKILL		m_vecTrapSkill;	// 当前技能
	SET_UNIT_ID			m_setUnitID;	// 已被打中过的目标

	// 移动相关
	uint64				m_uLastMoveTick;// 上一次移动时间
	uint64				m_uStopTick;	// 停止的时间点
	_stPoint2F			m_stStartPoint;	// 起始坐标
	_stPoint2F			m_stEndPoint;	// 终止目标坐标
	_stPoint2F			m_stPrePoint;	// 上一次的坐标
	uint64				m_uStartTick;	// 起始移动时间
	float32				m_fStartDir;	// 起始方向
	float32				m_fDistance;	// 最远飞行距离
	float32				m_fHigh;		// 高度
	uint32				m_uTarUnitID;	// 目标ID
	_emCurMoveState		m_eMoveState;	// 当前移动状态
	_emCurMoveMethod	m_eMoveMethod;	// 当前移动方式
	uint32				m_uMoveStage;	// 运行轨迹的阶段
	bool				m_bNoUsekill;	// 是否不能使用技能
public:
	CSceneTrap();
	virtual ~CSceneTrap();
public:
	virtual bool		initialize	();

public:
	virtual CSceneTrap*	asTrap			()	{	return this;	}
	virtual	bool		packetGhost		(uint64 uTime,bool bSelf,uint32& updateMark,PBGhostInfo&pbInfo);

	virtual void		processLogic	(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay);

	virtual	bool		canUseSkill		(CSceneUnit*pTarget);
	virtual	bool		canByUseSkill	(CSceneUnit*pAtker);
	virtual bool		isVisible		(CSceneUnit*pObject);	// 目标对自己是否可见 

	virtual void		callbackAttack	(CSceneUnit* pUnit,int32 iDamage,bool bCritical);
	virtual void		callbackSkillHitTarget	(CSceneUnit* pUnit,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	virtual void		callbackSkillHit(const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	virtual void		process_move	(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay);
public:
	/*--->[ 构建 ]*/
	bool				build			(CSceneUnit* pOwner,const _stTrapModel*pModel,const _stPoint2F&stBuildPos,float32 fDir,float32 fHigh,_stPoint2F*const pTargetPos = NULL,uint32 uTarUnitID = 0);
private:
	void				processMove		(uint64 uTick,uint64 uTime,uint32 uSecond);

	void				processMove_Move(uint64 uTick,uint64 uTime,uint32 uSecond);
	void				processMove_Stop(uint64 uTick,uint64 uTime,uint32 uSecond);
	float32				getFlyDistance	();
	void				onExit			();
	void				destroy			();


public:
	_stPoint2F			getEndPoint()	{return m_stEndPoint;}
};