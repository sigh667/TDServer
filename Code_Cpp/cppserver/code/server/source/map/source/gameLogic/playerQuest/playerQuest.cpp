/*----------------- playerQuest.cpp
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 晁旭升
* Version      : V1.0
* Date         : 2016/4/13 17:47:59
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "gameLogic/playerQuest/playerQuest.h"
#include "gameLogic/sceneObject/scenePlayer.h"
#include "main/gameServer.h"
#include "protocol/CASMsg.pb.h"
#include "protocol/PlayerStruct.pb.h"
#include "protocol/MsgResult.pb.h"
#include "cfg_data/questData/questData.h"
#include "gameLogic/playerQuest/playerQuestMgr.h"
//#include "gameLogic/playerIdentity/playerIdentity.h"
//#include "gameLogic/playerPrize/playerPrize.h"
#include "log/log.h"
#include "cfg_data/monsterData/monsterData.h"
#include "gameScene/gameSceneCell.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
CPlayerQuest::CPlayerQuest()
{
	m_pModel		= NULL;
	m_pQuestMgr		= NULL;
}

//-------------------------------------------------------------
//------------------------------ 
CPlayerQuest::~CPlayerQuest()
{

}

//-------------------------------------------------------------
//------------------------------ 
void	CPlayerQuest::initialize			(CPlayerQuestMgr* pQuestMgr)
{
	m_pQuestMgr		= pQuestMgr;
	m_pModel		= NULL;
	m_uAcceptTime	= 0;
	m_eState		= QuestState_Ing;
	m_uParam	= 0;
	m_mapCounter.clear();
}
//-------------------------------------------------------------
//------------------------------ 
bool	CPlayerQuest::load	(const PBQuest& pbQuest)
{
	if (  !m_pQuestMgr )
		return false;

	m_pModel = g_clQuestData.findQuestModel(pbQuest.questid());
	if ( !m_pModel )
		return false;

	m_uAcceptTime = pbQuest.accepttime();

	m_uParam = pbQuest.param();
	// 解析任务目标计数
	for ( uint32 j = 0 ; j < (uint32)pbQuest.item_size();++j )
	{
		const PBQuestItem& pbQuestItem = pbQuest.item(j);
		uint32 uTarID = pbQuestItem.tarid();
		uint32 uCurCount = pbQuestItem.curcount();

		if (std::find(m_pModel->vecTargetID.begin(),m_pModel->vecTargetID.end(),uTarID) == m_pModel->vecTargetID.end() )
			continue;
		if (uCurCount < 0 )
			continue;

		m_mapCounter[uTarID] = uCurCount;
	}

	// 注册任务事件
	for(auto it = m_pModel->vecTargetID.begin(); it != m_pModel->vecTargetID.end(); ++it)
	{
		uint32 uTarID = *it;
		const _stQuestTarget* pTarModel = g_clQuestData.findQuestTarget(uTarID);
		if (!pTarModel)
			continue;

		m_pQuestMgr->regQuestEvent(pTarModel->uType, pTarModel->uSubType, this);
	}
	updateState();
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CPlayerQuest::save		(PBQuest& pbQuest)
{
	if ( !m_pModel )
		return false;

	pbQuest.set_questid(m_pModel->uID);
	pbQuest.set_accepttime(m_uAcceptTime);
	if (m_uParam)
		pbQuest.set_param(m_uParam);
	for ( auto _itr = m_mapCounter.begin();_itr != m_mapCounter.end() ; ++_itr )
	{
		PBQuestItem* pbQuestItem = pbQuest.add_item();
		if (!pbQuestItem)
			continue;

		pbQuestItem->set_tarid(_itr->first);
		pbQuestItem->set_curcount(_itr->second);
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------
bool	CPlayerQuest::randomQuestItem(PBQuest& pbQuest)
{
	if ( !m_pModel )
		return false;

	pbQuest.set_questid(m_pModel->uID);
	pbQuest.set_accepttime(m_uAcceptTime);

	if (m_mapCounter.empty()) // 为空时，随机出一个任务目标
	{
		uint32 rangeIndex = randRange((uint32)m_pModel->vecTargetID.size()-1);
		PBQuestItem* pbQuestItem = pbQuest.add_item();
		if (!pbQuestItem)
			return false;

		pbQuestItem->set_tarid(m_pModel->vecTargetID[rangeIndex]);
		pbQuestItem->set_curcount(0);
		m_mapCounter[m_pModel->vecTargetID[rangeIndex]] = 0;
		return true;
	}

	for ( auto _itr = m_mapCounter.begin(); _itr != m_mapCounter.end(); ++_itr )
	{
		PBQuestItem* pbQuestItem = pbQuest.add_item();
		if (!pbQuestItem)
			return false;
		
		pbQuestItem->set_tarid(_itr->first);
		pbQuestItem->set_curcount(_itr->second);
	}
	
	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CPlayerQuest::build(const _stQuestModel* pModel)
{
	if ( !m_pQuestMgr || !pModel )
		return false;

	m_pModel		= pModel;
	m_uAcceptTime	= getTime();
	m_mapCounter.clear();

	auto _pos = m_pModel->mapGiveItem.begin();
	for ( ; _pos != m_pModel->mapGiveItem.end() ; ++_pos )
	{
		m_pQuestMgr->getPlayer().getGameItemMgr().addItem(_pos->first,_pos->second,true,true,true, DoingType_Quest_Complete);
	}

	for (uint32 uIndex = 0; uIndex < pModel->vecTargetID.size(); ++uIndex)
	{
		uint32 uTarID = pModel->vecTargetID[uIndex];
		const _stQuestTarget* pTarModel = g_clQuestData.findQuestTarget(uTarID);
		if (!pTarModel)
			continue;
		m_pQuestMgr->regQuestEvent(pTarModel->uType, pTarModel->uSubType, this);
		refreshTargetCounter(*pTarModel, true);
		if (!m_pModel->bSerial || uIndex == 0 )
		{
			triggerTargetAction(pTarModel->vecStartAction);
		}
	}
	updateState();
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
void	CPlayerQuest::onQuestEvent			(uint8 uType,uint32 uSubType,uint32	uCount)
{
	if ( !canOperate() )
		return ;

	// 查找对应的任务目标
	uint32 uSize = m_pModel->vecTargetID.size();
	const _stQuestTarget* pTarModel = NULL;
	uint32	uIndex = 0;
	for ( ; uIndex < uSize; ++uIndex )
	{
		uint32 uTarID = m_pModel->vecTargetID[uIndex];
		pTarModel = g_clQuestData.findQuestTarget(uTarID);
		if (!pTarModel)
			continue;

		if ( (pTarModel->uType == uType) && ( pTarModel->uSubType == uSubType ) )
			break;
	}

	if ( uIndex >= uSize || !pTarModel )
		return ;
	
	// 师门任务时，m_mapCounter只能存在一个目标任务
	if (m_pModel->uType == QuestType_ClassMates && !m_mapCounter.is_exist(pTarModel->uID))
		return ;

	uint32 uOldCount = m_mapCounter[pTarModel->uID];
	if ( !refreshTargetCounter(*pTarModel,false) )
	{
		// 串行任务是否更新判断
		if(m_pModel->bSerial && !canSerialUpdate(uIndex))
			return ;

		if (pTarModel->uSceneID && (m_pQuestMgr->getPlayer().getSceneID() != pTarModel->uSceneID))
			return ;

		// 策划需求 概率获得计数
		if ( pTarModel->uType == QuestTargetType_MonsterDropGoods )
		{
			if ( pTarModel->uParam1 && (randRange((uint32)10000,(uint32)1) > pTarModel->uParam1)  )
				return ;
		}
		// 增加计数
		uint32& uCurCount = m_mapCounter[pTarModel->uID];
		uCurCount += uCount;
		uCurCount = MIN(pTarModel->uCount,uCurCount);
	}

	uint32 uCurCount = m_mapCounter[pTarModel->uID];
	if (uCurCount == uOldCount )
		return ;

	if ( uCurCount >= pTarModel->uCount && uOldCount < pTarModel->uCount )
	{
		triggerTargetAction(pTarModel->vecCompleteAction);
		if (m_pModel->bSerial && uIndex + 1 < m_pModel->vecTargetID.size() )
		{
			uint32 uNextTarID = m_pModel->vecTargetID[uIndex + 1];
			const _stQuestTarget* pNexrTarModel = g_clQuestData.findQuestTarget(uNextTarID);
			if (pNexrTarModel)
				triggerTargetAction( pNexrTarModel->vecStartAction);
		}
	}
	updateState();

	if (!m_pQuestMgr->getPlayer().haveState(StateType_WaitReady))
	{
		PBG2CQuestUpdateItem ack;
		ack.set_questid(m_pModel->uID);
		ack.set_tarid(pTarModel->uID);
		ack.set_count(uCurCount);
		m_pQuestMgr->getPlayer().sendMsg(P_S2C_Protocol_Quest,S2C_Quest_UpdateItem,&ack);
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CPlayerQuest::setCurCount				(uint32 uTarID,uint32 uCount)
{
	if ( !canOperate() )
		return ;

	uint32 uIndex = 0;
	for ( ; uIndex < m_pModel->vecTargetID.size() ; ++uIndex )
	{
		if ( m_pModel->vecTargetID[uIndex] == uTarID )
			break;
	}

	if ( uIndex >= m_pModel->vecTargetID.size() )
		return ;

	uint32 uOldCount = m_mapCounter[uTarID];
	m_mapCounter[uTarID] = uCount ;
	const _stQuestTarget*  pTargetModel = g_clQuestData.findQuestTarget(uTarID);
	if (pTargetModel && uCount >= pTargetModel->uCount && uOldCount < pTargetModel->uCount )
	{
		triggerTargetAction(pTargetModel->vecCompleteAction);
		if (m_pModel->bSerial && uIndex + 1 < m_pModel->vecTargetID.size() )
		{
			uint32 uNextTarID = m_pModel->vecTargetID[uIndex + 1];
			const _stQuestTarget* pNexrTarModel = g_clQuestData.findQuestTarget(uNextTarID);
			if (pNexrTarModel)
				triggerTargetAction( pNexrTarModel->vecStartAction);
		}
	}

	updateState();

	PBG2CQuestUpdateItem ack;
	ack.set_questid(m_pModel->uID);
	ack.set_tarid(uTarID);
	ack.set_count(uCount);
	m_pQuestMgr->getPlayer().sendMsg(P_S2C_Protocol_Quest,S2C_Quest_UpdateItem,&ack);
}
//-------------------------------------------------------------
//------------------------------ 
void	CPlayerQuest::updateState				()
{
	if ( !canOperate() )
		return ;

	m_eState = QuestState_Ing;
	if (m_pModel->uType != QuestType_ClassMates)
	{
		// 看看是否有未达成的任务目标
		uint32 uSize = m_pModel->vecTargetID.size();
		for ( uint32 uIndex = 0; uIndex < uSize; ++uIndex )
		{
			uint32 uTarID = m_pModel->vecTargetID[uIndex];
			stl_map<uint32,uint32>::iterator _pos = m_mapCounter.find(uTarID);
			if ( _pos == m_mapCounter.end() )
				return ;

			const _stQuestTarget* pTarModel = g_clQuestData.findQuestTarget(uTarID);
			if (!pTarModel)
				return ;

			if (_pos->second < pTarModel->uCount )
				return ;
		}
	}

	// 设置成可交付状态
	m_eState = QuestState_Done;
}
//-------------------------------------------------------------
//------------------------------ 
uint32	CPlayerQuest::getCurCount				(uint32 uTarID)
{
	if ( !canOperate() )
		return 0;

	stl_map<uint32,uint32>::iterator _pos = m_mapCounter.find(uTarID);
	if ( _pos == m_mapCounter.end() )
		return 0;

	return _pos->second;
}

//-------------------------------------------------------------
//------------------------------串行任务时，仅uIndex任务目标未完成，且其前一任务目标已完成，才可更新
bool	CPlayerQuest::canSerialUpdate				(uint32 uIndex)
{
	if( uIndex <= 0 )
		return true;

	if ( uIndex >= (uint32)m_pModel->vecTargetID.size()  )
		return false;

	uint32 uPreTarID = m_pModel->vecTargetID[uIndex - 1];
	stl_map<uint32,uint32>::iterator _pos = m_mapCounter.find(uPreTarID);
	if ( _pos == m_mapCounter.end() )
		return false;

	const _stQuestTarget* pTarModel = g_clQuestData.findQuestTarget(uPreTarID);
	if (!pTarModel)
		return false;

	return _pos->second >= pTarModel->uCount;
}
//-------------------------------------------------------------
//------------------------------
bool	CPlayerQuest::refreshTargetCounter			(const _stQuestTarget& tar,bool bBuild)
{
	CScenePlayer &player = m_pQuestMgr->getPlayer();
	switch ( tar.uType )
	{
	case QuestTargetType_UpgradeLevel1:
	case QuestTargetType_UpgradeLevel:
		{
			m_mapCounter[tar.uID] = player.getLevel();
			return true;
		}
		break;
	case QuestTargetType_Promotion:
		{
			m_mapCounter[tar.uID] = player.getPromotionStep();
			return true;
		}
		break;
	case QuestTargetType_CollectGoods:
		{
			m_mapCounter[tar.uID] = player.getGameItemMgr().getItemCount(tar.uSubType);
			return true;
		}
		break;
	case QuestTargetType_Force:
		{
			m_mapCounter[tar.uID] = player.getPlayerBase().uFightPower;
			return true;
		}
		break;
	case QuestTargetType_Identity:
		{ 
			//CPlayerIdentity* pPlayerIdentity = dynamic_cast<CPlayerIdentity*>(player.getPlayerFun(GameSystemType_Identity));
			//CHECKF(NULL != pPlayerIdentity);
			//m_mapCounter[tar.uID] = pPlayerIdentity->getIdentityNum();	
			//return true;
		}
	default:
		break;
	}
	return false;
}
//-------------------------------------------------------------
//------------------------------
void	CPlayerQuest::onComplete				()
{
	if ( !canOperate() )
		return ;

	stl_map<uint32,uint32>::const_iterator _pos = m_pModel->mapDeductItem.begin();
	stl_map<uint32,uint32>::const_iterator _end = m_pModel->mapDeductItem.end();
	for ( ; _pos != _end ; ++_pos )
	{
		uint32 uRemoveCount = m_pQuestMgr->getPlayer().getGameItemMgr().getItemCount(_pos->first);
		uRemoveCount = MIN(uRemoveCount,_pos->second);
		uint8 uCode11;
		m_pQuestMgr->getPlayer().getGameItemMgr().removeItem(_pos->first,uRemoveCount,true,uCode11,ItemBindType_All,DoingType_Quest_Complete);
	}

	{//奖励
		// 晋升任务奖励另外走邮件
		if (m_pModel->uType == QuestType_Promotion)
			return;

		//CPlayerPrize* pPlayerPrize = m_pQuestMgr->getPlayer().getPlayerPrize();
		//if (pPlayerPrize)
		//	pPlayerPrize->addQuestPrize(m_pModel->uID);
	}
}
//-------------------------------------------------------------
//------------------------------
void	CPlayerQuest::triggerTargetAction		(const stl_vector<_stQuestTargetAction>& vecAction)
{
	if ( !canOperate() )
		return ;

	// 位面任务不触发
	if (m_pModel->uPlaneID)
		return ;

	CGameSceneCell* pCell = m_pQuestMgr->getPlayer().getSceneCell();
	if (!pCell)
		return;

	auto _pos = vecAction.begin();
	auto _end = vecAction.end();
	for ( ; _pos != _end ; ++_pos )
	{
		const _stQuestTargetAction& stAction = *_pos;
		switch (stAction.uType)
		{
		case QuestTargetAction_AddMonster:
			{

				const _stMonsterModel*pModel = g_clMonsterData.find(stAction.stAddMonster.uMonsterID);
				if (!pModel)
					break;

				_stPoint2F stCenter(stAction.stAddMonster.fPosX,stAction.stAddMonster.fPosY);
				for ( uint32 i = 0 ; i < stAction.stAddMonster.uCount ;++i )
				{
					float fDir = getRandDir();
					float fDistance = (float) (randRange(int32(stAction.stAddMonster.fRadius*100), int32(0)) / 100.0f) ;
					_stPoint2F stTestPos;
					::getTargetPoint(stCenter, fDir, fDistance, stTestPos);
					_stPoint2F stReadPos = stCenter;
					pCell->getTargetPoint(stCenter,stTestPos,stReadPos);
					pCell->createMonster(pModel,stReadPos.x,stReadPos.y,stAction.stAddMonster.fDir <= 0.0f ? getRandDir() :stAction.stAddMonster.fDir, 0, true);
				}
			}
			break;
		case QuestTargetAction_RemoveMonster:
			{
				auto _pos = pCell->object_begin();
				auto _end = pCell->object_end();
				for ( ; _pos != _end ; ++_pos )
				{
					CSceneUnit*	pUnit = _pos->second;
					if (!pUnit)
						continue;

					CSceneMonster* pMonster = pUnit->asMonster();
					if (!pMonster)
						continue;

					if (stAction.stRemoveMonster.uMonsterID != pMonster->getMonsterID())
						continue;

					pCell->leaveScene(pMonster);
				}
			}
			break;
		case QuestTargetAction_ChangeCamp:
			{
				auto _pos = pCell->object_begin();
				auto _end = pCell->object_end();
				for ( ; _pos != _end ; ++_pos )
				{
					CSceneUnit*	pUnit = _pos->second;
					if (!pUnit)
						continue;

					CSceneMonster* pMonster = pUnit->asMonster();
					if (!pMonster)
						continue;

					if (stAction.stChangeCamp.uMonsterID != pMonster->getMonsterID())
						continue;

					pMonster->setCampID(stAction.stChangeCamp.uCampID);
				}
			}
			break;
		case QuestTargetAction_ChangeScene:
			{
				m_pQuestMgr->getPlayer().playerFlyScene_ByCellID(stAction.stChangeScene.uSceneID,0,stAction.stChangeScene.uRegionID);
			}
			break;
		case QuestTargetAction_AddSceneArea:
			{
				pCell->addArea(stAction.stAddSceneArea.uAreaID);
			}
			break;
		case QuestTargetAction_DelSceneArea:
			{
				pCell->delArea(stAction.stDelSceneArea.uAreaID);
			}
			break;
		default:
			break;
		}
	}
}