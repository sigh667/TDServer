/*------------- jobModel.h
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈腾
* Version      : V1.01
* Date         : 2016/02/23
*
*/ 
/***************************************************************
* 
***************************************************************/
#include "def_struct/stlDefine.h"
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 
struct _stJobModelBase
{
	uint8				uJob;
	uint16				uLandAnimTick;		// 倒地时长(毫秒)
	uint16				uClimbAnimTick;		// 爬起时长(毫秒)
	float32				fFlyHeight;			// 浮空高度
	float32				fModelRadius;		// 模型半径

	_stJobModelBase&	getModelBase()			{	return *this;					}
	void				initModelBase()			{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------  
struct _stRecovery
{
	uint32				uStepTick;			// 间隔时间毫秒
	int32				nAddValue;			// 增加值(可以为负)

	_stRecovery()			{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stJobModel : _stJobModelBase
{
	typedef	stl_map<uint8,_stRecovery> MAP_ENERGY_RECOVERY;
	typedef	stl_map<uint8,_stRecovery>::const_iterator ITER_MAP_ENERGY_RECOVERY;

	VEC_U8					vecEnergy;
	MAP_I32_I32				mapByAttack;
	MAP_I32_I32				mapUseSkill;
	MAP_I32_I32				mapFighting;
	MAP_I32_I32				mapNotFighting;
	MAP_U16_I32				mapHitAddEnergy;
	MAP_ENERGY_RECOVERY		mapTimeAddEnegy;
	std::string				strMoudleName;

	_stJobModel()				{ initModel();		}
	inline void	initModel()
	{
		initModelBase();
		vecEnergy.clear();
		mapByAttack.clear();
		mapUseSkill.clear();
		mapFighting.clear();
		strMoudleName.clear();
		mapNotFighting.clear();
		mapHitAddEnergy.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stJobModelBase::read(stream)&&
			stream.read(vecEnergy)&&
			stream.read(mapByAttack)&&
			stream.read(mapUseSkill)&&
			stream.read(mapFighting)&&
			stream.read(mapNotFighting)&&
			stream.read(mapHitAddEnergy)&&
			stream.read(mapTimeAddEnegy);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stJobModelBase::write(stream)&&
			stream.write(vecEnergy)&&
			stream.write(mapByAttack)&&
			stream.write(mapUseSkill)&&
			stream.write(mapFighting)&&
			stream.write(mapNotFighting)&&
			stream.write(mapHitAddEnergy)&&
			stream.write(mapTimeAddEnegy);
	}
};
