/*----------------- copymapMgr.h
*
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ³ÂÁÁ
* Version      : V1.0
* Date         : 2016/5/16 18:05:08
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_set.h"
#include "stl/std_map.h"
#include "def_struct/dartcarStruct.h"
#include "singleton/singleton.h"
#include "gameScene/copymapBase.h"
#include "memoryPool/memoryPool.h"
#include "gameScene/copymapNormal.h"
//#include "gameScene/copymapArrestCapture.h"
//#include "gameScene/copymapArrestNormal.h"
//#include "gameScene/copymapArrestDialog.h"
//#include "gameScene/copymapArrestGuard.h"
//#include "gameScene/copymapArrestTrap.h"
//#include "gameScene/copymapDartRob.h"
//#include "gameScene/copymapTreasureWenqu.h"
//#include "gameScene/copymapTreasureLiangyi.h"
//#include "gameScene/copymapTreasureSixiang.h"
//#include "gameScene/copymapRewardNormal.h"
//#include "gameScene/copymapRewardZhenshen.h"
//#include "gameScene/copymapRewardThug.h"
//#include "gameScene/copymapFactionBoss.h"
//#include "gameScene/copymapClimbTower.h"
//#include "gameScene/copymapGwTrain.h"
//#include "gameScene/copymapPvpBanner.h"
//#include "gameScene/copymapManorLobby.h"
//#include "gameScene/copymapManorBattleField.h"
//#include "gameScene/copymapSinglePvp.h"
//#include "gameScene/copymapFactionHome.h"
/*************************************************************/
struct _stCopymapModel;
class CCopymapMgr :_single_public(CCopymapMgr)
{
private:
	_single_friend(CCopymapMgr);

private:
	CMemoryPool<CCopymapNormal>				m_clCopymapNormal;
	//CMemoryPool<CCopymapArrestCapture>		m_clCopymapArrestCapture;
	//CMemoryPool<CCopymapArrestNormal>		m_clCopymapArrestNormal;
	//CMemoryPool<CCopymapArrestDialog>		m_clCopymapArrestDialog;
	//CMemoryPool<CCopymapArrestGuard>		m_clCopymapArrestGuard;
	//CMemoryPool<CCopymapArrestTrap>			m_clCopymapArrestTrap;
	//CMemoryPool<CCopymapDartRob>			m_clCopymapDartRob;
	//CMemoryPool<CCopymapTreasureWenqu>		m_clCopymapTreasureWenqu;
	//CMemoryPool<CCopymapTreasureLiangyi>	m_clCopymapTreasureLiangyi;
	//CMemoryPool<CCopymapTreasureSixiang>	m_clCopymapTreasureSixiang;
	//CMemoryPool<CCopymapRewardNormal>		m_clCopymapRewardNormal;
	//CMemoryPool<CCopymapRewardZhenshen>		m_clCopymapRewardZhenshen;
	//CMemoryPool<CCopymapRewardThug>			m_clCopymapRewardThug;
	//CMemoryPool<CCopymapFactionBoss>		m_clCopymapFactionBoss;
	//CMemoryPool<CCopymapClimbTower>			m_clCopymapClimbTower;
	//CMemoryPool<CCopymapGreatwallTrain>		m_clCopymapGwTrain;  
	//CMemoryPool<CCopymapPvpBanner>			m_clCopymapPvpBanner;  
	//CMemoryPool<CCopymapManorLobby>			m_clCopymapManorLobby;  
	//CMemoryPool<CCopymapManorBattleField>	m_clCopymapManorBattleField;  
	//CMemoryPool<CCopymapSinglePvp>			m_clCopymapSinglePvp;
	//CMemoryPool<CCopymapFactionHome>		m_clCopymapFactionHome;

public:
	CCopymapMgr();

public:
	CCopymapBase*		alloc		(_stCopymapModel*pModel/*, const _stCopyMapCreateParam* pCMCP*/);
	void				free		(CCopymapBase* pCopymap);
};

extern CCopymapMgr* g_pCopymapMgr;