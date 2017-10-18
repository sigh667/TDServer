/*------------- playerModel.h
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����
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
	uint16				uLevel;					//�ȼ�
	uint64				uLevelExp;				//��������һ���ľ���
	uint64				uTradeGetSliverLimit;	//���׿ɻ�������Ϸ������
	uint64				uTradeGiveSliverLimit;	//���׿����������Ϸ������
	uint64				uTradeGetSliverRecover;	//���׿����������Ϸ������
	uint64				uTradeGiveSliverRecover;	//���׿����������Ϸ������
	uint64				uFactionDrinkWinWisdomMax;	//�������ɻ��ÿ���������

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
	DMAP_ADD_SKILL		dmapAddSkill;		// key1:ְҵ key2=����ID��key3=���ܵȼ�

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