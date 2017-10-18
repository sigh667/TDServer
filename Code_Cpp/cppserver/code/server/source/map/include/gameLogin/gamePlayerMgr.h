/*----------------- gamePlayerMgr.h
*
* Copyright (C): 2011  LiuLei
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/9/13 10:03:59
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "timer/timers.h"
#include "memoryPool/mapPool.h"
#include "singleton/singleton.h"
#include "gameLogic/sceneObject/scenePlayer.h"
#include "protocol/PlayerStruct.pb.h"
/*************************************************************/
struct _stPlayerData;
struct _stNetHost;
class CClientPacket;
class CGamePlayerMgr	: public CMapPool<uint32,CScenePlayer>
						, public CTimerObject
						, _single_public(CGamePlayerMgr)
{
private:
	_single_friend(CGamePlayerMgr);

protected:
	typedef CMapPool<uint32,CScenePlayer>	Parent;
	typedef stl_map<uint32,CScenePlayer*>	MAP_PLAYER;

private:
	bool		m_bShutdown;
	MAP_PLAYER	m_mapLogoutPlayer;
	MAP_PLAYER	m_mapWaitLogin;

private:
	CGamePlayerMgr();

public:
	CScenePlayer*findPlayer			(uint32 uPlayerID);
	bool		onlinePlayer		(PBPlayerData&rPBPlayer,CClientPacket*pPacket,uint16 uToSceneID,uint64 uToSceneDynID,bool bLogin);
	void		shutdown			();
	void		logoutWorldPlayer	(uint16 uLogicWorldID);

public:
	bool		setPlayerPacket		(CScenePlayer*pPlayer,CClientPacket*pPacket);
public:
	//--- 角色删除
	void		onPlayerRemove		(uint32 uPlayerID,uint64 uLoginSN,bool bLogout = true);
public:
	/*----->{ 定时器处理 }*/ 
	virtual void	timerProcess	();
};
extern CGamePlayerMgr* g_pGamePlayerMgr;
