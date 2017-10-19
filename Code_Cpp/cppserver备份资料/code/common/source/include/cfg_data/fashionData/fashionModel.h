/*----------------- fashionModel.h
*
* Copyright (C): 2017  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2017/3/30 18:09:55
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
struct _stFashionBase
{
	uint32	uID;			//时装ID
	uint32	uType;			//时装类型

	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stFashionModel : _stFashionBase
{
	void initModel()	
	{ 
		initBase();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stFashionBase::read(stream);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stFashionBase::write(stream);
	}
};
