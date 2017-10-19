/*----------------- buffModel.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����
* Version      : V1.0
* Date         : 2016/2/1 20:14:11
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
#include "basic/memoryFunctions.h"
#include "math/range.h"
#include "def_struct/gameStruct.h"
/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ buff����������������
struct _stBuffContiueAddExpend
{
	uint32		uEffectTime;		//��Чʱ��
	uint32		uStepTime;			//���ʱ��
	uint16		uExpendType;		//_emExpendType
	uint8		uTargetCategory;	//_emEffectTargetType
	uint16		uAttrType;			//_emBattleAttribute
	int32		iRadio;				//���� ��ֱ�
	int32		iValue;				//���ӵ�ǰֵ
	uint16		uDelExpendType;		//_emExpendType
	int32		iDelValue;			//����ֵ
};
//-------------------------------------------------------------
//------------------------------ buff��������Buff
struct _stBuffContiueAddBuff
{
	uint32		uEffectTime;		//��Чʱ��
	uint32		uStepTime;			//���ʱ��
	uint32		uBuffID;
};

//-------------------------------------------------------------
//------------------------------ buff����λ��
struct _stBuffAddMove
{
	uint8		uBuffMoveType;		//λ������ _emBuffMoveType
	uint8		uDirectionType;		//�������� _emDirectionType
	float32		fSpeed;				//�ٶ�
	uint8		uTargetType;		//λ��Ŀ������ _emBuffMoveTargetType
};
//-------------------------------------------------------------
//------------------------------ buff��������ͷż���
struct _stBuffContiueUseSkill
{
	uint32		uEffectTime;		//��Чʱ��
	uint32		uStepTime;			//���ʱ��
	uint32		uSkillID;			//����ID
};
//-------------------------------------------------------------
//------------------------------ buff���Ӷ����˺�
struct _stBuffSkillExtraDamage
{
	uint32		uSkillID;			//����ID
	uint32		uValue;				//ֵ
	uint32		uRadio;				//��ֱ�
};
//-------------------------------------------------------------
//------------------------------ buff���ټ��������ȴʱ��
struct _stBuffSkillCoolTime
{
	uint32		uSkillID;			//����ID
	uint8		uSkillTimeType;		//����ʱ������ _emSkillTimeType
	uint32		uRadio;				//��ֱ�

	_stBuffSkillCoolTime()	{	memset(this,0,sizeof(*this)); }

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ buff���ټ��������ȴʱ��
struct _stBuffDeleteTriggerEvent
{
	uint8		uType;				//����ID
	uint8		uSkillTimeType;		//����ʱ������ _emSkillTimeType
	uint32		uRadio;				//��ֱ�
};
//-------------------------------------------------------------
//------------------------------ buff��������Ч�� ����
struct _stBuffAddCureEffet 
{
	float32		fHPRadio;		//�������� �����ٷֱ�
	bool		bGreater;		//�������� �Ƿ���ڵ���
	float32		fEffetRadio;	//���ӵ�Ч��
};
//-------------------------------------------------------------
//------------------------------ buff�����˺�Ч�� ����
struct _stBuffAddHarmEffet 
{
	float32		fHPRadio;		//�������� �����ٷֱ�
	bool		bGreater;		//�������� �Ƿ���ڵ���
	float32		fEffetRadio;	//���ӵ�Ч��
	uint32		uBeSkillID;		//����ID Ϊ0 ���м��ܶ���Ч
};
//-------------------------------------------------------------
//------------------------------ buff�������ӱ����� ����
struct _stBuffAddCritical 
{
	uint8		uConditionType; //�������� _emBuffCriticalConditionType
	int32		nParam;			//����
	float32		fEffetRadio;	//���ӵ�Ч��
};
//-------------------------------------------------------------
//------------------------------ ���ӳ���BUFF����ʱ�� ����
struct _stBuffAddBuffTime
{
	uint16		uBuffGroupID;	//buff����ID
	float32		fEffetRadio;	//���ӵ�Ч��
};
//-------------------------------------------------------------
//------------------------------ ��Ѫ
struct _stBuffSuckHP
{
	bool		bNeedCritical;	//�Ƿ���Ҫ����
	float32		fEffetRadio;	//��Ѫ�ٷֱ�
};
//-------------------------------------------------------------
//------------------------------ ���ӳ���BUFF����ʱ�� ����
struct _stBuffAddSkillBuffTime
{
	uint16		uBuffGroupID;	//buff����ID
	int32		nAddBuffMS;		//���ӵĺ���
};
//-------------------------------------------------------------
//------------------------------ ���ӳ���BUFF����ʱ�� ����
struct _stBuffAddTargetHarm
{
	float32		fHPRadioMax;	//�������� �����ٷֱ�����
	float32		fHPRadioMin;	//�������� �����ٷֱ�����
	float32		fHPSep;			//ÿ�������ٷֱ�
	int32		nAddHarmValue;	//�����˺�����ֵ
	float32		fAddHarmRadio;	//�����˺��ٷֱ�
};
//-------------------------------------------------------------
//------------------------------ һ������ֵ����Ӱ������һ������ֵ����
struct _stBuffExpendAddExpend
{
	uint16 uExpendType1;
	int32 iValue1;
	uint16 uExpendType2;
	int32 iValue2;
};
//-------------------------------------------------------------
//------------------------------ ���������з���ʹ�ü���
struct _stBuffBeSkillHitUseSkill
{
	uint32 uBeSkillID;
	uint32 uUseSkillID;
};
//-------------------------------------------------------------
//------------------------------ ����������ɾ��buff
struct _stBuffBeSkillHitDelBuff
{
	uint32 uBeSkillID;
};
//-------------------------------------------------------------
//------------------------------ 
struct _stBuffModelBase
{
	uint32					uBuffID;
	uint8					uLevel;	
	uint32					uGroupID;				//��ID ���ձ�cs_buff_group_mutual
	uint8					uDifRepeatType;			//��ͬ��Դ�������� _emBuffDifRepeatType
	uint8					uMaxLevel;				//���ȼ�
	uint8					uBuffType;				//���� _emBuffType
	uint16					uStateType;				//״̬
	uint32					uStateBuffAttrMark;		//״̬��buff���������� _emStateBuffAttr(Ϊ�˼ӿ�����ٶ� �Ż��������� )
	uint8					uEffectType;			//Ч������ _emEffectType
	uint8					uRepeatType;			//�������� _emBuffSameRepeatType
	uint32					uContinueTime;			//����ʱ��
	uint32					uMaxContinueTime;		//������ʱ��	
	uint8					uMaxLayer;				//������ �޲���Ĭ��1
	bool					bIsClearLayer;			//ʱ�䵽�Ƿ����
	uint32					uMark;					//���� _emBuffMark
	uint8					uBuffCat;				//���� _emBuffCat
	union 
	{// uBuffType ��һ������һ��	
		_stBuffContiueAddExpend		stContiueAddExpend;		//���������������� BuffType_ContiueAddExpend
		_stBuffContiueAddBuff		stContiueAddBuff;		//��������Buff BuffType_ContiueAddBuff
		_stBuffAddMove				stAddMove;				//buff����λ�� BuffType_Move
		_stBuffContiueUseSkill		stContiueUseSkill;		//�����ͷż��� BuffType_ContiueUseSkill
		_stAddExpend				stAttackAddExpend;		//�������������� BuffType_AttackAddExpend
		_stBuffSkillExtraDamage		stSkillExtraDamage;		//���ܹ��������˺� BuffType_SkillExtraDamage
		int32						nReduceSkillExpend;		//���ټ���������ֱ� BuffType_SkillExpendReduce
		_stBuffAddCureEffet			stBuffAddCureEffet;		//����Я���߱�����Ч�� BuffType_AddCureEffet
		_stBuffAddHarmEffet			stBuffAddHarmEffet;		//����Я���߱��˺�Ч�� BuffType_AddHarmEffet
		_stBuffAddCritical			stBuffAddCritical;		//����Я���߱����� BuffType_AddCriticalEffet
		_stBuffAddBuffTime			stBuffAddBuffTime;		//����Я����BUFF����ʱ�� BuffType_AddBuffTime
		_stBuffSuckHP				stBuffSuckHP;			//Я���߹�����Ѫ BuffType_SuckHP
		float32						fBuffAddHarmMonster;	//����Я���߶Թ�����˺� BuffType_AddHarmMonster
		_stBuffAddSkillBuffTime		stBuffAddSkillBuffTime; //ʹ��ѣ�μ���ʱ����buff����ʱ�� BuffType_AddSkillBuffTime
		_stBuffAddTargetHarm		stBuffAddTargetHarm;	//����Я���ߵ��˺�(Ѫ��Խ���˺�Խ��) BuffType_AddTargetHarm
		_stBuffExpendAddExpend		stExpendAddExpend;		//һ������ֵ����Ӱ������һ������ֵ���� BuffType_ExpendAddExpend
		_stBuffBeSkillHitUseSkill	stBeSkillHitUseSkill;	//������������Դ��buff����ʹ�ü��� BuffType_BeSkillHitUseSkill
		_stBuffBeSkillHitDelBuff	stBeSkillHitDelBuff;	//����������ɾ��buff BuffType_BeSkillHitDelBuff
		int32						nHatredLevel;			//�ı��޵ȼ� BuffType_HatredLevel
		_stBuffBeSkillHitUseSkill	stBeSkillHitTarUseSkill;//������������Դ��Ŀ����ʹ�ü��� BuffType_BeSkillHitUseSkill

	};

	_stBuffModelBase&	getBase		()				{	return *this;							}
	inline void			initBase	()				{	dMemset(this,0,sizeof(*this));			}

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ ״̬��buff������
struct _stBuffStateAttrModel
{
	uint32					uStateID;
	uint32					uStateAttrMark;		//״̬��buff����������

	_stBuffStateAttrModel&	getModel	()				{	return *this;							}
	inline void				initModel	()				{	dMemset(this,0,sizeof(*this));			}
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stBuffModel : _stBuffModelBase
{
	typedef stl_vector<_stAddBattleAttri> VecAddBattleAttri;
	typedef stl_map<uint32,_stBuffSkillCoolTime>	MAP_SKILL_COOLTIME;

	MAP_U8_U32			mapBuff;				//������buff
	MAP_U8_U32			mapSkillID;				//����ʹ�ü���ID
	MAP_U8_U32			mapDeleteTriggerEvent;	//ɾ��buff�����¼� KEY:_emBuffDeleteTriggerEventType
	VecAddBattleAttri	vecAddBattleAttr;		//����ս������
	MAP_SKILL_COOLTIME	mapSkillCoolTime;		//���ټ�����ȴʱ�� BuffType_ChangeSkillTime

	inline bool		check_mark	(uint8 _mark)	const	{	return _CHECK_BIT(uMark,_BIT32(_mark));	}
	_stBuffModel&	getModel	()						{	return *this;							}
	inline void		initModel	()				
	{
		initBase();
		mapBuff.clear();
		mapDeleteTriggerEvent.clear();
		mapSkillID.clear();
		vecAddBattleAttr.clear();
		mapSkillCoolTime.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stBuffModelBase::read(stream)&&
			stream.read(mapBuff)&&
			stream.read(mapSkillID)&&
			stream.read(vecAddBattleAttr)&&
			stream.read(mapDeleteTriggerEvent)&&
			stream.read(mapSkillCoolTime);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stBuffModelBase::write(stream)&&
			stream.write(mapBuff)&&
			stream.write(mapSkillID)&&
			stream.write(vecAddBattleAttr)&&
			stream.write(mapDeleteTriggerEvent)&&
			stream.write(mapSkillCoolTime);
	}
};