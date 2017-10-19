/*------------- clientPacketMgr.cpp
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.01
* Date         : 2011/2/24 9:49:23
*
*/ 
/***************************************************************
* 
***************************************************************/
#include "basic/stringFunctions.h"
#include "log/log.h"
//------------------------------ 
//------------------------------ 
#include "gameLogin/gameLogin.h"
#include "packet/clientPacket.h"
#include "main/gameServer.h"
#include "gameLogic/clientGMCommand/clientGMCommand.h"
#include "gameLogin/gamePlayerMgr.h"
#include "protocol/CASMsg.pb.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 

CClientPacket::CClientPacket()
{
	initialize();

	memset(m_gpHandleMsg,0,sizeof(m_gpHandleMsg));
}

//-------------------------------------------------------------
//------------------------------ 
CClientPacket::~CClientPacket()
{
}

//-------------------------------------------------------------
//------------------------------ 
void	CClientPacket::initialize()
{
	Parent::initialize();

	m_uDecodeType		= 1;
#ifdef _DEBUG
	m_uTimeoutReadEvent	= _TE_Normal;
#else
	m_uTimeoutReadEvent	= _TE_Close;
#endif
	m_uTimeoutWriteEvent= _TE_Normal;
	m_bPacketFlowSwitch = false;

	m_pContext			= NULL;

	m_uPlayerID			= 0;
	m_uLoginSN			= 0;
	m_pGamePlayer		= NULL;

	setPacketMaxNum(60);

}

//-------------------------------------------------------------
//------------------------------ 
void	CClientPacket::disconnect()
{
	if(!m_pContext)
		return;

	m_pContext->disconnect();
}

//-------------------------------------------------------------
//------------------------------ 
void	CClientPacket::onDisconnect()
{
	g_pGameLogin->logout(m_uPlayerID,this);
	if(m_pGamePlayer)
	{
		m_pGamePlayer->setClientPacket(NULL);
		m_pGamePlayer->setOfflineTime((uint64)getTime());
	}

	CLog::error("m_uPlayerID=%d, onDisconnect", m_uPlayerID);
	initialize();
}

//-------------------------------------------------------------
//------------------------------ 
void	CClientPacket::onKickOut()
{
	Parent::send_Kick_Notify();
}
//-------------------------------------------------------------
//------------------------------ 
pc_str	CClientPacket::remote_ip	()
{
	if(m_pContext)
		return m_pContext->remote_ip();

	return "";
}

//-------------------------------------------------------------
//------------------------------ 
ulong	CClientPacket::remote_address()
{
	if (m_pContext)
		return m_pContext->remote_address();

	return Parent::remote_address();
}

//-------------------------------------------------------------
//------------------------------ 
bool	CClientPacket::send(bool bCheck)
{
	if (!m_pContext || !m_pContext->can_send() || !Parent::send(bCheck))
		return false;

	if(m_pContext->send(m_szBuffer,getPosition()))
		return true;

	CLog::error("CClientPacket::send(%u)[%s]<%x>",getPosition(),remote_ip(),this);
	return false;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CClientPacket::onPacketClient(_stPacketHead_Client &pHead, CNetStream &clStream)
{
	FUNCTION_BEGIN_CHECK();
	if (!m_pContext || !m_pGamePlayer)
	{
		CLog::error("onPacketClient  failed m_pContext=%u|m_pGamePlayer=%u",m_pContext, m_pGamePlayer);
		return false;
	}
	if (pHead.uMainProtocol >= _emPC2S_Protocol_ARRAYSIZE)
	{
		CLog::error("onPacketClient failed pHead.uMainProtocol[%d] >= _emPC2S_Protocol_ARRAYSIZE[%d]", pHead.uMainProtocol, _emPC2S_Protocol_ARRAYSIZE);
		return false;
	}
	bool bResult = false;
	if (m_gpHandleMsg[pHead.uMainProtocol])
	{
		bResult = (this->*m_gpHandleMsg[pHead.uMainProtocol])(pHead.uProtocol,pHead.uEventFlag,clStream);
	}
	else
	{
		if (!m_pGamePlayer)
		{
			CLog::error("客户端请求消息非法,未经过登录验证![%s]",m_pContext ? m_pContext->remote_ip() : "");
			return false;
		}
		
		int32 begin = getTickCount();
		bResult = m_pGamePlayer->functionNetPacket(pHead.uMainProtocol,pHead.uProtocol,pHead.uEventFlag,clStream);
		int32 end = getTickCount();
		if (end - begin >= 100)
		{
			// 耗时超过100毫秒的打印
			CLog::error("cost too much time.[%d][%d]=%dms", pHead.uMainProtocol, pHead.uProtocol, end - begin);
		}
	}

	if(!bResult)
 		CLog::error("消息处理协议未定义或非法 mainid:%d,subid:%d",pHead.uMainProtocol,pHead.uProtocol);

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
void	CClientPacket::onBadPacket(uint32 uCount,_stPacketHead* pHead)
{
	CLog::error("CClientPacket::OnBadPacket(%u)[%s]([%u][%u])",uCount,m_pContext ? m_pContext->remote_ip() : "",pHead ? pHead->uMainProtocol : 0,pHead ? pHead->uProtocol : 0);
}

//-------------------------------------------------------------
//------------------------------ 设置压缩
void	CClientPacket::onSetCompress		()
{
	if(m_pContext)
	{
		m_pContext->setCompress(m_uCompress);
		m_pContext->setCompressMin(200);
	}
}

//-------------------------------------------------------------
//------------------------------ 
bool	CClientPacket::handle_Login_Ask(_stPacketHead &, CNetStream &clStream)
{
 	PBC2GLoginAsk ask;
 	if(!clStream.ParseFromStream(ask))
 		return false;
 	
	//是否断线重连
	if (ask.breconnet())
	{
		g_pGameLogin->reconnect(ask.playerid(),ask.loginsn(),this);
		return true;
	}


	/*在登录表中效验合法性,全部交由验证列表处理*/ 
 	if (g_pGameLogin->login(ask.playerid(),ask.loginsn(),this))
 	{
 		m_uPlayerID	= ask.playerid();
 		m_uLoginSN	= ask.loginsn();
 	}

	return true;
}
//------------------------------------------------------
//------------------------------ 处理断开
bool	CClientPacket::handle_Disconnect(_stPacketHead &/*pHead*/,CNetStream &/*clStream*/)
{
	disconnect();
	CLog::warn("player disconnect.");
	if (m_pGamePlayer)
		m_pGamePlayer->logout();
	return true;
}
//-------------------------------------------------------------
//------------------------------ 发送用户登录返回
bool	CClientPacket::send_Login_Ack(uint8 uCode,PBG2CLoginAck* pAck)
{
	build_Login_Ack(uCode);
	if ( pAck )
		SerializeToStream(*pAck);

	return send(false);
}
//-------------------------------------------------------------
//------------------------------ 
bool	CClientPacket::send_Login_Player		(uint8 uType,void* pData,uint32 uSize)
{
// 	buildPacketHead(P_S2C_Login,uType);
// 	write(uint8(S2C_Login_Player));
// 	write(uSize,pData);

	return send();
}

//-------------------------------------------------------------
//------------------------------ 发送游戏配置
bool	CClientPacket::send_Login_GameCfg	()
{
// 	buildPacketHead(P_S2C_Login);
// 	write(uint8(S2C_Login_GameCfg));
// 	write(g_pServer->m_stGameCfg);

	return send();
}
