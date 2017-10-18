/*----------------- playerBuff.cpp
*
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ³ÂÁÁ
* Version      : V1.0
* Date         : 2016/1/26 15:25:49
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "gameLogic/playerBuff/playerBuff.h"
#include "gameLogic/sceneObject/scenePlayer.h"
#include "main/gameServer.h"
#include "protocol/CASMsg.pb.h"
#include "protocol/PlayerStruct.pb.h"
#include "protocol/MsgResult.pb.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
CPlayerBuff::CPlayerBuff(CScenePlayer& clPlayer)
	: CGamePlayerFun(clPlayer)
{
	registerHandler(C2S_Buff_Del,(CGamePlayerFun::_handleSubMsg)&CPlayerBuff::handle_DelBuff_Ask	);
}

//-------------------------------------------------------------
//------------------------------ 
CPlayerBuff::~CPlayerBuff()
{

}

//-------------------------------------------------------------
//------------------------------ 
void	CPlayerBuff::initialize			()
{
	CGamePlayerFun::initialize();
}
//-------------------------------------------------------------
//------------------------------ 
bool	CPlayerBuff::loadFromProtobuf	(const PBPlayerData&pbPlayer)
{
	return m_clPlayer.getGameBuffMgr().load(pbPlayer.playersystem().buff());
}
//-------------------------------------------------------------
//------------------------------ 
bool	CPlayerBuff::saveToProtobuf		(PBPlayerData&pbPlayer)
{
	return m_clPlayer.getGameBuffMgr().save(pbPlayer.mutable_playersystem()->mutable_buff());
}
//-------------------------------------------------------------
//------------------------------  É¾³ýbuff
bool	CPlayerBuff::handle_DelBuff_Ask	(uint8 uEventID, CNetStream& clStream)
{
	PBC2GDelBuff ask;
	if (!clStream.ParseFromStream(ask))
		return false;

	uint8 uCode = R_ResultBuff_Fail;
	if (!m_clPlayer.getGameBuffMgr().delBuffAsk(ask.buffid(),uCode))
	{
		CGameBuffMgr::send_DelBuffAck(ask.buffid(),m_clPlayer.getUnitID(),uCode,false,&m_clPlayer);
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------ 
void	CPlayerBuff::onClientGo				()
{
	m_clPlayer.getGameBuffMgr().onClientGo();
}