/*------------- sceneItemMgr.cpp
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ³ÂÌÚ
* Version      : V1.01
* Date         : 2016/03/09
*
*/ 
/***************************************************************
* 
***************************************************************/
#include "gameLogic/sceneManage/sceneItemMgr.h"
#include "gameScene/gameSceneCell.h"
#include "cfg_data/dropData/dropData.h"
/*************************************************************/

//-------------------------------------------------------------
//------------------------------ 
CSceneItemMgr::~CSceneItemMgr()
{
	initialize(NULL);
}

//-------------------------------------------------------------
//------------------------------ 
void	CSceneItemMgr::initialize(CGameSceneCell*pSceneCell)
{
	m_pSceneCell	= pSceneCell;
	m_mapItem.clear();
	m_memItemPool.clear();
}

//-------------------------------------------------------------
//------------------------------ 
void	CSceneItemMgr::removeObject		(uint32 uUnitID)
{
	MAP_ITEM::iterator _pos = m_mapItem.find(uUnitID);
	if (_pos == m_mapItem.end())
		return ;

	m_memItemPool.release(_pos->second);
	m_mapItem.erase(_pos);
}

//-------------------------------------------------------------
//------------------------------ 
CSceneItem* CSceneItemMgr::dropItem(uint32 uDeadID, uint16 uDropID, CScenePlayer* pOwner,const _stPoint2I&stPoint)
{
	if ( !uDropID )
		return NULL;

	VEC_DROP_CELL vecDropGroup;
	g_clDropData.find(uDropID,vecDropGroup);
	if ( vecDropGroup.empty() )
		return NULL;

	CSceneItem* pItem = m_memItemPool.allocate();
	if (!pItem)
		return NULL;

	pItem->initialize();
	if (pItem->build(m_pSceneCell,uDeadID, uDropID,vecDropGroup, pOwner))
	{
		pItem->setPosition(stPoint, SetPositionReason_Build,false);
		if (m_pSceneCell->enterScene(pItem))
		{
			m_mapItem.insert_(pItem->getUnitID(), pItem);
			return pItem;
		}
	}
	
	m_memItemPool.release(pItem);
	return NULL;
}
