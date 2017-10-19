/*------------- factionCommon.h
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 林驹
* Version      : V1.01
* Date         : 2016/10/17
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include <set>
#include "basic/basicTypes.h"
#include "singleton/singleton.h"
#include "protocol/CASProtocol.pb.h"
#include "protocol/GameDefine.pb.h"
#include "cfg_data/factionData/factionData.h"
/*************************************************************/
class CFactionCommon
{
public:
	// 判断某个操作是否需要在帮会中才能进行
	static bool	needGuild(uint8 uProtocol)	
	{
		return 	(	C2S_Faction_Create != uProtocol	&&
					C2S_Faction_Apply != uProtocol	&&
					C2S_Faction_Syn != uProtocol	&&
					C2S_Faction_Applying != uProtocol	&&
					C2S_Faction_AgreeInviteJoin != uProtocol	&&
					C2S_Faction_RefuseInviteJoin != uProtocol	&&
					C2S_Faction_List != uProtocol);
	}
};