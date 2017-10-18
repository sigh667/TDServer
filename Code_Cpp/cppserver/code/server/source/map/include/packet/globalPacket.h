/*------------- chatPacket.h
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
#include "protocol/GLAGProtocol.pb.h"
#include "common/netMsgHandler.h"
/*************************************************************/
class CNetConnect;
class CBufferStream;
/*************************************************************/
class CGlobalPacket	: public CNetPacketSrv
{
private:
	typedef CNetPacket	Parent;

private:
	CNetConnect*	m_pConnect;
	CNetMsgHandler<CGlobalPacket>	m_gpHandleMsg;

	CNetSubMsgHandler<CGlobalPacket>	m_gpGL2GTradeSubMsg;
	CNetSubMsgHandler<CGlobalPacket>	m_gpGL2GSaleMoneySubMsg;
	CNetSubMsgHandler<CGlobalPacket>	m_gpGL2GAuctionSubMsg;
	CNetSubMsgHandler<CGlobalPacket>	m_gpGL2GCommonSubMsg;
	CNetSubMsgHandler<CGlobalPacket>	m_gpGL2GFriendSubMsg;
	CNetSubMsgHandler<CGlobalPacket>	m_gpGL2GWorldBossSubMsg;

private:
	_stNetAddress	m_stAdderss;

public:
	explicit CGlobalPacket();
	virtual ~CGlobalPacket();

public:
	inline void		bindConnect		(CNetConnect* pConnect)	{	m_pConnect = pConnect;	}

public:
	virtual void	initialize		();
	/*----->{ 断开回调 }*/ 
	virtual void	onDisconnect	();	

private:
	virtual bool	send			(bool bCheck = true);
	/*----->{ 断开 }*/ 
	virtual void	disconnect		();
	/*----->{ 数据包处理 }*/ 
	virtual bool	onPacketSrv		(_stPacketHead_Svr &pHead, CNetStream &clStream);
	/*----->{ 错误包处理 }*/
	virtual void	onBadPacket		(uint32 uCount,_stPacketHead* pHead);
	/*--->[ 链接 ]*/
	virtual void	onConnect		(bool bConnect);

private:
	/*--->[ 登录应答 ]*/
	bool	handle_Login_Ack		(_stPacketHead &pHead, CNetStream &clStream);
	/*--->[ 登录请求 ]*/
	bool	send_Login_Ask			();

private:
	bool	handle_Global_Start						(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_Global_ClientMsg					(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_Global_Trade						(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_Global_SaleMoney					(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_Global_Auction					(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_Global_Common					(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_Global_GameMsg					(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_Global_Friend					(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_Global_WorldBoss					(_stPacketHead_Svr &pHead, CNetStream &clStream);

	//////////////////////////////////////////////////////////////////////////
	//交易
	bool	handle_Global_Trade_Begin				(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_Global_Trade_End					(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_Global_Trade_check				(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_Global_Trade_delete				(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_Global_Trade_add					(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_Global_Trade_Return				(_stPacketHead_Svr &pHead, CNetStream &clStream);
	//////////////////////////////////////////////////////////////////////////
	//元宝寄售
	bool	handle_Global_SaleMoney_sale_Fail		(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_Global_SaleMoney_salecancel_Suc	(_stPacketHead_Svr &pHead, CNetStream &clStream);
	//////////////////////////////////////////////////////////////////////////
	//拍卖
	bool	handle_Global_Auction_sale_Fail			(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_Global_Auction_salecancel_Suc	(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_Global_Auction_buy_Fail			(_stPacketHead_Svr &pHead, CNetStream &clStream);
	//////////////////////////////////////////////////////////////////////////
	//通用
	bool	handle_Global_Common_FindPlayer			(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_Global_Common_Mail				(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_Global_Common_CallPlayer			(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_Global_Common_LocatePlayer		(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_Global_Common_LocatePlayerAck	(_stPacketHead_Svr &pHead, CNetStream &clStream);
	//////////////////////////////////////////////////////////////////////////
	//好友系统
	bool	handle_Global_Friend_Add_Ask_Transfer	(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_Global_Friend_Answer_Transfer	(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_Global_Friend_Update				(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_Global_Friend_Syn				(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_Global_Friend_Add				(_stPacketHead_Svr &pHead, CNetStream &clStream);
	//////////////////////////////////////////////////////////////////////////
	//世界boss系统
	bool	handle_Global_WorldBoss_Status_Ntf		(_stPacketHead_Svr &pHead, CNetStream &clStream);
	bool	handle_Global_WorldBoss_KillerInfo_Ack	(_stPacketHead_Svr &pHead, CNetStream &clStream);


};
/*************************************************************/
