/*------------- sceneItemMgr.h
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ³ÂÌÚ
* Version      : V1.01
* Date         : 2016/03/09
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "gameLogic/sceneObject/sceneItem.h"
#include "memoryPool/memoryPool.h"
#include "stl/std_map.h"
/*************************************************************/
struct _stDropModel;
struct _stDropCell;
class CSceneItemMgr
{
	//------------------------------ µôÂäÊý¾Ý³Ø
	friend	class CPlayerDropItem;
public:
	typedef CMemoryPool<CSceneItem>			MEM_POOL;
	typedef stl_map<uint32, CSceneItem*>	MAP_ITEM;
protected:
	CGameSceneCell*	m_pSceneCell;
	MEM_POOL		m_memItemPool;
	MAP_ITEM		m_mapItem;
	
public:
	virtual~CSceneItemMgr();

public:
	void		initialize		(CGameSceneCell*pSceneCell);
	void		removeObject	(uint32 uUnitID);
public:
	CSceneItem*	dropItem		(uint32 uDeadID, uint16 uDropID, CScenePlayer* pOwner,const _stPoint2I&stPoint);	
};
