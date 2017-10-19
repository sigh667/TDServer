/*------------- sceneItem.h
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈腾
* Version      : V1.01
* Date         : 2016/03/09
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "sceneUnit.h"
#include "cfg_data/dropData/dropData.h"
#include "def_struct/teamStruct.h"
#include "protocol/CASMsg.pb.h"
#include "stl/std_map.h"
#include "stl/stl_vector.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
// 掉落系统 道具信息
struct _stDropItemDropCell
{
	uint16	uIndex;					;	// 索引
	uint32	uItemID					;	// 道具ID
	uint16	uItemNum				;	// 道具数量
	uint32	uOwnerID				;	// 获得者信息
	bool	bBind					;	// 是否绑定
	stl_map<uint32,uint32>	mapDice	;	// 成员ID ==> 掷出的数

	_stDropItemDropCell()
	{
		init();
	}
	void	init()
	{
		uIndex		= 0;
		uItemID		= 0;
		uItemNum	= 0;
		uOwnerID	= 0;
		bBind		= false;
		mapDice.clear();
	}
	void		loadFromProtobuf(const PBDropItem_DropCell&pbInfo);
	void		saveToProtobuf	 (PBDropItem_DropCell&pbInfo);
};
//-------------------------------------------------------------
//------------------------------
class PBC2DropItem_Info;
class CSceneItem	: public CSceneUnit
{
private:
	typedef CSceneUnit Parent;
	friend	class CPlayerDropItem;
private:
	bool							m_bOpen;
	uint16							m_uDropID;			//掉落ID
	uint32							m_uDeadID;			//死亡对象ID
	uint32							m_uOwnerPlayerID;	//所有者
	uint64							m_uBirthTime;		//出生时间
	uint32							m_uCurAllotType;	//当前分配模式 m_uCurAllotType
	uint32							m_uAllotTypeEndTime;//当前分配模式结束时间点
	uint32							m_uExistTime;		//消失时间
	_stTeamInfo						m_stTeamInfo;		//队伍信息
	stl_vector<_stDropItemDropCell>	m_vecItem;			//道具信息
	bool							m_bCashPlayer;		//是否是赏金团					
public:
	CSceneItem();
public:
	virtual bool	initialize		();
	virtual uint32	getID			();
	virtual CSceneItem*	asSceneItem	()	{	return this;		}

public:
	virtual	bool	packetGhost		(uint64 uTime,bool bSelf,uint32& updateMark,PBGhostInfo&pbInfo);
	/*--->[ 处理 ]*/
	virtual void	processLogic	(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay);
public:
	//--- 构建
	bool	build					(CGameSceneCell* pCell,uint32 uDeadID, uint16 uDropID,const VEC_DROP_CELL&vecDropGroup, CScenePlayer* pOwner);

	bool	autoDiceAward			(_stDropItemDropCell& cell);

	bool	diceItem				(_stDropItemDropCell& cell,_stTeamMember* pMember,bool bAbandon,bool bSynDice = true);
	bool	awardItem				(_stDropItemDropCell& cell,_stPlayerDisplay* pDisplay);
	bool	awardItemBySendMail		(CScenePlayer * pPlayer, uint32 uLogicWorldID);
	bool	isOpen					();
	bool	open					();

	inline  bool  isCashPlayer		()	{	return m_bCashPlayer;	}

	bool	isItemOwner				(uint32 uPlayerID);
	bool	haveItem				();

	void	packPB					(PBC2DropItem_Info& ack);
public:
	void			broadCastMsg	(uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent = 0);
};
