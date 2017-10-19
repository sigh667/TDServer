/*----------------- dropModel.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/7/22 10:23:08
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
#include "basic/basicTypes.h"
#include "def_struct/stlDefine.h"
/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 
struct _stDropCellModel
{
	uint32		uItemID;			//
	uint16		uItemNum;			//
	bool		bBind;				//是否绑定

	void initModel()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stDropBase
{
	uint16		uDropID;			//掉落ID
	
	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
typedef stl_vector<const _stDropCellModel*> VEC_DROP_CELL;
//-------------------------------------------------------------
//------------------------------ 
struct _stDropModel : _stDropBase
{
	VEC_U16			 vecGroupIDAnd; //组与
 	VEC_U16			 vecGroupIDOr; //组或

	void initModel()	
	{ 
		initBase();
		vecGroupIDAnd.clear();
		vecGroupIDOr.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stDropBase::read(stream)&&
			stream.read(vecGroupIDAnd)&&
			stream.read(vecGroupIDOr);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stDropBase::write(stream)&&
			stream.write(vecGroupIDAnd)&&
			stream.write(vecGroupIDOr);
	}
};
