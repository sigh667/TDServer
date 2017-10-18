/*----------------- titleModel.h
*
* Copyright (C): 2017  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2017/3/15 10:49:49
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
struct _stTitleBase
{
	uint32	uID;			//称号ID
	uint8	uType;			//称号类型 战力 活动 任务
	uint32	uSortID;		//排序ID(越小越靠前)
	uint32	uContinueTime;	//持续时（秒）0表示永久
	bool	bHide;			//是否隐藏 true隐藏

	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stTitleModel : _stTitleBase
{
	MAP_U8_U32			mapCondition;		//条件同时满足
	_stGameAttribute	stAttribute;		//增加的属性

	void initModel()	
	{ 
		initBase();
		mapCondition.clear();
		stAttribute.initGameAttribute();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stTitleBase::read(stream)&&
			stream.read(mapCondition)&&
			stream.read(stAttribute);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stTitleBase::write(stream)&&
			stream.write(mapCondition)&&
			stream.write(stAttribute);
	}
};
