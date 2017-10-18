/*------------- gameLogin.h
* Copyright (C): 2011  LiuLei
* Author       : ����Դ
* Version      : V1.01
* Date         : 2011/3/24 17:40:16
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "timer/timers.h"
#include "memoryPool/mapPool.h"
#include "singleton/singleton.h"
#include "protocol/PlayerStruct.pb.h"
#include "protocol/AccountStruct.pb.h"
#include "protocol/GameDefine.pb.h"
#include "log/log.h"
#include "def_struct/playerDefine.h"
#include "basic/stringFunctions.h"
/*************************************************************/
class CScenePlayer;
class CClientPacket;
class CNetStream;
class CGameLogin	: public CTimerObject
	, _single_public(CGameLogin)
{
private:
	_single_friend(CGameLogin);

protected:
	/*��¼��Ϣ�ṹ*/ 
	struct CLoginObj	: PBPlayerData
	{
		uint64				uLoginSN;
		uint32				uPlayerID;
		bool				bLoad;
		bool				bLogin;
		uint16				uToSceneID;
		uint64				uToSceneDynID;
		CClientPacket*		pPacket;
		uint64				uTime;
		uint16				uDataFlag;
		uint8				uLoginState;

		inline void		setDataFlag		(uint8 uType,bool bBe)		{	_SET_BIT32(uDataFlag,uType,bBe);						}
		inline bool		checkData		(uint8 uType)				{	return _CHECK_BIT(uDataFlag,_BIT32(uType));				}
		inline bool		beAllReady		()
		{
			for (uint8 i = _emPlayerType_MIN;i <= _emPlayerType_MAX;++i)
			{
				if(!checkData(i))
					return false;
			}

			return true;
		}
		CLoginObj()			{	initialize();	}
		void	initialize()	
		{
			PBPlayerData::Clear();
			uLoginSN		= 0;
			uPlayerID		= 0;
			bLoad			= false;
			bLogin			= true;
			uToSceneID		= 0;
			uToSceneDynID	= 0;
			pPacket			= NULL;
			uTime			= 0;
			uDataFlag		= 0;
			uLoginState		= 0;
		}
		void setGLoginState(uint8 uState)
		{
			uLoginState = uState;
			std::string nameAnsi;
			_convertUTF8toANSI(playerbase().playerdisplay().playername(), nameAnsi);
			CLog::print("��ɫ[%u],����[%s],��ˮ[%lld],%s",
						uPlayerID, 
						nameAnsi.c_str(),
						uLoginSN,
						g_szGameLoginState[uState]);
		}
	};
private:
	typedef	CMapPool<uint32,CLoginObj>	MAP_LOGIN;
	typedef	MAP_LOGIN::iterator		iterator;

private:
	uint64			m_uOvertime;/*>[ ��ʱʱ��(��) ]*/ 
	MAP_LOGIN		m_mapLogin;	/*>[ ��¼�б� ]*/ 

private:
	CGameLogin();
	~CGameLogin();

public:
	inline CLoginObj*		findLogin	(uint32 uPlayerID)	{	return m_mapLogin.find(uPlayerID);	}

private:
	/*----->{ ������Ϸ }*/ 
	bool			enter		(CLoginObj* pObject);
	/*----->{ ȡ����¼ }*/ 
	bool			cancel		(CLoginObj* pObject);

public:
	/*----->{ �߳�������� }*/ 
	bool			kickPlayer	(uint32 uPlayerID,bool bSend = true);
	/*----->{ �߳��ƶ����	 }*/ 
	bool			kick		(uint32 uPlayerID);

public:
	/*----->{ �ͻ��˵�¼ }*/ 
	bool			login		(uint32 uPlayerID,uint64 uLoginSN,CClientPacket* pPacket);
	/*--->[ �ͻ��˶Ͽ� ]*/
	void			logout		(uint32 uPlayerID,CClientPacket* pPacket);
	/*--->[ �ͻ��˶������� ]*/
	void			reconnect	(uint32 uPlayerID,uint64 uLoginSN,CClientPacket* pPacket);
public:
	/*----->{ ��������¼ }*/ 
	bool			login		(PBAccount& stAccount,uint32 uPlayerID,uint16 uSceneID,uint64 uToSceneDynID,bool bTransfer);
	/*----->{ ���������°��� }*/ 
	bool			update		(uint32 uPlayerID,uint8 uType,CNetStream& clStream);
public:
	/*----->{ ��¼��ʱ���� }*/ 
	virtual void timerProcess();
};
extern CGameLogin* g_pGameLogin;
