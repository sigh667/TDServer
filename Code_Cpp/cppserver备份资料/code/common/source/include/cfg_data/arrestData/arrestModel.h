/*----------------- arrestModel.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/11/14 10:49:02
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
/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 
struct _stArrestQuestBase
{
	uint32			uID;				//缉拿ID
	uint8			uCopymapDifficulty;	//副本难度 _emCopymapDifficulty
	_stRangeU32		stRangeTeamNum;		//队伍人数
	uint32			uQuestTime;			//任务时间
	uint32			uNpcID;				//npcid
	uint16			uNeedLeaderIdentity;//需要身份
	uint32			uMaxEnterCount;		//进入最大次数
	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stArrestNpcBase
{
	uint32			uNpcID;				//缉拿npcID
	uint32			uNpcMapID;			//缉拿地图ID
	uint32			uTotleRate;			//总概率

	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stArrestQuestModel : _stArrestQuestBase
{
	DMAP_UINT8_32_32	dmapPreCondition;		//前提条件
	MAP_NEED_EXPEND		mapNeedExpend;			// 扣除的消耗
	MAP_NEED_ITEM		mapNeedItem;			//需要特殊道具

	void initModel()	
	{ 
		initBase();
		dmapPreCondition.clear();
		mapNeedExpend.clear();
		mapNeedItem.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stArrestQuestBase::read(stream)&&
			stream.read(dmapPreCondition)&&
			stream.read(mapNeedExpend)&&
			stream.read(mapNeedItem);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stArrestQuestBase::write(stream)&&
			stream.write(dmapPreCondition)&&
			stream.write(mapNeedExpend)&&
			stream.write(mapNeedItem);
	}
};
//-------------------------------------------------------------
//------------------------------ 
struct _stArrestNpcModel : _stArrestNpcBase
{
	MAP_U32_U8	mapArrestRate;		//缉拿权重比 key:权重 value:副本类型_emCopymapType
	void initModel()	
	{ 
		initBase();
		mapArrestRate.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stArrestNpcBase::read(stream)&&
			stream.read(mapArrestRate);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stArrestNpcBase::write(stream)&&
			stream.write(mapArrestRate);
	}
	const uint8	getRandCopymapType	()const
	{
		uint32 uRandNum = randRange(uTotleRate);
		CONST_ITER_MAP_U32_U8 _pos = mapArrestRate.lower_bound(uRandNum); //第一个大于等于uLevel
		return _pos != mapArrestRate.end() ? _pos->second : 0;
	}
};
