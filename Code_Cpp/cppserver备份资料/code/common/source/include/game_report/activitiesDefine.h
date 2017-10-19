/*----------------- activitiesDefine.h
*
* Copyright (C): 2013  Mokylin·Mokyqi
* Author       : 陶建平
* Version      : V1.0
* Date         : 2013/11/4 17:45:40
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
/*************************************************************/
//#############################################################
//##############################上报数据记录类型
//#############################################################
enum _emActivitiesType
{
	Activities_Type_Mount			,	//坐骑
	Activities_Type_RidingWeapon	,	//骑兵
	Activities_Type_Souls			,	//四灵
	Activities_Type_Bow				,	//弓箭
	Activities_Type_General			,	//美人
	Activities_Type_Precious		,	//宝石
	Activities_Type_CheckInCount	,	//签到次数
	Activities_Type_Equip			,	//装备
	Activities_Type_Fighters		,	//八门遁甲
	Activities_Type_Keepsake		,	//玉佩
	Activities_Type_Base			,	//角色基本信息表
	Activities_Type_MountEquip		,	//坐骑装备
	Activities_Type_CopyMap			,	//副本
	Activities_Type_Potential		,	//角色潜能
	Activities_Type_Skill			,	//角色技能
	Activities_Type_GeneralWeapon	,	//武将神兵
	Activities_Type_ItemRecord		,	//道具使用
	Activities_Type_City			,	//城池
	Activities_Type_MagicWeapon		,	//龙城飞将
	Activities_Type_DrugTripod		,	//药鼎
	Activities_Type_Banner			,	//旌旗
	Activities_Type_Spirit			,	//武魂
	Activities_Type_GiftActivities	,	//礼品活动

	Activities_Max
};

//-------------------------------------------------------------
//------------------------------ 

static pc_str	g_strActivitiesType[Activities_Max]=
{
	"t_mount"			,
	"t_ridingWeapon"	,
	"t_souls"			,
	"t_bow"				,
	"t_general"			,
	"t_precious"		,
	"t_checkInCount"	,
	"t_equip"			,
	"t_fighters"		,
	"t_keepsake"		,
	"t_base"			,
	"t_mountEquip"		,
	"t_copyMap"			,
	"t_potential"		,
	"t_skill"			,
	"t_generalWeapon"	,
	"t_itemRecord"		,
	"t_city"			,
	"t_magic_weapon"	,
	"t_drugTripod"		,
	"t_banner"			,
	"t_spirit"			,
	"t_gift_activities"	,
};
