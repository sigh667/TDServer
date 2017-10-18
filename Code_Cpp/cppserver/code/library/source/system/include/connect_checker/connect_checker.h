/*----------------- connect_checker.h
*
* Copyright (C): 2011  Mokylin°§Mokyqi
* Author       : ’‘Œƒ‘¥
* Version      : V1.0
* Date         : 2012/7/13 20:01:36
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "network/include/network.h"
#include "network/netStruct.h"
#include "timer/timers.h"
#include "singleton/singleton.h"
/*************************************************************/
class connect_checker	: public CTimerObject
{
protected:
	typedef stl_map<tcp_connect*,_stNetAddress>CONNECT_LIST;

protected:
	bool			m_bShutdown;
	CONNECT_LIST	m_listConnect;

public:
	connect_checker();

public:
	bool	add_test	(tcp_connect*_connect,uint64 _ip,uint16 _port);
	void	del_test	(tcp_connect*_connect);
	void	shutdown	();

public:
	void	timerProcess();
};
//-------------------------------------------------------------
//------------------------------ 
class CConnectChecker	: public connect_checker
						, _single_public(CConnectChecker)
{
private:
	_single_friend(CConnectChecker);
};
/*************************************************************/
extern CConnectChecker* g_pConnectChecker;
