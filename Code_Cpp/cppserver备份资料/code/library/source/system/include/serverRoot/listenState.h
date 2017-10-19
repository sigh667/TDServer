/*------------- ListenState.h
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
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
		RS_Normal		,/*正常状态*/
		RS_Overtime		,/*超时*/ 
		RS_Deny			,/*拒绝服务中*/ 
		RS_Loading		,/*装载*/			
		RS_Config		,/*配置文件装载失败*/ 
		RS_Events		,/*创建事件失败*/ 
		RS_DBConnect	,/*数据库链接*/ 
		RS_Max
	};

protected:
	static const char m_gszRootStateE[RS_Max][32];
	static const char m_gszRootStateC[RS_Max][32];

public:
	/*----->{ 登录服务器掩码 }*/ 
	enum _enAccountState
	{
		AS_NetEaseConnect= RS_Max	,	/*验证链接中*/ 
		AS_NetEaseMgr				,	/*验证管理*/ 
		AS_TimerMgr					,	/*定时器管理*/ 
		AS_LocalNetwork				,	/*本地网络*/ 
		AS_LoginNetwork				,	/*登录网络*/ 
		AS_Max
	};

protected:
	static const char m_gszAccountStateE[AS_Max][32];
	static const char m_gszAccountStateC[AS_Max][32];

public:
	/*----->{ DB代理服务器掩码 }*/ 
	enum _enDBAgentState
	{
		DBAS_Writeing= RS_Max	,	/*写入*/ 
		DBAS_DBTimerMgr			,	/*数据库定时器*/ 
		DBAS_PacketTimer		,	/*数据包定时器*/ 
		DBAS_Network			,	/*网络管理*/ 
		DBAS_Max
	};

protected:
	static const char m_gszDBAgentStateE[DBAS_Max][32];
	static const char m_gszDBAgentStateC[DBAS_Max][32];

public:
	/*----->{ 世界服务器掩码 }*/ 
	enum _enWorldState
	{
		WS_AccountConnect= RS_Max,	/*登录服务器链接失败*/ 
		WS_NetEaseMgr			,	/*验证管理*/ 
		WS_TimerMgr				,	/*定时器*/ 
		WS_LocalNetwork			,	/*内部网络*/ 
		WS_ClientNetwork		,	/*外部网络*/ 
		WS_Max
	};

protected:
	static const char m_gszWorldStateE[WS_Max][32];
	static const char m_gszWorldStateC[WS_Max][32];

public:
	/*----->{ 跨服服务器掩码 }*/ 
	enum _enChatState
	{
		CS_WorldConnect= RS_Max	,	/*世界服务器链接*/ 
		CS_DBLog				,	/*DB日志链接*/ 
		CS_GMConnect			,	/*GM帐号链接*/ 
		CS_LogCreate			,	/*日志创建*/ 
		CS_GMNetServer			,	/*GM网络启动*/ 
		CS_GMMgr				,	/*GM管理启动*/ 
		CS_TimerMgr				,	/*定时器启动*/ 
		CS_ChatNet				,	/*聊天网络启动*/ 
		CS_LocalNet				,	/*内部网络启动*/ 
		CS_Max
	};

protected:
	static const char m_gszChatStateE[CS_Max][32];
	static const char m_gszChatStateC[CS_Max][32];

public:
	/*----->{ 日志服务器掩码 }*/ 
	enum _enLogState
	{
		LS_DBInit	= RS_Max	,	/*数据库初始化*/ 
		LS_SaverInit			,	/*保存初始*/ 
		LS_TimerInit			,	/*定时器初始*/ 
		LS_SaverMgr				,	/*存储管理*/ 
		LS_TimerMgr				,	/*定时器*/ 
		LS_AlarmMgr				,	/*预警管理*/ 
		LS_LocalNet				,	/*Log网络*/ 
		LS_Max
	};

protected:
	static const char m_gszLogStateE[LS_Max][32];
	static const char m_gszLogStateC[LS_Max][32];

public:
	/*----->{ Game }*/ 
	enum _enGameState
	{
		GS_WorldConnect= RS_Max	,	/*世界链接*/ 
		GS_DBLog				,	/*DB日志链接*/ 
		GS_RegionConnect		,
		GS_ChatConnect			,
		GS_Port					,	/*启用端口冲突*/ 
		GS_Max
	};

protected:
	static const char m_gszGameStateE[GS_Max][32];
	static const char m_gszGameStateC[GS_Max][32];

public:
#pragma pack(push,1)
	//------------------------------ 状态消息
	struct _stStateMsg
	{
		uint8	ucServer;	/*服务器类型*/ 
		uint32	uState;		/*状态掩码*/ 
		uint32	uOrderID;	/*序列ID*/ 
		char	szMsg[256];	/*状态描述*/ 

		_stStateMsg			()	{	initMsg();						}
		_stStateMsg&getMsg	()	{	return *this;					}
		void		initMsg	()	{	memset(this,0,sizeof(*this));	}
	};
	struct _stServerState : _stStateMsg
	{
		DWORD	dwProcessID;		/*进程ID*/ 
		DWORD	dwThreadId;			/*主线程ID*/ 

		_stServerState()	{	initState();					}
		void	initState()	{	memset(this,0,sizeof(*this));	}
	};
#pragma pack(pop)

protected:
	bool			m_bSendState;
	ulong			m_uSendIP;			/*UDP接收IP*/ 
	uint16			m_uSendPort;		/*UDP接收端口*/ 
	_stServerState	m_stServerState;	/*服务器状态*/ 

public:
	CListenState();
	virtual~CListenState();

public:
	inline void		setServer	(uint8 ucServer)	{	m_stServerState.ucServer = ucServer;	}

public:
	/*----->{ 初始化 }*/ 
	virtual bool	initialize		();

public:
	/*----->{ 发迸ping状态 }*/ 
	virtual void	ping			(uint8 uMark = RS_Normal,bool bSet = true);

protected:
	/*----->{ 读取状态配置文件 }*/ 
	virtual bool	listenConfig	();

public:
	/*----->{ 获得状态数量 }*/ 
	static uint32		getStateNum	(uint8 ucServer);
	/*----->{ 获得状态消息 }*/ 
	static const char*	getStateMsg	(uint8 ucServer,uint8 ucState,bool bEnglish = false);
};
//****************************************************************
