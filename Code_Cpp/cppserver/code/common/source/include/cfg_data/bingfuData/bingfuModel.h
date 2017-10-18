/*----------------- bingfuModel.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/8/1 12:04:31
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
struct _stBingfuBase
{
	uint32		uBingfuID;			//兵符ID
	uint16		uBingfuLevel;		//兵符等级
	uint8		uQuality;			//品质	_emItemQualityType
	uint8		uBingfuType;		//兵符类型 _emBingfuType
	uint32		uTotalExp;			//当前总经验

	void initBase()	{   memset(this,0,sizeof(*this));	};

	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 兵符配置
struct _stBingfuModel : _stBingfuBase
{
	DMAP_UINT8_32_32	dmapPreCondition;			// 前提条件
	MAP_NEED_EXPEND		mapNeedExpend;				// 扣除的消耗
	_stGameAttribute	stAttribute;

	void initModel()	
	{ 
		initBase();
		dmapPreCondition.clear();
		mapNeedExpend.clear();
		stAttribute.initGameAttribute();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stBingfuBase::read(stream)&&
			stream.read(dmapPreCondition)&&
			stream.read(mapNeedExpend)&&
			stream.read(stAttribute);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stBingfuBase::write(stream)&&
			stream.write(dmapPreCondition)&&
			stream.write(mapNeedExpend)&&
			stream.write(stAttribute);
	}
};
