/*------------- gameDefine_inline.h
* Copyright (C): 2014 Mokylin°§Mokyqi
* Author       : ’‘Œƒ‘¥
* Version      : V1.01
* Date         : 2014/28/10 10:55
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "math/mathFunctions.h"
#include "def_struct/gameDefine.h"
/*************************************************************/
inline int32		getGridPostion	(float32 fPos,uint32 uGirdSize)
{
	return  (int32)(fPos/ uGirdSize);
}
