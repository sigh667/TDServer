/*----------------- equipModel.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/8/15 19:55:48
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
#include "basic/basicTypes.h"
#include "def_struct/stlDefine.h"
#include "def_struct/gameStruct.h"
#include "basic/systemFunction.h"
/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 装备强化
struct _stEquipEnhanceBase
{
	uint16		uLevel;				//强化的等级
	uint8		uEquipType;			//部件类型  _emEquipType
	uint32		uTotalExp;			//升到此级的最大经验

	void initBase()	{   memset(this,0,sizeof(*this));	};

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 装备琢磨
struct _stEquipForgeBase
{
	uint16		uLevel;				//强化的等级
	uint32		uTotalExp;			//升到此级的最大经验
	uint8		uEquipType;			//部件类型  _emEquipType
	uint32		uUpgradeRate;		//升级加的万分比
	uint32		uEnhanceLevel;
	void initBase()	{   memset(this,0,sizeof(*this));	};

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 装备极品属性
struct _stEquipSpecailAttrBase
{
	uint32		uID;				//唯一
	uint16		uType;				//属性类型
	uint32		uRate;				//权重
	uint16		uLevel;				//等级
	uint16		uNeeEquipLevel;		//装备等级
	uint16		uNeedForgeLevel;	//需要琢磨等级

	void initBase()	{   memset(this,0,sizeof(*this));	};

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 装备制造组
struct _stEquipMakeGroupBase
{
	uint32		uEquipID;			//装备ID

	void initBase()	{   memset(this,0,sizeof(*this));	};

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 装备制造
struct _stEquipMakeBase
{
	uint16		uFormulaID;					//打造装备公式ID
	uint16		uEquipLuckyType;			//装备吉运值类型
	uint32		uEquipNeedLucky;			//装备吉运值
	uint32		uEquipAddLucky;				//装备制造产生的吉运值
	uint32		uNeedMoney;					//需要金币
	uint32		uProductType;				//部位类型 _emEquipType
	uint32		uJobType;					//职业类型
	uint32      uSkilledLevel;				//熟练度等级要求
	uint32		uSkilledNum;				//需要的熟练度数值
	uint32		uGetSkilled;				//打造成功获得的熟练度
	uint32		uVigourCost;				//活力值消耗


	void initBase()	{   memset(this,0,sizeof(*this));	}

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stIDAndRate
{
	uint16	uID;
	uint32	uRate;
	_stIDAndRate()	{   memset(this,0,sizeof(*this));	}

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 装备制造
struct _stEquipSuitBase
{
	uint16		uSuitID;			//套装ID
	uint16		uSuitLevel;			//装备等级

	void initBase()	{   memset(this,0,sizeof(*this));	}

	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 装备栏强化
struct _stEquipEnhanceModel : _stEquipEnhanceBase
{
	DMAP_UINT8_32_32		dmapPreCondition;			// 前提条件
	MAP_NEED_EXPEND			mapNeedExpend;				// 扣除的消耗
	_stGameAttribute		stAttribute;				//增加的属性
	stl_set<uint8>			setMaterialType;			//需要材料类型
	MAP_U64_U16				mapNeedMaterial;			// 需要材料
	void initModel()	
	{ 
		initBase();
		dmapPreCondition.clear();
		mapNeedExpend.clear();
		stAttribute.initGameAttribute();
		setMaterialType.clear();
		mapNeedMaterial.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stEquipEnhanceBase::read(stream)&&
			stream.read(dmapPreCondition)&&
			stream.read(mapNeedExpend)&&
			stream.read(stAttribute)&&
			stream.read(setMaterialType)&&
			stream.read(mapNeedMaterial);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stEquipEnhanceBase::write(stream)&&
			stream.write(dmapPreCondition)&&
			stream.write(mapNeedExpend)&&
			stream.write(stAttribute)&&
			stream.write(setMaterialType)&&
			stream.write(mapNeedMaterial);
	}

	inline bool		isMaterialType(uint8 uType)	const
	{
		return setMaterialType.find(uType) != setMaterialType.end();
	}
};
//-------------------------------------------------------------
//------------------------------装备栏打磨
struct _stEquipForgeModel : _stEquipForgeBase
{
	DMAP_UINT8_32_32	dmapPreCondition;			// 前提条件
	MAP_NEED_EXPEND		mapNeedExpend;				// 扣除的消耗
	stl_set<uint8>		setMaterialType;			// 需要材料类型
	MAP_U64_U16			mapNeedMaterial;			// 需要材料

	void initModel()	
	{ 
		initBase();
		dmapPreCondition.clear();
		mapNeedExpend.clear();
		setMaterialType.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stEquipForgeBase::read(stream)&&
			stream.read(dmapPreCondition)&&
			stream.read(mapNeedExpend)&&
			stream.read(setMaterialType)&&
			stream.read(mapNeedMaterial);


	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stEquipForgeBase::write(stream)&&
			stream.write(dmapPreCondition)&&
			stream.write(mapNeedExpend)&&
			stream.write(setMaterialType)&&
				stream.write(mapNeedMaterial);
	}

	inline bool		isMaterialType(uint8 uType) const
	{
		return setMaterialType.find(uType) != setMaterialType.end();
	}
};
//-------------------------------------------------------------
//------------------------------ 装备制造
struct _stEquipMakeModel : _stEquipMakeBase
{
	MAP_U32_U32			mapAddEquipRate;		// 生成道具概率
	DMAP_NEED_ITEM		mapNeedItem;			//需要道具
	DMAP_NEED_ITEM		mapNeedSpecialItem;		//需要特殊道具
	DMAP_UINT8_32_32	dmapPreCondition;		//前提条件
	MAP_U32_U32			mapGetSkilledDecay;		//获得熟练度递减规则,key是等级差,value是百分比

	void initModel()	
	{ 
		initBase();
		mapAddEquipRate.clear();	
		mapNeedItem.clear();		
		mapNeedSpecialItem.clear();	
		dmapPreCondition.clear();	
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stEquipMakeBase::read(stream)&&
			stream.read(mapAddEquipRate)&&
			stream.read(mapNeedItem)&&
			stream.read(mapNeedSpecialItem)&&
			stream.read(dmapPreCondition);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stEquipMakeBase::write(stream)&&
			stream.write(mapAddEquipRate)&&
			stream.write(mapNeedItem)&&
			stream.write(mapNeedSpecialItem)&&
			stream.write(dmapPreCondition);
	}

	uint32		getRandEquipID			(bool bMaxQuality)	const
	{
		stl_vector<uint32> vecRand;
		uint32 uMin = 0;
		int32 iRand = 0;
		CONST_ITER_MAP_U32_U32 _pos = mapAddEquipRate.begin();
		for (; _pos != mapAddEquipRate.end(); ++_pos)
		{
			vecRand.push_back(_pos->first);
			if (!uMin || _pos->first < uMin)
			{
				uMin = _pos->first;
				iRand = vecRand.size()-1;
			}
		}

		if (!bMaxQuality)
			iRand = getRandIndex(vecRand);
		_pos = mapAddEquipRate.begin();
		if ((uint32)iRand > mapAddEquipRate.size())
			return 0;

		advance(_pos,iRand);
		return _pos != mapAddEquipRate.end() ? _pos->second : 0;
	}
};
//-------------------------------------------------------------
//------------------------------ 装备制造组
struct _stEquipMakeGroupModel : _stEquipMakeGroupBase
{
	stl_vector<_stIDAndRate>			vecNormalGroup;		// 普通随机
	stl_vector<_stIDAndRate>			vecAdvancedGroup;	// 高级随机

	void initModel()	
	{ 
		initBase();
		vecNormalGroup.clear();
		vecAdvancedGroup.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stEquipMakeGroupBase::read(stream)&&
			stream.read(vecNormalGroup)&&
			stream.read(vecAdvancedGroup);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stEquipMakeGroupBase::write(stream)&&
			stream.write(vecNormalGroup)&&
			stream.write(vecAdvancedGroup);
	}
};
//-------------------------------------------------------------
//------------------------------ 装备极品属性
struct _stEquipSpecailAttrModel : _stEquipSpecailAttrBase
{
	_stGameAttribute stAttri;

	void initModel()	
	{ 
		initBase();
		stAttri.initGameAttribute();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stEquipSpecailAttrBase::read(stream)&&
			stream.read(stAttri);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stEquipSpecailAttrBase::write(stream)&&
			stream.write(stAttri);
	}
};
//-------------------------------------------------------------
//------------------------------ 套装
struct _stEquipSuitModel : _stEquipSuitBase
{
	stl_set<uint16>						setEquipID;			//套装ID
	stl_map<uint8,_stGameAttribute>		mapGameAttr;		//套装属性 key:件数
	void initModel()	
	{ 
		initBase();
		setEquipID.clear();
		mapGameAttr.clear();
	}
	inline bool		read			(CStreamBase& stream)
	{
		return _stEquipSuitBase::read(stream)&&
			stream.read(setEquipID)&&
			stream.read(mapGameAttr);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stEquipSuitBase::write(stream)&&
			stream.write(setEquipID)&&
			stream.write(mapGameAttr);
	}
};
