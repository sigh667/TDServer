/*----------------- identityModel.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/9/18 16:48:25
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
#include "basic/basicTypes.h"
#include "def_struct/stlDefine.h"
#include "def_struct/gameStruct.h"
/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 
struct _stIdentityBase
{
	uint16	uID;				//身份ID
	uint16	uMaxSkillLevelDiff;	//技能相差的等级
	
	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stIdentityOpenBase
{
	uint16	uSetID;				//槽位ID
	uint16	uSkillExpendCount;	//技能消耗倍数
	bool	bMain;				//是否是主身份
	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stIdentityResetBase
{
	uint32	uCount;				//次数
	uint32	uCoolTime;			//冷却时间秒

	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stIdentitySkillBase
{
	uint32	uSkillID;			//技能ID
	uint16	uSkillLevel;		//技能等级
	uint32	uNeedSufferValue;	//需要历练值
	uint32	uTotalSufferValue;	//升到此级的总共历练值
	uint32	uDartSelfAddMoney;	//押镖个人增加金钱
	uint32	uArrestAddGuard;	//缉拿增加囚车几率
	uint32	uReardAddDaobao;	//悬赏增加盗宝小贼几率
	uint32	uTreasureAddBox;	//龙脉宝藏增加宝箱几率

	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stIdentityModel : _stIdentityBase
{
	DMAP_UINT8_32_32	dmapPreCondition;		//前提条件
	stl_set<uint32>		setSkillID;				//技能ID组

	void initModel()	
	{ 
		initBase();
		dmapPreCondition.clear();
		setSkillID.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stIdentityBase::read(stream)&&
			stream.read(dmapPreCondition)&&
			stream.read(setSkillID);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stIdentityBase::write(stream)&&
			stream.write(dmapPreCondition)&&
			stream.write(setSkillID);
	}
};
//-------------------------------------------------------------
//------------------------------ 
struct _stIdentityOpenModel : _stIdentityOpenBase
{
	DMAP_UINT8_32_32	dmapPreCondition;		// 前提条件
	MAP_NEED_EXPEND		mapNeedExpend;			// 扣除的消耗
	DMAP_NEED_ITEM		mapNeedItem;			//需要特殊道具

	void initModel()	
	{ 
		initBase();
		dmapPreCondition.clear();
		mapNeedExpend.clear();
		mapNeedItem.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stIdentityOpenBase::read(stream)&&
			stream.read(dmapPreCondition)&&
			stream.read(mapNeedExpend)&&
			stream.read(mapNeedItem);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stIdentityOpenBase::write(stream)&&
			stream.write(dmapPreCondition)&&
			stream.write(mapNeedExpend)&&
			stream.write(mapNeedItem);
	}
};
//-------------------------------------------------------------
//------------------------------ 
struct _stIdentityResetModel : _stIdentityResetBase
{
	MAP_NEED_EXPEND		mapNeedExpend;			// 扣除的消耗
	DMAP_NEED_ITEM		mapNeedItem;		//需要特殊道具

	void initModel()	
	{ 
		initBase();
		mapNeedExpend.clear();
		mapNeedItem.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stIdentityResetBase::read(stream)&&
			stream.read(mapNeedExpend)&&
			stream.read(mapNeedItem);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stIdentityResetBase::write(stream)&&
			stream.write(mapNeedExpend)&&
			stream.write(mapNeedItem);
	}
};
//-------------------------------------------------------------
//------------------------------ 
struct _stIdentitySkillModel : _stIdentitySkillBase
{
	typedef	stl_map<uint32,_stItemNum>		MAP_ADDITEMNUM;
	DMAP_UINT8_32_32			dmapPreCondition;		// 前提条件
	_stGameAttribute			stAttribute;			//增加的属性
	MAP_U32_U32					mapDartAddHonghuo;		//押镖红货出现提高几率 倍率_增加万分比
	MAP_U16_U32					mapCopymapAddRate;		//缉拿增加囚车副本几率	副本ID_增加万分比;
	MAP_U32_U32					mapRefreshUnitAddRate;	//增加副本刷怪几率		怪物ID_增加万分比;
	MAP_ADDITEMNUM				mapArrestAddItem;		//缉拿增加道具等级段_道具id_数量 注:小于等于此等级段
	MAP_ADDITEMNUM				mapRewardAddItem;		//悬赏增加道具等级段_道具id_数量 注:小于等于此等级段
	MAP_ADDITEMNUM				mapTreasureAddItem;		//宝藏增加道具等级段_道具id_数量 注:小于等于此等级段
	MAP_U32_U32					mapKillAddBuff;			//副本杀人获得buff	副本ID_buffid
	MAP_U32_U32					mapBeKillAddBuff;		//副本被杀人增加对方buff 副本ID_buffid;

	void initModel()	
	{ 
		initBase();
		dmapPreCondition.clear();
		stAttribute.initGameAttribute();
		mapDartAddHonghuo.clear();
		mapCopymapAddRate.clear();
		mapRefreshUnitAddRate.clear();
		mapArrestAddItem.clear();
		mapRewardAddItem.clear();
		mapTreasureAddItem.clear();
		mapKillAddBuff.clear();
		mapBeKillAddBuff.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stIdentitySkillBase::read(stream)&&
			stream.read(dmapPreCondition)&&
			stream.read(stAttribute)&&
			stream.read(mapDartAddHonghuo)&&
			stream.read(mapCopymapAddRate)&&
			stream.read(mapRefreshUnitAddRate)&&
			stream.read(mapArrestAddItem)&&
			stream.read(mapRewardAddItem)&&
			stream.read(mapTreasureAddItem)&&
			stream.read(mapKillAddBuff)&&
			stream.read(mapBeKillAddBuff);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stIdentitySkillBase::write(stream)&&
			stream.write(dmapPreCondition)&&
			stream.write(stAttribute)&&
			stream.write(mapDartAddHonghuo)&&
			stream.write(mapCopymapAddRate)&&
			stream.write(mapRefreshUnitAddRate)&&
			stream.write(mapArrestAddItem)&&
			stream.write(mapRewardAddItem)&&
			stream.write(mapTreasureAddItem)&&
			stream.write(mapKillAddBuff)&&
			stream.write(mapBeKillAddBuff);
	}
};
