/*------------- coolingData.h
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈腾
* Version      : V1.01
* Date         : 2016/01/23
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "basic/basicTypes.h"
#include "stl/std_map.h"
#include "def_struct/playerStuct.h"
#define  SKILL_GOOLING_GROUP_ID 31415		//技能的公共ID组ID
class CSceneUnit;
class PBCooling;
//-------------------------------------------------------------
//------------------------------
class CCooling
{
protected:
	typedef			stl_map<uint64, _stCoolingInfo>	MAP_COOLING;
	
	MAP_COOLING		m_mapCooling;
	CSceneUnit*		m_pOwner;

public:
	CCooling();
	virtual ~CCooling();
	
public:
	void			initialize	(CSceneUnit* pOwner);
	void			addCooling	(_emCoolingType uType, uint32 uCoolingID, uint32 uCoolingMS, uint32 uGroupID = 0, uint32 uGroupCoolingMS = 0,bool bSend = true);
	void			clearCooling(_emCoolingType uType, uint32 uCoolingID);
	bool			isCooling	(_emCoolingType uType, uint32 uCoolingID, uint32 uGroupID = 0);
	bool			isCooling	(_emCoolingType uType, uint32 uCoolingID, uint32 uGroupID, uint64 & uEndTick);
	bool			isCoolGroup	(_emCoolingType uType, uint32 uCoolingID, uint32 uGroupID = 0);
	bool			loadData	(const PBCooling& data);
	bool			saveData	(PBCooling& data);
};
