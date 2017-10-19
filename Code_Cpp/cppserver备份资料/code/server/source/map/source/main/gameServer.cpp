/*------------- gameServer.cpp
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.01
* Date         : 2011/2/24 13:38:19
*
*/ 
/***************************************************************
* 
***************************************************************/
#include "log/log.h"
#include "interactionMgr/interactionMgr.h"
//------------------------------ 
#include "report_log.h"
#include "game_log.h"
#include "def_struct/cfgJson.h"
#include "def_functions/common.h"
#include "protocol/PBConfig.pb.h"
//------------------------------ 
#include "cfg_data/loadData/loadData.h"
//------------------------------ 
#include "main/gameServer.h"
#include "luaScript/luaScript.h"
#include "packet/worldPacket.h"
#include "packet/globalPacket.h"
#include "gameLogin/gameLogin.h"
#include "gameSave/gameSave.h"
#include "gameLogin/gamePlayerMgr.h"
#include "gameScene/gameSceneMgr.h"
#include "gameLogic/sceneObject/aiRegister.h"
#include "behaviac/base/workspace.h"
#include "gameScene/copymapMgr.h"
#include "gameLogic/gameBroadcast/gameBroadcast.h"
//#include "gameLogic/globalObject/worldBossMgr.h"
/*************************************************************/
CGameServer*	g_pServer		= NULL;
//-------------------------------------------------------------
//------------------------------ 
CGameServer::CGameServer()
{
	setServer(ServerType_Game);
	setServerType(ServerType_Game);
	g_pCfgJsonRoot		= _single_instance_new(CfgJsonRoot);
}

//-------------------------------------------------------------
//------------------------------ 
CGameServer::~CGameServer()
{
	_single_instance_del(g_pCfgJsonRoot);
}

//-------------------------------------------------------------
//------------------------------ 初始化
bool	CGameServer::initialize(uint16 uServerID,const char* szConfigPath)
{
	if(m_bInitFlag)
		return true;
	
	if(!instance_new())
		return false;

	if (!Parent::initialize(uServerID, szConfigPath))
		return false;
	
	CLog::setDirectory(g_pCfgJsonRoot->game().common().logpath().c_str(), "game_server");
	CLog::setMaskFlag(CLog::LT_Log, g_pCfgJsonRoot->game().common().logprint());
	CLog::setMaskFlag(CLog::LT_Warn, g_pCfgJsonRoot->game().common().logwarning());
	CLog::setMaskFlag(CLog::LT_Error, g_pCfgJsonRoot->game().common().logerror());

	m_clGlobalPacket.bindConnect(&m_clGlobalConnect);
	m_clGlobalConnect.bindPacket(&m_clGlobalPacket);
	m_clGlobalConnect.bindLogicLock(getRootLock());
	m_clGlobalConnect.selfMsgQueue();
	m_clConnectChecker.add_test(&m_clGlobalConnect,m_stGlobalNet.uIP,m_stGlobalNet.uPort);

	m_clWorldPacket.bindConnect(&m_clWorldConnect);
	m_clWorldConnect.bindPacket(&m_clWorldPacket);
	m_clWorldConnect.bindLogicLock(getRootLock());
	m_clWorldConnect.selfMsgQueue();
	m_clConnectChecker.add_test(&m_clWorldConnect,m_stWorldNet.uIP,m_stWorldNet.uPort);


	m_clClientNetwork.setMainMutex(g_pServer->getRootLock());
	m_clClientNetwork.selfMsgQueue();

	//设置超时参数
	m_clClientNetwork.set_timeout(m_s_NetTimeout.uConnect
								,m_s_NetTimeout.uRead
								,m_s_NetTimeout.uWrite);
	m_clClientNetwork.setSendBufferMax(10);
	//m_clClientNetwork.set_time_write(30);

	m_clWorldConnect.set_timeout(m_c_NetTimeout.uConnect
								,m_c_NetTimeout.uRead
								,m_c_NetTimeout.uWrite);
	m_clGlobalConnect.set_timeout(m_c_NetTimeout.uConnect
								,m_c_NetTimeout.uRead
								,m_c_NetTimeout.uWrite);

	m_clTimer.add_object(g_pGameLogin			,200);
	m_clTimer.add_object(g_pGameSave			,200);
	m_clTimer.add_object(g_pInteractionMgr		,1*1000);
	m_clTimer.add_object(g_pGamePlayerMgr		,1000);
	m_clTimer.add_object(&m_clConnectChecker	,1*1000);
	m_clTimer.add_object(g_pGameSceneMgr		,33);

	return true;
}

