/*------------- gamePlayerFun.h
* Copyright (C): 2014 Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.01
* Date         : 2014/20/10 11:23
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "stream/bufferStream.h"
#include "protocol/PlayerStruct.pb.h"
#include "protocol/GameDefine.pb.h"
#include "def_struct/stlDefine.h"
#include "common/tickdown.h"
/*************************************************************/
class CScenePlayer;
class CClientPacket;
class CNetStream;
class CGamePlayerFun : public CTickDown
{
protected:
	typedef bool (CGamePlayerFun::*_handleSubMsg)	(uint8 uEventID,CNetStream& clStream);
	typedef stl_map<uint8,_handleSubMsg>			MAP_HANDLER;
protected:
	CScenePlayer&	m_clPlayer;
	MAP_HANDLER		m_mapHandler;
public:
	CScenePlayer&	getPlayer();
public:
	CGamePlayerFun(CScenePlayer& clPlayer);
	virtual~CGamePlayerFun();
public:
	virtual	bool	loadFromProtobuf		(const PBPlayerData&pbPlayer)				= 0;
	virtual	bool	saveToProtobuf			(PBPlayerData&pbPlayer)						= 0;
	virtual	_emGameSystemType	getFunType	()const										= 0;
public:
	virtual	bool	netPacket			(uint8 uProtocol,uint8 uEventID,CNetStream&clStream);
	virtual	void	registerHandler		(uint8 uProtocol,_handleSubMsg hHandler);
public:
	virtual bool	handleOfflineData	(uint32 uSignType, CNetStream&clStream){ return false; }

	virtual	void	initialize			(){		CTickDown::initialize();	};
	//--- ÿ������
	virtual void	cleanupDaily		(){}
	//--- ÿ������
	virtual void	cleanupWeekly		(){}
	//--- ÿ������
	virtual void	cleanupMonthly		(){}
	//--- ����
	virtual void	onLogin				(){}
	//--- ����
	virtual void	onLogout			(){}
	//--- ͬ��ǰ��
	virtual void	onClientGo			(){}
	//---������ɻص� 
	virtual	void	onBuildComplete		(){}
	//---�ռ�����
	virtual void	collectSkill		(MAP_U16_U8& mapSkill){}
	//---������
	virtual	void	newOnline			(){}
	virtual void	onLogicDayCleanup(uint64 uProcessTime, uint64 uTime){}
};
