/*------------- serverRoot.h
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
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
const int PING_EVENT_TIME	= 3;//ping�¼�ʱ��(��)
/*************************************************************/
class CServerRoot	: public CLuaConsole
					, public CListenState
					, public CTimerObject
{
public:/*Ctrl+C�˳��¼�*/ 
	static CCondEvent	m_clExitEvent;

private:
	volatile bool		m_bExitServices;	//�˳�����(�յ��˳���Ϣ)

protected:;
	CTimer				m_clTimer;	
	CCondEvent			m_clMaintainEvent;	//���ַ����¼�

protected:
	uint64				m_uPingTime;		//pingʱ��

protected:
	bool				m_bInitFlag;
	uint32				m_uLoadFlag;
	uint64				m_tmStartTime;
	char				m_szTitle[256];
	std::string			m_sConfigPath;		//�����ļ����Ŀ¼
 protected:
	_stSvrInfo			m_stSvrInfo;

public:
	CServerRoot();
	virtual ~CServerRoot();

public:
	/*----->{ ������ID }*/ 
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
	/*----->{ ��ʼ�� }*/ 
	virtual bool	initialize		(uint16 uServerID,const char* szConfigPath );
	/*----->{ �������� }*/ 
	virtual bool	startServices	();	
	/*----->{ ���ַ��� }*/ 
	virtual void	maintenances	();
	/*----->{ ֹͣ���� }*/ 
	virtual void	stopServices	();

protected:
	/*----->{ �ܾ����� }*/ 
	virtual void	denyServices	();	
	//--- �ܷ��˳�
	virtual bool	canExitServices	();

private:
	/*----->{ ��ʼ���¼� }*/ 
	bool	initEvent				();
	/*----->{ �����жϴ��� }*/ 
	void	installBreakHandlers	();

protected:
	/*----->{ �Ƿ�������� }*/ 
	virtual bool	isCanStart		();
	/*----->{ ���������ļ�����Ŀ¼ }*/ 
	virtual bool	loadConfig		(const char* szConfigPath);

public:
	//--- װ����Ϸ����
	virtual bool	loadData		();

	//--- װ�ؽű�����
	virtual bool	loadScript		();
protected:
	/*----->{ ��ʾ������Ϣ }*/ 
	virtual void	showServiceInfo();
	//--- 
	virtual void	timerProcess	();
	//--- ���̴߳���(ÿ��һ��)
	virtual void	mainProcess		();

public:
	/*----->{ ���÷��������� }*/ 
	virtual void	setServicesTitle(const char* pTitle,...);
	virtual void	showToConsole	(const char* pFormat,...);

public:
	static void		messageBoxOK	(const char* pCaption,const char* pText,...);
public:
	//----  �ṩһ��ע��timer�Ľӿ�
	bool	addTimer(CTimerObject *,long , bool );
	void	delTimer(CTimerObject *);


};
