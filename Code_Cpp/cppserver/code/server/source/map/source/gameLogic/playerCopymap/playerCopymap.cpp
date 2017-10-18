/*----------------- playerCopymap.cpp
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/5/10 14:59:49
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "gameLogic/playerCopymap/playerCopymap.h"
#include "protocol/CASProtocol.pb.h"
#include "protocol/CASMsg.pb.h"
#include "protocol/MsgResult.pb.h"
#include "protocol/WAGProtocol.pb.h"
#include "protocol/WAGMsg.pb.h"
#include "basic/basicTypes.h"
#include "log/log.h"
#include "packet/worldPacket.h"
#include "main/gameServer.h"
#include "cfg_data/copymapData/copymapData.h"
#include "cfg_data/monsterData/monsterData.h"
#include "gameScene/gameSceneCell.h"
//#include "gameLogic/playerQuest/playerQuestMgr.h"
//#include "gameLogic/playerPrize/playerPrize.h"
//#include "gameScene/copymapDartRob.h"
//#include "gameLogic/playerFactionBoss/playerFactionBoss.h"
//#include "gameLogic/playerTitle/playerTitle.h"
//#include "gameLogic/playerDart/playerDart.h"
//#include "gameLogic/playerTeam/playerTeam.h"
#include "cfg_data/constantData/constantData.h"
#include "cfg_data/teamtargetData/teamtargetData.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
void	_stPlayerCopymap::loadFromProtobuf		(const PBCopymapInfo&pbInfo)
{
	initialize();

	uCopymapID		= pbInfo.copymapid();
	uEnterCount		= pbInfo.entercount();
}
//-------------------------------------------------------------
//------------------------------ 
void	_stPlayerCopymap::saveToProtobuf		(PBCopymapInfo&pbInfo)
{
	pbInfo.Clear();

	pbInfo.set_copymapid(uCopymapID);
	pbInfo.set_entercount(uEnterCount);
}
//-------------------------------------------------------------
//------------------------------
CPlayerCopymap::CPlayerCopymap(CScenePlayer& clPlayer) : CGamePlayerFun(clPlayer)
{
	registerHandler(C2S_Copymap_Create,			(CGamePlayerFun::_handleSubMsg)&CPlayerCopymap::handle_Copymap_Create);
	registerHandler(C2S_Copymap_Exit,			(CGamePlayerFun::_handleSubMsg)&CPlayerCopymap::handle_Copymap_Exit);
	registerHandler(C2S_Copymap_Invite,			(CGamePlayerFun::_handleSubMsg)&CPlayerCopymap::handle_Copymap_Invite);
	registerHandler(C2S_Copymap_AcceptInvite,	(CGamePlayerFun::_handleSubMsg)&CPlayerCopymap::handle_Copymap_AcceptInvite);
	registerHandler(C2S_Copymap_TeamMemberInfo,	(CGamePlayerFun::_handleSubMsg)&CPlayerCopymap::handle_Copymap_TeamMemberInfo);
	registerHandler(C2S_Copymap_EnterNpc,		(CGamePlayerFun::_handleSubMsg)&CPlayerCopymap::handle_Copymap_EnterNpc);
	registerHandler(C2S_Copymap_FindAttackMonster,		(CGamePlayerFun::_handleSubMsg)&CPlayerCopymap::handle_Copymap_FindAttackMonster);
	registerHandler(C2S_Copymap_UnAcitve,		(CGamePlayerFun::_handleSubMsg)&CPlayerCopymap::handle_Copymap_UnAcitve);
}

//-------------------------------------------------------------
//------------------------------
CPlayerCopymap::~CPlayerCopymap()
{

}
//-------------------------------------------------------------
//------------------------------
void	CPlayerCopymap::onLogin					()
{
	synWorldServerAll();
}
//-------------------------------------------------------------
//------------------------------
void	CPlayerCopymap::synWorldServerAll				()
{
	if (!m_clPlayer.isWorldConneted())
		return ;

	//转发到世界服
	PBG2WCopymapAllCopymapCount msg;
	msg.set_playerid(m_clPlayer.getPlayerID());
	PBPlayerCopymap* pbData = msg.mutable_info();
	if (pbData )
	{
		save(*pbData);
	}
	m_clPlayer.sendMsgToWorld(P_G2W_Protocol_Copymap, G2W_Copymap_AllCopymapCount, &msg);
}
//-------------------------------------------------------------
//------------------------------
void	CPlayerCopymap::synWorldServerOne				(_stPlayerCopymap& stData)
{
	if (!m_clPlayer.isWorldConneted())
		return ;

	//转发到世界服
	PBG2WCopymapOneCopymapCount msg;
	msg.set_playerid(m_clPlayer.getPlayerID());
	PBCopymapInfo* pbData = msg.mutable_info();
	if (pbData )
	{
		stData.saveToProtobuf(*pbData);
	}
	m_clPlayer.sendMsgToWorld(P_G2W_Protocol_Copymap, G2W_Copymap_OneCopymapCount, &msg);
}
//-------------------------------------------------------------
//------------------------------
void CPlayerCopymap::initialize()
{
	CGamePlayerFun::initialize();
	m_uAssistCount = 0;
	m_mapCopymapInfo.clear();


}
//-------------------------------------------------------------
//------------------------------
void CPlayerCopymap::newOnline()
{
	initialize();
}
//-------------------------------------------------------------
//------------------------------
void CPlayerCopymap::onClientGo	()
{
	if (m_clPlayer.getSceneCell() && m_clPlayer.getSceneCell()->isCopymap())
	{
		m_clPlayer.getSceneCell()->onClientGo(&m_clPlayer);
	}
}
//-------------------------------------------------------------
//------------------------------ 
bool	CPlayerCopymap::loadFromProtobuf	(const PBPlayerData&pbPlayer)
{
	const PBPlayerCopymap& pSystem = pbPlayer.playersystem().copymap();
	m_uAssistCount	= pSystem.assistcount();
	for (int32 i = 0; i < pSystem.info_size(); ++i)
	{
		const PBCopymapInfo& pbInfo = pSystem.info(i);
		_stPlayerCopymap* pInfo = m_mapCopymapInfo.allocate((uint16)pbInfo.copymapid());
			if (!pInfo)
				continue;
		pInfo->loadFromProtobuf(pbInfo);
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CPlayerCopymap::save							(PBPlayerCopymap& pbData)
{
	pbData.clear_info();
	pbData.set_assistcount(m_uAssistCount);
	MAP_CopymapInfo::iterator _pos = m_mapCopymapInfo.begin();
	MAP_CopymapInfo::iterator _end = m_mapCopymapInfo.end();
	for ( ; _pos != _end ; ++_pos )
	{
		_stPlayerCopymap* pInfo = _pos->second;
		if ( !pInfo )
			continue;

		PBCopymapInfo* pbInfo = pbData.add_info();
		if ( !pbInfo )
			continue;

		pInfo->saveToProtobuf(*pbInfo);
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------ --- 每日清理
void	CPlayerCopymap::cleanupDaily			()
{
	synWorldServerAll();
	MAP_CopymapInfo::iterator _pos = m_mapCopymapInfo.begin();
	MAP_CopymapInfo::iterator _end = m_mapCopymapInfo.end();
	for ( ; _pos != _end ; ++_pos )
	{
		_stPlayerCopymap* pInfo = _pos->second;
		if ( !pInfo )
			continue;

		const _stCopymapModel* pCopymapModel = g_clCopymapData.find_model(pInfo->uCopymapID);
		if(!pCopymapModel || pCopymapModel->uResetType != CopymapResetType_Day)
			continue;
		pInfo->uEnterCount = 0;
	}
	m_uAssistCount = 0;
}
//-------------------------------------------------------------
//------------------------------ --- 每周清理
void	CPlayerCopymap::cleanupWeekly			()
{
	MAP_CopymapInfo::iterator _pos = m_mapCopymapInfo.begin();
	MAP_CopymapInfo::iterator _end = m_mapCopymapInfo.end();
	for ( ; _pos != _end ; ++_pos )
	{
		_stPlayerCopymap* pInfo = _pos->second;
		if ( !pInfo )
			continue;

		const _stCopymapModel* pCopymapModel = g_clCopymapData.find_model(pInfo->uCopymapID);
		if(!pCopymapModel || pCopymapModel->uResetType != CopymapResetType_Week)
			continue;
		pInfo->uEnterCount = 0;
	}
}
//-------------------------------------------------------------
//------------------------------ 
bool	CPlayerCopymap::saveToProtobuf		(PBPlayerData&pbPlayer)
{
	PBPlayerCopymap* pSystem = pbPlayer.mutable_playersystem()->mutable_copymap();
	if (!pSystem)
		return false;

	return save(*pSystem);
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerCopymap::handle_Copymap_Create(uint8 uEventID, CNetStream& clStream)
{
	PBC2GCopymapCreate ask;
	CHECKF(clStream.ParseFromStream(ask));
	uint8 uCode = R_ResultCopymap_Fail;
	if (!copymap_create(ask.copymapid(),ask.param(),uCode))
	{
		m_clPlayer.sendMsg(P_S2C_Protocol_Copymap,S2C_Copymap_Create_Ack,&ask,uCode);
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerCopymap::handle_Copymap_Exit(uint8 uEventID, CNetStream& clStream)
{
	PBC2GCopymapExit ask;
	CHECKF(clStream.ParseFromStream(ask));
	uint8 uCode = R_ResultCopymap_Fail;
	if (!copymap_exit(ask.copymapid(),uCode))
	{
		m_clPlayer.sendMsg(P_S2C_Protocol_Copymap,S2C_Copymap_Exit_Ack,&ask,uCode);
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerCopymap::handle_Copymap_Invite(uint8 uEventID, CNetStream& clStream)
{
	PBC2GCopymapInvite ask;
	CHECKF(clStream.ParseFromStream(ask));
	uint8 uCode = R_ResultCopymap_Fail;
	if (!copymap_invite(ask.copymapid(),ask.targetid(),uCode))
	{
		m_clPlayer.sendMsg(P_S2C_Protocol_Copymap,S2C_Copymap_Invite_Ack,&ask,uCode);
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerCopymap::handle_Copymap_AcceptInvite(uint8 uEventID, CNetStream& clStream)
{
	PBC2GCopymapAcceptInvite ask;
	CHECKF(clStream.ParseFromStream(ask));
	uint8 uCode = R_ResultCopymap_Fail;
	if (!copymap_acceptinvite(ask.copymapid(),ask.targetid(),ask.accept(),uCode))
	{
		m_clPlayer.sendMsg(P_S2C_Protocol_Copymap,S2C_Copymap_AcceptInvite_Ack,&ask,uCode);
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerCopymap::handle_Copymap_TeamMemberInfo(uint8 uEventID, CNetStream& clStream)
{
	PBC2GCopymapTeamMemberInfo ask;
	CHECKF(clStream.ParseFromStream(ask));
	uint8 uCode = R_ResultCopymap_Fail;
	if (!copymap_teammemberinfo(ask.copymapid(),uCode))
	{
		m_clPlayer.sendMsg(P_S2C_Protocol_Copymap,S2C_Copymap_TeamMemberInfo_Ack,&ask,uCode);
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerCopymap::handle_Copymap_EnterNpc(uint8 uEventID, CNetStream& clStream)
{
	PBC2GCopymapEnterNpc ask;
	CHECKF(clStream.ParseFromStream(ask));
	uint8 uCode = R_ResultCopymap_Fail;
	if (!copymap_enternpc(ask.copymapid(),ask.npcunitid(),uCode))
	{
		m_clPlayer.sendMsg(P_S2C_Protocol_Copymap,S2C_Copymap_Create_Ack,NULL,uCode);
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerCopymap::handle_Copymap_FindAttackMonster(uint8 uEventID, CNetStream& clStream)
{
	uint8 uCode = R_ResultCopymap_Fail;
	if (!findAttackMonster())
	{
		m_clPlayer.sendMsg(P_S2C_Protocol_Copymap,S2C_Copymap_FindAttackMonster,NULL,uCode);
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerCopymap::handle_Copymap_UnAcitve(uint8 uEventID, CNetStream& clStream)
{
	uint8 uCode = R_ResultCopymap_Fail;
	PBC2GCopymapUnactive ask;
	CHECKF(clStream.ParseFromStream(ask));

	CSceneUnit* pUnit = m_clPlayer.findCellObject(ask.unitid());
	if (!pUnit)
		return false;

	CGameSceneCell* pGameCell = m_clPlayer.getSceneCell();
	if (pGameCell->getGameCopymap())
	{
		//pGameCell->getGameCopymap()->unactivemonster(pUnit->asMonster(),&m_clPlayer,uCode);
		//m_clPlayer.sendMsg(P_S2C_Protocol_Copymap,S2C_Copymap_Unactive,NULL,uCode);
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool  CPlayerCopymap::findAttackMonster				()
{
	CGameSceneCell* pSceneCell = m_clPlayer.getSceneCell();
	if (!pSceneCell || !pSceneCell->getCopymapModel())
		return true;

	//怪物死完
	stl_vector<CSceneMonster*> vecMonster;
	stl_vector<CSceneMonster*> vecRealMonster;
	pSceneCell->getAllAliveMonster(vecMonster);
	for (uint32 i = 0; i < vecMonster.size(); ++i)
	{
		CSceneMonster* pMonster = vecMonster[i];
		if (!pMonster || m_clPlayer.getRelation(pMonster) != TargetCategory_Enemy)
			continue;

		vecRealMonster.push_back(pMonster);
	}

	//排序
	if (vecRealMonster.size() > 0)
		std::sort(vecRealMonster.begin(),vecRealMonster.end(),sort_time );

	CSceneMonster* pMonster = vecRealMonster[0];
	if (!pMonster)
		return false;

	PBG2CCopymapFindAttackMonster ack;
	ack.mutable_point()->set_posx(pMonster->getPosition().x);
	ack.mutable_point()->set_posy(pMonster->getPosition().y);
	m_clPlayer.sendMsg(P_S2C_Protocol_Copymap,S2C_Copymap_FindAttackMonster,&ack);
	return true;
}
//-------------------------------------------------------------
//------------------------------
bool  CPlayerCopymap::sort_time (CSceneMonster* left, CSceneMonster* right)
{
	if ( !left )
		return false;

	if ( !right )
		return true;

	return left->getBuildTick() < left->getBuildTick();
}

//-------------------------------------------------------------
//------------------------------ 
bool CPlayerCopymap::canCreate(const _stCopymapModel* pCopymapModel,uint32& uCash,uint8& uCode)
{
	uCode = R_ResultCopymap_Fail;
	if (!pCopymapModel)
		return false;

	//是否在时间内
	if (!pCopymapModel->isInOpenTime((uint64)getTime()))
	{
		uCode = R_ResultCopymap_Create_NoTime;
		return false;
	}

	if ( m_clPlayer.getCopymapState() != PlayerCopymapState_None )
		return false;

	CGameSceneCell* pCell = m_clPlayer.getSceneCell();
	if (!pCell)
		return false;

	const _stCopymapModel* pCurCopymapModel = pCell->getCopymapModel();
	if (  pCurCopymapModel )
	{

		if ( (pCopymapModel->uType == CopymapType_GwTrain && pCurCopymapModel->uType == CopymapType_GwTrain)
			|| (pCopymapModel->uType == CopymapType_ClimbTower && pCurCopymapModel->uType == CopymapType_ClimbTower)
			)
		{
			//这两个可以跳转 
		}
		else
		{
			uCode = R_ResultCopymap_Create_InCopymap;
			return false;
		}
	}
	//押镖中
	//if (m_clPlayer.getPlayerDart()->getDartGlobalID() != 0
	//	&& pCopymapModel->uType != CopymapType_DartRob)
	//{
	//	uCode = R_ResultCopymap_In_Dartcar;
	//	return false;
	//}

	if (!m_clPlayer.canPreCondition(pCopymapModel->dmapPreCondition,1))
	{
		uCode = R_ResultCopymap_Create_PreCondition;
		return false;
	}

	_stPlayerCopymap* pInfo = m_mapCopymapInfo.find(pCopymapModel->uCopymapID);
	if ( pCopymapModel->uEnterCount && pInfo && pInfo->uEnterCount >= pCopymapModel->uEnterCount )
	{
		uCode = R_ResultCopymap_Create_Count;
		return false;
	}

	//if (pCopymapModel->uType == CopymapType_FactionBoss )
	//{
	//	CPlayerFactionBoss* pPlayerFactionBoss = (CPlayerFactionBoss*)m_clPlayer.getPlayerFun(GameSystemType_FactionBoss);
	//	if (!pPlayerFactionBoss)
	//		return false;

	//	if (pPlayerFactionBoss->isEnterCooling())
	//	{
	//		uCode = R_ResultCopymap_FactionBoss_Cooling;
	//		return false;
	//	}

	//	if (!m_clPlayer.getFactionID())
	//	{
	//		uCode = R_ResultCopymap_FactionBoss_NeedFaction;
	//		return false;
	//	}
	//}

	//发送给世界服
	if (!m_clPlayer.isWorldConneted())
		return false;

	if (CopymapEnterType_Team == pCopymapModel->uEnterType)
	{
		if (!m_clPlayer.getTeam())
		{
			uCode = R_ResultCopymap_Create_NeedTeam;
			return false;
		}

		if (pCopymapModel->check_mark(CopymapMark_LeaderOpen))
		{
			if (m_clPlayer.getTeam()->uLeaderID != m_clPlayer.getPlayerID())
			{
				uCode = R_ResultCopymap_Create_NeedLeader;
				return false;
			}
		}
	}

	{// 消耗
		// 赏金团
		_stTeamInfo* pTeam = m_clPlayer.getTeam();
		if (pTeam && pTeam->uLeaderID == m_clPlayer.getPlayerID() && pTeam->m_stTeamTarget.uCash  )
		{
			const _stTeamTargetModel * pTeamTargetModel = g_clTeamTargetData.findTeamTargetData(pTeam->m_stTeamTarget.uTargetid);
			if (!pTeamTargetModel || !(pTeamTargetModel->uType == TeamTargetMainType_Copymap || pTeamTargetModel->uType == TeamTargetMainType_Copymap_Difficulty ) || pTeamTargetModel->uParam !=  pCopymapModel->uCopymapID )
			{
				uCode = R_ResultCopymap_Create_CashCopymapID;
				return false;
			}

			if ( m_clPlayer.getMoney(MoneyType_Cash) < pTeam->m_stTeamTarget.uCash )
			{
				uCode = R_ResultCopymap_Create_NeedCash;
				return false;
			}
		}
		if ( !m_clPlayer.getGameItemMgr().canRemoveItem(pCopymapModel->mapNeedItem, ItemBindType_All))
		{
			uCode = R_ResultCopymap_Create_NeedItem;
			return false;
		}

		if (pTeam && pTeam->uLeaderID == m_clPlayer.getPlayerID() && pTeam->m_stTeamTarget.uCash  )
		{
			m_clPlayer.updateMoney(MoneyType_Cash, -((int64)pTeam->m_stTeamTarget.uCash),DoingType_Copymap_Create);
			uCash = pTeam->m_stTeamTarget.uCash;
		}

		m_clPlayer.getGameItemMgr().removeItem(pCopymapModel->mapNeedItem, ItemBindType_All,DoingType_Copymap_Create);
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool CPlayerCopymap::canTeamForceEnter(const _stCopymapModel* pCopymapModel,CSceneMonster* pNpc,uint32& uCash,uint8& uCode)
{
	uCode = R_ResultCopymap_Fail;
	if (!pCopymapModel || !pNpc)
		return false;

	//是否在时间内
	if (!pCopymapModel->isInOpenTime((uint64)getTime()))
	{
		uCode = R_ResultCopymap_Create_NoTime;
		return false;
	}

	//是否需要限制玩家本人
	if (pNpc->getMonsterModel()->check_mark(MonsterMark_LimitOwner) && pNpc->getOwnerPlayerID() != m_clPlayer.getPlayerID())
	{
		uCode = R_ResultCopymap_LimitOwner;
		return false;
	}

	//组队副本
	SET_U32 setMember;
	if (CopymapEnterType_Team == pCopymapModel->uEnterType)
	{
		_stTeamInfo* pTeamInfo = m_clPlayer.getTeam();
		if (!pTeamInfo)
		{
			uCode = R_ResultCopymap_Create_NeedTeam;
			return false;
		}

		//是否队长开启
		if (pCopymapModel->check_mark(CopymapMark_LeaderOpen))
		{
			if (m_clPlayer.getTeam()->uLeaderID != m_clPlayer.getPlayerID())
			{
				uCode = R_ResultCopymap_Create_NeedLeader;
				return false;
			}
		}

		stl_map<uint32,_stTeamMember>::iterator _pos = pTeamInfo->mapMember.begin();
		for (; _pos != pTeamInfo->mapMember.end(); ++_pos)
		{
			setMember.insert(_pos->second.stAvatar.stDisplay.uPlayerID);
		}
	}
	else if (CopymapEnterType_Single == pCopymapModel->uEnterType) //个人进入
	{
		setMember.insert(m_clPlayer.getPlayerID());
	}
	
	//检查条件
	ITER_SET_U32 _pos = setMember.begin();
	for (; _pos != setMember.end(); ++_pos)
	{
		uint32 uMemberID =  *_pos;
		CScenePlayer* pMember = pNpc->findCellPlayer(uMemberID);
		if (!pMember)
		{
			uCode = R_ResultCopymap_MemberNotAround;
			return false;
		}

		if ( pMember->getCopymapState() != PlayerCopymapState_None )
		{
			uCode = R_ResultCopymap_EnterIng;
			return false;
		}

		////押镖中
		//if (pMember->getPlayerDart()->getDartGlobalID() != 0 && pCopymapModel->uType != CopymapType_DartRob)
		//{
		//	uCode = R_ResultCopymap_In_Dartcar;
		//	return false;
		//}

		//前提条件
		if (!pMember->canPreCondition(pCopymapModel->dmapPreCondition,1))
		{
			uCode = R_ResultCopymap_Create_PreCondition;
			return false;
		}

		//次数
		_stPlayerCopymap* pInfo = pMember->getPlayerCopymap()->getInfo(pCopymapModel->uCopymapID);
		if ( pCopymapModel->uEnterCount && pInfo && pInfo->uEnterCount >= pCopymapModel->uEnterCount )
		{
			uCode = R_ResultCopymap_Create_Count;
			return false;
		}

		//发送给世界服
		if (!pMember->isWorldConneted())
			return false;
	}

	////帮派副本
	//if (pCopymapModel->uType == CopymapType_FactionBoss )
	//{
	//	CPlayerFactionBoss* pPlayerFactionBoss = (CPlayerFactionBoss*)m_clPlayer.getPlayerFun(GameSystemType_FactionBoss);
	//	if (!pPlayerFactionBoss)
	//		return false;

	//	if (pPlayerFactionBoss->isEnterCooling())
	//	{
	//		uCode = R_ResultCopymap_FactionBoss_Cooling;
	//		return false;
	//	}

	//	if (!m_clPlayer.getFactionID())
	//	{
	//		uCode = R_ResultCopymap_FactionBoss_NeedFaction;
	//		return false;
	//	}
	//}
	
		//{// 消耗
		//// 赏金团
		//_stTeamInfo* pTeam = m_clPlayer.getTeam();
		//if (pTeam && pTeam->uLeaderID == m_clPlayer.getPlayerID() && pTeam->m_stTeamTarget.uCash  )
		//{
		//	const _stTeamTargetModel * pTeamTargetModel = g_clTeamTargetData.findTeamTargetData(pTeam->m_stTeamTarget.uTargetid);
		//	if (!pTeamTargetModel || !(pTeamTargetModel->uType == TeamTargetMainType_Copymap || pTeamTargetModel->uType == TeamTargetMainType_Copymap_Difficulty ) || pTeamTargetModel->uParam !=  pCopymapModel->uCopymapID )
		//	{
		//		uCode = R_ResultCopymap_Create_CashCopymapID;
		//		return false;
		//	}

		//	if ( m_clPlayer.getMoney(MoneyType_Cash) < pTeam->m_stTeamTarget.uCash )
		//	{
		//		uCode = R_ResultCopymap_Create_NeedCash;
		//		return false;
		//	}
		//}
		//if ( !m_clPlayer.getGameItemMgr().canRemoveItem(pCopymapModel->mapNeedItem, ItemBindType_All))
		//{
		//	uCode = R_ResultCopymap_Create_NeedItem;
		//	return false;
		//}

		//if (pTeam && pTeam->uLeaderID == m_clPlayer.getPlayerID() && pTeam->m_stTeamTarget.uCash  )
		//{
		//	m_clPlayer.updateMoney(MoneyType_Cash, -((int64)pTeam->m_stTeamTarget.uCash),DoingType_Copymap_Create);
		//	uCash = pTeam->m_stTeamTarget.uCash;
		//}
		//m_clPlayer.getGameItemMgr().removeItem(pCopymapModel->mapNeedItem, ItemBindType_All,DoingType_Copymap_Create);
	//}

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool CPlayerCopymap::canPrize		(uint32 uCopymapID)
{
	const _stCopymapModel* pCopymapModel = g_clCopymapData.find_model(uCopymapID);
	if (!pCopymapModel)
		return false;

	if (!pCopymapModel->uDayPrizeCount)
		return true;

	_stPlayerCopymap* pInfo = getInfo(uCopymapID);
	if (pInfo && pInfo->uEnterCount <= pCopymapModel->uDayPrizeCount)
		return true;

	return false;
}
//-------------------------------------------------------------
//------------------------------ 
bool CPlayerCopymap::copymap_create(uint16 uCopymapID,uint32 uNpcID,uint8& uCode)
{
	uCode = R_ResultCopymap_Fail;
	const _stCopymapModel* pCopymapModel = g_clCopymapData.find_model(uCopymapID);
	if (!pCopymapModel)
		return false;

	uint32 uCash = 0;
	if (!canCreate(pCopymapModel,uCash,uCode))
		return false;

	uint16 uLevel = 0;
	//if (pCopymapModel->check_mark(CopymapMark_UseTeamLevel) /*&&  m_clPlayer.getPlayerTeam())
		//uLevel = m_clPlayer.getPlayerTeam()->getAverageLevel()*/;

	//增加标记
	m_clPlayer.setCopymapState(PlayerCopymapState_WaitEnter);

	PBG2WCopymapCreate pbAsk;
	pbAsk.set_copymapid(uCopymapID);
	PBCreateCellParam* pbParam = pbAsk.mutable_param();
	if (pbParam)
	{
		m_clPlayer.getPlayerBase().saveToProtobuf(*pbParam->mutable_display());
		pbParam->set_npcid(uNpcID);
		pbParam->set_level(uLevel);
		pbParam->set_cash(uCash);
	}

	m_clPlayer.sendMsgToWorld(P_G2W_Protocol_Copymap, G2W_Copymap_Create, &pbAsk, uCode);

	uCode = R_ResultCopymap_Succeed;
	return true;
}
//-------------------------------------------------------------
//------------------------------ 通过NPC立即进入
bool CPlayerCopymap::copymap_enternpc(uint16 uCopymapID,uint32 uNpcUnitID,uint8& uCode)
{
	uCode = R_ResultCopymap_Fail;
	const _stCopymapModel* pCopymapModel = g_clCopymapData.find_model(uCopymapID);
	if (!pCopymapModel)
		return false;

	CSceneUnit* pNpcUnit = m_clPlayer.findCellObject(uNpcUnitID);
	if (!pNpcUnit)
		return false;

	CSceneMonster* pMonster = pNpcUnit->asMonster();
	if (!pMonster || !pMonster->getMonsterModel())
		return false;

	uint32 uCash = 0;
	if (!canTeamForceEnter(pCopymapModel,pMonster,uCash,uCode))
		return false;

	uint16 uLevel = 0;
	//if (pCopymapModel->check_mark(CopymapMark_UseTeamLevel) &&  m_clPlayer.getPlayerTeam())
	//	uLevel = m_clPlayer.getPlayerTeam()->getAverageLevel();

	//增加标记
	m_clPlayer.setCopymapState(PlayerCopymapState_WaitEnter);

	//是否删除
	if (pMonster->getMonsterModel()->check_mark(MonsterMark_EnterCopymapDel))
	{
		pMonster->exitScene();
	}

	PBG2WCopymapCreate pbAsk;
	pbAsk.set_copymapid(uCopymapID);
	PBCreateCellParam* pbParam = pbAsk.mutable_param();
	if (pbParam)
	{
		m_clPlayer.getPlayerBase().saveToProtobuf(*pbParam->mutable_display());
		pbParam->set_npcid(pMonster->getMonsterID());
		pbParam->set_level(uLevel);
		pbParam->set_isteamfore(true);
		pbParam->set_cash(uCash);
	}

	m_clPlayer.sendMsgToWorld(P_G2W_Protocol_Copymap, G2W_Copymap_Create, &pbAsk, uCode);

	uCode = R_ResultCopymap_Succeed;
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool CPlayerCopymap::copymap_exit(uint16 uCopymapID,uint8& uCode)
{
	uCode = R_ResultCopymap_Fail;

	const _stCopymapModel* pCopymapModel = g_clCopymapData.find_model(uCopymapID);
	CHECKF(pCopymapModel != NULL);
	CGameSceneCell* pGameSceneCell = m_clPlayer.getSceneCell();
	CHECKF(pGameSceneCell != NULL);
 	if ( pGameSceneCell->getCopymapModel() != pCopymapModel )
 		return false;

	leaveCopymap();
	uCode = R_ResultCopymap_Succeed;
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
void	CPlayerCopymap::leaveCopymap					()
{
	CGameSceneCell* pCell = m_clPlayer.getSceneCell();
	if ( !pCell )
		return ;

	_stCopymapModel* pCopymapModel = pCell->getCopymapModel();
	if (!pCopymapModel)
		return ;

	if ( pCopymapModel->uLeaveType == CopymapLeaveType_OldPositon && m_clPlayer.getPlayerBase().uOldSceneID)
	{
		if (m_clPlayer.playerFlyScene_ByDynID(m_clPlayer.getPlayerBase().uOldSceneID,m_clPlayer.getPlayerBase().uOldSceneDynID,m_clPlayer.getPlayerBase().fOldPosX,m_clPlayer.getPlayerBase().fOldPosY) )
			return ;
	}

	if ( m_clPlayer.playerFlyScene_ByDynID(pCopymapModel->uLeaveSceneID,0,pCopymapModel->uLeaveRegionID) )
		return ;

	m_clPlayer.playerFlyScene_ByDynID(1,0,1);
}
//-------------------------------------------------------------
//------------------------------ 
_stPlayerCopymap*	CPlayerCopymap::getInfo					(uint16 uCopymapID)
{
	return m_mapCopymapInfo.find(uCopymapID);
}

//-------------------------------------------------------------
//------------------------------ 
void	CPlayerCopymap::on_copymap_enter			(PBW2GCopymapEnter& ack,uint8 uCode)
{
	m_clPlayer.setCopymapState(PlayerCopymapState_None);
	if ( R_ResultCopymap_Succeed != uCode )
		return ;

	const _stCopymapModel* pCopymapModel = g_clCopymapData.find_model(ack.sceneid() );
	if (!pCopymapModel)
		return ;

	_stPlayerCopymap* pInfo = m_mapCopymapInfo.find(ack.sceneid());
	if (  pCopymapModel->uEnterCount && pInfo && pInfo->uEnterCount >= pCopymapModel->uEnterCount )
		return ;

	//删除匹配
	m_clPlayer.delState(StateType_Match);

	uint16 uEnterRegionID = pCopymapModel->getBirthRegion(m_clPlayer.getCampID());
	if (!m_clPlayer.playerFlyScene_ByDynID(ack.sceneid(),ack.dynid(),uEnterRegionID ))
		return ;

	if (pCopymapModel->uEnterCount && !pInfo )
	{
		pInfo = m_mapCopymapInfo.allocate(ack.sceneid());
		CHECK(NULL != pInfo);
		pInfo->uCopymapID	= ack.sceneid();
	}

	if (pCopymapModel->uCopymapTimeType == CopymapCopymapTimeType_Enter && ack.reason() ==  CopymapEnterReason_Assist )
		++m_uAssistCount;

	if (pInfo)
	{
		
		if (pCopymapModel->uCopymapTimeType == CopymapCopymapTimeType_Enter && pCopymapModel->uEnterCount)
			++pInfo->uEnterCount;

		PBCopymapInfo msg;
		pInfo->saveToProtobuf(msg);
		m_clPlayer.sendMsg(P_S2C_Protocol_Copymap,S2C_Copymap_InfoUpdate,&msg);

		synWorldServerOne(*pInfo);
	}
}
//-------------------------------------------------------------
//------------------------------ 
void CPlayerCopymap::on_copymap_complete(const _stCopymapModel* pModel,_emCopymapCompleteType emType,_emCopymapEnterReason emReason,uint32 uExtraNum)
{
	if (!pModel)
		return;

	//缉拿宝藏悬赏副本成功才扣次数
	if (emType != CopymapCompleteType_Failed)
	{
		 if (CopymapMainType_Arrest == pModel->uMainType || CopymapMainType_Treasure == pModel->uMainType ||  CopymapMainType_Reward == pModel->uMainType)
		 {
			 m_clPlayer.addDayCopymapCount(pModel->uMainType);
		 }
	}

	//CPlayerQuestMgr* pQuestMgr = m_clPlayer.getPlayerQuestMgr();
	//if (pQuestMgr)
	//	pQuestMgr->callback_completeCopyMap(pModel->uCopymapID, 1);

	//if (m_clPlayer.getPlayerTitle())
	//	m_clPlayer.getPlayerTitle()->callback_completeCopymap(pModel->uCopymapID);
}
//-------------------------------------------------------------
//------------------------------ 
bool CPlayerCopymap::copymap_invite(uint16 uCopymapID,uint32 uTargetID,uint8& uCode)
{
	uCode = R_ResultCopymap_Fail;
	if ( m_clPlayer.getCopymapState() != PlayerCopymapState_None )
		return false;

	const _stCopymapModel* pCopymapModel = g_clCopymapData.find_model(uCopymapID);
	CHECKF(pCopymapModel != NULL);
	if ( pCopymapModel->uEnterType != CopymapEnterType_Team )
	{
		uCode = R_ResultCopymap_Invite_EnterType;
		return false;
	}

	if ( !m_clPlayer.isInCopymap() )
	{
		uCode = R_ResultCopymap_Invite_NotInCopymap;
		return false;
	}

	_stTeamInfo* pTeam = m_clPlayer.getTeam();
	if ( !pTeam || pTeam->mapMember.find(uTargetID) == pTeam->mapMember.end() )
	{
		uCode = R_ResultCopymap_Invite_NotTeamMember;
		return false;
	}

	//发送给世界服
	if (!m_clPlayer.isWorldConneted())
		return false;

	PBG2WCopymapInvite pbAsk;
	pbAsk.set_copymapid(uCopymapID);
	pbAsk.set_playerid(m_clPlayer.getPlayerID());
	pbAsk.set_targetid(uTargetID);
	m_clPlayer.sendMsgToWorld(P_G2W_Protocol_Copymap, G2W_Copymap_Invite, &pbAsk, uCode);

	uCode = R_ResultCopymap_Succeed;
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool CPlayerCopymap::copymap_acceptinvite(uint16 uCopymapID,uint32 uTargetID,bool bOk,uint8& uCode)
{
	uCode = R_ResultCopymap_Fail;
	if ( m_clPlayer.getCopymapState() != PlayerCopymapState_None )
		return false;

	const _stCopymapModel* pCopymapModel = g_clCopymapData.find_model(uCopymapID);
	CHECKF(pCopymapModel != NULL);
	if ( pCopymapModel->uEnterType != CopymapEnterType_Team )
	{
		uCode = R_ResultCopymap_AcceptInvite_EnterType;
		return false;
	}

	if ( m_clPlayer.isInCopymap() )
	{
		uCode = R_ResultCopymap_AcceptInvite_InCopymap;
		return false;
	}

	_stTeamInfo* pTeam = m_clPlayer.getTeam();
	if ( !pTeam || pTeam->mapMember.find(uTargetID) == pTeam->mapMember.end())
	{
		uCode = R_ResultCopymap_AcceptInvite_NotTeamMember;
		return false;
	}
	if (pTeam->m_stTeamTarget.uCash && m_uAssistCount >= (uint32)g_clConstantData.Copymap_CashCount )
	{
		uCode = R_ResultCopymap_AcceptInvite_CashCount;
		return false;
	}

	//发送给世界服
	if (!m_clPlayer.isWorldConneted())
		return false;

	if ( !pTeam->m_stTeamTarget.uCash && !m_clPlayer.getGameItemMgr().removeItem(pCopymapModel->mapNeedItem, ItemBindType_All,DoingType_Copymap_Create))
	{
		uCode = R_ResultCopymap_Create_NeedItem;
		return false;
	}

	PBG2WCopymapAcceptInvite pbAsk;
	pbAsk.set_copymapid(uCopymapID);
	pbAsk.set_playerid(m_clPlayer.getPlayerID());
	pbAsk.set_targetid(uTargetID);
	pbAsk.set_accept(bOk);
	m_clPlayer.sendMsgToWorld(P_G2W_Protocol_Copymap, G2W_Copymap_AcceptInvite, &pbAsk, uCode);

	uCode = R_ResultCopymap_Succeed;
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool CPlayerCopymap::copymap_teammemberinfo(uint16 uCopymapID,uint8& uCode)
{
	uCode = R_ResultCopymap_Fail;
	const _stCopymapModel* pCopymapModel = g_clCopymapData.find_model(uCopymapID);
	CHECKF(pCopymapModel != NULL);

	_stTeamInfo* pTeam = m_clPlayer.getTeam();
	if ( !pTeam )
	{
		uCode = R_ResultCopymap_TeamMemberInfo_NoTeam;
		return false;
	}

	//发送给世界服
	if (!m_clPlayer.isWorldConneted())
		return false;

	PBG2WCopymapTeamMemberInfo pbAsk;
	pbAsk.set_copymapid(uCopymapID);
	pbAsk.set_playerid(m_clPlayer.getPlayerID());
	m_clPlayer.sendMsgToWorld(P_G2W_Protocol_Copymap, G2W_Copymap_TeamMemberInfo, &pbAsk, uCode);

	uCode = R_ResultCopymap_Succeed;
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CPlayerCopymap::copymap_clear					(uint16 uCopymapID)
{
	MAP_CopymapInfo::iterator _pos = m_mapCopymapInfo.begin();
	MAP_CopymapInfo::iterator _end = m_mapCopymapInfo.end();
	for ( ; _pos != _end ; ++_pos )
	{
		_stPlayerCopymap* pInfo = _pos->second;
		if ( !pInfo )
			continue;

		if ( uCopymapID && pInfo->uCopymapID != uCopymapID )
			continue;

		pInfo->uEnterCount = 0;

		PBCopymapInfo msg;
		pInfo->saveToProtobuf(msg);
		m_clPlayer.sendMsg(P_S2C_Protocol_Copymap,S2C_Copymap_InfoUpdate,&msg);
	}

	synWorldServerAll();
	return true;
}