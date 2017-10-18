/*----------------- identityQuestModel.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����
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
	uint32			uID;				//�������ID
	uint32			uCopymapMainType;	//����������	_emCopymapMainType
	uint8			uCopymapDifficulty;	//�����Ѷ� _emCopymapDifficulty
	_stRangeU32		stRangeTeamNum;		//��������
	uint32			uQuestTime;			//����ʱ��
	uint32			uNpcID;				//npcid
	uint16			uNeedLeaderIdentity;//�ӳ���Ҫ���
	uint32			uMaxEnterCount;		//����������
	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stIdentityWenquPoemBase
{
	uint32			uID;				//ʫ��ID
	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stIdentityRewardZhenshenBase
{
	uint32			uID;				//
	uint32			uRealNameID;		//���������ID
	uint32			uMonsterID;			//ʹ�õĹ���ID
	uint32			uContinueTime;		//�೤ʱ����������·���
	uint32			uZhenShenSpeakID;	//�����˵��ID
	uint32			uZhenShenSkillID;	//����ļ���ID
	uint32			uZhenShenSkillTime;	//����ļ���ʹ��ʱ��(��)
	_stPoint2F		stZhenShenOrginPos; //����ĳ�ʼ����
	uint32			uJiaShenSpeakID;	//�����˵��ID
	uint32			uJiaShenSkillID;	//����ļ���ID
	uint32			uJiaShenSkillTime;	//����ļ���ʹ��ʱ��(��)

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
	DMAP_UINT8_32_32	dmapPreCondition;		//ǰ������
	MAP_NEED_EXPEND		mapNeedExpend;			// �۳�������
	DMAP_NEED_ITEM		mapNeedItem;			//��Ҫ�������
	MAP_U16_U32			mapCopymapRate;			//����Ȩ�ر� key:����id valueȨ��:
	SET_U32				setRandNpc;				//���NPC

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
		CONST_ITER_MAP_U32_U16 _spos = newCopymap.lower_bound(uRandNum); //��һ�����ڵ���uLevel
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
	VEC_U32 vecWordID; //�����

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
	VEC_U32						vecNameID;		//��������ID
	stl_vector<_stPoint2F>		vecPosition;	//���������

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
