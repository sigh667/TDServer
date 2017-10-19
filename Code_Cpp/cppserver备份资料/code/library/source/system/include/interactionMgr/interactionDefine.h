/*----------------- interactionDefine.h
*
* Copyright (C): 2011  LiuLei
* Author       : 刘雷
* Version      : V1.0
* Date         : 2012/4/3 17:30:49
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 交互事件
enum _emInteractionEvent
{
	_IEvent_Trade		,//交易
	_IEvent_Friend		,//好友

	_IEvent_Max
};

//-------------------------------------------------------------
//------------------------------ 组队交互
enum _emInteractionTeam
{
	_ITeam_Build	,//创建
	_ITeam_Invite	,//邀请
	_ITeam_Apply	,//请求
	_ITeam_Head		,//设置队长
	_ITeam_Max
};

//-------------------------------------------------------------
//------------------------------ 交易交互
enum _emInteractionTrade
{
	_ITrade_Build	,//邀请
	_ITrade_Max
};
//-------------------------------------------------------------
//------------------------------ 帮会交互
enum _emInteractionFaction
{
	_IFaction_Invite	,//邀请
	_IFaction_Apply		,//请求
	_IFaction_Federal	,//联邦
	_IFaction_Convene	,//召集

	_IFaction_Max
};
//-------------------------------------------------------------
//------------------------------ 世界
enum _emInteractionWorld
{
	_IWorld_Migrate	,//移民

	_IWorld_Max
};
//-------------------------------------------------------------
//------------------------------ 城池
enum _emInteractionCity
{
	_ICity_WarRemind	,//城战提醒

	_ICity_Max
};

//-------------------------------------------------------------
//------------------------------ 婚姻事件
enum _emInteractionMarriage
{
	_IMarriage_Propose	,//求婚
	_IMarriage_Divorce	,//离婚
	_IMarriage_Wedding	,//婚礼
	_IMarriage_UpCar	,//上车

	_IMarriage_Max
};

//-------------------------------------------------------------
//------------------------------ 色子
enum _emInteractionDice
{
	_IDice_Help	,//帮助重置

	_IDice_Max
};
