/*----------------- factionStruct.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 林驹
* Version      : V1.0
* Date         : 2016/10/12 19:32:48
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "gameDefine.h"
#include "def_struct/stlDefine.h"
#include "def_struct/playerStuct.h"
#include "protocol/GameStruct.pb.h"
#include "memoryPool/mapPool.h"
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
class PBFactionBase;
struct _stFactionBase
{
	uint32		uFactionid;			// 帮派ID
	uint32		uLeader;			// 帮主ID
	uint32		uWorldID;			// 世界ID
	uint32		uLevel;				// 帮派等级
	uint32		uUnion;				// 联盟ID
	std::string	sName;				// 帮派名
	std::string	sBanner;			// 旌旗
	_emCorps	emCorps;			// 军团

	explicit _stFactionBase()	{	init();	}
	void	init()		
	{	
		uFactionid		= 0;
		uLeader			= 0;
		uWorldID		= 0;
		uLevel			= 0;
		uUnion			= 0;
		sName.clear();
		sBanner.clear();
		emCorps			= Corps_Unknown;
	}

	void	loadFromProtobuf(const PBFactionBase&pbInfo);
	void	saveToProtobuf	(PBFactionBase&pbInfo)const;

};

//-------------------------------------------------------------
//------------------------------	 // 帮派中成员信息
class PBFactionMember;
class PBFactionMemberBase;
struct _stFactionMember
{
	uint32				uPlayerID;			// 角色ID
	uint32				uContribution;		// 贡献
	uint32				uExperience;		// 威望
	uint32				uJointime;			// 加入时间
	_emFactionJob		emJob;				// 职位
	stl_set<uint32>		setTitle;			// 称号
	uint32				uLastOfflineTime;	//上次下线时间

	// 以下字段从game或global服务器同步过来
	_stPlayerDisplay	stDisplay;
	uint32				uFightPower;
	bool				bOnline;

	uint32				uFobitCount;		// 禁言人次
	uint32				uFobitTime;			// 第一次禁言的时间
	// 以下字段逻辑处理用
	bool				bNeedUpdate;		// 是否需要更新到其他服务器

	explicit _stFactionMember()
	{
		init();
	}

	void init()
	{
		uPlayerID		= 0;
		uContribution	= 0;
		uExperience		= 0;
		uJointime		= 0;
		emJob			= FactionJob_Unknown;
		uLastOfflineTime= 0;

		stDisplay.initDisplay();
		uFightPower		= 0;
		bOnline			= false;

		bNeedUpdate		= false;
	}

	inline void setUpdate(bool bUpdate = true)	{	bNeedUpdate = bUpdate;	}
	inline bool isNeedUpdate()	const			{	return bNeedUpdate;		}

	void	saveToPBFactionMember(PBFactionMember& info) const;
	void	saveToPBFactionMemberBase(PBFactionMemberBase& info) const;
};

//-------------------------------------------------------------
//------------------------------	 // 帮派申请列表中申请者信息
class PBFactionApplyData;
struct _stFactionApply
{
	uint32				uPlayerID;			// 角色ID
	uint32				uApplytime;			// 申请时间
	_stPlayerDisplay	stDisplay;
	uint32				uFightPower;

	explicit _stFactionApply()
	{
		init();
	}

	void init()
	{
		uPlayerID		= 0;
		uApplytime		= 0;
		stDisplay.initDisplay();
		uFightPower		= 0;
	}

	void	saveToPBFactionApplyData(PBFactionApplyData& info) const;
};
//-------------------------------------------------------------
//------------------------------帮会技能
class PBFactionSkill;
struct _stFactionSkill
{
	uint32				uSkillID;			// 技能ID
	uint32				uLevel;				// 等级
	uint32				eState;				// 技能状态
	uint32				uDevelopLeftTime;	// 研究剩余时间(秒) 非零代表正在升级中
	uint32				uDevelopLastTime;	// 最后一次减少研究剩余时间的时间点 该字段是为了关服后还能记录设计的 前端不需要关心该字段

	explicit _stFactionSkill()
	{
		init();
	}

	void init()
	{
		uSkillID			= 0;
		uLevel				= 0;
		eState				= 0;
		uDevelopLeftTime	= 0;
		uDevelopLastTime	= 0;
	}

	void	saveToPBFactionSkill(PBFactionSkill& info) const;
};
//-------------------------------------------------------------
//------------------------------游学列表
class PBFactionStudy;
struct _stFactionStudy
{
	uint32				uFactionID;			// 帮会ID
	uint32				uBeginTime;			// 开始时间

	explicit _stFactionStudy()
	{
		init();
	}

	void init()
	{
		uFactionID		= 0;
		uBeginTime		= 0;
	}

	void	saveToPBFactionStudy(PBFactionStudy& info) const;
};
//-------------------------------------------------------------
//------------------------------游学设置
class PBFactionStudyConfig;
struct _stFactionStudyConfig
{
	bool				bOpenStudy;			// 是否开启了游学
	uint32				uStudyMoneyType;	// 游学需要的货币类型 _emFactionMoneyType
	uint32				uStudyMoneyValue;	// 游学需要的货币值
	uint32				uTime;				// 设置时间

	explicit _stFactionStudyConfig()
	{
		init();
	}

	void init()
	{
		bOpenStudy		= false;
		uStudyMoneyType	= 0;
		uStudyMoneyValue= 0;
		uTime			= 0;
	}

	void	loadFromPBFactionStudyConfig(const PBFactionStudyConfig& info);
	void	saveToPBFactionStudyConfig(PBFactionStudyConfig& info) const;
};
//-------------------------------------------------------------
//------------------------------帮派弹劾信息
class PBFactionImpeach;
struct _stFactionImpeach
{
	uint32				uImpeachID;			// 发起者玩家ID
	uint32				uImpeachTime;		// 弹劾失效时间 0未弹劾
	uint32				uMaxMemberNum;		// 最大有效人数 弹劾确定
	SET_U32				setAgreeMember;		// 同意的成员
	SET_U32				setRefuseMember;	// 不同意的成员

	explicit _stFactionImpeach()
	{
		init();
	}

	void init()
	{
		uImpeachID = 0;
		uImpeachTime = 0;
		uMaxMemberNum = 0;
		setAgreeMember.clear();
		setRefuseMember.clear();
	}

	void	saveToPBFactionImpeach(PBFactionImpeach& info) const;
};

//-------------------------------------------------------------
//------------------------------

struct _stTopListInfo		//排行榜 数值
{
	uint32		uType;		//排行类型 _emTopListType
	uint64		uValue;		//排行值
	uint64		uSubValue;	//排行子值
	uint32		uTime;		//修改时间

	_stTopListInfo&getInfo()			{	return *this;					}
	inline void	initialize()			{	memset(this,0,sizeof(*this));	}

	inline bool	isValid			()const	{	return uValue||uSubValue;		}

	void		loadFromProtobuf(const PBTopListInfo &pbInfo);
	void		saveToProtobuf	 (PBTopListInfo &pbInfo)const;
};

//-------------------------------------------------------------
//------------------------------ 
struct _stTopListSortType	//排行榜 类型
{
	uint32		uType;		//排行类型 _emTopListType
	uint32		uJob;		//职业
	//uint32	uState;		//国家

	_stTopListSortType&getSortType	()		{	return *this;					}
	inline void	initSortType		()		{	memset(this,0,sizeof(*this));	}

	inline bool	operator <		(const _stTopListSortType& rhs)const
	{
		if ( uType != rhs.uType )
			return uType < rhs.uType;
		else 
		{
			return uJob < rhs.uJob ;
		}
		return false;
	}
	inline bool	operator ==		(const _stTopListSortType& rhs)const
	{
		if ( uType != rhs.uType )
			return false;

		if (uJob != rhs.uJob)
			return false;

		return true;
	}

	void		loadFromProtobuf(const PBTopListSortType &pbInfo);
	void		saveToProtobuf	 (PBTopListSortType &pbInfo)const;
};
//-------------------------------------------------------------
//------------------------------ 
struct _stTopListOrder	: _stTopListSortType	//排行榜 排行
{
	uint32		uOrder;		//排行

	_stTopListOrder&getInfo()			{	return *this;					}
	inline void	initInfo	()			{	memset(this,0,sizeof(*this));	}

	inline bool	isValid			()const	{	return 0!=uOrder;				}

	void		loadFromProtobuf(const PBTopListOrder &pbInfo);
	void		saveToProtobuf	 ( PBTopListOrder &pbInfo)const;
};

typedef stl_map<uint8,_stTopListInfo>		MAP_TOPLIST_INFO;	// <类型，排行信息>
typedef stl_set<_stTopListOrder>			SET_TOPLIST_ORDER;	// 排行值

//-------------------------------------------------------------
//------------------------------	 // 帮派信息
class PBFactionDBData;
class PBFactionBase;
struct _stFaction : public _stFactionBase
{
	typedef	stl_map<uint32, _stFactionMember>	MAP_PLAYERS;	// player id -> member
	typedef stl_map<uint32, _stFactionApply>	MAP_APPLYS;		// player id -> _stFactionApply
	typedef stl_map<uint32, _stFactionSkill>	MAP_SKILLSS;	// skill id -> _stFactionSkill
	typedef stl_map<uint32, _stFactionStudy>	MAP_STUDYS;		// faction id -> _stFactionStudy
	std::string				sDeclaration;		// 宣言
	uint32					uCreateTime;		// 创建时间
	uint64					uStone;				// 玉石
	uint64					uSilver;			// 资金
	uint64					uEnergy;			// 行动力
	uint64					uScore;				// 评定值
	uint64					uBoom;				// 繁荣度
	uint64					uExperience;		// 威望
	uint32					uOrder;				// 排名
	bool					bAutoApply;			// 是否自动入帮申请
	MAP_PLAYERS				mapPlayers;			// 存放成员列表
	MAP_APPLYS				mapApplys;			// 入帮申请列表
	uint32					uRequireLevel;		// 入帮等级要求
	MAP_U32_U32				mapRefuses;			// (拒绝/取消)入帮申请记录	player id -> 时间戳
	MAP_SKILLSS				mapSkills;			// 技能列表
	MAP_STUDYS				mapStudys;			// 游学列表
	_stFactionStudyConfig	stCurStudyConifg;	// 当前的游学学设置
	_stFactionStudyConfig	stFrontStudyConifg;	// 预设的游学学设置
	stl_map<uint32,uint64>	m_mapJobPrivilege;	// 职位权限管理
	_stFactionImpeach		m_stImpeach;		// 弹劾信息

	MAP_TOPLIST_INFO		m_mapTopListInfo;					// 数值
	SET_TOPLIST_ORDER		m_setTopListOrder;					// 排行

	explicit _stFaction()	{	init();		}
	void	init()
	{
		_stFactionBase::init();
		sDeclaration.clear();
		uCreateTime = 0;
		uStone = 0;
		uSilver = 0;
		uEnergy = 0;
		uScore = 0;
		uBoom = 0;
		uExperience = 0;
		uOrder = 0;
		mapPlayers.clear();
		mapApplys.clear();
		uRequireLevel = 0;
		mapRefuses.clear();
		mapSkills.clear();
		mapStudys.clear();
		stCurStudyConifg.init();
		stFrontStudyConifg.init();
		m_mapJobPrivilege.clear();
		m_stImpeach.init();
		bAutoApply = false;
	}

	void		loadFromProtobuf(const PBFactionDBData &pbInfo);
	void		saveToProtobuf(PBFactionDBData &pbInfo)	const;
	void		saveToPBFactionBase(PBFactionBase &pbInfo)	const;
	uint32		size() const;
};

//-----------------------------------------------------------------------------------------------------
// 帮派玩家离线数据类型
enum _emFactionOfflineType
{
	FactionOfflineType_Remove	= 1,	// 被移出帮派	PBFactionOfflineRemove
};
//-------------------------------------------------------------
//------------------------------
class PBCorpsMemberInfo;
struct _stCorpsMember
{
	uint32			uCorpsJob;		//军团职位 _emCorpsJob
	_stPlayerAvatar stAvatar;		//成员信息
	uint32			uFobitCount;	//禁言人数
	uint32			uNextSettime;	//下次更换成员时间
	explicit _stCorpsMember()	{	init();		}
	void	init()
	{
		uCorpsJob = 0;
		stAvatar.init();
		uFobitCount = 0;
		uNextSettime = 0;
	}
	void		loadFromProtobuf(const PBCorpsMemberInfo &pbInfo);
	void		saveToProtobuf(PBCorpsMemberInfo* pbInfo)	const;
};	
//-------------------------------------------------------------
//------------------------------军团
class PBCorpsInfo;
struct _stCorps
{
	typedef stl_map<uint32,_stCorpsMember> MAP_CORPSMEMBER; //key;职位
	uint32			uCorpID;			//军团ID	_emCorps
	std::string		strDeclaration;		//宣言
	MAP_CORPSMEMBER	mapMember;			//职位信息

	explicit _stCorps()	{	init();		}
	void	init()
	{
		uCorpID = 0;
		strDeclaration.clear();
		mapMember.clear();
	}

	void		loadFromProtobuf(const PBCorpsInfo &pbInfo);
	void		saveToProtobuf(PBCorpsInfo* pbInfo)	const;
};

//-------------------------------------------------------------
//-----------------------------军团离线数据类型
enum _emCorpsOfflineType
{
	CorpsOfflineType_ChangeJob	= 1,	// 改变职位	PBCorpsOfflineChangeJob
};