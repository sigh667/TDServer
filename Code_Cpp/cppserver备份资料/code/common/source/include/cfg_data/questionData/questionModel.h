/*----------------- questionModel.h
*
* Copyright (C): 2017  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2017/4/26 11:39:09
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "def_struct/stlDefine.h"
#include "def_struct/gameStruct.h"
#include "timer/dateTime.h"

#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 问题
struct _stQuestionModel
{
	uint32				uID;					// ID
	uint32				uWeight;				// 权重

	_stQuestionModel&	get()			{	return *this;					}
	void				init()			{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 配置
struct _stQuestionConifigModelBase
{
	uint32				uQuestionCount;			// 题目个数
	uint32				uWaitTime;				// 准备时间
	uint32				uAnswerTime;			// 答题时间
	uint32				uResultTime;			// 答案显示时间

	_stQuestionConifigModelBase&	get()			{	return *this;					}
	void							init()			{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stQuestionConifigModel	: public _stQuestionConifigModelBase
{
	stl_vector<_stDateTime>					vecOpenTime;				// 开启时间

	_stQuestionConifigModel&		get			(){	return *this;					}
	inline void		init()
	{
		_stQuestionConifigModelBase::init();
		vecOpenTime.clear();
	}
	inline bool		read			(CStreamBase& stream)
	{
		return _stQuestionConifigModelBase::read(stream)&&
			stream.read(vecOpenTime);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stQuestionConifigModelBase::write(stream)&&
			stream.write(vecOpenTime);
	}
	inline bool		isInOpenTime	(uint64 uTime) const 
	{
		if ( vecOpenTime.empty() )
			return false;

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