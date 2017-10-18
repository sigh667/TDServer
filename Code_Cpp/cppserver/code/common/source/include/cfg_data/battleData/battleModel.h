/*----------------- battleModel.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2016/1/20 16:20:52
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "def_struct/stlDefine.h"
#include "stream/streamBase.h"
/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 
struct _stBattleAttrBase
{
	uint8			uAttrType;
	int32			nMinValue;
	int32			nMaxValue;
	uint8			uLayer;
	float32			fFightpower;//每一点属性增加的战斗力
	_stBattleAttrBase&	getBase()	{	return *this;					}
	void			initBase()		{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stBattleAttrAffect
{
	uint8			uAttrType;			//_emBattleAttribute
	uint8			nAffectType;		//_emBattleAttrAffectType
	float32			fAffectValue;
	_stBattleAttrAffect() {	memset(this,0,sizeof(*this));	}

	READ_WRITE_STRUCT
};
#pragma pack(pop)
//-------------------------------------------------------------
//------------------------------ 
struct _stBattleAttrModel : _stBattleAttrBase
{
	stl_vector<_stBattleAttrAffect> vecAffect;
	_stBattleAttrModel&	getModel()		{	return *this;					}
	void			initModel()	
	{
		initBase();
		vecAffect.clear();
	}
	inline bool		read			(CStreamBase& stream)
	{
		return _stBattleAttrBase::read(stream)&&
			stream.read(vecAffect);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stBattleAttrBase::write(stream)&&
			stream.write(vecAffect);
	}
};
