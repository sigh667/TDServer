/*----------------- sceneMonsterMgr.h
*
* Copyright (C): 2015  Mokylin��Mokyqi
* Author       : ����Դ
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
//##############################��������ڷ�
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
	HASH_MONSTER				m_hashAliveMonster;		/*���Ĺ����б�*/ 
	VEC_MONSTER					m_vecDeadMonster;		/*���������б�*/ 

protected:
	uint64						m_uRebirthTime;			// MonsterRefreshType_IntervalTime ������
	uint64						m_uAllDeadTime;			// MonsterRefreshType_DeathTimeGroup ������
	bool						m_bInFixTime;			// MonsterRefreshType_FixedTime ������
protected:
	friend class CSceneMonsterMgr;

public:
	explicit CMonsterRegion();
	virtual ~CMonsterRegion();

protected:
	/*--->[ �ͷŹ�����Դ ]*/
	void			freeMonster		();

public:
	/*--->[ ��ʼ�� ]*/
	void			initialize			(CGameSceneCell* pSceneCell = NULL, _stMonsterRegion* pMonsterRegion = NULL);
	//--- ��������
	CSceneMonster*	createMonster		(const _stMonsterModel*pModel,float32 fPointX,float32 fPointY,float32 fDir,uint32 uRouteID=0,bool bNow = false,uint16 uLevel = 0);
	uint32			getMonsterCount		();
	void			getAllAliveMonster	(stl_vector<CSceneMonster*>& vecMonster);
	void			destroy				();
	void			remove				(uint64 uTime,uint32 uUnitID);
	//--- �����й����б�
	void			monster_life		(SET_MONSTER&_set);
	//--- �ƽ�����
	void			process				(uint64 uTick, uint64 uTime,uint32 uSecond);
};
//#############################################################
//##############################�����������
//#############################################################
class CSceneMonsterMgr
{
protected:
	typedef CMapPool<_stMonsterRegion*,CMonsterRegion>	MAP_REGION;

protected:
	CGameSceneCell*	m_pSceneCell;
	MAP_REGION		m_mapRegion;
	bool			m_bForbidBirth;
	float32			m_fMaxModelRadius;	//��������İ뾶

public:
	explicit CSceneMonsterMgr();
	virtual ~CSceneMonsterMgr();

public:
	/*--->[ �ͷŹ�����Դ ]*/
	void			freeMonster		();
	inline float32	getMaxModelRadius()							{	return m_fMaxModelRadius;								}
	inline void		setMaxModelRadius(float32 fModelRadius)		{ m_fMaxModelRadius = MAX(fModelRadius,m_fMaxModelRadius);	}

public:
	/*--->[ ��ʼ�� ]*/
	void			initialize		(CGameSceneCell* pSceneCell,VECTOR_MONSTER_REGION&vecMonsterRegion);
	//--- �������й���
	void			destroyMonster	();
	uint32			getMonsterCount	();
	void			getAllAliveMonster	(stl_vector<CSceneMonster*>& vecAliveMonster);

	//--- ��������
	CSceneMonster*	createMonster	(const _stMonsterModel*pModel,float32 fPointX,float32 fPointY,float32 fDir = -1.0f,uint32 uRouteID = 0,bool bNow = false,uint16 uLevel = 0);
	void			remove			(uint64 uTime,stl_vector<uint32>&vec);
	//--- �����й����б�
	void			monster_life	(SET_MONSTER&_set);
	//--- �ƽ�����
	void			processObject	(uint64 uTick, uint64 uTime, uint32 uSecond);

	void			forbidBirth		(bool bForbid);
};

