/*----------------- playerQuestMgr.cpp
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 晁旭升
* Version      : V1.0
* Date         : 2016/4/13 17:26:18
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "log/log.h"
#include "gameLogic/playerQuest/playerQuestMgr.h"
#include "gameLogic/sceneObject/scenePlayer.h"
#include "gameLogic/playerSystemSwitch/playerSystemSwitch.h"
//#include "gameLogic/playerActivity/playerActivity.h"
#include "main/gameServer.h"
#include "protocol/CASMsg.pb.h"
#include "protocol/PlayerStruct.pb.h"
#include "protocol/MsgResult.pb.h"
#include "cfg_data/questData/questData.h"
#include "cfg_data/constantData/constantData.h"
#include "gameLogic/sceneObject/sceneMonster.h"
//#include "gameLogic/playerTitle/playerTitle.h"
#include "cfg_data/monsterData/monsterModel.h"
#include "cfg_data/birthData/birthData.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
CPlayerQuestMgr::CPlayerQuestMgr(CScenePlayer& clPlayer):CGamePlayerFun(clPlayer)
{
	registerHandler(C2S_Quest_Accept,(CGamePlayerFun::_handleSubMsg)&CPlayerQuestMgr::handle_Accept_Ask);
	registerHandler(C2S_Quest_Complete,(CGamePlayerFun::_handleSubMsg)&CPlayerQuestMgr::handle_Complete_Ask);
	registerHandler(C2S_Quest_UpdateItem,(CGamePlayerFun::_handleSubMsg)&CPlayerQuestMgr::handle_UpdateItem_Ask);
	registerHandler(C2S_Quest_GiveUp,(CGamePlayerFun::_handleSubMsg)&CPlayerQuestMgr::handle_GiveUp_Ask);
	//g_pEventCenter->Subscribe(EVENT_MONSTER_DIE, this);
}

//-------------------------------------------------------------
//------------------------------ 
CPlayerQuestMgr::~CPlayerQuestMgr()
{

}

//-------------------------------------------------------------
//------------------------------ 
void	CPlayerQuestMgr::initialize			()
{
	CGamePlayerFun::initialize();
	m_mapQuest.clear();
	m_mapCompleted.clear();
	m_mapQuestEvent.clear();
}
//-------------------------------------------------------------
//------------------------------ 
bool	CPlayerQuestMgr::loadFromProtobuf	(const PBPlayerData&pbPlayer)
{
	m_mapQuest.clear();
	m_mapCompleted.clear();
	m_mapQuestEvent.clear();

	//当前正在进行的任务信息
	const PBPlayerQuest& pbPlayerQuest = pbPlayer.playerquest();
	for ( uint32 i = 0 ; i < (uint32)pbPlayerQuest.quest_size();++i)
	{
		const PBQuest& pbQuest = pbPlayerQuest.quest(i);
		CPlayerQuest* pQuest = m_mapQuest.allocate(pbQuest.questid());
		if ( !pQuest )
			continue;

		pQuest->initialize(this);
		if ( !pQuest->load(pbQuest) )
			m_mapQuest.release(pbQuest.questid());
	}

	//已经完成的任务计数
	for ( uint32 i = 0 ; i < (uint32)pbPlayerQuest.completedinfo_size();++i)
	{
		const PBQuestCompleted& pbQuestCompleted = pbPlayerQuest.completedinfo(i);
		m_mapCompleted[pbQuestCompleted.questid()] = pbQuestCompleted.count();
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CPlayerQuestMgr::saveToProtobuf		(PBPlayerData&pbPlayer)
{
	PBPlayerQuest* pbPlayerQuest = pbPlayer.mutable_playerquest();
	if ( !pbPlayerQuest )
		return false;

	saveToProtobuf(*pbPlayerQuest);
	return true;
}
//-------------------------------------------------------------
//------------------------------
void	CPlayerQuestMgr::saveToProtobuf					(PBPlayerQuest& pbPlayerQuest)
{
	{//当前正在进行的任务信息
		MAP_QUEST::iterator _pos = m_mapQuest.begin();
		MAP_QUEST::iterator _end = m_mapQuest.end();
		for ( ; _pos != _end ;++_pos )
		{
			CPlayerQuest* pQuest = _pos->second;
			if ( !pQuest )
				continue;

			PBQuest* pbQuest = pbPlayerQuest.add_quest();
			if (!pbQuest )
				continue;

			pQuest->save(*pbQuest);
		}
	}
	{//已经完成的任务计数
		stl_map<uint32,uint32>::iterator _pos = m_mapCompleted.begin();
		stl_map<uint32,uint32>::iterator _end = m_mapCompleted.end();
		for ( ; _pos != _end ;++_pos )
		{
			PBQuestCompleted* pbQuestCompleted = pbPlayerQuest.add_completedinfo();
			if ( !pbQuestCompleted )
				continue;

			pbQuestCompleted->set_questid(_pos->first);
			pbQuestCompleted->set_count(_pos->second);
		}
	}
}
//-------------------------------------------------------------
//------------------------------
uint64	CPlayerQuestMgr::makeEventKey					(uint8 uType,uint32 uSubType)
{
	return MAKE_UINT64(uType,uSubType);
}
//-------------------------------------------------------------
//------------------------------
void	CPlayerQuestMgr::fireQuestEvent					(uint8 uType,uint32 uSubType,uint32	uCount)
{
	uint64 uKey = makeEventKey(uType,uSubType);
	auto _pos = m_mapQuestEvent.find(uKey);
	if ( _pos == m_mapQuestEvent.end() )
		return ;

	SET_QUEST& setQuest = _pos->second;
	if ( setQuest.empty() )
	{
		m_mapQuestEvent.erase(_pos);
		return ;
	}

	SET_QUEST setTmpQuest = setQuest; // 因为onQuestEvent 可能会反注册事件 所以必须生成临时变量
	for ( auto _pos = setTmpQuest.begin() ; _pos != setTmpQuest.end() ; ++_pos )
	{
		CPlayerQuest* pQuest = *_pos;
		if ( !pQuest )
			continue;

		pQuest->onQuestEvent(uType,uSubType,uCount);
	}
}
//-------------------------------------------------------------
//------------------------------
void	CPlayerQuestMgr::regQuestEvent					(uint8 uType,uint32 uSubType,CPlayerQuest* pQuest)
{
	if ( !pQuest )
		return ;

	uint64 uKey = makeEventKey(uType,uSubType);
	SET_QUEST& setQuest = m_mapQuestEvent[uKey];
	setQuest.insert(pQuest);
}
//-------------------------------------------------------------
//------------------------------
bool	CPlayerQuestMgr::haveQuestEvent					(uint8 uType,uint32 uSubType)
{
	uint64 uKey = makeEventKey(uType,uSubType);
	stl_map<uint64,SET_QUEST>::iterator _pos = m_mapQuestEvent.find(uKey);
	if ( _pos == m_mapQuestEvent.end() )
		return false;

	return !_pos->second.empty();
}
//-------------------------------------------------------------
//------------------------------
void	CPlayerQuestMgr::unregQuestEvent				(CPlayerQuest* pQuest)
{
	for ( stl_map<uint64,SET_QUEST>::iterator _pos = m_mapQuestEvent.begin() ; _pos != m_mapQuestEvent.end() ; )
	{
		stl_map<uint64,SET_QUEST>::iterator _iter = _pos++;

		SET_QUEST& setQuest = _iter->second;
		setQuest.erase(pQuest);

		if ( setQuest.empty() )
			m_mapQuestEvent.erase(_iter);
	}
}
//-------------------------------------------------------------
//------------------------------
bool	CPlayerQuestMgr::canAccept						(const _stQuestModel* pModel,uint8& uCode)
{
	if ( !pModel )
		return false;

	// 正在进行中...
	if (m_mapQuest.find(pModel->uID))
	{
		uCode = R_ResultQuest_Accepted;
		return false;
	}

	// 已经完成过的
	if (pModel->uType != QuestType_Treasure)
	{
		if (m_mapCompleted.find(pModel->uID) != m_mapCompleted.end() )
		{
			uCode = R_ResultQuest_Accepted;
			return false;
		}
	}

	// 条件未达成
	if (!m_clPlayer.canPreCondition(pModel->mapPrecondition))
	{
		uCode = R_ResultQuest_PreCondition;
		return false;
	}

	//  师门任务， 是不已经完成
	if (pModel->uType == QuestType_ClassMates && isCompleteMastesQuest())
	{
		uCode = R_ResultQuest_MasterComplete;
		return false;
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------
bool	CPlayerQuestMgr::acceptQuest						(const _stQuestModel* pModel)
{
	if ( !pModel )
		return false;

	if (m_mapQuest.find(pModel->uID))
		return false;

	CPlayerQuest* pQuest = m_mapQuest.allocate(pModel->uID);
	if ( !pQuest )
		return false;

	pQuest->initialize(this);
	if ( !pQuest->build(pModel) )
	{
		m_mapQuest.release(pModel->uID);
		return false;
	}

	PBG2CQuestAccept ack;
	PBQuest* pbQuest = ack.mutable_quest();
	if (pbQuest)
	{
		if (pModel->uType == QuestType_ClassMates)	 // 师门任务
			pQuest->randomQuestItem(*pbQuest); 
		else 
			pQuest->save(*pbQuest);	
	}

	m_clPlayer.sendMsg(P_S2C_Protocol_Quest,S2C_Quest_Accept,&ack);

	refreshQuestVisibleMonster();
	return true;
}
//-------------------------------------------------------------
//------------------------------
void	CPlayerQuestMgr::refreshQuestVisibleMonster		()
{
	m_clPlayer.refreshVisibleMonster();
}
//-------------------------------------------------------------
//------------------------------
void	CPlayerQuestMgr::callback_depot				(CSceneMonster* pMonster)
{
	if ( !pMonster )
		return ;

	const _stMonsterModel* pMonsterModel = pMonster->getMonsterModel();
	if ( !pMonsterModel )
		return ;

	fireQuestEvent(QuestTargetType_DepotGoods,pMonsterModel->uMonsterID);
}
//-------------------------------------------------------------
//------------------------------
void	CPlayerQuestMgr::callback_updateItemNum			(uint32 uItemID)
{
	fireQuestEvent(QuestTargetType_CollectGoods,uItemID);
}
//-------------------------------------------------------------
//------------------------------
void	CPlayerQuestMgr::callback_killMonster			(CSceneMonster* pMonster)
{
	if ( !pMonster )
		return ;

	const _stMonsterModel* pMonsterModel = pMonster->getMonsterModel();
	if ( !pMonsterModel )
		return ;

	fireQuestEvent(QuestTargetType_KillMonster,pMonsterModel->uMonsterID);

	fireQuestEvent(QuestTargetType_MonsterDropGoods,pMonsterModel->uMonsterID);

	fireQuestEvent(QuestTargetType_Weapons,pMonsterModel->uMonsterID);
}
//-------------------------------------------------------------
//------------------------------
void	CPlayerQuestMgr::callback_enterRegion			(uint32 uQuestRegionID)
{
	fireQuestEvent(QuestTargetType_Location,uQuestRegionID);
	fireQuestEvent(QuestTargetType_EscortNPC,uQuestRegionID);
	fireQuestEvent(QuestTargetType_SceneFly,uQuestRegionID);
}
//-------------------------------------------------------------
//------------------------------
void	CPlayerQuestMgr::callback_useItem				(uint32 uItemID,uint16 uCount)
{
	fireQuestEvent(QuestTargetType_UseItem,uItemID,uCount);
}
//-------------------------------------------------------------
//------------------------------
void	CPlayerQuestMgr::callback_upLevel()
{
	fireQuestEvent(QuestTargetType_UpgradeLevel,0,1);
	fireQuestEvent(QuestTargetType_UpgradeLevel1,0,1);
}
//-------------------------------------------------------------
//------------------------------
void	CPlayerQuestMgr::callback_promote()
{
	fireQuestEvent(QuestTargetType_Promotion, 0, 1);
	fireQuestEvent(QuestTargetType_PromotionSuccess, 0, 1);
}
//-------------------------------------------------------------
//------------------------------
void	CPlayerQuestMgr::callback_force()
{
	fireQuestEvent(QuestTargetType_Force, 0);
}
//-------------------------------------------------------------
//------------------------------
void	CPlayerQuestMgr::callback_identity()
{
	fireQuestEvent(QuestTargetType_Identity, 0);
}
//-------------------------------------------------------------
//------------------------------
void	CPlayerQuestMgr::callback_identityDaily()
{
	fireQuestEvent(QuestTargetType_IdentityDaily, 0);
}
//-------------------------------------------------------------
//------------------------------
void	CPlayerQuestMgr::callback_factionDaily()
{
	fireQuestEvent(QuestTargetType_FactionDaily, 0);
}
//-------------------------------------------------------------
//------------------------------
void	CPlayerQuestMgr::callback_completeCopyMap(uint16 uCopyMapID, uint16 uCount)
{
	fireQuestEvent(QuestTargetType_CompleteCopymap, uCopyMapID, uCount);
	fireQuestEvent(QuestTargetType_Challenge, uCopyMapID, uCount);
	fireQuestEvent(QuestTargetType_CopymapCount, uCopyMapID, uCount);
}
//-------------------------------------------------------------
//------------------------------

void	CPlayerQuestMgr::callback_completeSceneFly(uint32 uQuestRegionID )
{
	fireQuestEvent(QuestTargetType_SceneFly,uQuestRegionID);
}

//-------------------------------------------------------------
//------------------------------
void CPlayerQuestMgr::callback_completeEnhance()
{
	fireQuestEvent(QuestTargetType_Enhance, 0);
}

//-------------------------------------------------------------
//------------------------------
void CPlayerQuestMgr::callback_completeItemForge()
{
	fireQuestEvent(QuestTargetType_ItemForge, 0);
}

//-------------------------------------------------------------
//------------------------------
void CPlayerQuestMgr::callback_completeItemMake()
{
	fireQuestEvent(QuestTargetType_ItemMake, 0);
}

//-------------------------------------------------------------
//------------------------------
void CPlayerQuestMgr::callback_completeQianxiu()
{
	fireQuestEvent(QuestTargetType_Qianxiu, 0);
}

//-------------------------------------------------------------
//------------------------------
void CPlayerQuestMgr::callback_completeWulueActivated()
{
	fireQuestEvent(QuestTargetType_WulueActivated, 0);
}

//-------------------------------------------------------------
//------------------------------
void CPlayerQuestMgr::callback_completeBingfuUpgrade()
{
	fireQuestEvent(QuestTargetType_BingfuUpgrade, 0);
}

//-------------------------------------------------------------
//------------------------------
void CPlayerQuestMgr::callback_completeIdentitySkillUpgrade()
{
	fireQuestEvent(QuestTargetType_IdentitySkillUpgrade, 0);
}

//-------------------------------------------------------------
//------------------------------
void CPlayerQuestMgr::callback_completeTreasure()
{
	fireQuestEvent(QuestTargetType_Treasure, 0);
}

//-------------------------------------------------------------
//------------------------------
void CPlayerQuestMgr::callback_completePvp5()
{
	fireQuestEvent(QuestTargetType_Pvp5, 0);
}

//-------------------------------------------------------------
//------------------------------
void CPlayerQuestMgr::callback_completeWorldBoss()
{
	fireQuestEvent(QuestTargetType_WorldBoss, 0);
}

//-------------------------------------------------------------
//------------------------------
void CPlayerQuestMgr::callback_completeFactionDrinkWine()
{
	fireQuestEvent(QuestTargetType_FactionDrinkWine, 0);
}

//-------------------------------------------------------------
//------------------------------
void CPlayerQuestMgr::callback_completeTakeASeat()
{
	fireQuestEvent(QuestTargetType_TakeASeat, 0);
}

//-------------------------------------------------------------
//------------------------------
void CPlayerQuestMgr::callback_completeStore()
{
	fireQuestEvent(QuestTargetType_Store, 0);
}

//-------------------------------------------------------------
//------------------------------
void CPlayerQuestMgr::callback_completeHunt()
{
	fireQuestEvent(QuestTargetType_Hunt, 0);
	fireQuestEvent(QuestTargetType_HuntCount, 0);
}

//-------------------------------------------------------------
//------------------------------
void CPlayerQuestMgr::callback_completeNewFriend()
{
	fireQuestEvent(QuestTargetType_NewFriend, 0);
}

//-------------------------------------------------------------
//------------------------------
void CPlayerQuestMgr::callback_completeAddFaction()
{
	fireQuestEvent(QuestTargetType_AddFaction, 0);
}


//-------------------------------------------------------------
//------------------------------
bool	CPlayerQuestMgr::giveupQuest					(uint32 uQuestID,uint8& uCode)
{
	CPlayerQuest* pQuest = m_mapQuest.find(uQuestID);
	if ( !pQuest )
	{
		uCode = R_ResultQuest_NoQuest;
		return false;
	}

	const _stQuestModel* pModel = pQuest->getModel();
	if ( !pModel )
	{
		uCode = R_ResultQuest_Fail;
		return false;
	}

	unregQuestEvent(pQuest);
	m_mapQuest.release(uQuestID);

	uCode = R_ResultQuest_Succeed;
	PBG2CQuestGiveUp ack;
	ack.set_questid(uQuestID);
	m_clPlayer.sendMsg(P_S2C_Protocol_Quest,S2C_Quest_GiveUp,&ack,uCode);
	return true;
}
//-------------------------------------------------------------
//------------------------------
bool	CPlayerQuestMgr::completeQuest					(uint32 uQuestID,uint8& uCode)
{
	CPlayerQuest* pQuest = m_mapQuest.find(uQuestID);
	if ( !pQuest )
	{
		uCode = R_ResultQuest_NoQuest;
		return false;
	}

	const _stQuestModel* pModel = pQuest->getModel();
	if ( !pModel )
	{
		uCode = R_ResultQuest_Fail;
		return false;
	}

	if ( (!pModel->check_mark(QuestMark_ClientComplete)) &&  (!pQuest->isDone())  )
	{
		uCode = R_ResultQuest_NoReach;
		return false;
	}


	pQuest->onComplete();
	unregQuestEvent(pQuest);
	m_mapQuest.release(uQuestID);
	refreshQuestVisibleMonster();

	// 增加完成计数
	if (pModel->uType != QuestType_Treasure)
	{
		++m_mapCompleted[uQuestID];
	}

	// 更新系统功能开关
	if (m_clPlayer.getPlayerSystemSwitch())
		m_clPlayer.getPlayerSystemSwitch()->callback_completeQuest(uQuestID);

	////称号
	//if (m_clPlayer.getPlayerTitle())
	//	m_clPlayer.getPlayerTitle()->callback_completeQuest(uQuestID);

	//藏宝图
	if (QuestType_Treasure == pModel->uType)
	{
		m_clPlayer.callbackCompleteTreasureQuest(uQuestID,pQuest->getParam());
	}
	
	//// 师门任务 回调活动
	//if (pModel->uType == QuestType_ClassMates)
	//{
	//	CPlayerActivity * pActivity = m_clPlayer.getPlayerActivity();
	//	if (!pActivity)
	//		return false;
	//	pActivity->callback_quest(QuestType_ClassMates);

	//}
	////狩猎
	//if (QuestType_Shoulie == pModel->uType)
	//{
	//	callback_completeHunt();
	//}
	uCode = R_ResultQuest_Succeed;
	PBG2CQuestComplete ack;
	ack.set_questid(uQuestID);
	m_clPlayer.sendMsg(P_S2C_Protocol_Quest,S2C_Quest_Complete,&ack,uCode);

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool	CPlayerQuestMgr::acceptQuest					(uint32 uQuestID,uint8& uCode)
{
	const _stQuestModel* pModel = g_clQuestData.findQuestModel(uQuestID);
	if ( !pModel )
	{
		uCode = R_ResultQuest_NoQuest;
		return false;
	}

	if ( !canAccept(pModel,uCode))
		return false;

	acceptQuest(pModel);
	uCode = R_ResultQuest_Succeed;
	return true;
}
//-------------------------------------------------------------
//------------------------------
bool	CPlayerQuestMgr::acceptTreasureQuest					(uint32 uQuestID,uint32 uItemID)
{
	const _stQuestModel* pModel = g_clQuestData.findQuestModel(uQuestID);
	if ( !pModel || pModel->uType != QuestType_Treasure)
		return false;

	//清除宝藏任务
	clearTreasureQuest();

	uint8 uCode = R_ResultQuest_Succeed;
	if (!acceptQuest(uQuestID,uCode))
		return false;

	CPlayerQuest* pPlayerQuest = getQuest(uQuestID);
	if (!pPlayerQuest)
		return false;

	pPlayerQuest->setParam(uItemID);
	return true;
}

//-------------------------------------------------------------
//------------------------------
bool	CPlayerQuestMgr::updateQuestItem				(uint32 uQuestID,uint32 uTarID,uint32 uCount,uint8& uCode)
{
	CPlayerQuest* pQuest = m_mapQuest.find(uQuestID);
	if ( !pQuest )
	{
		uCode = R_ResultQuest_NoQuest;
		return false;
	}
	pQuest->setCurCount(uTarID,uCount);
	uCode = R_ResultQuest_Succeed;
	return true;
}
//-------------------------------------------------------------
//------------------------------
uint32	CPlayerQuestMgr::getQuestItem	(uint32 uQuestID,uint32 uTarID)
{
	CPlayerQuest* pQuest = m_mapQuest.find(uQuestID);
	return pQuest ? pQuest->getCurCount(uTarID) : 0;
}
//-------------------------------------------------------------
//------------------------------  领取任务
bool	CPlayerQuestMgr::handle_Accept_Ask	(uint8 uEventID, CNetStream& clStream)
{
	PBC2GQuestAccept ask;
	if (!clStream.ParseFromStream(ask))
		return false;

	uint8 uCode = R_ResultQuest_Fail;
	if ( !acceptQuest(ask.questid(),uCode) )
	{
		PBG2CQuestAccept ack;
		m_clPlayer.sendMsg(P_S2C_Protocol_Quest,S2C_Quest_Accept,&ack,uCode);
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------  完成任务
bool	CPlayerQuestMgr::handle_Complete_Ask	(uint8 uEventID, CNetStream& clStream)
{
	PBC2GQuestComplete ask;
	if (!clStream.ParseFromStream(ask))
		return false;

	uint8 uCode = R_ResultQuest_Fail;
	if ( !completeQuest(ask.questid(),uCode) )
	{
		PBG2CQuestComplete ack;
		ack.set_questid(ask.questid());
		m_clPlayer.sendMsg(P_S2C_Protocol_Quest,S2C_Quest_Complete,&ack,uCode);
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------  更新任务目标
bool	CPlayerQuestMgr::handle_UpdateItem_Ask	(uint8 uEventID, CNetStream& clStream)
{
	PBC2GQuestUpdateItem ask;
	if (!clStream.ParseFromStream(ask))
		return false;

	uint8 uCode = R_ResultQuest_Fail;
	if (!updateQuestItem(ask.questid(),ask.tarid(),ask.count(),uCode))
	{
		PBG2CQuestUpdateItem ack;
		ack.set_questid(ask.questid());
		ack.set_tarid(ask.tarid());

		uint32 uCurCount = 0;
		CPlayerQuest* pQuest = m_mapQuest.find(ask.questid());
		if ( pQuest )
			uCurCount = pQuest->getCurCount(ask.tarid());

		ack.set_count(uCurCount);
		m_clPlayer.sendMsg(P_S2C_Protocol_Quest,S2C_Quest_UpdateItem,&ack,uCode);
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------  放弃任务
bool	CPlayerQuestMgr::handle_GiveUp_Ask	(uint8 uEventID, CNetStream& clStream)
{
	PBC2GQuestGiveUp ask;
	if (!clStream.ParseFromStream(ask))
		return false;

	uint8 uCode = R_ResultQuest_Fail;
	if ( !giveupQuest(ask.questid(),uCode) )
	{
		PBG2CQuestGiveUp ack;
		ack.set_questid(ask.questid());
		m_clPlayer.sendMsg(P_S2C_Protocol_Quest,S2C_Quest_GiveUp,&ack,uCode);
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CPlayerQuestMgr::isComplete						(uint32 uQuestID)
{
	return m_mapCompleted.find(uQuestID) != m_mapCompleted.end() ;
}
//-------------------------------------------------------------
//------------------------------ 
void	CPlayerQuestMgr::onClientGo				()
{
	PBPlayerQuest pbPlayerQuest;
	saveToProtobuf(pbPlayerQuest);
	m_clPlayer.sendMsg(P_S2C_Protocol_Player,S2C_Player_QuestInfo	,&pbPlayerQuest);
}
//-------------------------------------------------------------
//------------------------------ 
void	CPlayerQuestMgr::onBuildComplete			()
{
	clearPlaneQuest();

	if ( m_mapQuest.empty() && m_mapCompleted.empty() )
	{
		const _stBirthModel* pBirthModel = g_clBirthData.find(m_clPlayer.getJob());
		CHECKL(NULL!=pBirthModel);
		//给初始任务
		if (pBirthModel && pBirthModel->uQuestID)
		{
			uint8 uCode = 0;
			acceptQuest(pBirthModel->uQuestID,uCode);
		}
	}
}

//-------------------------------------------------------------
//------------------------------
void	CPlayerQuestMgr::cleanupDaily()
{
	clearClassMatesQuest();
}

//-------------------------------------------------------------
//------------------------------
void	CPlayerQuestMgr::clearClassMatesQuest()
{
	{// 遍历已接任务列表（必须反响遍历 不然删不干净） 注意反向迭代器删除元素的用法
		for ( MAP_QUEST::reverse_iterator _pos = m_mapQuest.rbegin() ; _pos != m_mapQuest.rend(); )
		{
			uint32 uQuestID = _pos->first;
			if ( isNeedClearClassMastesQuest(uQuestID) )
			{
				m_mapQuest.release(uQuestID);
				refreshQuestVisibleMonster();
			}
			else
			{
				++_pos;
			}
		}
	}

	{// 遍历已经完成列表（必须反响遍历 不然删不干净） 注意反向迭代器删除元素的用法
		for  (  stl_map<uint32,uint32>::reverse_iterator _pos = m_mapCompleted.rbegin(); _pos != m_mapCompleted.rend() ; )
		{
			uint32 uQuestID = _pos->first;
			if (isNeedClearClassMastesQuest(uQuestID))
				m_mapCompleted.erase(uQuestID);
			else
				++_pos;
		}
	}
}

//-------------------------------------------------------------
//------------------------------清除藏宝图任务
void	CPlayerQuestMgr::clearTreasureQuest()
{
	VEC_U32 vecQuest;
	for ( MAP_QUEST::iterator _pos = m_mapQuest.begin() ; _pos != m_mapQuest.end(); ++_pos)
	{
		uint32 uQuestID = _pos->first;
		CPlayerQuest* pQuest = _pos->second; 
		if (pQuest && pQuest->getQuestType() == QuestType_Treasure)
		{
			vecQuest.push_back(uQuestID);
		}
	}

	//放弃
	uint8 uCode = 0;
	for (uint32 i = 0; i < vecQuest.size(); ++i)
	{
		giveupQuest(vecQuest[i],uCode);
	}
}
//-------------------------------------------------------------
//------------------------------ 
bool	CPlayerQuestMgr::isNeedClearPlaneQuest			(uint32 uQuestID)
{
	const _stQuestModel* pModel = g_clQuestData.findQuestModel(uQuestID);
	if (!pModel  || !pModel->uPlaneID || !pModel->uNextPlaneTaskID )
		return false;

	return !isComplete(pModel->uNextPlaneTaskID);
}

//-------------------------------------------------------------
//------------------------------
bool	CPlayerQuestMgr::isNeedClearClassMastesQuest	(uint32 uQuestID)
{
	const _stQuestModel* pModel = g_clQuestData.findQuestModel(uQuestID);
	if (!pModel  || pModel->uType != QuestType_ClassMates)
		return false;

	return !isComplete(pModel->uNextPlaneTaskID);
}
//-------------------------------------------------------------
//------------------------------ 
void	CPlayerQuestMgr::clearPlaneQuest					()
{
	{// 遍历已接任务列表（必须反响遍历 不然删不干净） 注意反向迭代器删除元素的用法
		for ( MAP_QUEST::reverse_iterator _pos = m_mapQuest.rbegin() ; _pos != m_mapQuest.rend(); )
		{
			uint32 uQuestID = _pos->first;
			if ( isNeedClearPlaneQuest(uQuestID) )
			{
				m_mapQuest.release(uQuestID);
				refreshQuestVisibleMonster();
			}
			else
			{
				++_pos;
			}
		}
	}


	{// 遍历已经完成列表（必须反响遍历 不然删不干净） 注意反向迭代器删除元素的用法
		for  (  stl_map<uint32,uint32>::reverse_iterator _pos = m_mapCompleted.rbegin(); _pos != m_mapCompleted.rend() ; )
		{
			uint32 uQuestID = _pos->first;
			if ( isNeedClearPlaneQuest(uQuestID) )
			{
				m_mapCompleted.erase(uQuestID);
			}
			else
			{
				++_pos;
			}
		}
	}
}

//-------------------------------------------------------------
//------------------------------
bool	CPlayerQuestMgr::isCompleteMastesQuest()
{
	// 遍历已经完成列表
	uint32 uCount = 0;
	for  (  auto _pos = m_mapCompleted.rbegin(); _pos != m_mapCompleted.rend() ; ++_pos)
	{
		uint32 uQuestID = _pos->first;
		const _stQuestModel* pModel = g_clQuestData.findQuestModel(uQuestID);
		if (!pModel  || pModel->uType != QuestType_ClassMates)
				continue; ;

		uCount++;
	}
	
	uint32 MasterQuestMax = (uint32)g_clConstantData.Quest_MasterQueseMax;
	if (uCount == MasterQuestMax) // 已经完成师门任务
		return true;
	
	return false;
}

//-------------------------------------------------------------
//------------------------------
bool		CPlayerQuestMgr::gmReset							()
{
	initialize();

	const _stBirthModel* pBirthModel = g_clBirthData.find(m_clPlayer.getJob());
	CHECKF(NULL!=pBirthModel);
	//给初始任务
	if (pBirthModel->uQuestID)
	{
		const _stQuestModel* pModel = g_clQuestData.findQuestModel(pBirthModel->uQuestID);
		CHECKF(NULL!=pModel);
		CPlayerQuest* pQuest = m_mapQuest.allocate(pModel->uID);
		if ( !pQuest )
			return false;

		pQuest->initialize(this);
		if ( !pQuest->build(pModel) )
		{
			m_mapQuest.release(pModel->uID);
			return false;
		}
	}

	PBPlayerQuest pbPlayerQuest;
	saveToProtobuf(pbPlayerQuest);
	m_clPlayer.sendMsg(P_S2C_Protocol_Quest,S2C_Quest_Refresh	,&pbPlayerQuest);

	refreshQuestVisibleMonster();
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool		CPlayerQuestMgr::gmAccept						(uint32 uQuestID)
{
	if (m_mapQuest.find(uQuestID))
		return true;

	const _stQuestModel* pModel = g_clQuestData.findQuestModel(uQuestID);
	if ( !pModel )
		return false;

	// 清理前置任务
	uint32 uPreQuestID = pModel->uPreTaskID;
	while(uPreQuestID)
	{
		const _stQuestModel* pPreModel = g_clQuestData.findQuestModel(uPreQuestID);
		if ( !pPreModel )
			break;

		m_mapCompleted[uPreQuestID] = 1;
		uPreQuestID = pPreModel->uPreTaskID;

		CPlayerQuest* pPreQuest = m_mapQuest.find(uPreQuestID);
		if (pPreQuest)
		{
			unregQuestEvent(pPreQuest);
			m_mapQuest.release(uPreQuestID);
		}
	}

	m_mapCompleted.erase(uQuestID);
	CPlayerQuest* pQuest = m_mapQuest.allocate(pModel->uID);
	if ( !pQuest )
		return false;

	pQuest->initialize(this);
	if ( !pQuest->build(pModel) )
	{
		m_mapQuest.release(pModel->uID);
		return false;
	}

	refreshQuestVisibleMonster();

	PBPlayerQuest pbPlayerQuest;
	saveToProtobuf(pbPlayerQuest);
	m_clPlayer.sendMsg(P_S2C_Protocol_Quest,S2C_Quest_Refresh	,&pbPlayerQuest);
	return true;
}