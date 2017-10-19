/*----------------- playerItem.cpp
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/4/12 17:16:30
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "gameLogic/playerItem/playerItem.h"
#include "gameLogic/gameItem/gameItemManager.h"
#include "gameLogin/gamePlayerMgr.h"
#include "protocol/CASProtocol.pb.h"
#include "protocol/CASMsg.pb.h"
#include "protocol/MsgResult.pb.h"
#include "basic/basicTypes.h"
#include "log/log.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------
CPlayerItem::CPlayerItem(CScenePlayer& clPlayer) : CGamePlayerFun(clPlayer)
{
	
	registerHandler(C2S_Item_Del,		(CGamePlayerFun::_handleSubMsg)&CPlayerItem::handle_DelItem_Ask);
	registerHandler(C2S_Item_Use,		(CGamePlayerFun::_handleSubMsg)&CPlayerItem::handle_UseItem_Ask);
	registerHandler(C2S_Item_Equip,		(CGamePlayerFun::_handleSubMsg)&CPlayerItem::handle_EquipItem_Ask);
	registerHandler(C2S_Item_Sell,		(CGamePlayerFun::_handleSubMsg)&CPlayerItem::handle_SellItem_Ask);
	registerHandler(C2S_Item_BuyBack,	(CGamePlayerFun::_handleSubMsg)&CPlayerItem::handle_BuyBackItem_Ask);
	registerHandler(C2S_Item_Split,		(CGamePlayerFun::_handleSubMsg)&CPlayerItem::handle_SplitItem_Ask);
	registerHandler(C2S_Item_Sort,		(CGamePlayerFun::_handleSubMsg)&CPlayerItem::handle_SortItem_Ask);
	registerHandler(C2S_Item_Inherit,	(CGamePlayerFun::_handleSubMsg)&CPlayerItem::handle_EquipInherit_Ask);
	registerHandler(C2S_Item_Make,		(CGamePlayerFun::_handleSubMsg)&CPlayerItem::handle_EquipMake_Ask);
	registerHandler(C2S_Item_Mounting,	(CGamePlayerFun::_handleSubMsg)&CPlayerItem::handle_Mounting_Ask);
	registerHandler(C2S_Item_BingfuSplit,	(CGamePlayerFun::_handleSubMsg)&CPlayerItem::handle_BingfuSplit_Ask);
	registerHandler(C2S_Item_BingfuUpgrade,	(CGamePlayerFun::_handleSubMsg)&CPlayerItem::handle_BingfuUpgrade_Ask);
	registerHandler(C2S_Item_BingfuUpOneLevel,(CGamePlayerFun::_handleSubMsg)&CPlayerItem::handle_BingfuUpOneLevel_Ask);
	registerHandler(C2S_Item_Compound,	(CGamePlayerFun::_handleSubMsg)&CPlayerItem::handle_Compound_Ask);

	registerHandler(C2S_Item_OpenEquipMakeDlg,			(CGamePlayerFun::_handleSubMsg)&CPlayerItem::handle_OpenEquipMakeDlg_Ask);

	registerHandler(C2S_Item_OpenEquipMakeOEMDlg,		(CGamePlayerFun::_handleSubMsg)&CPlayerItem::handle_OpenEquipMakeOEMDlg_Ask);
	registerHandler(C2S_Item_CommitEquipMakeOEM,		(CGamePlayerFun::_handleSubMsg)&CPlayerItem::handle_CommitEquipMakeOEM_Ask);
	registerHandler(C2S_Item_VisitEquipMakeOEM,			(CGamePlayerFun::_handleSubMsg)&CPlayerItem::handle_VisitEquipMakeOEM_Ask);
	registerHandler(C2S_Item_EquipMakeOEM,				(CGamePlayerFun::_handleSubMsg)&CPlayerItem::handle_EquipMakeOEM_Ask);
	//registerHandler(C2S_Item_EquipChangeOEMFee,			(CGamePlayerFun::_handleSubMsg)&CPlayerItem::handle_EquipChangeOEMFee_Ask);


	m_pGameItemMgr = &clPlayer.getGameItemMgr();
}


//-------------------------------------------------------------
//------------------------------
CPlayerItem::~CPlayerItem()
{

}
//-------------------------------------------------------------
//------------------------------
void CPlayerItem::initialize()
{
	CGamePlayerFun::initialize();
	m_pGameItemMgr->initialize(&m_clPlayer);
}

//-------------------------------------------------------------
//------------------------------
bool CPlayerItem::loadFromProtobuf(const PBPlayerData& pbPlayer)
{
	return m_pGameItemMgr->load(pbPlayer.playerbag());
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerItem::saveToProtobuf(PBPlayerData& pbPlayer)
{
	return m_pGameItemMgr->save(pbPlayer.mutable_playerbag());
}
//-------------------------------------------------------------
//------------------------------每日清理
void CPlayerItem::cleanupDaily()
{
	return m_pGameItemMgr->cleanupDaily();
}
//-------------------------------------------------------------
//------------------------------每周清理
void CPlayerItem::cleanupWeekly()
{
	return m_pGameItemMgr->cleanupWeekly();
}
//-------------------------------------------------------------
//------------------------------每月清理
void CPlayerItem::cleanupMonthly()
{
	return m_pGameItemMgr->cleanupMonthly();
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerItem::handle_DelItem_Ask(uint8 uEventID, CNetStream& clStream)
{
	PBC2GDelItem ask;
	CHECKF(clStream.ParseFromStream(ask));
	uint8 uCode = R_ResultItem_Fail;
	if (!m_pGameItemMgr->removeItem(ask.itemsnid(),true,uCode, DoingType_DelItem))
		m_pGameItemMgr->send_DelItemAck(ask.itemsnid(),uCode);

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerItem::handle_UseItem_Ask(uint8 uEventID, CNetStream& clStream)
{
	PBC2GUseItem ask;
	CHECKF(clStream.ParseFromStream(ask));
	uint8 uCode = R_ResultItem_Fail;
	if (!m_pGameItemMgr->useItem(ask.itemsnid(),ask.itemnum(),uCode))
		m_pGameItemMgr->send_UseItemAck(ask.itemsnid(),ask.itemnum(),uCode);

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerItem::handle_EquipItem_Ask(uint8 uEventID, CNetStream& clStream)
{
	PBC2GEquipItem ask;
	CHECKF(clStream.ParseFromStream(ask));
	uint8 uCode = R_ResultItem_Fail;
	if (!m_pGameItemMgr->equipItem(ask.itemsnid(),ask.equiptype(),uCode))
		m_pGameItemMgr->send_EquipItemAck(ask.itemsnid(),ask.equiptype(),uCode);

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerItem::handle_SellItem_Ask(uint8 uEventID, CNetStream& clStream)
{
	PBC2GSellItem ask;
	CHECKF(clStream.ParseFromStream(ask));
	uint8 uCode = R_ResultItem_Fail;
	if (!m_pGameItemMgr->sellItem(ask.itemsnid(),uCode))
		m_pGameItemMgr->send_SellItemAck(ask.itemsnid(),uCode);

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerItem::handle_BuyBackItem_Ask(uint8 uEventID, CNetStream& clStream)
{
	PBC2GSellItem ask;
	CHECKF(clStream.ParseFromStream(ask));
	uint8 uCode = R_ResultItem_Fail;
	if (!m_pGameItemMgr->buyBackItem(ask.itemsnid(),uCode))
		m_pGameItemMgr->send_BuyBackItemAck(ask.itemsnid(),uCode);

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerItem::handle_SplitItem_Ask(uint8 uEventID, CNetStream& clStream)
{
	PBC2GSplitItem ask;
	if (!clStream.ParseFromStream(ask))
		return false;

	SET_U64 setItemSN;
	for (int32 i = 0; i < ask.itemsnid_size(); ++i)
		setItemSN.insert(ask.itemsnid(i));

	uint8 uCode = R_ResultItem_Fail;
	if (!m_pGameItemMgr->splitItem(setItemSN,uCode))
		m_pGameItemMgr->send_SplitItemAck(setItemSN,uCode);

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerItem::handle_SortItem_Ask(uint8 uEventID, CNetStream& clStream)
{
	PBC2GSortItem ask;
	CHECKF(clStream.ParseFromStream(ask));

	bool bResult = true;
	uint8 uCode = R_ResultItem_Fail;
	for (uint16 i = 0; i < ask.pbsort_size(); ++i)
	{
		const PBItemSortData& pbSort = ask.pbsort(i);
		bResult = m_pGameItemMgr->moveItem(pbSort.fromitemsnid(),pbSort.toitemsnid(),pbSort.itemnum(),uCode);
		if (!bResult)
			break;
	}

	if (!bResult)
		m_pGameItemMgr->send_SortItemAck(uCode);

	return true;
}

//-------------------------------------------------------------
//------------------------------
bool CPlayerItem::handle_EquipInherit_Ask(uint8 uEventID, CNetStream& clStream)
{
	PBC2GEquipInherit ask;
	CHECKF(clStream.ParseFromStream(ask));

	uint8 uCode = R_ResultItem_Fail;
	if (!m_pGameItemMgr->equipInherit(ask.sourcesn(),ask.targetsn(),uCode))
		m_pGameItemMgr->send_EquipInheritAck(ask.sourcesn(),uCode);

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerItem::handle_EquipMake_Ask(uint8 uEventID, CNetStream& clStream)
{
	PBC2GEquipMake ask;
	CHECKF(clStream.ParseFromStream(ask));

	uint8 uCode = R_ResultItem_Fail;
	if (!m_pGameItemMgr->preCheckMakeSkilled(ask.makeid(), uCode))
	{//做熟练度检查
		m_pGameItemMgr->send_EquipMakeAck(ask.makeid(),ask.mark(),uCode);
	}
	else if (!m_pGameItemMgr->equipMake(ask.makeid(),ask.mark(),uCode))
	{
		m_pGameItemMgr->send_EquipMakeAck(ask.makeid(),ask.mark(),uCode);
	}
	else
	{
		//一切正常,要做熟练度,活力方面的调整.
		m_pGameItemMgr->equipMakeSuccessAddSkilled(ask.makeid());
		m_pGameItemMgr->openMakeEquipDlgAsk(uCode);
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerItem::handle_Mounting_Ask(uint8 uEventID, CNetStream& clStream)
{
	PBC2GMounting ask;
	CHECKF(clStream.ParseFromStream(ask));

	uint8 uCode = R_ResultItem_Fail;
	if (!m_pGameItemMgr->mounting(ask.ismount(),uCode))
	{
		m_pGameItemMgr->send_MountingAck(uCode);
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerItem::handle_BingfuSplit_Ask(uint8 uEventID, CNetStream& clStream)
{
	PBC2GBingfuSplitAsk pbAsk;
	CHECKF(clStream.ParseFromStream(pbAsk));
	MAP_U64_U16 mapSplit;
	for (int32 i = 0; i < pbAsk.expend_size(); ++i)
	{
		const PBBingfuNumData& pbData = pbAsk.expend(i);
		mapSplit.insert_(pbData.bingfusn(),pbData.num());
	}
	uint8 uCode = R_ResultBingfu_Fail;
	if (!m_pGameItemMgr->bingfuSplitAsk(mapSplit,uCode))
	{
		m_pGameItemMgr->send_BingfuSplit_Ack(uCode);
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerItem::handle_BingfuUpgrade_Ask(uint8 uEventID, CNetStream& clStream)
{
	PBC2GBingfuUpgradeAsk pbAsk;
	CHECKF(clStream.ParseFromStream(pbAsk));

	MAP_U64_U16 mapExpend;
	for (int32 i = 0; i < pbAsk.expend_size(); ++i)
	{
		const PBBingfuNumData& pbData = pbAsk.expend(i);
		mapExpend.insert_(pbData.bingfusn(),pbData.num());
	}

	uint8 uCode = R_ResultBingfu_Fail;
	if (!m_pGameItemMgr->bingfuUpgradeAsk(pbAsk.sn(),mapExpend,uCode))
	{
		m_pGameItemMgr->send_BingfuUpgrade_Ack(pbAsk.sn(),uCode);
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerItem::handle_BingfuUpOneLevel_Ask(uint8 uEventID, CNetStream& clStream)
{
	PBC2GBingfuUpOneLevelAsk ask;
	CHECKF(clStream.ParseFromStream(ask));

	uint8 uCode = R_ResultItem_Fail;
	if (!m_pGameItemMgr->bingfuUpOneLevelAsk(ask.sn(),uCode))
	{
		m_pGameItemMgr->send_BingfuUpgrade_Ack(ask.sn(),uCode);
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerItem::handle_Compound_Ask(uint8 uEventID, CNetStream& clStream)
{
	PBC2GItemCompoundAsk ask;
	CHECKF(clStream.ParseFromStream(ask));

	MAP_U64_U16 mapItem;
	for (int32 i = 0; i < ask.itemnum_size(); ++i)
	{
		const PBItemSnNum& pbData = ask.itemnum(i);
		mapItem.insert_(pbData.itemsn(),pbData.itemnum());
	}

	uint8 uCode = R_ResultItem_Fail;
	if (!m_pGameItemMgr->compoundAsk(ask.recipeid(),mapItem,uCode))
	{
		m_pGameItemMgr->send_Compound_Ack(0,uCode);
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerItem::handle_OpenEquipMakeDlg_Ask			(uint8 uEventID, CNetStream& clStream)// 打开装备制造界面,返回熟练度信息							PBC2GItemOpenEquipMakeDlgAsk
{
	uint8 uCode = R_ResultItem_Fail;
	if (!m_pGameItemMgr->openMakeEquipDlgAsk(uCode))
	{
		uCode = R_ResultItem_Fail;
		m_pGameItemMgr->send_OpenMakeDlgAck(NULL,uCode);
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerItem::handle_OpenEquipMakeOEMDlg_Ask		(uint8 uEventID, CNetStream& clStream)// 打开自己的装备代工界面									PBC2GItemOpenEquipMakeOEMDlgAsk
{
	uint8 uCode = R_ResultItem_Fail;
	if (!m_pGameItemMgr->openMakeEquipOemDlgAsk(uCode))
	{
		uCode = R_ResultItem_Fail;
		m_pGameItemMgr->send_OpenMakeOEMDlgAck(NULL,uCode);
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerItem::handle_CommitEquipMakeOEM_Ask		(uint8 uEventID, CNetStream& clStream)// 提交自己的装备代工信息,如果取消就提交一个空的列表		PBC2GItemCommitEquipMakeOEMAsk
{
	PBC2GItemCommitEquipMakeOEMAsk ask;
	CHECKF(clStream.ParseFromStream(ask));

	uint8 uCode = R_ResultItem_Fail;
	if (!m_pGameItemMgr->commitMakeEquipOemAsk(ask, uCode))
	{
		uCode = R_ResultItem_Fail;
		m_pGameItemMgr->send_CommitMakeOEMAck(NULL,uCode);
	}
	else
	{
		//设置成功以后,重新发送界面
		m_pGameItemMgr->openMakeEquipOemDlgAsk(uCode);
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerItem::handle_VisitEquipMakeOEM_Ask		(uint8 uEventID, CNetStream& clStream)// 打开别人的装备代工界面									PBC2GItemVisitEquipMakeOEMAsk
{
	PBC2GItemVisitEquipMakeOEMAsk ask;
	CHECKF(clStream.ParseFromStream(ask));

	uint8 uCode = R_ResultItem_Fail;
	if (!m_pGameItemMgr->visitMakeEquipOemAsk(ask.playerid(), uCode))
	{
		uCode = R_ResultItem_Fail;
		m_pGameItemMgr->send_VisitMakeOEMAck(NULL,uCode);
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerItem::handle_EquipMakeOEM_Ask				(uint8 uEventID, CNetStream& clStream)// 请求别人装备代工											PBC2GItemEquipMakeOEMAsk
{
	PBC2GItemEquipMakeOEMAsk ask;
	CHECKF(clStream.ParseFromStream(ask));


	uint8 uCode = R_ResultItem_Fail;

	CScenePlayer* pMaker = g_pGamePlayerMgr->findPlayer(ask.playerid());
	if (ask.playerid() == m_clPlayer.getPlayerID())
	{
		uCode = R_ResultItem_Fail;
		m_pGameItemMgr->send_VisitMakeOEMAck(NULL,uCode);
	}
	else if (!pMaker)
	{
		uCode = R_ResultItem_OEM_Not_Online;
		m_pGameItemMgr->send_VisitMakeOEMAck(NULL,uCode);
	}
	else if (!pMaker->getGameItemMgr().preCheckMakeOEMSkilled(ask.makeid(), &m_clPlayer, uCode))
	{
		m_pGameItemMgr->send_VisitMakeOEMAck(NULL,uCode);
	}
	else if (!m_pGameItemMgr->equipMake(ask.makeid(), ask.mark(), uCode))
	{
		uCode = R_ResultItem_Fail;
		m_pGameItemMgr->send_VisitMakeOEMAck(NULL,uCode);
	}
	else
	{//成功

		//卖家得钱,扣除活力,买家扣掉钱
		pMaker->getGameItemMgr().oemMakeSuccessAddSkilled(ask.makeid(), &m_clPlayer);
		pMaker->getGameItemMgr().oemCheckRemove(ask.makeid());
		uint8 uCode = 0;
		pMaker->getGameItemMgr().openMakeEquipOemDlgAsk(uCode);
	}
	//重新访问
	m_pGameItemMgr->visitMakeEquipOemAsk(ask.playerid(), uCode);

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerItem::handle_EquipChangeOEMFee_Ask		(uint8 uEventID, CNetStream& clStream)// 更改代工费用											PBC2GItemEquipMakeOEMAsk
{
	PBC2GItemEquipChangeOEMFeeAsk ask;
	CHECKF(clStream.ParseFromStream(ask));

	uint8 uCode = R_ResultItem_Fail;
	if (!m_pGameItemMgr->changeOemFeeAsk(ask.makeid(), ask.fee(), uCode))
	{
		uCode = R_ResultItem_Fail;
		m_pGameItemMgr->send_ChangeOEMFeeAck(NULL,uCode);
	}
	else
	{
		//设置成功以后,重新发送界面
		m_pGameItemMgr->openMakeEquipOemDlgAsk(uCode);
	}
	return true;

}

