/*----------------- copymapNormal.cpp
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/5/16 18:22:04
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "gameScene/gameSceneCell.h"
#include "gameScene/copymapNormal.h"
#include "cfg_data/copymapData/copymapData.h"
#include "cfg_data/questData/questData.h"
#include "log/log.h"
#include "cfg_data/monsterData/monsterModel.h"
#include "cfg_data/monsterData/monsterData.h"
/*************************************************************/

void	CCopymapNormal::initialize()
{
	Parent::initialize();
	m_mapQuestCounter.clear();
	m_pQuestModel = NULL;
}
//-------------------------------------------------------------
//------------------------------ 
void	CCopymapNormal::restart				(CGameSceneCell*pScene,const PBCreateCellParam&pbParam)
{
	Parent::restart(pScene,pbParam);
	m_pQuestModel = NULL;
	m_mapQuestCounter.clear();
	if ( m_pCopymapModel )
	{
		m_pQuestModel = g_clQuestData.findQuestModel(m_pCopymapModel->uTaskID);
		CHECK( NULL != m_pQuestModel && m_pQuestModel->uType == QuestType_Copymap );

		for (uint32 uIndex = 0; uIndex < m_pQuestModel->vecTargetID.size(); ++uIndex)
		{
			const _stQuestTarget* pTarModel = g_clQuestData.findQuestTarget(m_pQuestModel->vecTargetID[uIndex]);
			if (!pTarModel)
				continue;

			if (!m_pQuestModel->bSerial || uIndex == 0 )
			{
				triggerTargetAction(pTarModel->vecStartAction);
			}
		}
	}
}
//-------------------------------------------------------------
//------------------------------ 角色上线处理
void	CCopymapNormal::playerOnline	(CScenePlayer*pPlayer)
{
	Parent::playerOnline(pPlayer);

	if ( !pPlayer || !m_pQuestModel )
		return ;



	{//  任务计数
		PBG2CCopymapQuest msg;
		msg.set_questid(m_pQuestModel->uID);
		stl_map<uint32,uint32>::iterator _pos = m_mapQuestCounter.begin();
		stl_map<uint32,uint32>::iterator _end = m_mapQuestCounter.end();
		for ( ; _pos != _end; ++_pos )
		{
			PBG2CCopymapQuestTarget* pbData = msg.add_tar();
			if ( pbData )
			{
				pbData->set_tarid(_pos->first);
				pbData->set_count(_pos->second);
			}
		}

		pPlayer->sendMsg(P_S2C_Protocol_Copymap,S2C_Copymap_Quest,&msg);
	}
}
//-------------------------------------------------------------
//------------------------------ 
bool	CCopymapNormal::trigerQuestEvent	(uint8 uType,uint32 uSubType,uint32	uCount )
{
	if ( !m_pQuestModel )
		return false;

	// 查找对应任务目标的索引
	uint32 uSize = m_pQuestModel->vecTargetID.size();
	uint32	uIndex = 0;
	const _stQuestTarget* pTarModel = NULL;
	for ( ; uIndex < uSize; ++uIndex )
	{
		pTarModel = g_clQuestData.findQuestTarget(m_pQuestModel->vecTargetID[uIndex]);
		if (!pTarModel)
			continue;

		if ( (pTarModel->uType == uType) && ( pTarModel->uSubType == uSubType ) )
			break;
	}

	if ( uIndex >= uSize || !pTarModel )
		return false;

	uint32& uCurCount = m_mapQuestCounter[pTarModel->uID];
	if ( uCurCount >= pTarModel->uCount )
		return true;

	// 增加计数
	uCurCount	+= uCount;
	uCurCount	= MIN(uCurCount,pTarModel->uCount);

	// 广播计数
	PBG2CCopymapQuestTarget msg;
	msg.set_tarid(pTarModel->uID);
	msg.set_count(uCurCount);
	broadCastMsg(P_S2C_Protocol_Copymap,S2C_Copymap_QuestItem,&msg);

	if ( uCurCount < pTarModel->uCount )
		return true;

	triggerTargetAction(pTarModel->vecCompleteAction);
	if (m_pQuestModel->bSerial && uIndex + 1 < m_pQuestModel->vecTargetID.size() )
	{
		uint32 uNextTarID = m_pQuestModel->vecTargetID[uIndex + 1];
		const _stQuestTarget* pNextTarModel = g_clQuestData.findQuestTarget(uNextTarID);
		if (pNextTarModel)
			triggerTargetAction(pNextTarModel->vecStartAction);
	}

	// 看看是否有未达成的任务目标
	for ( uint32 i = 0; i < uSize; ++i )
	{
		uint32 uTarID = m_pQuestModel->vecTargetID[i];
		stl_map<uint32,uint32>::iterator _pos = m_mapQuestCounter.find(uTarID);
		if ( _pos == m_mapQuestCounter.end() )
			return true;

		const _stQuestTarget* pTarModel = g_clQuestData.findQuestTarget(uTarID);
		if (!pTarModel)
			return true;

		if (_pos->second < pTarModel->uCount )
			return true;
	}
	onQuestFinished();
	return true;
}

