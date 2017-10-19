/*----------------- gameDefine.h
*
* Copyright (C): 2011  LiuLei
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/9/13 14:13:17
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "protocol/GameDefine.pb.h"
#include "basic/basicTypes.h"
#include "def_struct/playerDefine.h"
/*************************************************************/
static const uint32 g_uBattleObject	= _BIT32(GameObject_Player) | _BIT32(GameObject_Monster)| _BIT32(GameObject_Pet)| _BIT32(GameObject_Trap);
//-------------------------------------------------------------
//------------------------------ 游戏Ghost对像
enum _em_GameGhost
{
	Ghost_Remove		,//移除此对像(从自己的视野中消失)
	Ghost_State			,//状态更新
	Ghost_Property		,//属性更新
	Ghost_HPMP			,//生命魔法更新
	Ghost_Move			,//对像移动
	Ghost_Base			,//基本数据更新
	Ghost_SuperArmor	,//霸体信息
	Ghost_Pk			,//Pk信息
	Ghost_FightPower	,//战斗力
	Ghost_Qinggong		,//轻功
	Ghost_Camp			,//阵营
	Ghost_Speed			,//移动速度

	Ghost_Max
};
enum _emUpdateValue
{
	_UV_AttributeBase	,//更新计算基本属性除buff外
	_UV_Shape			,//更新计算外形
	_UV_AttributeBuff	,//更新计算Buff属性
	_UV_TempSkill		,//更新临时技能
	_UV_PassiveSkillBuff,//更新被动技能增加的buff

	_UV_Max
};
#include "gameDefine_inline.h"
