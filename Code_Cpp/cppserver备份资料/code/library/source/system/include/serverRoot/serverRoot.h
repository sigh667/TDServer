/*------------- serverRoot.h
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.01
* Date         : 2011/3/24 9:08:04
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include <signal.h>
#include "listenState.h"
#include "timer/timers.h"
#include "threads/thread.h"
#include "threads/condEvent.h"
#include "console/luaConsole.h"
#include "serverRoot/server_define.h"
#include "network/netStruct.h"
/*************************************************************/
const int PING_EVENT_TIME	= 3;//ping事件时间(秒)
/*************************************************************/
class CServerRoot	: public CLuaConsole
					, public CListenState
					, public CTimerObject
{
public:/*Ctrl+C退出事件*/ 
	static CCondEvent	m_clExitEvent;

private:
	volatile bool		m_bExitServices;	//退出变量(收到退出消息)

protected:;
	CTimer				m_clTimer;	
	CCondEvent			m_clMaintainEvent;	//保持服务事件

protected:
	uint64				m_uPingTime;		//ping时间

protected:
	bool				m_bInitFlag;
	uint32				m_uLoadFlag;
	uint64				m_tmStartTime;
	char				m_szTitle[256];
	std::string			m_sConfigPath;		//配置文件存放目录
 protected:
	_stSvrInfo			m_stSvrInfo;

public:
	CServerRoot();
	virtual ~CServerRoot();

public:
	/*----->{ 服务器ID }*/ 
	inline void			setServerType		(uint8 uType)					{	m_stSvrInfo.uServerType = uType;				}
	inline uint8		getServerType		()const							{	return m_stSvrInfo.uServerType;					}
	inline void			setServerID			(uint16 uServerID)				{	m_stSvrInfo.uServerID	= uServerID;			}
	inline uint16		getServerID			()const							{	return m_stSvrInfo.uServerID;					}
	inline void			setLogicWorldID		(uint16 uLogicWorldID)			{	m_stSvrInfo.uLogicWorldID	= uLogicWorldID;	}
	inline uint16		getLogicWorldID		()const							{	return m_stSvrInfo.uLogicWorldID;				}
	inline CMutex_*		getRootLock			()								{	return m_clTimer.get_timer_locket();			}
	inline  _stSvrInfo&	getSvrInfo			()								{	return m_stSvrInfo;								}
	inline  bool		isBigWorldServer	()								{	return getLogicWorldID() == BIG_WORLD_LOGIC_ID;	}
public:
	/*----->{ 初始化 }*/ 
	virtual bool	initialize		(uint16 uServerID,const char* szConfigPath );
	/*----->{ 启动服务 }*/ 
	virtual bool	startServices	();	
	/*----->{ 保持服务 }*/ 
	virtual void	maintenances	();
	/*----->{ 停止服务 }*/ 
	virtual void	stopServices	();

protected:
	/*----->{ 拒绝服务 }*/ 
	virtual void	denyServices	();	
	//--- 能否退出
	virtual bool	canExitServices	();

private:
	/*----->{ 初始化事件 }*/ 
	bool	initEvent				();
	/*----->{ 设置中断处理 }*/ 
	void	installBreakHandlers	();

protected:
	/*----->{ 是否可以启动 }*/ 
	virtual bool	isCanStart		();
	/*----->{ 传入配置文件所在目录 }*/ 
	virtual bool	loadConfig		(const char* szConfigPath);

public:
	//--- 装载游戏数据
	virtual bool	loadData		();

	//--- 装载脚本数据
	virtual bool	loadScript		();
protected:
	/*----->{ 显示服务信息 }*/ 
	virtual void	showServiceInfo();
	//--- 
	virtual void	timerProcess	();
	//--- 主线程处理(每秒一次)
	virtual void	mainProcess		();

public:
	/*----->{ 设置服务器标题 }*/ 
	virtual void	setServicesTitle(const char* pTitle,...);
	virtual void	showToConsole	(const char* pFormat,...);

public:
	static void		messageBoxOK	(const char* pCaption,const char* pText,...);
public:
	//----  提供一个注册timer的接口
	bool	addTimer(CTimerObject *,long , bool );
	void	delTimer(CTimerObject *);


};
