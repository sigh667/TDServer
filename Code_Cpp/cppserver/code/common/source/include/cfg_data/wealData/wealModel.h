/*----------------- wealModel.h
*
* Copyright (C): 2017  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2017/6/15 16:28:48
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
#include "basic/basicTypes.h"
#include "stream/netStream.h"
#include "def_struct/stlDefine.h"
#include "basic/systemFunction.h"
#include "timer/dateTime.h"
/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 
struct _stLoginWealBase
{
	uint32			uID;				//
	uint32			uDayCount;			//登陆天数
	
	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stPrizeWealBase
{
	uint32			uID;				//
	uint32			uDay;				//天数

	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stLoginWealModel : _stLoginWealBase
{
	stl_vector<_stDateTime>		vecOpenTime;		// 开启时间

	void initModel()	
	{ 
		initBase();
		vecOpenTime.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stLoginWealBase::read(stream)&&
			stream.read(vecOpenTime);
	}

	inline bool		write			(CStreamBase& stream)
	{
		return _stLoginWealBase::write(stream)&&
			stream.write(vecOpenTime);
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
struct _stPrizeWealModel : _stPrizeWealBase
{
	DMAP_NEED_ITEM	dmapAddItem;		//奖励道具

	void initModel()	
	{ 
		initBase();
		dmapAddItem.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stPrizeWealBase::read(stream)&&
			stream.read(dmapAddItem);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stPrizeWealBase::write(stream)&&
			stream.write(dmapAddItem);
	}
};