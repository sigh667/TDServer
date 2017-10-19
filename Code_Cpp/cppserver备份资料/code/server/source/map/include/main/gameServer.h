/*------------- gameServer.h
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.01
* Date         : 2011/2/24 13:34:32
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "operators.h"
//------------------------------ oshen
#include "network/netConnect.h"
#include "connect_checker/connect_checker.h"
#include "serverRoot/serverRoot.h"
//------------------------------ 
#include "def_struct/configStruct.h"
//------------------------------ 
#include "network/clientNetwork.h"
#include "stl/std_map.h"
#include "packet/worldPacket.h"
#include "packet/globalPacket.h"
/*************************************************************/
class CGameServer	: public CServerRoot
	, _single_public(CGameServer)
{
private:
	_single_friend(CGameServer);

public:
	typedef CServerRoot Parent;

public:
	_stGameConfig			m_stGameCfg;		//游戏配置
	_stDebugMsgConfig		m_stDebugMsgCfg;	//协议打印配置
	_stNetAddress			m_stConfig;			//服务器配置(针对客户端网络链接)
	_stNetHost				m_stHostClient;
	_stNetAddress			m_stGlobalNet;		//全局服务器链接
	_stNetAddress			m_stWorldNet;		//世界服务器链接
	_stNetTimeout			m_c_NetTimeout;
	_stNetTimeout			m_s_NetTimeout;

public:
	CClientNetwork			m_clClientNetwork;	/*Client通讯服务*/ 
	CNetConnect				m_clGlobalConnect;	/*全局服务器连接*/ 
	CNetConnect				m_clWorldConnect;	/*世界服务器连接*/ 
	connect_checker			m_clConnectChecker;
public:
	CGlobalPacket			m_clGlobalPacket;
	CWorldPacket			m_clWorldPacket;
private:
	stl_set<uint16>			m_setRegisterWorld;
	stl_set<uint16>			m_setRegisterGlobal;
	stl_map<uint16,uint32>	m_mapWorldStartTime;
private:
	CGameServer();
	~CGameServer();

public:
	inline bool		checkGameSwitch		(uint8 _switch)						{	return m_stGameCfg.checkSwitch(_switch);				}
	inline bool		checkDebugMsgSwitch	(uint16 _switch)					{	return m_stDebugMsgCfg.checkSwitch(_switch);			}
	inline bool		checkDebugMsgFilter	(uint32 mainCode, uint32 subCode)	{	return m_stDebugMsgCfg.checkFilter(mainCode, subCode);	}

public:
	/*----->{ 初始化 }*/ 
	bool			initialize			(uint16 uServerID,const char* szConfigFile);
	/*----->{ 启动服务 }*/ 
	bool			startServices		();	
	/*----->{ 停止服务 }*/ 
	void			stopServices		();
	
protected:
	/*----->{ 拒绝服务 }*/ 
	virtual void	denyServices		();
	//--- 能否退出
	virtual bool	canExitServices		();

private:
	/*----->{ 是否可以启动 }*/ 
	bool			isCanStart			();
	/*----->{ 从本地配置文件Config.ini读取数据 }*/ 
	bool			loadConfig			(const char* szConfigFile);

public:
	//--- 装载数据
	bool			loadData			();
	//--- 装载数据
	bool			reLoadData			();
	//--- 装载脚本
	bool			loadScript			();
private:
	/*----->{ 显示服务信息 }*/ 
	void			showServiceInfo		();
	/*----->{ 服务处理 }*/ 
	void			timerProcess		();

private:
	bool			instance_new		();
	void			instance_del		();

public:
	bool			isLoaded			();

public:
	bool			startClientNetwork	();
public:
	uint32			getWorldStartTime	(uint16 uLogicWorldID);
	void			setWorldStartTime	(uint16 uLogicWorldID,uint32 uTime);
public:
	// 给全局服务器发送消息
	bool			isGlobalConneted	(uint16 uLogicWorldID);
	bool			sendMsgToGlobal		(uint16 uLogicWorldID,uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent = 0);
	bool			sendBufferToGlobal	(uint16 uLogicWorldID,uint8 uMainProtocol, uint8 uProtocol, void* pBuffer,uint32 uSize, uint8 uEvent = 0);

	// 给世界服务器发送消息
	bool			isWorldConneted		(uint16 uLogicWorldID);
	bool			sendMsgToWorld		(uint16 uLogicWorldID,uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent = 0);
	bool			sendBufferToWorld	(uint16 uLogicWorldID,uint8 uMainProtocol, uint8 uProtocol, void* pBuffer,uint32 uSize, uint8 uEvent = 0);
public:
	void			registerWorld		(uint16 uLogicWorldID);
	void			unregisterWorld		(uint16 uLogicWorldID);
	void			unregisterAllWorld	();

	void			registerGlobal		(uint16 uLogicWorldID);
	void			unregisterGlobal	(uint16 uLogicWorldID);
	void			unregisterAllGlobal	();
};
//****************************************************************
extern	CGameServer*	g_pServer;
//****************************************************************

