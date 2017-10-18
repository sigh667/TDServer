/*------------- gameHurtMgr.cpp
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ³ÂÌÚ
* Version      : V1.01
* Date         : 2016/04/21
*
*/ 
/***************************************************************
* 
***************************************************************/
#include "gameLogic/gameHurt/gameHurtMgr.h"
#include "gameLogic/sceneObject/sceneUnit.h"
#include "gameScene/gameSceneCell.h"
#include "def_struct/teamStruct.h"
/*************************************************************/

//-------------------------------------------------------------
//------------------------------ 
CGameHurtMgr::CGameHurtMgr()
{
	initialize(NULL);
}

//-------------------------------------------------------------
//------------------------------ 
CGameHurtMgr::~CGameHurtMgr()
{

}

//-------------------------------------------------------------
//------------------------------ 
void CGameHurtMgr::initialize(CSceneUnit* pUnit)
{
	m_pOwner	= pUnit;
	m_uHurtSum	= 0;
	m_mapHurt.clear();
	m_pFirstPlayer = NULL;
}

//-------------------------------------------------------------
//------------------------------ 
void CGameHurtMgr::updateHurt(CScenePlayer* pPlayer, int32 nHP)
{
	if (!pPlayer || m_pOwner == pPlayer ||  nHP <= 0)
		return;

	if (!m_pFirstPlayer)
		m_pFirstPlayer = pPlayer;

	uint32 uPlayerID		= pPlayer->getPlayerID();
	_stHurt& stHurt		= m_mapHurt[uPlayerID];
	stHurt.uObjectID	= uPlayerID;
	stHurt.uHurt		+= nHP;
	stHurt.uHurtTime	= getTime();
	m_uHurtSum			+= nHP;
}

//-------------------------------------------------------------
//------------------------------ 
void CGameHurtMgr::clearHurt()
{
	m_uHurtSum	= 0;
	m_mapHurt.clear();
}

//-------------------------------------------------------------
//------------------------------ 
float32 CGameHurtMgr::getHurtRatio(uint64 uHurt)
{
	if ( !m_uHurtSum )
		return 0.0f;
	return (float32)((float64)uHurt / m_uHurtSum);
}

//-------------------------------------------------------------
//------------------------------ 
CScenePlayer* CGameHurtMgr::getHurtMaxPlayer()
{
	CGameSceneCell* pCell = m_pOwner ? m_pOwner->getSceneCell() : NULL;
	if (!pCell)
		return NULL;

	stl_vector<const _stHurt*> vecHurt;
	MAP_HURT::iterator _pos = m_mapHurt.begin();
	MAP_HURT::iterator _end = m_mapHurt.end();
	for (; _pos != _end; ++_pos)
	{
		vecHurt.push_back(&_pos->second);
	}

	if (vecHurt.empty())
		return NULL;

	std::sort(vecHurt.begin(), vecHurt.end(), _stHurtSort());
	return pCell->findPlayer(vecHurt[0]->uObjectID);
}

//-------------------------------------------------------------
//------------------------------ 
void CGameHurtMgr::clear()
{
	m_uHurtSum	= 0;
	m_mapHurt.clear();
}
