/*----------------- skillAnim.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/1/27 19:58:58
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
#include "math/point2.h"
#include "cfg_data/skillData/skillModel.h"
#include "gameLogic/ghostObject/ghostObject.h"
/*************************************************************/
class CSceneUnit;
//-------------------------------------------------------------
//------------------------------ 
class CSkillAnim 
{
private:
	CSceneUnit*					m_pOwner;			//归属者
	const _stSkillModel*		m_pSkillModel;		//技能模板
	const _stSkillAnimModel*	m_pSkillAnimModel;	//当前动作模板
	uint64						m_uStartAnimTick;	//开始动作毫秒
	uint64						m_uEndAnimTick;		//结束动作毫秒
	bool						m_bBreak;			//是否中断

	_stAttEffect		m_stAttEffect;		//当前的效果帧
	uint8				m_uCurEffect;		//当前的效果

public:
	CSkillAnim();

public:
	inline	bool	canOperate	()				{   return m_pSkillModel != NULL && m_pOwner != NULL && m_pSkillAnimModel != NULL && !m_bBreak;	}
	inline	bool	isBreak		()				{   return m_bBreak;	}

public:
	void			initialize	(CSceneUnit* pOwner,uint64 uStartTick,const _stSkillModel* pSkillModel,const _stSkillAnimModel* pSkillAnimModel);
	void			start		();
	void			end			();
public:
	static bool		canChooseTo	(CSceneUnit* pOwner,CSceneUnit*pTarget,const _stSkillModel* pModel);
	static bool		gatherObject(CSceneUnit* pOwner,CSceneUnit*pTarget,const _stPoint2F&stTargetPoint,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel,MAP_GAME_UNIT&mapTarget,float32 fMoveDis);
	static bool		gatherObject(CSceneUnit* pOwner,const _stPoint2F&stTargetPoint,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel,VECTOR_RANGE_UNIT&vecRangeUnit,float32 fMoveDis);
	//--- 开始伤害
	static bool		beginDamage	(CSceneUnit* pOwner,CSceneUnit* pTarget,const _stPoint2F&stTargetPoint,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel,float32 fMoveDis = 0);
	//--- 延时攻击
	static bool		delayDamage	(CSceneUnit* pOwner,CSceneUnit* pTarget,const _stPoint2F&stTargetPoint,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	//--- 真正伤害
	static bool		realDamage	(CSceneUnit* pOwner,MAP_GAME_UNIT&mapTarget,const _stPoint2F&stTargetPoint,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	//--- 所有效果
	static bool		skillAllEffect(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stPoint2F&stTargetPoint,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	//--- 治疗
	static bool		skillCure	(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stPoint2F&stTargetPoint,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	//--- 攻击
	static bool		skillAttack	(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stPoint2F&stTargetPoint,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	//--- 计算伤害
	static bool		attackDamage(CSceneUnit* pOwner,CSceneUnit*pTarget,const _stPoint2F&stTargetPoint,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel,uint8 uAttackNum,uint8 uTargetNum);
	//--- 技能加buff
	static bool		skillAddBuff(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel,const _stSkillEffectType& stSkillEffectType);
	//--- 技能加场景对象
	static bool		skillAddSceneObject(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stPoint2F&stTargetPoint,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	//--- 加消耗
	static bool		skillAddExpend(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel,const _stSkillEffectType& stSkillEffectType);
	//--- 技能清除buff
	static bool		skillClearBuff(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel,const _stSkillEffectType& stSkillEffectType);
	//--- 技能加位移
	static bool		skillAddMove(CSceneUnit* pOwner,const _stPoint2F&stTargetPoint,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel,const _stSkillEffectType& stSkillEffectType);
	//--- 命中目标使用技能
	static bool		skillUseSkill(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel,const _stSkillEffectType& stSkillEffectType);
	//--- 命中目标使用技能
	static bool		skillOutRingAddBuff(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel,const _stSkillEffectType& stSkillEffectType);
	//--- 重置buff时间
	static bool		skillResetBuffTime(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel,const _stSkillEffectType& stSkillEffectType);
	//--- 技能命中完成
	static bool		skillDamageOver(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	
public:
	bool			process			(uint64 uTick);
	bool			processAttack	(uint64 uTick);

};
