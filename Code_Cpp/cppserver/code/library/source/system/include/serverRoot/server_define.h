/*----------------- server_define.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2012/8/1 18:17:20
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
enum _em_ServerType
{
	ServerType_None			,

	ServerType_Login		,//��¼������
	ServerType_Register		,//ע�������
	ServerType_DataBase		,//DB������
	ServerType_World		,//���������
	ServerType_Global		,//ȫ�ַ�����
	ServerType_Game			,//��Ϸ������
	ServerType_BigWorld		,//���������
	ServerType_Rooter		,//·�ɷ�����
	ServerType_Robot		,//�����˷�����
	ServerType_Dip			,//��̨������
	ServerType_Record		,//��¼������

	ServerType_Max
};

#define BIG_WORLD_LOGIC_ID 60000