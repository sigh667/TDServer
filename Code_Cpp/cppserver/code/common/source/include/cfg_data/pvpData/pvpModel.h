/*----------------- pvpModel.h
*
* Copyright (C): 2017  Mokylin��Mokyqi
* Author       : ����
* Version      : V1.0
* Date         : 2017/4/21 16:22:24
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
#include "timer/dateTime.h"
/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 
struct _stPvpBase
{
	uint32			uID;				//�������ID
	uint32			uMaxEnterCount;		//���������
	uint32			uNeedPlayerNum;		//ÿ����Ҫ������
	uint32			uExitReadyCoolTime;	//�˳�׼����ȴʱ����
	uint16			uCopymapID;			//����ĸ���ID
	uint32			uStepMatchTime;		//ƥ����ʱ��
	uint32			uReadyTime;			//׼��ʱ��

	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stSinglePvpPrizeBase
{
	uint32			uStartScore;		//��ʼ�ķ���
	uint32			uID;				//ID
	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stPvpModel : _stPvpBase
{
	stl_vector<_stDateTime>		vecOpenTime;		// ����ʱ��
	DMAP_UINT8_32_32			dmapPreCondition;	//ǰ������
	MAP_NEED_EXPEND				mapNeedExpend;		//��Ҫ����
	DMAP_NEED_ITEM				mapNeedItem;		//��Ҫ����

	void initModel()	
	{ 
		initBase();
		vecOpenTime.clear();
		dmapPreCondition.clear();
		mapNeedExpend.clear();
		mapNeedItem.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stPvpBase::read(stream)&&
			stream.read(vecOpenTime)&&
			stream.read(dmapPreCondition)&&
			stream.read(mapNeedExpend)&&
			stream.read(mapNeedItem);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stPvpBase::write(stream)&&
			stream.write(vecOpenTime)&&
			stream.write(dmapPreCondition)&&
			stream.write(mapNeedExpend)&&
			stream.write(mapNeedItem);
	}
	inline bool		isInOpenTime	(uint64 uTime) const 
	{
		if ( vecOpenTime.empty() )
			return true;

		uint32	uSize 	= vecOpenTime.size();
		for ( uint32 i = 0; i < uSize; ++i )
		{
			const  _stDateTime& stDataTime = vecOpenTime[i];
			if (stDataTime.toDataTime(uTime))
				return true;
		}
		return false;
	}
};
//-------------------------------------------------------------
//------------------------------ 
struct _stSinglePvpPrizeModel : _stSinglePvpPrizeBase
{
	DMAP_NEED_ITEM				mapFirstAddItem;	//�״����ӵ���
	DMAP_NEED_ITEM				mapWeekAddItem;		//�����ӵ���

	void initModel()	
	{ 
		initBase();
		mapFirstAddItem.clear();
		mapWeekAddItem.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stSinglePvpPrizeBase::read(stream)&&
			stream.read(mapFirstAddItem)&&
			stream.read(mapWeekAddItem);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stSinglePvpPrizeBase::write(stream)&&
			stream.write(mapFirstAddItem)&&
			stream.write(mapWeekAddItem);
	}
};
//-------------------------------------------------------------
//------------------------------ 
struct _stSinglePvpDayPrizeModel 
{
	DMAP_NEED_ITEM				mapAddItem;	//���ӵ���

	void initModel()	
	{ 
		mapAddItem.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return 		stream.read(mapAddItem);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return 		stream.write(mapAddItem);
	}
};
