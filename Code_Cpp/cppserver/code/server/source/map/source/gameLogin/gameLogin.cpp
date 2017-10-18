/*------------- gameLogin.cpp
* Copyright (C): 2011  LiuLei
* Author       : 赵文源
* Version      : V1.01
* Date         : 2011/3/24 17:42:17
*
*/ 
/***************************************************************
* 
***************************************************************/
#include "gameLogin/gameLogin.h"
#include "main/gameServer.h"
#include "gameLogin/gamePlayerMgr.h"
#include "log/log.h"

#include "protocol/MsgResult.pb.h"
/*************************************************************/
CGameLogin* g_pGameLogin = NULL;
//-------------------------------------------------------------
//------------------------------ 
CGameLogin::CGameLogin()
{
	m_mapLogin.clear();
	m_uOvertime	= 60*5;
}

//-------------------------------------------------------------
//------------------------------ 
CGameLogin::~CGameLogin()
{
}

//-------------------------------------------------------------
//------------------------------ 
bool	CGameLogin::enter(CLoginObj* pObject)
{
	if (!pObject || !pObject->bLoad || !pObject->uLoginSN || !pObject->beAllReady() )
		return false;

	pObject->setGLoginState(_GLS_PlayerCreate);
	if(!g_pGamePlayerMgr->onlinePlayer(*pObject,pObject->pPacket,pObject->uToSceneID,pObject->uToSceneDynID,pObject->bLogin))
	{
		if(pObject->pPacket)
		{
			pObject->pPacket->send_Login_Ack(NetResult_Fail);
			pObject->pPacket->send_Disconnection();
		}
	}

	m_mapLogin.release(pObject->uPlayerID);
	return true;
}

//-------------------------------------------------------------
//------------------------------ 取消登录
bool	CGameLogin::cancel(CLoginObj* pObject)
{
	if (!pObject)
		return false;

	if(pObject->pPacket)
		pObject->pPacket->send_Login_Ack(R_Login_Timeout);
	pObject->setGLoginState(_GLS_LoginCancle);
	m_mapLogin.release(pObject->uPlayerID);
	return true;
}

//-------------------------------------------------------------
//------------------------------ 踢出线上玩家
bool	CGameLogin::kickPlayer(uint32 uPlayerID,bool bSend)
{
	CScenePlayer* pPlayer = g_pGamePlayerMgr->find(uPlayerID);
	if(pPlayer)
	{
		pPlayer->SetGLoginState(_GLS_PlayerKickOut);
		if(pPlayer->m_pPacket)
		{
			
			CLog::warn("CGameLogin::KickPlayer(%d)PlayerName[%s]一个在线玩家[%s]", \
				uPlayerID,pPlayer->getPlayerBase().szPlayerName, bSend ? "保存角色数据" : "不保存角色数据" );
			pPlayer->m_pPacket->send_Kick_Notify(R_Login_Kickout);
		}
		pPlayer->logout();
		return true;
	}   

	return false;
}

//-------------------------------------------------------------
//------------------------------ 踢出制定玩家
bool	CGameLogin::kick(uint32 uPlayerID)
{
	//取消登录
	bool	bKick = cancel(findLogin(uPlayerID));
	//踢除在线角色
	bKick	|= kickPlayer(uPlayerID);

	return bKick;
}
//-------------------------------------------------------------
//------------------------------ 客户端断线重连
void	CGameLogin::reconnect(uint32 uPlayerID,uint64 uLoginSN,CClientPacket* pPacket)
{
	if (!pPacket)
		return;

	CScenePlayer* pPlayer = g_pGamePlayerMgr->find(uPlayerID);
	if (!pPlayer)
		return;

	if(pPlayer->m_pPacket|| pPlayer->getLoginSN() != uLoginSN)
	{
		pPacket->send_Login_Ack(NetResult_Fail);
		pPacket->send_Disconnection();

		CLog::error("用户断线重练数据不对[%u][%llu]!...",uPlayerID,uLoginSN);
		return;
	}

	pPlayer->addState(StateType_WaitReady);
	pPlayer->setOfflineTime(0);
	g_pGamePlayerMgr->setPlayerPacket(pPlayer,pPacket);

	CLog::warn("断线重连成功:%u，%llu",uPlayerID,uLoginSN);
}

