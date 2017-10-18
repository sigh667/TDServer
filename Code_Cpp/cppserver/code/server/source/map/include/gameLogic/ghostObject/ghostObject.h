/*----------------- ghostObject.h
*
* Copyright (C): 2011  
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/11/26 15:21:54
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "stl/std_hash_map.h"
#include "stl/stl_vector.h"
#include "def_struct/gameStruct.h"
/*************************************************************/
class CGameSceneBlock;
class CSceneUnit;
//------------------------------ 
//-------------------------------------------------------------
//------------------------------ Ghost对像
struct _stGhost
{
	uint32			updateMark;
	CSceneUnit*		pObject;
	_stGameUnitID	stGameUnitID;

	_stGhost(_stGameUnitID&stUnitID,CSceneUnit*pObject)
	{
		this->pObject		= pObject;
		this->stGameUnitID= stUnitID;
		initMark();
	}
	_stGhost&getGhost	()				{	return *this;								}
	void	initMark	()				{	this->updateMark	= 0xFFFFFFFF;update();	}
	void	update		()				{	_SET_BIT32(updateMark,Ghost_Remove,false);	}
	bool	testMark	(uint8 uMark)	{	return _CHECK_BIT(updateMark,_BIT32(uMark));}
};
//-------------------------------------------------------------
//------------------------------ 范围对像
struct _stRangeUnit
{
	CSceneUnit*	pUnit;
	float32		fDistance;

	_stRangeUnit(CSceneUnit*pUnit,float32 fDistance)
	{
		this->pUnit		= pUnit;
		this->fDistance	= fDistance;
	}
};
//-------------------------------------------------------------
//------------------------------ 
typedef stl_vector<CSceneUnit*>			VECTOR_GAME_UNIT;
typedef stl_vector<_stRangeUnit>		VECTOR_RANGE_UNIT;
typedef stl_map<uint32,CSceneUnit*>		MAP_GAME_UNIT;
//------------------------------ 
//#############################################################
//##############################
//#############################################################
class CGhostObject
{
public:
	enum
	{
		ViewRange	= 12,	/*搜索可视范围*/ 
	};
public:
	typedef stl_map<uint32,_stGhost>		MAP_GHOST;

protected:
	bool				m_bRegister;	//是否已注册
	_stGameUnitID		m_stUnitID;		//单位唯一ID

	//------------------------------ 游戏Ghost
protected:
	int32				m_uGhostObject;	/*需要ghost对像*/ 
	uint32				m_uUpdateMark;	/*自身更新标志*/ 
	MAP_GHOST			m_mapGhost;		/*可视范围游戏对像*/ 

public:
	CGhostObject();

public:
	virtual bool	initialize			();

public:
	inline uint32			getUnitID			()const					{	return m_stUnitID.uUnitID;		}
	inline uint8			getUnitType			()const					{	return m_stUnitID.uUnitType;	}
	inline _stGameUnitID&	getGameUnitID		()						{	return m_stUnitID;				}

protected:
	virtual void			clearGhost			(bool bRemove = false);
	virtual void			updateRemove		();
	virtual void			clearRemove			();
	virtual void			updateGhostList		(MAP_GAME_UNIT& _map);
	virtual void			removeViewRange		(const _stPoint2I&stPoint);

public:
	virtual void			resetGhost			();
	virtual void			setGhost			(uint32 uMark,bool bSet = true,bool bOnlySelf = false);
	virtual bool			testGhost			(uint32 uMark);
	virtual void			removeGhost			(uint32 uMark);
	virtual bool			updateGhost			(const uint32 uID,uint32 uMark,bool bSet = true);
	virtual _stGhost*		findGhost			(const uint32 uID);
	virtual CSceneUnit*		findGhostObject		(const uint32 uID);
	virtual bool			insertGhostObject	(CSceneUnit*pObject,bool bRecursive = true);
public:
	//--- 距离排序qsort()
	static int32 _rangeUnit_distance	(const void *p1, const void *p2);
};
