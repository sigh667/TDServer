/*------------- worldPacket.cpp
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.01
* Date         : 2011/2/24 9:48:55
*
*/ 
/***************************************************************
* 
***************************************************************/
#include "main/gameServer.h"
#include "operators_struct.h"
#include "open_qq/open_qq_callback.h"
#include "log/log.h"
//------------------------------ 
#include "network/netConnect.h"
#include "packet/worldPacket.h"
//------------------------------ 
#include "gameLogin/gameLogin.h"
#include "gameSave/gameSave.h"
#include "gameScene/gameSceneMgr.h"
#include "gameScene/copymapBase.h"
//#include "gameScene/copymapManorLobby.h"
//#include "gameScene/copymapManorBattleField.h"

#include "protocol/GameStruct.pb.h"
#include "protocol/WAGProtocol.pb.h"
#include "protocol/WAGMsg.pb.h"
#include "protocol/Team.pb.h"
#include "protocol/WAGMsg.pb.h"
#include "protocol/PvpWar.pb.h"

#include "gameLogin/gamePlayerMgr.h"
#include "gameLogic/gamePlayerFun/gamePlayerFun.h"
#include "gameLogic/gameMail/gameMail.h"
//#include "gameLogic/playerTeam/playerTeam.h"
//#include "gameLogic/playerFaction/playerFaction.h"
#include "gameLogic/gameBroadcast/gameBroadcast.h"
#include "gameLogic/playerCopymap/playerCopymap.h"
//#include "gameLogic/playerDart/playerDart.h"
//#include "gameLogic/playerQuestion/playerQuestion.h"
//#include "gameLogic/playerPvpWar/playerPvpWar.h"
//#include "gameLogic/playerCorps/playerCorps.h"
//#include "gameLogic/playerManor/playerManor.h"
//#include "gameLogic/playerSinglePvp/playerSinglePvp.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
CWorldPacket::CWorldPacket()
	: m_gpHandleMsg				(_emPW2G_Protocol_ARRAYSIZE,				this)
	, m_gpHandlePlayerSubMsg	(_emW2G_Protocol_Player_ARRAYSIZE,			this)
	, m_gpHandleSceneSubMsg		(_emW2G_Protocol_Scene_ARRAYSIZE,			this)
	, m_gpHandleCopymapSubMsg	(_emW2G_Protocol_Copymap_ARRAYSIZE,			this)
	, m_gpHandleCommonSubMsg	(_emW2G_Protocol_Common_ARRAYSIZE,			this)
	, m_gpHandleFactionSubMsg	(_emS2C_Faction_Protocol_ARRAYSIZE,			this)
	, m_gpHandleDartCarSubMsg 	(_emW2G_Protocol_DartCar_ARRAYSIZE,			this)
	, m_gpHandleQuestionSubMsg	(_emS2C_AnswerQuestion_Protocol_ARRAYSIZE,	this)
	, m_gpHandleManorMsg		(_emW2G_Manor_Protocol_ARRAYSIZE,			this)

	

