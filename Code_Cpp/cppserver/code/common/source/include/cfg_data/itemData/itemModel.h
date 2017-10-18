/*----------------- itemModel.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/4/11 11:32:05
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/memoryFunctions.h"
#include "basic/basicTypes.h"
#include "stream/netStream.h"
#include "def_struct/stlDefine.h"
#include "timer/dateTime.h"
#include "def_struct/gameStruct.h"
#include "basic/systemFunction.h"
/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 
struct _stSplitProb
{
	uint16		uMaterialId;		// 分解出的物品ID
	uint16		uMaterialNum;		// 分解出的物品数量
	uint16		uProb;				// 分解出的概率

	_stSplitProb()			{	dMemset(this,0,sizeof(*this));			}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stItemModelBase
{
	uint32			uItemID;
	uint8			uType;				// 物品大类型 _emItemType
	uint8			uSubType;			// 物品小类型 装备部件:_emEquipType 兵符部件:_emBingfuType 材料:_emMaterialType
	uint8			uUseType;			// 使用类型 _emItemUseType
	uint16			uLevel;				// 物品等级
	uint16			uMaxEnhanceLevel;	// 最大强化等级
	uint16			uMaxForgeLevel;		// 最大琢磨等级
	uint32			uMark;				// 掩码 _emItemMark
	uint16			uMaxCount;			// 最大堆叠数
	uint32			uItemMark;			// 道具标示(位_em_ItemMark)
	uint64			uBuyPrice;			// 商店购买价格
	uint64			uSellPrice;			// 出售商店价格
	uint32			uCD;				// CD时间（毫秒）
	uint16			uCDGroupID;			// CD组ID
	uint32			uGroupCD;			// CD组时间（毫秒）
	uint8			uQuality;			// 品质		_emItemQualityType
	uint32			uDurability;		// 耐久值
	uint16			uSplitItemID;		// 分解所得道具ID
	uint16			uSplitItemNum;		// 分解所得道具个数
	uint32			uExpireTime;		// 有效时间秒
	uint32			uMaxDayUseCount;	// 每日使用次数
	uint32			uMaxWeekUseCount;	// 每周使用次数
	uint32			uMaxMonthUseCount;	// 每月使用次数
	uint32			uBingfuExp;			// 兵符经验

	_stItemModelBase&	getBase		()			{	return *this;							}
	inline void			initBase	()			{	dMemset(this,0,sizeof(*this));			}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stItemBuild
{
	uint32	uItemID;			//物品ID 
	uint32	uItemNum;			//物品数量 	
	uint16	uRate;				//几率(最大万分比)

	_stItemBuild	()				{	initItemBuild();				}
	_stItemBuild&getItemBuild	()	{	return *this;					}
	inline void	initItemBuild	()	{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 礼包
struct _stItemPackModelBase
{
	uint32			uPacketID;		//礼包ID
	uint8			uType;			//礼包类型(_emItemPackType)

	_stItemPackModelBase&	getBase		()			{	return *this;							}
	inline void				initBase	()			{	dMemset(this,0,sizeof(*this));			}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 道具合成
struct _stItemCompoundBase
{
	uint32			uRecipeID;		//配方

	_stItemCompoundBase&	getBase		()			{	return *this;							}
	inline void				initBase	()			{	dMemset(this,0,sizeof(*this));			}
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stItemModel	: _stItemModelBase
{
	std::string					strScript;			// 使用后触发的脚本
	stl_vector<_stSplitProb>	splitVec;			// 分解列表
	stl_vector<uint32>			vecFunValue;		// 功能参数
	DMAP_UINT8_32_32			dmapUseCondition;	// 前提条件
	stl_vector<_stDateTime>		vecUseTimeLimit;	// 使用时间限制
	stl_vector<_stGameAttribute> vecAttribute;		// 基础属性 
	MAP_U16_U32					mapAddEnhanceExp;	// 增加强化经验 key:等级 value:经验
	MAP_U16_U32					mapAddForgeExp;		// 增加琢磨经验 key:等级 value:经验
	std::string					strName;			// 物品名字

	inline bool		check_mark	(uint8 _mark) const	{	return _CHECK_BIT(uMark,_BIT32(_mark));	}
	_stItemModel&	getModel	()					{	return *this;							}
	inline void		initModel	()				
	{
		initBase();
		strScript.clear();
		splitVec.clear();
		vecFunValue.clear();
		dmapUseCondition.clear();
		vecUseTimeLimit.clear();
		mapAddEnhanceExp.clear();
		mapAddForgeExp.clear();
		strName.clear();
	}
	inline bool		read			(CStreamBase& stream)
	{
		return _stItemModelBase::read(stream)&&
			stream.read(strScript)&&
			stream.read(splitVec)&&
			stream.read(vecFunValue)&&
			stream.read(dmapUseCondition)&&
			stream.read(vecAttribute)&&
			stream.read(vecUseTimeLimit)&&
			stream.read(mapAddEnhanceExp)&&
			stream.read(mapAddForgeExp)&&
			stream.read(strName);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stItemModelBase::write(stream)&&
			stream.write(strScript)&&
			stream.write(splitVec)&&
			stream.write(vecFunValue)&&
			stream.write(dmapUseCondition)&&
			stream.write(vecAttribute)&&
			stream.write(vecUseTimeLimit)&&
			stream.write(mapAddEnhanceExp)&&
			stream.write(mapAddForgeExp)&&
			stream.write(strName);
	}
	inline bool		isUseTimeLimit	(uint64 uTime)	const
	{
		if ( vecUseTimeLimit.empty() )
			return true;

		uint32	uSize 	= vecUseTimeLimit.size();
		for ( uint32 i = 0; i < uSize; ++i )
		{
			const _stDateTime& stDataTime = vecUseTimeLimit[ i ];
			if (stDataTime.toDataTime(uTime))
				return false;
		}
		return true;
	}
	inline uint32 getEnhanceExp(uint16 uLevel)	const
	{
		CONST_ITER_MAP_U16_U32 _pos = mapAddEnhanceExp.lower_bound(uLevel); //第一个大于等于uLevel
		return _pos != mapAddEnhanceExp.end() ? _pos->second : 0;
	}
	inline uint32 getForgeExp(uint16 uLevel)	const
	{
		CONST_ITER_MAP_U16_U32 _pos = mapAddForgeExp.lower_bound(uLevel); //第一个大于等于uLevel
		return _pos != mapAddForgeExp.end() ? _pos->second : 0;
	}
};
//-------------------------------------------------------------
//------------------------------ 礼包
struct _stItemPackModel : _stItemPackModelBase
{
	typedef stl_vector<_stItemBuild>	VECTOR_ITEM_BUILD;
	VECTOR_ITEM_BUILD	vecItemBuild;	//礼包内容

	_stItemPackModel&getItemPackModel()	{	return *this;	}
	inline void	initModel()
	{
		initBase();
		vecItemBuild.clear();
	}
	inline bool		read			(CStreamBase& stream)
	{
		return _stItemPackModelBase::read(stream)&&
			stream.read(vecItemBuild);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stItemPackModelBase::write(stream)&&
			stream.write(vecItemBuild);
	}
};
//-------------------------------------------------------------
//------------------------------ 道具合成
struct _stItemCompoundModel : _stItemCompoundBase
{
	DMAP_NEED_ITEM		mapNeedItem;
	MAP_U32_U32			mapItemRate;			//增加道具 key:权重 value:道具id

	_stItemCompoundModel&	getModel()	{	return *this;	}
	inline void	initModel()
	{
		initBase();
		mapNeedItem.clear();
		mapItemRate.clear();

	}
	inline bool		read			(CStreamBase& stream)
	{
		return _stItemCompoundBase::read(stream)&&
			stream.read(mapNeedItem)&
			stream.read(mapItemRate);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stItemCompoundBase::write(stream)&&
			stream.write(mapNeedItem)&
			stream.write(mapItemRate);
	}
	inline uint32	findRandItem() const
	{
		MAP_U32_U32::const_reverse_iterator _rpos = mapItemRate.rbegin();
		if (_rpos == mapItemRate.rend())
			return 0;
		uint32 uRandNum = randRange(_rpos->first);
		CONST_ITER_MAP_U32_U32 _spos = mapItemRate.lower_bound(uRandNum); //第一个大于等于uRandNum
		return _spos != mapItemRate.end() ? _spos->second : 0;
	}
};
