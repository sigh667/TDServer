/*----------------- corpsModel.h
*
* Copyright (C): 2017  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2017/6/9 16:50:36
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
#include "basic/basicTypes.h"
#include "stream/netStream.h"
#include "def_struct/stlDefine.h"
#include "basic/systemFunction.h"
/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 
struct _stCorpsJobBase
{
	uint32			uCorpsJob;			//军团职位 _emCorpsJob
	uint32			uAddTitle;			//增加称号ID
	uint32			uAddMountID;		//增加坐骑ID
	uint32			uSetCoolTime;		//设置当前职位冷却时间
	uint32			uForbitSpeakTime;	//禁言时间秒
	uint32			uForbitSpeakHourNum;//每小时禁言的人数

	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stCorpsJobModel : _stCorpsJobBase
{
	SET_U32		setJobSet;				// 设置权限的职位
	SET_U32		setForbitSpeakJob;		// 能禁言的职位

	void initModel()	
	{ 
		initBase();
		setJobSet.clear();
		setForbitSpeakJob.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stCorpsJobBase::read(stream)&&
			stream.read(setJobSet)&&
			stream.read(setForbitSpeakJob);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stCorpsJobBase::write(stream)&&
			stream.write(setJobSet)&&
			stream.write(setForbitSpeakJob);
	}
};
