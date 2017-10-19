/*----------------- skilling.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/1/26 9:54:19
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
#include "math/point2.h"
#include "gameLogic/singing/singing.h"
#include "cfg_data/skillData/skillModel.h"
#include "gameLogic/skilling/skillAnim.h"
/*************************************************************/
class CSceneUnit;
//-------------------------------------------------------------
//------------------------------ 
class CSkilling : public CSingObject
{
private:
	struct _stCompositeInfo
	{
		uint32	m_uNextSkill;	//下一个技能
		uint32	m_uBaseSkill;	//基础技能
		uint64	m_uExpireTick;	//等待输入时间

		_stCompositeInfo() {	memset(this,0,sizeof(*this)); }
	};
	typedef stl_map<uint32,_stCompositeInfo> MAP_COMPOSITEINFO;

private:
	CSceneUnit*			m_pOwner;			//归属者
	const _stSkillModel*m_pSkillModel;		//技能模板
	_stSkillAnimModel*	m_pSkillAnimModel;	//动作模板

	uint8				m_uCurStage;
	uint64				m_uStartAnimTick;	//开始动作毫秒
	uint64				m_uEndAnimTick;		//结束动作毫秒
	CSkillAnim			m_clSkillAnim;		//动作帧管理	
	uint8				m_uExpendCount;		//扣能量次数

	float32				m_fChargeRate;		//蓄力系数
	_stPoint2F			m_stOldPosition;	//位移前的地方 瞬移父子技能专用

	uint64				m_uLastCombatTick;  //上一段连击的时间
	uint32				m_uNextCombatSkill;	//下一段连击技能ID

	MAP_COMPOSITEINFO	m_mapChildSkill;	//父子技能

	uint32				m_uTarUnitID;		//有些技能需要取实时坐标
	SET_U32				m_setChooseTarget;	//已被选中目标
	SET_U32				m_setGatherTarget;	//搜集选中目标

public:
	CSkilling();

public:
	inline	bool					canOperate		()		{   return m_pSkillModel != NULL && m_pOwner != NULL;	}
	inline	float32					getChargeRate	()		{   return m_fChargeRate;	}
	inline	const _stSkillModel*	getSkillModel	()const	{   return m_pSkillModel;	}
	inline	bool					haveChoose		(uint32 uUnitID)const	{   return m_setChooseTarget.is_exist(uUnitID);	}
	inline	void					addChooseTarget	(uint32 uUnitID)	{   m_setChooseTarget.insert(uUnitID);				}
	inline	void					addGatherTarget	(uint32 uUnitID)	{   m_setGatherTarget.insert(uUnitID);				}
	
public:
	void				initialize				(CSceneUnit* pOwner, const _stSkillModel*	pSkillModel);
	void				beginTime				();
	uint32				getSkillTimeByType		(const _stSkillModel* pSkillModel,_emSkillTimeType emType);
	uint32				getCurentTargetUnitId	()	{return m_uTarUnitID;}
	_stCompositeInfo*	getChildSkill			(uint32 uBaseSkillID)	{ return m_mapChildSkill.find_(uBaseSkillID);	}

	void				callbackKillTarget		(CSceneUnit* pTarget);
public:
	virtual void	onDoneSinging	(uint32 uSingingTick);
	void			onDoneSkill		(bool bBreak = false);
private:
	bool			canUse			(CSceneUnit* pOwner, const _stSkillModel* pSkillModel,uint8& uCode);
	bool			useConsume		(bool bSinging,uint8& uCode);
	bool	        useColletionSkill(uint8& nCode);
	bool	        arrestSkill		(uint8& nCode);
	bool	        activeSkill		(uint8& nCode);
	bool	        useRewardSkill	(uint8& nCode);
	bool			useSkillInnerAsk(CSceneUnit* m_pOwner, const _stSkillModel* pSkillModel,uint32 uTarUnitID,_stPoint2F&stTargetPoint,uint8& uCode);

private:
	bool			canSinging		(uint32 uTarUnitID,uint8& uCode);
	bool			singing			(uint32 uTarUnitID,_stPoint2F&stTargetPoint,uint8& uCode);
	bool			readyUseSkill	(uint32 uTarUnitID,uint8& uCode);

public:
	bool			process			(uint64 uTick);
	bool			processEndAnim	(uint64 uTick);
	void			processExpend	(uint64 uTick);

public:
	//--- 使用技能请求
	bool			useSkillAsk		(CSceneUnit* m_pOwner, const _stSkillModel* pSkillModel,uint32 uTarUnitID,_stPoint2F&stTargetPoint,uint8& uCode);
	//--- 打断技能请求
	bool			breakSkillAsk	(uint32 uSkillID,bool bForce,uint8& uCode);
	//--- 释放蓄力技能请求
	bool			useChargeAsk	(uint32 uSkillID,uint8& uCode);
	//--- 使用无动作技能 包括瞬发
	static bool		useNoAnimSkill	(CSceneUnit* m_pOwner, const _stSkillModel* pSkillModel,uint32 uTarUnitID,_stPoint2F&stTargetPoint);

	

};
