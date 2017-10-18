/*----------------- sceneMonsterMgr.cpp
*
* Copyright (C): 2015  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2015/12/26 18:31:26
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "gameLogic/sceneManage/sceneMonsterMgr.h"
#include "main/gameServer.h"
#include "basic/timeFunction.h"
#include "gameScene/gameSceneCell.h"
#include "gameScene/gameScene.h"
#include "log/log.h"
#include "cfg_data/monsterData/monsterData.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
CMonsterRegion::CMonsterRegion()
{
	initialize(NULL,NULL);
}

//-------------------------------------------------------------
//------------------------------ 
CMonsterRegion::~CMonsterRegion()
{
	freeMonster();
}

//-------------------------------------------------------------
//------------------------------ 释放怪物资源
void	CMonsterRegion::freeMonster	()
{
	m_hashAliveMonster.clear();
	m_vecDeadMonster.clear();
	g_clMonsterPool.clear();
}
//-------------------------------------------------------------
//------------------------------ 初始化
void	CMonsterRegion::initialize(CGameSceneCell* pSceneCell,_stMonsterRegion* pMonsterRegion)
{
	CTickDown::initialize();
	freeMonster();
	m_pSceneCell		= pSceneCell;
	m_pRegionModel			= pMonsterRegion;
	m_uRebirthTime		= (uint64)getTime();
	m_uAllDeadTime		= (uint64)getTime();
	m_bInFixTime		= false;
	if(!m_pSceneCell || !m_pRegionModel )
		return;

	CGameScene* pScene = m_pSceneCell->getSceneMap();
	if ( !pScene || !pScene->getSceneGridWide() || !pScene->getSceneGridHigh()  )
		return ;

	if(!m_pSceneCell->canMove(pMonsterRegion->fPosX,pMonsterRegion->fPosY))
	{
		//g_pThreadLog->error_PF
		CLog::error("地图id[%d]刷怪中心点坐标非法[%f,%f](donot move)!",pScene->getSceneID(),pMonsterRegion->fPosX,pMonsterRegion->fPosY);
		//return ;
	}

	for (uint32 i = 0;i < pMonsterRegion->vecMonster.size();i++)
	{
		_stMonsterCount& stCount = pMonsterRegion->vecMonster[i];
		const _stMonsterModel* pModel = g_clMonsterData.find(stCount.uMonsterID);
		if(!pModel)
		{
			CLog::error("地图id[%d]创建怪物[%d]找不到怪物模版!",pScene->getSceneID(),stCount.uMonsterID);
			continue;
		}

		// 循环创建怪物
		for (uint8 count = 0;count < stCount.uCount;count++)
			createMonster(pModel,pMonsterRegion->fPosX,pMonsterRegion->fPosY,pMonsterRegion->fDir, stCount.uRouteID);
	}//end for(i)
}

//-------------------------------------------------------------
//------------------------------ 创建怪物
CSceneMonster*	CMonsterRegion::createMonster	(const _stMonsterModel*pModel,float32 fPointX,float32 fPointY,float32 fDir,uint32 uRouteID,bool bNow/* = false*/,uint16 uLevel/* = 0*/)
{
	if(!m_pSceneCell || !pModel)
		return NULL;
	
	if(m_vecDeadMonster.size() > 10000)
		return NULL;

	CSceneMonster*pMonster = g_clMonsterPool.allocate();
	if(!pMonster)
		return NULL;
	pMonster->initialize();
	pMonster->build(pModel,m_pRegionModel, fPointX,fPointY,fDir,uRouteID,uLevel);

	m_pSceneCell->setMaxModelRadius(pMonster->getSkillModelRadius());
	//立即创建
	if (bNow)
	{
		//进如场景失败
		if(m_pSceneCell->enterScene(pMonster))
		{
			m_hashAliveMonster.insert_(pMonster->getUnitID(),pMonster);
			return pMonster;
		}
		g_clMonsterPool.release(pMonster);
		return NULL;
	}

	m_vecDeadMonster.push_back(pMonster);
	return pMonster;
}

