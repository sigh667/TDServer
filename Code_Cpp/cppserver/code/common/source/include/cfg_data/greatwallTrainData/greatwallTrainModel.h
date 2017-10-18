/*----------------- greatwallTrainModel.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ������
* Version      : V1.0
* Date         : 2017/04/25 15:39:53
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
struct _stgGwTrainModel	
{
	uint32					uCopymapID;		// ����ID 
	uint32					uLayer;			// �㼶
	uint32					uBroadCastMsgType;	   // ����ID
	
	_stgGwTrainModel&			getGwTrainModel()					{	return *this;					}
	void						initGwTrainModel()					{	memset(this,0,sizeof(*this));	}
    
	READ_WRITE_STRUCT
};

#pragma pack(pop)