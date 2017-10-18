/*----------------- registerDefine.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/9/6 18:00:37
*--------------------------------------------------------------
*注册事件
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 注册事件
enum _em_RegisterEvent
{
	RegEvent_RegisterPlayer,	//注册角色名字
	RegEvent_RegisterFaction,	//注册帮派
	RegEvent_DeleteFaction,		//删除帮派
	RegEvent_CheckPlayerName,	//检测角色名字
	RegEvent_Max
};
