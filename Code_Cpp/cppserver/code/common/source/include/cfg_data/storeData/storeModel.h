/*----------------- itemModel.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ������
* Version      : V1.0
* Date         : 2016/5/13 11:32:05
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "def_struct/stlDefine.h"
#include "../itemData/itemModel.h"
/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------

struct _stGoodsModelBase
{
	uint32			uStore;				//�����̵�����
	uint32			uIndex;				//��Ʒ����
	uint32			uPrice;				//�ۼ�
	uint32			uLimit;				//�޹�����
	uint8			uCircle;			//�޹�����
	uint8			uValid;				//�Ƿ���Ч1:��Ч���ɹ��򣬿ɼ���0����Ч
	uint8			uFree;				//�Ƿ���� 1 ��� 2 �׹����
	bool			bBind;				//��Ʒ�Ƿ��

	_stGoodsModelBase&	getBase		()			{	return *this;							}
	inline void			initBase	()			{	dMemset(this,0,sizeof(*this));			}
	READ_WRITE_STRUCT
};
#pragma pack(pop)
//-------------------------------------------------------------
//------------------------------ ��Ʒ
struct _stGoodsModel : _stGoodsModelBase
{
	stl_vector<_stDateTime>	vecOnShelf;		//�ϼ�ʱ��
	VEC_U8	vecCurrency;					//��������
	DMAP_UINT8_32_32	condition;			//��������
	_stItemBuild		itemInfo;			//��Ʒ����

	_stGoodsModel&	getModel	()				{	return *this;							}
	inline void		initModel	()				
	{
		initBase();
		vecOnShelf.clear();
		vecCurrency.clear();
		condition.clear();
		itemInfo.initItemBuild();
	}
	inline bool		read			(CStreamBase& stream)
	{
		return _stGoodsModelBase::read(stream)&&
			stream.read(vecOnShelf)&&
			stream.read(vecCurrency)&&
			stream.read(condition)&&
			stream.read(itemInfo);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stGoodsModelBase::write(stream)&&
			stream.write(vecOnShelf)&&
			stream.write(vecCurrency)&&
			stream.write(condition)&&
			stream.write(itemInfo);
	}
};
