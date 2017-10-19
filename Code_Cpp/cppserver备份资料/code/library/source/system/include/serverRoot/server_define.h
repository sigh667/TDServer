/*----------------- server_define.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/8/1 18:17:20
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
enum _em_ServerType
{
	ServerType_None			,

	ServerType_Login		,//登录服务器
	ServerType_Register		,//注册服务器
	ServerType_DataBase		,//DB服务器
	ServerType_World		,//世界服务器
	ServerType_Global		,//全局服务器
	ServerType_Game			,//游戏服务器
	ServerType_BigWorld		,//跨服服务器
	ServerType_Rooter		,//路由服务器
	ServerType_Robot		,//机器人服务器
	ServerType_Dip			,//后台服务器
	ServerType_Record		,//记录服务器

	ServerType_Max
};

#define BIG_WORLD_LOGIC_ID 60000