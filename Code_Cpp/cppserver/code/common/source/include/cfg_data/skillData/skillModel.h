/*----------------- skillModel.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����
* Version      : V1.0
* Date         : 2016/1/21 13:39:53
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/memoryFunctions.h"
#include "math/range.h"
#include "def_struct/stlDefine.h"
#include "math/point2.h"
#include "basic/basicTypes.h"
#include "def_struct/gameStruct.h"
/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ ���Ӷ���
struct _stSkillAddObject
{
	uint8		uObjectType;		//�������� _em_GameUnitType
	uint32		uOjbectID;			//����ID
	uint8		uTargetPositonType;	//Ŀ��λ������  _emEffectTargetType
	float32		fDistance;			//���λ�� ����Ϊ��
	float32		fHigh;				//�߶�
	float32		fRandDis;			//����뾶��
	uint8		uObjectNum;			//����������������;
	uint16		uParam1;			//��������_emTrapAngleType ���ﲻ��
	uint16		uParam2;			//�����ӵ�����Ƕ� �����ܺ�;   ���ﲻ��
	uint8		uExpendType;		//��������ֵ
	int32		iValue;				//��������
	uint8		uAddNum;			//���Ӹ���

	_stSkillAddObject()	{	memset(this,0,sizeof(*this));	}
};
//-------------------------------------------------------------
//------------------------------ ���Ӷ���
struct _stSkillAddBuff
{
	uint16	uExpendType;//���ĳɹ�
	int32	iValue;
	uint32	uBuffID;

	_stSkillAddBuff()	{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};

//-------------------------------------------------------------
//------------------------------ ����Ч������
struct _stSkillEffectType
{
	uint8	uEffectType;		//_emSkillEffectType
	uint8	uEffectTargetType;	//_emEffectTargetType
	bool	bSkillHit;			//������������
	bool	bAddOneCount;		//�Ƿ�ֻ��һ��

	_stSkillEffectType()	{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};

//-------------------------------------------------------------
//------------------------------ ���ܷ�Χ
struct _stSkillRangeData
{
	uint8				uRangeType;			//��Χ���� _emRangeType
	float32				fLength;			//�� Բ��ʾ�뾶 ����ʾ��뾶
	float32				fWidth;				//�� Բ��ʾ�Ƕ� ����ʾ�ڰ뾶
	float32				fOffset;			//ƫ����
	float32				fOffsetAngle;		//ƫ�Ƴ���ĽǶ�

	_stSkillRangeData()	{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ ��ϼ���
struct _stSkillCompositeData
{
	uint8		uType;							//������� _emSkillCompositeType
	uint8		uAtkPart;						//��λ
	uint32		uNextSkillID;					//��һ������ID
	uint32		uAtkPartCoolTicket;				//��λ��ȴʱ��(����)

	_stSkillCompositeData()	{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ ���Ӷ���
struct _stSkillBeAttackMove
{
	uint8				uDirectionType;		//�ܻ�λ������ _emDirectionType
	float32				fBeAttackMove;		//�ܻ�λ�ƾ���
	uint32				uBuffID;			//buffID ѣ����

	_stSkillBeAttackMove()	{	memset(this,0,sizeof(*this));	}
};
//-------------------------------------------------------------
//------------------------------ �������Ӷ����˺�
struct _stSkillExpendAddDamage
{
	uint8	uExpendType;		//_emExpendType
	int32	iValue;
	uint32	uRate;				//��ֱ�
	uint32	uDamage;			//�˺�ֵ

	_stSkillExpendAddDamage()	{	memset(this,0,sizeof(*this));	}
};
//-------------------------------------------------------------
//------------------------------ ����λ��
struct _stSkillEffectAddMove
{
	uint8				uAddMoveType;		//�ܻ�λ������ _emEffectAddMoveType
	int32				iDistance;			//λ�ƾ��룬������ʾ����

	_stSkillEffectAddMove()	{	memset(this,0,sizeof(*this));	}
};
//-------------------------------------------------------------
//------------------------------ ����Ч����������
struct _stSkillEffectBase
{
	uint32				uEffectID;			//Ч��ID
	uint8				uLevel;				//Ч���ȼ�
	uint8				uMaxAffectNum;		//����Ŀ�����
	uint8				uTargetSelectType;	//Ŀ��ѡȡ��ʽ  _emTargetSelectType
	int32				nHatredValue;		//���ӳ��
	uint32				uExtraDamage;		//�����˺�ֵ
	uint32				uExtraDamageRadio;	//�����˺�ϵ��
	uint32				uAverageDamageRadio;//��̯�˺�ϵ�� ����Ŀ����� Ӱ�챩������
	uint32				uParamDamageRadio;	//�⻷�˺�ϵ��
	uint32				uOutRingAddBuff;	//�⻷����buff
	uint32				uExtraCure;			//��������ֵ
	uint32				uExtraCureRadio;	//��������ϵ��
	_stSkillAddObject	stAddObject;		//���Ӷ���
	_stSkillBeAttackMove stBeAttackMove;	//�ܻ�λ��
	uint16				uHitUseSkill;		//����Ŀ�괦ʹ�ü���
	_stSkillExpendAddDamage stExpendAddDamage; //�������Ӷ����˺�
	uint32				uResetBuffID;		//����buffʱ���ID
	_stSkillEffectAddMove	stAddMove;		//����λ��
	_stSkillEffectBase&	getBase()	{	return *this;					}
	inline void			initBase()	{	dMemset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};

//-------------------------------------------------------------
//------------------------------ ���ܶ�����������
struct _stSkillAnimBase
{
	uint32	uTotalTick;		//������ʱ��

	_stSkillAnimBase&	getBase()	{	return *this;					}
	inline void		initBase()		{	dMemset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};

//-------------------------------------------------------------
//------------------------------ ���ܻ�������
struct _stSkillBase
{
	uint32		uSkillID;						//����ID
	uint8		uType;							//���� _emSkillType
	uint8		uMaxLevel;						//�������ȼ�
	uint8		uLevel;							//���ܵȼ�
	uint8		uBehavior;						//ʹ������(_emSkillBehavior)
	uint64		uMark;							//��������(_emSkillMark)
	uint8		uDirStrategy;					//���ܷ���ѡ�� _emSkillDirStrategy
	uint32		uChargeTime;					//��������ʱ��(����)
	uint32		uContinueTime;					//���ܳ���ʱ��(����)
	uint32		uCoolTicket;					//��ȴʱ��(����)
	uint32		uGroupCoolTicket;				//����CD��ȴʱ��(����)
	uint32		uTagetCategory;					//Ŀ������ _emTargetCategory
	uint32		uReleaseRange;					//�������
	uint16		uHaloID;						//�⻷ID
	_stSkillCompositeData stCompositeData;		//�������
	uint8		uUseType;						//ʹ������_emSkillUseType
	int32		nUseParam;						//ʹ�ò���
	int32		nRangeWarnType;					//���ܾ�������
	int32		nRangeWarnID;					//���ܾ���id
	uint32		uCompositeBaseSkillID;			//��ϻ������ܼ���ID
	uint32		uInterruptValue;				//�ж�ֵ
	_stSkillExpendAddDamage stExpendAddDamage; //�������Ӷ����˺�

	_stSkillBase&	getBase()	{	return *this;					}
	inline void		initBase()	{	dMemset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ ����Ч������ģ��
struct _stSkillEffectModel	: _stSkillEffectBase
{
	typedef stl_map<uint8,_stSkillRangeData> MapSkillRangeData;
	typedef double_map<uint8,uint32,_stSkillAddBuff> DMAP_SKILLADDBUFF;
	DMAP_SKILLADDBUFF						dmapAddBuff;		//����buff key1:_emEffectTargetType,key2:buffid
	MAP_U16_I64								mapAddExpend;		//����Ŀ������
	MAP_U16_I64								mapAddSelfExpend;	//������������
	stl_map<uint16,_stAddBattleAttri>		mapAddSelfAttr;		//������������
	stl_vector<_stSkillEffectType>			vecEffectType;		//Ч������
	MapSkillRangeData						mapRangeData;		//��Χ���� �����Χ
	
	_stSkillEffectModel()		{	initModel();					}
	inline void		initModel()
	{
		initBase();
		dmapAddBuff.clear();
		mapAddExpend.clear();
		mapAddSelfExpend.clear();
		mapAddSelfAttr.clear();
		vecEffectType.clear();
		mapRangeData.clear();

	}
	inline _stAddBattleAttri* findAttr(uint16 uAttriType)
	{
		return mapAddSelfAttr.find_(uAttriType);
	}
	inline bool		read			(CStreamBase& stream)
	{
		return _stSkillEffectBase::read(stream)&&
			stream.read(dmapAddBuff)&&
			stream.read(mapAddExpend)&&
			stream.read(mapAddSelfExpend)&&
			stream.read(mapAddSelfAttr)&&
			stream.read(vecEffectType)&&
			stream.read(mapRangeData);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stSkillEffectBase::write(stream)&&
			stream.write(dmapAddBuff)&&
			stream.write(mapAddExpend)&&
			stream.write(mapAddSelfExpend)&&
			stream.write(mapAddSelfAttr)&&
			stream.write(vecEffectType)&&
			stream.write(mapRangeData);
	}
};

//-------------------------------------------------------------
//------------------------------ 
struct _stAttEffect
{
	uint32						uTick;
	float32						fDir;
	float32						fDistance;
	uint8						uUseRange;
	const _stSkillEffectModel*	pEffectModel;

	_stAttEffect()
	{
		init();
	}

	void init()
	{
		uTick = 0;
		pEffectModel = NULL;
		uUseRange = 1;
		fDir		= 0.f;
		fDistance	= 0.f;
	}

	inline bool		read			(CStreamBase& stream)
	{
		return stream.read(uTick)&&
			stream.read(fDir)&&
			stream.read(fDistance)&&
			stream.read(uUseRange);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return stream.write(uTick)&&
			stream.write(fDir)&&
			stream.write(fDistance)&&
			stream.write(uUseRange);
	}
};

//-------------------------------------------------------------
//------------------------------ ���ܶ�������ģ��
struct _stSkillAnimModel	: _stSkillAnimBase
{
	stl_vector<_stAttEffect>		vecAttEffect;//����λ��֡
	inline void		initModel()
	{
		initBase();
		vecAttEffect.clear();
	}
	inline bool		read			(CStreamBase& stream)
	{
		return _stSkillAnimBase::read(stream)&&
			stream.read(vecAttEffect);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stSkillAnimBase::write(stream)&&
			stream.write(vecAttEffect);
	}
};

//-------------------------------------------------------------
//------------------------------
struct _stSkillSpareParam
{
	stl_vector<float32> vecParams;
	_stSkillSpareParam() {   vecParams.clear(); }

	inline bool		read			(CStreamBase& stream)
	{
		return 	stream.read(vecParams);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return 	stream.write(vecParams);
	}
};

//-------------------------------------------------------------
//------------------------------ ���ܻ���ģ��
struct _stSkillModel	: _stSkillBase
{
	_stSkillAnimModel*						pAnimModel;		//����ģ��
	MAP_U16_I64								mapAddExpend;	//��������
	VEC_U32									vecEffect;		//Ч����
	stl_vector<const _stSkillEffectModel*>	vecEffectModel;	//������ģ��
	stl_map<uint8,_stSkillSpareParam>		mapSpareParam;	//���ɲ���
	SET_U32									setOugiSkill;	//���弼��
	SET_U32									setNeedJob;		//��Ҫ��ְҵ

	inline bool		check_mark	(uint8 _mark)const	{	return _CHECK_BIT(uMark,_BIT64(_mark));	}
	_stSkillModel() {   initModel(); }
	inline const _stSkillSpareParam* findParam(uint8 uType)	const	{	return mapSpareParam.find_(uType);	}
	inline void		initModel()
	{
		initBase();
		mapAddExpend.clear();
		pAnimModel = NULL;
		vecEffect.clear();
		vecEffectModel.clear();
		mapSpareParam.clear();
		setOugiSkill.clear();
		setNeedJob.clear();
	}
	inline bool		read			(CStreamBase& stream)
	{
		return _stSkillBase::read(stream)&&
			stream.read(mapAddExpend)&&
			stream.read(vecEffect)&&
			stream.read(mapSpareParam)&&
			stream.read(setNeedJob)&&
			stream.read(setOugiSkill);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stSkillBase::write(stream)&&
			stream.write(mapAddExpend)&&
			stream.write(vecEffect)&&
			stream.write(mapSpareParam)&&
			stream.write(setNeedJob)&&
			stream.write(setOugiSkill);
	}
};