//-------------------------------------------------------------
//------------------------------ 启动服务
bool	CGameServer::startServices()
{
	//if (g_pThreadLog->startLogThread(3) == false)
	//{
	//	CLog::error("日志线程启动失败!...");
	//	return false;
	//}
	
	if ( !g_pGameSceneMgr->start() )
	{
		CLog::error("场景加载失败!...");
		return false;
	}

	if (!m_clWorldConnect.connect(m_stWorldNet.getPort(),(ulong)m_stWorldNet.uIP))
	{
		Parent::ping(GS_WorldConnect);
		CLog::error("世界服务器新连接失败!...");
	}

	Parent::startServices();

	if ( !startClientNetwork() )
		return false;

	showToConsole("<Begin Game Service...>");

	operators::set_server_info(getServerType(),getServerID());
	if(!operators::start(getRootLock()))
	{
		CLog::error("operators::start_operators()!...");
		return false;
	}

	//////要在读取global数据以后才启动
	//g_pWorldBossMgr->initTimer();

	return true;
}

//-------------------------------------------------------------
//------------------------------ 停止服务
void	CGameServer::stopServices()
{
	//g_pThreadLog->stopLogThread();

	CLog::warn("停止服务");

	m_clConnectChecker.shutdown();
	m_clClientNetwork.shutdown();

	m_clGlobalPacket.send_Disconnection();
	m_clGlobalConnect.shutdown();

	m_clWorldPacket.send_Disconnection();
	m_clWorldConnect.shutdown();

	Parent::stopServices();
	instance_del();
}

