/*------------- jobModel.h
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 林驹
* Version      : V1.01
* Date         : 2016/10/11
*
*/ 
/***************************************************************
* 
***************************************************************/
#include "def_struct/stlDefine.h"
#include "def_struct/gameStruct.h"
#include "timer/dateTime.h"

#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 
struct _stFactionPrivilegeModelBase
{
	uint32				uFactionOpt;		// 帮会操作
	uint8				uFactionJob;		// 帮会职位

	_stFactionPrivilegeModelBase&	get()			{	return *this;					}
	void							init()			{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 帮会技能
struct _stFactionSkillModel
{
	uint32				uSkllID;				// 帮会技能ID
	uint32				uLevel;					// 帮会技能等级
	uint32				uNeedFactionLevel;		// 研究需要帮派等级
	uint32				uNeedSliver;			// 研究需要资金
	uint32				uNeedStone;				// 研究需要玉石
	uint32				uNeedTime;				// 研究时间（秒）
	uint32				uLearnNeedSliver;		// 学习需要的银两
	uint32				uLearnNeedContribution;	// 学习需要的帮贡
	_stGameAttribute	stAttr;					// 学习增加人物属性

	_stFactionSkillModel&			get()			{	return *this;					}
	void							init()			{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 捐献资金
struct _stFactionDonateSliverModel
{
	uint32				uID;					// 捐献ID
	uint32				uSliver;				// 捐献金额
	uint32				uContribution;			// 获得帮贡
	uint32				uFactionSliver;			// 帮会获得帮会资金
	uint32				uCount;					// 每日可捐献次数
	bool				bBroadCast;				// 是否在帮会频道广播

	_stFactionDonateSliverModel&	get()			{	return *this;					}
	void							init()			{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 帮会聚饮 常量配置
struct _stFactionDrinkWineConstantModelBase
{
	uint32				uNpcID;			// NPC ID
	float32				fPosX;			// NPC POSX
	float32				fPosY;			// NPC POSY
	float32				fAngleDir;		// NPC DIRECTION
	uint16				uRegionID;		// 饮酒区域ID

	_stFactionDrinkWineConstantModelBase&	get()		{	return *this;					}
	void								init()			{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};

//-------------------------------------------------------------
//------------------------------ 帮会聚饮
struct _stFactionDrinkWineModel
{
	uint32				uID;					// 帮会聚饮ID
	uint32				uNeedContribution;		// 所需帮贡
	uint32				uWisdom;				// 可获得的总韬略数
	uint32				uTimeInterval;			// 增加韬略间隔(秒)
	uint32				uWisdomInterval;		// 每次增加的韬略数

	_stFactionDrinkWineModel&	get()				{	return *this;					}
	void							init()			{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 帮派任务
struct _stFactionTaskModelBase
{
	uint32				uID;					// 任务ID
	uint32				uFactionLevel;			// 帮会等级
	uint32				uNeedCount;				// 需要材料总数
	uint32				uTypeCount;				// 需要材料种类数
	uint32				uAwardContribution;		// 奖励的帮贡
	uint32				uAwardWisdom;			// 奖励的韬略
	uint32				uFactionSliver;			// 增加的帮会资金
	uint32				uFactionBoom;			// 增加的帮会繁荣度

	_stFactionTaskModelBase&		get()			{	return *this;					}
	void							init()			{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 帮会等级配置
struct _stFactionLevelModel
{
	uint32				uLevel;					// 帮会等级
	uint32				uMemberCount;			// 最大帮派人数
	uint32				uEnergyMax;				// 行动力上限
	uint32				uNeedBoom;				// 升级需要繁荣度
	uint32				uNeedExperience;		// 升级需要威望

	_stFactionLevelModel&			get()			{	return *this;					}
	void							init()			{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 帮派职务
struct _stFactionJobModelBase
{
	uint32				uJob;					// 职务
	uint32				uCountLimit;			// 数量上限
	uint64				uDefaultPrivilegeMark;	// 默认开放权限
	uint64				uForbidPrivilegeMark;	// 不可修改权限

	_stFactionJobModelBase&		get()			{	return *this;					}
	void						init()			{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stFactionJobModel : _stFactionJobModelBase
{
	_stFactionJobModel()	{ init();	}
	inline void	init()
	{
		_stFactionJobModelBase::init();
	}

	inline bool read(CStreamBase& stream)
	{
		return _stFactionJobModelBase::read(stream);
	}
	inline bool write(CStreamBase& stream)
	{
		return _stFactionJobModelBase::write(stream);
	}
};
//-------------------------------------------------------------
//------------------------------ 
struct _stFactionPrivilegeModel : _stFactionPrivilegeModelBase
{
	_stFactionPrivilegeModel()	{ init();	}
	inline void	init()
	{
		_stFactionPrivilegeModelBase::init();
	}

	inline bool read(CStreamBase& stream)
	{
		return _stFactionPrivilegeModelBase::read(stream);
	}
	inline bool write(CStreamBase& stream)
	{
		return _stFactionPrivilegeModelBase::write(stream);
	}
};
//-------------------------------------------------------------
//------------------------------ 
struct _stFactionTaskModel : _stFactionTaskModelBase
{
	stl_vector<uint32>		vecItemID;
	_stFactionTaskModel()	{ init();	}
	inline void	init()
	{
		_stFactionTaskModelBase::init();
		vecItemID.clear();
	}

	inline bool read(CStreamBase& stream)
	{
		return _stFactionTaskModelBase::read(stream)
			&& stream.read(vecItemID);
	}
	inline bool write(CStreamBase& stream)
	{
		return _stFactionTaskModelBase::write(stream)
			&& stream.write(vecItemID);
	}
};
//-------------------------------------------------------------
//------------------------------ 
struct _stFactionDrinkWineConstantModel	: public _stFactionDrinkWineConstantModelBase
{
	stl_vector<_stDateTime>					vecOpenTime;				// 开启时间

	_stFactionDrinkWineConstantModel&		get			(){	return *this;					}
	inline void		init()
	{
		_stFactionDrinkWineConstantModelBase::init();
		vecOpenTime.clear();
	}
	inline bool		read			(CStreamBase& stream)
	{
		return _stFactionDrinkWineConstantModelBase::read(stream)&&
			stream.read(vecOpenTime);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stFactionDrinkWineConstantModelBase::write(stream)&&
			stream.write(vecOpenTime);
	}
	inline bool		isInOpenTime	(uint64 uTime) const 
	{
		if ( vecOpenTime.empty() )
			return false;

		uint32	uSize 	= vecOpenTime.size();
		for ( uint32 i = 0; i < uSize; ++i )
		{
			const  _stDateTime& stDataTime = vecOpenTime[i];
			if (stDataTime.toDataTime(uTime))
				return true;
		}
		return false;
	}
};