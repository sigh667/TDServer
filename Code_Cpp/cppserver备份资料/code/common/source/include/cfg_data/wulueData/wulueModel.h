/*----------------- wulueModel.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����
* Version      : V1.0
* Date         : 2016/7/28 16:45:02
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
#include "basic/basicTypes.h"
#include "def_struct/stlDefine.h"
#include "def_struct/gameStruct.h"
/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 
struct _stWulueYaodianBase
{
	uint16		uYaodianID;			//Ҫ��ID
	uint8		uWulueType;			//�������� ���ֻ�ɽ
	uint8		uZhanlueLevel;		//ս�Եȼ�
	uint8		uYaodianType;		//Ҫ������ _emWulueYaodianType
	uint16		uPreYaodianID;		//ǰ��Ҫ��ID
	uint8		uBingfuType;		//�������� Բ��

	void initBase()	{   memset(this,0,sizeof(*this));	};

	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ ����Ҫ������
struct _stWulueYaodianModel : _stWulueYaodianBase
{
	DMAP_UINT8_32_32	dmapPreCondition;			// ǰ������
	MAP_NEED_EXPEND		mapNeedExpend;				// �۳�������
	_stGameAttribute	stAttribute;

	void initModel()	
	{ 
		initBase();
		dmapPreCondition.clear();
		mapNeedExpend.clear();
		stAttribute.initGameAttribute();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stWulueYaodianBase::read(stream)&&
			stream.read(dmapPreCondition)&&
			stream.read(mapNeedExpend)&&
			stream.read(stAttribute);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stWulueYaodianBase::write(stream)&&
			stream.write(dmapPreCondition)&&
			stream.write(mapNeedExpend)&&
			stream.write(stAttribute);
	}
};
