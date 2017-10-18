/*----------------- playerCommon.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2016/1/8 15:44:47
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "gameLogic/gamePlayerFun/gamePlayerFun.h"
#include "gameLogic/clientGMCommand/clientGMCommand.h"
class CCooling;
struct _stGameAttribute;
/*************************************************************/
class CPlayerCommon	: public CGamePlayerFun
{
private:
	PBClientData		m_pbClientData;
	CClientGMCommand	m_clGMCommand;
	uint32				m_uDailyDazuoguajiaExp; //每日打坐挂机获得的经验总量
	uint32				m_uDailyDazuoguajiaWisdom; //每日打坐挂机获得的韬略总量

	stl_set<uint32>		m_uStrongerFightID;		// 变强宝箱ID

public:
	CPlayerCommon(CScenePlayer& clPlayer);
	virtual~CPlayerCommon();
public:
	virtual	void	initialize				();
	virtual	bool	loadFromProtobuf		(const PBPlayerData&pbPlayer);
	virtual	bool	saveToProtobuf			(PBPlayerData&pbPlayer);
	virtual	_emGameSystemType	getFunType	()const					{	return GameSystemType_Common ; }
	virtual	void	onLogin					();
	virtual	void	onBuildComplete			();
	virtual void	cleanupDaily			();

public:
	CClientGMCommand&	getClientGMCommand	()						{	return m_clGMCommand ;			}
public:
	bool			calculateAttri			();
public:
	bool			handle_go				(uint8 uEventID,CNetStream& clStream);
	bool			handle_client_save		(uint8 uEventID,CNetStream& clStream);
	bool			handle_move				(uint8 uEventID,CNetStream& clStream);
	bool			handle_move_stop		(uint8 uEventID,CNetStream& clStream);
	bool			handle_turn				(uint8 uEventID,CNetStream& clStream);
	bool			handle_gm_cmd			(uint8 uEventID,CNetStream& clStream);
	bool			handle_pickup			(uint8 uEventID,CNetStream& clStream);
	bool			handle_get_client_data	(uint8 uEventID,CNetStream& clStream);
	bool			handle_find_player		(uint8 uEventID,CNetStream& clStream);
	bool			handle_promotion		(uint8 uEventID,CNetStream& clStream);
	bool			handle_relive			(uint8 uEventID,CNetStream& clStream);
	bool			handle_planeenter		(uint8 uEventID,CNetStream& clStream);
	bool			handle_planeleave		(uint8 uEventID,CNetStream& clStream);
	bool			handle_bosslist			(uint8 uEventID,CNetStream& clStream);
	bool			handle_fly				(uint8 uEventID,CNetStream& clStream);
	bool			handle_task_fly			(uint8 uEventID,CNetStream& clStream);
	bool			handle_search_player	(uint8 uEventID,CNetStream& clStream);
	bool			handle_npcdialog_complete(uint8 uEventID,CNetStream& clStream);
	bool			handle_get_battleattr_detail(uint8 uEventID,CNetStream& clStream);
	bool			handle_pktype			(uint8 uEventID,CNetStream& clStream); 
	bool			handle_takeaseat		(uint8 uEventID,CNetStream& clStream); 
	bool			handle_forbit_speak		(uint8 uEventID,CNetStream& clStream); 
	bool			handle_stronger_reward	(uint8 uEventID,CNetStream& clStream); 
	bool			handle_sub_fight_power	(uint8 uEventID,CNetStream& clStream);
	
public:
	bool			sendBeFindNoticeAck		();
	bool			sendCopymapDayCountAck	(uint8 uCopymapMainType,uint32 uCount);
	bool			sendTeamTaskDayCountAck	(uint32 uID,uint32 uCount);
	bool			broadcastQinggong		(uint32 uQinggongID);
	bool			sendBattleAttrDetail	(const PBS2CGetBattleAttrDetailAck *ack, uint8 uCode);
	bool			onGetBattleAttrDetail	(uint32 uType, PBS2CGetBattleAttrDetailAck & ask, uint8& uCode);
	bool			onTakeASeat				(uint8& uCode);
	bool			cancelTakeASeat			();
	bool			sendDazuoguajiNtf		(uint32 uExpLeft, uint32 uWisdomLeft, uint32 uExpAdded, uint32 uWisdomAdded, bool bShowPrompt, bool  bClosePanel);
	bool			strongerGetReWard(uint32 uID, uint8 &uCode);

public:
	inline uint32   getDailyDazuoguajiExp(){return m_uDailyDazuoguajiaExp;};
	inline void		setDailyDazuoguajiExp(uint32 uExp){m_uDailyDazuoguajiaExp = uExp;}
	inline uint32   getDailyDazuoguajiTaolue(){return m_uDailyDazuoguajiaWisdom;};
	inline void	    setDailyDazuoguajiTaolue(uint32 uTaolue){m_uDailyDazuoguajiaWisdom = uTaolue;}

};
/*************************************************************/
