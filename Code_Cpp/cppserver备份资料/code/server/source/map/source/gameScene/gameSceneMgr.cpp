/*------------- gameSceneMgr.cpp
* Copyright (C): 2011  
* Author       : 赵文源
* Version      : V1.01
* Date         : 2011/3/23 11:54:28
*
*/ 
/***************************************************************
* 
***************************************************************/
#include "gameScene/gameSceneMgr.h"
#include "main/gameServer.h"
#include "log/log.h"
#include "cfg_data/loadData/loadData.h"
#include "protocol/WAGMsg.pb.h"
//#include "gameLogic/globalObject/worldBossMgr.h"
/*************************************************************/
CGameSceneMgr* g_pGameSceneMgr = NULL;
//-------------------------------------------------------------
//------------------------------ 
CGameSceneMgr::CGameSceneMgr()
{
}

//-------------------------------------------------------------
//------------------------------ 
CGameSceneMgr::~CGameSceneMgr()
{
	shutdown();
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameSceneMgr::shutdown		()
{
	MAP_GAME_MAP::iterator _pos;
	m_mapGamemap.getHead(_pos);
	while(!m_mapGamemap.isTail(_pos))
	{
		CGameScene* pScene = m_mapGamemap.getNext(_pos);
		if(pScene)
			pScene->shutdown();
	}

	m_mapGamemap.clear();
}

//-------------------------------------------------------------
//------------------------------ 
CGameScene*	CGameSceneMgr::findSceneMap	(uint16 uSceneID)
{
	return m_mapGamemap.find(uSceneID);
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameSceneMgr::start	()
{
	auto _pos = g_clSceneMapData.begin();
	auto _end = g_clSceneMapData.end();
	for ( ; _pos != _end ; ++_pos )
	{
		_stSceneMapModel& model = _pos->second;
		CGameScene* pScene = m_mapGamemap.find(  model.uSceneID );
		if ( pScene )
			continue;

		pScene = m_mapGamemap.allocate(  model.uSceneID );
		if ( !pScene )
			continue;

		if ( !pScene->initialize(&model) )
			return false;
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameSceneMgr::playerEnterScene	(CScenePlayer*pPlayer,uint16 uSceneID,uint64 uDynID)
{
	if(!pPlayer)
		return false;

	CGameScene*pScene = findSceneMap(uSceneID);
	if(pScene)
		return pScene->playerEnterScene(pPlayer,uDynID);

	return false;
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameSceneMgr::timerProcess()
{
	static uint64 uProcessTime = 0;

	uint64 uTime	= (uint64)getTime();
	uint64 uTick	= getSystemTick64();
	if(!uProcessTime)
		uProcessTime = uTime;

	bool bNewDay	= (diffTimeDay(uProcessTime,uTime) != 0);
	uint32 uSecond	= uint32((uTime > uProcessTime) ? (uTime - uProcessTime) : 0);


	MAP_GAME_MAP::iterator _pos;
	m_mapGamemap.getHead(_pos);
	while(!m_mapGamemap.isTail(_pos))
	{
		CGameScene* pScene = m_mapGamemap.getNext(_pos);
		if(pScene)
			pScene->process(uTick,uTime,uSecond,bNewDay);
	}
	g_clLoadData.del_reload_mark();
	uProcessTime = uTime;
}
//-------------------------------------------------------------
//------------------------------ 
CGameSceneCell*CGameSceneMgr::findSceneCell_ByDynID	(uint16 uSceneID,uint64 uDynID)
{
	CGameScene* pScene = m_mapGamemap.find(uSceneID);
	if(pScene)
		return pScene->findSceneCell_ByDynID(uDynID);

	return NULL;
}
//-------------------------------------------------------------
//------------------------------ 
CGameSceneCell*CGameSceneMgr::findSceneCell_ByCellID	(uint16 uSceneID,uint32 uCellID)
{
	CGameScene* pScene = m_mapGamemap.find(uSceneID);
	if(pScene)
		return pScene->findSceneCell_ByCellID(uCellID);

	return NULL;
}
//-------------------------------------------------------------
//------------------------------ 
CGameSceneCell*	CGameSceneMgr::createSceneCell	(uint16 uSceneID,uint64 uDynID,uint32 uCellID,const PBCreateCellParam&pbParam)
{
	CGameScene* pScene = m_mapGamemap.find( uSceneID );
	if ( !pScene )
	{
		CLog::error("createSceneCell failed. uSceneID=%d", uSceneID);
		return NULL;
	}

	CGameSceneCell* pCell = pScene->createSceneCell(uDynID,uCellID,pbParam);
	if ( !pCell )
		return NULL;

	{
		PBG2WSceneRegisterCell info;
		info.set_scenedynid(pCell->getDynID());
		info.set_scenecellid(pCell->getCellID());
		info.set_sceneid(pCell->getSceneID());

		g_pServer->sendMsgToWorld(g_pServer->getLogicWorldID(),P_G2W_Protocol_Scene, G2W_Scene_RegisterCell,&info);
	
		//世界boss回调.
		if (!pCell->isCopymap())
		{
			//g_pWorldBossMgr->onSceneLoaded(uSceneID, uCellID);
		}
	
	}

	return pCell;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameSceneMgr::closeSceneCell	(uint16 uSceneID,uint64 uDynID)
{
	CGameScene* pScene = m_mapGamemap.find( uSceneID );
	if ( !pScene )
		return false;

	if ( !pScene->closeSceneCell(uDynID) )
		return false;

	{
		PBG2WSceneCloseCell info;
		info.set_scenedynid(uDynID);
		info.set_sceneid(uSceneID);
		g_pServer->sendMsgToWorld(g_pServer->getLogicWorldID(),P_G2W_Protocol_Scene, G2W_Scene_CloseCell,&info);
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameSceneMgr::printData	()
{
	return true;
}