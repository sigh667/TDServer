/*----------------- registerDefine.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2011/9/6 18:00:37
*--------------------------------------------------------------
*ע���¼�
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ ע���¼�
enum _em_RegisterEvent
{
	RegEvent_RegisterPlayer,	//ע���ɫ����
	RegEvent_RegisterFaction,	//ע�����
	RegEvent_DeleteFaction,		//ɾ������
	RegEvent_CheckPlayerName,	//����ɫ����
	RegEvent_Max
};
