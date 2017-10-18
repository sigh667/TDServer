/*------------- playerRelive.cpp
 * Copyright (C): 2016  Mokylin·Mokyqi
 * Author       : 方声辉
 * Version      : V1.01
 * Date         : 2017/05/31
 * 
 */
/***************************************************************
* 
***************************************************************/
#include "gameLogic/playerRelive/playerRelive.h"
#include "gameLogic/sceneObject/scenePlayer.h"
//#include "gameLogic/playerQuest/playerQuestMgr.h"
#include "gameLogic/sceneObject/sceneMonster.h"
#include "gameScene/gameSceneCell.h"
#include "cfg_data/sceneMapData/sceneMapData.h"
#include "cfg_data/constantData/constantData.h"
#include "cfg_data/reliveData/reliveData.h"
#include "gameScene/copymapBase.h"

#include "protocol/CASMsg.pb.h"
#include "protocol/MsgResult.pb.h"
#include "protocol/CASProtocol.pb.h"
#include "protocol/PlayerStruct.pb.h"
#include "log/log.h"
//-------------------------------------------------------------
//------------------------------
CPlayerRelive::CPlayerRelive(CScenePlayer& clPlayer) : CGamePlayerFun(clPlayer)
{
	initialize();
	registerHandler(C2S_Relive_Born_Ack,	(CGamePlayerFun::_handleSubMsg)&CPlayerRelive::handle_Relive);
}
//-------------------------------------------------------------
//------------------------------
CPlayerRelive::~CPlayerRelive()
{

}

//-------------------------------------------------------------
//------------------------------
void CPlayerRelive::initialize()
{
	m_FreeCount = 0;
	m_CostCount    = 0;
}

//-------------------------------------------------------------
//------------------------------
void CPlayerRelive::newOnline()
{
	initialize();
}

//-------------------------------------------------------------
//------------------------------

void CPlayerRelive::cleanupDaily()
{
	initialize();
}

