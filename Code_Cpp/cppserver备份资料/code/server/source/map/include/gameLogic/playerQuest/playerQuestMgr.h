/*----------------- playerQuestMgr.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 晁旭升
* Version      : V1.0
* Date         : 2016/4/13 17:26:10
*--------------------------------------------------------------
*任务管理器
*------------------------------------------------------------*/
#pragma once
#include "gameLogic/gamePlayerFun/gamePlayerFun.h"
#include "memoryPool/mapPool.h"
#include "gameLogic/playerQuest/playerQuest.h"
/*************************************************************/
class CSceneMonster;
//-------------------------------------------------------------
//------------------------------
class CPlayerQuestMgr	: public CGamePlayerFun
{
	typedef	CMapPool<uint32,CPlayerQuest>	MAP_QUEST;
	typedef	stl_set<CPlayerQuest*>			SET_QUEST;
private:
	MAP_QUEST								m_mapQuest;			//当前正在进行的任务信息 <任务id，任务>
	stl_map<uint32,uint32>					m_mapCompleted;		//已经完成的任务计数 <任务id，完成次数>

	stl_map<uint64,SET_QUEST>				m_mapQuestEvent;	//任务事件触发 <事件键值，正在进行中的任务列表>
public:
	CPlayerQuestMgr(CScenePlayer& clPlayer);
	~CPlayerQuestMgr();
public:
	virtual	void	initialize				();
	virtual	bool	loadFromProtobuf		(const PBPlayerData&pbPlayer);
	virtual	bool	saveToProtobuf			(PBPlayerData&pbPlayer);
	virtual	_emGameSystemType	getFunType	()const	{	return GameSystemType_Quest;	}

	virtual void	onClientGo				();
	virtual	void	onBuildComplete			();
	virtual void	cleanupDaily			();
public:
	void		saveToProtobuf					(PBPlayerQuest& pbPlayerQuest);
	bool		canAccept						(const _stQuestModel* pModel,uint8& uCode);
	bool		canBranchAccept					(const _stQuestModel* pModel,uint8 & uCode);
	bool		acceptQuest						(const _stQuestModel* pModel);
	bool		giveupQuest						(uint32 uQuestID,uint8& uCode);
	bool		completeQuest					(uint32 uQuestID,uint8& uCode);
	bool		acceptQuest						(uint32 uQuestID,uint8& uCode);
	bool		acceptTreasureQuest				(uint32 uQuestID,uint32 uItemID);
	bool		updateQuestItem					(uint32 uQuestID,uint32 uTarID,uint32 uCount,uint8& uCode);
	uint32		getQuestItem					(uint32 uQuestID,uint32 uTarID);
	inline CPlayerQuest*	getQuest			(uint32 uQuestID)	{ return m_mapQuest.find(uQuestID); };
	bool		isComplete						(uint32 uQuestID);
	bool		gmAccept						(uint32 uQuestID);
	bool		gmReset							();
	void		clearPlaneQuest					();	//清除无效的位面任务记录（一个位面的任务 只能全部完成或者全部不完成）
	void		clearClassMatesQuest			(); // 清除师门任务
	void		clearTreasureQuest				(); // 清除藏宝图任务

//任务目标完成事件相关
public:
	void	fireQuestEvent					(uint8 uType,uint32 uSubType,uint32	uCount = 1);
	void	regQuestEvent					(uint8 uType,uint32 uSubType,CPlayerQuest* pQuest);
	void	unregQuestEvent					(CPlayerQuest* pQuest);

	bool	haveQuestEvent					(uint8 uType,uint32 uSubType);
private:
	uint64	makeEventKey					(uint8 uType,uint32 uSubType);

	bool	isNeedClearPlaneQuest			(uint32 uQuestID);
	bool	isNeedClearClassMastesQuest		(uint32 uQuestID);
	bool	isCompleteMastesQuest			();
	void	refreshQuestVisibleMonster		();
//外部事件回掉
public:
	void	callback_updateItemNum			(uint32 uItemID);
	void	callback_depot					(CSceneMonster* pMonster);
	void	callback_killMonster			(CSceneMonster* pMonster);
	void	callback_enterRegion			(uint32 uQuestRegionID);
	void	callback_useItem				(uint32 uItemID,uint16 uCount);
	void	callback_upLevel				();	
	void	callback_promote				();
	void	callback_force					();
	void	callback_identity				();
	void	callback_identityDaily			();
	void	callback_factionDaily			();
	void	callback_completeCopyMap		(uint16 uCopyMapID, uint16 uCount);
	void	callback_completeSceneFly		(uint32 uQuestRegionID );

	void	callback_completeEnhance				();
	void	callback_completeItemForge				();
	void	callback_completeItemMake				();
	void	callback_completeQianxiu				();
	void	callback_completeWulueActivated			();
	void	callback_completeBingfuUpgrade			();
	void	callback_completeIdentitySkillUpgrade	();
	void	callback_completeTreasure				();
	void	callback_completePvp5					();
	void	callback_completeWorldBoss				();
	void	callback_completeFactionDrinkWine		();
	void	callback_completeAddFaction				();

	void	callback_completeTakeASeat				();
	void	callback_completeStore					();
	void	callback_completeHunt					();
	void	callback_completeNewFriend				();
	

// 网络消息处理
protected:
	bool	handle_Accept_Ask				(uint8 uEventID,CNetStream& clStream);
	bool	handle_Complete_Ask				(uint8 uEventID,CNetStream& clStream);
	bool	handle_GiveUp_Ask				(uint8 uEventID,CNetStream& clStream);
	bool	handle_UpdateItem_Ask			(uint8 uEventID,CNetStream& clStream);
};
