/*------------- playerEquipColumn.cpp
 * Copyright (C): 2016  Mokylin·Mokyqi
 * Author       : 方声辉
 * Version      : V1.01
 * Date         : 2017/04/06
 * 
 */
/***************************************************************
* 
***************************************************************/
#include "gameLogic/playerEquipColumn/playerEquipColumn.h"
#include "gameLogic/sceneObject/scenePlayer.h"
//#include "gameLogic/playerQuest/playerQuestMgr.h"
#include "gameLogic/sceneObject/sceneMonster.h"
#include "gameScene/gameSceneCell.h"
#include "cfg_data/equipData/equipData.h"
#include "protocol/CASMsg.pb.h"
#include "protocol/MsgResult.pb.h"
#include "protocol/CASProtocol.pb.h"
#include "protocol/PlayerStruct.pb.h"
#include "log/log.h"
//-------------------------------------------------------------
//------------------------------
CPlayerEquipColumn::CPlayerEquipColumn(CScenePlayer& clPlayer) : CGamePlayerFun(clPlayer)
{
	initialize();
	registerHandler(C2S_EquipColumn_Enhance,	(CGamePlayerFun::_handleSubMsg)&CPlayerEquipColumn::handle_EquipColumnEnhance_Ask);
	registerHandler(C2S_EquipColumn_Forge,		(CGamePlayerFun::_handleSubMsg)&CPlayerEquipColumn::handle_EquipColumnForge_Ask);

}
//-------------------------------------------------------------
//------------------------------
CPlayerEquipColumn::~CPlayerEquipColumn()
{

}

//-------------------------------------------------------------
//------------------------------
void CPlayerEquipColumn::initialize()
{
	CGamePlayerFun::initialize();
	m_pGameAttr = m_clPlayer.registerAttribute(GameSystemType_EqiupColumn);
	for (uint8 type = 1; type < (uint8)_emEquipType_ARRAYSIZE; ++type)
	{
		m_mapEqiupColumn[type].uID = type;
		m_mapEqiupColumn[type].uEnhanceLevel = 0;
		m_mapEqiupColumn[type].uForgeLevel = 0;
	}
}

//-------------------------------------------------------------
//------------------------------
void CPlayerEquipColumn::newOnline()
{
	initialize();
}

