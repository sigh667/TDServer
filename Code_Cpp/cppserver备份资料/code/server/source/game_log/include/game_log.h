/*----------------- game_log.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/7/9 16:23:27
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "network/include/network.h"
#include "basic/basicTypes.h"
#include "stream/numberStream.h"
#include "stream/bufferStream.h"
#include "memoryPool/keyDeques.h"
#include "singleton/singleton.h"
#include "protocol/AccountStruct.pb.h"
#include "protocol/PlayerStruct.pb.h"
#include "protocol/GameDefine.pb.h"
/*************************************************************/
struct _stItem;
class game_log	: _single_public(game_log)
{
private:
	_single_friend(game_log);

protected:
	uint64			m_uDoingTime;
	uint64			m_uInsertID;
	CNumberStream	m_clNumberStream;

protected:
	//_stDoingStream	m_stDoingStream;

protected:
	//UDP上报
	bool			m_bUDPReport;
	char			m_szNetAddr[1024];
	uint16			m_uNetPort;
	char			m_szReportBuffer[1024*8];
	CUDPNetwork		m_clUDPNetwork;

public:
	inline uint64	buildDoingID	()				{	return m_clNumberStream.buildStreamNumber();			}
	inline uint64	lastDoingID		()				{	return m_clNumberStream.lastStreamNumber();				}
	inline uint64	buldDoingTime	()				{	m_uDoingTime = (uint64)getTime();return	m_uDoingTime;			}
	inline uint64	lastDoingTime	()				{	if(!m_uDoingTime)buldDoingTime();return m_uDoingTime;	}
	inline void		newDoingID		()				{	buildDoingID();buldDoingTime();							}

	//废弃函数
//public:
//	inline void		setPath			(pc_str pPath)	{	}
//	inline void		setLogDay		(int32 iDay)	{	}
//	inline void		setLogHour		(int32 iHour)	{	}
//	inline void		setLogMinute	(int32 iMinute)	{	}
//	inline void		setSaveFile		(bool _save)	{	}

public:
	explicit game_log();
	virtual ~game_log();

public:
	virtual	bool	load_config				(bool bReport,
											const std::string &addr,
											uint16 uPort);

public:
	void	init_log		(uint16 uServerID);
	void	setUDPReport	(bool bReport,pc_str pAddr,uint16 uPort);
	bool	sendTo			(const void*pBuffer,uint32 uSize);

protected:
	bool	sendLog			(const google::protobuf::Message &pb);

public:
	//道具日志
	bool	item_log		(uint32 uPlayerID,_emDoingType emDoingType,_stItem&stItem);

	//游戏操作日志
public:
	bool	doing_log		(uint16 uWorldID,uint16 uSceneID,uint32 uSceneCell,uint8 uDoingEvent,uint8 uDoingObject,uint32 uPlayerID,uint64 uObjectID,uint32 uLinkID,uint64 uFromValue,uint64 uToValue,int64 iValue);
	//bool	item_log		(uint8 uDoingEvent,uint8 uDoingType,uint32 uPlayerID,_stItem&stItem,);
	//bool	drop_log		(uint64 uDoingID,uint16 uWorldID,uint16	uSceneID,uint8 uSceneCell,uint32 uPlayerID,uint32 uMonsterID,_stItem&stItem,uint64 uDoingTime);
public:
	bool	dartcar_log		(uint16 uWorldID,uint16 uSceneID,uint32 uSceneCellID,uint64 uDynID,_emGameSystemType uSystemType, _emDartLogType uDartlogType,uint32 uMemberPlayerID,std::string strMemberName, uint32 uDartID, uint64 uDartGlobalID, uint32 uHonghuo, uint32 uPrizeID, uint32 uPrizeCount);

	//消费相关日志
public:
// 	bool	recharge_log	(_stRecharge&stRecharge);
// 	bool	consume_log		(uint32 uPlayerID,uint32 _ip_vip_id,pc_str pContractID,const _stPFInfo&_pf_info,uint8 uLevels,open_qq::_stCallbackPay&stCallback);
// 	bool	consume_log		(uint32 uPlayerID,uint32 _ip_vip_id,const _stPFInfo&_pf_info,uint8 uLevels,open_qq::_stCallbackTask&stCallback);
// 	bool	consume_log		(uint32 uPlayerID,uint32 _ip_vip_id,const _stPFInfo&_pf_info,uint8 uLevels,open_qq::_stCallbackToken&stCallback);


};
/*************************************************************/
extern game_log* g_game_log;
