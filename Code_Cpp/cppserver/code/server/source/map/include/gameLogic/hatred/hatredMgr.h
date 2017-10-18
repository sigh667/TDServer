/*------------- hatredMgr.h
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈腾
* Version      : V1.01
* Date         : 2016/02/26
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "basic/memoryFunctions.h"
#include "stl/std_map.h"
#include "math/point2.h"
#include "def_struct/gameDefine.h"
#include "memoryPool/mapPool.h"
/*************************************************************/
class CScenePlayer;
class CSceneUnit;
struct _stSkillModel;
struct _stSkillEffectModel;
class CHatredMgr
{
	friend		class	CSceneUnitAI;
public:
	struct _stHatred
	{
		uint32	uUnitID;	//对象ID
		int32	nLevel;		//仇恨等级
		int32	nHatred;	//仇恨值

		inline void		initialize	()	{	dMemset(this, 0, sizeof(_stHatred));	}
	};
private:
	typedef CMapPool<uint32, _stHatred>		MAP_HATRED;
	typedef stl_set<uint32>					SET_HOST;
private:
	CSceneUnit*		m_pOwner;		//拥有者
	//仇恨列表和被仇恨列表是一一对应的，
	//当怪物将玩家加入仇恨列表的时候，玩家也要将怪物添加入被仇恨列表，
	//删除仇恨关系，添加仇恨关系，更新仇恨值，应该是双向操作
	//当玩家下线，跳转地图，死亡，清空被仇恨列表，删除所有仇恨关系
	//当玩家脱离具体的怪物视线，删除某一个仇恨关系
	//玩家收到治疗，治疗者也要更新到被治疗者的被仇恨列表怪物的仇恨列表里。
	//主动攻击怪物待机状态下，主动将警戒范围内玩家加入仇恨关系，玩家不会有这个操作。
	//怪物收到玩家或者npc攻击造成伤害，将对方加入仇恨列表
	//怪物死亡，清空仇恨列表，删除所有仇恨关系
	MAP_HATRED		m_mapHatred;			//仇恨列表    怪物拥有仇恨列表
	SET_HOST		m_setBeHatred;			//被仇恨列表  玩家对应有被仇恨列表。
	_stHatred*		m_pFirstHatred;			//最高仇恨
public:
	void			initialize				(CSceneUnit* pUnit);

	void			clear					();
	void			clearHatred				();
	void			clearBeHatred			();

	uint32			getFirstHatredID		();
	uint32			getOrderHatredID		(uint32 uOrder);
	uint32			getRandomHatredID		();

	void			updateHatred			(CSceneUnit* pUnit, int32 nHatred, bool bCall = true);
	void			addHatredLevel			(CSceneUnit* pUnit, int32 nAddHatredLevel);
	_stHatred*		newHatred				(CSceneUnit* pUnit, int32 nHatred);
	void			deleteHatred			(CSceneUnit* pUnit);
	bool			hasHatred				();
	bool			hasBeHatred				();
	bool			isHatred				(uint32 uUnitID);

	bool			canHatred				(CSceneUnit* pUnit);
	bool			getHatredPlayer			(stl_set<CScenePlayer*>& setPlayer);
public:
	// 死亡回调
	void			onDead					();
	// 离开场景回调
	void			onLeaveScene			();
	// 被技能伤害回调
	void			onByDamage_Skill		(CSceneUnit* pSource, int32 nHP,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	// 被buff伤害回调
	void			onByDamage_Buff			(CSceneUnit* pSource, int32 nHP);
	// 被技能治疗回调
	void			onByCure_Skill			(CSceneUnit* pSource, int32 nHP,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	// 被buff治疗回调
	void			onByCure_Buff			(CSceneUnit* pSource, int32 nHP);
	// 可见状态改变回调
	void			onRefreshVisible		();
	// 进入警戒范围回调
	void			onEnterFightDis			(CSceneUnit* pSource);
private:
	void			updateFirstHatred		();
public:
	CHatredMgr();
	~CHatredMgr();
};
