/*----------------- prizeModel.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2016/4/21 14:30:25
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
#include "def_struct/stlDefine.h"
#include "stream/streamBase.h"
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------
struct _stPrizeModelBase
{
	uint32				uPrizeID;		//编号
	uint8				uType;			//类型 _emPrizeType
	uint32				uSubType;		//子类型
	uint8				uAcceptType;	//领取类型 _emPrizeAcceptType
	uint8				uKeyType;		//key 副本:_emKeyCopymapType

	inline void					initPrizeModelBase()	{	memset(this,0,sizeof(*this));			}
	inline _stPrizeModelBase&	getPrizeModelBase()		{	return *this;							}
	READ_WRITE_STRUCT
};

struct _stPrizeByJob
{
	uint32 uItemID;
	uint32 uCount;
	uint32 uJob;
	bool parse(std::string s)
	{
		uItemID = (uint32)readInt64FromString(s, '_');
		uCount = (uint32)readInt64FromString(s, '_');
		uJob = (uint32)readInt64FromString(s, '_');
		return true;
	}
	void initZero(){	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};


/*************************************************************/
#pragma pack(pop)
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stPrizeModel	: _stPrizeModelBase
{
	MAP_NEED_EXPEND			mapAddExpend;	//增加值
	stl_map<uint32,uint32>	mapItem;		//道具
	stl_vector<_stPrizeByJob> vcItemByJob;	//按照职业获得的道具

	_stPrizeModel&		getPrizeModel()							{	return *this;							}
	inline void			initPrizeModel()
	{
		initPrizeModelBase();
		mapAddExpend.clear();
		mapItem.clear();
		vcItemByJob.clear();
	}
	inline bool		read			(CStreamBase& stream)
	{
		return _stPrizeModelBase::read(stream)&&
			stream.read(mapItem)&&
			stream.read(mapAddExpend)&&
			stream.read(vcItemByJob);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stPrizeModelBase::write(stream)&&
			stream.write(mapItem)&&
			stream.write(mapAddExpend)&&
			stream.write(vcItemByJob);
	}
};

//-------------------------------------------------------------
