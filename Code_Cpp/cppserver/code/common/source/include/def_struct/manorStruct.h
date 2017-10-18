/*----------------- teamStruct.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : tanghaibo
* Version      : V1.0
* Date         : 2016/12/2o 19:32:48
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "gameDefine.h"
#include "cfg_data/manorData/manorData.h"
#include "def_struct/stlDefine.h"
#include "def_struct/playerStuct.h"
#include "def_struct/factionStruct.h"
#include "protocol/GameDefine.pb.h"
#include "protocol/WAGMsg.pb.h"
#include "protocol/CASMsg.pb.h"
#include "protocol/WADBMsg.pb.h"
#include <map>
#include <string>
/*************************************************************/
/*************************************************************/

struct _stManorChallengerDB
{
	uint32		uFactionid;			// 帮派ID
	uint32		uApplyTime;			// 申请时间
	bool		bIsChallenger;		// 是否挑战者,true表示挑战者 ,红色
	uint32		uManorid;			// 领地ID这个是不存数据库的 读入内存的时候初始化

	void		init()
	{
		uFactionid = 0;
		uApplyTime = 0;
		bIsChallenger = false;
	}

	void	loadFromProtobuf(const PBFactionChallengerDB&pbInfo);
	void	saveToProtobuf	(PBFactionChallengerDB&pbInfo);
};

struct _stManorInfoDB
{
	uint32										uManorid		;	// 索引到manor表格
	uint32										uStatus			;	// 领地状态 _emManorOccupyStatus
	uint32										uOwnerfactionid	;	// 当前占有帮派
	typedef std::map<uint32 , _stManorChallengerDB> MAPID2CHALLENGER;
	MAPID2CHALLENGER							mpChallengers	;	// 参赛队列,两个队伍都在这里显示,攻击防御双方看标志
	uint64										uWeekPrizeTime	;	// 每次发放奖励的时间点,记为这一周的起始时间
	uint64										uDayPrizeTime	;	// 每次发放奖励的时间点,记为这一天的起始时间
	void		init()
	{
		uManorid		= 0;
		uStatus			= 0;
		uOwnerfactionid = 0;
		mpChallengers.clear();
		uWeekPrizeTime  = 0;
		uDayPrizeTime	= 0;
	}
	void	loadFromProtobuf					(const PBManorInfoDB&pbInfo);
	void	saveToProtobuf						(PBManorInfoDB&pbInfo);
	_stManorChallengerDB* getChallengerByFactionID(uint32 uFactionID);
	void	addChallenger						(uint32 uFactionID);
	void	removeChallenger					(uint32 uFactionID);
};

struct _stManorChallenger
{
	_stFactionBase		stFactionBase;		// 领地基础信息
	uint32				uApplyTime;			// 申请时间
	bool				bIsChallenger;		// 是否挑战者,true表示挑战者 ,红色
	void	loadFromProtobuf(const PBFactionChallenger&pbInfo);
	void	saveToProtobuf	(PBFactionChallenger&pbInfo);
	void    init()
	{
		stFactionBase.init();
		uApplyTime = 0;
		bIsChallenger = false;
	}
};

struct _stManorCpymapInfo
{
	uint32				uSceneID;			// 场景id
	uint64				uDynID;				// 动态id
	uint32				uCellID;			// 线id
	uint32				regionid;			// 如果是战场,需要这个id

	void		init()
	{
		uSceneID		= 0;			// 场景id
		uDynID			= 0;			// 动态id
		uCellID			= 0;			// 线id
		regionid		= 0;			// 如果是战场,需要这个id
	}
	void	loadFromProtobuf(const PBManorCpymapInfo&pbInfo);
	void	saveToProtobuf	(PBManorCpymapInfo&pbInfo);
};

