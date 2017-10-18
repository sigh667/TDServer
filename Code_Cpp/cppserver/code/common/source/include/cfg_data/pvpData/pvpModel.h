/*----------------- pvpModel.h
*
* Copyright (C): 2017  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2017/4/21 16:22:24
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
#include "basic/basicTypes.h"
#include "def_struct/stlDefine.h"
#include "def_struct/gameStruct.h"
#include "math/range.h"
#include "basic/systemFunction.h"
#include "timer/dateTime.h"
/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 
struct _stPvpBase
{
	uint32			uID;				//身份任务ID
	uint32			uMaxEnterCount;		//最大进入次数
	uint32			uNeedPlayerNum;		//每队需要多少人
	uint32			uExitReadyCoolTime;	//退出准备冷却时间秒
	uint16			uCopymapID;			//进入的副本ID
	uint32			uStepMatchTime;		//匹配检查时间
	uint32			uReadyTime;			//准备时间

	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stSinglePvpPrizeBase
{
	uint32			uStartScore;		//开始的分数
	uint32			uID;				//ID
	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stPvpModel : _stPvpBase
{
	stl_vector<_stDateTime>		vecOpenTime;		// 开启时间
	DMAP_UINT8_32_32			dmapPreCondition;	//前提条件
	MAP_NEED_EXPEND				mapNeedExpend;		//需要消耗
	DMAP_NEED_ITEM				mapNeedItem;		//需要道具

	void initModel()	
	{ 
		initBase();
		vecOpenTime.clear();
		dmapPreCondition.clear();
		mapNeedExpend.clear();
		mapNeedItem.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stPvpBase::read(stream)&&
			stream.read(vecOpenTime)&&
			stream.read(dmapPreCondition)&&
			stream.read(mapNeedExpend)&&
			stream.read(mapNeedItem);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stPvpBase::write(stream)&&
			stream.write(vecOpenTime)&&
			stream.write(dmapPreCondition)&&
			stream.write(mapNeedExpend)&&
			stream.write(mapNeedItem);
	}
	inline bool		isInOpenTime	(uint64 uTime) const 
	{
		if ( vecOpenTime.empty() )
			return true;

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
//-------------------------------------------------------------
//------------------------------ 
struct _stSinglePvpPrizeModel : _stSinglePvpPrizeBase
{
	DMAP_NEED_ITEM				mapFirstAddItem;	//首次增加道具
	DMAP_NEED_ITEM				mapWeekAddItem;		//周增加道具

	void initModel()	
	{ 
		initBase();
		mapFirstAddItem.clear();
		mapWeekAddItem.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stSinglePvpPrizeBase::read(stream)&&
			stream.read(mapFirstAddItem)&&
			stream.read(mapWeekAddItem);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stSinglePvpPrizeBase::write(stream)&&
			stream.write(mapFirstAddItem)&&
			stream.write(mapWeekAddItem);
	}
};
//-------------------------------------------------------------
//------------------------------ 
struct _stSinglePvpDayPrizeModel 
{
	DMAP_NEED_ITEM				mapAddItem;	//增加道具

	void initModel()	
	{ 
		mapAddItem.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return 		stream.read(mapAddItem);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return 		stream.write(mapAddItem);
	}
};
