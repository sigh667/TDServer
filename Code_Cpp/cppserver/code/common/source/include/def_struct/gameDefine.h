/*----------------- gameDefine.h
*
* Copyright (C): 2011  LiuLei
* Author       : ����Դ
* Version      : V1.0
* Date         : 2011/9/13 14:13:17
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "protocol/GameDefine.pb.h"
#include "basic/basicTypes.h"
#include "def_struct/playerDefine.h"
/*************************************************************/
static const uint32 g_uBattleObject	= _BIT32(GameObject_Player) | _BIT32(GameObject_Monster)| _BIT32(GameObject_Pet)| _BIT32(GameObject_Trap);
//-------------------------------------------------------------
//------------------------------ ��ϷGhost����
enum _em_GameGhost
{
	Ghost_Remove		,//�Ƴ��˶���(���Լ�����Ұ����ʧ)
	Ghost_State			,//״̬����
	Ghost_Property		,//���Ը���
	Ghost_HPMP			,//����ħ������
	Ghost_Move			,//�����ƶ�
	Ghost_Base			,//�������ݸ���
	Ghost_SuperArmor	,//������Ϣ
	Ghost_Pk			,//Pk��Ϣ
	Ghost_FightPower	,//ս����
	Ghost_Qinggong		,//�Ṧ
	Ghost_Camp			,//��Ӫ
	Ghost_Speed			,//�ƶ��ٶ�

	Ghost_Max
};
enum _emUpdateValue
{
	_UV_AttributeBase	,//���¼���������Գ�buff��
	_UV_Shape			,//���¼�������
	_UV_AttributeBuff	,//���¼���Buff����
	_UV_TempSkill		,//������ʱ����
	_UV_PassiveSkillBuff,//���±����������ӵ�buff

	_UV_Max
};
#include "gameDefine_inline.h"
