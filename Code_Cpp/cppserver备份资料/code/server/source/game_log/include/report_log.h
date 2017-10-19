/*----------------- report_log.h
*
* Copyright (C): 2011  Mokylin°§Mokyqi
* Author       : ’‘Œƒ‘¥
* Version      : V1.0
* Date         : 2013/3/21 18:48:27
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "network/include/network.h"
#include "basic/basicTypes.h"
#include "timer/timers.h"
#include "singleton/singleton.h"
#include "stream/bufferStream.h"
#include "game_report/game_report.h"
/*************************************************************/
class PBCfg_Reportlog;
class report_log	: public game_report
					, _single_public(report_log)
{
private:
	_single_friend(report_log);

public:
	typedef game_report Parent;

protected:
	bool			m_is_report;
	char			m_net_addr[1024];
	uint16			m_net_port;
	char			m_separate;

protected:
	char			m_buffer[1024*8];
	CUDPNetwork		m_network;

private:
	report_log();
	virtual~report_log();

public:
	virtual bool	load_config		(const PBCfg_Reportlog &cfgLog);

public:
	virtual void	set_report		(bool _report, pc_str _addr, uint16 _port, char _separate);

protected:
	//--- ∑¢ÀÕ
	virtual	void	sendTo			(const void*pBuffer,uint32 uSize);

public:
	//virtual bool	report_player	(_stAccountFlag&_account,_stGamePlayer&_player,uint16 _mount,uint16 _bow,uint16 _riding_weapon);
};
/*************************************************************/
extern report_log*	g_report_log;
