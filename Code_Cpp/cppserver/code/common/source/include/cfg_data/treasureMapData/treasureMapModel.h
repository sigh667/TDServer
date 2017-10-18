/*----------------- treasureMapModel.h
*
* Copyright (C): 2017  Mokylin��Mokyqi
* Author       : ����
* Version      : V1.0
* Date         : 2017/4/25 20:46:51
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
struct _stTreasureMapBase
{
	uint32			uTreasureType;				//����������_emTreasureType
	
	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stTreasurePrizeBase
{
	uint32			uPrizeID;				//����ID
	uint32			uPrizeType;				//�������� _emTreasurePrizeType
	uint32			uEnterCopymapID;		//���븱��
	uint32			uCopymapNpc;			//���븱��NPC
	uint32			uAddBuffID;				//buffID
	uint32			uDayTreasureValue;		//ȫ����������ֵ
	uint32			uDayPrizeCount;			//ȫ�����ս�������

	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stMonsterRefresh
{
	uint32			uMonsterID;				//����ID
	uint32			uMonsterNum;			//�������
	float32			fRandDis;				//����뾶

	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stTreasurePrizeModel : _stTreasurePrizeBase
{
	DMAP_NEED_ITEM					mapAddItem;				//���ӵ���
	MAP_NEED_EXPEND					mapAddExpend;			//��������
	stl_vector<_stMonsterRefresh>	vecMonsterRefresh;		//ˢboss

	void initModel()	
	{ 
		initBase();
		mapAddItem.clear();
		mapAddExpend.clear();
		vecMonsterRefresh.clear();

	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stTreasurePrizeBase::read(stream)&&
			stream.read(mapAddItem)&&
			stream.read(mapAddExpend)&&
			stream.read(vecMonsterRefresh);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stTreasurePrizeBase::write(stream)&&
			stream.write(mapAddItem)&&
			stream.write(mapAddExpend)&&
			stream.write(vecMonsterRefresh);
	}
};
//-------------------------------------------------------------
//------------------------------ 
struct _stTreasureMapModel : _stTreasureMapBase
{
	MAP_U32_U32							mapQuestRate;			//������� key:Ȩ�� value����id
	MAP_U32_U32							mapPrizeRate;			//�������	key:����id value:Ȩ��
	stl_vector<_stTreasurePrizeModel*>	vecPrize;		

	void initModel()	
	{ 
		initBase();
		mapQuestRate.clear();
		mapPrizeRate.clear();
		vecPrize.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stTreasureMapBase::read(stream)&&
			stream.read(mapQuestRate)&&
			stream.read(mapPrizeRate);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stTreasureMapBase::write(stream)&&
			stream.write(mapQuestRate)&&
			stream.write(mapPrizeRate);
	}
	inline uint32	getRandQuest() const
	{
		MAP_U32_U32::const_reverse_iterator _rpos = mapQuestRate.rbegin();
		if (_rpos == mapQuestRate.rend())
			return 0;
		uint32 uRandNum = randRange(_rpos->first);
		CONST_ITER_MAP_U32_U32 _spos = mapQuestRate.lower_bound(uRandNum); //��һ�����ڵ���uRandNum
		return _spos != mapQuestRate.end() ? _spos->second : 0;
	}
	const uint32	getRandPrize	(const MAP_U32_U32& c_mapRandPrize)const
	{
		MAP_U32_U32 newPrize;
		uint32 uTotleRate = 0;
		for (auto _pos = c_mapRandPrize.begin(); _pos != c_mapRandPrize.end(); ++_pos )
		{
			uTotleRate += _pos->second;
			newPrize.insert_(uTotleRate,_pos->first);
		}

		uint32 uRandNum = randRange(uTotleRate);
		CONST_ITER_MAP_U32_U32 _spos = newPrize.lower_bound(uRandNum); //��һ�����ڵ���uLevel
		return _spos != newPrize.end() ? _spos->second : 0;
	}
};