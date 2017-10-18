/*------------- sceneItem.cpp
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈腾
* Version      : V1.01
* Date         : 2016/03/09
*
*/ 
/***************************************************************
* 
***************************************************************/
#include "gameLogic/sceneObject/sceneItem.h"
#include "protocol/GhostStruct.pb.h"
#include "gameScene/gameSceneCell.h"
#include "cfg_data/itemData/itemData.h"
#include "protocol/MsgResult.pb.h"
#include <algorithm>
#include "gameLogin/gamePlayerMgr.h"
#include "cfg_data/constantData/constantData.h"
#include "log/log.h"
//#include "gameLogic/playerDropItem/playerDropItem.h"
#include "gameScene/copymapBase.h"
#include "gameLogic/gameMail/gameMail.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
void		_stDropItemDropCell::loadFromProtobuf(const PBDropItem_DropCell&pbInfo)
{
	uIndex		= (uint16)pbInfo.itemidx();
	uItemID		= (uint32)pbInfo.itemid();
	uItemNum	= (uint16)pbInfo.itemnum();
	uOwnerID	= pbInfo.ownerid();
	bBind		= pbInfo.bind();
	for ( uint32 i = 0 ; i < (uint32)pbInfo.dice_size() ; ++i )
	{
		const PBDropItem_DiceInfo& pBDice = pbInfo.dice(i);
		mapDice[pBDice.memberid()] = pBDice.number();
	}
}
//-------------------------------------------------------------
//------------------------------
void		_stDropItemDropCell::saveToProtobuf	 (PBDropItem_DropCell&pbInfo)
{
	pbInfo.set_itemidx(uIndex);
	pbInfo.set_itemid(uItemID);
	pbInfo.set_itemnum(uItemNum);
	pbInfo.set_ownerid(uOwnerID);
	pbInfo.set_bind(bBind);

	stl_map<uint32,uint32>::iterator _pos = mapDice.begin();
	stl_map<uint32,uint32>::iterator _end = mapDice.end();
	for ( ;_pos != _end ; ++_pos )
	{
		PBDropItem_DiceInfo* pBDice = pbInfo.add_dice();
		if ( pBDice )
		{
			pBDice->set_memberid(_pos->first);
			pBDice->set_number(_pos->second);
		}
	}
}
//-------------------------------------------------------------
//------------------------------ 
CSceneItem::CSceneItem()
{
	m_stUnitID.uUnitType	= GameObject_Item;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CSceneItem::initialize()
{
	if(!Parent::initialize())
		return false;

	m_uExistTime		= 0;
	m_bOpen				= false;
	m_uDropID			= 0;
	m_uDeadID			= 0;
	m_uOwnerPlayerID	= 0;
	m_uBirthTime		= 0;
	m_stTeamInfo.init();
	m_vecItem.clear();
	m_uCurAllotType		= AllotType_None;
	m_uAllotTypeEndTime	= 0;
	m_bCashPlayer		= false;

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
uint32	CSceneItem::getID()
{
	return m_uDropID;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneItem::packetGhost		(uint64 uTime,bool bSelf,uint32& updateMark,PBGhostInfo&pbInfo)
{
	if(_CHECK_BIT(updateMark,_BIT64(Ghost_Remove)))
		return true;

	bool bSend = Parent::packetGhost(uTime,bSelf,updateMark,pbInfo);
	PBGhostItem& pbItem= *pbInfo.mutable_item();

	//基本信息
	if(_CHECK_BIT(updateMark,_BIT64(Ghost_Base)))
	{
		bSend = true;
		pbItem.set_dropid(getID());
		pbItem.set_deadid(m_uDeadID);
		pbItem.set_ownerid(m_uOwnerPlayerID);
		pbItem.set_birthtime(m_uBirthTime);
	}
	_SET_BIT64(updateMark,Ghost_Base,false);

	return bSend;
}
//-------------------------------------------------------------
//------------------------------
bool	CSceneItem::haveItem				()
{
	bool bHaveItem = false;
	stl_vector<_stDropItemDropCell>::iterator _pos = m_vecItem.begin();
	stl_vector<_stDropItemDropCell>::iterator _end = m_vecItem.end();
	for ( ; _pos != _end ; ++_pos )
	{
		if (!_pos->uOwnerID)
		{
			bHaveItem = true;
			break;
		}
	}

	return bHaveItem;
}
//-------------------------------------------------------------
//------------------------------
void	CSceneItem::packPB					(PBC2DropItem_Info& ack)
{
	ack.set_dropunitid(getUnitID());
	ack.set_allottype(m_uCurAllotType);
	ack.set_endtime(m_uAllotTypeEndTime);
	PBTeamInfo* pbTeam = ack.mutable_team();
	if ( pbTeam)
		m_stTeamInfo.saveToProtobuf(*pbTeam);

	stl_vector<_stDropItemDropCell>::iterator _pos = m_vecItem.begin();
	stl_vector<_stDropItemDropCell>::iterator _end = m_vecItem.end();
	for ( ; _pos != _end ; ++_pos )
	{
		PBDropItem_DropCell* pbDropCell = ack.add_cell();
		if (pbDropCell)
			_pos->saveToProtobuf(*pbDropCell);
	}
}
//-------------------------------------------------------------
//------------------------------ 处理
void	CSceneItem::processLogic(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay)
{
	if(!uSecond)
		return;

	Parent::processLogic(uTick,uTime,uSecond,bNewDay);

	if( m_uExistTime && uTime >= m_uExistTime)
	{
		
		if (isCashPlayer())
		{
			CScenePlayer* pPlayer = m_pSceneCell->findPlayer(m_uOwnerPlayerID);
			awardItemBySendMail(pPlayer, pPlayer->getLogicWorldID());
		}
		
		if (m_pSceneCell)
			m_pSceneCell->leaveScene(this);
		return ;
	}
	
	if ( !haveItem() )
	{
		if (m_pSceneCell)
			m_pSceneCell->leaveScene(this);
		return ;
	}

	if( !m_uAllotTypeEndTime || uTime < m_uAllotTypeEndTime)
		return ;

	switch (m_uCurAllotType)
	{
	case AllotType_Dice:
		{
			stl_vector<_stDropItemDropCell>::iterator _pos = m_vecItem.begin();
			stl_vector<_stDropItemDropCell>::iterator _end = m_vecItem.end();
			for ( ; _pos != _end ; ++_pos )
			{
				autoDiceAward(*_pos);
			}
			if (m_pSceneCell)
				m_pSceneCell->leaveScene(this);
		}
		break;
	case AllotType_Assign:
		{
			bool bHaveItem = haveItem();
			if (bHaveItem)
			{
				m_uCurAllotType		= AllotType_Dice;
				m_uAllotTypeEndTime	= (uint32)uTime + g_clConstantData.DropItem_Dice_Time;

				PBC2DropItem_ChangeAllot ack;
				ack.set_dropunitid(getUnitID());
				ack.set_allottype(m_uCurAllotType);
				ack.set_endtime(m_uAllotTypeEndTime);

				broadCastMsg(P_S2C_Protocol_DropItem,S2C_DropItem_ChangeAllot,&ack);
			}
			else
			{
				if (m_pSceneCell)
					m_pSceneCell->leaveScene(this);
			}
		}
		break;
	default:
		{
			if (m_pSceneCell)
				m_pSceneCell->leaveScene(this);
		}
		break;
	}
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneItem::build	(CGameSceneCell* pCell,uint32 uDeadID, uint16 uDropID,const VEC_DROP_CELL&vecDropGroup, CScenePlayer* pOwner)
{
	if ( !pCell )
		return false;

	m_uExistTime		= getTime() + g_clConstantData.DropItem_Exist_Time;

	m_uDropID			= uDropID;
	m_uDeadID			= uDeadID;
	m_uBirthTime		= getSystemTick64();

	if (pCell->getGameCopymap()&&pCell->getGameCopymap()->getCashPlayerID())
	{
		m_uOwnerPlayerID = pCell->getGameCopymap()->getCashPlayerID();
		pCell->getGameCopymap()->AddSceneItem(this);
		m_bCashPlayer	 = true;
	}
	else
	{
		if ( pOwner )
		{
			m_uOwnerPlayerID= pOwner->getPlayerID();
			_stTeamInfo* pTeam = pOwner->getTeam();
			if ( pTeam )
			{
				m_uCurAllotType		= pTeam->uAllotType;
				m_stTeamInfo		= *pTeam;
				stl_map<uint32,_stTeamMember>::iterator _pos = pTeam->mapMember.begin();
				while ( _pos != pTeam->mapMember.end() )
				{
					stl_map<uint32,_stTeamMember>::iterator _itr = _pos++;
					CScenePlayer* pPlayer = pCell->findPlayer(_itr->second.stAvatar.stDisplay.uPlayerID);
					if ( !pPlayer )
						pTeam->mapMember.erase_(_itr);
				}

				if (pTeam->mapMember.size() <= 1 )
				{
					m_uCurAllotType		= AllotType_None;
					m_stTeamInfo.init();
				}
				else
				{
					if ( !pCell->findPlayer(m_stTeamInfo.uLeaderID) )
						m_uCurAllotType		= AllotType_Dice;
				}
			}
		}
	}
	if ( !vecDropGroup.empty() )
	{
		uint32 uSize = (uint32)vecDropGroup.size();
		m_vecItem.resize(uSize);
		for ( uint32 i = 0 ; i < uSize ; ++i )
		{
			_stDropItemDropCell& stCell = m_vecItem[i];
			stCell.init();
			stCell.uIndex	= i;

			const _stDropCellModel* pCellModel = vecDropGroup[i];
			if ( !pCellModel )
				continue;

			stCell.uItemID	= pCellModel->uItemID;
			stCell.uItemNum	= pCellModel->uItemNum;
			stCell.bBind	= pCellModel->bBind;
		}
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneItem::broadCastMsg					(uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent)
{
	if ( !m_pSceneCell )
		return ;

	if ( !m_stTeamInfo.mapMember.empty() )
	{
		stl_map<uint32,_stTeamMember>::iterator _pos = m_stTeamInfo.mapMember.begin();
		stl_map<uint32,_stTeamMember>::iterator _end = m_stTeamInfo.mapMember.end();
		for ( ; _pos != _end ; ++_pos )
		{
			_stTeamMember& memeber = _pos->second;
			CScenePlayer* pPlayer = m_pSceneCell->findPlayer(memeber.stAvatar.stDisplay.uPlayerID);
			if ( !pPlayer )
				continue;

			pPlayer->sendMsg(uMainProtocol,uProtocol,msg,uEvent);
		}
	}
	else
	{
		CScenePlayer* pPlayer = m_pSceneCell->findPlayer(m_uOwnerPlayerID);
		if ( !pPlayer )
			return;

		pPlayer->sendMsg(uMainProtocol,uProtocol,msg,uEvent);
	}
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneItem::autoDiceAward				(_stDropItemDropCell& cell)
{
	if ( cell.uOwnerID || cell.mapDice.empty() )
		return false;

	uint32 uMaxDice = 0;
	{
		stl_map<uint32,uint32>::iterator _pos = cell.mapDice.begin();
		stl_map<uint32,uint32>::iterator _end = cell.mapDice.end();
		for ( ; _pos != _end ; ++_pos )
		{
			uMaxDice = MAX(uMaxDice,_pos->second);
		}
	}

	if (uMaxDice<=0)
		return true;

	stl_vector<_stTeamMember*> vec_mem;
	{
		stl_map<uint32,uint32>::iterator _pos = cell.mapDice.begin();
		stl_map<uint32,uint32>::iterator _end = cell.mapDice.end();
		for ( ; _pos != _end ; ++_pos )
		{
			if ( _pos->second < uMaxDice )
				continue;

			stl_map<uint32,_stTeamMember>::iterator _pos1 = m_stTeamInfo.mapMember.find(_pos->first);
			if ( _pos1 == m_stTeamInfo.mapMember.end() )
				continue;

			vec_mem.push_back(&_pos1->second);
		}
	}

	uint32 uSize = (uint32)vec_mem.size();
	if ( uSize <= 0 )
		return false;

	uint32 uIndex =  randRange(uSize,(uint32)0);
	if ( uIndex >= uSize )
		uIndex = 0;

	_stTeamMember* pOwner = vec_mem[uIndex];
	if ( !pOwner )
		return false;

	return awardItem(cell,&pOwner->stAvatar.stDisplay);
}
//-------------------------------------------------------------
//------------------------------
bool	CSceneItem::diceItem				(_stDropItemDropCell& cell,_stTeamMember* pMember,bool bAbandon,bool bSynDice)
{
	if ( !pMember || cell.uOwnerID )
		return false;

	uint16 uRand = 0;
	if ( !bAbandon )
	{
		uRand = ::randRange((uint32)100,(uint32)1);
	}
	cell.mapDice[pMember->stAvatar.stDisplay.uPlayerID] = uRand;

	if ( bSynDice )
	{
		PBC2DropItem_Dice ack;
		ack.set_dropunitid(getUnitID());
		ack.set_itemindex(cell.uIndex);
		PBDropItem_DiceInfo* pbDice = ack.mutable_info();
		if (pbDice)
		{
			pbDice->set_memberid(pMember->stAvatar.stDisplay.uPlayerID);
			pbDice->set_number(uRand);
		}
		broadCastMsg(P_S2C_Protocol_DropItem,S2C_DropItem_Dice,&ack);
	}

	if ( cell.mapDice.size() < m_stTeamInfo.mapMember.size() )
		return true;

	autoDiceAward(cell);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneItem::awardItem				(_stDropItemDropCell& cell,_stPlayerDisplay* pDisplay)
{
	if ( !pDisplay || cell.uOwnerID )
		return false;

	cell.uOwnerID			= pDisplay->uPlayerID;
	CScenePlayer* pPlayer	= g_pGamePlayerMgr->findPlayer(pDisplay->uPlayerID);
	if ( pPlayer )
	{
		pPlayer->getGameItemMgr().addItem(cell.uItemID,cell.uItemNum,cell.bBind,true,true, DoingType_Drop);
	}
	else
	{
		PBMail stMail;
		stMail.set_playerid(pDisplay->uPlayerID); 
		stMail.set_worldid(pDisplay->uLogicWorldID); 
		stMail.set_type(MailType_DropItem); 
		stMail.set_sendtime((uint64)getTime());
		PBItem* pbItem = stMail.add_item();
		if ( pbItem )
		{
			pbItem->set_itemid(cell.uItemID);
			pbItem->set_itemnum(cell.uItemNum);
			uint32 uFlag = 0;
			_SET_BIT32(uFlag,ItemFlag_Binded,cell.bBind);
			pbItem->set_flag(uFlag);
		}
	}

	PBC2DropItem_Assign ack;
	ack.set_dropunitid(getUnitID());
	PBDropItem_DropCell* pbDice = ack.mutable_cell();
	if (pbDice)
		cell.saveToProtobuf(*pbDice);

	PBPlayerDisplay* pbDisplay = ack.mutable_display();
	if (pbDisplay)
		pDisplay->saveToProtobuf(*pbDisplay);

	broadCastMsg(P_S2C_Protocol_DropItem,S2C_DropItem_Assign,&ack);
	return true;
}

//-------------------------------------------------------------
//------------------------------
bool CSceneItem::awardItemBySendMail(CScenePlayer * pPlayer, uint32 uLogicWorldID)
{
	if (!pPlayer )
		return false;
	PBMail stMail;
	stMail.set_playerid( pPlayer->getPlayerID()); 
	stMail.set_worldid(uLogicWorldID); 
	stMail.set_type(MailType_DropItem); 
	stMail.set_sendtime((uint64)getTime());

	for ( uint32 i = 0; i < (uint32)m_vecItem.size(); ++i )
	{
		_stDropItemDropCell& stCell = m_vecItem[i];
		if ( stCell.uOwnerID )
			continue;

		stCell.uOwnerID = pPlayer->getPlayerID();

		PBItem* pbItem = stMail.add_item();
		if ( pbItem )
		{
			pbItem->set_itemid(stCell.uItemID);
			pbItem->set_itemnum(stCell.uItemNum);
			uint32 uFlag = 0;
			_SET_BIT32(uFlag,ItemFlag_Binded,stCell.bBind);
			pbItem->set_flag(uFlag);
		}

	}

	return CPlayerMail::sendMail(stMail);
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneItem::isOpen					()
{
	return m_bOpen;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneItem::isItemOwner					(uint32 uPlayerID)
{
	return m_uOwnerPlayerID == uPlayerID || m_stTeamInfo.mapMember.find(uPlayerID) != m_stTeamInfo.mapMember.end();
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneItem::open					()
{
	if ( m_uCurAllotType == AllotType_Assign )
	{
		m_uAllotTypeEndTime	= getTime() + g_clConstantData.DropItem_Assign_Time;
	}
	else if (m_uCurAllotType == AllotType_Dice)
	{
		m_uAllotTypeEndTime	= getTime() + g_clConstantData.DropItem_Dice_Time;
	}
	else
	{
		m_uAllotTypeEndTime	= getTime() + g_clConstantData.DropItem_Pick_Time;
	}

	PBC2DropItem_Info ack;
	packPB(ack);

	//广播
	broadCastMsg(P_S2C_Protocol_DropItem,S2C_DropItem_Info,&ack,R_ResultDropItem_Succeed);

	m_bOpen = true;
	{
		stl_map<uint32,_stTeamMember>::iterator _pos = m_stTeamInfo.mapMember.begin();
		stl_map<uint32,_stTeamMember>::iterator _end = m_stTeamInfo.mapMember.end();
		for ( ; _pos != _end ; ++_pos )
		{
			_stTeamMember& memeber = _pos->second;
			CScenePlayer* pPlayer = m_pSceneCell->findPlayer(memeber.stAvatar.stDisplay.uPlayerID);
			if ( !pPlayer )
				continue;

			//CPlayerDropItem* pDropItem = (CPlayerDropItem*)pPlayer->getPlayerFun(GameSystemType_DropItem);
			//CHECKF(NULL != pDropItem);
			//pDropItem->setOpenDropItem(getUnitID());
		}
	}
	return true;
}