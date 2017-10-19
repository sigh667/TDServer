/*----------------- wulueModel.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/7/28 16:45:02
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
struct _stWulueYaodianBase
{
	uint16		uYaodianID;			//要点ID
	uint8		uWulueType;			//武略类型 风林火山
	uint8		uZhanlueLevel;		//战略等级
	uint8		uYaodianType;		//要点类型 _emWulueYaodianType
	uint16		uPreYaodianID;		//前置要点ID
	uint8		uBingfuType;		//兵符类型 圆方

	void initBase()	{   memset(this,0,sizeof(*this));	};

	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 武略要点配置
struct _stWulueYaodianModel : _stWulueYaodianBase
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
		return _stWulueYaodianBase::read(stream)&&
			stream.read(dmapPreCondition)&&
			stream.read(mapNeedExpend)&&
			stream.read(stAttribute);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stWulueYaodianBase::write(stream)&&
			stream.write(dmapPreCondition)&&
			stream.write(mapNeedExpend)&&
			stream.write(stAttribute);
	}
};