//-------------------------------------------------------------
//------------------------------ 拒绝服务
void	CGameServer::denyServices()
{
	showToConsole("<Deny user service ...>");

	m_clClientNetwork.rejectConnect(true);
	CLog::warn("开始拒绝世界网络链接...");

	m_clClientNetwork.rejectRead();
	m_clClientNetwork.disconnectAll();

	g_pGamePlayerMgr->shutdown();

	showToConsole("<Waiting for Queue process ...>");
}
//-------------------------------------------------------------
//------------------------------ 能否退出
bool	CGameServer::canExitServices	()
{
	if(!g_pGamePlayerMgr->empty())
	{
		CLog::warn("等待角色下线...");
		return false;
	}

	if(!g_pGameSave->isEmpty())
	{
		CLog::warn("正在保存角色数据...");
		return false;
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------ 是否可以启动
bool	CGameServer::isCanStart()
{
	if(!m_clWorldConnect.connected())
	{
		Parent::ping(RS_Loading);
		CLog::error("世界服务器新连接失败!...");
		return false;
	}

	if(!m_clGlobalConnect.connected())
	{
		Parent::ping(RS_Loading);
		CLog::error("全局服务器新连接失败!...");
		return false;
	}

	Parent::ping(RS_Loading,false);
	return true;
}

//-------------------------------------------------------------
//------------------------------ 读取配置
bool	CGameServer::loadConfig(const char* szConfigFile)
{
	if(!Parent::loadConfig(szConfigFile))
		return false;

	std::string jsonPath = m_sConfigPath + "/cfg_root.json";
	CHECKF(g_pCfgJsonRoot->load(jsonPath));
	setLogicWorldID(g_pCfgJsonRoot->game().common().logicworldid());

	//前端网络服务
	{
		const PBCfg_Net &net = g_pCfgJsonRoot->game().common().clientnet();
		m_stHostClient.setHost(net.ip().c_str());
		m_stHostClient.setPort(net.port() + getServerID());
	}

	if ( isBigWorldServer() )
	{
		const PBCfg_Net &net = g_pCfgJsonRoot->rooter().common().servicenet();
		{
			m_stWorldNet.setIP(net.ip().c_str());
			m_stWorldNet.setPort(net.port() + getServerID()*2);

			m_stGlobalNet.setIP(net.ip().c_str());
			m_stGlobalNet.setPort(net.port() + getServerID()*2 + 1);
		}
	}
	else
	{
		//世界服务器链接
		pb2_stNetAddress(m_stWorldNet,	g_pCfgJsonRoot->world().common().servicenet());

		//全局服务器
		pb2_stNetAddress(m_stGlobalNet,	g_pCfgJsonRoot->global().common().servicenet());
	}


	if (operators::is_operators_type(operators::_operators_open_qq))
	{
		//g_data_collector	= _single_instance_new(data_collector);
		//if(!g_data_collector)
		//	return false;

		//g_data_collector->init(ServerType_Game,open_qq::get_appid(),g_pServer->m_stConfig.getIP());
	}

	{
		g_game_log->init_log(getServerID());
		const PBCfg_Gamelog &gameLog = g_pCfgJsonRoot->game().common().gamelog();
		if(!g_game_log->load_config(gameLog.sendgamelog(),
			gameLog.ip(),
			gameLog.port()))
		{
			return false;
		}
	}

	//上报
	g_report_log->load_config(g_pCfgJsonRoot->game().common().reportlog());

	//读取网络链接时间
	pb2_stNetTimeout(m_c_NetTimeout, g_pCfgJsonRoot->game().common().connectnet());

	//读取网络服务时间
	pb2_stNetTimeout(m_s_NetTimeout, g_pCfgJsonRoot->game().common().clientnet());

	m_stConfig.uIP		= CSocket::get_host_address();

	// 读取游戏开关配置
	{
		const PBCfg_Switch &switch_ = g_pCfgJsonRoot->game().switch_();
		m_stGameCfg.setSwitch(GameSwitch_ClientGM,	switch_.gm());
		m_stGameCfg.setSwitch(GameSwitch_Adult,		switch_.adult());
		m_stGameCfg.setSwitch(GameSwitch_DebugMsg,	switch_.debugmsg());
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 显示服务信息
void	CGameServer::showServiceInfo()
{
	showToConsole("***************************************************************");
	showToConsole("* Game Server Application Ver %d.%d",0,0);
	showToConsole("* Logic World ID      : %.4d  ",getLogicWorldID());
	showToConsole("* Game Server ID      : %.4d  ",getServerID());
	showToConsole("* Network:");
	showToConsole("* ->Client Timeout	 : connect=%ds read=%ds write=%ds", m_s_NetTimeout.uConnect, m_s_NetTimeout.uRead, m_s_NetTimeout.uWrite);
	showToConsole("* ->Client NetService : %s:%d ",m_stHostClient.getHost(),m_stHostClient.getPort());
	showToConsole("* Connected to:");
	showToConsole("* ->Connect Timeout	 : connect=%ds read=%ds write=%ds", m_c_NetTimeout.uConnect, m_c_NetTimeout.uRead, m_c_NetTimeout.uWrite);
	showToConsole("* ->Connect World	 : %s:%d ",m_stWorldNet.getIP(),m_stWorldNet.getPort());
	showToConsole("* ->Connect Global	 : %s:%d ",m_stGlobalNet.getIP(),m_stGlobalNet.getPort());
	showToConsole("***************************************************************");

	setServicesTitle("Game:<%.4d><%.4d>",getLogicWorldID(),getServerID());
}

//-------------------------------------------------------------
//------------------------------ 服务处理
void	CGameServer::timerProcess		()
{
	uint64 uClock = getMillisecond();
	//CLog::print("tick clock=%ld", uClock);
	behaviac::Workspace::GetInstance()->SetTimeSinceStartup((float)(uClock / 1000.0));
	Parent::timerProcess();
	{//处理世界服务器消息
		m_clWorldConnect.processMsgQueue();
	}
	{//处理全局服务器消息
		m_clGlobalConnect.processMsgQueue();
	}

	{//处理客户端消息
		m_clClientNetwork.processMsgQueue();
	}
}

//-------------------------------------------------------------
//------------------------------ 
bool	CGameServer::isLoaded()
{
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameServer::instance_new	()
{
	g_game_log = _single_instance_new(game_log);
	g_report_log = _single_instance_new(report_log);
	g_pInteractionMgr = _single_instance_new(CInteractionMgr);
	g_pGameLogin = _single_instance_new(CGameLogin);
	g_pGameSave = _single_instance_new(CGameSave);
	g_pGamePlayerMgr = _single_instance_new(CGamePlayerMgr);
	g_pGameSceneMgr = _single_instance_new(CGameSceneMgr);
	g_pCopymapMgr = _single_instance_new(CCopymapMgr);
	g_pGameBroadcast = _single_instance_new(CGameBroadcast);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameServer::instance_del	()
{
	//_single_instance_del(g_pWorldBossMgr);
	//_single_instance_del(g_data_collector);
	_single_instance_del(g_game_log);
	_single_instance_del(g_report_log);
	_single_instance_del(g_pInteractionMgr);

	//------------------------------ 
	_single_instance_del(g_pGameLogin);
	_single_instance_del(g_pGameSave);
	_single_instance_del(g_pGamePlayerMgr);
	_single_instance_del(g_pGameSceneMgr);
	_single_instance_del(g_pCopymapMgr);
	_single_instance_del(g_pGameBroadcast);
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameServer::loadData		()
{
	//加载资源配置路径
	static bool b_LoadPath = false;
	if(!b_LoadPath)
	{
		b_LoadPath = true;
		//资源配置信息
		g_clLoadData.load_from_pb(g_pCfgJsonRoot->game().common().res());
		AIRegister::registerAI();
	}

	return reLoadData();
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameServer::reLoadData		()
{
	CLoadData::_stLoadMark	g_stReloadMark;
	g_stReloadMark.setMark(CLoadData::_D_Battle			,true);
	g_stReloadMark.setMark(CLoadData::_D_Trap			,true);
	g_stReloadMark.setMark(CLoadData::_D_Copymap		,true);
	g_stReloadMark.setMark(CLoadData::_D_Scene			,true);
	g_stReloadMark.setMark(CLoadData::_D_Birth			,true);
	g_stReloadMark.setMark(CLoadData::_D_Constant		,true);
	g_stReloadMark.setMark(CLoadData::_D_Skill			,true);
	g_stReloadMark.setMark(CLoadData::_D_Buff			,true);
	g_stReloadMark.setMark(CLoadData::_D_Player			,true);
	g_stReloadMark.setMark(CLoadData::_D_Monster		,true);
	g_stReloadMark.setMark(CLoadData::_D_Item			,true);
	g_stReloadMark.setMark(CLoadData::_D_Expend			,true);
	g_stReloadMark.setMark(CLoadData::_D_Job			,true);
	g_stReloadMark.setMark(CLoadData::_D_Quest			,true);
	g_stReloadMark.setMark(CLoadData::_D_Prize			,true);
	g_stReloadMark.setMark(CLoadData::_D_Camp			,true);
	g_stReloadMark.setMark(CLoadData::_D_Store			,true);
	g_stReloadMark.setMark(CLoadData::_D_Drop			,true);
	g_stReloadMark.setMark(CLoadData::_D_Bingfu			,true);
	g_stReloadMark.setMark(CLoadData::_D_Wulue			,true);
	g_stReloadMark.setMark(CLoadData::_D_WenTao			,true);
	g_stReloadMark.setMark(CLoadData::_D_Equip			,true);
	g_stReloadMark.setMark(CLoadData::_D_Identity		,true);
	g_stReloadMark.setMark(CLoadData::_D_GM				,true);
	g_stReloadMark.setMark(CLoadData::_D_FLY			,true);
	g_stReloadMark.setMark(CLoadData::_D_PROMOTION		,true);
	g_stReloadMark.setMark(CLoadData::_D_SystemSwitch   ,true);
	g_stReloadMark.setMark(CLoadData::_D_Faction		,true);
	g_stReloadMark.setMark(CLoadData::_D_Wuxue			,true);
	g_stReloadMark.setMark(CLoadData::_D_IdentityQuest	,true);
	g_stReloadMark.setMark(CLoadData::_D_Dart			,true);
	g_stReloadMark.setMark(CLoadData::_D_AI				,true);
	g_stReloadMark.setMark(CLoadData::_D_TeamTarget		,true);
	g_stReloadMark.setMark(CLoadData::_D_Title			,true);
	g_stReloadMark.setMark(CLoadData::_D_Dazuoguaji		,true);
	g_stReloadMark.setMark(CLoadData::_D_Fashion		,true);
	g_stReloadMark.setMark(CLoadData::_D_TeamTask		,true);
	g_stReloadMark.setMark(CLoadData::_D_ClimbTower		,true);
	g_stReloadMark.setMark(CLoadData::_D_Pvp			,true);
	g_stReloadMark.setMark(CLoadData::_D_GwTrain		,true);
	g_stReloadMark.setMark(CLoadData::_D_TreasureMap	,true);
	g_stReloadMark.setMark(CLoadData::_D_WorldBoss		,true);
	g_stReloadMark.setMark(CLoadData::_D_Activity		,true);
	g_stReloadMark.setMark(CLoadData::_D_Relive			,true);
	g_stReloadMark.setMark(CLoadData::_D_TopList		,true);
	g_stReloadMark.setMark(CLoadData::_D_Corps			,true);
	g_stReloadMark.setMark(CLoadData::_D_Weal			,true);
	g_stReloadMark.setMark(CLoadData::_D_Manor			,true);
	g_stReloadMark.setMark(CLoadData::_D_Stronger		,true);
	if(!g_clLoadData.reLoadFile(g_stReloadMark))
		return false;

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameServer::loadScript		()
{
	return reLoadLuaScript(m_luaState,g_clLoadData.buld_script_path("").c_str());
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameServer::startClientNetwork	()
{
	/*启动客户端网络服务*/ 
	if (!m_clClientNetwork.start(m_stHostClient.getPort()))
	{
		Parent::ping(GS_Port);
		CLog::error("客户网络服务启动失败!...");
		return false;
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
uint32	CGameServer::getWorldStartTime	(uint16 uLogicWorldID)
{
	return m_mapWorldStartTime[uLogicWorldID];
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameServer::setWorldStartTime	(uint16 uLogicWorldID,uint32 uTime)
{
	m_mapWorldStartTime[uLogicWorldID] = uTime;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameServer::isWorldConneted	(uint16 uLogicWorldID)
{
	if (!uLogicWorldID)
		return false;

	if ( m_clWorldConnect.disconnected() )
		return false;

	if (m_setRegisterWorld.find(uLogicWorldID) == m_setRegisterWorld.end() )
		return false;

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameServer::sendMsgToWorld		(uint16 uLogicWorldID,uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent)
{
	if ( !isWorldConneted(uLogicWorldID) )
		return false;

	_stSvrInfo stToServer;
	stToServer.uLogicWorldID	= uLogicWorldID;
	stToServer.uServerType		= ServerType_World;
	return m_clWorldPacket.sendMsg(this->getSvrInfo(),stToServer,uMainProtocol,uProtocol,msg,uEvent);
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameServer::sendBufferToWorld	(uint16 uLogicWorldID,uint8 uMainProtocol, uint8 uProtocol, void* pBuffer,uint32 uSize, uint8 uEvent)
{
	if ( !isWorldConneted(uLogicWorldID) )
		return false;

	_stSvrInfo stToServer;
	stToServer.uLogicWorldID	= uLogicWorldID;
	stToServer.uServerType		= ServerType_World;
	return m_clWorldPacket.sendBuffer(this->getSvrInfo(),stToServer,uMainProtocol,uProtocol,pBuffer,uSize,uEvent);
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameServer::isGlobalConneted	(uint16 uLogicWorldID)
{
	if ( m_clGlobalConnect.disconnected() )
		return false;

	if ( uLogicWorldID && m_setRegisterGlobal.find(uLogicWorldID) == m_setRegisterGlobal.end() )
		return false;

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameServer::sendMsgToGlobal		(uint16 uLogicWorldID,uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent)
{
	if ( !isGlobalConneted(uLogicWorldID) )
		return false;

	_stSvrInfo stToServer;
	stToServer.uLogicWorldID	= uLogicWorldID;
	stToServer.uServerType		= ServerType_Global;
	return m_clGlobalPacket.sendMsg(this->getSvrInfo(),stToServer,uMainProtocol,uProtocol,msg,uEvent);
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameServer::sendBufferToGlobal	(uint16 uLogicWorldID,uint8 uMainProtocol, uint8 uProtocol, void* pBuffer,uint32 uSize, uint8 uEvent)
{
	if ( !isGlobalConneted(uLogicWorldID) )
		return false;

	_stSvrInfo stToServer;
	stToServer.uLogicWorldID	= uLogicWorldID;
	stToServer.uServerType		= ServerType_Global;
	return m_clGlobalPacket.sendBuffer(this->getSvrInfo(),stToServer,uMainProtocol,uProtocol,pBuffer,uSize,uEvent);
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameServer::registerWorld		(uint16 uLogicWorldID)
{
	m_setRegisterWorld.insert(uLogicWorldID);
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameServer::registerGlobal		(uint16 uLogicWorldID)
{
	m_setRegisterGlobal.insert(uLogicWorldID);
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameServer::unregisterWorld		(uint16 uLogicWorldID)
{
	m_setRegisterWorld.erase(uLogicWorldID);

	//通知该worldid的玩家下线
	g_pGamePlayerMgr->logoutWorldPlayer(uLogicWorldID);
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameServer::unregisterGlobal		(uint16 uLogicWorldID)
{
	m_setRegisterGlobal.erase(uLogicWorldID);
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameServer::unregisterAllWorld		()
{
	m_setRegisterWorld.clear();
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameServer::unregisterAllGlobal		()
{
	m_setRegisterGlobal.clear();
}