//-------------------------------------------------------------
//------------------------------ 
void	CCopymapNormal::onQuestFinished		()
{
	on_copymap_complete(getTime(),CopymapCompleteReson_Success);
}

//-------------------------------------------------------------
//------------------------------ 
void	CCopymapNormal::callback_killmonster(CSceneMonster* pMonster,CScenePlayer* pPlayer)
{
	if ( !pMonster )
		return ;
	
	Parent::callback_killmonster(pMonster,pPlayer);

	const _stMonsterModel* pMonsterModel = pMonster->getMonsterModel();
	if ( !pMonsterModel )
		return ;

	trigerQuestEvent(QuestTargetType_KillMonster,pMonsterModel->uMonsterID);
}

//-------------------------------------------------------------
//------------------------------ 采集完成回调
void	CCopymapNormal::callback_depotGoods(CSceneMonster* pMonster,CScenePlayer* pPlayer)
{
	if ( !pMonster )
		return ;

	const _stMonsterModel* pMonsterModel = pMonster->getMonsterModel();
	if ( !pMonsterModel )
		return ;

	trigerQuestEvent(QuestTargetType_DepotGoods,pMonsterModel->uMonsterID);
}
//-------------------------------------------------------------
//------------------------------ 检测副本完成
void	CCopymapNormal::check_complete	(uint32 uTime)
{
	if(CopymapState_Ing != m_eCopymapState || !m_pSceneCell)
		return;

	//副本关闭时间
	if(m_uCloseTime < uTime)
	{
		on_copymap_complete(uTime,CopymapCompleteReson_TimeOut);
		return ;
	}
}
//-------------------------------------------------------------
//------------------------------
void	CCopymapNormal::changeCopymapTask	(uint32 uQuestID)
{
	const _stQuestModel* pQuestModel = g_clQuestData.findQuestModel(uQuestID);
	if (!pQuestModel)
		return ;

	CHECK( NULL != pQuestModel && pQuestModel->uType == QuestType_Copymap );
	m_pQuestModel = pQuestModel;

	m_mapQuestCounter.clear();

	// 广播计数
	PBG2CCopymapChangeQuest msg;
	msg.set_questid(uQuestID);
	broadCastMsg(P_S2C_Protocol_Copymap,S2C_Copymap_ChangeQuest,&msg);
}
//-------------------------------------------------------------
//------------------------------
void	CCopymapNormal::triggerTargetAction		(const stl_vector<_stQuestTargetAction>& vecAction)
{
	if (!m_pSceneCell)
		return ;

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
					m_pSceneCell->getTargetPoint(stCenter,stTestPos,stReadPos);
					m_pSceneCell->createMonster(pModel,stReadPos.x,stReadPos.y, stAction.stAddMonster.fDir <= 0.0f ? getRandDir() :stAction.stAddMonster.fDir , 0, true);
				}
			}
			break;
		case QuestTargetAction_RemoveMonster:
			{
				auto _pos = m_pSceneCell->object_begin();
				auto _end = m_pSceneCell->object_end();
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

					m_pSceneCell->leaveScene(pMonster);
				}
			}
			break;
		case QuestTargetAction_ChangeCamp:
			{
				auto _pos = m_pSceneCell->object_begin();
				auto _end = m_pSceneCell->object_end();
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
		case QuestTargetAction_AddSceneArea:
			{
				m_pSceneCell->addArea(stAction.stAddSceneArea.uAreaID);
			}
			break;
		case QuestTargetAction_DelSceneArea:
			{
				m_pSceneCell->delArea(stAction.stDelSceneArea.uAreaID);
			}
			break;
		default:
			break;
		}
	}
}
