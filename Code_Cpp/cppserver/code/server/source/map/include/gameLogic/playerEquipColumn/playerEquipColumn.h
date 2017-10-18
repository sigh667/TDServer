/*------------- playerEquipColumn.h
 * Copyright (C): 2016  Mokylin·Mokyqi
 * Author       : 方声辉
 * Version      : V1.01
 * Date         : 2017/04/06
 * 
 */
/***************************************************************
* 
***************************************************************/
#pragma once
#include "gameLogic/sceneObject/sceneUnit.h"
#include "gameLogic/gamePlayerFun/gamePlayerFun.h"
#include "protocol/PlayerStruct.pb.h"
#include "basic/memoryFunctions.h"

/*************************************************************/
struct _stEqiupColumn
{
	uint8	uID;				// 装备栏的位置
	uint16	uEnhanceLevel;		// 强化等级		
	uint16	uForgeLevel;		// 打磨等级

	_stEqiupColumn		()	
	{
		dMemset(this,0,sizeof(*this));
	}
};

class CPlayerEquipColumn	: public CGamePlayerFun
{

private:
	typedef stl_map<uint8,_stEqiupColumn>		MAP_EQIUPCONLUMN_INFO;// <装备栏位,装备栏信息>
	MAP_EQIUPCONLUMN_INFO						m_mapEqiupColumn;
	_stGameAttribute*							m_pGameAttr;		  // 属性

public:
	CPlayerEquipColumn(CScenePlayer& clPlayer);
	virtual~CPlayerEquipColumn();

public:
	virtual	void	initialize				();
	virtual	bool	loadFromProtobuf		(const PBPlayerData&pbPlayer);
	virtual	bool	saveToProtobuf			(PBPlayerData&pbPlayer);
	virtual void	newOnline				();
	virtual	_emGameSystemType	getFunType	()const					{	return GameSystemType_EqiupColumn ; }

public:
	bool	handle_EquipColumnEnhance_Ask		(uint8 uEventID, CNetStream& clStream);
	bool	handle_EquipColumnForge_Ask			(uint8 uEventID, CNetStream& clStream);
	bool	send_EquipColumnEnhanceAck			(uint8 uID,uint8 uCode = 0);
	bool	send_EquipColumnForgeAck			(uint8 uID,uint8 uCode = 0);
	
	bool				equipColumnEnhance(uint8 uEquipType,stl_map<uint64, _stExpendItem> &mapExpend,uint8& uCode);
	bool				equipColumnForge(uint8 uEquipType,stl_map<uint64, _stExpendItem> &mapExpend,uint8& uCode);

	uint32	getForgeLevel(uint8 uEquipType)	;
private:

	_stEqiupColumn*		find(uint8 uID)					{ return m_mapEqiupColumn.find_(uID);	}
	bool				calculateAttri();


};


/*************************************************************/