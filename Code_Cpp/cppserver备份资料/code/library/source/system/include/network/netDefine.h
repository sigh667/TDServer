/*----------------- netDefine.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/8/21 13:03:08
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
/*************************************************************/
//------------------------------------------------------
//------------------------------ 
#ifndef _IO_BUFFER_
#endif /*_IO_BUFFER_*/ 
//------------------------------ 
#define IP_Length_Max	16	/*IP长度*/
#define Net_Key_Max		16	/*网络密钥长度*/
//------------------------------------------------------
//------------------------------ 网络消息主协议标志
enum _e_ProtocolFlag
{
	P_Operate	= 255	,	/*连接设置*/ 

	P_Flag_Max,
};
//static const char* g_szpProtocolFlag[P_Flag_Max] =
//{
//	"连接设置"		,
//};
//inline const char*	getProtocolFlag(uint8 uCode)
//{
//	if (uCode >= P_Flag_Max)
//		return "";
//
//	return g_szpProtocolFlag[uCode];
//}
