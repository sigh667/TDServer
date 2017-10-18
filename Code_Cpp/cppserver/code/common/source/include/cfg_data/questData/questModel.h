/*----------------- questModel.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 晁旭升
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
	QuestMark_ClientComplete			,//前端可以主动完成
	QuestMark_AutoReceive				,//自动接收

	QuestMark_Max,
};
//-------------------------------------------------------------
//------------------------------任务目标完成后事件
struct _stQuestTargetAction
{
	uint8	uType;						// _emQuestTargetAction

	union 
	{// uType 不一样而不一样

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
//------------------------------任务目标数据
struct _stQuestTargetBase 
{
	uint32	uID;
	uint8	uType;						// _emQuestTargetType
	uint32	uSubType;					// 子类型
	uint32	uCount;						// 所需数量
	uint16	uSceneID;					// 场景限制
	uint32	uParam1;					// 额外参数

	_stQuestTargetBase()
	{
		initBase();
	}
	inline void			initBase()			{	dMemset(this,0,sizeof(*this));			}

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------任务模板
struct _stQuestModelBase
{
	uint32	uID;						// 任务ID
	uint32	uPreTaskID;					// 前置任务ID
	uint32	uType;						// 任务类型_emQuestType
	uint32	uMark;						// 掩码(_emQuestMark)
	bool	bSerial;					// 任务目标关系：true-串行,false-并行
	uint32	uPlaneID;					// 位面任务的 位面ID
	uint32	uNextPlaneTaskID;			// 位面任务的 后续位面任务ID

	_stQuestModelBase&	getBase()		{	return *this;							}
	inline void			initBase()		{	dMemset(this,0,sizeof(*this));			}
	READ_WRITE_STRUCT
};
#pragma pack(pop)
//-------------------------------------------------------------
//------------------------------任务模板
struct _stQuestTarget : _stQuestTargetBase
{
	stl_vector<_stQuestTargetAction> vecStartAction;		// 目标开始事件
	stl_vector<_stQuestTargetAction> vecCompleteAction;		// 目标完成后事件

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
//------------------------------任务模板
struct _stQuestModel : _stQuestModelBase
{
	DMAP_UINT8_32_32					mapPrecondition;	//前置条件
	stl_vector<uint32>					vecTargetID;		//任务目标ID
	stl_map<uint32,uint32>				mapGiveItem;		//任务领取后给予物品
	stl_map<uint32,uint32>				mapDeductItem;		//任务结束后扣除物品

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
