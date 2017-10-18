/*----------------- activityMode.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ������
* Version      : V1.0
* Date         : 2017/05/16 15:41:59
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "def_struct/gameStruct.h"
#include "stream/streamBase.h"
#include "def_struct/stlDefine.h"
#include "basic/basicTypes.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
//-------------------------------------------------------------
//------------------------------ 
#pragma pack(push,1)

struct _stActivityModel
{
	uint32 uID;				// �ID
	uint32 uType;			// ����
	uint32 uCount;			// ����
	uint32 uLiveness;		//���ӻ�Ծ��
	uint32 uCompleteID;       //����Ͳ��� ��ɶ���ID
	

	_stActivityModel& getactivityModel()                {   return *this;					}
	void initactivityModel()							{   memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};

//
struct _stLivenessBass
{
	uint32 uID;				// ID
	uint32 uLiveness;       // ��Ծ��

	_stLivenessBass&  getlivenessBase()             {   return *this;					 }
	void              initlivenessBase()			{   memset(this,0,sizeof(*this));	 }

	READ_WRITE_STRUCT
};
#pragma pack(pop)

//-------------------------------------------------------------
//------------------------------

struct _stLivenessModel :_stLivenessBass
{
	stl_map<uint32,uint32>	mapItem;		// ����

	_stLivenessModel&		getLivenessModel()		{	return *this;						}
	
	inline void			initLivenessModel()
	{
		initlivenessBase();
		mapItem.clear();
	}
	
	inline bool		read			(CStreamBase& stream)
	{
		return _stLivenessBass::read(stream)&&
			stream.read(mapItem);
	}
	
	inline bool		write			(CStreamBase& stream)
	{
		return _stLivenessBass::write(stream)&&
			stream.write(mapItem);
	}
};




