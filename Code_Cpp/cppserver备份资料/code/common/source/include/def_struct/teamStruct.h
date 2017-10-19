/*----------------- teamStruct.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2016/8/5 19:32:48
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "gameDefine.h"
#include "def_struct/stlDefine.h"
#include "def_struct/playerStuct.h"
#include "protocol/GameDefine.pb.h"
/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 
class PBTeamTarget;
class PBTeamTask;
class CWorldPlayer;
struct _stTeamTarget
{
	uint32	uTargetid			; //组队目标,如果填0表示没有目标,其他目标需要定义
	uint32	uDifficulty			; //有些组队目标里含有难度信息
	uint32	uLevelmin			; //最低等级限制,初始应该从活动表里获得,后期队长可以调整
	uint32	uLevelmax			; //最高等级限制,初始应该从活动表里获得,后期队长可以调整
	uint32	uActiveid			; //活动id,用来进行相关检测,等级,开放时间等
	bool	bIsAutoAccept		; //是否自动接受申请
	bool	bIsAutoMatch		; //是否自动匹配
	uint32	uCash				; //赏金数量( 元宝)
	_stTeamTarget()
	{
		init();
	}
	void init()
	{
		uTargetid = 0			; 
		uDifficulty	= 0			;
		uLevelmin = 0			;
		uLevelmax = 0			;
		uActiveid = 0			;
		uCash	  = 0			;	
		bIsAutoMatch = false	;
		bIsAutoAccept = false	;
	}
	void		loadFromProtobuf	(const PBTeamTarget&pbInfo);
	void		saveToProtobuf		(PBTeamTarget&pbInfo);
	bool		canMatchTarget		(_stPlayerDisplay& stPlayerDisplay);
};

struct _stTeamInfoBase
{
	uint64			uTeamID;				//队伍ID
	uint32			uLeaderID;				//队长ID 
	uint32			uAllotType;				//分配类型 _emTeamAllotType

	_stTeamInfoBase()		{	initBase();							}
	void	initBase()		
	{	
		uTeamID			= 0;
		uLeaderID		= 0;
		uAllotType		= AllotType_Dice;
	}
};
//-------------------------------------------------------------
//------------------------------ 
class PBTeamMember;
class PBTeamMemberSimple;
enum _emTeamVote
{
	TeamVote_None,	
	TeamVote_Agree,		// 同意
	TeamVote_Refuse		// 拒绝
};
//-------------------------------------------------------------
//------------------------------
struct _stTeamMember				// 队伍成员
{
	_stPlayerAvatar		stAvatar;				//外观2017-06-22 客户端要求描绘角色
	uint32				uFightPower;
	bool				bOnline;
	uint32				uTime;
	uint32				uSceneID;
	uint32				uCellID;
	_emTeamVote			eKickVote;
	_emTeamVote			eSetAllotVote;
	uint32				uMark;					//位 _emTeamMark
	uint32				uCallupTime;			//召集时间
	uint32				uTeamConfirmState;		//确认状态	_emTeamConfirmState

	_stTeamMember()		{	init();							}
	void	init()		{
		stAvatar.init();
		uFightPower			= 0;
		bOnline				= false;
		eKickVote			= TeamVote_None;
		eSetAllotVote		= TeamVote_None;
		uTime				= 0;
		uSceneID			= 0;
		uCellID				= 0;
		uMark				= 0;
		uCallupTime			= 0;
	}

	void		loadFromProtobuf	(const PBTeamMember&pbInfo);
	void		saveToProtobuf		(PBTeamMember&pbInfo);
	void		saveToProtobuf		(PBTeamMemberSimple&pbInfo);
	inline bool		check_mark		(uint8 _mark)	const			{	return _CHECK_BIT(uMark,_BIT32(_mark));	}
	inline void		set_mark		(uint8 _mark,bool bSet)			{	_SET_BIT32(uMark,_mark,bSet);			}

};
//-------------------------------------------------------------
//------------------------------缉拿副本数据
struct _stCopymapArrestData
{
	uint32		uCopymapID;				//副本ID
	uint8		uCopymapCompleteType;	//副本完成类型 _emCopymapCompleteType
	uint32		uExtraParam;			//额外参数 囚车个数
	_stCopymapArrestData()			{	memset(this,0,sizeof(*this));							}
};
//-------------------------------------------------------------
//------------------------------队伍空白位置职业需求
class PBTeamMemberJobRequire;
struct _stTeamMemberJobRequire
{
	typedef std::set<uint32> SETJOBS;
	uint32					uPostion;		//位置信息[1,2,3,4,5]
	SETJOBS					setJobs;		//职业信息
	_stTeamMemberJobRequire()
	{
		uPostion = 0;
	}
	void		loadFromProtobuf		(const PBTeamMemberJobRequire&pbInfo);
	void		saveToProtobuf			(PBTeamMemberJobRequire&pbInfo);
	uint32		findMatchCount			(uint32 uJob);
};
//-------------------------------------------------------------
//------------------------------申请者信息
class PBTeamApplicant;
struct _stApplicant
{
	uint32 uPlayerid				;//player id
	std::string strName				;//名字
	uint32 uLevel					;//等级
	uint32 uJob						;//职业
	uint32 uFightpower				;//战斗力
	uint32 uRelationship			;//社会关系
	uint64 uLastApplyTick			;//上一次申请的时间戳
	std::string strFactionName		;//帮会名字
	uint32 uFactionID				;//帮会id
	_stApplicant()	{init();	}
	void	init ()
	{
		strName = "";
		uPlayerid		= 0;		//player id
		uLevel			= 0;		//等级
		uJob			= 0;		//职业
		uFightpower		= 0;		//战斗力
		uRelationship	= 0;		//社会关系
		uLastApplyTick  = 0;		//上一次申请的时间戳
		strFactionName	= "";		//帮会名字
		uFactionID		= 0;		//帮会id
	}
	void		loadFromProtobuf		(const PBTeamApplicant&pbInfo);
	void		saveToProtobuf			(PBTeamApplicant&pbInfo);

};
//-------------------------------------------------------------
//------------------------------
struct _stTeamTask
{
	uint32	uID;		//玩法ID
	uint32	uNpcID;		//当前的任务ID
	uint32	uCopymapID;	//副本ID
	uint8	uState;		//副本ID	_emTeamTaskState

	_stTeamTask	()	{	init();								}
	void init	()	{	memset(this,0,sizeof(*this));		}
	void		loadFromProtobuf	(const PBTeamTask&pbInfo);
	void		saveToProtobuf		(PBTeamTask&pbInfo);
};
#pragma pack(pop)
//-------------------------------------------------------------
//------------------------------	 // 队伍信息
class PBTeamInfo;
class PBTeamInfoSimple;
class PBGetApplicantListAck;
class PBSetAutoMatchJobLimitAck;
struct _stTeamInfo : public _stTeamInfoBase
{
	stl_map<uint32,_stTeamMember>							mapMember;			//成员列表
	_stTeamTarget											m_stTeamTarget;		//组队目标
	_stTeamTask												m_stTeamTask;		//组队任务
	typedef std::vector<_stTeamMemberJobRequire>			VECJOBREQUIRE;
	VECJOBREQUIRE											m_vcJobRequire;		//空闲位置职业需求信息
	typedef 	stl_map<uint32, _stApplicant> MAP_PLAYERID_APPLICANT;
	MAP_PLAYERID_APPLICANT									m_mpApplicants;		//申请人集合信息
	_stTeamInfo()		{	init();							}
	void	init()
	{
		initBase();
		mapMember.clear();
		m_stTeamTarget.init();
		m_vcJobRequire.clear();
		m_mpApplicants.clear();
		m_stTeamTask.init();
	}

	void		loadFromProtobuf(const PBTeamInfo&pbInfo);
	void		saveToProtobuf	 (PBTeamInfo&pbInfo);
	void		saveToProtobuf	 (PBTeamInfoSimple&pbInfo);
	
	bool set_jobRequire(_stTeamMemberJobRequire& req);
	bool build_applicantList(PBGetApplicantListAck& ack);
	bool get_allApplicant(std::vector<uint32> & vcAllPlayerId);
	bool add_applicant(_stApplicant& applicant);
	bool clear_applicantList();
	bool check_applicant(uint32 uPlayerId);
	bool remove_applicant(uint32 uPlayerId);
	bool canMatchTarget(_stPlayerDisplay& stPlayerDisplay);
	bool adjustJobRequire(uint32 uNewMemberJob);
	bool build_jobRequires(PBSetAutoMatchJobLimitAck& ack);
};
