/*----------------- sceneMonsterMgr.h
*
* Copyright (C): 2015  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2015/12/26 18:31:12
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "gameLogic/sceneObject/sceneMonster.h"
//#include "gameLogic/sceneObject/sceneDartCar.h"
#include "stl/std_deque.h"
#include "memoryPool/memoryPool.h"
#include "common/tickdown.h"
#include "memoryPool/mapPool.h"
#include "cfg_data/sceneData/sceneModel.h"
/*************************************************************/
//#############################################################
//##############################场景怪物摆放
//#############################################################
struct _stMonsterModel;
typedef stl_hash_map<uint32,CSceneMonster*>	HASH_MONSTER;
typedef stl_set<CSceneMonster*>				SET_MONSTER;
class CMonsterRegion : public CTickDown
{
private:
	CMemoryPool<CSceneMonster>	g_clMonsterPool;
public:
	typedef stl_vector<CSceneMonster*>			VEC_MONSTER;

protected:
	CGameSceneCell*				m_pSceneCell;
	_stMonsterRegion*			m_pRegionModel;

protected:
	HASH_MONSTER				m_hashAliveMonster;		/*存活的怪物列表*/ 
	VEC_MONSTER					m_vecDeadMonster;		/*死亡怪物列表*/ 

protected:
	uint64						m_uRebirthTime;			// MonsterRefreshType_IntervalTime 类型用
	uint64						m_uAllDeadTime;			// MonsterRefreshType_DeathTimeGroup 类型用
	bool						m_bInFixTime;			// MonsterRefreshType_FixedTime 类型用
protected:
	friend class CSceneMonsterMgr;

public:
	explicit CMonsterRegion();
	virtual ~CMonsterRegion();

protected:
	/*--->[ 释放怪物资源 ]*/
	void			freeMonster		();

public:
	/*--->[ 初始化 ]*/
	void			initialize			(CGameSceneCell* pSceneCell = NULL, _stMonsterRegion* pMonsterRegion = NULL);
	//--- 创建怪物
	CSceneMonster*	createMonster		(const _stMonsterModel*pModel,float32 fPointX,float32 fPointY,float32 fDir,uint32 uRouteID=0,bool bNow = false,uint16 uLevel = 0);
	uint32			getMonsterCount		();
	void			getAllAliveMonster	(stl_vector<CSceneMonster*>& vecMonster);
	void			destroy				();
	void			remove				(uint64 uTime,uint32 uUnitID);
	//--- 生存中怪物列表
	void			monster_life		(SET_MONSTER&_set);
	//--- 推进处理
	void			process				(uint64 uTick, uint64 uTime,uint32 uSecond);
};
//#############################################################
//##############################场景怪物管理
//#############################################################
class CSceneMonsterMgr
{
protected:
	typedef CMapPool<_stMonsterRegion*,CMonsterRegion>	MAP_REGION;

protected:
	CGameSceneCell*	m_pSceneCell;
	MAP_REGION		m_mapRegion;
	bool			m_bForbidBirth;
	float32			m_fMaxModelRadius;	//怪物的最大的半径

public:
	explicit CSceneMonsterMgr();
	virtual ~CSceneMonsterMgr();

public:
	/*--->[ 释放怪物资源 ]*/
	void			freeMonster		();
	inline float32	getMaxModelRadius()							{	return m_fMaxModelRadius;								}
	inline void		setMaxModelRadius(float32 fModelRadius)		{ m_fMaxModelRadius = MAX(fModelRadius,m_fMaxModelRadius);	}

public:
	/*--->[ 初始化 ]*/
	void			initialize		(CGameSceneCell* pSceneCell,VECTOR_MONSTER_REGION&vecMonsterRegion);
	//--- 销毁所有怪物
	void			destroyMonster	();
	uint32			getMonsterCount	();
	void			getAllAliveMonster	(stl_vector<CSceneMonster*>& vecAliveMonster);

	//--- 创建怪物
	CSceneMonster*	createMonster	(const _stMonsterModel*pModel,float32 fPointX,float32 fPointY,float32 fDir = -1.0f,uint32 uRouteID = 0,bool bNow = false,uint16 uLevel = 0);
	void			remove			(uint64 uTime,stl_vector<uint32>&vec);
	//--- 生存中怪物列表
	void			monster_life	(SET_MONSTER&_set);
	//--- 推进处理
	void			processObject	(uint64 uTick, uint64 uTime, uint32 uSecond);

	void			forbidBirth		(bool bForbid);
};

