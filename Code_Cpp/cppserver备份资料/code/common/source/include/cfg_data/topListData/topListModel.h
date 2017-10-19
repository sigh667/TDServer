/*----------------- topListData.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 方声辉
* Version      : V1.0
* Date         : 2017/06/06 15:46:16
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/memoryFunctions.h"
#include "math/range.h"
#include "def_struct/stlDefine.h"
#include "math/point2.h"
#include "basic/basicTypes.h"
#include "def_struct/gameStruct.h"
/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------  长城试炼
struct _strTopListModel	
{
	uint32					uType;				// 排行榜类型
	uint32					uRecordLine;		// 最大记录数
	uint32					uTimeReset;			// 重置类型
	
	_strTopListModel&			getTopListModel()					{	return *this;					}
	void						initTopListModel()					{	memset(this,0,sizeof(*this));	}
    
	READ_WRITE_STRUCT
};

struct _strTopListRewardBase
{
	uint32  uID;
	uint32	uType;			// 排行类型
	uint32	uRank;			// 排名 
	
	_strTopListRewardBase & getTopListRewardBase()	{	return *this;	};
	void					initTopListBase()		{	memset(this, 0, sizeof(*this));	}

	READ_WRITE_STRUCT
};
#pragma pack(pop)

//-------------------------------------------------------------
//------------------------------

struct _strTopListRewardModel : _strTopListRewardBase
{
	stl_map<uint32,uint32>	mapItem;		// 奖励

	_strTopListRewardModel &	getTopListRewardModel()	{	return *this;		}

	inline void initTopListRewardModel()
	{
		initTopListBase();
		mapItem.clear();
	}

	inline bool read	(CStreamBase & stream)
	{
		return _strTopListRewardBase::read(stream)		&&
			stream.read(mapItem);
	}

	inline bool write	(CStreamBase& stream)
	{
		return _strTopListRewardBase::write(stream)		&&
			stream.write(mapItem);
	}
};

