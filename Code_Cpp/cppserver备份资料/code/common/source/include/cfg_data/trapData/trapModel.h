/*----------------- trapModel.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2016/2/23 10:57:35
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "def_struct/gameStruct.h"
#include "stream/streamBase.h"
#include "def_struct/stlDefine.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
enum _emTrapMark
{
	TrapMark_IsBeAtkable	,//是否能被攻击
	TrapMark_MultiDmg		,//是否对单个目标造成多次伤害
	TrapMark_IsThrough		,//是否可穿透障碍物
	TrapMark_IsHitDelete	,//是否命中敌人删除
	TrapMark_IsHitChangeCamp,//是否命中改变阵营

	TrapMark_Max
};
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 
struct _stTrapModelBase
{
	uint32				uTrapID;
	uint16				uTapLevel;		//等级
	uint16				uTapMaxLevel;	//最大等级
	uint32				uExistTimeMS;	//生存时间
	uint32				uMark;			//掩码_emTrapMark
	uint8				uMoveType;		//移动轨迹类型_emTrapMoveType
	float32				fDistance;		//射程
	uint32				uCampID;		//阵营ID
	uint32				uStayTime;		//到达射程停留时间
	uint32				uDelayMoveTick;	//延迟飞行时间
	float32				fRadius;		//子弹半径
	uint8				uStopType;		//停止类型 _emTrapStopType
	uint32				uPenNumber;		//可穿透数量
	uint32				uDelayDestroyTick;	//延时销毁时间 0直接销毁
	_stTrapModelBase&	getTrapModelBase()				{	return *this;					}
	void				initTrapModelBase()				{	memset(this,0,sizeof(*this));	}

	inline bool			check_mark(uint32 _mark)const	{	return _CHECK_BIT(uMark,_BIT32(_mark));	}
	inline void			set_mark(uint8 _mark,bool _set)	{	_SET_BIT32(uMark,_mark,_set);	}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stTrapSkillModel
{
	uint32	uSkillID;		// 技能ID
	uint32	uFirstAffectMS; // 首次使用延迟(毫秒)
	uint32	uIntervalMS;	// 使用频率(毫秒)
	_stTrapSkillModel()				{	memset(this,0,sizeof(*this));		}
	READ_WRITE_STRUCT
};
#pragma pack(pop)
typedef stl_map<uint32,_stTrapSkillModel>	MAP_TRAP_SKILL_MODEL;
//-------------------------------------------------------------
//------------------------------ 
struct _stVecParma
{
	VEC_U32 vecParam;

	_stVecParma()
	{
		vecParam.clear();
	}	
	inline bool		read			(CStreamBase& stream)
	{
		return 	stream.read(vecParam);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return 	stream.write(vecParam);
	}
};
typedef stl_vector<float32>	VEC_TRAP_PEN_DMG;
struct _stTrapModel : _stTrapModelBase
{
	typedef stl_map<uint8,_stVecParma> MAP_DESTROY;
	MAP_TRAP_SKILL_MODEL	mapSkillModel;
	VEC_TRAP_PEN_DMG		vecPenDmg;
	_stGameAttribute		stAttribute;
	MAP_DESTROY				mapDestroyAction;	//key:_emTrapDestroyType
	MAP_U32_U32				mapSpareParam;		//自由参数 key:_emTrapSpareParamType

	_stTrapModel&			getTrapModel()			{	return *this;					}
	void					initTrapModel()
	{
		initTrapModelBase();
		stAttribute.initGameAttribute();
	}	
	inline bool		read			(CStreamBase& stream)
	{
		return _stTrapModelBase::read(stream)&&
			stream.read(mapSkillModel)&&
			stream.read(vecPenDmg)&&
			stream.read(mapDestroyAction)&&
			stream.read(stAttribute)&&
			stream.read(mapSpareParam);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stTrapModelBase::write(stream)&&
			stream.write(mapSkillModel)&&
			stream.write(vecPenDmg)&&
			stream.write(mapDestroyAction)&&
			stream.write(stAttribute)&&
			stream.write(mapSpareParam);
	}
	inline uint32	findSpareParam(uint32 uType) const
	{
		const uint32* pValue = mapSpareParam.find_(uType);
		return pValue ? *pValue : 0;
	}

};