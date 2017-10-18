/*----------------- systemSwitchMode.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 方声辉
* Version      : V1.0
* Date         : 2016/09/21 13:39:53
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
//------------------------------ 系统功能开关
struct _stSystemSwitchBase
{
	uint32					uID;				// ID 
	int8					MainProtocol;		// 大类型（对应MainPorctocalID） 
	uint16					uLevel;				// 条件1 等级
	uint32					uQuestID;			// 条件2 任务ID
	uint16					uVip;				// 条件3 VIP等级
	uint8					uSwitch;			// 全局开关  

	_stSystemSwitchBase&	getBase()	{	return *this;					}
	inline void				initBase()	{	dMemset(this,0,sizeof(*this));	}

	READ_WRITE_STRUCT

};
#pragma pack(pop)
//-------------------------------------------------------------
//------------------------------

struct _stSystemSwitchModel	:_stSystemSwitchBase
{
	SET_U8		setSubProtocol;

	_stSystemSwitchModel&		getSystemSwitchModel()			{	return *this;					}
	
	void						initSystemSwitchModel()	
	{	initBase();
		setSubProtocol.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stSystemSwitchBase::read(stream)&&
			stream.read(setSubProtocol);
	}
	
	inline bool		write			(CStreamBase& stream)
	{
		return _stSystemSwitchBase::write(stream)&&
			stream.write(setSubProtocol);
	}
	
};

//-------------------------------------------------------------
//------------------------------ 