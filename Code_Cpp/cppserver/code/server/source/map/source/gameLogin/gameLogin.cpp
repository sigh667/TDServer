/*------------- gameLogin.cpp
* Copyright (C): 2011  LiuLei
* Author       : ����Դ
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
//------------------------------ ȡ����¼
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
//------------------------------ �߳��������
bool	CGameLogin::kickPlayer(uint32 uPlayerID,bool bSend)
{
	CScenePlayer* pPlayer = g_pGamePlayerMgr->find(uPlayerID);
	if(pPlayer)
	{
		pPlayer->SetGLoginState(_GLS_PlayerKickOut);
		if(pPlayer->m_pPacket)
		{
			
			CLog::warn("CGameLogin::KickPlayer(%d)PlayerName[%s]һ���������[%s]", \
				uPlayerID,pPlayer->getPlayerBase().szPlayerName, bSend ? "�����ɫ����" : "�������ɫ����" );
			pPlayer->m_pPacket->send_Kick_Notify(R_Login_Kickout);
		}
		pPlayer->logout();
		return true;
	}   

	return false;
}

//-------------------------------------------------------------
//------------------------------ �߳��ƶ����
bool	CGameLogin::kick(uint32 uPlayerID)
{
	//ȡ����¼
	bool	bKick = cancel(findLogin(uPlayerID));
	//�߳����߽�ɫ
	bKick	|= kickPlayer(uPlayerID);

	return bKick;
}
//-------------------------------------------------------------
//------------------------------ �ͻ��˶�������
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

		CLog::error("�û������������ݲ���[%u][%llu]!...",uPlayerID,uLoginSN);
		return;
	}

	pPlayer->addState(StateType_WaitReady);
	pPlayer->setOfflineTime(0);
	g_pGamePlayerMgr->setPlayerPacket(pPlayer,pPacket);

	CLog::warn("���������ɹ�:%u��%llu",uPlayerID,uLoginSN);
}

//-------------------------------------------------------------
//------------------------------ �ͻ��˵�¼
bool	CGameLogin::login(uint32 uPlayerID,uint64 uLoginSN,CClientPacket* pPacket)
{
	if (!uPlayerID || !uLoginSN || !pPacket)
		return false;

	CLoginObj*pLogin = findLogin(uPlayerID);
	if (pLogin)
	{/*����������ĵ�¼��ɫ��Ϣ�����������*/ 
		/*�Լ�����*/ 
		pLogin->setGLoginState(_GLS_LoginAsk);
		CLog::error("�ͻ��˵�¼�������������[%u],oldLoginSN[%lld],newLoginsn[%lld]",uPlayerID,pLogin->uLoginSN,uLoginSN);
		if (pLogin->pPacket == pPacket)
		{
			CLog::warn("ͬһ�����û��ظ���¼!...");
			return true;
		}

		/*�Լ��ǷǷ���ɫ���������*/ 
		if(pLogin->uLoginSN != uLoginSN || pLogin->pPacket)
		{
			pPacket->send_Login_Ack(NetResult_Fail);
			pPacket->send_Disconnection();

			CLog::error("�û���¼��֤���д���UID��ͬ���������߳��Լ�(%lld != %lld)(%p)!...",pLogin->uLoginSN,uLoginSN,pLogin->pPacket);
			return false;
		}
		pLogin->pPacket	= pPacket;
	}
	
	if(!pLogin)
	{
		//����Ƿ��Ѵ���
		CScenePlayer* pPlayer = g_pGamePlayerMgr->find(uPlayerID);
		if(pPlayer)
		{
			if(pPlayer->m_pPacket || pPlayer->getLoginSN() != uLoginSN)
			{
				pPacket->send_Login_Ack(NetResult_Fail);
				pPacket->send_Disconnection();

				CLog::error("�û���¼��֤��������Ƿ��Ѵ���[%u][%llu]!...",uPlayerID,uLoginSN);
				return false;
			}

			g_pGamePlayerMgr->setPlayerPacket(pPlayer,pPacket);
			CLog::warn("������Packet���:%u",uPlayerID);
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
//------------------------------ �ͻ��˶Ͽ�
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
//------------------------------ ��������¼
bool	CGameLogin::login(PBAccount& stAccount,uint32 uPlayerID,uint16 uSceneID,uint64 uToSceneDynID,bool bTransfer)
{
	kickPlayer(uPlayerID,false);

	CLoginObj*pLogin = findLogin(uPlayerID);
	if (pLogin)
	{
		pLogin->setGLoginState(_GLS_WorldLoginAsk);
		//CLog::error("�������¼�������������[%u],oldLoginSN[%u],newLoginsn[%u]",uPlayerID,pLogin->uLoginSN,stAccount.accountflag().loginsn());
		if (pLogin->uLoginSN != stAccount.accountflag().loginsn() )
		{	/*��¼ʧ��*/ 
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
//------------------------------ ���������°���
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

	/*������Ϸ*/ 
	if (pLogin->beAllReady())
		enter(pLogin);
	return true;
}
//-------------------------------------------------------------
//------------------------------ ��¼��ʱ����
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
