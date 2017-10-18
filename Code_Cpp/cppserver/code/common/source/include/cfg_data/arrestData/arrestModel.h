/*----------------- arrestModel.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����
* Version      : V1.0
* Date         : 2016/11/14 10:49:02
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
#include "basic/basicTypes.h"
#include "def_struct/stlDefine.h"
#include "def_struct/gameStruct.h"
#include "math/range.h"
#include "basic/systemFunction.h"
/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 
struct _stArrestQuestBase
{
	uint32			uID;				//����ID
	uint8			uCopymapDifficulty;	//�����Ѷ� _emCopymapDifficulty
	_stRangeU32		stRangeTeamNum;		//��������
	uint32			uQuestTime;			//����ʱ��
	uint32			uNpcID;				//npcid
	uint16			uNeedLeaderIdentity;//��Ҫ���
	uint32			uMaxEnterCount;		//����������
	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stArrestNpcBase
{
	uint32			uNpcID;				//����npcID
	uint32			uNpcMapID;			//���õ�ͼID
	uint32			uTotleRate;			//�ܸ���

	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stArrestQuestModel : _stArrestQuestBase
{
	DMAP_UINT8_32_32	dmapPreCondition;		//ǰ������
	MAP_NEED_EXPEND		mapNeedExpend;			// �۳�������
	MAP_NEED_ITEM		mapNeedItem;			//��Ҫ�������

	void initModel()	
	{ 
		initBase();
		dmapPreCondition.clear();
		mapNeedExpend.clear();
		mapNeedItem.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stArrestQuestBase::read(stream)&&
			stream.read(dmapPreCondition)&&
			stream.read(mapNeedExpend)&&
			stream.read(mapNeedItem);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stArrestQuestBase::write(stream)&&
			stream.write(dmapPreCondition)&&
			stream.write(mapNeedExpend)&&
			stream.write(mapNeedItem);
	}
};
//-------------------------------------------------------------
//------------------------------ 
struct _stArrestNpcModel : _stArrestNpcBase
{
	MAP_U32_U8	mapArrestRate;		//����Ȩ�ر� key:Ȩ�� value:��������_emCopymapType
	void initModel()	
	{ 
		initBase();
		mapArrestRate.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stArrestNpcBase::read(stream)&&
			stream.read(mapArrestRate);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stArrestNpcBase::write(stream)&&
			stream.write(mapArrestRate);
	}
	const uint8	getRandCopymapType	()const
	{
		uint32 uRandNum = randRange(uTotleRate);
		CONST_ITER_MAP_U32_U8 _pos = mapArrestRate.lower_bound(uRandNum); //��һ�����ڵ���uLevel
		return _pos != mapArrestRate.end() ? _pos->second : 0;
	}
};
