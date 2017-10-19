/*------------- jobModel.h
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : �־�
* Version      : V1.01
* Date         : 2016/10/11
*
*/ 
/***************************************************************
* 
***************************************************************/
#include "def_struct/stlDefine.h"
#include "def_struct/gameStruct.h"
#include "timer/dateTime.h"

#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 
struct _stFactionPrivilegeModelBase
{
	uint32				uFactionOpt;		// ������
	uint8				uFactionJob;		// ���ְλ

	_stFactionPrivilegeModelBase&	get()			{	return *this;					}
	void							init()			{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ ��Ἴ��
struct _stFactionSkillModel
{
	uint32				uSkllID;				// ��Ἴ��ID
	uint32				uLevel;					// ��Ἴ�ܵȼ�
	uint32				uNeedFactionLevel;		// �о���Ҫ���ɵȼ�
	uint32				uNeedSliver;			// �о���Ҫ�ʽ�
	uint32				uNeedStone;				// �о���Ҫ��ʯ
	uint32				uNeedTime;				// �о�ʱ�䣨�룩
	uint32				uLearnNeedSliver;		// ѧϰ��Ҫ������
	uint32				uLearnNeedContribution;	// ѧϰ��Ҫ�İﹱ
	_stGameAttribute	stAttr;					// ѧϰ������������

	_stFactionSkillModel&			get()			{	return *this;					}
	void							init()			{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ �����ʽ�
struct _stFactionDonateSliverModel
{
	uint32				uID;					// ����ID
	uint32				uSliver;				// ���׽��
	uint32				uContribution;			// ��ðﹱ
	uint32				uFactionSliver;			// ����ð���ʽ�
	uint32				uCount;					// ÿ�տɾ��״���
	bool				bBroadCast;				// �Ƿ��ڰ��Ƶ���㲥

	_stFactionDonateSliverModel&	get()			{	return *this;					}
	void							init()			{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ ������ ��������
struct _stFactionDrinkWineConstantModelBase
{
	uint32				uNpcID;			// NPC ID
	float32				fPosX;			// NPC POSX
	float32				fPosY;			// NPC POSY
	float32				fAngleDir;		// NPC DIRECTION
	uint16				uRegionID;		// ��������ID

	_stFactionDrinkWineConstantModelBase&	get()		{	return *this;					}
	void								init()			{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};

//-------------------------------------------------------------
//------------------------------ ������
struct _stFactionDrinkWineModel
{
	uint32				uID;					// ������ID
	uint32				uNeedContribution;		// ����ﹱ
	uint32				uWisdom;				// �ɻ�õ��������
	uint32				uTimeInterval;			// ������Լ��(��)
	uint32				uWisdomInterval;		// ÿ�����ӵ������

	_stFactionDrinkWineModel&	get()				{	return *this;					}
	void							init()			{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ ��������
struct _stFactionTaskModelBase
{
	uint32				uID;					// ����ID
	uint32				uFactionLevel;			// ���ȼ�
	uint32				uNeedCount;				// ��Ҫ��������
	uint32				uTypeCount;				// ��Ҫ����������
	uint32				uAwardContribution;		// �����İﹱ
	uint32				uAwardWisdom;			// ���������
	uint32				uFactionSliver;			// ���ӵİ���ʽ�
	uint32				uFactionBoom;			// ���ӵİ�ᷱ�ٶ�

	_stFactionTaskModelBase&		get()			{	return *this;					}
	void							init()			{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ ���ȼ�����
struct _stFactionLevelModel
{
	uint32				uLevel;					// ���ȼ�
	uint32				uMemberCount;			// ����������
	uint32				uEnergyMax;				// �ж�������
	uint32				uNeedBoom;				// ������Ҫ���ٶ�
	uint32				uNeedExperience;		// ������Ҫ����

	_stFactionLevelModel&			get()			{	return *this;					}
	void							init()			{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ ����ְ��
struct _stFactionJobModelBase
{
	uint32				uJob;					// ְ��
	uint32				uCountLimit;			// ��������
	uint64				uDefaultPrivilegeMark;	// Ĭ�Ͽ���Ȩ��
	uint64				uForbidPrivilegeMark;	// �����޸�Ȩ��

	_stFactionJobModelBase&		get()			{	return *this;					}
	void						init()			{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stFactionJobModel : _stFactionJobModelBase
{
	_stFactionJobModel()	{ init();	}
	inline void	init()
	{
		_stFactionJobModelBase::init();
	}

	inline bool read(CStreamBase& stream)
	{
		return _stFactionJobModelBase::read(stream);
	}
	inline bool write(CStreamBase& stream)
	{
		return _stFactionJobModelBase::write(stream);
	}
};
//-------------------------------------------------------------
//------------------------------ 
struct _stFactionPrivilegeModel : _stFactionPrivilegeModelBase
{
	_stFactionPrivilegeModel()	{ init();	}
	inline void	init()
	{
		_stFactionPrivilegeModelBase::init();
	}

	inline bool read(CStreamBase& stream)
	{
		return _stFactionPrivilegeModelBase::read(stream);
	}
	inline bool write(CStreamBase& stream)
	{
		return _stFactionPrivilegeModelBase::write(stream);
	}
};
//-------------------------------------------------------------
//------------------------------ 
struct _stFactionTaskModel : _stFactionTaskModelBase
{
	stl_vector<uint32>		vecItemID;
	_stFactionTaskModel()	{ init();	}
	inline void	init()
	{
		_stFactionTaskModelBase::init();
		vecItemID.clear();
	}

	inline bool read(CStreamBase& stream)
	{
		return _stFactionTaskModelBase::read(stream)
			&& stream.read(vecItemID);
	}
	inline bool write(CStreamBase& stream)
	{
		return _stFactionTaskModelBase::write(stream)
			&& stream.write(vecItemID);
	}
};
//-------------------------------------------------------------
//------------------------------ 
struct _stFactionDrinkWineConstantModel	: public _stFactionDrinkWineConstantModelBase
{
	stl_vector<_stDateTime>					vecOpenTime;				// ����ʱ��

	_stFactionDrinkWineConstantModel&		get			(){	return *this;					}
	inline void		init()
	{
		_stFactionDrinkWineConstantModelBase::init();
		vecOpenTime.clear();
	}
	inline bool		read			(CStreamBase& stream)
	{
		return _stFactionDrinkWineConstantModelBase::read(stream)&&
			stream.read(vecOpenTime);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stFactionDrinkWineConstantModelBase::write(stream)&&
			stream.write(vecOpenTime);
	}
	inline bool		isInOpenTime	(uint64 uTime) const 
	{
		if ( vecOpenTime.empty() )
			return false;

		uint32	uSize 	= vecOpenTime.size();
		for ( uint32 i = 0; i < uSize; ++i )
		{
			const  _stDateTime& stDataTime = vecOpenTime[i];
			if (stDataTime.toDataTime(uTime))
				return true;
		}
		return false;
	}
};