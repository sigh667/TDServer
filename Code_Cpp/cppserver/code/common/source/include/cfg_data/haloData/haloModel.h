/*----------------- haloModel.h
*
* Copyright (C): 2015  Mokylin·Mokyqi
* Author       : 陈腾
* Version      : V1.0
* Date         : 2015/3/10 18:21
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
struct _stHaloModelBase
{
	uint16				uHaloID;						//编号
	uint16				uRadius;						//作用半径

	inline void					initHaloModelBase()		{	memset(this,0,sizeof(*this));			}
	inline _stHaloModelBase&	getHaloModelBase()		{	return *this;							}
	READ_WRITE_STRUCT
};

/*************************************************************/
#pragma pack(pop)
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stHaloModel	: _stHaloModelBase
{
	VEC_U16				vecEffect;						//效果列表

	_stHaloModel&		getHaloModel()					{	return *this;	}
	inline void			initHaloModel()
	{
		initHaloModelBase();
		vecEffect.clear();
	}
	inline bool		read			(CStreamBase& stream)
	{
		return _stHaloModelBase::read(stream)&&
			stream.read(vecEffect);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stHaloModelBase::write(stream)&&
			stream.write(vecEffect);
	}
};

//-------------------------------------------------------------
