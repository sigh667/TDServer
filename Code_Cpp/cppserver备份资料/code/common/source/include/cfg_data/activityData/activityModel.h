/*----------------- activityMode.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 方声辉
* Version      : V1.0
* Date         : 2017/05/16 15:41:59
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "def_struct/gameStruct.h"
#include "stream/streamBase.h"
#include "def_struct/stlDefine.h"
#include "basic/basicTypes.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
//-------------------------------------------------------------
//------------------------------ 
#pragma pack(push,1)

struct _stActivityModel
{
	uint32 uID;				// 活动ID
	uint32 uType;			// 类型
	uint32 uCount;			// 次数
	uint32 uLiveness;		//增加活跃度
	uint32 uCompleteID;       //活动类型参数 完成对象ID
	

	_stActivityModel& getactivityModel()                {   return *this;					}
	void initactivityModel()							{   memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};

//
struct _stLivenessBass
{
	uint32 uID;				// ID
	uint32 uLiveness;       // 活跃度

	_stLivenessBass&  getlivenessBase()             {   return *this;					 }
	void              initlivenessBase()			{   memset(this,0,sizeof(*this));	 }

	READ_WRITE_STRUCT
};
#pragma pack(pop)

//-------------------------------------------------------------
//------------------------------

struct _stLivenessModel :_stLivenessBass
{
	stl_map<uint32,uint32>	mapItem;		// 奖励

	_stLivenessModel&		getLivenessModel()		{	return *this;						}
	
	inline void			initLivenessModel()
	{
		initlivenessBase();
		mapItem.clear();
	}
	
	inline bool		read			(CStreamBase& stream)
	{
		return _stLivenessBass::read(stream)&&
			stream.read(mapItem);
	}
	
	inline bool		write			(CStreamBase& stream)
	{
		return _stLivenessBass::write(stream)&&
			stream.write(mapItem);
	}
};




