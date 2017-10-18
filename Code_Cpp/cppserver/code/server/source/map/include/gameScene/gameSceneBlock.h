/*----------------- gameSceneBlock.h
*
* Copyright (C): 2011  
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/9/13 14:58:28
*--------------------------------------------------------------
*场景数块
*------------------------------------------------------------*/
#pragma once
#include "stl/stl_list.h"
#include "stl/std_deque.h"
#include "stl/std_hash_map.h"
#include "gameLogic/sceneObject/sceneUnit.h"
#include "gameLogic/sceneObject/scenePlayer.h"
#include "gameLogic/sceneObject/sceneMonster.h"
/*************************************************************/
typedef stl_list<CSceneUnit*>				LIST_OBJECT;
typedef stl_map<uint32,CScenePlayer*>		HASH_PLAYER;
//--- 存在对象管理
class CGameSceneHave
{
public:
	CGameSceneHave()
	{
		clear();	
	}
protected:
	uint16	uHave[_em_GameUnitType_ARRAYSIZE];

public:
	inline void		clear	()				{	memset(&uHave,0,sizeof(uHave));								}
	inline void		increase(uint8 uType)	{	if(uType < _em_GameUnitType_ARRAYSIZE)++uHave[uType];					}
	inline void		reduce	(uint8 uType)	{	if(uType < _em_GameUnitType_ARRAYSIZE && uHave[uType])--uHave[uType];	}
	inline uint16	size	(uint8 uType)	{	return ((uType < _em_GameUnitType_ARRAYSIZE) ? uHave[uType]: 0);		}
};
//-------------------------------------------------------------
//------------------------------ 
class CGameSceneBlock
{
protected:
public:
	uint16			m_uPointX;		/*中心点坐标*/ 
	uint16			m_uPointY;
	LIST_OBJECT		m_listObject;

public:
	CGameSceneBlock()
	{
		init(0,0);
		clear();
	}

public:
	inline void			init	(uint16 x,uint16 y)	
	{
		m_uPointX = x;
		m_uPointY = y;
	}
	/*--->[ 清理 ]*/
	inline void			clear	()
	{
		m_listObject.clear();
	}
	//--- 是否拥有对象
	inline bool			have	(uint32 uTypeMark,_stPoint2I stPoint)
	{
		if(!uTypeMark)
			return false;

		CSceneUnit*pUnit;
		LIST_OBJECT::iterator _pos = m_listObject.begin();
		LIST_OBJECT::iterator _end = m_listObject.end();
		for(;_pos != _end;++_pos)
		{
			//pUnit = pos->second;
			pUnit = *(_pos);
			if(!pUnit || !_CHECK_BIT(uTypeMark,_BIT32(pUnit->getUnitType())) || pUnit->isDead())
				continue;

			if(pUnit->getPosition() == stPoint)
				return true;
		}
		return false;
	}
	inline void			gather(_stPoint2I stPoint,uint32 uTypeMark,MAP_GAME_UNIT& _map,bool canDead)
	{
		CSceneUnit*pUnit;
		LIST_OBJECT::iterator _pos = m_listObject.begin();
		LIST_OBJECT::iterator _end = m_listObject.end();
		for(;_pos != _end;++_pos)
		{
			pUnit = *(_pos);
			if(!pUnit || !_CHECK_BIT(uTypeMark,_BIT32(pUnit->getUnitType())))
				continue;

			if(!canDead && pUnit->isDead())
				continue;

			if (pUnit->isHide())
				//if (pUnit->haveState(StateType_Plane) || pUnit->haveState(StateType_Hide))
				continue;

			if(pUnit->getPosition() == stPoint)
				_map.insert_(pUnit->getUnitID(),pUnit);
		}
	}
	/*--->[ 收集对像 ]*/
	inline void			gather(const _stPoint2F&stPoint,float32 fRadius,uint32 uTypeMark,MAP_GAME_UNIT& _map)
	{
		CSceneUnit*pUnit;
		LIST_OBJECT::iterator _pos = m_listObject.begin();
		LIST_OBJECT::iterator _end = m_listObject.end();
		for(;_pos != _end;++_pos)
		{
			//pUnit = pos->second;
			pUnit = *(_pos);
			if(!pUnit || !_CHECK_BIT(uTypeMark,_BIT32(pUnit->getUnitType())))
				continue;

			if(!inRange(stPoint,pUnit->getPosition(),fRadius))
				continue;

			_map.insert_(pUnit->getUnitID(),pUnit);
		}
	}
	/*--->[ 擦除对像 ]*/
	inline void			erase	(CSceneUnit*pObject)
	{
		m_listObject.remove(pObject);
	}
	/*--->[ 插入对像 ]*/
	inline void			insert(CSceneUnit*pObject)
	{
		if(pObject)
			m_listObject.push_back(pObject);
	}
};
