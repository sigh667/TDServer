/*------------- worldPacket.h
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.01
* Date         : 2011/2/24 9:49:14
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "network/netPacketSrv.h"
#include "protocol/WAGMsg.pb.h"
#include "protocol/WAGProtocol.pb.h"
#include "common/netMsgHandler.h"
/*************************************************************/
class CNetConnect;
class CWorldPacket	: public CNetPacketSrv
{
private:
	typedef CNetPacketSrv	Parent;

private:
	CNetConnect*						m_pConnect;
	CNetMsgHandler<CWorldPacket>		m_gpHandleMsg;

	CNetSubMsgHandler<CWorldPacket>		m_gpHandlePlayerSubMsg;
	CNetSubMsgHandler<CWorldPacket>		m_gpHandleSceneSubMsg;
	CNetSubMsgHandler<CWorldPacket>		m_gpHandleCopymapSubMsg;
	CNetSubMsgHandler<CWorldPacket>		m_gpHandleCommonSubMsg;
	CNetPlayerSubMsgHandler<CWorldPacket>	m_gpHandleFactionSubMsg;
	CNetSubMsgHandler<CWorldPacket>		m_gpHandleDartCarSubMsg;
	CNetPlayerSubMsgHandler<CWorldPacket>	m_gpHandleQuestionSubMsg;
	CNetSubMsgHandler<CWorldPacket>		m_gpHandleManorMsg;
public:
	explicit CWorldPacket();
	virtual ~CWorldPacket();

public:
	inline void		bindConnect			(CNetConnect* pConnect)	{	m_pConnect = pConnect;	}

public:
	virtual void	initialize			();
	/*----->{ 断开回调 }*/ 
	virtual void	onDisconnect		();	

private:
	virtual bool	send				(bool bCheck = true);
	/*----->{ 断开 }*/ 
	virtual void	disconnect			();
	/*----->{ 数据包处理 }*/ 
	virtual bool	onPacketSrv			(_stPacketHead_Svr &pHead, CNetStream &clStream);
	/*----->{ 错误包处理 }*/ 
	virtual void	onBadPacket			(uint32 uCount,_stPacketHead* pHead);
	/*--->[ 链接 ]*/
	virtual void	onConnect			(bool bConnect);

private:
	/*--->[ 登录应答 ]*/
	virtual bool	handle_Login_Ack	(_stPacketHead &pHead, CNetStream &clStream);

public:
	/*--->[ 登录请求 ]*/
	bool	send_Login_Ask				();
 
	bool	send_Register_Ask			(_stSvrInfo& stWorldInfo);

private:
	bool	handle_World_Player				(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_World_Start				(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_World_Scene				(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_World_Team				(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_World_Faction			(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_World_DartCar			(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_World_Question			(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_World_PvpWar				(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_World_Corps				(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_World_Manor				(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_World_SinglePvp			(_stPacketHead_Svr &pHead, CNetStream &clStream);

private:
	bool	handle_World_Player_Load			(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_World_Player_SaveDone		(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_World_Player_FlyMapAck		(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_World_Player_KickOutAsk		(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_World_Player_OfflineDataAdd	(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_World_Player_OfflineDataLoad	(_stPacketHead_Svr &pHead, CNetStream &clStream);

	bool	handle_World_Scene_CreateCell	(_stPacketHead_Svr &pHead, CNetStream &clStream);

	bool	handle_World_Copymap			(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_World_Copymap_Enter		(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_World_Copymap_Exit		(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_World_Copymap_PlayerInfo	(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_World_Copymap_KickPlayer	(_stPacketHead_Svr &pHead, CNetStream &clStream);

	bool	handle_World_Faction_Create					(_stPacketHead_Svr &pHead, CNetStream& clStream, uint32 playerid);
	bool	handle_World_Faction_Syn					(_stPacketHead_Svr &pHead, CNetStream& clStream, uint32 playerid);
	bool	handle_World_Faction_UpgradeSelfSkill		(_stPacketHead_Svr &pHead, CNetStream& clStream, uint32 playerid);
	bool	handle_World_Faction_DonateMoney			(_stPacketHead_Svr &pHead, CNetStream& clStream, uint32 playerid);
	bool	handle_World_Faction_UpdateSelfContribution	(_stPacketHead_Svr &pHead, CNetStream& clStream, uint32 playerid);

	bool	handle_World_AnswerQuestion_Result		(_stPacketHead_Svr &pHead, CNetStream& clStream, uint32 playerid);

	bool	handle_World_Common					(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_World_Common_GMReloadData	(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_World_Common_GMWorldStartTime(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_World_Common_Mail			(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_World_Common_MsgTransfer		(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_World_Common_TreasurePrize	(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_World_Common_ForbitSpeak		(_stPacketHead_Svr &pHead, CNetStream &clStream);
	
	bool	handle_World_DartCar_Fly			(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_World_DartCar_Fly_Create		(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_World_DartCar_RobFinished	(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_World_DartCar_MemberLeave	(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_World_DartCar_Find			(_stPacketHead_Svr &pHead, CNetStream &clStream);
		
	bool	handle_World_Broadcast				(_stPacketHead_Svr &pHead, CNetStream &clStream);

	bool	handle_World_Manor_Battlefield_Ntf	(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_World_Manor_Copymap_Ntf		(_stPacketHead_Svr &pHead, CNetStream &clStream);


};
/*************************************************************/
