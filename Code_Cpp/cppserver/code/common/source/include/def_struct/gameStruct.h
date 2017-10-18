/*----------------- gameStruct.h
*
* Copyright (C): 2011  LiuLei
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/9/13 13:52:20
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "gameDefine.h"
#include "math/point2.h"
#include "basic/basicFunctions.h"
#include "def_struct/stlDefine.h"
/*************************************************************/
/*************************************************************/
struct _stSkillModel;
struct _stSkillEffectModel;
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 游戏对像ID
struct _stGameUnitID
{
	uint8	uUnitType;
	uint32	uUnitID;

	_stGameUnitID&		getGameUnitID	()	{	return *this;					}
	void				initialize		()	{	memset(this,0,sizeof(*this));	}
};
//-------------------------------------------------------------
//------------------------------ 游戏基本属性
struct _stGameAttribute
{
	int32	iAttribute	[_emBattleAttribute_ARRAYSIZE];	//游戏属性固定值
	int32	iRadio		[_emBattleAttribute_ARRAYSIZE];	//游戏属性万分比

	_stGameAttribute()							{	initGameAttribute();			}
	_stGameAttribute&	getGameAttribute	()	{	return *this;					}
	void				initGameAttribute	()	{	memset(this,0,sizeof(*this));	}

	//--- 获得属性固定值
	inline int32	getAttribute		(uint8 uType)const	{	return ((uType >= _emBattleAttribute_ARRAYSIZE) ? 0 : iAttribute[uType]);	}
	//--- 获得属性万分比
	inline int32	getRadio			(uint8 uType)const	{	return ((uType >= _emBattleAttribute_ARRAYSIZE) ? 0 : iRadio[uType]);		}
	//--- 更新属性
	int32			updateAttribute		(uint8 uType, int32 nUpdate, int32 nRatio = 0);
	void			updateAttributeExistedAllByRatio	(int32 nRatio , std::set<int32> setIncludeAttr);//倍数,不是万分比
	inline bool		isNull()const
	{
		for (int32 i = 0;i < _emBattleAttribute_ARRAYSIZE;++i)
		{
			if(this->iAttribute[i] != 0 || iRadio[i] != 0)
				return false;
		}

		return true;
	}	
	inline bool		read			(CStreamBase& stream)
	{
		int32 uSize = 0;
		bool bResult = stream.read(uSize);
		for (int32 i = 0;i < uSize && i < _emBattleAttribute_ARRAYSIZE; ++i)
		{
			bResult &= stream.read(iAttribute[i]);
			bResult &= stream.read(iRadio[i]);
		}
		
		return bResult;
	}
	inline bool		write			(CStreamBase& stream)
	{
		bool bResult = stream.write(_emBattleAttribute_ARRAYSIZE);		
		for (int32 i = 0;i < _emBattleAttribute_ARRAYSIZE;++i)
		{
			bResult &= stream.write(iAttribute[i]);
			bResult &= stream.write(iRadio[i]);
		}

		return bResult;
	}
	//--- 属性增加
	_stGameAttribute& operator+=(const _stGameAttribute&_add);
	//--- 属性增加
	_stGameAttribute& add(const _stGameAttribute* pAdd);
	//--- 属性增加
	// _stGameAttribute& addTmp(const _stGameAttribute* pAdd);
	inline _stGameAttribute& addTmp(const _stGameAttribute* pAdd)
	{
		if (!pAdd)
			return *this;

		for (int i = 0;i < _emBattleAttribute_ARRAYSIZE;++i)
		{
			updateInt(this->iAttribute[i],pAdd->iAttribute[i]);
			updateInt(this->iRadio[i],pAdd->iRadio[i]);
		}

		return *this;
	}

	//--- 属性减少
	_stGameAttribute& operator-=(const _stGameAttribute&_reduce);
	//--- 属性翻倍
	_stGameAttribute& operator*=(int32 _mul);
	//--- 属性翻倍
	_stGameAttribute& operator*=(float32 _mul);
};
//****************************************************************
//-------------------------------------------------------------
//------------------------------ 技能造成Buff
struct _stSkillBuff
{
	uint32			uAttackID;		//攻击对像ID
	uint32			uBuffID;		//加buff
	uint64			uDelayTime;		//【多少毫秒之后】
	uint32			uDurationTime;	//buff持续时间ms

	void		initSkillBuff()	{	memset(this,0,sizeof(*this));	}
	_stSkillBuff&getSkillBuff()	{	return *this;					}
};

