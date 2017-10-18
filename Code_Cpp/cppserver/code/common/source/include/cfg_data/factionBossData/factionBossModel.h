/*----------------- factionBossModel.h
*
* Copyright (C): 2017  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2017/1/12 15:53:40
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
#include "def_struct/stlDefine.h"
#include "stream/streamBase.h"
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------
struct _stFactionBossPrizeBase
{
	uint8				uType;			//���� _emFactionBossTopType
	uint32				uSubType;		//������

	inline void						initFactionBossPrizeBase()	{	memset(this,0,sizeof(*this));			}
	inline _stFactionBossPrizeBase&	getFactionBossPrizeBase()	{	return *this;							}
	READ_WRITE_STRUCT
};

/*************************************************************/
#pragma pack(pop)
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stFactionBossPrizeModel	: _stFactionBossPrizeBase
{
	DMAP_NEED_ITEM	mapItem;		//����

	_stFactionBossPrizeModel&		getModel()							{	return *this;							}
	inline void						initModel()
	{
		initFactionBossPrizeBase();
		mapItem.clear();
	}
	inline bool		read			(CStreamBase& stream)
	{
		return _stFactionBossPrizeBase::read(stream)&&
			stream.read(mapItem);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stFactionBossPrizeBase::write(stream)&&
			stream.write(mapItem);
	}
};

//-------------------------------------------------------------
