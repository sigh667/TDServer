/*----------------- questModel.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ������
* Version      : V1.0
* Date         : 2016/4/13 17:25:14
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "def_struct/gameStruct.h"
#include "def_struct/stlDefine.h"
#include "basic/memoryFunctions.h"
/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 
enum _emQuestMark {
	QuestMark_ClientComplete			,//ǰ�˿����������
	QuestMark_AutoReceive				,//�Զ�����

	QuestMark_Max,
};
//-------------------------------------------------------------
//------------------------------����Ŀ����ɺ��¼�
struct _stQuestTargetAction
{
	uint8	uType;						// _emQuestTargetAction

	union 
	{// uType ��һ������һ��

		struct 
		{//QuestTargetAction_AddMonster
			uint32	uMonsterID;
			uint32	uCount;
			float32	fPosX;
			float32	fPosY;
			float32	fDir;
			float32	fRadius;
			uint32	uRouteID;
		}	stAddMonster;

		struct 
		{//QuestTargetAction_RemoveMonster
			uint32	uMonsterID;
		}	stRemoveMonster;

		struct 
		{//QuestTargetAction_ChangeCamp
			uint32	uMonsterID;
			uint8	uCampID;
		}	stChangeCamp;

		struct 
		{//QuestTargetAction_ChangeScene
			uint16	uSceneID;
			uint16	uRegionID;
		}	stChangeScene;	

		struct 
		{//QuestTargetAction_AddSceneArea
			uint32	uAreaID;
		}	stAddSceneArea;

		struct 
		{//QuestTargetAction_DelSceneArea
			uint32	uAreaID;
		}	stDelSceneArea;
	};

	_stQuestTargetAction()
	{
		init();
	}
	inline void			init()			{	dMemset(this,0,sizeof(*this));			}

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------����Ŀ������
struct _stQuestTargetBase 
{
	uint32	uID;
	uint8	uType;						// _emQuestTargetType
	uint32	uSubType;					// ������
	uint32	uCount;						// ��������
	uint16	uSceneID;					// ��������
	uint32	uParam1;					// �������

	_stQuestTargetBase()
	{
		initBase();
	}
	inline void			initBase()			{	dMemset(this,0,sizeof(*this));			}

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------����ģ��
struct _stQuestModelBase
{
	uint32	uID;						// ����ID
	uint32	uPreTaskID;					// ǰ������ID
	uint32	uType;						// ��������_emQuestType
	uint32	uMark;						// ����(_emQuestMark)
	bool	bSerial;					// ����Ŀ���ϵ��true-����,false-����
	uint32	uPlaneID;					// λ������� λ��ID
	uint32	uNextPlaneTaskID;			// λ������� ����λ������ID

	_stQuestModelBase&	getBase()		{	return *this;							}
	inline void			initBase()		{	dMemset(this,0,sizeof(*this));			}
	READ_WRITE_STRUCT
};
#pragma pack(pop)
//-------------------------------------------------------------
//------------------------------����ģ��
struct _stQuestTarget : _stQuestTargetBase
{
	stl_vector<_stQuestTargetAction> vecStartAction;		// Ŀ�꿪ʼ�¼�
	stl_vector<_stQuestTargetAction> vecCompleteAction;		// Ŀ����ɺ��¼�

	_stQuestTarget()
	{
		init();
	}
	inline void			init()			
	{	
		initBase();
		vecCompleteAction.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stQuestTargetBase::read(stream)&&
			stream.read(vecStartAction)&&
			stream.read(vecCompleteAction);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stQuestTargetBase::write(stream)&&
			stream.write(vecStartAction)&&
			stream.write(vecCompleteAction);
	}
};
//-------------------------------------------------------------
//------------------------------����ģ��
struct _stQuestModel : _stQuestModelBase
{
	DMAP_UINT8_32_32					mapPrecondition;	//ǰ������
	stl_vector<uint32>					vecTargetID;		//����Ŀ��ID
	stl_map<uint32,uint32>				mapGiveItem;		//������ȡ�������Ʒ
	stl_map<uint32,uint32>				mapDeductItem;		//���������۳���Ʒ

	inline bool		check_mark	(uint8 _mark)	const		{	return _CHECK_BIT(uMark,_BIT32(_mark));	}
	inline void		set_mark	(uint8 _mark,bool bSet)		{	_SET_BIT32(uMark,_mark,bSet);			}

	_stQuestModel()											{	initQuestModel();						}
	_stQuestModel&	getQuestModel()							{	return *this;							}
	void			initQuestModel()
	{
		initBase();
		mapPrecondition.clear();
		vecTargetID.clear();
		mapGiveItem.clear();
		mapDeductItem.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stQuestModelBase::read(stream)&&
			stream.read(mapPrecondition)&&
			stream.read(vecTargetID);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stQuestModelBase::write(stream)&&
			stream.write(mapPrecondition)&&
			stream.write(vecTargetID);
	}
};
