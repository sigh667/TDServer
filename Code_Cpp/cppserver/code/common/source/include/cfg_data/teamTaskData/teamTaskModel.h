/*----------------- teamTaskModel.h
*
* Copyright (C): 2017  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2017/4/1 11:18:47
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
struct _stTeamTaskBase
{
	uint32			uID;				//身份任务ID
	uint32			uTaskNpcID;			//接任务NPCID
	uint32			uMaxAcceptCount;	//最大任务次数
	_stRangeU32		stRangeTeamNum;		//队伍人数
	
	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stTeamTaskPrizeBase
{
	uint32			uID;				//玩法ID
	uint32			uCount;				//次数
	uint16			uLevel;				//等级

	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stTeamTaskNpc
{
	uint32			uNpcID;				//npcID
	uint32			uCopymapID;			//副本ID

	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stTeamTaskModel : _stTeamTaskBase
{
	typedef stl_map<uint32,_stTeamTaskNpc> MAP_NPC;

	MAP_NPC mapNpcRate;//权重比 key:总权重 value:任务ID
	DMAP_UINT8_32_32	dmapPreCondition;	//前提条件

	void initModel()	
	{ 
		initBase();
		mapNpcRate.clear();
		dmapPreCondition.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stTeamTaskBase::read(stream)&&
			stream.read(mapNpcRate)&&
			stream.read(dmapPreCondition);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stTeamTaskBase::write(stream)&&
			stream.write(mapNpcRate)&&
			stream.write(dmapPreCondition);
	}
	const _stTeamTaskNpc*	getRandNpc	()const
	{
		MAP_NPC::const_reverse_iterator _rpos = mapNpcRate.rbegin();
		if (_rpos == mapNpcRate.rend())
			return NULL;

		uint32 uTotleRate = _rpos->first;
		uint32 uRandNum = randRange(uTotleRate);
		MAP_NPC::const_iterator _spos = mapNpcRate.lower_bound(uRandNum); //第一个大于等于rate
		return _spos != mapNpcRate.end() ? &_spos->second : NULL;
	}
};
//-------------------------------------------------------------
//------------------------------ 
struct _stTeamTaskPrizeModel : _stTeamTaskPrizeBase
{
	MAP_NEED_EXPEND			mapAddExpend;	//增加值
	DMAP_NEED_ITEM			mapItem;		//道具
	DMAP_NEED_ITEM			mapLeaderItem;	//道具

	void initModel()	
	{ 
		initBase();
		mapAddExpend.clear();
		mapItem.clear();
		mapLeaderItem.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stTeamTaskPrizeBase::read(stream)&&
			stream.read(mapAddExpend)&&
			stream.read(mapItem)&&
			stream.read(mapLeaderItem);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stTeamTaskPrizeBase::write(stream)&&
			stream.write(mapAddExpend)&&
			stream.write(mapItem)&&
			stream.write(mapLeaderItem);
	}
};
