/*------------- gameServer.h
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
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
	_stGameConfig			m_stGameCfg;		//��Ϸ����
	_stDebugMsgConfig		m_stDebugMsgCfg;	//Э���ӡ����
	_stNetAddress			m_stConfig;			//����������(��Կͻ�����������)
	_stNetHost				m_stHostClient;
	_stNetAddress			m_stGlobalNet;		//ȫ�ַ���������
	_stNetAddress			m_stWorldNet;		//�������������
	_stNetTimeout			m_c_NetTimeout;
	_stNetTimeout			m_s_NetTimeout;

public:
	CClientNetwork			m_clClientNetwork;	/*ClientͨѶ����*/ 
	CNetConnect				m_clGlobalConnect;	/*ȫ�ַ���������*/ 
	CNetConnect				m_clWorldConnect;	/*�������������*/ 
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
	/*----->{ ��ʼ�� }*/ 
	bool			initialize			(uint16 uServerID,const char* szConfigFile);
	/*----->{ �������� }*/ 
	bool			startServices		();	
	/*----->{ ֹͣ���� }*/ 
	void			stopServices		();
	
protected:
	/*----->{ �ܾ����� }*/ 
	virtual void	denyServices		();
	//--- �ܷ��˳�
	virtual bool	canExitServices		();

private:
	/*----->{ �Ƿ�������� }*/ 
	bool			isCanStart			();
	/*----->{ �ӱ��������ļ�Config.ini��ȡ���� }*/ 
	bool			loadConfig			(const char* szConfigFile);

public:
	//--- װ������
	bool			loadData			();
	//--- װ������
	bool			reLoadData			();
	//--- װ�ؽű�
	bool			loadScript			();
private:
	/*----->{ ��ʾ������Ϣ }*/ 
	void			showServiceInfo		();
	/*----->{ ������ }*/ 
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
	// ��ȫ�ַ�����������Ϣ
	bool			isGlobalConneted	(uint16 uLogicWorldID);
	bool			sendMsgToGlobal		(uint16 uLogicWorldID,uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent = 0);
	bool			sendBufferToGlobal	(uint16 uLogicWorldID,uint8 uMainProtocol, uint8 uProtocol, void* pBuffer,uint32 uSize, uint8 uEvent = 0);

	// �����������������Ϣ
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

