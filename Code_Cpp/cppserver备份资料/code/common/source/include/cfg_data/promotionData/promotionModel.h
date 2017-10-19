/*------------- promotionModel.h
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 林驹
* Version      : V1.01
* Date         : 2016/09/20
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "def_struct/stlDefine.h"
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 
struct _stPromotionModelBase
{
	uint8				uJob;				// 职业
	uint8				uStep;				// 晋升阶数
	uint32				uOpenHour;			// 开服多少小时后开放晋升
	uint32				uNeedLevel;			// 晋升需要等级
	uint32				uQuest;				// 晋升任务ID

	inline _stPromotionModelBase&	get()		{	return *this;					}
	inline void						init()		{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stPromotionModel : _stPromotionModelBase
{
	_stPromotionModel()	{ init();	}
	inline void	init()
	{
		_stPromotionModelBase::init();
	}

	inline bool read(CStreamBase& stream)
	{
		return _stPromotionModelBase::read(stream);
	}
	inline bool write(CStreamBase& stream)
	{
		return _stPromotionModelBase::write(stream);
	}
};
