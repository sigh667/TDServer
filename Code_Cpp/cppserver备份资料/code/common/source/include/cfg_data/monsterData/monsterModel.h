/*----------------- monsterModel.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 赵文源
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
	uint32				uDelayTick;			// 间隔毫秒
	uint32				uBuffID;			// buffID

	_stDelayAddBuff	()		{	init();							}
	void	init	()		{	memset(this,0,sizeof(*this));	}

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stAIOrdinalSkill
{
	uint32				uSkillID;			// 技能ID
	uint32				uPriority;			// 优先级

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
	uint8				uType;				// 类型 _emMonsterType
	uint8				uSubType;			// 子类型 _emMonsterSubType
	uint8				uCampID;			// 阵营
	uint32				uMark;				// 掩码	_emMonsterMark
	float32				fCallDist;			// 战斗呼叫距离
	uint32				uBirthTick;			// 诞生时间(毫秒)
	uint32				uDeadTick;			// 死亡消失时间(毫秒)
	uint16				uLandAnimTick;		// 倒地时长(毫秒)
	uint16				uClimbAnimTick;		// 爬起时长(毫秒)
	float32				fFlyHeight;			// 浮空高度
	float32				fModelRadius;		// 模型半径
	uint32				uBeAttackMoveTick;	// 受击位移时间ms
	uint16				uHatredType;		// 仇恨类型
	uint32				uExp;				// 经验
	uint8				uExpType;			// 经验衰减类型
	uint64				uSuperArmor;		// 霸体值
	uint32				uSuperArmorTime;	// 霸体值恢复时间
	uint32				uSuperArmorBuffID;	// 霸体
	uint16				uDropID;			// 掉落ID
	uint32				uActiveSkillID;		// 主动技能 石碑的五雷轰顶
	uint32				uAIID;				// ai
	uint32				uDropDistribute;	//掉落归属 _emMonsterDeadDropDistribute
	uint32				uAttrIndex;			//属性索引  MonsterLevelAttr表中的ID

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
	stl_vector<_stDelayAddBuff>		vecFightAddBuff;// 脱离战斗增加buff
	stl_vector<_stDelayAddBuff>		vecBirthAddBuff;// 出生延时增加buff
	_stGameAttribute				stAttribute;
	std::string						strMoudleName;
	VEC_U32							vecHatredTask;
	VEC_U32							vecDartIDs;
	VEC_U32							vecQuestIDs;			// 只有接取了特定任务后才能看到的怪
	SET_U32							setTeamTaskVisibleID;			// 只有接取了组队任务后才能看到的怪 玩法ID

	stl_map<uint32,float32>			mapQuestDistribute;		//任务归属 key:_emMonsterDeadQuestDistribute,value:参数 
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
	uint32				uMonsterID;			// 采集物ID
	uint32				uType;				// 采集物类型
	uint32				uTask;              // 是否是任务采集怪				
	uint32				uSkillID;			// 技能ID
	uint32				uCount;				// 可采集次数
	uint32				uEvent;				// 采集后事件
	uint32				Time;				// 刷新时间
	bool				bNeedDestory;		//是否销毁

	_stMonsterColletionBase &getMonsterColletionBase()			{	return *this;					}
	void					initMonsterColletionBase()			{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT

};
//-------------------------------------------------------------
//------------------------------ 
struct _stMonsterColletionModel : _stMonsterColletionBase
{
	VEC_U32  vecEvent;
	VEC_U32  vecRefreshCampMonster;			//采集完刷怪 通过阵营ID
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