/*----------------- itemModel.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 晁旭升
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
	uint32			uStore;				//所在商店类型
	uint32			uIndex;				//商品索引
	uint32			uPrice;				//售价
	uint32			uLimit;				//限购数量
	uint8			uCircle;			//限购周期
	uint8			uValid;				//是否有效1:有效，可购买，可见；0：无效
	uint8			uFree;				//是否免费 1 免费 2 首购免费
	bool			bBind;				//物品是否绑定

	_stGoodsModelBase&	getBase		()			{	return *this;							}
	inline void			initBase	()			{	dMemset(this,0,sizeof(*this));			}
	READ_WRITE_STRUCT
};
#pragma pack(pop)
//-------------------------------------------------------------
//------------------------------ 商品
struct _stGoodsModel : _stGoodsModelBase
{
	stl_vector<_stDateTime>	vecOnShelf;		//上架时间
	VEC_U8	vecCurrency;					//货币类型
	DMAP_UINT8_32_32	condition;			//购买条件
	_stItemBuild		itemInfo;			//物品参数

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
