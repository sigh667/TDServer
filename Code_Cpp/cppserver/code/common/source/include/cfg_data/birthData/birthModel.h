/*----------------- birthModel.h
*
* Copyright (C): 2016  Mokylin°§Mokyqi
* Author       : ’‘Œƒ‘¥
* Version      : V1.0
* Date         : 2016/1/7 13:53:21
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "def_struct/stlDefine.h"
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stBirthModelBase
{
	uint8				uJob;
	uint16				uSceneID;
	uint16				uSpawnID;
	float				fDirection;
	uint32				uQuestID;
	_stBirthModelBase&getBirthModelBase()	{	return *this;					}
	void			initBirthModelBase()	{	memset(this,0,sizeof(*this));	}
};
//-------------------------------------------------------------
//------------------------------ 
struct _stBirthModel	: _stBirthModelBase
{
	MAP_U8_U32		mapEquip;
	MAP_U32_U16		mapItem;
	VEC_U32			vecBirthSkill;

	void initBirthModel()
	{
		initBirthModelBase();
		mapItem.clear();
		mapEquip.clear();
		vecBirthSkill.clear();
	}
	
	inline bool		read			(CStreamBase& stream)
	{
		return stream.read(uJob)&&
			stream.read(uSceneID)&&
			stream.read(uSpawnID)&&
			stream.read(fDirection)&&
			stream.read(uQuestID)&&
			stream.read(mapEquip)&&
			stream.read(mapItem)&&
			stream.read(vecBirthSkill);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return stream.write(uJob)&&
			stream.write(uSceneID)&&
			stream.write(uSpawnID)&&
			stream.write(fDirection)&&
			stream.write(uQuestID)&&
			stream.write(mapEquip)&&
			stream.write(mapItem)&&
			stream.write(vecBirthSkill);
	}
};
