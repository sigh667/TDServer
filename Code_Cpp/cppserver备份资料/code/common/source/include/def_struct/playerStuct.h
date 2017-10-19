/*----------------- playerStuct.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/1/6 14:21:09
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/basicFunctions.h"
#include <string>
#include "stlDefine.h"
#include "stl/stl_queue.h"
#include "playerDefine.h"
#include "protocol/GameDefine.pb.h"
/*************************************************************/
class PBPlayerBase;
class PBPlayerDisplay;
class PBItem;
class PBPlayerShapeData;
class PBBingfu;
class PBEquipLucky;
class PBCoolingInfo;
class PBItemUseCount;
class PBPlayerAvatar;
class PBPlayerClimbTowerItem;
class PBPlayerClimbTower;
class PBPlayerPvpData;
class PBTreasureValueInfo;
class PBPvpMember;
class PBPlayerAvatar;
class PBPlayerLoginWealData;
class PBSinglePvpMember;
class PBSinglePvpRecord;
class PBPlayerSinglePvp;
typedef stl_map<uint8, int32>		MAP_ENERGY;
//-------------------------------------------------------------
//------------------------------
struct _stPlayerStream
{
	uint32 uSaveOrder;
	uint16 uSize;
	char szStream[_IO_BUFFER_ - 1024];

	_stPlayerStream()						{	init();							}
	void			init()					{	memset(this,0,sizeof(*this));	}
};
//-------------------------------------------------------------
//------------------------------ 
struct _stPlayerDisplay
{
	char					szPlayerName[Player_Name_Max];//角色名
	uint32					uPlayerID;			// 角色ID
	uint32					uLevel;				// 角色等级
	uint32					uWorldID;			// 世界ID
	uint32					uLogicWorldID;		// 逻辑世界ID
	uint32					uJob;				// 职业
	uint32					uGender;			// 性别
	uint32					uCampID;			// 阵营
	uint32					uFactionID;			// 帮会ID
	char					szFactionName[Faction_Name_Max];// 帮会名
	char					szAlianceName[Faction_Name_Max];// 联盟名
	_emCorps				emCorps;			// 军团枚举
	uint32					uIdentity;			// 主身份ID
	uint32					uTitleID;			// 称号ID
	char					szFalctionBanner[Faction_Name_Max];	// 帮会旌旗

	_stPlayerDisplay		&getDisplay()		{	return *this;					}
	const _stPlayerDisplay	&getDisplay() const	{	return *this;					}
	inline void	initDisplay	()					{	memset(this,0,sizeof(*this));	}
	bool	loadFromProtobuf		(const PBPlayerDisplay&pbDisplay);
	bool	saveToProtobuf			(PBPlayerDisplay&pbDisplay) const;
};
//-------------------------------------------------------------
//------------------------------ 角色显示信息
struct _stPlayerShape
{
	uint32	uShape[_emPlayerShapeType_ARRAYSIZE];

	_stPlayerShape()				{	initPlayerShape();				}
	_stPlayerShape&getPlayerShape()	{	return *this;					}
	inline void	initPlayerShape()	{	memset(this,0,sizeof(*this));	}
};
//-------------------------------------------------------------
//------------------------------ 角色外显示信息
struct _stPlayerAvatar
{
	_stPlayerDisplay	stDisplay;			//外线
	_stPlayerShape		stShape;			//外观
	SET_U32				setFashion;			//时装

	_stPlayerAvatar()					{	init();							}
	_stPlayerAvatar&getPlayerAvatar()	{	return *this;					}
	inline void	init()	
	{
		stDisplay.initDisplay();
		stShape.initPlayerShape();
		setFashion.clear();
	}

	bool	loadFromProtobuf	(const PBPlayerAvatar&pbInfo);
	bool	saveToProtobuf		(PBPlayerAvatar&pbInfo) const;
};

