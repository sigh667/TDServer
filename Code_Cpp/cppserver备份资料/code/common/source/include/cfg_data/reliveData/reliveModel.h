/*----------------- reliveMode.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 方声辉
* Version      : V1.0
* Date         : 2017/05/31 18:41:59
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

#pragma pack(push,1)

struct _stReliveModel
{
	uint32 uReliveCount;				// 重生次数
	uint32 uItemCount;					// 道具个数


	_stReliveModel& getReliveModel()            {   return *this;					}
	void initReliveModel()						{   memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};

#pragma pack(pop)





