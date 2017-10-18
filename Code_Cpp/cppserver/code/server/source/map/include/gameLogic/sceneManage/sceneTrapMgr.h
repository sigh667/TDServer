/*----------------- sceneTrapMgr.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2016/2/23 10:29:07
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "gameLogic/sceneObject/sceneTrap.h"
#include "memoryPool/memoryPool.h"
#include "stl/std_hash_map.h"
#include "stl/stl_vector.h"
#include "common/tickdown.h"
/*************************************************************/
struct _stTrapModel;
class CSceneTrapMgr
{
	//------------------------------ 掉落数据池
public:
	typedef CMemoryPool<CSceneTrap>	BUFF_POOL;

public:
	static BUFF_POOL	g_clTrapPool;

public:
	typedef stl_hash_map<uint32,CSceneTrap*>	HASH_BUFF;
	typedef stl_deque<CSceneTrap*>				QUE_BUFF;

protected:
	CGameSceneCell*		m_pSceneCell;
	HASH_BUFF			m_hashTrap;
public:
	explicit CSceneTrapMgr();
	virtual ~CSceneTrapMgr();

public:
	CSceneTrap*	findTrap	(uint32 uTrapID);
	void		initialize	(CGameSceneCell*pSceneCell);
	CSceneTrap*	newTrap		(CSceneUnit* pOwner,const _stTrapModel*pModel,float32 fPointX,float32 fPointY,float32 fDir,float32 fHigh,uint32 uTargetUnitID,_stPoint2F*const pTargetPos = NULL);

	void		removeObject(uint32 uUnitID);
	void		processObject(uint64 uTick, uint64 uTime);
};
