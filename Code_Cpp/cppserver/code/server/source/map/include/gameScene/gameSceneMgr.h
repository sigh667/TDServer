/*------------- gameSceneMgr.h
* Copyright (C): 2011  
* Author       : ����Դ
* Version      : V1.01
* Date         : 2011/3/23 11:54:30
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "gameScene.h"
#include "timer/timers.h"
#include "singleton/singleton.h"
#include "memoryPool/mapPool.h"
/*************************************************************/
class CGameSceneCell;
class CGameSceneMgr	: public CTimerObject
					, _single_public(CGameSceneMgr)
{
private:
	_single_friend(CGameSceneMgr);

public:
	typedef CMapPool<uint16,CGameScene>		MAP_GAME_MAP;

protected:
	MAP_GAME_MAP	m_mapGamemap;

private:
	CGameSceneMgr();
	~CGameSceneMgr();

public:
	CGameScene*		findSceneMap					(uint16 uSceneID);

public: 
	bool			start							();
	void			shutdown						();
public:
	// ���ҳ��� ͨ��Ψһ��ʶ
	CGameSceneCell*	findSceneCell_ByDynID			(uint16 uSceneID,uint64 uDynID);

	// ���ҳ��� ͨ��������(������id ���ͼ���Ͷ���ͬ ��ͨ��ͼ�Ļ����ǹ���id ����ͼ�Ļ����ǰ��id ...)
	CGameSceneCell*	findSceneCell_ByCellID			(uint16 uSceneID,uint32 uCellID);

	CGameSceneCell*	createSceneCell					(uint16 uSceneID,uint64 uDynID,uint32 uCellID,const PBCreateCellParam&pbParam);
	bool			closeSceneCell					(uint16 uSceneID,uint64 uDynID);

	bool			playerEnterScene				(CScenePlayer*pPlayer,uint16 uSceneID,uint64 uDynID);
public:
	virtual void	timerProcess();

public:
	bool			printData	();
};
extern CGameSceneMgr* g_pGameSceneMgr;
