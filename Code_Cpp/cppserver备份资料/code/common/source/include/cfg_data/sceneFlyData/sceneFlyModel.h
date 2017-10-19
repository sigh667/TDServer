/*----------------- sceneFlyModel.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 方声辉
* Version      : V1.0
* Date         : 2016/09/21 13:39:53
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
//------------------------------  马夫传递
struct _stHostlerModel	
{
	uint32					uID;			// ID 
	uint16					uRegionID;		// 区域ID
	uint16					uSceneID;		// 场景ID
	_stPoint2F				stPos;			// 坐标
	
	_stHostlerModel&			getHostlerModel()					{	return *this;					}
	void						initHostlerModel()					{	memset(this,0,sizeof(*this));	}
    
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 御风神行
struct _stFlyModel	
{
	uint32					uID;			    // ID 
	uint16					uType;              // 类型 
	uint16					uStartSceneID;		// 出发场景
	uint16					uEndSceneID;		// 结束场景
	uint16                  uRegionID;			// 结束场景的区域ID
	_stPoint2F				stPos;				// 坐标

	_stFlyModel&			getFlyModel()					{	return *this;					}
	void					initFlyModel()					{	memset(this,0,sizeof(*this));	}

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 御风神行
struct _stFlyNpcModel	
{
	uint32					uID;			    // ID 
	uint16					uSceneID;			// 场景ID
	uint16                  uRegionID;			// 区域ID
	_stPoint2F				stPos;				// 坐标

	_stFlyNpcModel&			getModel()					{	return *this;					}
	void					initModel()					{	memset(this,0,sizeof(*this));	}

	READ_WRITE_STRUCT
};
#pragma pack(pop)