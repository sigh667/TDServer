/*----------------- aiModel.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2016/12/21 19:14:05
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
#include "def_struct/stlDefine.h"
#include "stream/streamBase.h"
#include <string>
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------
struct _stAIModelBase
{
	uint32				uID;				//编号
	uint32				uAICallFrequency;	//ai执行频率(帧数)
	uint8				uSelectTargetType;	//目标选择方式
	uint8				uIdleType;			//休闲类型 _emAIIdleType
	float32				fPatrolRadius;		//巡逻半径 _emAISelectTargetType
	uint32				uPatrolTimeMin;		//巡逻间隔最小值(秒)
	uint32				uPatrolTimeMax;		//巡逻间隔最大值(秒)
	float32				fFightRadius;		//警戒半径
	float32				fPursueDis;			//追击距离
	float32				fHatredRadius;		//仇恨半径
	uint32				uDefaultSkillLevel;	//默认技能等级
	uint32				uDefaultBuffLevel;	//默认Buff等级

	inline void				initAIModelBase()	{	memset(this,0,sizeof(*this));			}
	inline _stAIModelBase&	getAIModelBase()	{	return *this;							}
	READ_WRITE_STRUCT
};

/*************************************************************/
#pragma pack(pop)
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stAIModel	: _stAIModelBase
{
	std::string		strBTName;		//行为树名字

	_stAIModel&		getAIModel()							{	return *this;							}
	inline void		initAIModel()
	{
		initAIModelBase();
		strBTName.clear();
	}
	inline bool		read			(CStreamBase& stream)
	{
		return _stAIModelBase::read(stream)&&
			stream.read(strBTName);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stAIModelBase::write(stream)&&
			stream.write(strBTName);
	}
};

//-------------------------------------------------------------