//-------------------------------------------------------------
//------------------------------ 角色社会属性
struct _stPlayerBase : _stPlayerDisplay
{
	uint32					uAccountID;			// 账号ID
	uint32					uPlayerMark;		// 角色标示(位_em_PlayerMark)
	uint32					uSceneID;			// 所在场景ID
	uint32					uSceneCellID;		// 所在场景线ID
	uint64					uSceneDynID;		// 所在场景动态ID
	uint8					uSpawnID;			// 出生点
	float32					fPosX;				// 位置x
	float32					fPosY;				// 位置y
	float32					fDir;				// 朝向
	uint32					uLastLoginTime;		// 最后一次登录时间
	uint32					uLastLogoutTime;	// 最后一次登出时间
	uint32					uCreateTime;		// 创建角色时间
	uint8					uPKType;			// PK模式
	uint32					uPKValue;			// PK值
	uint32					uKillTime;			// 灰名 开始时间
	uint64					uExp;				// 当前经验
	uint64					uWisdom;			// 韬略
	int32					nCurHP;				// 当前生命
	uint32					uItemSequence;		// 道具构建序号
	_stPlayerShape			stPlayerShape;		// 角色外显
	uint32					uFightPower;		// 战斗力
	uint32					uMaxFightPower;		// 历史最高战斗力
	uint32					uEquipLucky;		// 装备吉运值
	uint32					uMountID;			// 骑乘ID 0无骑乘
	uint32					uOldSceneID;		// 老的 所在场景ID
	uint32					uOldSceneCellID;	// 老的 所在场景线ID
	uint64					uOldSceneDynID;		// 老的 所在场景动态ID
	float32					fOldPosX;			// 老的 位置x
	float32					fOldPosY;			// 老的 位置y
	float32					fOldDir;			// 老的 朝向
	uint64					uSufferValue;		// 历练值
	uint32					uCorpsJob;			// 军团职位
	uint32					uDailyCleanTime;	// 日清理时间
	uint32					uWeeklyCleanTime;	// 周清理时间
	uint32					uMonthlyCleanTime;	// 月清理时间


	_stPlayerBase&getPlayerBase	()						{	return *this;					}
	inline void	initPlayerBase	()						{	memset(this,0,sizeof(*this));	}

	inline void	setPlayerMark	(uint8 uMark,bool bSet)	{	_SET_BIT32(this->uPlayerMark,uMark,bSet);				}
	inline bool testPlayerMark	(uint8 uMark)const		{	return _CHECK_BIT(this->uPlayerMark,_BIT32(uMark));		}
};
//-------------------------------------------------------------
//------------------------------ 
// 角色社会属性
struct _stPlayer : _stPlayerBase
{
	stl_map<uint8,uint64>	mapMoney;			// 金钱
	MAP_ENERGY				mapEnergy;			// 能量
	std::string				strOnlyIndex;		// 唯一索引 永久
	std::string				strOnlyDayIndex;	// 唯一索引日
	std::string				strOnlyWeekIndex;	// 唯一索引周
	std::string				strOnlyMonthIndex;	// 唯一索引 月
	MAP_U8_U32				mapCopymapDayCount;	// 副本日进入次数
	SET_U32					setFashion;			// 时装
	MAP_U32_U32				mapTeamTaskDayCount;// 组队任务日进入次数 key team_task玩法ID
	MAP_U32_U32				mapForbitSpeak;		// 禁言

	_stPlayer&getPlayer			()					{	return *this;					}
	inline void	initPlayer		()
	{	
		initPlayerBase();
		mapMoney.clear();
		mapEnergy.clear();
		strOnlyIndex.clear();
		strOnlyDayIndex.clear();
		strOnlyWeekIndex.clear();
		strOnlyMonthIndex.clear();
		mapCopymapDayCount.clear();
		setFashion.clear();
		mapTeamTaskDayCount.clear();
		mapForbitSpeak.clear();
	}

	bool	loadFromProtobuf	(const PBPlayerBase&pbPlayerBase);
	bool	saveToProtobuf		(PBPlayerBase&pbPlayerBase);

	bool	saveToAvatar		(PBPlayerAvatar&pbAvatar);
};
//------------------------------------------------------
//------------------------------ 道具/装备属性
struct _stItemBase
{
	uint32	uItemID;				//物品ID 
	uint16	uItemNum;				//物品数量
	uint64	uItemSN;				//物品流水号 
	uint32	uFlag;					//物品标示(_em_ItemFlag)
	uint32	uLeftTime;				//剩余时间秒
	uint32	uEndtime;				//结束时间 为0 只取leftick
	uint32	uEnhanceExp;			//强化经验
	uint32	uForgeExp;				//琢磨经验
	uint32	uBingfuExp;				//兵符经验

	_stItemBase	&getBase	()	{	return *this;					}
	void		initBase	()	{	memset(this,0,sizeof(*this));	}
};
//------------------------------------------------------
//------------------------------ 道具/装备属性
struct _stItem : _stItemBase
{
	stl_vector<uint32> vecSpecialID;	//极品属性

