/*----------------- monsterModel.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2016/2/19 17:08:23
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "def_struct/gameStruct.h"
#include "stream/streamBase.h"
#include "def_struct/stlDefine.h"
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 
struct _stDelayAddBuff
{
	uint32				uDelayTick;			// �������
	uint32				uBuffID;			// buffID

	_stDelayAddBuff	()		{	init();							}
	void	init	()		{	memset(this,0,sizeof(*this));	}

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stAIOrdinalSkill
{
	uint32				uSkillID;			// ����ID
	uint32				uPriority;			// ���ȼ�

	_stAIOrdinalSkill	()		{	init();							}
	void	init	()		{	memset(this,0,sizeof(*this));	}

	inline bool operator < (const _stAIOrdinalSkill& rhs)const
	{
		if (uPriority != rhs.uPriority)
			return uPriority > rhs.uPriority ;

		return this > &rhs;
	}

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stMonsterModelBase
{
	uint32				uMonsterID;
	uint8				uType;				// ���� _emMonsterType
	uint8				uSubType;			// ������ _emMonsterSubType
	uint8				uCampID;			// ��Ӫ
	uint32				uMark;				// ����	_emMonsterMark
	float32				fCallDist;			// ս�����о���
	uint32				uBirthTick;			// ����ʱ��(����)
	uint32				uDeadTick;			// ������ʧʱ��(����)
	uint16				uLandAnimTick;		// ����ʱ��(����)
	uint16				uClimbAnimTick;		// ����ʱ��(����)
	float32				fFlyHeight;			// ���ո߶�
	float32				fModelRadius;		// ģ�Ͱ뾶
	uint32				uBeAttackMoveTick;	// �ܻ�λ��ʱ��ms
	uint16				uHatredType;		// �������
	uint32				uExp;				// ����
	uint8				uExpType;			// ����˥������
	uint64				uSuperArmor;		// ����ֵ
	uint32				uSuperArmorTime;	// ����ֵ�ָ�ʱ��
	uint32				uSuperArmorBuffID;	// ����
	uint16				uDropID;			// ����ID
	uint32				uActiveSkillID;		// �������� ʯ�������׺䶥
	uint32				uAIID;				// ai
	uint32				uDropDistribute;	//������� _emMonsterDeadDropDistribute
	uint32				uAttrIndex;			//��������  MonsterLevelAttr���е�ID

	_stMonsterModelBase&getMonsterModelBase()			{	return *this;					}
	void				initMonsterModelBase()			{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
typedef stl_vector<std::string>				VEC_AIFILE;
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stMonsterModel : _stMonsterModelBase
{
	stl_vector<_stAIOrdinalSkill>	vecSkills;
	stl_vector<_stDelayAddBuff>		vecFightAddBuff;// ����ս������buff
	stl_vector<_stDelayAddBuff>		vecBirthAddBuff;// ������ʱ����buff
	_stGameAttribute				stAttribute;
	std::string						strMoudleName;
	VEC_U32							vecHatredTask;
	VEC_U32							vecDartIDs;
	VEC_U32							vecQuestIDs;			// ֻ�н�ȡ���ض��������ܿ����Ĺ�
	SET_U32							setTeamTaskVisibleID;			// ֻ�н�ȡ������������ܿ����Ĺ� �淨ID

	stl_map<uint32,float32>			mapQuestDistribute;		//������� key:_emMonsterDeadQuestDistribute,value:���� 
	inline bool			check_mark	(uint8 _mark)const	{	return _CHECK_BIT(uMark,_BIT32(_mark));	}
	_stMonsterModel&	getMonsterModel()				{	return *this;							}
	void				initMonsterModel()
	{
		initMonsterModelBase();
		stAttribute.initGameAttribute();
		vecSkills.clear();
		vecFightAddBuff.clear();
		vecBirthAddBuff.clear();
		strMoudleName.clear();
		stAttribute.initGameAttribute();
		vecHatredTask.clear();
		vecDartIDs.clear();
		vecQuestIDs.clear();
		setTeamTaskVisibleID.clear();
		mapQuestDistribute.clear();
	}	
	inline bool		read			(CStreamBase& stream)
	{
		return _stMonsterModelBase::read(stream)&&
			stream.read(stAttribute)&&
			stream.read(vecSkills)&&
			stream.read(vecHatredTask)&&
			stream.read(vecFightAddBuff)&&
			stream.read(vecBirthAddBuff)&&
			stream.read(vecQuestIDs)&&
			stream.read(setTeamTaskVisibleID)&&
			stream.read(mapQuestDistribute)&&
			stream.read(vecDartIDs);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stMonsterModelBase::write(stream)&&
			stream.write(stAttribute)&&
			stream.write(vecSkills)&&
			stream.write(vecHatredTask)&&
			stream.write(vecFightAddBuff)&&
			stream.write(vecBirthAddBuff)&&
			stream.write(vecQuestIDs)&&
			stream.write(setTeamTaskVisibleID)&&
			stream.write(mapQuestDistribute)&&
			stream.write(vecDartIDs); 
	}
};
//-------------------------------------------------------------
//------------------------------ 
struct _stMonsterColletionBase
{
	uint32				uMonsterID;			// �ɼ���ID
	uint32				uType;				// �ɼ�������
	uint32				uTask;              // �Ƿ�������ɼ���				
	uint32				uSkillID;			// ����ID
	uint32				uCount;				// �ɲɼ�����
	uint32				uEvent;				// �ɼ����¼�
	uint32				Time;				// ˢ��ʱ��
	bool				bNeedDestory;		//�Ƿ�����

	_stMonsterColletionBase &getMonsterColletionBase()			{	return *this;					}
	void					initMonsterColletionBase()			{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT

};
//-------------------------------------------------------------
//------------------------------ 
struct _stMonsterColletionModel : _stMonsterColletionBase
{
	VEC_U32  vecEvent;
	VEC_U32  vecRefreshCampMonster;			//�ɼ���ˢ�� ͨ����ӪID
	_stMonsterColletionModel&	getMonsterColletionModel()			{	return *this;					}
	void						initMonsterColletionModel()
	{
		initMonsterColletionBase();
		vecRefreshCampMonster.clear();
		vecEvent.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stMonsterColletionBase::read(stream)&&
			stream.read(vecEvent)&&
			stream.read(vecRefreshCampMonster);
	}

	inline bool		write			(CStreamBase& stream)
	{
		return _stMonsterColletionBase::write(stream)&&
			stream.write(vecEvent)&&
			stream.write(vecRefreshCampMonster);
	}
};