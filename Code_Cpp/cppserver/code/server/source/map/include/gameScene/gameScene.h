/*------------- gameScene.h
* Copyright (C): 2011  
* Author       : 赵文源
* Version      : V1.01
* Date         : 2011/3/23 11:54:06
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "gameSceneCell.h"
#include "cfg_data/sceneMapData/sceneMapData.h"
#include "cfg_data/sceneData/sceneData.h"
#include "memoryPool/mapPool.h"
#include "def_struct/dartcarStruct.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
class CGameScene	: public CSceneData
{
public:
	typedef CSceneData						Parent;
	friend	class							CGameSceneCell;
protected:
	typedef CMapPool<uint64,CGameSceneCell>	POOL_CELL;
	typedef stl_map<uint32,CGameSceneCell*>	MAP_CELL;
private:
	POOL_CELL			m_poolCell;			/*构建场景层列表*/ 
	MAP_CELL			m_mapCellID;
private:
	_stSceneMapModel*	m_pSceneModel;
	_stCopymapModel*	m_pCopymapModel;
public:
	CGameScene();
	~CGameScene();

public:
	inline uint16			getSceneID		()				{	return m_pSceneModel ? m_pSceneModel->uSceneID	: 0;	}
	inline bool				isCopymap		()				{	return (m_pCopymapModel != NULL);					}
	inline _stSceneMapModel*getSceneModel	()				{	return m_pSceneModel;								}
	inline _stCopymapModel*	getCopymapModel	()				{	return m_pCopymapModel;								}

	inline uint32			getCellCount	()				{	return m_mapCellID.size();};

public:
	bool	initialize						(_stSceneMapModel*pScene = NULL);
	void	shutdown						();
public:
	// 查找场景 通过唯一标识
	CGameSceneCell*	findSceneCell_ByDynID	(uint64 uDynID);

	// 查找场景 通过所属者(所属者id 因地图类型而不同 普通地图的话就是国家id 帮会地图的话就是帮会id ...)
	CGameSceneCell*	findSceneCell_ByCellID	(uint32 uCellID);

	CGameSceneCell*	createSceneCell			(uint64 uDynID,uint32 uCellID,const PBCreateCellParam&pbParam);
	bool			closeSceneCell			(uint64 uDynID);
public:
	bool			playerEnterScene		(CScenePlayer*pPlayer,uint64 uDynID);

public:
	void			process					(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay);
public:
	bool				getPointByRouteIDAndIndex(uint32 uRouteID, int32 uIndex, _stPoint2F& uReturnPoint);
};
