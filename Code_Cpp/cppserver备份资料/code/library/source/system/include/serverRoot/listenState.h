/*------------- ListenState.h
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.01
* Date         : 2011/3/24 9:07:08
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <iostream>
#include "basic/basicTypes.h"
#include "network/include/udpNetwork.h"
#include "server_define.h"
/*************************************************************/

const int PING_TIMEOUT = 10;

#define Listen_ConfigFile	"config/listenState.ini"
//------------------------------------------------------
//------------------------------ 
class CListenState	: public CUDPNetwork
{
protected:
	typedef CUDPNetwork	Parent;

public:
	enum _enRootState
	{
		RS_Normal		,/*����״̬*/
		RS_Overtime		,/*��ʱ*/ 
		RS_Deny			,/*�ܾ�������*/ 
		RS_Loading		,/*װ��*/			
		RS_Config		,/*�����ļ�װ��ʧ��*/ 
		RS_Events		,/*�����¼�ʧ��*/ 
		RS_DBConnect	,/*���ݿ�����*/ 
		RS_Max
	};

protected:
	static const char m_gszRootStateE[RS_Max][32];
	static const char m_gszRootStateC[RS_Max][32];

public:
	/*----->{ ��¼���������� }*/ 
	enum _enAccountState
	{
		AS_NetEaseConnect= RS_Max	,	/*��֤������*/ 
		AS_NetEaseMgr				,	/*��֤����*/ 
		AS_TimerMgr					,	/*��ʱ������*/ 
		AS_LocalNetwork				,	/*��������*/ 
		AS_LoginNetwork				,	/*��¼����*/ 
		AS_Max
	};

protected:
	static const char m_gszAccountStateE[AS_Max][32];
	static const char m_gszAccountStateC[AS_Max][32];

public:
	/*----->{ DB������������� }*/ 
	enum _enDBAgentState
	{
		DBAS_Writeing= RS_Max	,	/*д��*/ 
		DBAS_DBTimerMgr			,	/*���ݿⶨʱ��*/ 
		DBAS_PacketTimer		,	/*���ݰ���ʱ��*/ 
		DBAS_Network			,	/*�������*/ 
		DBAS_Max
	};

protected:
	static const char m_gszDBAgentStateE[DBAS_Max][32];
	static const char m_gszDBAgentStateC[DBAS_Max][32];

public:
	/*----->{ ������������� }*/ 
	enum _enWorldState
	{
		WS_AccountConnect= RS_Max,	/*��¼����������ʧ��*/ 
		WS_NetEaseMgr			,	/*��֤����*/ 
		WS_TimerMgr				,	/*��ʱ��*/ 
		WS_LocalNetwork			,	/*�ڲ�����*/ 
		WS_ClientNetwork		,	/*�ⲿ����*/ 
		WS_Max
	};

protected:
	static const char m_gszWorldStateE[WS_Max][32];
	static const char m_gszWorldStateC[WS_Max][32];

public:
	/*----->{ ������������� }*/ 
	enum _enChatState
	{
		CS_WorldConnect= RS_Max	,	/*�������������*/ 
		CS_DBLog				,	/*DB��־����*/ 
		CS_GMConnect			,	/*GM�ʺ�����*/ 
		CS_LogCreate			,	/*��־����*/ 
		CS_GMNetServer			,	/*GM��������*/ 
		CS_GMMgr				,	/*GM��������*/ 
		CS_TimerMgr				,	/*��ʱ������*/ 
		CS_ChatNet				,	/*������������*/ 
		CS_LocalNet				,	/*�ڲ���������*/ 
		CS_Max
	};

protected:
	static const char m_gszChatStateE[CS_Max][32];
	static const char m_gszChatStateC[CS_Max][32];

public:
	/*----->{ ��־���������� }*/ 
	enum _enLogState
	{
		LS_DBInit	= RS_Max	,	/*���ݿ��ʼ��*/ 
		LS_SaverInit			,	/*�����ʼ*/ 
		LS_TimerInit			,	/*��ʱ����ʼ*/ 
		LS_SaverMgr				,	/*�洢����*/ 
		LS_TimerMgr				,	/*��ʱ��*/ 
		LS_AlarmMgr				,	/*Ԥ������*/ 
		LS_LocalNet				,	/*Log����*/ 
		LS_Max
	};

protected:
	static const char m_gszLogStateE[LS_Max][32];
	static const char m_gszLogStateC[LS_Max][32];

public:
	/*----->{ Game }*/ 
	enum _enGameState
	{
		GS_WorldConnect= RS_Max	,	/*��������*/ 
		GS_DBLog				,	/*DB��־����*/ 
		GS_RegionConnect		,
		GS_ChatConnect			,
		GS_Port					,	/*���ö˿ڳ�ͻ*/ 
		GS_Max
	};

protected:
	static const char m_gszGameStateE[GS_Max][32];
	static const char m_gszGameStateC[GS_Max][32];

public:
#pragma pack(push,1)
	//------------------------------ ״̬��Ϣ
	struct _stStateMsg
	{
		uint8	ucServer;	/*����������*/ 
		uint32	uState;		/*״̬����*/ 
		uint32	uOrderID;	/*����ID*/ 
		char	szMsg[256];	/*״̬����*/ 

		_stStateMsg			()	{	initMsg();						}
		_stStateMsg&getMsg	()	{	return *this;					}
		void		initMsg	()	{	memset(this,0,sizeof(*this));	}
	};
	struct _stServerState : _stStateMsg
	{
		DWORD	dwProcessID;		/*����ID*/ 
		DWORD	dwThreadId;			/*���߳�ID*/ 

		_stServerState()	{	initState();					}
		void	initState()	{	memset(this,0,sizeof(*this));	}
	};
#pragma pack(pop)

protected:
	bool			m_bSendState;
	ulong			m_uSendIP;			/*UDP����IP*/ 
	uint16			m_uSendPort;		/*UDP���ն˿�*/ 
	_stServerState	m_stServerState;	/*������״̬*/ 

public:
	CListenState();
	virtual~CListenState();

public:
	inline void		setServer	(uint8 ucServer)	{	m_stServerState.ucServer = ucServer;	}

public:
	/*----->{ ��ʼ�� }*/ 
	virtual bool	initialize		();

public:
	/*----->{ ����ping״̬ }*/ 
	virtual void	ping			(uint8 uMark = RS_Normal,bool bSet = true);

protected:
	/*----->{ ��ȡ״̬�����ļ� }*/ 
	virtual bool	listenConfig	();

public:
	/*----->{ ���״̬���� }*/ 
	static uint32		getStateNum	(uint8 ucServer);
	/*----->{ ���״̬��Ϣ }*/ 
	static const char*	getStateMsg	(uint8 ucServer,uint8 ucState,bool bEnglish = false);
};
//****************************************************************
