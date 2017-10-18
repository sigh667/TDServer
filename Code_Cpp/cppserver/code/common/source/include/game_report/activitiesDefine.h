/*----------------- activitiesDefine.h
*
* Copyright (C): 2013  Mokylin��Mokyqi
* Author       : �ս�ƽ
* Version      : V1.0
* Date         : 2013/11/4 17:45:40
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
/*************************************************************/
//#############################################################
//##############################�ϱ����ݼ�¼����
//#############################################################
enum _emActivitiesType
{
	Activities_Type_Mount			,	//����
	Activities_Type_RidingWeapon	,	//���
	Activities_Type_Souls			,	//����
	Activities_Type_Bow				,	//����
	Activities_Type_General			,	//����
	Activities_Type_Precious		,	//��ʯ
	Activities_Type_CheckInCount	,	//ǩ������
	Activities_Type_Equip			,	//װ��
	Activities_Type_Fighters		,	//���Ŷݼ�
	Activities_Type_Keepsake		,	//����
	Activities_Type_Base			,	//��ɫ������Ϣ��
	Activities_Type_MountEquip		,	//����װ��
	Activities_Type_CopyMap			,	//����
	Activities_Type_Potential		,	//��ɫǱ��
	Activities_Type_Skill			,	//��ɫ����
	Activities_Type_GeneralWeapon	,	//�佫���
	Activities_Type_ItemRecord		,	//����ʹ��
	Activities_Type_City			,	//�ǳ�
	Activities_Type_MagicWeapon		,	//���Ƿɽ�
	Activities_Type_DrugTripod		,	//ҩ��
	Activities_Type_Banner			,	//���
	Activities_Type_Spirit			,	//���
	Activities_Type_GiftActivities	,	//��Ʒ�

	Activities_Max
};

//-------------------------------------------------------------
//------------------------------ 

static pc_str	g_strActivitiesType[Activities_Max]=
{
	"t_mount"			,
	"t_ridingWeapon"	,
	"t_souls"			,
	"t_bow"				,
	"t_general"			,
	"t_precious"		,
	"t_checkInCount"	,
	"t_equip"			,
	"t_fighters"		,
	"t_keepsake"		,
	"t_base"			,
	"t_mountEquip"		,
	"t_copyMap"			,
	"t_potential"		,
	"t_skill"			,
	"t_generalWeapon"	,
	"t_itemRecord"		,
	"t_city"			,
	"t_magic_weapon"	,
	"t_drugTripod"		,
	"t_banner"			,
	"t_spirit"			,
	"t_gift_activities"	,
};
