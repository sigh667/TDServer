/*------------- globalPacket.cpp
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.01
* Date         : 2011/2/24 9:48:55
*
*/ 
/***************************************************************
* 
***************************************************************/
#include "network/netConnect.h"
#include "log/log.h"
//------------------------------ 
#include "packet/globalPacket.h"
#include "main/gameServer.h"
#include "gameLogin/gamePlayerMgr.h"
#include "protocol/GLAGMsg.pb.h"
//#include "gameLogic/playerTrade/playerTrade.h"
#include "protocol/MsgResult.pb.h"
//#include "gameLogic/playerSaleMoney/playerSaleMoney.h"
#include "gameLogic/gameMail/gameMail.h"
//#include "gameLogic/playerAuction/playerAuction.h"
//#include "gameLogic/playerFriend/playerFriend.h"
#include "gameLogic/playerCommon/playerCommon.h"
//#include "gameLogic/globalObject/worldBossMgr.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
CGlobalPacket::CGlobalPacket()
	: m_gpHandleMsg				(_emPGL2G_Protocol_ARRAYSIZE,			this)
	, m_gpGL2GTradeSubMsg		(_emGL2G_Trade_Protocol_ARRAYSIZE,		this)
	, m_gpGL2GSaleMoneySubMsg	(_emGL2G_SaleMoney_Protocol_ARRAYSIZE,	this)
	, m_gpGL2GAuctionSubMsg		(_emGL2G_Auction_Protocol_ARRAYSIZE,	this)
	, m_gpGL2GCommonSubMsg		(_emGLAG_Common_Protocol_ARRAYSIZE,		this)
	, m_gpGL2GFriendSubMsg		(_emGL2G_Friend_Protocol_ARRAYSIZE,		this)
	, m_gpGL2GWorldBossSubMsg	(_emGL2G_WorldBoss_Protocol_ARRAYSIZE,	this)
{
	initialize();
	m_gpHandleMsg.bind(P_GL2G_Protocol_Start,		&CGlobalPacket::handle_Global_Start		);
	m_gpHandleMsg.bind(P_GL2G_Protocol_ClientMsg,	&CGlobalPacket::handle_Global_ClientMsg	);
	m_gpHandleMsg.bind(P_GL2G_Protocol_Trade,		&CGlobalPacket::handle_Global_Trade		);
	m_gpHandleMsg.bind(P_GL2G_Protocol_SaleMoney,	&CGlobalPacket::handle_Global_SaleMoney	);
	m_gpHandleMsg.bind(P_GL2G_Protocol_Auction,		&CGlobalPacket::handle_Global_Auction	);
	m_gpHandleMsg.bind(P_GL2G_Protocol_Common,		&CGlobalPacket::handle_Global_Common	);
	m_gpHandleMsg.bind(P_GL2G_Protocol_Friend,		&CGlobalPacket::handle_Global_Friend	);
	m_gpHandleMsg.bind(P_GL2G_Protocol_WorldBoss,	&CGlobalPacket::handle_Global_WorldBoss	);

	m_gpGL2GTradeSubMsg.bind(GL2G_Trade_Begin					, &CGlobalPacket::handle_Global_Trade_Begin		);
	m_gpGL2GTradeSubMsg.bind(GL2G_Trade_End						, &CGlobalPacket::handle_Global_Trade_End		);
	m_gpGL2GTradeSubMsg.bind(GL2G_Trade_check					, &CGlobalPacket::handle_Global_Trade_check		);
	m_gpGL2GTradeSubMsg.bind(GL2G_Trade_delete					, &CGlobalPacket::handle_Global_Trade_delete	);
	m_gpGL2GTradeSubMsg.bind(GL2G_Trade_add						, &CGlobalPacket::handle_Global_Trade_add		);
	m_gpGL2GTradeSubMsg.bind(GL2G_Trade_Return					, &CGlobalPacket::handle_Global_Trade_Return	);

	m_gpGL2GSaleMoneySubMsg.bind(GL2G_SaleMoney_sale_Fail		, &CGlobalPacket::handle_Global_SaleMoney_sale_Fail			);
	m_gpGL2GSaleMoneySubMsg.bind(GL2G_SaleMoney_salecancel_Suc	, &CGlobalPacket::handle_Global_SaleMoney_salecancel_Suc	);

	m_gpGL2GAuctionSubMsg.bind(GL2G_Auction_sale_Fail			, &CGlobalPacket::handle_Global_Auction_sale_Fail			);
	m_gpGL2GAuctionSubMsg.bind(GL2G_Auction_salecancel_Suc		, &CGlobalPacket::handle_Global_Auction_salecancel_Suc		);
	m_gpGL2GAuctionSubMsg.bind(GL2G_Auction_buy_Fail			, &CGlobalPacket::handle_Global_Auction_buy_Fail			);

	m_gpGL2GCommonSubMsg.bind(GLAG_Common_FindPlayer			, &CGlobalPacket::handle_Global_Common_FindPlayer		);
	m_gpGL2GCommonSubMsg.bind(GLAG_Common_Mail					, &CGlobalPacket::handle_Global_Common_Mail				);
	m_gpGL2GCommonSubMsg.bind(GLAG_Common_CallPlayer			, &CGlobalPacket::handle_Global_Common_CallPlayer		);
	m_gpGL2GCommonSubMsg.bind(GLAG_Common_LocatePlayer			, &CGlobalPacket::handle_Global_Common_LocatePlayer		);
	m_gpGL2GCommonSubMsg.bind(GLAG_Common_LocatePlayerAck		, &CGlobalPacket::handle_Global_Common_LocatePlayerAck	);

	m_gpGL2GFriendSubMsg.bind(GL2G_Friend_Add_Ask_Transfer		, &CGlobalPacket::handle_Global_Friend_Add_Ask_Transfer	);
	m_gpGL2GFriendSubMsg.bind(GL2G_Friend_Answer_Transfer		, &CGlobalPacket::handle_Global_Friend_Answer_Transfer	);
	m_gpGL2GFriendSubMsg.bind(GL2G_Friend_Update				, &CGlobalPacket::handle_Global_Friend_Update			);
	m_gpGL2GFriendSubMsg.bind(GL2G_Friend_Syn					, &CGlobalPacket::handle_Global_Friend_Syn				);
	m_gpGL2GFriendSubMsg.bind(GL2G_Friend_Add					, &CGlobalPacket::handle_Global_Friend_Add				);

	m_gpGL2GWorldBossSubMsg.bind(GL2G_WorldBoss_Status_Ntf		, &CGlobalPacket::handle_Global_WorldBoss_Status_Ntf	);
	m_gpGL2GWorldBossSubMsg.bind(GL2G_WorldBoss_KillerInfo_Ack	, &CGlobalPacket::handle_Global_WorldBoss_KillerInfo_Ack	);

}

