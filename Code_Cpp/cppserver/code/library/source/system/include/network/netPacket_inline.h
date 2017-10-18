/*----------------- netPacket_inline.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/11/8 10:30:36
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 设置每秒最大包数量
inline void		CNetPacket::setPacketMaxNum	(uint32 uNum)
{
	m_uPacketCountMax = uNum * Check_PacketCount_Time;
}
//-------------------------------------------------------------
//------------------------------ 
inline ulong	CNetPacket::getTeleNetAddress	()
{
	return m_uTeleNetAddress;
}
//-------------------------------------------------------------
//------------------------------ 
inline void		CNetPacket::setConnectCheck	(uint8 uTpye)
{
	m_uConnectCheck = uTpye;
}

//-------------------------------------------------------------
//------------------------------ 
inline uint32	CNetPacket::get_apply_key	()const
{
	return m_apply_key;
}
//-------------------------------------------------------------
//------------------------------ 
inline CMutex_*	CNetPacket::get_apply_mutex	()
{
	return &m_apply_mutex;
}
//-------------------------------------------------------------
//------------------------------ 
inline void	CNetPacket::set_apply_lock	(bool _lock)
{
	if(_lock)
		m_apply_mutex.lock();
	else
		m_apply_mutex.unlock();
}

//-------------------------------------------------------------
//------------------------------ 构建带结构类型包头
inline char*CNetPacket::buildPacketHeads(uint8 uMainProtocol,uint8 uProtocol,uint8 uEventFlag/* = 0*/)
{
	Parent::initPosition();
	_stPacketHead *pHead	= (_stPacketHead*)m_szBuffer;
	pHead->uMainProtocol		= uMainProtocol;
	pHead->uProtocol			= uProtocol;
	pHead->uEventFlag		= uEventFlag;

	Parent::skipPosition(sizeof(_stPacketHead));

	return Parent::getSpareBuffer();
}

//-------------------------------------------------------------
//------------------------------ 设置包事件
inline void	CNetPacket::setPacketEvent(uint8 uEvent)
{
	_stPacketHead *pHead	= (_stPacketHead*)m_szBuffer;
	pHead->uEventFlag		= uEvent;
}

//-------------------------------------------------------------
//------------------------------ 
inline char*CNetPacket::build_Login_Ask(uint8 uEventFlag/* = 0*/)
{
	return buildPacketHeads(P_Operate,P_SO_Login_Ask,uEventFlag);
}

//-------------------------------------------------------------
//------------------------------ 
inline char*CNetPacket::build_Login_Ack(uint8 uEventFlag/* = 0*/)
{
 	return buildPacketHeads(P_Operate,P_SO_Login_Ack,uEventFlag);
}
