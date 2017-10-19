/*----------------- gameStruct.h
*
* Copyright (C): 2011  LiuLei
* Author       : ����Դ
* Version      : V1.0
* Date         : 2011/9/13 13:52:20
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "gameDefine.h"
#include "math/point2.h"
#include "basic/basicFunctions.h"
#include "def_struct/stlDefine.h"
/*************************************************************/
/*************************************************************/
struct _stSkillModel;
struct _stSkillEffectModel;
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ ��Ϸ����ID
struct _stGameUnitID
{
	uint8	uUnitType;
	uint32	uUnitID;

	_stGameUnitID&		getGameUnitID	()	{	return *this;					}
	void				initialize		()	{	memset(this,0,sizeof(*this));	}
};
//-------------------------------------------------------------
//------------------------------ ��Ϸ��������
struct _stGameAttribute
{
	int32	iAttribute	[_emBattleAttribute_ARRAYSIZE];	//��Ϸ���Թ̶�ֵ
	int32	iRadio		[_emBattleAttribute_ARRAYSIZE];	//��Ϸ������ֱ�

	_stGameAttribute()							{	initGameAttribute();			}
	_stGameAttribute&	getGameAttribute	()	{	return *this;					}
	void				initGameAttribute	()	{	memset(this,0,sizeof(*this));	}

	//--- ������Թ̶�ֵ
	inline int32	getAttribute		(uint8 uType)const	{	return ((uType >= _emBattleAttribute_ARRAYSIZE) ? 0 : iAttribute[uType]);	}
	//--- ���������ֱ�
	inline int32	getRadio			(uint8 uType)const	{	return ((uType >= _emBattleAttribute_ARRAYSIZE) ? 0 : iRadio[uType]);		}
	//--- ��������
	int32			updateAttribute		(uint8 uType, int32 nUpdate, int32 nRatio = 0);
	void			updateAttributeExistedAllByRatio	(int32 nRatio , std::set<int32> setIncludeAttr);//����,������ֱ�
	inline bool		isNull()const
	{
		for (int32 i = 0;i < _emBattleAttribute_ARRAYSIZE;++i)
		{
			if(this->iAttribute[i] != 0 || iRadio[i] != 0)
				return false;
		}

		return true;
	}	
	inline bool		read			(CStreamBase& stream)
	{
		int32 uSize = 0;
		bool bResult = stream.read(uSize);
		for (int32 i = 0;i < uSize && i < _emBattleAttribute_ARRAYSIZE; ++i)
		{
			bResult &= stream.read(iAttribute[i]);
			bResult &= stream.read(iRadio[i]);
		}
		
		return bResult;
	}
	inline bool		write			(CStreamBase& stream)
	{
		bool bResult = stream.write(_emBattleAttribute_ARRAYSIZE);		
		for (int32 i = 0;i < _emBattleAttribute_ARRAYSIZE;++i)
		{
			bResult &= stream.write(iAttribute[i]);
			bResult &= stream.write(iRadio[i]);
		}

		return bResult;
	}
	//--- ��������
	_stGameAttribute& operator+=(const _stGameAttribute&_add);
	//--- ��������
	_stGameAttribute& add(const _stGameAttribute* pAdd);
	//--- ��������
	// _stGameAttribute& addTmp(const _stGameAttribute* pAdd);
	inline _stGameAttribute& addTmp(const _stGameAttribute* pAdd)
	{
		if (!pAdd)
			return *this;

		for (int i = 0;i < _emBattleAttribute_ARRAYSIZE;++i)
		{
			updateInt(this->iAttribute[i],pAdd->iAttribute[i]);
			updateInt(this->iRadio[i],pAdd->iRadio[i]);
		}

		return *this;
	}

	//--- ���Լ���
	_stGameAttribute& operator-=(const _stGameAttribute&_reduce);
	//--- ���Է���
	_stGameAttribute& operator*=(int32 _mul);
	//--- ���Է���
	_stGameAttribute& operator*=(float32 _mul);
};
//****************************************************************
//-------------------------------------------------------------
//------------------------------ �������Buff
struct _stSkillBuff
{
	uint32			uAttackID;		//��������ID
	uint32			uBuffID;		//��buff
	uint64			uDelayTime;		//�����ٺ���֮��
	uint32			uDurationTime;	//buff����ʱ��ms

	void		initSkillBuff()	{	memset(this,0,sizeof(*this));	}
	_stSkillBuff&getSkillBuff()	{	return *this;					}
};