//-------------------------------------------------------------
//------------------------------ 
CGlobalPacket::~CGlobalPacket()
{
}

//-------------------------------------------------------------
//------------------------------ 
void CGlobalPacket::initialize()
{
	Parent::initialize();

	m_pConnect		= NULL;

	m_stAdderss.initAddress();
}

//-------------------------------------------------------------
//------------------------------ 断开回调
void	CGlobalPacket::onDisconnect()
{
	CLog::warn("与全局服务器断开");
	g_pServer->unregisterAllGlobal();
}

//-------------------------------------------------------------
//------------------------------ 
bool	CGlobalPacket::send(bool bCheck/* = true*/)
{
	if (!m_pConnect || !Parent::send(bCheck))
		return false;

	return m_pConnect->send(m_szBuffer,getPosition());
}

//-------------------------------------------------------------
//------------------------------ 
void	CGlobalPacket::disconnect()
{
	if (m_pConnect)
		m_pConnect->disconnect();
}

//-------------------------------------------------------------
//------------------------------ 数据包处理
bool	CGlobalPacket::onPacketSrv(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	if (!m_pConnect)
		return false;
	return m_gpHandleMsg.onMessage(pHead, clStream);
}

//-------------------------------------------------------------
//------------------------------ 错误包处理
void	CGlobalPacket::onBadPacket(uint32 uCount,_stPacketHead* pHead)
{
}

//-------------------------------------------------------------
//------------------------------ 
void	CGlobalPacket::onConnect(bool bConnect)
{
	if(bConnect)
	{
		send_Login_Ask();
	}
	else
	{
		CLog::error("全局服务器链接失败:%s:%d",m_stAdderss.getIP(),m_stAdderss.getPort());
	}
}