{
	initialize();
	m_gpHandleMsg.bind(P_W2G_Protocol_Start			, &CWorldPacket::handle_World_Start		);
	m_gpHandleMsg.bind(P_W2G_Protocol_Player		, &CWorldPacket::handle_World_Player	);
	m_gpHandleMsg.bind(P_W2G_Protocol_Scene			, &CWorldPacket::handle_World_Scene		);
	m_gpHandleMsg.bind(P_W2G_Protocol_Team			, &CWorldPacket::handle_World_Team		);
	m_gpHandleMsg.bind(P_W2G_Protocol_Copymap		, &CWorldPacket::handle_World_Copymap	);
	m_gpHandleMsg.bind(P_W2G_Protocol_Common		, &CWorldPacket::handle_World_Common	);
	m_gpHandleMsg.bind(P_W2G_Protocol_Faction		, &CWorldPacket::handle_World_Faction	);
	m_gpHandleMsg.bind(P_W2G_Protocol_Broadcast		, &CWorldPacket::handle_World_Broadcast	);
	m_gpHandleMsg.bind(P_W2G_Protocol_DartCar		, &CWorldPacket::handle_World_DartCar	);
	m_gpHandleMsg.bind(P_W2G_Protocol_AnswerQuestion, &CWorldPacket::handle_World_Question	);
	m_gpHandleMsg.bind(P_W2G_Protocol_PvpWar		, &CWorldPacket::handle_World_PvpWar	);
	m_gpHandleMsg.bind(P_W2G_Protocol_Corps			, &CWorldPacket::handle_World_Corps		);
	m_gpHandleMsg.bind(P_W2G_Protocol_Manor			, &CWorldPacket::handle_World_Manor		);
	m_gpHandleMsg.bind(P_W2G_Protocol_SinglePvp		, &CWorldPacket::handle_World_SinglePvp	);

	m_gpHandlePlayerSubMsg.bind(W2G_Player_Load				, &CWorldPacket::handle_World_Player_Load			);
	m_gpHandlePlayerSubMsg.bind(W2G_Player_SaveDone			, &CWorldPacket::handle_World_Player_SaveDone		);
	m_gpHandlePlayerSubMsg.bind(W2G_Player_FlyMapAck		, &CWorldPacket::handle_World_Player_FlyMapAck		);
	m_gpHandlePlayerSubMsg.bind(W2G_Player_ExitAsk			, &CWorldPacket::handle_World_Player_KickOutAsk		);
	m_gpHandlePlayerSubMsg.bind(W2G_Player_OfflineDataAdd	, &CWorldPacket::handle_World_Player_OfflineDataAdd	);
	m_gpHandlePlayerSubMsg.bind(W2G_Player_OfflineDataLoad	, &CWorldPacket::handle_World_Player_OfflineDataLoad);

	m_gpHandleSceneSubMsg.bind(W2G_Scene_CreateCell		, &CWorldPacket::handle_World_Scene_CreateCell	);

	m_gpHandleCopymapSubMsg.bind(W2G_Copymap_Enter		, &CWorldPacket::handle_World_Copymap_Enter		);
	m_gpHandleCopymapSubMsg.bind(W2G_Copymap_Exit		, &CWorldPacket::handle_World_Copymap_Exit		);
	m_gpHandleCopymapSubMsg.bind(W2G_Copymap_PlayerInfo	, &CWorldPacket::handle_World_Copymap_PlayerInfo);
	m_gpHandleCopymapSubMsg.bind(W2G_Copymap_KickPlayer	, &CWorldPacket::handle_World_Copymap_KickPlayer);

	m_gpHandleFactionSubMsg.bind(S2C_Faction_Create					, &CWorldPacket::handle_World_Faction_Create					);
	m_gpHandleFactionSubMsg.bind(S2C_Faction_Syn					, &CWorldPacket::handle_World_Faction_Syn						);
	m_gpHandleFactionSubMsg.bind(S2C_Faction_UpgradeSelfSkill		, &CWorldPacket::handle_World_Faction_UpgradeSelfSkill			);
	m_gpHandleFactionSubMsg.bind(S2C_Faction_DonateMoney			, &CWorldPacket::handle_World_Faction_DonateMoney				);
	m_gpHandleFactionSubMsg.bind(S2C_Faction_UpdateSelfContribution	, &CWorldPacket::handle_World_Faction_UpdateSelfContribution	);

	m_gpHandleCommonSubMsg.bind(W2G_Common_GmReloadData		, &CWorldPacket::handle_World_Common_GMReloadData		);
	m_gpHandleCommonSubMsg.bind(W2G_Common_GmWorldStartTime	, &CWorldPacket::handle_World_Common_GMWorldStartTime	);
	m_gpHandleCommonSubMsg.bind(W2G_Common_Mail				, &CWorldPacket::handle_World_Common_Mail				);
	m_gpHandleCommonSubMsg.bind(W2G_Common_MsgTransfer		, &CWorldPacket::handle_World_Common_MsgTransfer		);
	m_gpHandleCommonSubMsg.bind(W2G_Common_TreasurePrize	, &CWorldPacket::handle_World_Common_TreasurePrize		);
	m_gpHandleCommonSubMsg.bind(W2G_Common_ForbitSpeak		, &CWorldPacket::handle_World_Common_ForbitSpeak		);
	
	m_gpHandleDartCarSubMsg.bind(W2G_DartCar_Fly_Ack		, &CWorldPacket::handle_World_DartCar_Fly				);
	m_gpHandleDartCarSubMsg.bind(W2G_DartCar_Fly_Create_Ntf	, &CWorldPacket::handle_World_DartCar_Fly_Create		);
	m_gpHandleDartCarSubMsg.bind(W2G_DartCar_RobFinished_Ack, &CWorldPacket::handle_World_DartCar_RobFinished		);
	m_gpHandleDartCarSubMsg.bind(W2G_DartCar_MemberLeave_Ntf, &CWorldPacket::handle_World_DartCar_MemberLeave		);
	m_gpHandleDartCarSubMsg.bind(W2G_DartCar_FindDartcar_Ack, &CWorldPacket::handle_World_DartCar_Find				);

	
	m_gpHandleQuestionSubMsg.bind(S2C_AnswerQuestion_QuestionResult	, &CWorldPacket::handle_World_AnswerQuestion_Result	);

	m_gpHandleManorMsg.bind(W2G_Manor_BattleField_FighterCount_Ntf		, &CWorldPacket::handle_World_Manor_Battlefield_Ntf						);
	m_gpHandleManorMsg.bind(W2G_Manor_Copymap_Ntf						, &CWorldPacket::handle_World_Manor_Copymap_Ntf						);

}

//-------------------------------------------------------------
//------------------------------ 
CWorldPacket::~CWorldPacket()
{
}

//-------------------------------------------------------------
//------------------------------ 
void CWorldPacket::initialize()
{
	Parent::initialize();

	m_pConnect		= NULL;
}

//-------------------------------------------------------------
//------------------------------ 断开回调
void	CWorldPacket::onDisconnect()
{
	g_pServer->unregisterAllWorld();

	CLog::warn("与世界服务器断开");
}

//-------------------------------------------------------------
//------------------------------ 
bool	CWorldPacket::send(bool bCheck/* = true*/)
{
	if (!m_pConnect || !Parent::send(bCheck))
		return false;

	return m_pConnect->send(m_szBuffer,getPosition());
}

