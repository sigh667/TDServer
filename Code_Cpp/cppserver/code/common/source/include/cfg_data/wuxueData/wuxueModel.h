/*----------------- wuxueModel.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/10/27 20:40:21
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
struct _stWuxueBase
{
	uint8	uJob;	//职业ID _em_GameUnitJob
	uint8	uPage;	//页签

	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stWuxueModel : _stWuxueBase
{
	VEC_U32	vecSkillID;		//技能ID

	void initModel()	
	{ 
		initBase();
		vecSkillID.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stWuxueBase::read(stream)&&
			stream.read(vecSkillID);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stWuxueBase::write(stream)&&
			stream.write(vecSkillID);
	}
};