//-------------------------------------------------------------
//------------------------------ 客户端登录
bool	CGameLogin::login(uint32 uPlayerID,uint64 uLoginSN,CClientPacket* pPacket)
{
	if (!uPlayerID || !uLoginSN || !pPacket)
		return false;

	CLoginObj*pLogin = findLogin(uPlayerID);
	if (pLogin)
	{/*世界服务器的登录角色消息先来到的情况*/ 
		/*自己链接*/ 
		pLogin->setGLoginState(_GLS_LoginAsk);
		CLog::error("客户端登录，已有玩家在线[%u],oldLoginSN[%lld],newLoginsn[%lld]",uPlayerID,pLogin->uLoginSN,uLoginSN);
		if (pLogin->pPacket == pPacket)
		{
			CLog::warn("同一连接用户重复登录!...");
			return true;
		}

		/*自己是非法角色或后链接者*/ 
		if(pLogin->uLoginSN != uLoginSN || pLogin->pPacket)
		{
			pPacket->send_Login_Ack(NetResult_Fail);
			pPacket->send_Disconnection();

			CLog::error("用户登录验证已有存在UID不同或已连接踢出自己(%lld != %lld)(%p)!...",pLogin->uLoginSN,uLoginSN,pLogin->pPacket);
			return false;
		}
		pLogin->pPacket	= pPacket;
	}
	
	if(!pLogin)
	{
		//玩家是否已存在
		CScenePlayer* pPlayer = g_pGamePlayerMgr->find(uPlayerID);
		if(pPlayer)
		{
			if(pPlayer->m_pPacket || pPlayer->getLoginSN() != uLoginSN)
			{
				pPacket->send_Login_Ack(NetResult_Fail);
				pPacket->send_Disconnection();

				CLog::error("用户登录验证已有玩家是否已存在[%u][%llu]!...",uPlayerID,uLoginSN);
				return false;
			}

			g_pGamePlayerMgr->setPlayerPacket(pPlayer,pPacket);
			CLog::warn("后设置Packet玩家:%u",uPlayerID);
			return true;
		}

		pLogin = m_mapLogin.allocate(uPlayerID);
		if(!pLogin)
			return false;
		pLogin->uPlayerID	= uPlayerID;
		pLogin->uLoginSN	= uLoginSN;
		pLogin->pPacket		= pPacket;
		pLogin->uTime		= (uint64)getTime() + m_uOvertime;
		pLogin->setGLoginState(_GLS_LoginAsk);
		return true;
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------ 客户端断开
void	CGameLogin::logout		(uint32 uPlayerID,CClientPacket* pPacket)
{
	CLoginObj*pLogin = findLogin(uPlayerID);
	if(!pLogin)
		return;

	if (pLogin->pPacket != pPacket)
		return;

	pLogin->pPacket		= NULL;
	if(!pLogin->bLoad)
		m_mapLogin.release(uPlayerID);
}

//-------------------------------------------------------------
//------------------------------ 服务器登录
bool	CGameLogin::login(PBAccount& stAccount,uint32 uPlayerID,uint16 uSceneID,uint64 uToSceneDynID,bool bTransfer)
{
	kickPlayer(uPlayerID,false);

	CLoginObj*pLogin = findLogin(uPlayerID);
	if (pLogin)
	{
		pLogin->setGLoginState(_GLS_WorldLoginAsk);
		//CLog::error("世界服登录，已有玩家在线[%u],oldLoginSN[%u],newLoginsn[%u]",uPlayerID,pLogin->uLoginSN,stAccount.accountflag().loginsn());
		if (pLogin->uLoginSN != stAccount.accountflag().loginsn() )
		{	/*登录失败*/ 
			if(pLogin->pPacket)
				pLogin->pPacket->send_Login_Ack(NetResult_Fail);

			pLogin->pPacket= NULL;
		}
	}
	if(!pLogin)
	{
		pLogin = m_mapLogin.allocate(uPlayerID);
		if(!pLogin)
			return false;
		pLogin->uPlayerID	= uPlayerID;
		pLogin->uLoginSN	= stAccount.accountflag().loginsn();
	}
	pLogin->uToSceneID		= uSceneID;
	pLogin->uToSceneDynID	= uToSceneDynID;
	pLogin->bLoad			= true;
	pLogin->bLogin			= !bTransfer;
	pLogin->uTime			= (uint64)getTime() + m_uOvertime;
	pLogin->setDataFlag(_PInfo_Account,true);
	pLogin->mutable_accountinfo()->CopyFrom(stAccount);
	pLogin->setGLoginState(_GLS_WorldLoginAsk);
	return true;
}

//-------------------------------------------------------------
//------------------------------ 服务器更新包裹
bool	CGameLogin::update		(uint32 uPlayerID,uint8 uType,CNetStream& clStream)
{
	if (!uPlayerID)
		return false;

	CLoginObj*pLogin = findLogin(uPlayerID);
	if (!pLogin)
		return false;
	pLogin->setGLoginState(_GLS_Load);
	switch (uType)
	{
	case _PInfo_Base:
		{
			if (!clStream.ParseFromStream(*pLogin->mutable_playerbase()))
				return false;
		}
		break;
	case _PInfo_System:
		{
			if (!clStream.ParseFromStream(*pLogin->mutable_playersystem()))
				return false;
		}
		break;
	case _PInfo_Bag:
		{
			if (!clStream.ParseFromStream(*pLogin->mutable_playerbag()))
				return false;
		}
		break;
	case _PInfo_Client:
		{
			if (!clStream.ParseFromStream(*pLogin->mutable_clientdata()))
				return false;
		}
		break;
	case _PInfo_Quest:
		{
			if (!clStream.ParseFromStream(*pLogin->mutable_playerquest()))
				return false;
		}
		break;
	case _PInfo_Mail:
		{
			if (!clStream.ParseFromStream(*pLogin->mutable_playermail()))
				return false;
		}
		break;
	case _PInfo_Friend:
		{
			if (!clStream.ParseFromStream(*pLogin->mutable_playerfriend()))
				return false;
		}
		break;
	case _PInfo_Faction:
		{
			if (!clStream.ParseFromStream(*pLogin->mutable_playerfaction()))
				return false;
		}
		break;
	default:
		return false;
	}
	pLogin->setDataFlag(uType,true);

	/*进入游戏*/ 
	if (pLogin->beAllReady())
		enter(pLogin);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 登录超时处理
void	CGameLogin::timerProcess()
{
	uint64 uTime = (uint64)getTime();

	iterator	pos;
	CLoginObj*	lpLogin = NULL;
	m_mapLogin.getHead(pos);
	while (!m_mapLogin.isTail(pos))
	{
		lpLogin	= m_mapLogin.getNext(pos);
		if (lpLogin && lpLogin->uTime > uTime)
			continue;

		cancel(lpLogin);
	}
}