//-------------------------------------------------------------
//------------------------------ 
bool CPlayerEquipColumn::loadFromProtobuf(const PBPlayerData& pbPlayer)
{
	const PBPlayerEquipColumn& pbPlayerEquipColumn = pbPlayer.playersystem().equipcolumn();

	for (int32 i = 0; i < pbPlayerEquipColumn.info_size(); ++i)
	{
		const PBEquipColumn & pbEquipColumn = pbPlayerEquipColumn.info(i);
		_stEqiupColumn& pEqiupColumn = m_mapEqiupColumn[pbEquipColumn.id()];
		pEqiupColumn.uID			= pbEquipColumn.id();
		pEqiupColumn.uEnhanceLevel	= pbEquipColumn.enhancelevel();
		pEqiupColumn.uForgeLevel	= pbEquipColumn.forgelevel();
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool CPlayerEquipColumn::saveToProtobuf(PBPlayerData& pbPlayer)
{
	PBPlayerSystem* pSystem = pbPlayer.mutable_playersystem();
	CHECKF(pSystem != NULL);
	
	PBPlayerEquipColumn	* pEquipColumn = pSystem->mutable_equipcolumn();
	CHECKF(pEquipColumn != NULL);

	pEquipColumn->Clear();
	{
		auto _pos = m_mapEqiupColumn.begin();
		auto _end = m_mapEqiupColumn.end();
		for (; _pos != _end; ++_pos)
		{
			PBEquipColumn	*pInfo	= pEquipColumn->add_info();
			if (!pInfo)
				continue;

			pInfo->set_id(_pos->first);
			pInfo->set_enhancelevel(_pos->second.uEnhanceLevel);
			pInfo->set_forgelevel(_pos->second.uForgeLevel);
		}
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------  装备栏强化
bool CPlayerEquipColumn::handle_EquipColumnEnhance_Ask(uint8 uEventID, CNetStream& clStream)
{
	PBC2GEquipColumnEnhance ask;
	CHECKF(clStream.ParseFromStream(ask));
	stl_map<uint64, _stExpendItem> mapExpend;
	
	for (int32 i = 0; i < ask.pbitemdata().size(); ++i)
	{
		const PBDelItemata& pbData = ask.pbitemdata(i);
		_stExpendItem &stItem = mapExpend[ pbData.itemsn()];
		stItem.uItemSN = pbData.itemsn();
		stItem.uItemNum = pbData.num();
	}

	uint8 uCode = R_ResultEquipColumn_Fail;
	if (!equipColumnEnhance(ask.equiptype(),mapExpend,uCode))
		send_EquipColumnEnhanceAck(ask.equiptype(),uCode);

	return true;
}

//-------------------------------------------------------------
//------------------------------装备栏打磨
bool CPlayerEquipColumn::handle_EquipColumnForge_Ask(uint8 uEventID, CNetStream& clStream)
{
	PBC2GEquipColumnForge ask;
	CHECKF(clStream.ParseFromStream(ask));

	stl_map<uint64, _stExpendItem> mapExpend;

	for (int32 i = 0; i < ask.pbitemdata().size(); ++i)
	{
		const PBDelItemata& pbData = ask.pbitemdata(i);
		_stExpendItem &stItem = mapExpend[ pbData.itemsn()];
		stItem.uItemSN = pbData.itemsn();
		stItem.uItemNum = pbData.num();
	}

	uint8 uCode = R_ResultEquipColumn_Fail;
	if (!equipColumnForge(ask.equiptype(),mapExpend,uCode))
		send_EquipColumnForgeAck(ask.equiptype(),uCode);

	return true;
}

//-------------------------------------------------------------
//------------------------------ 强化返回
bool CPlayerEquipColumn::send_EquipColumnEnhanceAck	(uint8 uID,uint8 uCode/* = R_ResultItem_Succeed*/)
{
	PBG2CEquipColumnEnhance ack;

	_stEqiupColumn *pEquiupColumn = find(uID);
	if (!pEquiupColumn)
		return false;
	
	ack.set_equiptype(uID);
	ack.set_level(pEquiupColumn->uEnhanceLevel);
	return m_clPlayer.sendMsg(P_S2C_Protocol_EquipColumn,S2C_EquipColumn_Enhance,&ack,uCode);
}

//-------------------------------------------------------------
//------------------------------  打磨返回
bool CPlayerEquipColumn::send_EquipColumnForgeAck(uint8 uID,uint8 uCode/* = R_ResultItem_Succeed*/)
{
	PBG2CEquipColumnForge ack;
	_stEqiupColumn *pEquiupColumn = find(uID);
	if (!pEquiupColumn)
		return false;

	ack.set_equiptype(uID);
	ack.set_level(pEquiupColumn->uForgeLevel);
	return m_clPlayer.sendMsg(P_S2C_Protocol_EquipColumn,S2C_EquipColumn_Forge,&ack,uCode);
}

//-------------------------------------------------------------
//------------------------------
bool CPlayerEquipColumn::equipColumnEnhance(uint8 uEquipType,stl_map<uint64, _stExpendItem> &mapExpend,uint8& uCode)
{
	_stEqiupColumn *pEquiupColumn = find(uEquipType);
	if (!pEquiupColumn)
	{
		uCode = R_ResultEquipColumn_NoFind;
		return false;
	}
	
	if (mapExpend.empty())
	{
		uCode = R_ResultEquipColumn_NoItem;
		return false;
	}

	// 已经满级
	if (pEquiupColumn->uEnhanceLevel >= g_clEquipData.getEnhanceMaxLevel(uEquipType))
	{
		uCode = R_ResultEquipColumn_EnhanceLevelFull;
		return false;
	}
	
	uint16 upLevel = pEquiupColumn->uEnhanceLevel + 1;
	_stEquipEnhanceModel *pEquipEnhanceModel = g_clEquipData.findEnhanceModelByLevel(uEquipType, upLevel);
	if (!pEquipEnhanceModel)
	{
		uCode= R_ResultEquipColumn_Fail;
		return false;
	}

	// 检查前提条件
	if (!m_clPlayer.canPreCondition(pEquipEnhanceModel->dmapPreCondition))
	{
		uCode = R_ResultEquipColumn_PreCondition;
		return false;
	}

	// 检查消耗
	if (!m_clPlayer.canDelExpend(pEquipEnhanceModel->mapNeedExpend, DoingType_Equip_Forge))
	{
		uCode = R_ResultEquipColumn_NeedExpend;
		return false;
	}
	MAP_U32_U16 mapItemIDNum;
	// 检查材料
	auto _pos = mapExpend.begin();
	auto _end	= mapExpend.end();
	
	for (;_pos!= _end; ++_pos )
	{
		uint64 uItemSN =  _pos->second.uItemSN;
		uint32 uItemNum = _pos->second.uItemNum;
		CGameItem* pTmpGameItem = m_clPlayer.getGameItemMgr().findItem(uItemSN);
		if (!pTmpGameItem )
		{
			uCode= R_ResultEquipColumn_NoItem;
			return false;
		}

		const _stItemModel* pTmpModel = pTmpGameItem->getModel();
		if (!pTmpModel || pTmpModel->uType != ItemType_Material )
		{
				uCode= R_ResultEquipColumn_ItemTypeErr;
				return false;
		}

		mapItemIDNum[pTmpGameItem->getItemID()] += uItemNum;
	}

	

	//扣除消耗
	CHECKL(m_clPlayer.delExpend(pEquipEnhanceModel->mapNeedExpend, DoingType_Equip_Enhance));

	//扣除材料
	auto _pos1   = mapItemIDNum.begin();
	auto _end1   = mapItemIDNum.end();
	for (;_pos1!= _end1; ++_pos1 )
	{
		uint32 uItemID =  _pos1->first;
		uint32 uItemNum = _pos1->second;

		uint16 *uItemNuminMod = pEquipEnhanceModel->mapNeedMaterial.find_(uItemID);
		if (!uItemNuminMod || uItemNum < *uItemNuminMod)
		{
			uCode= R_ResultEquipColumn_ItemNumErr;
			return false;
		}

		m_clPlayer.getGameItemMgr().removeItem(uItemID,	uItemNum, true, uCode, ItemBindType_All, DoingType_Equip_Forge);
	}
	// 升一级
	pEquiupColumn->uEnhanceLevel += 1;
	
	calculateAttri();

	//CPlayerQuestMgr* pQuestMgr = m_clPlayer.getPlayerQuestMgr();
	//if (pQuestMgr)
	//	pQuestMgr->callback_completeEnhance();
	//发送客户端
	send_EquipColumnEnhanceAck(uEquipType,R_ResultItem_Succeed);
	return true;
}

//-------------------------------------------------------------
//------------------------------
bool CPlayerEquipColumn::equipColumnForge(uint8 uEquipType,stl_map<uint64, _stExpendItem> &mapExpend,uint8& uCode)
{
	_stEqiupColumn *pEquiupColumn = find(uEquipType);
	if (!pEquiupColumn)
	{
		uCode = R_ResultEquipColumn_NoFind;
		return false;
	}

	if (mapExpend.empty())
	{
		uCode = R_ResultEquipColumn_NoItem;
		return false;
	}

	// 已经满级
	if (pEquiupColumn->uForgeLevel >= g_clEquipData.getForgeMaxLevel(uEquipType))
	{
		uCode = R_ResultEquipColumn_ForgeLevelFull;
		return false;
	}

	uint16 upLevel = pEquiupColumn->uForgeLevel + 1;
	_stEquipForgeModel *pForgeModel = g_clEquipData.findForgeModelByLevel(uEquipType, upLevel);
	if (!pForgeModel)
	{
		uCode= R_ResultEquipColumn_Fail;
		return false;
	}

	// 检查前提条件
	if (!m_clPlayer.canPreCondition(pForgeModel->dmapPreCondition))
	{
		uCode = R_ResultEquipColumn_PreCondition;
		return false;
	}

	if (pEquiupColumn->uEnhanceLevel < pForgeModel->uEnhanceLevel)
	{
		uCode = R_ResultEquipColumn_NeedEnhanceLevel;
		return false;
	}

	// 检查消耗
	if (!m_clPlayer.canDelExpend(pForgeModel->mapNeedExpend))
	{
		uCode = R_ResultEquipColumn_NeedExpend;
		return false;
	}

	MAP_U32_U16 mapItemIDNum;
	// 检查材料
	auto _pos = mapExpend.begin();
	auto _end	= mapExpend.end();
	for (;_pos!= _end; ++_pos )
	{
		uint64 uItemSN =  _pos->second.uItemSN;
		uint32 uItemNum = _pos->second.uItemNum;
		CGameItem* pTmpGameItem = m_clPlayer.getGameItemMgr().findItem(uItemSN);
		if (!pTmpGameItem )
		{
			uCode= R_ResultEquipColumn_NoItem;
			return false;
		}

		const _stItemModel* pTmpModel = pTmpGameItem->getModel();
		if (!pTmpModel || pTmpModel->uType != ItemType_Material )
		{
			uCode= R_ResultEquipColumn_ItemTypeErr;
			return false;
		}

		mapItemIDNum[pTmpGameItem->getItemID()] += uItemNum;

		
	}

	//扣除消耗
	CHECKL(m_clPlayer.delExpend(pForgeModel->mapNeedExpend, DoingType_Equip_Forge));

	//扣除材料
	 auto _pos1   = mapItemIDNum.begin();
	 auto _end1   = mapItemIDNum.end();
	 for (;_pos1!= _end1; ++_pos1 )
	 {
		 uint32 uItemID =  _pos1->first;
		 uint32 uItemNum = _pos1->second;
		
		 uint16 *uItemNuminMod = pForgeModel->mapNeedMaterial.find_(uItemID);
		 if (!uItemNuminMod || uItemNum < *uItemNuminMod)
		 {
			 uCode= R_ResultEquipColumn_ItemNumErr;
			 return false;
		 }
		 
		 m_clPlayer.getGameItemMgr().removeItem(uItemID,	uItemNum, true, uCode, ItemBindType_All, DoingType_Equip_Forge);
	}

	// 升一级
	pEquiupColumn->uForgeLevel += 1;

	calculateAttri();
	
	//CPlayerQuestMgr* pQuestMgr = m_clPlayer.getPlayerQuestMgr();
	//if (pQuestMgr)
	//	pQuestMgr->callback_completeItemForge();
	//发送客户端
	send_EquipColumnForgeAck(uEquipType,R_ResultItem_Succeed);
	return true;
}

//-------------------------------------------------------------
//------------------------------查询打磨等级
uint32	CPlayerEquipColumn::getForgeLevel(uint8 uEquipType)	
{
	_stEqiupColumn*	p = find(uEquipType);
	if (p)
	{
		return p->uForgeLevel;
	}
	return 0;
}

//-------------------------------------------------------------
//------------------------------重新计算属性计算属性
bool	CPlayerEquipColumn::calculateAttri()
{
	if (!m_pGameAttr)
		return true;

	m_pGameAttr->initGameAttribute();

	auto _pos = m_mapEqiupColumn.begin();
	for (; _pos != m_mapEqiupColumn.end(); ++_pos)
	{
		 _stEqiupColumn* pEqiupColumn = &_pos->second;
		if (!pEqiupColumn)
			continue;
		
		// 装备栏强化加属性		 
		_stEquipEnhanceModel *pEquipEnhanceModel = g_clEquipData.findEnhanceModelByLevel(pEqiupColumn->uID,\
			pEqiupColumn->uEnhanceLevel);
		if (pEquipEnhanceModel)
		{
			*m_pGameAttr += pEquipEnhanceModel->stAttribute;
		}

		//装备栏打磨加属性 百分比
		_stEquipForgeModel *pForgeModel = g_clEquipData.findForgeModelByLevel(pEqiupColumn->uID,\
			pEqiupColumn->uForgeLevel);
		if (pForgeModel)
		{
			CGameItem * pGameEquip = m_clPlayer.getGameItemMgr().findEquip(pEqiupColumn->uID);
			if (!pGameEquip)
				continue;

			const _stItemModel*	pItmeModel = pGameEquip->getModel();
			if(!pItmeModel ||pItmeModel->vecAttribute.empty())
				continue;
		
			_stGameAttribute stBaseGameAttri = pItmeModel->vecAttribute[0];
			stBaseGameAttri *= (pForgeModel->uUpgradeRate / 10000.f);
			*m_pGameAttr  += stBaseGameAttri;
		}
	}

	m_clPlayer.setUpdateValue(_UV_AttributeBase);
	return true;
}
