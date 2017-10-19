/*----------------- wentaoModel.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2016/8/3 10:32:49
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
//------------------------------ 兵法类型
struct _stBingFaTypeBase
{
	uint16		uBingFaID;			//兵法ID

	void initBase()	{   memset(this,0,sizeof(*this));	};

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 突破
struct _stBingFaTuPoBase
{
	uint16		uBingFaID;			//兵法ID
	uint16		uTuPoLevel;			//突破等级

	void initBase()	{   memset(this,0,sizeof(*this));	};

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------  潜修
struct _stBingFaQianXiuBase
{
	uint16		uBingFaID;			//兵法ID
	uint16		uQianXiuLevel;		//潜修等级
	uint32		uAddAttrRadio;		//增加属性万分比

	void initBase()	{   memset(this,0,sizeof(*this));	};

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 要点
struct _stBingFaYaoDianBase
{
	uint32		uYaoDianID;			//要点ID
	uint32		uYaoDianLevelMax;	//要点最高等级

	void initBase()	{   memset(this,0,sizeof(*this));	};

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 兵法槽位
struct _stBingFaHoleBase
{
	uint16		uHoleID;			//槽位ID
	uint32		uEffectAttr;		//属性生效万分比

	void initBase()	{   memset(this,0,sizeof(*this));	};

	READ_WRITE_STRUCT
};

//-------------------------------------------------------------
//------------------------------
struct _stBingFaHoleWulueEffect
{
	uint8	uWulueType;
	uint16	uYaodianID;
	uint32	uValue;				// 影响的万分比

	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------
struct _stBingFaQianXiuModel : _stBingFaQianXiuBase
{
	DMAP_NEED_ITEM				mapNeedItem;			// 消耗的道具

	void initModel()	
	{ 
		initBase();
		mapNeedItem.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stBingFaQianXiuBase::read(stream)&&
			stream.read(mapNeedItem);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stBingFaQianXiuBase::write(stream)&&
			stream.write(mapNeedItem);
	}
};
//-------------------------------------------------------------
//------------------------------
struct _stBingFaTuPoModel : _stBingFaTuPoBase
{
	DMAP_NEED_ITEM		mapNeedItem;				// 消耗的道具
	MAP_NEED_EXPEND		mapNeedExpend;				// 消耗的数值
	MAP_U32_U8			mapSkill;					// 携带的技能
	VEC_U32				vecYaoDian;					// 要点
	_stGameAttribute	stAttribute;				// 当前突破等级增加的属性

	void initModel()	
	{ 
		initBase();
		mapNeedItem.clear();
		mapNeedExpend.clear();
		mapSkill.clear();
		vecYaoDian.clear();
		stAttribute.initGameAttribute();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stBingFaTuPoBase::read(stream)&&
			stream.read(mapNeedItem)&&
			stream.read(mapNeedExpend)&&
			stream.read(stAttribute)&&
			stream.read(vecYaoDian)&&
			stream.read(mapSkill);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stBingFaTuPoBase::write(stream)&&
			stream.write(mapNeedItem)&&
			stream.write(mapNeedExpend)&&
			stream.write(stAttribute)&&
			stream.write(vecYaoDian)&&
			stream.write(mapSkill);
	}
};
//-------------------------------------------------------------
//------------------------------
struct _stBingFaYaoDianModel : _stBingFaYaoDianBase
{
	DMAP_NEED_BINGFU	dmapNeedBingFu;				// 消耗的兵符 key1:兵ID KEy2:兵符等级 key3:数量
	MAP_NEED_EXPEND		mapNeedExpend;				// 扣除的消耗
	_stGameAttribute	stAttribute;				// 当前要点增加的属性

	void initModel()	
	{ 
		initBase();
		dmapNeedBingFu.clear();
		mapNeedExpend.clear();
		stAttribute.initGameAttribute();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stBingFaYaoDianBase::read(stream)&&
			stream.read(dmapNeedBingFu)&&
			stream.read(mapNeedExpend)&&
			stream.read(stAttribute);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stBingFaYaoDianBase::write(stream)&&
			stream.write(dmapNeedBingFu)&&
			stream.write(mapNeedExpend)&&
			stream.write(stAttribute);
	}
};
//-------------------------------------------------------------
//------------------------------
struct _stBingFaTypeModel : _stBingFaTypeBase
{
	DMAP_NEED_ITEM						mapNeedItem;	// 激活消耗的道具
	stl_map<uint16,_stGameAttribute>	mapAttr;		// 兵法相生 兵法ID ==> 相生属性
	void initModel()	
	{ 
		initBase();
		mapAttr.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stBingFaTypeBase::read(stream)&&
			stream.read(mapNeedItem)&&
			stream.read(mapAttr);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stBingFaTypeBase::write(stream)&&
			stream.write(mapNeedItem)&&
			stream.write(mapAttr);
	}
};
//-------------------------------------------------------------
//------------------------------
struct _stBingFaHoleModel : _stBingFaHoleBase
{
	DMAP_UINT8_32_32	dmapPreCondition;						// 前提条件
	stl_vector<_stBingFaHoleWulueEffect>	vecWulueEffect;		// 筋脉影响值  武略要点 ==〉 数值
	void initModel()	
	{ 
		initBase();
		dmapPreCondition.clear();
		vecWulueEffect.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stBingFaHoleBase::read(stream)&&
			stream.read(vecWulueEffect)&&
			stream.read(dmapPreCondition);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stBingFaHoleBase::write(stream)&&
			stream.write(vecWulueEffect)&&
			stream.write(dmapPreCondition);
	}
};