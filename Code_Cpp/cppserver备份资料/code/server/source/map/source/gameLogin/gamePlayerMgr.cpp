/*----------------- gamePlayerMgr.cpp
*
* Copyright (C): 2011  LiuLei
* Author       : ����Դ
* Version      : V1.0
* Date         : 2011/9/13 10:02:49
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "gameLogin/gamePlayerMgr.h"
#include "main/gameServer.h"
#include "log/log.h"
#include "gameScene/gameSceneMgr.h"
#include "protocol/MsgResult.pb.h"
#include "protocol/WAGProtocol.pb.h"
#include "cfg_data/constantData/constantData.h"
/*************************************************************/
CGamePlayerMgr* g_pGamePlayerMgr = NULL;
//-------------------------------------------------------------
//------------------------------ 
CGamePlayerMgr::CGamePlayerMgr()
{
	m_bShutdown= false;
	m_mapLogoutPlayer.clear();
	m_mapWaitLogin.clear();
}
//-------------------------------------------------------------
//------------------------------ 
CScenePlayer*CGamePlayerMgr::findPlayer(uint32 uPlayerID)
{
	return Parent::find(uPlayerID);
}

//-------------------------------------------------------------
//------------------------------ 
bool	CGamePlayerMgr::onlinePlayer(PBPlayerData&rPBPlayer,CClientPacket*pPacket,uint16 uToSceneID,uint64 uToSceneDynID,bool bLogin)
{
	if(m_bShutdown )
		return false;

	uint32 uPlayerID	= rPBPlayer.playerbase().playerdisplay().playerid();
	uint64 uLoginSN = rPBPlayer.accountinfo().accountflag().loginsn();

	//���������б����Ƴ�
	m_mapLogoutPlayer.erase(uPlayerID);
	m_mapWaitLogin.erase(uPlayerID);

	CScenePlayer*pGamePlayer = Parent::find(uPlayerID);
	if(pGamePlayer)
	{//��������ʱ�߳�����
		CLog::warn("��ɫ[%d]�ظ�����!",uPlayerID);
		CClientPacket*pOldPacket = pGamePlayer->m_pPacket;
		if (pOldPacket)
		{
			pOldPacket->bindPlayer(NULL);
			//�����߳��¼�
			pGamePlayer->setClientPacket(NULL);
		}
		pGamePlayer->logoutLeave();
	}
	if (!pGamePlayer)
		pGamePlayer = Parent::allocate(uPlayerID);

	if (!pGamePlayer)
	{
		CLog::error("��ɫ[%d]����ʧ��!",uPlayerID);
		return false;
	}
	pGamePlayer->initialize();
	pGamePlayer->buildPlayer(rPBPlayer,bLogin);
	//�����ͼ
	uint16 uSceneID = g_pGameSceneMgr->playerEnterScene(pGamePlayer,uToSceneID,uToSceneDynID);
	if(!uSceneID)
	{
		onPlayerRemove(uPlayerID,uLoginSN,true);
		return false;
	}
	
	if(!setPlayerPacket(pGamePlayer,pPacket))
		m_mapWaitLogin.insert_(pGamePlayer->getPlayerID(),pGamePlayer);

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
void	CGamePlayerMgr::shutdown()
{
	m_bShutdown= true;
	//֪ͨ���н�ɫ
	CScenePlayer*pPlayer = NULL;
	Parent::iterator _pos;
	Parent::iterator _end;
	Parent::getHead(_pos);
	Parent::getEnd(_end);
	while(_pos != _end)
	{
		pPlayer = Parent::getNext(_pos);
		if(!pPlayer)
			continue;

		pPlayer->logout();
	}
	Parent::clear();
	m_mapWaitLogin.clear();
	m_mapLogoutPlayer.clear();
}
//-------------------------------------------------------------
//------------------------------ 
void	CGamePlayerMgr::logoutWorldPlayer(uint16 uLogicWorldID)
{
	CScenePlayer*pPlayer = NULL;
	{//֪ͨ���н�ɫ
		Parent::iterator _pos;
		Parent::iterator _end;
		Parent::getHead(_pos);
		Parent::getEnd(_end);
		while(_pos != _end)
		{
			pPlayer = Parent::getNext(_pos);
			if(!pPlayer || pPlayer->getLogicWorldID() != uLogicWorldID)
				continue;

			pPlayer->logout();
			Parent::release(pPlayer->getPlayerID());
		}
	}

	{
		MAP_PLAYER::iterator _pos = m_mapWaitLogin.begin();
		for (; _pos != m_mapWaitLogin.end();)
		{
			pPlayer = _pos->second;
			if (!pPlayer || pPlayer->getLogicWorldID() != uLogicWorldID)
			{
				++_pos;
				continue;
			}
			m_mapWaitLogin.erase(_pos++);
		}
	}
	
	{
		MAP_PLAYER::iterator _pos = m_mapLogoutPlayer.begin();
		for (; _pos != m_mapLogoutPlayer.end();)
		{
			pPlayer = _pos->second;
			if (!pPlayer || pPlayer->getLogicWorldID() != uLogicWorldID)
			{
				++_pos;
				continue;
			}
			m_mapLogoutPlayer.erase(_pos++);
		}
	}
}
//-------------------------------------------------------------
//------------------------------ ��ɫɾ��
void	CGamePlayerMgr::onPlayerRemove	(uint32 uPlayerID,uint64 uLoginSN,bool bLogout)
{
	CLog::print("��ɫ[%d]�Ƴ�",uPlayerID);
	CScenePlayer*pPlayer = Parent::find(uPlayerID);
	if(!pPlayer || pPlayer->getLoginSN() != uLoginSN)
		return;

	//����
	m_mapLogoutPlayer.insert_(uPlayerID,pPlayer);
}
//-------------------------------------------------------------
//------------------------------ ��ʱ������
void	CGamePlayerMgr::timerProcess()
{
	CScenePlayer*pPlayer = NULL;

	uint64 uCurrentTime	= (uint64)getTime();
	{//�ȴ���¼�н�ɫ
		MAP_PLAYER::iterator _pos = m_mapWaitLogin.begin();
		MAP_PLAYER::iterator _end = m_mapWaitLogin.end();
		while (_pos != _end)
		{
			pPlayer = _pos->second;
			if(pPlayer && (pPlayer->getBuildTime() + 30) > uCurrentTime)
			{
				++_pos;
				continue;
			}
			if(pPlayer)
				pPlayer->logout();
			_pos = m_mapWaitLogin.erase_(_pos);
		}
	}

	{//�뿪��ɫ
		uint32 uPlayerID = 0;
		MAP_PLAYER::iterator _pos = m_mapLogoutPlayer.begin();
		MAP_PLAYER::iterator _end = m_mapLogoutPlayer.end();
		for (;_pos != _end;++_pos)
		{
			uPlayerID = _pos->first;
			m_mapWaitLogin.erase(uPlayerID);
			pPlayer = Parent::find(uPlayerID);
			if(pPlayer)
			{
				pPlayer->logoutScene();
				pPlayer->unregisterObject();
			}

			Parent::release(uPlayerID);
		}
		m_mapLogoutPlayer.clear();
	}

	//�����޳� �����
	uint32 uReconnectTime = g_clConstantData.Game_ReconnetTime;
	Parent::iterator	pos = Parent::begin();
	while (!Parent::isTail(pos))
	{
		CScenePlayer* pPlayer = Parent::getNext(pos);
		if (!pPlayer || !pPlayer->getOfflineTime())
			continue;

		if (pPlayer->getOfflineTime() + uReconnectTime <= uCurrentTime)
		{
			pPlayer->setOfflineTime(0);
			pPlayer->logout();
		}
	}
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGamePlayerMgr::setPlayerPacket		(CScenePlayer*pPlayer,CClientPacket*pPacket)
{
	if(!pPlayer || !pPacket || m_bShutdown)
		return false;

	pPlayer->setClientPacket(pPacket);
	pPacket->bindPlayer(pPlayer);

	PBG2CLoginAck ack;
	ack.set_sceneid(pPlayer->getSceneID());
	ack.set_scenecellid(pPlayer->getSceneCellID());
	ack.set_scenedynid(pPlayer->getSceneDynID());
	ack.set_unitid(pPlayer->getUnitID());
	ack.mutable_pbpoint()->set_posx(pPlayer->getPosition().x);
	ack.mutable_pbpoint()->set_posy(pPlayer->getPosition().y);
	pPacket->send_Login_Ack(NetResult_Succeed,&ack);

	//��ͻ��˷�����Ϣ
	pPlayer->sendPlayer();

	// �����
	pPlayer->sendMsg(P_S2C_Protocol_Common,S2C_Common_Ready,NULL);

	//֪ͨ����������������
	pPlayer->sendPlayerMsgToWorld(P_G2W_Protocol_Common,G2W_Common_PlayerReady);


	m_mapWaitLogin.erase(pPlayer->getPlayerID());
	return true;
}