	_stItem					()			{	initialize();		}
	void		initialize	()	
	{	
		initBase();
		vecSpecialID.clear();
	}
	_stItem	&getItem			()						{	return *this;					}
	bool	testFlag			(uint8 flag)const		{	return _CHECK_BIT(this->uFlag,_BIT32(flag));}
	void	setFlag				(uint8 flag,bool bSet)	{	_SET_BIT32(this->uFlag,flag,bSet);			}
	bool	loadFromProtobuf	(const PBItem& pbData);
	bool	saveToProtobuf		(PBItem& pbData,bool bOfflineNoTime = false);

};
//------------------------------------------------------
//------------------------------ 道具/装备属性
struct _stItemUseCount
{
	uint32	uItemID;
	uint32	uDayUseCount;
	uint32	uWeekUseCount;
	uint32	uMonthUseCount;

	_stItemUseCount				()			{	init();		}
	void	init				()			{	memset(this,0,sizeof(*this));	}
	bool	loadFromProtobuf	(const PBItemUseCount& pbData);
	bool	saveToProtobuf		(PBItemUseCount& pbData);
};
//------------------------------------------------------
//------------------------------冷却数据
struct _stCoolingInfo
{
	uint16			eType;		//类型 _emCoolingType
	uint32			uID;		//id
	uint64			uBeginTick;	//开始时间	服务器启动时间(毫秒)
	uint64			uEndTick;	//结束时间	服务器启动时间(毫秒)
	uint32			uBeginTime; //开始的UNIX时间 (秒)  保存和恢复冷却信息使用

	_stCoolingInfo	()						{	init();								}
	void			init				()	{	memset(this,0,sizeof(*this));		}

	bool			loadFromProtobuf	(const PBCoolingInfo& pbData);
	bool			saveToProtobuf		(PBCoolingInfo* pbData);
};
//------------------------------------------------------
//------------------------------个人爬塔玩法数据,兵器谱
struct _stClimbTowerItem
{
	uint32 			uItemID 	;	// 道具id
	uint32			uCount		;	// 数量
	_stClimbTowerItem	()						{	init();								}
	void			init				()		{	memset(this,0,sizeof(*this));		}
	bool			loadFromProtobuf	(const PBPlayerClimbTowerItem& pbData);
	bool			saveToProtobuf		(PBPlayerClimbTowerItem* pbData);

};
//------------------------------------------------------
//------------------------------
struct _stClimbTower
{

	uint32 							uDayCurrentCopymapID 		;		// 每日清零,今天已经完成的任务链进度,是一个副本id,需要根据副本id去找对应的排名,每日清0,默认0
	std::vector<_stClimbTowerItem> 	vcDayItemUnsettled			;		// 每日清零,今天已经获得的道具奖励,还没结算的.如果选择放弃则清空,如果选择领取,则添加挑战次数,领取奖励
	uint32 							uDayPlayCount				;		// 每日清零,今天挑战的次数,不能超过限定
	uint32 							uWeekCurrentClass			;		// 每周清零,这周的难度阶段,默认0
	uint32 							uWeekMaxCopyID		 		;		// 每周清零,这周的最高进度,默认0,清零邮件发放奖励,不在线则登陆处理

	uint32							uNextJumpCopymapID			;		// 连续爬塔的跳转目标记录

	uint32							uLuckDrawRightNow			;		// 进入下一个副本立即发放的奖励
	_stClimbTower					()		{	init();								}
	void			init			()		
	{	
		uDayCurrentCopymapID = 0;
		uDayPlayCount		 = 0;
		uWeekCurrentClass	 = ClimbTowerClass_Easy;//默认是简单难度,起始难度
		uWeekMaxCopyID		 =  0;
		vcDayItemUnsettled.clear();
		uNextJumpCopymapID   = 0;
		uLuckDrawRightNow	 = 0;
	}
;
	bool			loadFromProtobuf	(const PBPlayerClimbTower& pbData);
	bool			saveToProtobuf		(PBPlayerClimbTower* pbData);

};
//------------------------------------------------------
//------------------------------ 道具/装备属性
struct _stPvpWar
{
	uint32	uID					;	// 玩法ID
	uint32	uEntercount			;	// 当日进入次数
	uint32	uWinnum				;	// 胜利总场次
	uint32	uFailnum			;	// 失败总场次
	uint32	uCapturenum			;	// 洗旗次数
	uint32	uKillnum			;	// 击杀次数
	uint32	uAssistnum			;	// 助攻次数
	uint32	uDeadnum			;	// 死亡次数
	uint32	uMegakill 			;	// 暴走次数
	uint32	uGodlike 			;	// 超神次数
	uint32	uMvpnum 			;	// MVP次数
	uint32	uExitReadyCount		;	// 当日退出准备次数
	uint32	uNextEnterTime		;	// 下次能进入时间 0表示无限制

