/*----------------- sceneTrapMgr.cpp
*
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ÕÔÎÄÔ´
* Version      : V1.0
* Date         : 2016/2/23 10:29:20
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "gameLogic/sceneManage/sceneTrapMgr.h"
#include "main/gameServer.h"
#include "gameScene/gameSceneCell.h"
/*************************************************************/
CSceneTrapMgr::BUFF_POOL	CSceneTrapMgr::g_clTrapPool;
//-------------------------------------------------------------
//------------------------------ 
CSceneTrapMgr::CSceneTrapMgr()
{
	initialize(NULL);
}
//-------------------------------------------------------------
//------------------------------ 
CSceneTrapMgr::~CSceneTrapMgr()
{
	initialize(NULL);
}

//-------------------------------------------------------------
//------------------------------ 
void	CSceneTrapMgr::initialize(CGameSceneCell*pSceneCell)
{
	m_pSceneCell	= pSceneCell;

	HASH_BUFF::iterator _pos = m_hashTrap.begin();
	HASH_BUFF::iterator _end = m_hashTrap.end();
	for (;_pos != _end;++_pos)
	{
		g_clTrapPool.release(_pos->second);
	}
	m_hashTrap.clear();
}
//-------------------------------------------------------------
//------------------------------ 
CSceneTrap*	CSceneTrapMgr::newTrap		(CSceneUnit* pOwner,const _stTrapModel*pModel,float32 fPointX,float32 fPointY,float32 fDir,float32 fHigh,uint32 uTargetUnitID,_stPoint2F*const pTargetPos)
{
	if(!pModel || !m_pSceneCell )
		return NULL;

	CSceneTrap*pGameTrap = g_clTrapPool.allocate();
	if(!pGameTrap)
		return NULL;

	pGameTrap->initialize();
	_stPoint2F stTmpPoint(fPointX,fPointY);
	_stPoint2F stModifyStart(fPointX,fPointY);
	if ( pOwner)
		getTargetPoint(stTmpPoint,fDir,pOwner->getModelRadius(),stModifyStart);

	pGameTrap->build(pOwner,pModel,stModifyStart,fDir,fHigh,pTargetPos,uTargetUnitID);
	pGameTrap->registerObject();
	m_pSceneCell->enterScene(pGameTrap);
	m_hashTrap.insert_(pGameTrap->getUnitID(),pGameTrap);
	return pGameTrap;
}

//-------------------------------------------------------------
//------------------------------ 
void	CSceneTrapMgr::removeObject		(uint32 uUnitID)
{
	HASH_BUFF::iterator _pos = m_hashTrap.find(uUnitID);
	if(_pos == m_hashTrap.end())
		return;

	g_clTrapPool.release(_pos->second);
	m_hashTrap.erase(_pos);
}

//-------------------------------------------------------------
//------------------------------ 
void	CSceneTrapMgr::processObject(uint64 uTick, uint64 uTime)
{
}
//-------------------------------------------------------------
//------------------------------ 
CSceneTrap*	CSceneTrapMgr::findTrap	(uint32 uTrapID)
{
	HASH_BUFF::iterator _pos =  m_hashTrap.find(uTrapID);
	return _pos != m_hashTrap.end() ? _pos->second : NULL;
}