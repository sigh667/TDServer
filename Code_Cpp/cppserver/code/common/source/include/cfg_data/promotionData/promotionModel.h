/*------------- promotionModel.h
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : �־�
* Version      : V1.01
* Date         : 2016/09/20
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "def_struct/stlDefine.h"
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 
struct _stPromotionModelBase
{
	uint8				uJob;				// ְҵ
	uint8				uStep;				// ��������
	uint32				uOpenHour;			// ��������Сʱ�󿪷Ž���
	uint32				uNeedLevel;			// ������Ҫ�ȼ�
	uint32				uQuest;				// ��������ID

	inline _stPromotionModelBase&	get()		{	return *this;					}
	inline void						init()		{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stPromotionModel : _stPromotionModelBase
{
	_stPromotionModel()	{ init();	}
	inline void	init()
	{
		_stPromotionModelBase::init();
	}

	inline bool read(CStreamBase& stream)
	{
		return _stPromotionModelBase::read(stream);
	}
	inline bool write(CStreamBase& stream)
	{
		return _stPromotionModelBase::write(stream);
	}
};