//-------------------------------------------------------------
//------------------------------ 
uint32	CMonsterRegion::getMonsterCount	()
{
	uint32 uCount = 0;
	CSceneMonster*pMonster = NULL;
	{//所有存活怪物
		HASH_MONSTER::iterator _pos = m_hashAliveMonster.begin();
		HASH_MONSTER::iterator _end = m_hashAliveMonster.end();
		for (;_pos != _end;++_pos)
		{
			pMonster = (_pos->second);
			if(pMonster && !pMonster->isDead())
				++uCount;
		}
	}
	return uCount;
}

//-------------------------------------------------------------
//------------------------------ 
void	CMonsterRegion::getAllAliveMonster	(stl_vector<CSceneMonster*>& vecMonster)
{
	//uint32 uCount = 0;
	//CSceneMonster*pMonster = NULL;
	{//所有存活怪物
		HASH_MONSTER::iterator _pos = m_hashAliveMonster.begin();
		HASH_MONSTER::iterator _end = m_hashAliveMonster.end();
		for (;_pos != _end;++_pos)
		{
			vecMonster.push_back(_pos->second);
		}
	}
}

//-------------------------------------------------------------
//------------------------------ 
void	CMonsterRegion::destroy			()
{
	CSceneMonster*pMonster = NULL;

	{//所有存活怪物退出场景
		HASH_MONSTER::iterator _pos = m_hashAliveMonster.begin();
		HASH_MONSTER::iterator _end = m_hashAliveMonster.end();
		for (;_pos != _end;++_pos)
		{
			pMonster = (_pos->second);
			if(pMonster)
			{
				pMonster->exitScene();
				pMonster->unregisterObject();
			}
		}
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CMonsterRegion::remove		(uint64 uTime,uint32 uUnitID)
{
	HASH_MONSTER::iterator _pos = m_hashAliveMonster.find(uUnitID);
	if(_pos == m_hashAliveMonster.end())
		return;

	CSceneMonster*pMonster = (_pos->second);
	if(pMonster)
	{
		pMonster->unregisterObject();
		if ( m_pRegionModel && (m_pRegionModel->uRefreshType != MonsterRefreshType_None) )
		{
			m_vecDeadMonster.push_back(pMonster);
		}
		else
		{
			g_clMonsterPool.release(pMonster);
		}
	}
	m_hashAliveMonster.erase(_pos);
}
//-------------------------------------------------------------
//------------------------------ 生存中怪物列表
void	CMonsterRegion::monster_life	(SET_MONSTER&_set)
{
	CSceneMonster*pMonster = NULL;
	HASH_MONSTER::iterator _pos = m_hashAliveMonster.begin();
	HASH_MONSTER::iterator _end = m_hashAliveMonster.end();
	for (;_pos != _end;++_pos)
	{
		pMonster = (_pos->second);
		if(pMonster && !pMonster->isDead())
			_set.insert(pMonster);
	}
}
//-------------------------------------------------------------
//------------------------------ 推进处理
void	CMonsterRegion::process(uint64 uTick, uint64 uTime, uint32 uSecond)
{
	CTickDown::process((int64)uTick);
	if(!m_pSceneCell || !uSecond)
		return;

	if ( m_pRegionModel )
	{
		switch (m_pRegionModel->uRefreshType)
		{
		case MonsterRefreshType_IntervalTime:
			{
				if ( uTime < m_uRebirthTime + m_pRegionModel->uIntervalSecond )
					return ;

				m_uRebirthTime = uTime;
			}
			break;
		case MonsterRefreshType_FixedTime:
			{
				bool InTime = false;
				for ( uint32 i = 0 ; i < m_pRegionModel->vecFixedTime.size() ; ++i)
				{
					if ( m_pRegionModel->vecFixedTime[i].toDataTime(uTime))
					{
						InTime = true;
						break;
					}
				}

				bool bNeedBirth = false;
				if ( !m_bInFixTime && InTime )
					bNeedBirth = true;

				m_bInFixTime = InTime;
				if ( !bNeedBirth )
					return ;
			}
			break;
		case MonsterRefreshType_DeathTimeGroup:
			{
				if ( !m_hashAliveMonster.empty() )
					return ;

				if ( !m_uAllDeadTime )
				{
					m_uAllDeadTime = uTime;
					return ;
				}

				if ( uTime < m_uAllDeadTime + m_pRegionModel->uIntervalSecond )
					return ;

				m_uAllDeadTime = 0;
			}
			break;
		default:
			break;
		}
	}

	VEC_MONSTER::iterator _pos = m_vecDeadMonster.begin();
	while(_pos != m_vecDeadMonster.end())
	{
		CSceneMonster* pMonster = (*_pos);
		if (!pMonster)
		{
			_pos = m_vecDeadMonster.erase(_pos);
			continue;
		}

		if (  m_pRegionModel && (m_pRegionModel->uRefreshType == MonsterRefreshType_DeathTime) )
		{
			if ( uTime < pMonster->getDeadTime() + m_pRegionModel->uIntervalSecond )
			{
				++_pos;
				continue;
			}
		}

		float32 fPosX = pMonster->getPosition().x;
		float32 fPosY = pMonster->getPosition().y;
		float32 fDir  = pMonster->getDirection();
		if ( m_pRegionModel )
		{
			//位置随机
			_stPoint2F stOut(m_pRegionModel->fPosX,m_pRegionModel->fPosY);
			m_pSceneCell->randGetPoint(stOut,m_pRegionModel->fPosX,m_pRegionModel->fPosY,m_pRegionModel->fRadius);
			fPosX = stOut.x;
			fPosY = stOut.y;

			// 方向随机
			fDir = m_pRegionModel->fDir;
			if ( fDir < 0.0f )
			{
				fDir = getRandDir();
			}
		}

		// 重新构建
		uint32 uPatollPathID = pMonster->getRouteID();
		const _stMonsterModel* pModel = pMonster->getMonsterModel();
		pMonster->initialize();
		pMonster->build(pModel,m_pRegionModel, fPosX,fPosY,fDir,uPatollPathID);
		if(m_pSceneCell->enterScene(pMonster))
		{
			m_hashAliveMonster.insert_(pMonster->getUnitID(),pMonster);
			_pos = m_vecDeadMonster.erase(_pos); 
			continue;
		}

		//加入场景失败释放
		g_clMonsterPool.release(pMonster);
		_pos = m_vecDeadMonster.erase(_pos); 
	}
}
//#############################################################
//##############################
//#############################################################
//-------------------------------------------------------------
//------------------------------ 
CSceneMonsterMgr::CSceneMonsterMgr()
{
	m_pSceneCell	= NULL;
	m_bForbidBirth	= false;
	m_fMaxModelRadius = 0.0f;
}
//-------------------------------------------------------------
//------------------------------ 
CSceneMonsterMgr::~CSceneMonsterMgr()
{
	freeMonster();
}

//-------------------------------------------------------------
//------------------------------ 释放怪物资源
void	CSceneMonsterMgr::freeMonster	()
{
	CMonsterRegion*pPlace = NULL;
	MAP_REGION::iterator pos;
	m_mapRegion.getHead(pos);
	while(!m_mapRegion.isTail(pos))
	{
		pPlace = m_mapRegion.getNext(pos);
		if(pPlace)
			pPlace->freeMonster();
	}
	m_mapRegion.clear();
}
//-------------------------------------------------------------
//------------------------------ 初始化
void	CSceneMonsterMgr::initialize(CGameSceneCell* pSceneCell,VECTOR_MONSTER_REGION&vecMonsterRegion)
{
	m_pSceneCell = pSceneCell;
	m_bForbidBirth = false;

	freeMonster();
	if(!m_pSceneCell)
		return;

	CMonsterRegion*pPlace = NULL;
	for (uint32 i = 0;i < vecMonsterRegion.size();i++)
	{
		_stMonsterRegion&stRegion = vecMonsterRegion[i];
		pPlace = m_mapRegion.allocate(&stRegion);
		if(pPlace)
			pPlace->initialize(m_pSceneCell,&stRegion);
	}
}
//-------------------------------------------------------------
//------------------------------ 销毁所有怪物
void	CSceneMonsterMgr::destroyMonster	()
{
	CMonsterRegion*pPlace = NULL;
	MAP_REGION::iterator _pos = m_mapRegion.begin();
	MAP_REGION::iterator _end = m_mapRegion.end();
	while(_pos != _end)
	{
		pPlace = m_mapRegion.getNext(_pos);
		if(pPlace)
			pPlace->destroy();
	}
}

//-------------------------------------------------------------
//------------------------------ 
uint32	CSceneMonsterMgr::getMonsterCount	()
{
	uint32 uCount = 0;
	CMonsterRegion*pPlace = NULL;
	MAP_REGION::iterator _pos = m_mapRegion.begin();
	MAP_REGION::iterator _end = m_mapRegion.end();
	while(_pos != _end)
	{
		pPlace = m_mapRegion.getNext(_pos);
		if(pPlace)
			uCount += pPlace->getMonsterCount();
	}

	return uCount;
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneMonsterMgr::getAllAliveMonster	(stl_vector<CSceneMonster*>& vecAliveMonster)
{
	CMonsterRegion*pPlace = NULL;
	MAP_REGION::iterator _pos = m_mapRegion.begin();
	MAP_REGION::iterator _end = m_mapRegion.end();
	while(_pos != _end)
	{
		pPlace = m_mapRegion.getNext(_pos);
		if(pPlace)
			pPlace->getAllAliveMonster(vecAliveMonster);
	}
}

//-------------------------------------------------------------
//------------------------------ 创建怪物
CSceneMonster*	CSceneMonsterMgr::createMonster(const _stMonsterModel*pModel,float32 fPointX,float32 fPointY,float32 fDir,uint32 uRouteID/*0*/,bool bNow/*false*/,uint16 uLevel/*=0*/)
{
	if(!m_pSceneCell || !pModel)
		return NULL;

	if(!m_pSceneCell->canMove(fPointX,fPointY))
	{
		CLog::error("CSceneMonsterMgr::createMonster[%f,%f] donot move!",fPointX,fPointY);
		//return NULL;
	}

	CMonsterRegion*pPlace = m_mapRegion.find(NULL);
	if(!pPlace)
	{
		pPlace = m_mapRegion.allocate(NULL);
		if(pPlace)
			pPlace->initialize(m_pSceneCell,NULL);
	}
	if(!pPlace)
		return NULL;

	return pPlace->createMonster(pModel,fPointX,fPointY,fDir,uRouteID,bNow,uLevel);
}

//-------------------------------------------------------------
//------------------------------ 
void	CSceneMonsterMgr::remove(uint64 uTime,stl_vector<uint32>&vec)
{
	if(vec.empty())
		return;

	CMonsterRegion*pPlace = NULL;
	MAP_REGION::iterator _pos;
	MAP_REGION::iterator _end;
	m_mapRegion.getHead(_pos);
	m_mapRegion.getEnd(_end);
	while(_pos != _end)
	{
		pPlace = m_mapRegion.getNext(_pos);
		if(pPlace)
		{
			uint32 uSize	= vec.size();
			for (uint32 i = 0;i < uSize;i++)
			{
				uint32 uUnitID = vec[i];
				pPlace->remove(uTime,uUnitID);
			}
		}
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneMonsterMgr::monster_life	(SET_MONSTER&_set)
{
	CMonsterRegion*pPlace = NULL;
	MAP_REGION::iterator _pos;
	MAP_REGION::iterator _end;
	m_mapRegion.getHead(_pos);
	m_mapRegion.getEnd(_end);
	while(_pos != _end)
	{
		pPlace = m_mapRegion.getNext(_pos);
		if(pPlace)
			pPlace->monster_life(_set);		
	}
}
//------------------------------------------------------------------------------
//------------------------------------- 
void	CSceneMonsterMgr::processObject(uint64 uTick, uint64 uTime, uint32 uSecond)
{
	if ( m_bForbidBirth )
		return ;

	CMonsterRegion*pPlace = NULL;
	MAP_REGION::iterator _pos;
	MAP_REGION::iterator _end;
	m_mapRegion.getHead(_pos);
	m_mapRegion.getEnd(_end);
	while(_pos != _end)
	{
		pPlace = m_mapRegion.getNext(_pos);
		if(pPlace)
			pPlace->process(uTick, uTime,uSecond);
	}
}
//------------------------------------------------------------------------------
//------------------------------------- 
void	CSceneMonsterMgr::forbidBirth( bool bForbid )
{
	m_bForbidBirth = bForbid;
}

