/*----------------- strongerMode.h
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

#pragma pack(push,1)

struct _stStrongerBase
{
	uint32 uID;						// 档次ID
	uint32 uFightPower;				// 战力
	

	_stStrongerBase&	getStrongerBase()           {   return *this;					}
	void				initStrongerBase()			{   memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};

#pragma pack(pop)

//-------------------------------------------------------------
//------------------------------

struct _stStrongerModel :_stStrongerBase
{
	stl_map<uint32,uint32>	mapItem;		// 奖励

	_stStrongerModel&		getStrongerModel()		{	return *this;					}
	
	inline void			initStongerModel()
	{
		initStrongerBase();
		mapItem.clear();
	}
	
	inline bool		read			(CStreamBase& stream)
	{
		return _stStrongerBase::read(stream)&&
			stream.read(mapItem);
	}
	
	inline bool		write			(CStreamBase& stream)
	{
		return _stStrongerBase::write(stream)&&
			stream.write(mapItem);
	}
};




