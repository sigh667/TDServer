/*----------------- identityQuestModel.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/12/7 17:31:15
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
#include "basic/basicTypes.h"
#include "def_struct/stlDefine.h"
#include "def_struct/gameStruct.h"
#include "math/range.h"
#include "basic/systemFunction.h"
/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 
struct _stIdentityQuestBase
{
	uint32			uID;				//身份任务ID
	uint32			uCopymapMainType;	//副本主类型	_emCopymapMainType
	uint8			uCopymapDifficulty;	//副本难度 _emCopymapDifficulty
	_stRangeU32		stRangeTeamNum;		//队伍人数
	uint32			uQuestTime;			//任务时间
	uint32			uNpcID;				//npcid
	uint16			uNeedLeaderIdentity;//队长需要身份
	uint32			uMaxEnterCount;		//进入最大次数
	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stIdentityWenquPoemBase
{
	uint32			uID;				//诗句ID
	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stIdentityRewardZhenshenBase
{
	uint32			uID;				//
	uint32			uRealNameID;		//真身的名字ID
	uint32			uMonsterID;			//使用的怪物ID
	uint32			uContinueTime;		//多长时间打不死就重新分身
	uint32			uZhenShenSpeakID;	//真身的说话ID
	uint32			uZhenShenSkillID;	//真身的技能ID
	uint32			uZhenShenSkillTime;	//真身的技能使用时长(秒)
	_stPoint2F		stZhenShenOrginPos; //真身的初始坐标
	uint32			uJiaShenSpeakID;	//假身的说话ID
	uint32			uJiaShenSkillID;	//假身的技能ID
	uint32			uJiaShenSkillTime;	//假身的技能使用时长(秒)

	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stIdentityQuestModel : _stIdentityQuestBase
{
	DMAP_UINT8_32_32	dmapPreCondition;		//前提条件
	MAP_NEED_EXPEND		mapNeedExpend;			// 扣除的消耗
	DMAP_NEED_ITEM		mapNeedItem;			//需要特殊道具
	MAP_U16_U32			mapCopymapRate;			//缉拿权重比 key:副本id value权重:
	SET_U32				setRandNpc;				//随机NPC

	void initModel()	
	{ 
		initBase();
		dmapPreCondition.clear();
		mapNeedExpend.clear();
		mapNeedItem.clear();
		mapCopymapRate.clear();
		setRandNpc.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stIdentityQuestBase::read(stream)&&
			stream.read(dmapPreCondition)&&
			stream.read(mapNeedExpend)&&
			stream.read(mapNeedItem)&&
			stream.read(mapCopymapRate)&&
			stream.read(setRandNpc);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stIdentityQuestBase::write(stream)&&
			stream.write(dmapPreCondition)&&
			stream.write(mapNeedExpend)&&
			stream.write(mapNeedItem)&&
			stream.write(mapCopymapRate)&&
			stream.write(setRandNpc);
	}
	const uint16	getRandCopymapID	(const MAP_U16_U32& c_mapCopymapRate)const
	{
		MAP_U32_U16 newCopymap;
		uint32 uTotleRate = 0;
		for (auto _pos = mapCopymapRate.begin(); _pos != mapCopymapRate.end(); ++_pos )
		{
			uTotleRate += _pos->second;
			const uint32* pAddRate = c_mapCopymapRate.find_(_pos->first);
			if (pAddRate)
				uTotleRate += *pAddRate;
			newCopymap.insert_(uTotleRate,_pos->first);
		}
		
		uint32 uRandNum = randRange(uTotleRate);
		CONST_ITER_MAP_U32_U16 _spos = newCopymap.lower_bound(uRandNum); //第一个大于等于uLevel
		return _spos != newCopymap.end() ? _spos->second : 0;
	}
	const uint32	getRandNpcID	(uint32 uLastNpcID)const
	{
		if (!setRandNpc.size())
			return 0;
		SET_U32 setTmp = setRandNpc;
		setTmp.erase(uLastNpcID);
		if (setTmp.empty())
			return uLastNpcID;

		uint32 uRandIndex = randRange(uint32(setTmp.size() -1));
		ITER_SET_U32 _pos = setTmp.begin();
		advance(_pos,uRandIndex);
		return _pos != setTmp.end() ? *_pos : 0;
	}
};
//-------------------------------------------------------------
//------------------------------ 
struct _stIdentityWenquPoemModel : _stIdentityWenquPoemBase
{
	VEC_U32 vecWordID; //词组和

	void initModel()	
	{ 
		initBase();
		vecWordID.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stIdentityWenquPoemBase::read(stream)&&
			stream.read(vecWordID);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stIdentityWenquPoemBase::write(stream)&&
			stream.write(vecWordID);
	}
};
//-------------------------------------------------------------
//------------------------------ 
struct _stIdentityRewardZhenshenModel : _stIdentityRewardZhenshenBase
{
	VEC_U32						vecNameID;		//分身名字ID
	stl_vector<_stPoint2F>		vecPosition;	//分身的坐标

	void initModel()	
	{ 
		initBase();
		vecNameID.clear();
		vecPosition.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stIdentityRewardZhenshenBase::read(stream)&&
			stream.read(vecNameID)&&
			stream.read(vecPosition);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stIdentityRewardZhenshenBase::write(stream)&&
			stream.write(vecNameID)&&
			stream.write(vecPosition);
	}
};