//-------------------------------------------------------------
//------------------------------ 登录应答
bool	CGlobalPacket::handle_Login_Ack(_stPacketHead &pHead, CNetStream &clStream)
{
	if (pHead.uEventFlag == NetResult_Succeed)
	{
		if(!clStream.read(m_stSvrInfo))
			return false;

		CLog::warn("注册到全局服务器成功!");

	}
	else if(pHead.uEventFlag == NetResult_VersionError)
	{
		CLog::error("注册到全局服务器失败(版本号不一致)!");
	}
	else
	{
		CLog::error("注册到全局服务器失败!");
 	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 登录请求
bool	CGlobalPacket::send_Login_Ask()
{
	build_Login_Ask();
	write(g_pServer->getSvrInfo());

	return send(false);
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGlobalPacket::handle_Global_Start			(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	switch ( pHead.uProtocol )
	{
	case GL2G_Start_Register:
		{
			g_pServer->registerGlobal(pHead.stFromSrvInfo.uLogicWorldID);
			//世界boss 请求信息 
			//g_pWorldBossMgr->tryGetWorldBossKillerInfo();
		}
		break;
	case GL2G_Start_UnRegister:
		{
			g_pServer->unregisterGlobal(pHead.stFromSrvInfo.uLogicWorldID);
		}
		break;
	default:
		break;
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGlobalPacket::handle_Global_ClientMsg			(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	uint32 uPlayerID;
	uint8 uMainProtocol;
	uint8 uProtocol;
	if ( !clStream.read(uPlayerID) || !clStream.read(uMainProtocol) ||  !clStream.read(uProtocol) )
		return false;

	CScenePlayer* pPlayer = g_pGamePlayerMgr->findPlayer(uPlayerID);
	if ( pPlayer )
		pPlayer->sendBuffer(uMainProtocol,uProtocol,clStream.getSpareBuffer(),clStream.getSpareSize(),pHead.uEventFlag);

	return true;
}
//------------------------------------------------------
//------------------------------ 
bool	CGlobalPacket::handle_Global_Trade			(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	return m_gpGL2GTradeSubMsg.onMessage(pHead, clStream);
}
//------------------------------------------------------
//------------------------------ 
bool	CGlobalPacket::handle_Global_SaleMoney		(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	return m_gpGL2GSaleMoneySubMsg.onMessage(pHead, clStream);
}
//------------------------------------------------------
//------------------------------ 
bool	CGlobalPacket::handle_Global_Auction			(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	return m_gpGL2GAuctionSubMsg.onMessage(pHead, clStream);
}
//------------------------------------------------------
//------------------------------ 
bool	CGlobalPacket::handle_Global_Friend			(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	return m_gpGL2GFriendSubMsg.onMessage(pHead, clStream);
}
//------------------------------------------------------
//------------------------------ 
bool	CGlobalPacket::handle_Global_WorldBoss			(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	return m_gpGL2GWorldBossSubMsg.onMessage(pHead, clStream);
}
//------------------------------------------------------
//------------------------------ 
bool	CGlobalPacket::handle_Global_Trade_Begin		(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBG2GLTradePlayer msg;
	if (!clStream.ParseFromStream(msg))
		return false;

	_stPlayerDisplay stTar;
	stTar.loadFromProtobuf(msg.tar());
	//CPlayerTrade::trade_begin(msg.playerid(),stTar);
	return true;
}
//------------------------------------------------------
//------------------------------ 
bool	CGlobalPacket::handle_Global_Trade_End		(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBG2GLTradePlayer msg;
	if (!clStream.ParseFromStream(msg))
		return false;

	//CPlayerTrade::trade_end(msg.playerid(),pHead.uEventFlag);
	return true;
}
//------------------------------------------------------
//------------------------------ 
bool	CGlobalPacket::handle_Global_Trade_check		(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBG2GLTradeInfo msg;
	if (!clStream.ParseFromStream(msg))
		return false;

	uint8 uCode = R_ResultTrade_Fail;
	//CPlayerTrade::trade_check(msg,uCode,true);
	sendMsg(g_pServer->getSvrInfo(), getSvrInfo(), P_G2GL_Protocol_Trade, G2GL_Trade_check, &msg ,uCode);
	return true;
}
//------------------------------------------------------
//------------------------------ 
bool	CGlobalPacket::handle_Global_Trade_delete		(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBG2GLTradeInfo msg;
	if (!clStream.ParseFromStream(msg))
		return false;

	uint8 uCode = R_ResultTrade_Fail;
	//CPlayerTrade::trade_delete(msg,uCode);
	sendMsg(g_pServer->getSvrInfo(), getSvrInfo(), P_G2GL_Protocol_Trade, G2GL_Trade_delete, &msg ,uCode);
	return true;
}
//------------------------------------------------------
//------------------------------ 
bool	CGlobalPacket::handle_Global_Trade_add		(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBG2GLTradeInfo msg;
	if (!clStream.ParseFromStream(msg))
		return false;

	uint8 uCode = R_ResultTrade_Fail;
	//CPlayerTrade::trade_add(msg,uCode);
	return true;
}
//------------------------------------------------------
//------------------------------ 
bool	CGlobalPacket::handle_Global_Trade_Return		(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBG2GLTradeInfo msg;
	if (!clStream.ParseFromStream(msg))
		return false;

	uint8 uCode = R_ResultTrade_Fail;
	//CPlayerTrade::trade_return(msg,uCode);
	return true;
}
//------------------------------------------------------
//------------------------------ 
bool	CGlobalPacket::handle_Global_SaleMoney_sale_Fail(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBGL2GSaleMoneyFail msg;
	if (!clStream.ParseFromStream(msg))
		return false;

	uint8 uCode = R_ResultSaleMoney_Fail;
	//CPlayerSaleMoney::salemoney_sale_fail(msg,uCode);
	return true;
}
//------------------------------------------------------
//------------------------------ 
bool	CGlobalPacket::handle_Global_SaleMoney_salecancel_Suc	(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBGL2GSaleMoneyCancel msg;
	if (!clStream.ParseFromStream(msg))
		return false;

	uint8 uCode = R_ResultSaleMoney_Fail;
	//CPlayerSaleMoney::salemoney_salecancel_suc(msg,uCode);
	return true;
}
//------------------------------------------------------
//------------------------------ 
bool	CGlobalPacket::handle_Global_Auction_sale_Fail	(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBGL2GAuctionSaleFail msg;
	if (!clStream.ParseFromStream(msg))
		return false;

	uint8 uCode = R_ResultSaleMoney_Fail;
	//CPlayerAuction::auction_sale_fail(msg,uCode);
	return true;
}
//------------------------------------------------------
//------------------------------ 
bool	CGlobalPacket::handle_Global_Auction_salecancel_Suc	(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBGL2GAuctionSaleCancel msg;
	if (!clStream.ParseFromStream(msg))
		return false;

	uint8 uCode = R_ResultSaleMoney_Fail;
	//CPlayerAuction::auction_salecancel_suc(msg,uCode);
	return true;
}
//------------------------------------------------------
//------------------------------ 
bool	CGlobalPacket::handle_Global_Auction_buy_Fail	(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBGL2GAuctionBuyFail msg;
	if (!clStream.ParseFromStream(msg))
		return false;

	uint8 uCode = R_ResultSaleMoney_Fail;
	//CPlayerAuction::auction_buy_fail(msg,uCode);
	return true;
}
//------------------------------------------------------
//------------------------------ 
bool	CGlobalPacket::handle_Global_Common			(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	return m_gpGL2GCommonSubMsg.onMessage(pHead, clStream);
}

//------------------------------------------------------
//------------------------------ 
bool	CGlobalPacket::handle_Global_Common_FindPlayer		(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBGLAGFindPlayer msg;
	CHECKF(clStream.ParseFromStream(msg));

	PBGLAGFindPlayerAck ack;
	ack.set_targetid(msg.targetid());
	ack.set_sourceid(msg.sourceid());
	uint8 uEvenID = 0;
	
	CScenePlayer* pPlayer = g_pGamePlayerMgr->findPlayer(msg.targetid());
	if (!pPlayer)
	{
		uEvenID = R_ResultCommon_NotOnline;
	}
	else
	{
		pPlayer->packetView(ack.mutable_pbview(),true);
	}

	return g_pServer->sendMsgToGlobal((uint16)msg.worldid(),P_G2GL_Protocol_Common,GLAG_Common_FindPlayerAck,&ack,uEvenID);
}
//------------------------------------------------------
//------------------------------ 
bool	CGlobalPacket::handle_Global_Common_Mail		(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBMail stMail;
	CHECKF(clStream.ParseFromStream(stMail));

	return CPlayerMail::sendMail(stMail);
}
//------------------------------------------------------
//------------------------------ 
bool	CGlobalPacket::handle_Global_Friend_Add_Ask_Transfer	(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBG2GLFriend_AddAsk msg;
	if (!clStream.ParseFromStream(msg))
		return false;

	//CPlayerFriend::on_global_addask_transfer(msg);
	return true;
}
//------------------------------------------------------
//------------------------------ 
bool	CGlobalPacket::handle_Global_Friend_Answer_Transfer	(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBG2GLFriend_Answer msg;
	if (!clStream.ParseFromStream(msg))
		return false;

	//CPlayerFriend::on_global_answer_transfer(msg);
	return true;
}
//------------------------------------------------------
//------------------------------ 
bool	CGlobalPacket::handle_Global_Friend_Update				(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBGL2GFriendInfoUpdate msg;
	if (!clStream.ParseFromStream(msg))
		return false;

	//CPlayerFriend::on_global_update(msg);
	return true;
}
//------------------------------------------------------
//------------------------------ 
bool	CGlobalPacket::handle_Global_Friend_Add				(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBGL2GFriendAdd msg;
	if (!clStream.ParseFromStream(msg))
		return false;

	//CPlayerFriend::on_global_add(msg);
	return true;
}
//------------------------------------------------------
//------------------------------ 
bool	CGlobalPacket::handle_Global_Friend_Syn				(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBGL2GFriendInfoSyn msg;
	if (!clStream.ParseFromStream(msg))
		return false;

	//CPlayerFriend::on_global_syn(msg);
	return true;
}

//------------------------------------------------------
//------------------------------ 
bool	CGlobalPacket::handle_Global_Common_CallPlayer		(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBGLAGCallPlayer msg;
	CHECKF(clStream.ParseFromStream(msg));

	CScenePlayer* pTarget = g_pGamePlayerMgr->findPlayer(msg.tarid());
	if (pTarget)
	{
		pTarget->playerFlyScene_ByDynID(msg.sceneid(),msg.scenedynid(),msg.posx(),msg.posy());
	}

	return true;
}

//------------------------------------------------------
//------------------------------ 
bool	CGlobalPacket::handle_Global_Common_LocatePlayer		(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBGLAGLocatePlayer msg;
	CHECKF(clStream.ParseFromStream(msg));

	CScenePlayer* pTarget = g_pGamePlayerMgr->findPlayer(msg.tarid());
	if (pTarget)
	{
		PBGLAGLocatePlayerAck ack;
		ack.set_playerid(msg.playerid());
		ack.set_reason(msg.reason());
		PBPlayerDisplay* pbDisplay = ack.mutable_tar();
		if (pbDisplay)
		{
			pTarget->getPlayerBase().saveToProtobuf(*pbDisplay);
		}
		ack.set_suc(true);
		ack.set_sceneid(pTarget->getSceneID());
		ack.set_scenecellid(pTarget->getSceneCellID());
		ack.set_scenedynid(pTarget->getSceneDynID());
		ack.set_posx(pTarget->getPosition().x);
		ack.set_posy(pTarget->getPosition().y);

		return pTarget->sendMsgToGlobal(P_G2GL_Protocol_Common,GLAG_Common_LocatePlayerAck,&ack);
	}

	return true;
}
//------------------------------------------------------
//------------------------------ 
bool	CGlobalPacket::handle_Global_Common_LocatePlayerAck		(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBGLAGLocatePlayerAck msg;
	CHECKF(clStream.ParseFromStream(msg));

	CScenePlayer* pPlayer = g_pGamePlayerMgr->findPlayer(msg.playerid());
	if (pPlayer)
	{
		switch (msg.reason() )
		{
		case LocatePlayerReason_GMQuery:
			{
				CPlayerCommon* pCommon = pPlayer->getPlayerCommon();
				if (!pCommon)
					return true;

				//pCommon->getClientGMCommand().on_find_result(msg);
			}
			break;
		case LocatePlayerReason_GMQueryGOTO:
			{
				CPlayerCommon* pCommon = pPlayer->getPlayerCommon();
				if (!pCommon)
					return true;

				//pCommon->getClientGMCommand().on_go_result(msg);
			}
			break;
		default:
			break;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
//世界boss系统
bool	CGlobalPacket::handle_Global_WorldBoss_Status_Ntf		(_stPacketHead_Svr &pHead, CNetStream &clStream)
{
	PBGL2GWorldBossStatusNtf msg;
	CHECKF(clStream.ParseFromStream(msg));

	//g_pWorldBossMgr->onWorldBossStatusNtf(msg);
	return true;
}
bool	CGlobalPacket::handle_Global_WorldBoss_KillerInfo_Ack	(_stPacketHead_Svr &pHead, CNetStream &clStream)
{

	PBGL2GWorldBossKillerInfoAck msg;
	CHECKF(clStream.ParseFromStream(msg));

	//g_pWorldBossMgr->onWorldBossKillerInfo(msg);
	return true;
}