//-------------------------------------------------------------
//------------------------------ 
bool CPlayerRelive::loadFromProtobuf(const PBPlayerData& pbPlayer)
{
	m_FreeCount	= pbPlayer.playersystem().relive().freemaxcount();
	m_CostCount		= pbPlayer.playersystem().relive().costcount();
	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool CPlayerRelive::saveToProtobuf(PBPlayerData& pbPlayer)
{
	PBPlayerSystem* pbSystem = pbPlayer.mutable_playersystem();
	if ( pbSystem )
	{
		PBLlayerRelive* pbRelive = pbSystem->mutable_relive();
		if (pbRelive)
		{
			pbRelive->set_freemaxcount(m_FreeCount);
			pbRelive->set_costcount(m_CostCount);
		}	
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------
bool	CPlayerRelive::handle_Relive(uint8 uEventID,CNetStream& clStream)
{
	uint8 uCode = R_ResultRelive_Failed;
	PBC2GRelive_Born ask;
	CHECKF(clStream.ParseFromStream(ask));

	if (!relive(ask.type(),uCode))
	{
		m_clPlayer.sendMsg(P_S2C_Protocol_Relive,S2C_Relive_Born_Ack,NULL,uCode);
	}
	
	return true;
}

//-------------------------------------------------------------
//------------------------------
bool CPlayerRelive::send_Relive_Ack()
{
	PBG2Relive_SceneBorn ack;
	ack.set_relivecount(m_CostCount);
	ack.set_freecount(m_FreeCount);
	m_clPlayer.sendMsg(P_S2C_Protocol_Relive,S2C_Relive_Born_Ack,&ack);
	return true;
}

//-------------------------------------------------------------
//------------------------------
bool	CPlayerRelive::relive(uint32 uType , uint8 & uCode)
{
	uCode = R_ResultRelive_Failed; 	
	switch (uType)
	{
	case ReliveType_Origin:
		{
			if (!origin_relive(uCode))
				return false;
		}
		break;

	case ReliveType_RelivePoint:
		{
			if (!safepoint_relive(uCode))
				return false;
		}
		break;

	default:
		return false;
	}

	uCode = R_ResultCommon_Succeed;
	return true;
}

//-------------------------------------------------------------
//------------------------------
uint16	CPlayerRelive::getRelivePoint	()
{
	CGameSceneCell *pSceneCell = m_clPlayer.getSceneCell();
	if ( !pSceneCell )
		return 0;

	const _stSceneMapModel* pSceneModel = pSceneCell->getSceneModel();
	if ( !pSceneModel)
		return 0;

	const uint16* pRegionID = pSceneModel->mapRelivePoint.find_(m_clPlayer.getCampID());
	if (!pRegionID)
	{
		pRegionID = pSceneModel->mapRelivePoint.find_(0);
	}

	return pRegionID ? *pRegionID : 0;
}

//-------------------------------------------------------------
//------------------------------
bool CPlayerRelive::origin_relive(uint8 &uCode)
{
	CGameSceneCell *pSceneCell = m_clPlayer.getSceneCell();

	if ( !pSceneCell )
		return false;

	if ( !m_clPlayer.isDead() )
		return false;
	
	if (m_clPlayer.isInCopymap() )
	{
		CCopymapBase * pCopyMapBase = m_clPlayer.getGameCopymap();
		if (!pCopyMapBase)
			return false;
		if (!pCopyMapBase->canRelive())
		{
			uCode = R_ResultRelive_CopyMap;
			return false;
		}
	}
	const _stSceneMapModel* pSceneModel = pSceneCell->getSceneModel();
	if ( !pSceneModel)
		return false;
	// 区域是否可以原地复活
	if (!pSceneModel->testSceneMark(SceneMapMarkType_canReliveOrigin))
	{
		uCode = R_ResultRelive_CopyMap;
		return false;
	}

	if (m_FreeCount < (uint32)g_clConstantData.Relive_Max_Free_Count)// 免费复活
	{
		m_FreeCount++;
	}
	else if (pSceneModel->nReliveLocalLimit != 1)// 消耗道理复活 
	{

		if(m_CostCount >= (uint32)pSceneModel->nReliveLocalLimit)
		{
			uCode = R_ResultRelive_CountLimit;
			return false;
		}
		uint32 uUsedCount	= m_CostCount + 1;	
		uint32 uItemID		= (uint32)g_clConstantData.Relive_Item_ID;
		uint32 uNeedItemNum = g_clReliveData.findReliveModel(uUsedCount);

		if (g_clConstantData.Relive_Item_ID &&
			uNeedItemNum	&&
			m_clPlayer.getGameItemMgr().canRemoveItem(uItemID, uNeedItemNum, ItemBindType_All)
			)
		{
			uint8 uCode1 = R_ResultRelive_Failed;
			m_clPlayer.getGameItemMgr().removeItem(uItemID,uNeedItemNum,true,uCode1,ItemBindType_All, DoingType_Relive);
		}
		else 
		{
			uCode = R_ResultRelive_NeedItemNum;
			return false;
		}
		m_CostCount = uUsedCount;
	}

	m_clPlayer.setHP(m_clPlayer.getHPMax());
	m_clPlayer.callbackLife();	
	send_Relive_Ack();
	uCode = R_ResultCommon_Succeed;
	return true;
}

//-------------------------------------------------------------
//------------------------------
bool CPlayerRelive::safepoint_relive(uint8 & uCode)
{
	if ( !m_clPlayer.isDead() )
		return false;			
	
	if (m_clPlayer.isInCopymap() )
	{
		CCopymapBase * pCopyMapBase = m_clPlayer.getGameCopymap();
		if (!pCopyMapBase)
			return false;
		if (!pCopyMapBase->canRelive())
		{
			uCode = R_ResultRelive_CopyMap;
			return false;
		}	
	}

	uint16 uRegionID = getRelivePoint();
	m_clPlayer.setHP(m_clPlayer.getHPMax());
	m_clPlayer.callbackLife();	
	m_clPlayer.randPos(uRegionID);

	//加buff
	m_clPlayer.getGameBuffMgr().addBuff(g_clConstantData.Relive_Buff_ID, 1,m_clPlayer.getUnitID(), 0, DoingType_Relive);

	send_Relive_Ack();
	uCode = R_ResultCommon_Succeed;
	return true;
}