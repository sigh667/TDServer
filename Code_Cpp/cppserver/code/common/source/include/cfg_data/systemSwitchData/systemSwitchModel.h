/*----------------- systemSwitchMode.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ������
* Version      : V1.0
* Date         : 2016/09/21 13:39:53
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/memoryFunctions.h"
#include "math/range.h"
#include "def_struct/stlDefine.h"
#include "math/point2.h"
#include "basic/basicTypes.h"
#include "def_struct/gameStruct.h"
/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ ϵͳ���ܿ���
struct _stSystemSwitchBase
{
	uint32					uID;				// ID 
	int8					MainProtocol;		// �����ͣ���ӦMainPorctocalID�� 
	uint16					uLevel;				// ����1 �ȼ�
	uint32					uQuestID;			// ����2 ����ID
	uint16					uVip;				// ����3 VIP�ȼ�
	uint8					uSwitch;			// ȫ�ֿ���  

	_stSystemSwitchBase&	getBase()	{	return *this;					}
	inline void				initBase()	{	dMemset(this,0,sizeof(*this));	}

	READ_WRITE_STRUCT

};
#pragma pack(pop)
//-------------------------------------------------------------
//------------------------------

struct _stSystemSwitchModel	:_stSystemSwitchBase
{
	SET_U8		setSubProtocol;

	_stSystemSwitchModel&		getSystemSwitchModel()			{	return *this;					}
	
	void						initSystemSwitchModel()	
	{	initBase();
		setSubProtocol.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stSystemSwitchBase::read(stream)&&
			stream.read(setSubProtocol);
	}
	
	inline bool		write			(CStreamBase& stream)
	{
		return _stSystemSwitchBase::write(stream)&&
			stream.write(setSubProtocol);
	}
	
};

//-------------------------------------------------------------
//------------------------------ 