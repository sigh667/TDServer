/*----------------- titleModel.h
*
* Copyright (C): 2017  Mokylin��Mokyqi
* Author       : ����
* Version      : V1.0
* Date         : 2017/3/15 10:49:49
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
struct _stTitleBase
{
	uint32	uID;			//�ƺ�ID
	uint8	uType;			//�ƺ����� ս�� � ����
	uint32	uSortID;		//����ID(ԽСԽ��ǰ)
	uint32	uContinueTime;	//����ʱ���룩0��ʾ����
	bool	bHide;			//�Ƿ����� true����

	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stTitleModel : _stTitleBase
{
	MAP_U8_U32			mapCondition;		//����ͬʱ����
	_stGameAttribute	stAttribute;		//���ӵ�����

	void initModel()	
	{ 
		initBase();
		mapCondition.clear();
		stAttribute.initGameAttribute();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stTitleBase::read(stream)&&
			stream.read(mapCondition)&&
			stream.read(stAttribute);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stTitleBase::write(stream)&&
			stream.write(mapCondition)&&
			stream.write(stAttribute);
	}
};
