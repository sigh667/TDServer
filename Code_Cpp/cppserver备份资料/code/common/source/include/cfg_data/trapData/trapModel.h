/*----------------- trapModel.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����Դ
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
	TrapMark_IsBeAtkable	,//�Ƿ��ܱ�����
	TrapMark_MultiDmg		,//�Ƿ�Ե���Ŀ����ɶ���˺�
	TrapMark_IsThrough		,//�Ƿ�ɴ�͸�ϰ���
	TrapMark_IsHitDelete	,//�Ƿ����е���ɾ��
	TrapMark_IsHitChangeCamp,//�Ƿ����иı���Ӫ

	TrapMark_Max
};
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 
struct _stTrapModelBase
{
	uint32				uTrapID;
	uint16				uTapLevel;		//�ȼ�
	uint16				uTapMaxLevel;	//���ȼ�
	uint32				uExistTimeMS;	//����ʱ��
	uint32				uMark;			//����_emTrapMark
	uint8				uMoveType;		//�ƶ��켣����_emTrapMoveType
	float32				fDistance;		//���
	uint32				uCampID;		//��ӪID
	uint32				uStayTime;		//�������ͣ��ʱ��
	uint32				uDelayMoveTick;	//�ӳٷ���ʱ��
	float32				fRadius;		//�ӵ��뾶
	uint8				uStopType;		//ֹͣ���� _emTrapStopType
	uint32				uPenNumber;		//�ɴ�͸����
	uint32				uDelayDestroyTick;	//��ʱ����ʱ�� 0ֱ������
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
	uint32	uSkillID;		// ����ID
	uint32	uFirstAffectMS; // �״�ʹ���ӳ�(����)
	uint32	uIntervalMS;	// ʹ��Ƶ��(����)
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
	MAP_U32_U32				mapSpareParam;		//���ɲ��� key:_emTrapSpareParamType

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