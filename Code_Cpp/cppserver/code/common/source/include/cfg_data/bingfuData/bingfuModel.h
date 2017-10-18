/*----------------- bingfuModel.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����
* Version      : V1.0
* Date         : 2016/8/1 12:04:31
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
struct _stBingfuBase
{
	uint32		uBingfuID;			//����ID
	uint16		uBingfuLevel;		//�����ȼ�
	uint8		uQuality;			//Ʒ��	_emItemQualityType
	uint8		uBingfuType;		//�������� _emBingfuType
	uint32		uTotalExp;			//��ǰ�ܾ���

	void initBase()	{   memset(this,0,sizeof(*this));	};

	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ ��������
struct _stBingfuModel : _stBingfuBase
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
		return _stBingfuBase::read(stream)&&
			stream.read(dmapPreCondition)&&
			stream.read(mapNeedExpend)&&
			stream.read(stAttribute);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stBingfuBase::write(stream)&&
			stream.write(dmapPreCondition)&&
			stream.write(mapNeedExpend)&&
			stream.write(stAttribute);
	}
};