//-------------------------------------------------------------
//------------------------------ ����ս������
struct _stAddBattleAttri
{
	uint16		uAttrType;		//_emBattleAttribute
	int32		iValue;			//ֵ
	int32		iRadio;			//���� ��ֱ�
	uint16		uTransAttrType;	//ת�������� 0������
	int32		iTransRadio;	//ת����������ֱ�

	READ_WRITE_STRUCT
};

//-------------------------------------------------------------
//------------------------------ģ������
struct _stAvatarAnimCfg
{
	uint16	uLandAnimTick;		// ����ʱ��(����)
	uint16	uClimbAnimTick;		// ����ʱ��(����)
	float32	fModelRadius;		// ģ�Ͱ뾶
	uint32  uBeAttackMoveTick;  //�ܻ�λ��ʱ��
	uint32	uBirthTick;			// ����ʱ��(����)
	uint32	uDeadTick;			// ����ʱ��(����)
	_stAvatarAnimCfg()
	{
		uLandAnimTick = 0;
		uClimbAnimTick = 0;
		uBirthTick = 0;
		uDeadTick = 0;
		fModelRadius = 0;
	}
};

//-------------------------------------------------------------
//------------------------------�����ָ�
struct _stEnergyRecovery
{
	uint64	uTick;		// ʱ��
	int32	iValue;		// �ָ���

	_stEnergyRecovery()
	{
		uTick	= 0;
		iValue	= 0;
	}
};

//-------------------------------------------------------------
//------------------------------ ����ս������
struct _stAddExpend
{
	uint16		uExpendType;		//_emExpendType
	int32		iValue;			//ֵ

	READ_WRITE_STRUCT
};

//-------------------------------------------------------------
//------------------------------ ����ս������
//	�û����Ե����
struct _stPlayerAttributeAlloc
{
	uint32 uTotalAttributes;			// �ܵĿɷ�������Ե�
	uint32 uLeftAttributes;				// ʣ��ɷ�������Ե�
	uint32 uStrengthAlloced;			// ���������
	uint32 uBoneAlloced;				// ����Ľ��
	uint32 uBloodAlloced;				// �������Ѫ
	uint32 uBodyAlloced;				// �������
	uint32 uMurderAlloced;				// �����ɱ��
	_stPlayerAttributeAlloc() {	 clear();	}
	void clear() {
		memset(this,0,sizeof(*this));
	}
	void reset() {
		uTotalAttributes = 0;
		uTotalAttributes += uStrengthAlloced;
		uTotalAttributes += uBoneAlloced;
		uTotalAttributes += uBloodAlloced;
		uTotalAttributes += uBodyAlloced;
		uTotalAttributes += uMurderAlloced;

		uStrengthAlloced = 0;
		uBoneAlloced = 0;
		uBloodAlloced = 0;
		uBodyAlloced = 0;
		uMurderAlloced = 0;
		uLeftAttributes = uTotalAttributes;
	}
};
//-------------------------------------------------------------
//------------------------------
struct _stExpendItem
{
	uint64 uItemSN;
	uint16 uItemNum;

	_stExpendItem()		{	memset(this, 0, sizeof(*this));	}
};
//-------------------------------------------------------------
//------------------------------ ���ܹ����ӳ�
struct _stItemNum
{
	uint32 uItemID;
	uint16 uItemNum;

	_stItemNum()		{	init();							}
	void init()			{	memset(this, 0, sizeof(*this));	}
	READ_WRITE_STRUCT;

};
//-------------------------------------------------------------
//------------------------------ ��Ϸ�洢����
struct _stGameLogSave
{
	uint32	uPlayerID;			//���ID
	uint32	uTime;				//ʱ��
	uint32	uDoingMainType;		//������
	uint32	uDoingSubType;		//С����
	uint32	uObjectID;			//����ID(����ID������ID)
	uint32	uCurValue;			//��ǰ��ֵ
	int32	iChangeValue;		//�ı�ֵ

	_stGameLogSave()	{	memset(this, 0, sizeof(*this)); }
};
#pragma pack(pop)
//-------------------------------------------------------------
//------------------------------ ���ܹ����ӳ�
struct _stSkillAttack
{
	uint32					uSouceID;				//ʩ����ID
	_stPoint2F				stTargetPoint;			//ʩ����Ŀ���
	uint64					uDelayTick;				// ��ʱ����
	const _stSkillModel*	pSkillModel;		
	const _stSkillEffectModel*	pSkillEffectModel;

	_stSkillAttack()	
	{
		uSouceID = 0;
		stTargetPoint.zero();
		pSkillEffectModel = NULL;
		pSkillModel = NULL;
		uDelayTick = 0;
	}
};
