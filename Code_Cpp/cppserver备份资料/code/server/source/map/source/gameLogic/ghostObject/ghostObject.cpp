/*----------------- ghostObject.cpp
*
* Copyright (C): 2011  
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/11/26 15:21:52
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "gameLogic/ghostObject/ghostObject.h"
#include "gameLogic/sceneObject/sceneUnit.h"
#include "log/log.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
CGhostObject::CGhostObject()
{
	FUNCTION_BEGIN_CHECK();
	m_uGhostObject	= 0;
	m_uUpdateMark	= 0;

	m_mapGhost.clear();
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGhostObject::initialize()
{
	FUNCTION_BEGIN_CHECK();
	m_uUpdateMark	= 0;

	m_mapGhost.clear();
	return true;
}

//-------------------------------------------------------------
//------------------------------ 
_stGhost*	CGhostObject::findGhost		(const uint32 uID)
{
	FUNCTION_BEGIN_CHECK();
	return m_mapGhost.find_(uID); 
}

//-------------------------------------------------------------
//------------------------------ 
bool	CGhostObject::insertGhostObject(CSceneUnit*pObject,bool bRecursive)
{
	FUNCTION_BEGIN_CHECK();
	if(!pObject || pObject == this)
		return true;

	CSceneUnit*pThis = dynamic_cast<CSceneUnit*>(this);
	if(!pThis)
	{
		CLog::error("CGhostObject::insertGhostObject(!pThis)");
		return false;
	}

	MAP_GHOST::_Pairib Pair = m_mapGhost.insert(MAP_GHOST::value_type(pObject->getUnitID(),_stGhost(pObject->getGameUnitID(),pObject)));
	if(Pair.first == m_mapGhost.end())
	{
		CLog::error("CGhostObject::insertGhostObject(end)");
		return false;
	}

	_stGhost&stGhost= (Pair.first)->second;
	stGhost.pObject	= pObject;
	stGhost.initMark();

	//递归时将自己加入到目标可视列表
	if(bRecursive)
		pObject->insertGhostObject(pThis,false);

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
void	CGhostObject::resetGhost()
{
	m_uUpdateMark	= 0xffffffff;
	_SET_BIT32(m_uUpdateMark,Ghost_Remove,false);

	static uint32 updateMark = 0xffffffff;
	_SET_BIT32(updateMark,Ghost_Remove,false);

	MAP_GHOST::iterator _pos = m_mapGhost.begin();
	MAP_GHOST::iterator _end = m_mapGhost.end();
	for (;_pos != _end;++_pos)
	{
		_stGhost&ghost = _pos->second;
		ghost.updateMark |= updateMark;
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CGhostObject::setGhost		(uint32 uMark,bool bSet,bool bOnlySelf)
{
	FUNCTION_BEGIN_CHECK();
	if(!uMark)
		return;

	if(bSet)
		m_uUpdateMark |= uMark;
	else
		m_uUpdateMark &=~uMark;

	if(bOnlySelf)
		return;

	MAP_GHOST::iterator _pos = m_mapGhost.begin();
	MAP_GHOST::iterator _end = m_mapGhost.end();
	for (;_pos != _end;++_pos)
	{
		_stGhost&ghost = _pos->second;
		if(ghost.pObject)
			ghost.pObject->updateGhost(getUnitID(),uMark,bSet);
	}
}

//-------------------------------------------------------------
//------------------------------ 
bool	CGhostObject::testGhost(uint32 uMark)
{
	FUNCTION_BEGIN_CHECK();
	return _CHECK_BIT(m_uUpdateMark,uMark);
}
//-------------------------------------------------------------
//------------------------------ 
void	CGhostObject::removeGhost(uint32 uMark)
{
	FUNCTION_BEGIN_CHECK();
	m_uUpdateMark &=~uMark;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CGhostObject::updateGhost		(const uint32 uID,uint32 uMark,bool bSet)
{
	FUNCTION_BEGIN_CHECK();
	_stGhost*pGhost = findGhost(uID);
	if(!pGhost)
	{
		//CLog::error("CGhostObject::updateGhost(%d findGhost %d)",getUnitID(),uID);
		return false;
	}

	if(bSet)
		pGhost->updateMark |= uMark;
	else
		pGhost->updateMark &=~uMark;

	if(_CHECK_BIT(uMark,_BIT32(Ghost_Remove)))
		pGhost->pObject = NULL;

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
void	CGhostObject::clearGhost(bool bRemove)
{
	FUNCTION_BEGIN_CHECK();

	m_uUpdateMark	= 0xffffffff;
	_SET_BIT32(m_uUpdateMark,Ghost_Remove,false);

	if(bRemove)
		setGhost(_BIT32(Ghost_Remove));
	else
		setGhost(_BIT32(Ghost_Remove),false);

	m_mapGhost.clear();
}

//-------------------------------------------------------------
//------------------------------ 
void	CGhostObject::updateRemove()
{
	FUNCTION_BEGIN_CHECK();
	MAP_GHOST::iterator _pos = m_mapGhost.begin();
	MAP_GHOST::iterator _end = m_mapGhost.end();
	for (;_pos != _end;++_pos)
	{
		_stGhost&stGhost	= _pos->second;
		stGhost.updateMark|=_BIT32(Ghost_Remove);
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CGhostObject::clearRemove()
{
	FUNCTION_BEGIN_CHECK();
	//m_uUpdateMark = 0;
	MAP_GHOST::iterator _pos = m_mapGhost.begin();
	MAP_GHOST::iterator _end = m_mapGhost.end();
	while(_pos != _end)
	{
		_stGhost&stGhost = (_pos->second);
		if(!stGhost.testMark(Ghost_Remove) && stGhost.pObject)
		{
			++_pos;
			continue;
		}
		if(stGhost.pObject)
			stGhost.pObject->updateGhost(getUnitID(),_BIT32(Ghost_Remove),true);

		_pos = m_mapGhost.erase_(_pos);
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CGhostObject::updateGhostList	(MAP_GAME_UNIT& _map)
{
	FUNCTION_BEGIN_CHECK();
	//设置移除标志
	updateRemove();
	/*收集可视对像*/ 
	_stGhost*	pGhost = NULL;
	CSceneUnit* pObject = NULL;
	MAP_GAME_UNIT::iterator _pos = _map.begin();
	MAP_GAME_UNIT::iterator _end = _map.end();
	for (;_pos != _end;++_pos)
	{
		pObject = _pos->second;
		if(!pObject || pObject == this)
			continue;

		pGhost = findGhost(_pos->first);
		if(pGhost && pGhost->pObject)
		{
			pGhost->update();
			continue;
		}

		insertGhostObject(_pos->second);
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CGhostObject::removeViewRange		(const _stPoint2I&stPoint)
{
	MAP_GHOST::iterator _pos = m_mapGhost.begin();
	MAP_GHOST::iterator _end = m_mapGhost.end();
	for (;_pos != _end;++_pos)
	{
		_stGhost&stGhost = _pos->second;
		if(!stGhost.pObject || !inRange(stPoint,stGhost.pObject->getPosition(),float32(ViewRange)))
			stGhost.updateMark|=_BIT32(Ghost_Remove);
	}
}

//-------------------------------------------------------------
//------------------------------ 
CSceneUnit*CGhostObject::findGhostObject	(const uint32 uID)
{
	FUNCTION_BEGIN_CHECK();
	_stGhost*pGhost = findGhost(uID);

	return (pGhost ? pGhost->pObject : NULL);
}

//-------------------------------------------------------------
//------------------------------ 距离排序qsort()
int32 CGhostObject::_rangeUnit_distance(const void *p1, const void *p2)
{
	FUNCTION_BEGIN_CHECK();
	const _stRangeUnit* st1 = (const _stRangeUnit*)(p1);
	const _stRangeUnit* st2 = (const _stRangeUnit*)(p2);

	int32 res = 0;
	if(st1->fDistance > st2->fDistance)
		res = 1;

	if(st1->fDistance < st2->fDistance)
		res = -1;

	return res;
}