	_stPvpWar					()			{	init();		}
	void	init				()			{	memset(this,0,sizeof(*this));	}
	bool	loadFromProtobuf	(const PBPlayerPvpData& pbData);
	bool	saveToProtobuf		(PBPlayerPvpData* pbData);
};
//------------------------------------------------------
//------------------------------ 藏宝图信息
struct _stTreasureValueInfo
{
	uint32	uPrizeID	;	// 奖励ID treasure_map.xls
	uint32	uDayValue	;	// 日幸运值
	uint32	uDayCount	;	// 日次数
							   
	_stTreasureValueInfo		()			{	init();		}
	void	init				()			{	memset(this,0,sizeof(*this));	}
	bool	loadFromProtobuf	(const PBTreasureValueInfo& pbData);
	bool	saveToProtobuf		(PBTreasureValueInfo* pbData);
};
//-------------------------------------------------------------
//------------------------------
struct _stPvpMember
{
	_stPlayerDisplay	stDisplayer;	//玩家信息
	uint64	uMaxFightPower;				//历史最高战力 * 1000
	uint64	uTeamID;					//队伍ID
	bool	bReady;						//是否准备

	_stPvpMember				()			{	init();		}
	void		init			()			{	memset(this,0,sizeof(*this));	}
	bool	loadFromProtobuf	(const PBPvpMember& pbData);
	bool	saveToProtobuf		(PBPvpMember* pbData);
};
//-------------------------------------------------------------
//------------------------------
struct _stWealLogin
{
	uint32 	uID 		;	// cs_weal.xls login_weal中ID
	uint32 	uPrizeday	;	// 已经领奖的天数
	uint32 	uPrizetime	;	// 上次领奖时间

	_stWealLogin				()			{	init();		}
	void		init			()			{	memset(this,0,sizeof(*this));	}
	bool	loadFromProtobuf	(const PBPlayerLoginWealData& pbData);
	bool	saveToProtobuf		(PBPlayerLoginWealData* pbData);
};
//------------------------------------------------------
//------------------------------ 1v1记录
struct _stSinglePvpRecord
{
	_stPlayerDisplay	stDisplayer		;	//玩家信息
	uint32				uSinglePvpScore	;	// 1v1积分
	uint32				uResult			;	// 结果 _emCopymapCompleteType

	_stSinglePvpRecord			()			{	init();		}
	void	init				()			{	memset(this,0,sizeof(*this));	}
	bool	loadFromProtobuf	(const PBSinglePvpRecord& pbData);
	bool	saveToProtobuf		(PBSinglePvpRecord* pbData);
};
//------------------------------------------------------
//------------------------------ 1v1数据
struct _stSinglePvpBase
{
	uint32	uDayEnterCount		;	// 今日进入次数
	uint32	uWinCount			;	// 胜利场数
	uint32	uFailCount			;	// 失败场数
	uint32	uEqualCount			;	// 平局场数
	uint32	uWeekCount			;	// 周场次
	uint32	uSinglePvpScore		;	// 1v1段位积分
	uint32	uSinglePvpMaxScore	;	// 1v1段位最高积分

	_stSinglePvpBase			()			{	initBase();		}
	void	initBase			()			{	memset(this,0,sizeof(*this));	}
};
//------------------------------------------------------
//------------------------------ 1v1数据
struct _stSinglePvp : _stSinglePvpBase
{
	SET_U32							setFirstPrizeID;	//已经领取的首次ID
	SET_U32							setDayPrize;		//已经领取的每日奖励
	stl_list<_stSinglePvpRecord>	listPvpRecord;		//记录

	_stSinglePvp			()			{	init();		}
	void	init				()			
	{	
		initBase();
		setFirstPrizeID.clear();
		setDayPrize.clear();
		listPvpRecord.clear();
	}
	bool	loadFromProtobuf	(const PBPlayerSinglePvp& pbData);
	bool	saveToProtobuf		(PBPlayerSinglePvp* pbData);
};
//-------------------------------------------------------------
//------------------------------ 1v1成员信息
struct _stSinglePvpMember
{
	_stPlayerDisplay	stDisplayer;	//玩家信息
	uint32	uSinglepvpscore	;			// 1v1积分
	uint32	uMaxfightpower	;			// 历史最高战力
	uint32	uWincount		;			// 胜利场数
	uint32	uFailcount		;			// 失败场数
	uint32	uEqualcount		;			// 平局场数

	_stSinglePvpMember			()			{	init();		}
	void		init			()			{	memset(this,0,sizeof(*this));	}
	bool	loadFromProtobuf	(const PBSinglePvpMember& pbData);
	bool	saveToProtobuf		(PBSinglePvpMember* pbData);
};