struct _stManorInfo
{
	uint32						manorid		;	// 索引到manor表格
	uint32						status		;	// 领地状态 _emManorOccupyStatus
	_stFactionBase				owner		;	// 当前占有帮派
	typedef std::vector<_stManorChallenger> VECTOR_MANORCHALLENGER;
	VECTOR_MANORCHALLENGER		vcChallengers	;	// 参赛队列,两个队伍都在这里显示,攻击防御双方看标志
	typedef std::map<uint32, _stManorCpymapInfo> MAP_REGIONID_2_BATTLEFIELD;
	MAP_REGIONID_2_BATTLEFIELD	mpBattlefield;					//战场入口,根据防守和挑战分别设置
	_stManorCpymapInfo			defenderLobby;					//防守方大厅
	_stManorCpymapInfo			challengerLobby;				//挑战方大厅
	typedef std::set<uint32>	SET_ELITE_PLAYERS;				
	SET_ELITE_PLAYERS			setElitePlayerIDs;				//精英玩家的id集合
	void	init()
	{
		manorid = 0;
		status = 0;
		owner.init();
		vcChallengers.clear();
		mpBattlefield.clear();
		defenderLobby.init();
		challengerLobby.init();
		setElitePlayerIDs.clear();
	}
	void	loadFromProtobuf(const PBManorInfo&pbInfo);
	void	saveToProtobuf	(PBManorInfo&pbInfo);
};

struct _stManorBattleFieldPlayerScore
{
	bool						ischallenger	 ;	//	true表示是挑战方 红色,否则蓝色
	uint32						job				 ;	//	职业
	std::string					name			 ;	//	名字
	uint32						score			 ;	//	分数
	uint32						kill			 ;	//	击杀次数
	uint32						dead			 ;	//	被击杀次数
	uint32						playerid		 ;	//	playerid
	uint64						takeHurt		 ;	//	承受伤害
	uint32						assistkill		 ;	//	助攻次数
	void	init()
	{
		job				 = 0;
		name			 = "";	//	名字
		score			 =	0;	//	分数
		kill			 =	0;	//	击杀次数
		dead			 =	0;	//	被击杀次数
		ischallenger	 =	false;	//	true表示是挑战方 红色,否则蓝色
		playerid		 =	0;	//	playerid
		takeHurt		 =	0;	//	承受伤害
		assistkill		 =	0;	//	助攻次数

		//内存中使用
		mpUseful.clear();
		mpHarmful.clear();
		safeZoneStayTime = 0;//进入安全区等待的起始时间
		lostEliteTime = 0;
		continusKill = 0;
	}
	void	loadFromProtobuf(const PBManorBattlefieldPlayerScore&pbInfo);
	void	saveToProtobuf	(PBManorBattlefieldPlayerScore&pbInfo);

	bool operator < (const _stManorBattleFieldPlayerScore &m)const {
		return score < m.score;
	}

	//不参与数据传送,内存中使用
	typedef  std::map<uint32, uint32> MAP_PLAYERID_2_PLAYERID_BUFF_RECORD;
	MAP_PLAYERID_2_PLAYERID_BUFF_RECORD mpUseful;
	MAP_PLAYERID_2_PLAYERID_BUFF_RECORD mpHarmful;
	void	addBuffRecordUseful(uint32 uPlayerID);
	void	addBuffRecordHarmful(uint32 uPlayerID);
	MAP_PLAYERID_2_PLAYERID_BUFF_RECORD& getUsefulBuffRecord(){return mpUseful;}
	MAP_PLAYERID_2_PLAYERID_BUFF_RECORD& getHarmfulBuffRecord(){return mpHarmful;}
	uint32	safeZoneStayTime;//进入安全区等待的起始时间
	uint32	lostEliteTime;//丢失精英的起始时间
	uint32	continusKill;//连续杀人记录
};

struct _stManorBattleFieldReport
{
	uint32											factionid		;		//帮会ID
	uint32											count			;		//当前战场人数
	typedef std::map<uint32,_stManorBattleFieldPlayerScore>  MAP_PLAYERID_2_SCORE;
	MAP_PLAYERID_2_SCORE							mpPlayerscores	;		//每个人的得分 
	uint32											score			;		//该帮会的得分

	void	init()
	{
		factionid	= 0;
		count		= 0;
		mpPlayerscores.clear();
		score		= 0;
	}
	void	loadFromProtobuf(const PBManorBattleFieldReport&pbInfo);//注意是增量的,
	void	saveToProtobuf	(PBManorBattleFieldReport&pbInfo);

};