//-------------------------------------------------------------
//------------------------------ 增加战斗属性
struct _stAddBattleAttri
{
	uint16		uAttrType;		//_emBattleAttribute
	int32		iValue;			//值
	int32		iRadio;			//概率 万分比
	uint16		uTransAttrType;	//转换的属性 0无属性
	int32		iTransRadio;	//转换的属性万分比

	READ_WRITE_STRUCT
};

//-------------------------------------------------------------
//------------------------------模型配置
struct _stAvatarAnimCfg
{
	uint16	uLandAnimTick;		// 倒地时长(毫秒)
	uint16	uClimbAnimTick;		// 爬起时长(毫秒)
	float32	fModelRadius;		// 模型半径
	uint32  uBeAttackMoveTick;  //受击位移时间
	uint32	uBirthTick;			// 出生时长(毫秒)
	uint32	uDeadTick;			// 死亡时长(毫秒)
	_stAvatarAnimCfg()
	{
		uLandAnimTick = 0;
		uClimbAnimTick = 0;
		uBirthTick = 0;
		uDeadTick = 0;
		fModelRadius = 0;
	}
};

//-------------------------------------------------------------
//------------------------------能量恢复
struct _stEnergyRecovery
{
	uint64	uTick;		// 时刻
	int32	iValue;		// 恢复量

	_stEnergyRecovery()
	{
		uTick	= 0;
		iValue	= 0;
	}
};

//-------------------------------------------------------------
//------------------------------ 增加战斗属性
struct _stAddExpend
{
	uint16		uExpendType;		//_emExpendType
	int32		iValue;			//值

	READ_WRITE_STRUCT
};

//-------------------------------------------------------------
//------------------------------ 增加战斗属性
//	用户属性点分配
struct _stPlayerAttributeAlloc
{
	uint32 uTotalAttributes;			// 总的可分配的属性点
	uint32 uLeftAttributes;				// 剩余可分配的属性点
	uint32 uStrengthAlloced;			// 分配的力道
	uint32 uBoneAlloced;				// 分配的筋骨
	uint32 uBloodAlloced;				// 分配的气血
	uint32 uBodyAlloced;				// 分配的身法
	uint32 uMurderAlloced;				// 分配的杀气
	_stPlayerAttributeAlloc() {	 clear();	}
	void clear() {
		memset(this,0,sizeof(*this));
	}
	void reset() {
		uTotalAttributes = 0;
		uTotalAttributes += uStrengthAlloced;
		uTotalAttributes += uBoneAlloced;
		uTotalAttributes += uBloodAlloced;
		uTotalAttributes += uBodyAlloced;
		uTotalAttributes += uMurderAlloced;

		uStrengthAlloced = 0;
		uBoneAlloced = 0;
		uBloodAlloced = 0;
		uBodyAlloced = 0;
		uMurderAlloced = 0;
		uLeftAttributes = uTotalAttributes;
	}
};
//-------------------------------------------------------------
//------------------------------
struct _stExpendItem
{
	uint64 uItemSN;
	uint16 uItemNum;

	_stExpendItem()		{	memset(this, 0, sizeof(*this));	}
};
//-------------------------------------------------------------
//------------------------------ 技能攻击延迟
struct _stItemNum
{
	uint32 uItemID;
	uint16 uItemNum;

	_stItemNum()		{	init();							}
	void init()			{	memset(this, 0, sizeof(*this));	}
	READ_WRITE_STRUCT;

};
//-------------------------------------------------------------
//------------------------------ 游戏存储数据
struct _stGameLogSave
{
	uint32	uPlayerID;			//玩家ID
	uint32	uTime;				//时间
	uint32	uDoingMainType;		//大类型
	uint32	uDoingSubType;		//小类型
	uint32	uObjectID;			//对象ID(道具ID，副本ID)
	uint32	uCurValue;			//当前数值
	int32	iChangeValue;		//改变值

	_stGameLogSave()	{	memset(this, 0, sizeof(*this)); }
};
#pragma pack(pop)
//-------------------------------------------------------------
//------------------------------ 技能攻击延迟
struct _stSkillAttack
{
	uint32					uSouceID;				//施法者ID
	_stPoint2F				stTargetPoint;			//施法者目标点
	uint64					uDelayTick;				// 延时毫秒
	const _stSkillModel*	pSkillModel;		
	const _stSkillEffectModel*	pSkillEffectModel;

	_stSkillAttack()	
	{
		uSouceID = 0;
		stTargetPoint.zero();
		pSkillEffectModel = NULL;
		pSkillModel = NULL;
		uDelayTick = 0;
	}
};
