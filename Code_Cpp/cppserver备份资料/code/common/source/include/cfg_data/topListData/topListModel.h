/*----------------- topListData.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ������
* Version      : V1.0
* Date         : 2017/06/06 15:46:16
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/memoryFunctions.h"
#include "math/range.h"
#include "def_struct/stlDefine.h"
#include "math/point2.h"
#include "basic/basicTypes.h"
#include "def_struct/gameStruct.h"
/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------  ��������
struct _strTopListModel	
{
	uint32					uType;				// ���а�����
	uint32					uRecordLine;		// ����¼��
	uint32					uTimeReset;			// ��������
	
	_strTopListModel&			getTopListModel()					{	return *this;					}
	void						initTopListModel()					{	memset(this,0,sizeof(*this));	}
    
	READ_WRITE_STRUCT
};

struct _strTopListRewardBase
{
	uint32  uID;
	uint32	uType;			// ��������
	uint32	uRank;			// ���� 
	
	_strTopListRewardBase & getTopListRewardBase()	{	return *this;	};
	void					initTopListBase()		{	memset(this, 0, sizeof(*this));	}

	READ_WRITE_STRUCT
};
#pragma pack(pop)

//-------------------------------------------------------------
//------------------------------

struct _strTopListRewardModel : _strTopListRewardBase
{
	stl_map<uint32,uint32>	mapItem;		// ����

	_strTopListRewardModel &	getTopListRewardModel()	{	return *this;		}

	inline void initTopListRewardModel()
	{
		initTopListBase();
		mapItem.clear();
	}

	inline bool read	(CStreamBase & stream)
	{
		return _strTopListRewardBase::read(stream)		&&
			stream.read(mapItem);
	}

	inline bool write	(CStreamBase& stream)
	{
		return _strTopListRewardBase::write(stream)		&&
			stream.write(mapItem);
	}
};

