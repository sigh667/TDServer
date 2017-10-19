/*------------- coolingData.cpp
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈腾
* Version      : V1.01
* Date         : 2016/01/23
*
*/ 
/***************************************************************
* 
***************************************************************/
#include "gameLogic/cooling/cooling.h"
#include "protocol/GameDefine.pb.h"
#include "protocol/CASProtocol.pb.h"
#include "protocol/CASMsg.pb.h"
#include "basic/timeFunction.h"
#include "gameLogic/sceneObject/scenePlayer.h"
//-------------------------------------------------------------
//------------------------------ 
CCooling::CCooling()
{

}

//-------------------------------------------------------------
//------------------------------ 
CCooling::~CCooling()
{

}

//-------------------------------------------------------------
//------------------------------
void	CCooling::initialize	(CSceneUnit* pOwner)
{
	m_mapCooling.clear();
	m_pOwner = pOwner;
}
//-------------------------------------------------------------
//------------------------------ 增加冷却
void CCooling::addCooling(_emCoolingType uType, uint32 uCoolingID, uint32 uCoolingMS, uint32 uGroupID, uint32 uGroupCoolingMS/* = 0*/,bool bSend/* = true*/)
{
	uint64	uCurTick		= getMillisecond();
	uint32	uTime			= getTime();

	bool bTmpSend = false;
	PBG2CUpCoolTime ack;
	if (uCoolingMS)
	{
		uint64 uKey				= MAKE_UINT64(uType,uCoolingID);

		_stCoolingInfo& stInfo	= m_mapCooling[uKey];
		//自身冷却
		stInfo.eType			= uType;
		stInfo.uID				= uCoolingID;
		stInfo.uBeginTick		= uCurTick;
		stInfo.uEndTick			= uCurTick + uCoolingMS;
		stInfo.uBeginTime		= uTime;
		
		stInfo.saveToProtobuf(ack.mutable_coolinfo());
		bTmpSend = true;
	}

	//分组冷却
	if (uGroupID && uGroupCoolingMS )
	{
		uint64 uKey				= MAKE_UINT64(CoolingType_Group,uGroupID);
		_stCoolingInfo& stInfo	= m_mapCooling[uKey];
		stInfo.eType		= CoolingType_Group;
		stInfo.uID			= uGroupID;
		stInfo.uBeginTick	= uCurTick;
		stInfo.uEndTick		= uCurTick + uGroupCoolingMS;
		stInfo.uBeginTime	= uTime;

		stInfo.saveToProtobuf(ack.mutable_coolgroupinfo());
		bTmpSend = true;
	}

	if (bSend && bTmpSend && m_pOwner->asPlayer())
	{
		m_pOwner->asPlayer()->sendMsg(P_S2C_Protocol_Skill,S2C_Skill_UpCoolTime,&ack);
	}
}
//-------------------------------------------------------------
//------------------------------ 清除冷却
void CCooling::clearCooling(_emCoolingType uType, uint32 uCoolingID)
{
	uint64 uKey				= MAKE_UINT64(uType,uCoolingID);
	m_mapCooling.erase(uKey);
}

//-------------------------------------------------------------
//------------------------------ 是否冷却中
bool CCooling::isCooling(_emCoolingType uType, uint32 uCoolingID, uint32 uGroupID)
{
	uint64 uCurTick			= getMillisecond();

	uint64 uKey				= MAKE_UINT64(uType,uCoolingID);
	MAP_COOLING::iterator _pos = m_mapCooling.find(uKey);
	//自身是否冷却中
	if (  _pos != m_mapCooling.end() && uCurTick <= _pos->second.uEndTick )
		return true;

	//分组是否冷却中
	if (uGroupID != 0)
	{
		uKey	= MAKE_UINT64(CoolingType_Group,uGroupID);
		_pos	= m_mapCooling.find(uKey);
		if (  _pos != m_mapCooling.end() && uCurTick <= _pos->second.uEndTick )
			return true;
	}
	return false;
}
//-------------------------------------------------------------
//------------------------------ 是否冷却中
bool CCooling::isCoolGroup(_emCoolingType uType, uint32 uCoolingID, uint32 uGroupID)
{
	if (!uGroupID)
		return false;

	uint64 uCurTick			= getMillisecond();

	//分组是否冷却中
	uint64 uKey	= MAKE_UINT64(CoolingType_Group,uGroupID);
	MAP_COOLING::iterator _pos	= m_mapCooling.find(uKey);
	if (  _pos != m_mapCooling.end() && uCurTick <= _pos->second.uEndTick )
		return true;

	return false;
}
//-------------------------------------------------------------
//------------------------------获取冷却还剩时间
bool CCooling::isCooling(_emCoolingType uType, uint32 uCoolingID,uint32 uGroupID, uint64 & uEndTick)
{
	uint64 uCurTick			= getMillisecond();
	uint64 uKey					= MAKE_UINT64(uType,uCoolingID);
	MAP_COOLING::iterator _pos	= m_mapCooling.find(uKey);
	
	//自身是否冷却中
	if (  _pos != m_mapCooling.end() && uCurTick <= _pos->second.uEndTick)
	{
		uEndTick = _pos->second.uEndTick;
		return true;
	}
	//分组是否冷却中
	if (uGroupID != 0)
	{
		uKey	= MAKE_UINT64(CoolingType_Group,uGroupID);
		_pos	= m_mapCooling.find(uKey);
		if (  _pos != m_mapCooling.end() && uCurTick <= _pos->second.uEndTick )
		{
			uEndTick = _pos->second.uEndTick;
			return true;
		}
	}
	return false;
}

//-------------------------------------------------------------
//------------------------------ 
bool CCooling::loadData(const PBCooling& data)
{
	m_mapCooling.clear();
	uint64		uCurTick	= getMillisecond();
	uint32		uCurTime	= getTime();
	for (int32 i = 0; i < data.cooling_size(); ++i)
	{
		const PBCoolingInfo& info = data.cooling(i);

		uint64 uPassMS			= (uint64)(uCurTime - info.time())*1000;
		uint64 uCoolingMS		= (uint64)(info.end() - info.begin());
		if ( uPassMS  >= uCoolingMS )
			continue;

		uint64 uKey				= MAKE_UINT64(info.type(),info.id());
		_stCoolingInfo& stInfo	= m_mapCooling[uKey];
		stInfo.eType			= (uint16)info.type();
		stInfo.uID				= info.id();
		stInfo.uBeginTick		= uCurTick;
		stInfo.uEndTick			= uCurTick + ( uCoolingMS - uPassMS );
		stInfo.uBeginTime		= uCurTime;
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool CCooling::saveData(PBCooling& data)
{
	uint64 uCurTick = getMillisecond();
	MAP_COOLING::iterator _pos	= m_mapCooling.begin();
	MAP_COOLING::iterator _end	= m_mapCooling.end();
	for (; _pos != _end; ++_pos)
	{
		_stCoolingInfo& info = _pos->second;
		if ( uCurTick >= info.uEndTick )
			continue;

		PBCoolingInfo* pbInfo	= data.add_cooling();
		if (pbInfo)
		{
			pbInfo->set_type((_emCoolingType)info.eType);
			pbInfo->set_id(info.uID);
			pbInfo->set_begin(info.uBeginTick);
			pbInfo->set_end(info.uEndTick);
			pbInfo->set_time(info.uBeginTime);
		}
	}
	return true;
}
