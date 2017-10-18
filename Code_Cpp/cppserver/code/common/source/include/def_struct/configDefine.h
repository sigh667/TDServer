/*----------------- configDefine.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/8/13 10:41:23
*--------------------------------------------------------------
*配置定义
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
/*************************************************************/
#define DATE_TIME_STRING_MAX	20	//日期时间字符串长度
//-------------------------------------------------------------
//------------------------------ 游戏开关
enum _emGameSwitch
{
	//系统相关
	GameSwitch_ClientGM			,//客户端GM指令
	GameSwitch_Adult			,//防沉迷
	GameSwitch_DebugMsg			,//是否打印网络消息

	GameSwitch_Max
};

extern const pc_str g_strGameSwitch[GameSwitch_Max];
