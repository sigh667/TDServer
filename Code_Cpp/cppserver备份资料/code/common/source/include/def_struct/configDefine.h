/*----------------- configDefine.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2012/8/13 10:41:23
*--------------------------------------------------------------
*���ö���
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
/*************************************************************/
#define DATE_TIME_STRING_MAX	20	//����ʱ���ַ�������
//-------------------------------------------------------------
//------------------------------ ��Ϸ����
enum _emGameSwitch
{
	//ϵͳ���
	GameSwitch_ClientGM			,//�ͻ���GMָ��
	GameSwitch_Adult			,//������
	GameSwitch_DebugMsg			,//�Ƿ��ӡ������Ϣ

	GameSwitch_Max
};

extern const pc_str g_strGameSwitch[GameSwitch_Max];
