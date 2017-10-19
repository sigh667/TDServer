/*------------- jobModel.h
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����
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
	uint16				uLandAnimTick;		// ����ʱ��(����)
	uint16				uClimbAnimTick;		// ����ʱ��(����)
	float32				fFlyHeight;			// ���ո߶�
	float32				fModelRadius;		// ģ�Ͱ뾶

	_stJobModelBase&	getModelBase()			{	return *this;					}
	void				initModelBase()			{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------  
struct _stRecovery
{
	uint32				uStepTick;			// ���ʱ�����
	int32				nAddValue;			// ����ֵ(����Ϊ��)

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
