/*----------------- report_log.cpp
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2013/3/21 18:49:19
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "report_log.h"
#include "basic/timeFunction.h"
#include "def_struct/cfgJson.h"
#include "protocol/PBConfig.pb.h"
/*************************************************************/
report_log*	g_report_log	= NULL;
//-------------------------------------------------------------
//------------------------------ 
report_log::report_log()
{
	m_is_report		= false;
	m_net_port		= 0;
	m_separate		= '|';
	memset(m_net_addr,0,sizeof(m_net_addr));

	Parent::set_separate(m_separate);
}
//-------------------------------------------------------------
//------------------------------ 
report_log::~report_log()
{

}

//-------------------------------------------------------------
//------------------------------ 
bool	report_log::load_config(const PBCfg_Reportlog &cfgLog)
{
	char sep = '\t';
	if (cfgLog.sep().length() > 0)
		sep = cfgLog.sep().at(0);
	set_report(	cfgLog.sendloadlog(), 
				cfgLog.ip().c_str(),
				cfgLog.port(),
				sep);
	//�������̷߳�������udp�ϱ��ӿ�
	m_network.createThread(false);
	if(m_is_report && !m_network.start(0))
		return false;

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
void	report_log::set_report(bool _report, pc_str _addr, uint16 _port, char _separate)
{
	m_is_report = _report;
	m_net_port	= _port;
	m_separate	= (_separate != '\0') ? _separate : '\t';
	dSprintf(m_net_addr,sizeof(m_net_addr),"%s",_addr ? _addr : "");

	if(m_is_report && (!m_net_addr[0] || !m_net_port))
		m_is_report = false;

	Parent::set_separate(m_separate);
}

//-------------------------------------------------------------
//------------------------------ ����
void	report_log::sendTo			(const void*pBuffer,uint32 uSize)
{
	if(m_is_report)
		m_network.sendTo(pBuffer,uSize,m_net_addr,m_net_port);
}
//
////-------------------------------------------------------------
////------------------------------ 
//bool	report_log::report_player	(_stAccountFlag&_account,_stGamePlayer&_player,uint16 _mount,uint16 _bow,uint16 _riding_weapon)
//{
//	if(!m_is_report || !_player.uPlayerID || !_player.uIpVipID)
//		return true;
//
//	CBufferStream clStream(m_buffer,sizeof(m_buffer));
//
//	//��Ҫ���˽�ɫ����"|"�ַ�
//	char*pName = _convertUTF8toANSI(_player.szPlayerName,Player_Name_Max);
//	if(pName)
//		wipeOffChar(pName,dStrlen(pName),m_separate);
//
//	//����	��ɫid	����id	�ʺ�ID	�ʺ���	��ɫ��	ע��ʱ��	����ʱ��	��ɫ�ȼ�	������	����ID	����ID	��ս����ID
//	clStream.fprintf("report_player%c"	,m_separate						);
//	clStream.fprintf("%u%c"	,_player.uPlayerID							,m_separate	);
//	clStream.fprintf("%u%c"	,_player.uIpVipID							,m_separate	);
//	clStream.fprintf("%u%c"	,_player.uAccountID							,m_separate	);
//	clStream.fprintf("%s%c"	,_account.szAccount							,m_separate	);
//	clStream.fprintf("%s%c"	,_convertANSItoUTF8(pName,Player_Name_Max)	,m_separate	);
//	clStream.fprintf("%s%c"	,time_format(_player.uBirthTime)			,m_separate	);
//	clStream.fprintf("%s%c"	,time_format(getTime())						,m_separate	);
//	clStream.fprintf("%u%c"	,_player.uLevel								,m_separate	);
//	clStream.fprintf("%u%c"	,_player.uJob								,m_separate	);
//	clStream.fprintf("%u%c"	,_player.uTotalConsume						,m_separate	);
//
//	//��չ����
//	clStream.fprintf("%u%c"	,_mount										,m_separate	);
//	clStream.fprintf("%u%c"	,_bow										,m_separate	);
//	clStream.fprintf("%u%c"	,_riding_weapon								,m_separate	);
//	clStream.fprintf("%u"	,_player.uWorldID										);
//
//	clStream.fprintf("\n");
//
//	sendTo(m_buffer,clStream.getPosition());
//	return true;
//}
