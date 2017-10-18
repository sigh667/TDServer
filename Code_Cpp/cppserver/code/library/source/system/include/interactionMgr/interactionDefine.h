/*----------------- interactionDefine.h
*
* Copyright (C): 2011  LiuLei
* Author       : ����
* Version      : V1.0
* Date         : 2012/4/3 17:30:49
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ �����¼�
enum _emInteractionEvent
{
	_IEvent_Trade		,//����
	_IEvent_Friend		,//����

	_IEvent_Max
};

//-------------------------------------------------------------
//------------------------------ ��ӽ���
enum _emInteractionTeam
{
	_ITeam_Build	,//����
	_ITeam_Invite	,//����
	_ITeam_Apply	,//����
	_ITeam_Head		,//���öӳ�
	_ITeam_Max
};

//-------------------------------------------------------------
//------------------------------ ���׽���
enum _emInteractionTrade
{
	_ITrade_Build	,//����
	_ITrade_Max
};
//-------------------------------------------------------------
//------------------------------ ��ύ��
enum _emInteractionFaction
{
	_IFaction_Invite	,//����
	_IFaction_Apply		,//����
	_IFaction_Federal	,//����
	_IFaction_Convene	,//�ټ�

	_IFaction_Max
};
//-------------------------------------------------------------
//------------------------------ ����
enum _emInteractionWorld
{
	_IWorld_Migrate	,//����

	_IWorld_Max
};
//-------------------------------------------------------------
//------------------------------ �ǳ�
enum _emInteractionCity
{
	_ICity_WarRemind	,//��ս����

	_ICity_Max
};

//-------------------------------------------------------------
//------------------------------ �����¼�
enum _emInteractionMarriage
{
	_IMarriage_Propose	,//���
	_IMarriage_Divorce	,//���
	_IMarriage_Wedding	,//����
	_IMarriage_UpCar	,//�ϳ�

	_IMarriage_Max
};

//-------------------------------------------------------------
//------------------------------ ɫ��
enum _emInteractionDice
{
	_IDice_Help	,//��������

	_IDice_Max
};