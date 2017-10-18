/*----------------- interactionStruct.h
*
* Copyright (C): 2011  LiuLei
* Author       : ΑυΐΧ
* Version      : V1.0
* Date         : 2012/4/3 17:31:04
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "interactionDefine.h"
#include "basic/memoryFunctions.h"
/*************************************************************/
struct _stInteraction
{
	uint8			uEventID;
	uint8			uEventSubID;
	uint32			uTimeout;
	uint32			uSendID;
	uint32			uRecvID;
	uint64			uEvent;
	uint32			uDataID;

	inline void	initialize()		{	dMemset(this,0,sizeof(*this));	}
	
};
