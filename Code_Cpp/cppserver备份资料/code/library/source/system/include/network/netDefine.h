/*----------------- netDefine.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
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
#define IP_Length_Max	16	/*IP����*/
#define Net_Key_Max		16	/*������Կ����*/
//------------------------------------------------------
//------------------------------ ������Ϣ��Э���־
enum _e_ProtocolFlag
{
	P_Operate	= 255	,	/*��������*/ 

	P_Flag_Max,
};
//static const char* g_szpProtocolFlag[P_Flag_Max] =
//{
//	"��������"		,
//};
//inline const char*	getProtocolFlag(uint8 uCode)
//{
//	if (uCode >= P_Flag_Max)
//		return "";
//
//	return g_szpProtocolFlag[uCode];
//}
