/*----------------- sceneFlyModel.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ������
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
//------------------------------  ��򴫵�
struct _stHostlerModel	
{
	uint32					uID;			// ID 
	uint16					uRegionID;		// ����ID
	uint16					uSceneID;		// ����ID
	_stPoint2F				stPos;			// ����
	
	_stHostlerModel&			getHostlerModel()					{	return *this;					}
	void						initHostlerModel()					{	memset(this,0,sizeof(*this));	}
    
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ ��������
struct _stFlyModel	
{
	uint32					uID;			    // ID 
	uint16					uType;              // ���� 
	uint16					uStartSceneID;		// ��������
	uint16					uEndSceneID;		// ��������
	uint16                  uRegionID;			// ��������������ID
	_stPoint2F				stPos;				// ����

	_stFlyModel&			getFlyModel()					{	return *this;					}
	void					initFlyModel()					{	memset(this,0,sizeof(*this));	}

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ ��������
struct _stFlyNpcModel	
{
	uint32					uID;			    // ID 
	uint16					uSceneID;			// ����ID
	uint16                  uRegionID;			// ����ID
	_stPoint2F				stPos;				// ����

	_stFlyNpcModel&			getModel()					{	return *this;					}
	void					initModel()					{	memset(this,0,sizeof(*this));	}

	READ_WRITE_STRUCT
};
#pragma pack(pop)