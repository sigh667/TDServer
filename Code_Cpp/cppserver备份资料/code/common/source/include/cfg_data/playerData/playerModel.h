/*------------- playerModel.h
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈腾
* Version      : V1.01
* Date         : 2016/02/18
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "def_struct/gameStruct.h"
#include "timer/dateTime.h"
/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 
struct _stPlayerModelBase
{
	uint16				uLevel;					//等级
	uint64				uLevelExp;				//升级到下一级的经验
	uint64				uTradeGetSliverLimit;	//交易可获得最大游戏币数量
	uint64				uTradeGiveSliverLimit;	//交易可赠予最大游戏币数量
	uint64				uTradeGetSliverRecover;	//交易可赠予最大游戏币数量
	uint64				uTradeGiveSliverRecover;	//交易可赠予最大游戏币数量
	uint64				uFactionDrinkWinWisdomMax;	//帮会聚饮可获得每日韬略上限

	_stPlayerModelBase&	getPlayerModelBase()		{	return *this;					}
	void				initPlayerModelBase()		{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
#pragma pack(pop)
//-------------------------------------------------------------
//------------------------------ 
struct _stPlayerModel : _stPlayerModelBase
{
	typedef double_map<uint8,uint32,uint8> DMAP_ADD_SKILL;
	typedef double_map<uint8,uint32,uint8>::_sub_map SUB_DMAP_ADD_SKILL;

	_stGameAttribute	stAttribute;
	DMAP_ADD_SKILL		dmapAddSkill;		// key1:职业 key2=技能ID，key3=技能等级

	_stPlayerModel&		getPlayerModel()		{	return *this;					}
	void				initPlayerModel()
	{
		initPlayerModelBase();
		stAttribute.initGameAttribute();
		dmapAddSkill.clear();
	}	
	inline bool		read			(CStreamBase& stream)
	{
		return _stPlayerModelBase::read(stream)&&
			stream.read(stAttribute)&&
			stream.read(dmapAddSkill);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stPlayerModelBase::write(stream)&&
			stream.write(stAttribute)&&
			stream.write(dmapAddSkill);
	}
};