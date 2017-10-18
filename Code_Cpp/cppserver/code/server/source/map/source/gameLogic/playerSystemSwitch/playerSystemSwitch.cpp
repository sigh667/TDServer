/*------------- playerSystemSwitch.cpp
 * Copyright (C): 2016  Mokylin·Mokyqi
 * Author       : 方声辉
 * Version      : V1.01
 * Date         : 2016/09/30
 * 
 */
/***************************************************************
* 
***************************************************************/

#include "cfg_data/systemSwitchData/systemSwitchData.h"
//#include "gameLogic/playerQuest/playerQuestMgr.h"
#include "gameLogic/playerSystemSwitch/playerSystemSwitch.h"
#include "gameLogic/sceneObject/scenePlayer.h"
#include "protocol/CASMsg.pb.h"
#include "protocol/MsgResult.pb.h"
#include "protocol/CASProtocol.pb.h"
#include "log/log.h"
/************************************************************************/
/************************************************************************/
//-------------------------------------------------------------
//------------------------------ 
CPlayerSystemSwitch::CPlayerSystemSwitch(CScenePlayer& clPlayer) : CGamePlayerFun(clPlayer)
{

}

//-------------------------------------------------------------
//------------------------------
CPlayerSystemSwitch::~CPlayerSystemSwitch()
{

}

//-------------------------------------------------------------
//------------------------------
void CPlayerSystemSwitch::initialize()
{
	CGamePlayerFun::initialize();
	m_setSystemSwith.clear();
}

//-------------------------------------------------------------
//------------------------------
bool CPlayerSystemSwitch::saveToProtobuf(PBPlayerData&pbPlayer)
{
	PBPlayerSystem* pSystem = pbPlayer.mutable_playersystem();
	CHECKF(pSystem != NULL);
	
	PBSystemSwitch * pSwitch = pSystem->mutable_systemswitch();
	CHECKF(pSwitch != NULL);

	pSwitch->clear_id();
	auto _pos = m_setSystemSwith.begin();
	auto _end = m_setSystemSwith.end();
	for ( ; _pos != _end; ++ _pos)
	{
		pSwitch->add_id(*_pos);
	}
	
	return true;
}
//-------------------------------------------------------------
//------------------------------
void CPlayerSystemSwitch::newOnline()
{
	// 新用户需要更新等级 
	callback_upLevel(m_clPlayer.getLevel());
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerSystemSwitch::loadFromProtobuf(const PBPlayerData&pbPlayer)
{
	const PBSystemSwitch& pSystem = pbPlayer.playersystem().systemswitch();
	for (int32 i = 0; i < pSystem.id_size(); ++i)
	{
		m_setSystemSwith.insert(pSystem.id(i));
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------回调完成任务
bool CPlayerSystemSwitch::callback_completeQuest(uint32 uQuestID)
{
	MAP_SYSTEMSWITCH_MODEL mapModel;
	g_clSystemSwitchData.findQuest(uQuestID,mapModel);
	MAP_SYSTEMSWITCH_MODEL::iterator _pos = mapModel.begin();
	for (; _pos != mapModel.end(); ++_pos)
	{
		uint32 uID = _pos->first;
		_stSystemSwitchModel* pModel = _pos->second;
		if (m_setSystemSwith.is_exist(uID) || !pModel)
			continue;

		if (pModel->uLevel && m_clPlayer.getLevel() < pModel->uLevel)
			continue;

		if (pModel->uVip && m_clPlayer.getVipLevel() < pModel->uVip)
			continue;

		m_setSystemSwith.insert(uID);
		//同步给客户端
		send_SystemSwitchOpen_Ack(uID);
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------回调升级
bool CPlayerSystemSwitch::callback_upLevel(uint16 uLevel)
{
	MAP_SYSTEMSWITCH_MODEL mapModel;
	g_clSystemSwitchData.findLevel(uLevel,mapModel);
	MAP_SYSTEMSWITCH_MODEL::iterator _pos = mapModel.begin();
	for (; _pos != mapModel.end(); ++_pos)
	{
		uint32 uID = _pos->first;
		_stSystemSwitchModel* pModel = _pos->second;
		if (m_setSystemSwith.is_exist(uID) || !pModel)
			continue;

		//判断任务
		//if (pModel->uQuestID && !m_clPlayer.getPlayerQuestMgr()->isComplete(pModel->uQuestID))
		//	continue;

		if (pModel->uVip && m_clPlayer.getVipLevel() < pModel->uVip)
			continue;

		m_setSystemSwith.insert(uID);
		//同步给客户端
		send_SystemSwitchOpen_Ack(uID);
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------回调Vip
bool CPlayerSystemSwitch::callback_upVip(uint16 uVipLevel)
{
	MAP_SYSTEMSWITCH_MODEL mapModel;
	g_clSystemSwitchData.findVip(uVipLevel,mapModel);
	MAP_SYSTEMSWITCH_MODEL::iterator _pos = mapModel.begin();
	for (; _pos != mapModel.end(); ++_pos)
	{
		uint32 uID = _pos->first;
		_stSystemSwitchModel* pModel = _pos->second;
		if (m_setSystemSwith.is_exist(uID) || !pModel)
			continue;

		//判断任务
		//if (pModel->uQuestID && !m_clPlayer.getPlayerQuestMgr()->isComplete(pModel->uQuestID))
		//	continue;

		if (pModel->uLevel && m_clPlayer.getLevel() < pModel->uLevel)
			continue;

		m_setSystemSwith.insert(uID);
		//同步给客户端
		send_SystemSwitchOpen_Ack(uID);
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------通知客户端开启
bool CPlayerSystemSwitch::send_SystemSwitchOpen_Ack(uint32 uID)
{
	PBG2CCommon_SystemSwitchAck ack;
	ack.set_id(uID);

	return m_clPlayer.sendMsg(P_S2C_Protocol_Common,S2C_Common_SystemSwitch,&ack,R_ResultCommon_Succeed);
}

//-------------------------------------------------------------
//------------------------------
bool CPlayerSystemSwitch::openSystemSwitch(uint32 uID)
{
	m_setSystemSwith.insert(uID);
	return false;
}

//-------------------------------------------------------------
//------------------------------
bool CPlayerSystemSwitch::isExit(uint32 ID)
{
	return m_setSystemSwith.is_exist(ID);
}
