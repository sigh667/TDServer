/*------------- clientPacketMgr.h
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.01
* Date         : 2011/2/24 9:49:38
*
*/ 
/***************************************************************
* 对角色角色服务器处理
***************************************************************/
#pragma once
#include "network/netPacketClient.h"
#include "protocol/CASProtocol.pb.h"
#include "protocol/CASMsg.pb.h"
/*************************************************************/

class tcp_context;
class CScenePlayer;
class CClientPacket	: public CNetPacketClient
{
private:
	typedef CNetPacketClient	Parent;

private:
	typedef bool (CNetPacket::*_cmdMsg)	(uint8 uProtocol,uint8 uEventID,CBufferStream& clStream);

private:
	tcp_context*	m_pContext;
	_cmdMsg			m_gpHandleMsg[_emPC2S_Protocol_ARRAYSIZE];

private:
	uint32			m_uPlayerID;
	uint64			m_uLoginSN;
	CScenePlayer*	m_pGamePlayer;

public:
	CClientPacket();
	~CClientPacket();

public:
	inline void			bindContext		(tcp_context* pContext)	{	m_pContext	= pContext;	}
	inline void			bindPlayer		(CScenePlayer* pPlayer)	{	m_pGamePlayer= pPlayer;	}
	inline CScenePlayer*getBindPlayer	()						{	return m_pGamePlayer;	}

public:
	virtual void	initialize			();
	virtual void	disconnect			();
	virtual void	onDisconnect		();
	virtual void	onKickOut			();
	virtual pc_str	remote_ip			();
	virtual ulong	remote_address		();

private:
	virtual bool	send				(bool bCheck = true);
	virtual bool	onPacketClient		(_stPacketHead_Client &pHead, CNetStream &clStream);
	virtual void	onBadPacket			(uint32 uCount,_stPacketHead* pHead);
	//--- 设置压缩
	virtual void	onSetCompress		();

private:
	/*----->{ 登录请求 }*/ 
	bool	handle_Login_Ask			(_stPacketHead &pHead, CNetStream &clStream);
	/*----->{ 主动断开请求 }*/ 
	bool	handle_Disconnect			(_stPacketHead &pHead, CNetStream &clStream);
public:
	/*----->{ 登录返回 }*/ 
	bool	send_Login_Ack				(uint8 uCode,PBG2CLoginAck* pAck= NULL);
public:
	bool	send_Login_Player			(uint8 uType,void* pData,uint32 uSize);
	bool	send_Login_GameCfg			();
};
