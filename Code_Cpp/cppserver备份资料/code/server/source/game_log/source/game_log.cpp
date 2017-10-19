/*----------------- game_log.cpp
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/7/9 16:23:24
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "serverRoot/server_define.h"
#include "game_log.h"
#include "stream/netStream.h"
#include "basic/basicTypes.h"
#include "log/log.h"
#include "protocol/RecordStruct.pb.h"
#include "def_struct/playerStuct.h"
/*************************************************************/
game_log* g_game_log = NULL;
//-------------------------------------------------------------
//------------------------------ 
game_log::game_log()
{
	init_log(0);
}
//-------------------------------------------------------------
//------------------------------ 
game_log::~game_log()
{
}

//-------------------------------------------------------------
//------------------------------
bool	game_log::load_config(bool bReport,
							  const std::string &addr,
							  uint16 uPort)
{
	//日志服务器链接
	{
		setUDPReport(bReport, addr.c_str(), uPort);
	}

	//不创建线程方法启动udp上报接口
	m_clUDPNetwork.createThread(false);
	if(m_bUDPReport && !m_clUDPNetwork.start(0))
		return false;

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
void	game_log::init_log(uint16 uServerID)
{
	m_uDoingTime	= 0;

	m_uInsertID		= 0;
	m_clNumberStream.setFlag(uServerID);

	m_bUDPReport= false;
	m_uNetPort	= 0;
	memset(&m_szNetAddr,0,sizeof(m_szNetAddr));
	memset(&m_szReportBuffer,0,sizeof(m_szReportBuffer));
}

//-------------------------------------------------------------
//------------------------------ 
void	game_log::setUDPReport(bool bReport,pc_str pAddr,uint16 uPort)
{
	m_bUDPReport= bReport;
	m_uNetPort	= uPort;
	dSprintf(m_szNetAddr,sizeof(m_szNetAddr),"%s",pAddr ? pAddr : "");

	if(m_bUDPReport && (!m_szNetAddr[0] || !m_uNetPort))
		m_bUDPReport = false;
}

//-------------------------------------------------------------
//------------------------------ 
bool	game_log::sendTo			(const void*pBuffer,uint32 uSize)
{
	return m_clUDPNetwork.sendTo(m_szReportBuffer,uSize,m_szNetAddr,m_uNetPort);
}

//-------------------------------------------------------------
//------------------------------ 
bool	game_log::sendLog(const google::protobuf::Message &pb)
{
	memset(&m_szReportBuffer,0,sizeof(m_szReportBuffer));
	CNetStream clStream(m_szReportBuffer,sizeof(m_szReportBuffer));
	clStream.write(pb.GetTypeName());
	CHECKF(clStream.SerializeToStream(pb));
	return sendTo(clStream.getBuffer(),clStream.getPosition());
}
//-------------------------------------------------------------
//------------------------------ 
bool	game_log::item_log		(uint32 uPlayerID,_emDoingType emDoingType,_stItem&stItem)
{
	static PBLog_Item pbLog;
	pbLog.Clear();
	PBLog_Common *common = pbLog.mutable_common();
	if (common)
	{
		common->set_doingevent(emDoingType);
		common->set_worldid(2);
		common->set_stamp(getTime());
	}
	pbLog.set_playerid(uPlayerID);
	pbLog.set_itemid(stItem.uItemID);
	pbLog.set_itemnum(stItem.uItemNum);
	pbLog.set_itemsn(stItem.uItemSN);
	return sendLog(pbLog);
}
//-------------------------------------------------------------
//------------------------------ 
bool	game_log::doing_log		(uint16 uWorldID,uint16 uSceneID,uint32 uSceneCell,uint8 uDoingEvent,uint8 uDoingObject,uint32 uPlayerID,uint64 uObjectID,uint32 uLinkID,uint64 uFromValue,uint64 uToValue,int64 iValue)
{
	static PBLog_Doing pbLog;
	pbLog.Clear();
	PBLog_Common *common = pbLog.mutable_common();
	common->set_doingevent(uDoingEvent);
	common->set_worldid(uWorldID);
	common->set_stamp((uint32)getMillisecond());
	pbLog.set_sceneid(uSceneID);
	pbLog.set_scenecell(uSceneCell);
	pbLog.set_playerid(uPlayerID);
	pbLog.set_doingobj(uDoingObject);
	pbLog.set_objectid(uObjectID);
	pbLog.set_linkid(uLinkID);
	pbLog.set_fromvalue(uFromValue);
	pbLog.set_tovalue(uToValue);
	pbLog.set_value(iValue);
	return sendLog(pbLog);
}

bool	game_log::dartcar_log		(uint16 uWorldID,uint16 uSceneID,uint32 uSceneCellID,uint64 uDynID,_emGameSystemType uSystemType, _emDartLogType uDartlogType,uint32 uMemberPlayerID,std::string strMemberName, uint32 uDartID,uint64 uDartGlobalID, uint32 uHonghuo, uint32 uPrizeID, uint32 uPrizeCount)
{
	static PBLog_Dartcar pbLog;
	pbLog.Clear();
	PBLog_Common *common = pbLog.mutable_common();
	if (common)
	{
		common->set_doingevent(uSystemType);
		common->set_worldid(uWorldID);
		common->set_stamp(getTime());
	}
	pbLog.set_event(uDartlogType);
	pbLog.set_liushui(uDartGlobalID);
	pbLog.set_dartcarid(uDartID);
	pbLog.set_honghuo(uHonghuo);
	pbLog.set_playerid(uMemberPlayerID);
	pbLog.set_playername(strMemberName);
	pbLog.set_prizecount(uPrizeCount);
	pbLog.set_prizeid(uPrizeID);
	pbLog.set_scenecellid(uSceneCellID);
	pbLog.set_sceneid(uSceneID);
	pbLog.set_dynid(uDynID);
	return sendLog(pbLog);
}