//-------------------------------------------------------------
//------------------------------ 
void	CWorldPacket::disconnect()
{
	if (m_pConnect)
		m_pConnect->disconnect();
}

//-------------------------------------------------------------
//------------------------------ 数据包处理
bool	CWorldPacket::onPacketSrv(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	if (!m_pConnect)
		return false;
	return m_gpHandleMsg.onMessage(pHead, clStream);
}

//-------------------------------------------------------------
//------------------------------ 错误包处理
void	CWorldPacket::onBadPacket(uint32 uCount,_stPacketHead* pHead)
{
}

//-------------------------------------------------------------
//------------------------------ 
void	CWorldPacket::onConnect(bool bConnect)
{
	if(bConnect)
		send_Login_Ask();
	else
		CLog::error("world server connect fail!...");
}

//-------------------------------------------------------------
//------------------------------ 
bool	CWorldPacket::handle_Login_Ack(_stPacketHead &pHead, CNetStream &clStream)
{
	if (pHead.uEventFlag == NetResult_Succeed)
	{
		if(!clStream.read(m_stSvrInfo) )
			return false;

		CLog::warn("register world succeed!");
	}
	else if(pHead.uEventFlag == NetResult_VersionError)
	{
		CLog::error("register world fail(version error)!");
	}
	else
	{
		CLog::error("register world fail!");
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CWorldPacket::send_Login_Ask()
{
	build_Login_Ask();
	write(g_pServer->getSvrInfo());
	write(g_pServer->m_stHostClient);

	return send(false);
}
//------------------------------------------------------------------------------
//------------------------------------- 
bool	CWorldPacket::send_Register_Ask	(_stSvrInfo& stWorldInfo)
{
	buildPacketHeadsSrv(g_pServer->getSvrInfo(),stWorldInfo, P_G2W_Protocol_Register , 0 );
	write(g_pServer->m_stHostClient);

	return send();
}

//-------------------------------------------------------------
//------------------------------ 
bool	CWorldPacket::handle_World_Start			(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	switch ( pHead.uProtocol )
	{
	case W2G_Start_Register:
		{
			PBWorldSvrInfo info;
			if ( !clStream.ParseFromStream(info) )
				return false;

			g_pServer->setWorldStartTime(info.logicworldid(),info.starttime());
			g_pServer->registerWorld(pHead.stFromSrvInfo.uLogicWorldID);

			send_Register_Ask(pHead.stFromSrvInfo);
		}
		break;
	case W2G_Start_UnRegister:
		{
			g_pServer->unregisterWorld(pHead.stFromSrvInfo.uLogicWorldID);
		}
		break;
	default:
		return false;
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CWorldPacket::handle_World_Player			(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	return m_gpHandlePlayerSubMsg.onMessage(pHead, clStream);
}

//-------------------------------------------------------------
//------------------------------ 
bool	CWorldPacket::handle_World_Scene			(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	return m_gpHandleSceneSubMsg.onMessage(pHead, clStream);
}

//-------------------------------------------------------------
//------------------------------ 
bool	CWorldPacket::handle_World_Player_Load		(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	uint16	uSceneID;
	uint64	uSceneDynID;
	uint32	uPlayerID;
	uint8	uType;
	if (!clStream.read(uSceneID) || !clStream.read(uSceneDynID) || !clStream.read(uPlayerID) || !clStream.read(uType))
		return false;

	if(uType == _PInfo_Account)
	{
		PBAccount stAccount;
		if (!clStream.ParseFromStream(stAccount))
			return false;

		g_pGameLogin->login(stAccount,uPlayerID,uSceneID,uSceneDynID,0!=pHead.uEventFlag);
	}
	else
	{
		g_pGameLogin->update(uPlayerID,uType,clStream);
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CWorldPacket::handle_World_Player_SaveDone	(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBW2GPlayerSaveDone info;
	if ( !clStream.ParseFromStream(info))
		return false;

	g_pGameSave->onPlayerSaveDone(info.playerid(),info.type(),info.saveorder());
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CWorldPacket::handle_World_Player_FlyMapAck	(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBW2GPlayerFlyMapAck ack;
	if ( !clStream.ParseFromStream(ack))
		return false;

	CScenePlayer* pPlayer = g_pGamePlayerMgr->findPlayer(ack.playerid());
	if ( !pPlayer )
		return true;

	if ( !pHead.uEventFlag )
	{
		_stSvrInfo info;
		_stNetHost stTargetHost;
		info.uLogicWorldID = ack.pbsvrinfo().logicworldid();
		info.uServerType = ack.pbsvrinfo().servertype();
		info.uServerID = ack.pbsvrinfo().serverid();
		stTargetHost.setHost(ack.host().c_str());
		stTargetHost.setPort(ack.port());
		pPlayer->playerRealFlyScene(ack.sceneid(),ack.scenedynid(),ack.scenecellid(),ack.fixpos(), ack.sceneregionid(),ack.posx(),ack.posy(), info,stTargetHost);
	}
	else
	{
		pPlayer->setTransfer(false);
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CWorldPacket::handle_World_Player_KickOutAsk	(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBW2GPlayerExitAsk ask;
	if (!clStream.ParseFromStream(ask))
		return false;

	//踢出
	g_pGameLogin->kick(ask.playerid());

	PBG2WPlayerExitAck ack;
	ack.set_accountid(ask.accountid());
	ack.set_playerid(ask.playerid());
	ack.set_loginsn(ask.loginsn());
	sendMsg(g_pServer->getSvrInfo(),pHead.stFromSrvInfo, P_G2W_Protocol_Player,G2W_Player_ExitAck,&ack,pHead.uEventFlag);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CWorldPacket::handle_World_Scene_CreateCell	(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBW2GSceneCreateCell info;
	if (!clStream.ParseFromStream(info))
		return false;

	g_pGameSceneMgr->createSceneCell(info.sceneid(),info.scenedynid(),info.scenecellid(),info.param());
	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool CWorldPacket::handle_World_Team(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	uint32 uPlayerID;
	if ( !clStream.read(uPlayerID))
		return false;

	CScenePlayer* pPlayer = g_pGamePlayerMgr->findPlayer(uPlayerID);
	if (!pPlayer)
		return true;

	//switch ( pHead.uProtocol )
	//{
	//case W2G_Team_MyTeamInfo  : 
	//	{
	//		PBTeamInfo msg;
	//		if ( !clStream.ParseFromStream(msg))
	//			return false;

	//		CPlayerTeam* pTeam = pPlayer->getPlayerTeam();
	//		if (!pTeam )
	//			return true;

	//		pTeam->updateTeam(msg);
	//	}
	//	break;
	//case W2G_Team_AcceptTeamTask: 
	//	{
	//		PBTeamTask msg;
	//		if ( !clStream.ParseFromStream(msg))
	//			return false;

	//		CPlayerTeam* pTeam = pPlayer->getPlayerTeam();
	//		if (!pTeam )
	//			return true;

	//		pTeam->accpetTeamTaskFromWorld(msg);
	//	}
	//	break;
	//case W2G_Team_CompleteTeamTask: 
	//	{
	//		PBTeamTask msg;
	//		if ( !clStream.ParseFromStream(msg))
	//			return false;

	//		CPlayerTeam* pTeam = pPlayer->getPlayerTeam();
	//		if (!pTeam )
	//			return true;

	//		pTeam->completeTeamTaskFromWorld();
	//	}
	//	break;
	//case W2G_Team_Confirm: 
	//	{
	//		PBG2CTeamConfirmAck msg;
	//		if ( !clStream.ParseFromStream(msg))
	//			return false;

	//		CPlayerTeam* pTeam = pPlayer->getPlayerTeam();
	//		if (!pTeam )
	//			return true;

	//		pTeam->confirmFromWorld(msg.playerid(),msg.isconfirm());
	//	}
	//	break;
	//case W2G_Team_MemberAdd: 
	//	{
	//		PBTeamMember msg;
	//		if ( !clStream.ParseFromStream(msg))
	//			return false;

	//		CPlayerTeam* pTeam = pPlayer->getPlayerTeam();
	//		if (!pTeam )
	//			return true;

	//		pTeam->memberAddFromWorld(msg);
	//	}
	//	break;
	//case W2G_Team_MemberDel: 
	//	{
	//		PBTeamMember msg;
	//		if ( !clStream.ParseFromStream(msg))
	//			return false;

	//		CPlayerTeam* pTeam = pPlayer->getPlayerTeam();
	//		if (!pTeam )
	//			return true;

	//		pTeam->memberDelWorld(msg);
	//	}
	//	break;
	//default:
	//	return false;
	//}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool CWorldPacket::handle_World_Faction(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	uint32 uPlayerID;
	CHECKF(clStream.read(uPlayerID));
	return m_gpHandleFactionSubMsg.onMessage(pHead, clStream, uPlayerID);
}
//-------------------------------------------------------------
//------------------------------ 
bool CWorldPacket::handle_World_Question(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	uint32 uPlayerID;
	CHECKF(clStream.read(uPlayerID));
	return m_gpHandleQuestionSubMsg.onMessage(pHead, clStream, uPlayerID);
}
//-------------------------------------------------------------
//------------------------------ 
bool CWorldPacket::handle_World_PvpWar(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	uint32 uPlayerID;
	if ( !clStream.read(uPlayerID))
		return false;

	CScenePlayer* pPlayer = g_pGamePlayerMgr->findPlayer(uPlayerID);
	if (!pPlayer)
		return true;

	//CPlayerPvpWar* pPlayerPvpWar = pPlayer->getPlayerPvpWar();
	//if (!pPlayerPvpWar)
	//	return true;

	//switch ( pHead.uProtocol )
	//{
	//case W2G_PvpWar_EnterMatch  : 
	//	{
	//		PBBW2WPvpWarEnterMatchAck ack;
	//		CHECKF(clStream.ParseFromStream(ack));
	//		pPlayerPvpWar->enterMatchFromWorldAck(ack.warid(),pHead.uEventFlag);
	//	}
	//	break;
	//case W2G_PvpWar_ExitMatch: 
	//	{
	//		PBBW2WPvpWarExitMatchAck ack;
	//		CHECKF(clStream.ParseFromStream(ack));
	//		pPlayerPvpWar->exitMatchFromWorldAck(ack.playerid(),pHead.uEventFlag);
	//	}
	//	break;
	//case W2G_PvpWar_EnterReady: 
	//	{
	//		PBBW2WPvpWarReadyEnterAck ack;
	//		CHECKF(clStream.ParseFromStream(ack));
	//		pPlayerPvpWar->readyEnterFromWorldAck(ack.playerid(),pHead.uEventFlag);
	//	}
	//	break;
	//case W2G_PvpWar_ExitReady: 
	//	{
	//		PBBW2WPvpWarExitReadyEnterAck ack;
	//		CHECKF(clStream.ParseFromStream(ack));
	//		pPlayerPvpWar->exitEnterFromWorldAck(ack.playerid(),pHead.uEventFlag);
	//	}
	//	break;
	//case W2G_PvpWar_CompleteMatch: 
	//	{
	//		PBBW2WPvpWarCompleteMatchAck ack;
	//		CHECKF(clStream.ParseFromStream(ack));
	//		pPlayerPvpWar->completeMatchFromWorldAck(ack,pHead.uEventFlag);
	//	}
	//	break;
	//default:
	//	return false;
	//}
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool CWorldPacket::handle_World_Corps(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	uint32 uPlayerID;
	if ( !clStream.read(uPlayerID))
		return false;

	CScenePlayer* pPlayer = g_pGamePlayerMgr->findPlayer(uPlayerID);
	if (!pPlayer)
		return true;

	//CPlayerCorps* pPlayerCorps = pPlayer->getPlayerCorps();
	//if (!pPlayerCorps)
	//	return true;

	//switch ( pHead.uProtocol )
	//{
	//case W2G_Corps_ChangeJob  : 
	//	{
	//		PBW2GCorpsChangeJobAck ack;
	//		CHECKF(clStream.ParseFromStream(ack));
	//		pPlayerCorps->changeJob(ack.addjob(),ack.deljob());
	//	}
	//	break;
	//default:
	//	return false;
	//}
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool CWorldPacket::handle_World_Player_OfflineDataAdd(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	uint32 uPlayerID;
	uint16 uSysType;
	uint32 uSignType;
	if ( !clStream.read(uPlayerID) || !clStream.read(uSysType) || !clStream.read(uSignType) )
		return false;

	CScenePlayer* pPlayer	= g_pGamePlayerMgr->findPlayer(uPlayerID);
	CGamePlayerFun* pFun	= pPlayer ? pPlayer->getPlayerFun((_emGameSystemType)uSysType):NULL;
	if (!pFun || !pFun->handleOfflineData(uSignType, clStream))
	{
		_stSvrInfo& stInfo = getSvrInfo();
		CScenePlayer::offlineData(stInfo.uLogicWorldID, uPlayerID, uSysType, uSignType, clStream.getSpareBuffer(), clStream.getSpareSize());
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool CWorldPacket::handle_World_Player_OfflineDataLoad(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	uint32 uPlayerID;
	uint32 uOrderNum;
	if ( !clStream.read(uPlayerID) || !clStream.read(uOrderNum) )
		return false;

	CScenePlayer* pPlayer	= g_pGamePlayerMgr->findPlayer(uPlayerID);
	if (!pPlayer)
		return true;

	pPlayer->handleOfflineDataLoad(uOrderNum, clStream.getSpareBuffer(), clStream.getSpareSize() );
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CWorldPacket::handle_World_Copymap			(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	return m_gpHandleCopymapSubMsg.onMessage(pHead, clStream);
}

//-------------------------------------------------------------
//------------------------------ 
bool	CWorldPacket::handle_World_Copymap_Enter	(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	uint32 uPlayerID;
	CHECKF(clStream.read(uPlayerID));
	CScenePlayer* pPlayer	= g_pGamePlayerMgr->findPlayer(uPlayerID);
	if (!pPlayer)
		return true;

	PBW2GCopymapEnter ack;
	CHECKF(clStream.ParseFromStream(ack));
	CPlayerCopymap* pCopymap  = (CPlayerCopymap*)pPlayer->getPlayerFun(GameSystemType_Copymap);
	if ( pCopymap )
	{
		pCopymap->on_copymap_enter(ack,pHead.uEventFlag);
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CWorldPacket::handle_World_Copymap_Exit	(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBW2GCopymapExit ack;
	CHECKF(clStream.ParseFromStream(ack));
	CScenePlayer* pPlayer	= g_pGamePlayerMgr->findPlayer(ack.playerid());
	if (!pPlayer)
		return true;

	CPlayerCopymap* pCopymap  = (CPlayerCopymap*)pPlayer->getPlayerFun(GameSystemType_Copymap);
	if ( pCopymap )
	{
		pCopymap->leaveCopymap();
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CWorldPacket::handle_World_Copymap_PlayerInfo	(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBW2GCopymapPlayerInfo msg;
	CHECKF(clStream.ParseFromStream(msg));
	CGameSceneCell* pCell = g_pGameSceneMgr->findSceneCell_ByDynID(msg.sceneid(),msg.dynid() );
	if ( !pCell  )
		return true;

	CCopymapBase* pCopymap = pCell->getGameCopymap();
	if ( !pCopymap )
		return true;

	pCopymap->updatePlayerInfo(msg);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CWorldPacket::handle_World_Copymap_KickPlayer	(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBW2GCopymapKickPlayer msg;
	CHECKF(clStream.ParseFromStream(msg));
	CGameSceneCell* pCell = g_pGameSceneMgr->findSceneCell_ByDynID(msg.sceneid(),msg.dynid() );
	if ( !pCell  )
		return true;

	CScenePlayer* pPlayer = pCell->findPlayer(msg.playerid());
	if (!pPlayer)
		return true;

	CCopymapBase* pCopymap = pCell->getGameCopymap();
	if ( !pCopymap )
		return true;

	CPlayerCopymap* pPlayerCopymap = (CPlayerCopymap*)pPlayer->getPlayerFun(GameSystemType_Copymap);
	if (pPlayerCopymap)
	{
		pPlayerCopymap->leaveCopymap();
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CWorldPacket::handle_World_Faction_Create(_stPacketHead_Svr &pHead, CNetStream &clStream, uint32 playerid)
{
	PBC2GFactionCreate msg;
	if ( !clStream.ParseFromStream(msg))
		return false;

	CScenePlayer* pPlayer = g_pGamePlayerMgr->findPlayer(msg.playerid());
	if (!pPlayer)
		return false;

	//CPlayerFaction* pFaction = dynamic_cast<CPlayerFaction*> (pPlayer->getPlayerFun(GameSystemType_Faction));
	//if (!pFaction)
	//	return false;

	//return pFaction->onCreate_Ack(msg, pHead.uEventFlag);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CWorldPacket::handle_World_Faction_Syn(_stPacketHead_Svr &pHead, CNetStream &clStream, uint32 playerid)
{
	PBW2GFactionSyn msg;
	if ( !clStream.ParseFromStream(msg))
		return false;

	CScenePlayer* pPlayer = g_pGamePlayerMgr->findPlayer(playerid);
	if (!pPlayer)
		return false;

	//CPlayerFaction* pFaction = (CPlayerFaction*) (pPlayer->getPlayerFun(GameSystemType_Faction));
	//if (!pFaction)
	//	return false;

	//return pFaction->onSyn_Ack(msg);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CWorldPacket::handle_World_Faction_UpgradeSelfSkill(_stPacketHead_Svr &pHead, CNetStream &clStream, uint32 playerid)
{
	PBGAWUpgradeSelfSkill msg;
	if ( !clStream.ParseFromStream(msg))
		return false;

	CScenePlayer* pPlayer = g_pGamePlayerMgr->findPlayer(playerid);
	if (!pPlayer)
		return false;

	//CPlayerFaction* pFaction = (CPlayerFaction*) (pPlayer->getPlayerFun(GameSystemType_Faction));
	//if (!pFaction)
	//	return false;

	//return pFaction->onUpgradeSelfSkill_Ack(msg,pHead.uEventFlag);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CWorldPacket::handle_World_Faction_DonateMoney(_stPacketHead_Svr &pHead, CNetStream &clStream, uint32 playerid)
{
	PBGAWFactionDonateMoney msg;
	if ( !clStream.ParseFromStream(msg))
		return false;

	CScenePlayer* pPlayer = g_pGamePlayerMgr->findPlayer(playerid);
	if (!pPlayer)
		return false;

	////CPlayerFaction* pFaction = (CPlayerFaction*)(pPlayer->getPlayerFun(GameSystemType_Faction));
	////if (!pFaction)
	////	return false;

	////return pFaction->onDonateMoney_Ack(msg,pHead.uEventFlag);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CWorldPacket::handle_World_Faction_UpdateSelfContribution	(_stPacketHead_Svr &pHead, CNetStream& clStream, uint32 playerid)
{
	PBW2GFactionMemberAttr msg;
	if ( !clStream.ParseFromStream(msg))
		return false;

	CScenePlayer* pPlayer = g_pGamePlayerMgr->findPlayer(playerid);
	if (!pPlayer)
		return false;

	//CPlayerFaction* pFaction = (CPlayerFaction*) (pPlayer->getPlayerFun(GameSystemType_Faction));
	//if (!pFaction)
	//	return false;

	//return pFaction->onUpdateSelfContribution(msg);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CWorldPacket::handle_World_Common			(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	return m_gpHandleCommonSubMsg.onMessage(pHead, clStream);
}

//-------------------------------------------------------------
//------------------------------ 
bool	CWorldPacket::handle_World_Common_GMReloadData	(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	g_pServer->reLoadData();
	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CWorldPacket::handle_World_Common_GMWorldStartTime	(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBG2WGMWorldStartTime pbTime;
	CHECKF(clStream.ParseFromStream(pbTime));
	g_pServer->setWorldStartTime((uint16)pbTime.logicworldid(), (uint32)pbTime.stamp());
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CWorldPacket::handle_World_Common_Mail	(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBMail stMail;
	CHECKF(clStream.ParseFromStream(stMail));

	return CPlayerMail::sendMail(stMail);
}

//-------------------------------------------------------------
//------------------------------ 
bool	CWorldPacket::handle_World_Common_MsgTransfer	(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	uint32 uPlayerID;
	uint8 uMainPro;
	uint8 uProtocol;
	uint8 uEventFlag = pHead.uEventFlag;

	if (!clStream.read(uPlayerID) || !clStream.read(uMainPro) || !clStream.read(uProtocol))
		return false;

	CScenePlayer* pPlayer = g_pGamePlayerMgr->findPlayer(uPlayerID);
	if (!pPlayer)
		return false;

	return pPlayer->sendBuffer(uMainPro, uProtocol, clStream.getSpareBuffer(),clStream.getSpareSize(), uEventFlag);
}
//-------------------------------------------------------------
//------------------------------ 
bool	CWorldPacket::handle_World_Common_TreasurePrize	(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBW2GCommonTreasurePrizeAck ack;
	CHECKF(clStream.ParseFromStream(ack));

	CScenePlayer* pPlayer = g_pGamePlayerMgr->findPlayer(ack.playerid());
	if (!pPlayer)
		return false;

	pPlayer->treasurePrize(ack.treasureid(),ack.prizeid(),ack.posx(),ack.posy());
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CWorldPacket::handle_World_Common_ForbitSpeak	(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	uint32 uPlayerID = 0;
	CHECKF(clStream.read(uPlayerID));

	PBW2GCommonForbitSpeak ack;
	CHECKF(clStream.ParseFromStream(ack));

	CScenePlayer* pPlayer = g_pGamePlayerMgr->findPlayer(uPlayerID);
	if (!pPlayer)
		return false;

	pPlayer->addForbitSpeak(ack.channel(),ack.forbittime());
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CWorldPacket::handle_World_Broadcast			(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	uint16 uLogicWorldID;
	uint64 uID;
	uint8 uMainProtocol;
	uint8 uProtocol;
	if (!clStream.read(uLogicWorldID) || !clStream.read(uID) || !clStream.read(uMainProtocol) || !clStream.read(uProtocol) )
		return false;

	g_pGameBroadcast->on_world_broadCast(uLogicWorldID,(_emBroadcast_Channel)pHead.uProtocol,uID,uMainProtocol,uProtocol,clStream.getSpareBuffer(),clStream.getSpareSize(),pHead.uEventFlag);
	return false;
}

bool	CWorldPacket::handle_World_DartCar			(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	return m_gpHandleDartCarSubMsg.onMessage(pHead, clStream);
}

bool	CWorldPacket::handle_World_DartCar_Fly			(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	//PBW2GDartCarFlyAck pbDCFA;
	//CHECKF(clStream.ParseFromStream(pbDCFA));
	////找到场景
	//CGameSceneCell* pSceneCell = g_pGameSceneMgr->findSceneCell_ByDynID(pbDCFA.sourcesceneid(), pbDCFA.sourcescenedynid());
	//if (pSceneCell)
	//{
	//	//CSceneDartCar* pSDC = pSceneCell->findDartCarByGlobalID(pbDCFA.globaldartid());
	//	//if (pSDC)
	//	//{
	//	//	pSDC->onDartFlyAck(pbDCFA.targetsceneid(), pbDCFA.targetscenedynid());
	//	//}
	//}
	return true;
}

bool	CWorldPacket::handle_World_DartCar_RobFinished	(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	//PBW2GDartCarRobFinishedAck pbDCRFA;
	//CHECKF(clStream.ParseFromStream(pbDCRFA));

	////找到镖车, 设置状态
	//CGameSceneCell* pSceneCell = g_pGameSceneMgr->findSceneCell_ByDynID(pbDCRFA.sceneid(), pbDCRFA.scenedynid());
	//if (pSceneCell)
	//{
	//	CSceneDartCar* pSDC = pSceneCell->findDartCarByGlobalID(pbDCRFA.globaldartid());
	//	if (pSDC)
	//	{
	//		pSDC->setDartStatus((_emDartStatus)pbDCRFA.newstatus());
	//	}
	//}
	return true;
}

bool	CWorldPacket::handle_World_DartCar_MemberLeave(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	//PBW2GDartCarMemberLeaveNtf pbDCMLN;
	//CHECKF(clStream.ParseFromStream(pbDCMLN));

	////找到镖车, 设置状态
	//CGameSceneCell* pSceneCell = g_pGameSceneMgr->findSceneCell_ByDynID(pbDCMLN.sceneid(), pbDCMLN.scenedynid());
	//if (pSceneCell)
	//{
	//	CSceneDartCar* pSDC = pSceneCell->findDartCarByGlobalID(pbDCMLN.globaldartid());
	//	if (pSDC)
	//	{
	//		pSDC->onMemberLeave(pbDCMLN.playerid());
	//	}
	//}
	return true;
}


bool	CWorldPacket::handle_World_DartCar_Fly_Create		(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	//
	//PBW2GDartCarFlyCreateNtf pbDCFCN;
	//CHECKF(clStream.ParseFromStream(pbDCFCN));

	////进入场景创建
	//CGameSceneCell* pSceneCell = g_pGameSceneMgr->findSceneCell_ByDynID(pbDCFCN.sceneid(), pbDCFCN.scenedynid());
	//if (pSceneCell)
	//{
	//	_stDartInfoRT stDartInfoRT ;
	//	stDartInfoRT.initialize();
	//	stDartInfoRT.loadFromProtobuf(pbDCFCN.dartcarinfort());
	//	pSceneCell->createDartCar(stDartInfoRT, true);
	//}
	return true;
}

bool	CWorldPacket::handle_World_DartCar_Find	(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	//PBW2GFindDartCarAck pbAck;
	//CHECKF(clStream.ParseFromStream(pbAck));

	//CGameSceneCell* pSceneCell = g_pGameSceneMgr->findSceneCell_ByDynID(pbAck.sceneid(), pbAck.scenedynid());
	//if (pSceneCell )
	//{
	//	CScenePlayer * pPlayer = pSceneCell->findPlayer(pbAck.playerid());
	//	if (pPlayer
	//		&& pPlayer->getPlayerDart())
	//	{
	//		pPlayer->getPlayerDart()->onFindDartcar(pbAck.globaldartid(), pbAck.targetsceneid(), pbAck.targetscenedynid(), pbAck.x(), pbAck.y());
	//		return true;
	//	}
	//}
	//CScenePlayer * pPlayer = g_pGamePlayerMgr->findPlayer(pbAck.playerid());
	//if (pPlayer
	//	&& pPlayer->getPlayerDart())
	//{
	//	pPlayer->getPlayerDart()->onFindDartcar(0, 0, 0, 0, 0);
	//}
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CWorldPacket::handle_World_AnswerQuestion_Result(_stPacketHead_Svr &pHead, CNetStream &clStream, uint32 playerid)
{
	//PBG2CAnswerQuestion_QuestionResult msg;
	//if ( !clStream.ParseFromStream(msg))
	//	return false;

	//CScenePlayer* pPlayer = g_pGamePlayerMgr->findPlayer(playerid);
	//if (!pPlayer)
	//	return false;

	//CPlayerQuestion* pQuestion = dynamic_cast<CPlayerQuestion*> (pPlayer->getPlayerFun(GameSystemType_Question));
	//if (!pQuestion)
	//	return false;

	//return pQuestion->onResult_Ack(msg);
	return true;
}

//-------------------------------------------------------------
//------------------------------ 领地相关

bool	CWorldPacket::handle_World_Manor				(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	return m_gpHandleManorMsg.onMessage(pHead, clStream);
}

bool	CWorldPacket::handle_World_Manor_Battlefield_Ntf	(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	//PBW2GManorBattleFieldFightCountNtf msg;
	//if ( !clStream.ParseFromStream(msg))
	//	return false;
	////找到领地副本,
	//CGameSceneCell * pCell = g_pGameSceneMgr->findSceneCell_ByDynID(msg.sceneid(), msg.dynid());
	//if (pCell && pCell->getGameCopymap() 
	//	&& pCell->getGameCopymap()->getCopymapType() == CopymapType_ManorLobby)
	//{
	//	CCopymapManorLobby* p = dynamic_cast<CCopymapManorLobby*> (pCell->getGameCopymap());
	//	if (p)
	//	{
	//		p->onReportPlayerCountNtf(msg);
	//	}
	//}
	return true;
}

bool	CWorldPacket::handle_World_Manor_Copymap_Ntf	(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	//PBW2GManorCopymapNtf msg;
	//if ( !clStream.ParseFromStream(msg))
	//	return false;
	////找到领地副本,
	//CGameSceneCell * pCell = g_pGameSceneMgr->findSceneCell_ByDynID(msg.sceneid(), msg.dynid());
	//if (pCell && pCell->getGameCopymap() 
	//	&& pCell->getGameCopymap()->getCopymapType() == CopymapType_ManorLobby)
	//{
	//	CCopymapManorLobby* p = dynamic_cast<CCopymapManorLobby*> (pCell->getGameCopymap());
	//	if (p)
	//	{
	//		p->onCopymapNtf(msg);
	//	}
	//}
	//else if (pCell && pCell->getGameCopymap() 
	//	&& pCell->getGameCopymap()->getCopymapType() == CopymapType_ManorBattleField)
	//{
	//	CCopymapManorBattleField* p = dynamic_cast<CCopymapManorBattleField*> (pCell->getGameCopymap());
	//	if (p)
	//	{
	//		p->onCopymapNtf(msg);
	//	}

	//}

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool CWorldPacket::handle_World_SinglePvp(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	//uint32 uPlayerID;
	//if ( !clStream.read(uPlayerID))
	//	return false;

	//CScenePlayer* pPlayer = g_pGamePlayerMgr->findPlayer(uPlayerID);
	//if (!pPlayer)
	//	return false;

	//CPlayerSinglePvp* pPlayerSinglePvp = pPlayer->getPlayerSinglePvp();
	//if (!pPlayerSinglePvp)
	//	return true;

	//switch ( pHead.uProtocol )
	//{
	//case W2G_Protocol_SinglePvp_EnterMatch  : 
	//	{
	//		pPlayerSinglePvp->enterMatchFromWorldAck(pHead.uEventFlag);
	//	}
	//	break;
	//case W2G_Protocol_SinglePvp_ExitMatch: 
	//	{
	//		pPlayerSinglePvp->exitMatchFromWorldAck(pHead.uEventFlag);
	//	}
	//	break;
	//default:
	//	return false;
	//}
	return true;
}