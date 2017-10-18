/*------------- gameScene.cpp
* Copyright (C): 2011  
* Author       : 赵文源
* Version      : V1.01
* Date         : 2011/3/23 11:54:08
*
*/ 
/***************************************************************
* 
***************************************************************/
#include "gameScene/gameScene.h"
#include "basic/systemFunction.h"
#include "cfg_data/copymapData/copymapData.h"
#include "cfg_data/loadData/loadData.h"
#include "log/log.h"
#include "def_struct/gameDefine_inline.h"
#include "cfg_data/sceneData/sceneData.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
CGameScene::CGameScene()
{
	initialize(NULL);
}

//-------------------------------------------------------------
//------------------------------ 
CGameScene::~CGameScene()
{
	shutdown();
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameScene::initialize(_stSceneMapModel*pSceneModel)
{
	if(!pSceneModel)
		return false;
	m_pSceneModel	= pSceneModel;
	m_pCopymapModel	= NULL;	

	if(pSceneModel->isCopymap())
		m_pCopymapModel = pSceneModel->pCopymapModel;

	shutdown();
	m_mapCellID.clear();

	std::string strFileName;

	///地图信息
	strFileName = g_clLoadData.buld_scene_path("%s.bytes",m_pSceneModel->strResoureName.c_str());
	if(!Parent::loadFile(strFileName.c_str()))
	{
		CLog::error("loadFile(%s) fail...",strFileName.c_str());
		return false;
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameScene::shutdown	()
{
	POOL_CELL::iterator _pos;
	m_poolCell.getHead(_pos);
	while(!m_poolCell.isTail(_pos))
	{
		CGameSceneCell* pCell = m_poolCell.getNext(_pos);
		if (pCell)
			pCell->shutdown();
	}

	m_poolCell.clear();
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameScene::process(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay)
{
	POOL_CELL::iterator _pos;
	m_poolCell.getHead(_pos);
	while(!m_poolCell.isTail(_pos))
	{
		CGameSceneCell* pCell = m_poolCell.getNext(_pos);
		if(pCell)
		{
			pCell->process(uTick,uTime,uSecond,bNewDay);

			if (pCell->isClose())
			{
				uint32 uCellID	= pCell->getCellID();
				uint64 uDynID	= pCell->getDynID();
				pCell->shutdown();
				m_mapCellID.erase(uCellID);
				m_poolCell.release(uDynID);
			}
		}
	}
}

//-------------------------------------------------------------
//------------------------------ 
CGameSceneCell*	CGameScene::createSceneCell(uint64 uDynID,uint32 uCellID,const PBCreateCellParam&pbParam)
{
	if (!m_pSceneModel || !uDynID)
		return NULL;

	//查找场景单元是否已被创建并使用
	CGameSceneCell* pCell = m_poolCell.find(uDynID);
	if(pCell)
	{
		CLog::error("CGameScene::createSceneCell(%u) existed...",m_pSceneModel->uSceneID);
		return NULL;
	}

	if ( uCellID )
	{
		MAP_CELL::iterator _pos = m_mapCellID.find(uCellID);
		if(_pos != m_mapCellID.end() )
		{
			CLog::error("CGameScene::createSceneCell(%u) owner existed...",m_pSceneModel->uSceneID);
			return NULL;
		}
	}

	pCell	= m_poolCell.allocate(uDynID);
	if(!pCell)
	{
		CLog::error("CGameScene::createSceneCell(%u) allocate fail...",m_pSceneModel->uSceneID);
		return NULL;
	}
	pCell->initialize(uDynID,uCellID,this, &pbParam);

	if ( uCellID )
		m_mapCellID.insert_(uCellID,pCell);


	
	return pCell;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CGameScene::closeSceneCell(uint64 uDynID)
{
	CGameSceneCell*pCell = m_poolCell.find(uDynID);
	if(!pCell)
		return false;

	pCell->close();
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
CGameSceneCell*	CGameScene::findSceneCell_ByDynID	(uint64 uDynID)
{
	return m_poolCell.find(uDynID);
}
//-------------------------------------------------------------
//------------------------------ 
CGameSceneCell*	CGameScene::findSceneCell_ByCellID	(uint32 uCellID)
{
	MAP_CELL::iterator _pos = m_mapCellID.find(uCellID);
	if(_pos == m_mapCellID.end() )
		return NULL;

	return _pos->second;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameScene::playerEnterScene	(CScenePlayer*pPlayer,uint64 uDynID)
{
	if(!pPlayer)
		return false;

	CGameSceneCell*pCell = m_poolCell.find(uDynID);
	if(!pCell)
		return false;

	return pCell->playerEnterScene(pPlayer);
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameScene::getPointByRouteIDAndIndex(uint32 uRouteID, int32 uIndex, _stPoint2F& stReturnPoint)
{
	const _stSceneRout* pSR = findRout(uRouteID);
	if (pSR != NULL && uIndex >= 0 && (int32)pSR->vecPoint.size() > uIndex)
	{
		//取指定的点
		stReturnPoint = pSR->vecPoint[uIndex];
		return true;
	}
	else if (pSR != NULL && uIndex < 0 && pSR->vecPoint.size() > 0)
	{
		//取最后一个点
		stReturnPoint = pSR->vecPoint[pSR->vecPoint.size() - 1];
		return true;
	}
	return false;
}