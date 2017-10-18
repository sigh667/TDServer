/*----------------- gameItemManager.cpp
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����
* Version      : V1.0
* Date         : 2016/4/12 17:09:46
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "gameLogic/gameItem/gameItemManager.h"
#include "gameLogic/sceneObject/scenePlayer.h"
#include "gameLogic/gameItem/itemFactory.h"
#include "gameLogic/gameMail/gameMail.h"
//#include "gameLogic/playerWulue/playerWulue.h"
//#include "gameLogic/playerQuest/playerQuestMgr.h"
#include "gameLogic/playerCommon/playerCommon.h"
//#include "gameLogic/playerFashion/playerFashion.h"
//#include "gameLogic/playerActivity/playerActivity.h"

#include "cfg_data/itemData/itemData.h"
#include "cfg_data/constantData/constantData.h"
#include "cfg_data/equipData/equipData.h"
#include "cfg_data/copymapData/copymapModel.h"
#include "cfg_data/bingfuData/bingfuData.h"
#include "cfg_data/treasureMapData/treasureMapData.h"

#include "protocol/MsgResult.pb.h"
#include "protocol/CASProtocol.pb.h"
#include "protocol/PlayerStruct.pb.h"
#include "protocol/CASMsg.pb.h"
#include "protocol/GameDefine.pb.h"

#include "basic/basicTypes.h"
#include "log/log.h"
#include "game_log.h"
#include "gameScene/gameScene.h"
#include "gameScene/copymapBase.h"
#include "gameLogin/gamePlayerMgr.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
CGameItemMgr::CGameItemMgr()
{
	//����
	memset(m_fnUseItem,0,sizeof(m_fnUseItem));
	m_fnUseItem[ItemUseType_Expend				]	= (_use_item)&CGameItemMgr::itemUse_AddExpend	;
	m_fnUseItem[ItemUseType_Buff				]	= (_use_item)&CGameItemMgr::itemUse_AddBuff		;
	m_fnUseItem[ItemUseType_Transfer			]	= (_use_item)&CGameItemMgr::itemUse_Tranfer		;
	m_fnUseItem[ItemUseType_Packet				]	= (_use_item)&CGameItemMgr::itemUse_Packet		;
	m_fnUseItem[ItemUseType_Treasure			]	= (_use_item)&CGameItemMgr::itemUse_Treasure	;
	m_fnUseItem[ItemUseType_Fashion				]	= (_use_item)&CGameItemMgr::itemUse_Fashion		;

	initialize(NULL);
}

//-------------------------------------------------------------
//------------------------------ 
CGameItemMgr::~CGameItemMgr()
{
	initialize(NULL);
}

//-------------------------------------------------------------
//------------------------------ 
void CGameItemMgr::initialize(CScenePlayer* pPlayer)
{
	m_pPlayer			= pPlayer;
	m_uNextCheckExpire	= getTime() + 10;
	clear();
	if (pPlayer)
		m_pItemGameAttr = pPlayer->registerAttribute(GameSystemType_Item);
}
//-------------------------------------------------------------
//------------------------------ 
CGameItem*	CGameItemMgr::findAllItem (uint64 uItemSN)
{
	CGameItem* pGameItem = findItem(uItemSN);
	if (!pGameItem)
	{
		ITER_MAP_EQUIP _pos = m_mapEquip.begin();
		for (; _pos != m_mapEquip.end(); ++_pos)
		{
			CGameItem* pTmpGameItem = _pos->second;
			if (pTmpGameItem->getItemSN() == uItemSN)
				return pTmpGameItem;
		}
	}
	return pGameItem;
}

//-------------------------------------------------------------
//------------------------------ 
CGameItem*	CGameItemMgr::findNoFullBingfu (const _stItem& stItem)
{
	DMAP_TYPE_BINGFU::_sub_map* pSubmap = m_dmapBingfu.find_(stItem.uItemID);
	if (!pSubmap)
		return NULL;

	DMAP_TYPE_BINGFU::_sub_map::iterator _pos = pSubmap->begin();
	for (; _pos != pSubmap->end(); ++_pos)
	{
		CGameItem* pGameItem = _pos->second;
		if (!pGameItem || pGameItem->getBingfuExp() != stItem.uBingfuExp || pGameItem->isBind() != stItem.testFlag(ItemFlag_Binded))
			continue;

		if (stItem.testFlag(ItemFlag_IsInWulue) || !pGameItem->getSpareNum())
			continue;

		return pGameItem;
	}

	return NULL;
}
//-------------------------------------------------------------
//------------------------------ 
uint16 CGameItemMgr::getAllForgeMinLevel()
{
	uint32 uLevel = 0;
	for (uint8 i = EquipType_Weapon; i < _emEquipType_ARRAYSIZE; ++i)
	{
		if (EquipType_Mount == i)
			continue;

		CGameItem* pGameItem = findEquip(i);
		if (!pGameItem || 0 == pGameItem->getForgeExp())
			return 0;
		const _stEquipForgeModel* pModel = g_clEquipData.findForgeModel(i,pGameItem->getForgeExp());
		if (pModel)
		{
			if (!uLevel)
			{
				uLevel = pModel->uLevel;
			}
			else
			{
				uLevel = MIN(uLevel,(uint32)pModel->uLevel);
			}
		}

	}
	
	return uLevel;
}
//-------------------------------------------------------------
//------------------------------ 
bool CGameItemMgr::addUseCount(const _stItemModel* pModel,uint16 uCount)
{
	if (!pModel || !uCount || !canOperate())
		return false;

	_stItemUseCount& stUseCount = m_mapUseCount[pModel->uItemID];
	stUseCount.uItemID = pModel->uItemID;
	if (pModel->uMaxDayUseCount)
		stUseCount.uDayUseCount += uCount;
	if (pModel->uMaxWeekUseCount)
		stUseCount.uWeekUseCount += uCount;
	if (pModel->uMaxMonthUseCount)
		stUseCount.uMonthUseCount += uCount;

	//֪ͨ�ͻ���
	PBItemUseCount pbCount;
	stUseCount.saveToProtobuf(pbCount);
	return m_pPlayer->sendMsg(P_S2C_Protocol_Item,S2C_Item_UpUseCount,&pbCount);
}

//-------------------------------------------------------------
//------------------------------ 
void CGameItemMgr::clear()
{
	{	//��������
		ITER_MAP_ITEM _pos = m_mapBagItem.begin();
		for (; _pos != m_mapBagItem.end(); ++_pos)
		{
			Parent::releaseItem(_pos->second);
		}

		m_mapBagItem.clear();
	}

	{	//װ������
		ITER_MAP_EQUIP _pos = m_mapEquip.begin();
		for (; _pos != m_mapEquip.end(); ++_pos)
		{
			Parent::releaseItem(_pos->second);
		}
		m_mapEquip.clear();
	}

	{	//�ع�����
		ITER_MAP_ITEM _pos = m_mapSellItem.begin();
		for (; _pos != m_mapSellItem.end(); ++_pos)
		{
			Parent::releaseItem(_pos->second);
		}
		m_mapSellItem.clear();
	}
	m_mapUseCount.clear();
	m_dmapBingfu.clear();
	m_setExpBingfu.clear();
	m_stMakeVigour.init();
	m_mpOEMs.clear();
	m_mpSkilled.clear();
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameItemMgr::load(const PBPlayerBag& pbBag)
{
	if (!canOperate())
		return false;

	this->clear();
	for (uint16 i = 0; i < pbBag.baglist_size(); ++i)
	{
		const PBItem& pbItem = pbBag.baglist(i);
		addPBItem(pbItem,ItemListType_Bag);
	}
	for (uint16 i = 0; i < pbBag.equiplist_size(); ++i)
	{
		const PBItem& pbItem = pbBag.equiplist(i);
		addPBItem(pbItem,ItemListType_Equip);
	}
	for (uint16 i = 0; i < pbBag.usecount_size(); ++i)
	{
		const PBItemUseCount& pbItem = pbBag.usecount(i);
		_stItemUseCount stUseCount;
		stUseCount.loadFromProtobuf(pbItem);
		m_mapUseCount.insert_(stUseCount.uItemID,stUseCount);
	}
	//����������
	for (int32 i = 0; i< pbBag.skilled_size(); ++i)
	{
		const PBEquipMakeSkilled& pb = pbBag.skilled(i);
		_stEquipMakeSkilled  st;
		st.init();
		st.load(pb);
		m_mpSkilled.insert_(st.uEquipType, st);
	}
	//����
	m_stMakeVigour.load(pbBag.vigour());
	//������Ϣ
	for (int32 i = 0; i < pbBag.oems_size(); ++i)
	{
		const PBEquipMakeOEM & pb = pbBag.oems(i);
		_stEquipMakeOEM st;
		st.init();
		st.load(pb);
		m_mpOEMs.insert_(st.uMakeID, st);
	}
	//��������
	this->calculateAttri();
	
	return true;
}

//-------------------------------------------------------------
//------------------------------ �������ݿ��ȡ�ĵ���
bool	CGameItemMgr::addPBItem(const PBItem& pbItem,_em_ItemListType emType)
{
	if (!canOperate())
		return false;
	_stItem stItem;
	stItem.loadFromProtobuf(pbItem);
	const _stItemModel* pItemModel = g_clItemData.find(stItem.uItemID);
	CHECKF(pItemModel != NULL);
	CGameItem* pGameItem =Parent::buildItem(m_pPlayer,stItem,pItemModel, DoingType_Others);
	CHECKF(pGameItem != NULL);
	if (ItemListType_Bag == emType)
	{
		insertGameItem(pGameItem);
	}
	else if (ItemListType_Equip == emType)
	{
		if (pItemModel->uType != ItemType_Equip)
		{
			CLog::error("player(%d),itemid(%d)",m_pPlayer->getPlayerID(),pItemModel->uItemID);
		}
		else
		{
			m_mapEquip.insert_(pItemModel->uSubType,pGameItem);
		}
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameItemMgr::insertGameItem(CGameItem* pGameItem)
{
	if (!pGameItem || !pGameItem->getModel())
		return false;

	m_mapBagItem.insert_(pGameItem->getItemSN(),pGameItem);

	//���������
	if (ItemType_BingFu == pGameItem->getModel()->uType)
	{
		if (BingfuType_Exp == pGameItem->getModel()->uSubType)
		{
			m_setExpBingfu.insert(pGameItem);
		}
		else
		{
			m_dmapBingfu.insert(pGameItem->getItemID(),pGameItem->getItemSN(),pGameItem);
		}
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------ ɾ������
bool	CGameItemMgr::delGameItem	(CGameItem* pGameItem,bool bRelease/* = true*/)
{
	if (!pGameItem || !pGameItem->getModel())
		return false;

	if ( !findItem(pGameItem->getItemSN()))
	{
		CHECKF(pGameItem->getModel()->uType == ItemType_Equip);
		m_mapEquip.erase(pGameItem->getModel()->uSubType);
	}
	else
	{
		m_mapBagItem.erase(pGameItem->getItemSN());
		m_dmapBingfu.erase(pGameItem->getItemID(),pGameItem->getItemSN());
		m_setExpBingfu.erase(pGameItem);
	}

	if (bRelease)
		Parent::release(pGameItem->getItemSN());
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameItemMgr::save(PBPlayerBag* pbBag)
{
	pbBag->Clear();

	CGameItem* pGameItem = NULL;
	PBItem* pPBItem = NULL;

	{	//�汳������
		ITER_MAP_ITEM _pos = m_mapBagItem.begin();
		for (; _pos != m_mapBagItem.end(); ++_pos)
		{
			pGameItem = _pos->second;
			CHECKF(pGameItem != NULL && pGameItem->getModel() != NULL);
			pPBItem = pbBag->add_baglist();
			CHECKF(pPBItem != NULL);
			pGameItem->getItem().saveToProtobuf(*pPBItem,pGameItem->getModel()->check_mark(ItemMark_OfflineNoTime));
		}
	}

	{	//��װ������
		ITER_MAP_EQUIP _pos = m_mapEquip.begin();
		for (; _pos != m_mapEquip.end(); ++_pos)
		{
			pGameItem = _pos->second;
			CHECKF(pGameItem != NULL && pGameItem->getModel() != NULL);
			pPBItem = pbBag->add_equiplist();
			CHECKF(pPBItem != NULL);
			pGameItem->getItem().saveToProtobuf(*pPBItem,pGameItem->getModel()->check_mark(ItemMark_OfflineNoTime));
		}
	}

	//�����
	{
		MAP_ITEMUSECOUNT::iterator _pos = m_mapUseCount.begin();
		for (; _pos != m_mapUseCount.end(); ++_pos)
		{
			PBItemUseCount* pbCount = pbBag->add_usecount();
			CHECKF(pbCount != NULL);
			_pos->second.saveToProtobuf(*pbCount);
		}
	}

	//����������
	{
		MAP_SKILLED::iterator iter = m_mpSkilled.begin();
		for (; iter!= m_mpSkilled.end(); ++iter)
		{
			PBEquipMakeSkilled * pb = pbBag->add_skilled();
			if (pb)
			{
				iter->second.save(*pb);
			}
		}
	}
	//����
	{
		PBEquipMakeVigour* pPB = pbBag->mutable_vigour();
		if (pPB)
		{
			m_stMakeVigour.save(*pPB);
		}
	}
	//������Ϣ
	{
		MAP_OEM::iterator iter = m_mpOEMs.begin();
		for (; iter!= m_mpOEMs.end(); ++iter)
		{
			PBEquipMakeOEM * pb = pbBag->add_oems();
			if (pb)
			{
				iter->second.save(*pb);
			}
		}
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameItemMgr::saveToProbuff(PBPlayerEquipData* pbEquip)
{
	if (!pbEquip)
		return false;
	pbEquip->Clear();
	ITER_MAP_EQUIP _pos = m_mapEquip.begin();
	for (; _pos != m_mapEquip.end(); ++_pos)
	{
		CGameItem* pGameItem = _pos->second;
		if (!pGameItem)
			continue;
		PBItem* pItem = pbEquip->add_equip();
		if (!pItem)
			continue;

		pGameItem->getItem().saveToProtobuf(*pItem);
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameItemMgr::cleanupDaily()
{
	MAP_ITEMUSECOUNT::iterator _pos = m_mapUseCount.begin();
	for (; _pos != m_mapUseCount.end(); ++_pos)
	{
		_pos->second.uDayUseCount = 0;
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameItemMgr::cleanupWeekly()
{
	MAP_ITEMUSECOUNT::iterator _pos = m_mapUseCount.begin();
	for (; _pos != m_mapUseCount.end(); ++_pos)
	{
		_pos->second.uWeekUseCount = 0;
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameItemMgr::cleanupMonthly()
{
	MAP_ITEMUSECOUNT::iterator _pos = m_mapUseCount.begin();
	for (; _pos != m_mapUseCount.end(); ++_pos)
	{
		_pos->second.uMonthUseCount = 0;
	}
}
//-------------------------------------------------------------
//------------------------------ �ܷ����ӵ��� ��������
bool	CGameItemMgr::canAddItem(const _stItemModel* pItemModel,uint16 uItemNum)
{
	CHECKF(pItemModel != NULL && pItemModel->uMaxCount > 0);
	//�����µĵ���
	uint16 uNum = (uint16)ceil((float32)uItemNum / pItemModel->uMaxCount);
	
	if (ItemType_BingFu == pItemModel->uType)
	{
		uint16 uMaxBagNum = g_clConstantData.Game_MaxBingfuBagNum;
		if ( uNum + getBingfuBagNum() > uMaxBagNum)
			return false;
	}
	else
	{
		uint16 uMaxBagNum = g_clConstantData.Game_MaxBagItemNum;
		if ( uNum + getItemBagNum() > uMaxBagNum)
			return false;
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------
bool CGameItemMgr::canAddItem(uint32 uItemID, uint16 uItemNum)
{
	const _stItemModel* pItemModel = g_clItemData.find(uItemID);
	CHECKF(pItemModel != NULL && uItemNum > 0 && pItemModel->uMaxCount > 0);
	return canAddItem(pItemModel,uItemNum); 
}

//-------------------------------------------------------------
//------------------------------ ���������Եĵ���
bool	CGameItemMgr::addItem(const _stItemBuild& stItemBuild,_emDoingType emDoing)
{
	if (!canOperate())
		return false;

	if (!addItem(stItemBuild.uItemID,stItemBuild.uItemNum,true,true,true,emDoing))
		return false;

	return true;
}

//-------------------------------------------------------------
//------------------------------ ��������Ʒ ���ܼӵķ��ʼ�
bool	CGameItemMgr::addItem	(const DMAP_NEED_ITEM* pmapItem,bool bSend,_emDoingType emDoing)
{
	if (!canOperate() || !pmapItem )
		return false;

	//�ӵ���
	DMAP_NEED_ITEM dmapMailItem;
	CONST_ITER_DMAP_NEED_ITEM _pos = pmapItem->begin();
	for (; _pos != pmapItem->end(); ++_pos)
	{
		const DMAP_NEED_ITEM::_sub_map& stSubMap = _pos->second;
		DMAP_NEED_ITEM::sub_const_iterator _spos = stSubMap.begin();
		for (; _spos != stSubMap.end(); ++_spos)
		{
			if (!addItem(_pos->first,_spos->first,0 == _spos->second,bSend,false,emDoing))
			{
				dmapMailItem.insert(_pos->first,_spos->first,_spos->second);
			}
		}
	}
	
	//ʣ�µķ��ʼ�
	if (dmapMailItem.get_total_size() > 0)
		m_pPlayer->sendMail(MailType_FullBag,&dmapMailItem,emDoing);

	return true;
}
//-------------------------------------------------------------
//------------------------------ ��������Ʒ ���ܼӵķ��ʼ�
bool	CGameItemMgr::addItem	(const MAP_NEED_ITEM * pmapItem,bool bBind,bool bSend,_emDoingType emDoing)
{
	if (!canOperate() || !pmapItem )
		return false;

	//�ӵ���
	MAP_NEED_ITEM dmapMailItem;
	CONST_ITER_MAP_NEED_ITEM _pos = pmapItem->begin();
	for (; _pos != pmapItem->end(); ++_pos)
	{
		if (!addItem(_pos->first,_pos->second,bBind,bSend,false,emDoing))
		{
			dmapMailItem.insert_(_pos->first,_pos->second);
		}
	}

	//ʣ�µķ��ʼ�
	DMAP_NEED_ITEM DmapAddItem ;
	auto _pos1 =  pmapItem->begin();
	for (; _pos1 != pmapItem->end(); ++_pos1)
	{
		uint8 uBind= (bBind ? 1: 0);
		DmapAddItem.insert(_pos1->first, _pos1->second,uBind);
	}
	m_pPlayer->sendMail(MailType_FullBag,&DmapAddItem, emDoing);		
	return true;
}
//-------------------------------------------------------------
//------------------------------ ����������Ʒ ���ǵ���
bool	CGameItemMgr::addItem(uint32 uItemID,uint16 uItemNum,bool bBind,bool bSend, bool bMail,_emDoingType emDoing, bool bForceEmail, _emMailType emMailType )
{
	const _stItemModel* pItemModel = g_clItemData.find(uItemID);
	CHECKF(pItemModel != NULL && uItemNum > 0 && pItemModel->uMaxCount > 0);

	//if (bForceEmail)
	//{
	//	PBItem pItem;
	//	pItem.set_itemid(uItemID);
	//	pItem.set_itemnum(uItemNum);
	//	pItem.set_bingfuexp(pItemModel->uBingfuExp);
	//	uint32 uFlag = pItem.flag();
	//	_SET_BIT32(uFlag,ItemFlag_Binded,bBind);	
	//	pItem.set_flag(uFlag);
	//	return m_pPlayer->sendMail(emMailType,&pItem, emDoing);		
	//}

	//�ֱ��жϱ����Ƿ�����
	if (!canAddItem(pItemModel,uItemNum) ) 
	{
		if (!bMail)  //���ʼ�	
			return false;
		
		uint16 MinNum = MIN(uItemNum, (uint16)g_clConstantData.Game_MailItemMax);
		if (pItemModel->uMaxCount > 1) // ���Ե���
		{
			uint8 nBind = (bBind ? 1: 0);
			DMAP_NEED_ITEM DmapAddItem;

			DmapAddItem.insert(uItemID, MinNum, nBind);
			return m_pPlayer->sendMail(MailType_FullBag,&DmapAddItem, emDoing);		
		}
		else 
		{
			
			for (uint16 i=0; i< MinNum; i++)
			{
				DMAP_NEED_ITEM DmapAddItem;
				uint8 nBind = (bBind ? 1: 0);
				DmapAddItem.insert(uItemID, 1, nBind);
				m_pPlayer->sendMail(MailType_FullBag,&DmapAddItem, emDoing);
			}
			return true;
		}
	}

	//���ӵ�������Ʒ��
	if (pItemModel->uMaxCount > 1)
	{	
		auto _pos = m_mapBagItem.begin();
		auto _end = m_mapBagItem.end();
		for ( ; _pos != _end ; ++_pos )
		{
			CGameItem* pGameItem = _pos->second;
			if (!pGameItem || pGameItem->getItemID() != uItemID )
				continue;

			if (pGameItem->isExpireItem() || pGameItem->isBind() != bBind || pGameItem->getBingfuExp() != pItemModel->uBingfuExp)
				continue;

			if (pItemModel->uMaxCount <= pGameItem->getItemNum())
				continue;
			uint16 uAddNum = MIN(uint16(pItemModel->uMaxCount - pGameItem->getItemNum()),uItemNum);
			uItemNum -= uAddNum;
			updateItemNum(pGameItem,uAddNum,bSend,emDoing);
			if (!uItemNum)
				return true;
		}
	}

	if (!uItemNum)
		return true;

	//�����µĵ���
	uint16 uNum = (uint16)ceil((float32)uItemNum / pItemModel->uMaxCount);
	for (uint16 i = 0; i < uNum; ++i)
	{
		uint32 uAddNum = MIN(uItemNum,pItemModel->uMaxCount);
		uItemNum -= uAddNum;
		addNewItem(pItemModel,uAddNum,bBind,bSend,emDoing);
		if (!uItemNum)
			return true;
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool	CGameItemMgr::addItem (const PBItem& pbItem,bool bSend,_emDoingType emDoing)
{
	if (!canOperate())
		return false;

	_stItem stItem;
	stItem.loadFromProtobuf(pbItem);
	
	//�������� ���ʼ�
	if (getBagSpare() <= 0)
	{
		DMAP_NEED_ITEM dmapNeedItem;
		dmapNeedItem.insert(stItem.uItemID, stItem.uItemNum, stItem.testFlag(ItemBindType_Bind) ? 0 : 1);
		return m_pPlayer->sendMail(MailType_FullBag,&dmapNeedItem, emDoing);
	}

	//ǿ����������sn
	stItem.uItemSN = Parent::buildItemSN(m_pPlayer->getPlayerID(),m_pPlayer->getItemSequence());
	const _stItemModel* pItemModel = g_clItemData.find(stItem.uItemID);
	CHECKF(pItemModel != NULL);

	CGameItem* pGameItem = Parent::buildItem(m_pPlayer,stItem,pItemModel, emDoing);
	CHECKF(pGameItem != NULL);
	insertGameItem(pGameItem);

	//��־����
	g_game_log->item_log(m_pPlayer->getPlayerID(),emDoing,pGameItem->getItem());

	//֪ͨ�ͻ���
	if (bSend)
		send_AddItemAck(stItem, emDoing);

	uint32 uItemID = pGameItem->getItemID();
	//CPlayerQuestMgr* pQuestMgr = m_pPlayer->getPlayerQuestMgr();
	//if(pQuestMgr)
	//{
	//	pQuestMgr->callback_updateItemNum(uItemID);
	//}

	return true;
}
//-------------------------------------------------------------
//------------------------------ ����װ�� ֱ�Ӵ���
bool	CGameItemMgr::addEquipItem (uint32 uItemID,_emDoingType emDoing)
{
	const _stItemModel* pItemModel = g_clItemData.find(uItemID);
	CHECKF(pItemModel != NULL);
	CHECKF(pItemModel->uType == ItemType_Equip );
	CGameItem* pGameItem = addNewItem(pItemModel,1,true,false,emDoing);
	CHECKF(pGameItem != NULL);
	uint8 uCode = 0;
	return equipItem(pGameItem->getItemSN(),pItemModel->uSubType,uCode,false);
}

//-------------------------------------------------------------
//------------------------------ �����µĵ��� ������
CGameItem* 	CGameItemMgr::addNewItem(const _stItemModel* pItemModel,uint16 uItemNum,bool bBind,bool bSend,_emDoingType emDoing)
{
	if (!canOperate())
		return NULL;
	CHECKN(pItemModel != NULL && uItemNum > 0 && pItemModel->uMaxCount > 0);
	_stItem stItem;
	stItem.uItemID = pItemModel->uItemID;
	stItem.uItemNum = uItemNum;
	stItem.uBingfuExp = pItemModel->uBingfuExp;
	stItem.setFlag(ItemFlag_Binded,bBind);
	return addNewItem(stItem,bSend,emDoing);
}
//-------------------------------------------------------------
//------------------------------ �����µĵ��� ������
CGameItem* 	CGameItemMgr::addNewItem(_stItem& stItem,bool bSend,_emDoingType emDoing)
{
	if (!canOperate() || stItem.uItemSN)
		return NULL;

	const _stItemModel* pItemModel = g_clItemData.find(stItem.uItemID);
	CHECKN(pItemModel != NULL && stItem.uItemNum > 0 );
	CGameItem* pGameItem = Parent::buildItem(m_pPlayer,stItem,pItemModel, emDoing);
	CHECKN(pGameItem != NULL);

	//�Ƿ��ÿ�ʼ��
	if (pItemModel->check_mark(ItemMark_GetBind))
		pGameItem->getItem().setFlag(ItemFlag_Binded,true);

	//�Ƿ��ÿ�ʼ����ʱ
	if (pItemModel->uExpireTime && pItemModel->check_mark(ItemMark_GetTimeLimit))
		pGameItem->getItem().uEndtime = getTime() + pItemModel->uExpireTime;

	//����
	insertGameItem(pGameItem);

	//֪ͨ�ͻ���
	if (bSend)
		send_AddItemAck(pGameItem->getItem(), emDoing);

	//��־����
	g_game_log->item_log(m_pPlayer->getPlayerID(),emDoing,pGameItem->getItem());

	uint32 uItemID = pGameItem->getItemID();
	//CPlayerQuestMgr* pQuestMgr = m_pPlayer->getPlayerQuestMgr();
	//if(pQuestMgr)
	//{
	//	pQuestMgr->callback_updateItemNum(uItemID);
	//}

	return pGameItem;
}

//-------------------------------------------------------------
//------------------------------
uint32	CGameItemMgr::getItemCount	(uint32 uItemID,_em_ItemBindType emType/* = ItemBindType_All*/)
{
	return getItemCount(g_clItemData.find(uItemID),emType);
}
//-------------------------------------------------------------
//------------------------------ ��ȡ���ߵĸ���
uint32	CGameItemMgr::getItemCount	(const _stItemModel* pItemModel,_em_ItemBindType emType/* = ItemBindType_All*/)
{
	CHECKF(pItemModel != NULL);
	uint32 uTotal = 0;
	auto _pos = m_mapBagItem.begin();
	auto _end = m_mapBagItem.end();
	for ( ; _pos != _end ; ++_pos )
	{
		CGameItem* pGameItem = _pos->second;
		if (!pGameItem || pGameItem->getItemID() != pItemModel->uItemID )
			continue;

		switch (emType)
		{
		case ItemBindType_All:
			{
				uTotal += pGameItem->getItemNum();
			}
			break;
		case ItemBindType_Bind:
			{
				if (pGameItem->isBind())
					uTotal += pGameItem->getItemNum();
			}
			break;
		case ItemBindType_NoBind:
			{
				if (!pGameItem->isBind())
					uTotal += pGameItem->getItemNum();
			}
			break;
		default:
			break;
		}
	}
	return uTotal;
}
//-------------------------------------------------------------
//------------------------------
uint32		CGameItemMgr::getBagSpare		()
{
	uint32 uMaxBagNum = g_clConstantData.Game_MaxBagItemNum;
	uint32 uCurBagNum = getItemBagNum();
	if (uCurBagNum >= uMaxBagNum)
		return 0;

	return uMaxBagNum - uCurBagNum;
}
//-------------------------------------------------------------
//------------------------------
uint32		CGameItemMgr::getBingfuBagNum		()
{
	return m_dmapBingfu.get_total_size();
}
//-------------------------------------------------------------
//------------------------------
uint32		CGameItemMgr::getItemBagNum		()
{
	return m_mapBagItem.size() - m_dmapBingfu.get_total_size();
}
//-------------------------------------------------------------
//------------------------------ �Ƿ����Ƴ�����
bool	CGameItemMgr::canRemoveItem	(const _stItemModel* pItemModel,uint16 uItemNum,_em_ItemBindType emType,uint8& uCode)
{
	CHECKF(pItemModel != NULL);
	if (uItemNum > getItemCount(pItemModel, emType))
	{
		uCode = R_ResultItem_NotEnough;
		return false;
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool	CGameItemMgr::canRemoveItem	(uint32 uItemID,uint16 uItemNum,_em_ItemBindType emType)
{
	const _stItemModel* pItemModel = g_clItemData.find(uItemID);
	CHECKF(pItemModel != NULL && uItemNum > 0 );

	uint8 uCode;
	return canRemoveItem(pItemModel,uItemNum,emType,uCode);
}
//-------------------------------------------------------------
//------------------------------
bool	CGameItemMgr::canRemoveItem	(const DMAP_NEED_ITEM& mapNeedItem)
{
	CONST_ITER_DMAP_NEED_ITEM _pos = mapNeedItem.begin();
	for (; _pos != mapNeedItem.end(); ++_pos)
	{
		const DMAP_NEED_ITEM::_sub_map& stSubMap = _pos->second;
		DMAP_NEED_ITEM::sub_const_iterator _spos = stSubMap.begin();
		for (; _spos != stSubMap.end(); ++_spos)
		{
			if ( !canRemoveItem(_pos->first,_spos->first,(_em_ItemBindType)_spos->second) )
				return false;
		}
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool	CGameItemMgr::canRemoveItem	(const DMAP_NEED_ITEM& mapNeedItem,_em_ItemBindType emType/* = ItemBindType_All*/)
{
	CONST_ITER_DMAP_NEED_ITEM _pos = mapNeedItem.begin();
	for (; _pos != mapNeedItem.end(); ++_pos)
	{
		const DMAP_NEED_ITEM::_sub_map& stSubMap = _pos->second;
		DMAP_NEED_ITEM::sub_const_iterator _spos = stSubMap.begin();
		for (; _spos != stSubMap.end(); ++_spos)
		{
			if ( !canRemoveItem(_pos->first,_spos->first,emType))
				return false;
		}
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool	CGameItemMgr::canRemoveItem	(const MAP_NEED_ITEM & mapNeedItem,_em_ItemBindType emType/* = ItemBindType_All*/)
{
	CONST_ITER_MAP_NEED_ITEM _pos = mapNeedItem.begin();
	for (; _pos != mapNeedItem.end(); ++_pos)
	{
		if ( !canRemoveItem(_pos->first,_pos->second,emType))
			return false;
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool	CGameItemMgr::removeItem		(const DMAP_NEED_ITEM& mapNeedItem,_em_ItemBindType emType/* = ItemBindType_All*/,_emDoingType emDoing)
{
	if ( !canRemoveItem(mapNeedItem,emType) )
		return false;

	uint8 uCode1;
	CONST_ITER_DMAP_NEED_ITEM _pos = mapNeedItem.begin();
	for (; _pos != mapNeedItem.end(); ++_pos)
	{
		const DMAP_NEED_ITEM::_sub_map& stSubMap = _pos->second;
		DMAP_NEED_ITEM::sub_const_iterator _spos = stSubMap.begin();
		for (; _spos != stSubMap.end(); ++_spos)
		{
			removeItem(_pos->first,_spos->first,true,uCode1,(_em_ItemBindType)_spos->second,emDoing);
		}
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool	CGameItemMgr::removeItem		(const MAP_NEED_ITEM& mapNeedItem,_em_ItemBindType emType/* = ItemBindType_All*/,_emDoingType emDoing)
{
	if ( !canRemoveItem(mapNeedItem,emType) )
		return false;

	uint8 uCode1;
	CONST_ITER_MAP_NEED_ITEM _pos = mapNeedItem.begin();
	for (; _pos != mapNeedItem.end(); ++_pos)
	{
		removeItem(_pos->first,_pos->second,true,uCode1,emType,emDoing);
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------
static bool  _sort_fun_remove_ (CGameItem* left, CGameItem* right)
{
	if ( !left )
		return false;

	if ( !right )
		return true;

	// ʱЧ����ɾ��
	if ( left->isExpireItem() != right->isExpireItem() )
		return left->isExpireItem();

	//����ʱЧ�� ��ʱ��
	if (left->isExpireItem() && left->getItem().uEndtime != right->getItem().uEndtime )
		return left->getItem().uEndtime < right->getItem().uEndtime;
	// �ٰ󶨵�ɾ��
	if ( left->isBind() != right->isBind() )
		return left->isBind();

	return left->getItemSN() < right->getItemSN();
}
//-------------------------------------------------------------
//------------------------------ �Ƴ�ĳһ�ֵ���ָ���ĸ���
bool	CGameItemMgr::removeItem(uint32 uItemID,uint16 uItemNum,bool bSend,uint8& uCode,\
								 _em_ItemBindType emType,_emDoingType emDoing)
{
	const _stItemModel* pItemModel = g_clItemData.find(uItemID);
	CHECKF(pItemModel != NULL && uItemNum > 0 && pItemModel->uMaxCount > 0);

	stl_vector<CGameItem*> vec_item;
	uint32 uTotal = 0; // ����canRemoveItem������һ�� û�б�Ҫ
	{//�ռ�����
		auto _pos = m_mapBagItem.begin();
		auto _end = m_mapBagItem.end();
		for ( ; _pos != _end ; ++_pos )
		{
			CGameItem* pGameItem = _pos->second;
			if (!pGameItem || pGameItem->getItemID() != pItemModel->uItemID )
				continue;

			switch (emType)
			{
			case ItemBindType_All:
				{
					uTotal += pGameItem->getItemNum();
					vec_item.push_back(pGameItem);
				}
				break;
			case ItemBindType_Bind:
				{
					if (pGameItem->isBind())
					{
						uTotal += pGameItem->getItemNum();
						vec_item.push_back(pGameItem);
					}
				}
				break;
			case ItemBindType_NoBind:
				{
					if (!pGameItem->isBind())
					{
						uTotal += pGameItem->getItemNum();
						vec_item.push_back(pGameItem);
					}
				}
				break;
			default:
				break;
			}
		}
	}

	if (uTotal < uItemNum )
	{
		uCode = R_ResultItem_NotEnough;
		return false;
	}

	if (vec_item.empty())
		return false;

	// ����
	if (vec_item.size() > 1 )
		std::sort(vec_item.begin(),vec_item.end(),_sort_fun_remove_ );

	{
		auto _pos = vec_item.begin();
		auto _end = vec_item.end();
		for ( ; _pos != _end ; ++_pos )
		{
			CGameItem* pGameItem = *_pos;
			if (!pGameItem)
				continue;

			uint16 uDelNum = MIN(pGameItem->getItemNum(),uItemNum);
			uItemNum -= uDelNum;
			updateItemNum(pGameItem,(int32)uDelNum * -1,true,emDoing);
			if (0 == uItemNum)
				break;
		}
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------
bool CGameItemMgr::findByItemID(uint32 uItemID, stl_vector<uint64> &vecItemSN)
{
	stl_vector<uint64> vec_item;
	auto _pos = m_mapBagItem.begin();
	auto _end = m_mapBagItem.end();
	for ( ; _pos != _end ; ++_pos )
	{
		CGameItem* pGameItem = _pos->second;
		if (!pGameItem || pGameItem->getItemID() != uItemID )
			continue;
	
		vecItemSN.push_back(pGameItem->getItemSN());
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------ �Ƴ�ָ���ĵ���ָ���ĸ���
bool	CGameItemMgr::removeItem(uint64 uItemSN,uint16 uItemNum,bool bSend,uint8& uCode,_em_ItemBindType emType,_emDoingType emDoing)
{
	if (!canOperate())
		return false;

	CGameItem* pGameItem = findItem(uItemSN);
	CHECKF(pGameItem != NULL);
	if (pGameItem->getItemNum() < uItemNum)
	{
		uCode = R_ResultItem_NotEnough;
		return false;
	}

	//���µ��ߵĸ���
	updateItemNum(pGameItem,(int32)uItemNum * -1,bSend,emDoing);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameItemMgr::canRemoveBingfu	(const DMAP_NEED_BINGFU& dmapNeedBingfu,_em_ItemBindType emType)
{
	CONST_ITER_DMAP_NEED_BINGFU _pos = dmapNeedBingfu.begin();
	for (; _pos != dmapNeedBingfu.end(); ++_pos)
	{
		DMAP_NEED_BINGFU::sub_const_iterator _spos = _pos->second.begin();
		for (; _spos != _pos->second.end(); ++_spos)
		{
			if (!canRemoveBingfu(_pos->first,_spos->first,_spos->second,emType))
			{
				return false;
			}
		}
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameItemMgr::canRemoveBingfu	(uint32 uBingfuID,uint16 uBingfuLevel,uint16 uBingfuNum,_em_ItemBindType emType)
{
	DMAP_TYPE_BINGFU::_sub_map* pSubmap = m_dmapBingfu.find_(uBingfuID);
	if (!pSubmap)
		return false;

	DMAP_TYPE_BINGFU::sub_iterator _pos = pSubmap->begin();
	for (; _pos != pSubmap->end(); ++_pos)
	{
		CGameItem* pGameItem = _pos->second;
		if (!pGameItem || pGameItem->getBingfuLevel() != uBingfuLevel)
			continue;

		if (emType == ItemBindType_NoBind && pGameItem->isBind())
			continue;

		uint16 uNeedItem = MIN(pGameItem->getItemNum(),uBingfuNum);
		uBingfuNum -= uNeedItem;
		if (!uBingfuNum)
			break;
	}
	return uBingfuNum == 0;
}
//-------------------------------------------------------------
//------------------------------ �Ƴ�����
bool	CGameItemMgr::removeBingfu	(const DMAP_NEED_BINGFU& dmapNeedBingfu,_em_ItemBindType emType,_emDoingType emDoing)
{
	CONST_ITER_DMAP_NEED_BINGFU _pos = dmapNeedBingfu.begin();
	for (; _pos != dmapNeedBingfu.end(); ++_pos)
	{
		DMAP_NEED_BINGFU::sub_const_iterator _spos = _pos->second.begin();
		for (; _spos != _pos->second.end(); ++_spos)
		{
			removeBingfu(_pos->first,_spos->first,_spos->second,emType,emDoing);
		}
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ �Ƴ�����
bool	CGameItemMgr::removeBingfu(uint32 uBingfuID,uint16 uBingfuLevel,uint16 uBingfuNum,_em_ItemBindType emType,_emDoingType emDoing)
{
	if (!canOperate())
		return false;

	DMAP_TYPE_BINGFU::_sub_map* pSubmap = m_dmapBingfu.find_(uBingfuID);
	if (!pSubmap)
		return false;

	stl_map<uint64,uint16> mapExpendItem;
	DMAP_TYPE_BINGFU::sub_iterator _pos = pSubmap->begin();
	for (; _pos != pSubmap->end(); ++_pos)
	{
		CGameItem* pGameItem = _pos->second;
		if (!pGameItem || pGameItem->getBingfuLevel() != uBingfuLevel)
			continue;

		if (emType == ItemBindType_NoBind && pGameItem->isBind())
			continue;

		uint16 uNeedItem = MIN(pGameItem->getItemNum(),uBingfuNum);
		uBingfuNum -= uNeedItem;
		mapExpendItem.insert_(pGameItem->getItemSN(),uNeedItem);
		if (!uBingfuNum)
			break;
	}

	if (uBingfuNum > 0)
		return false;

	uint8 uCode = 0;
	{//�۵���
		stl_map<uint64,uint16>::iterator _spos = mapExpendItem.begin();
		for (; _spos != mapExpendItem.end(); ++_spos)
		{
			removeItem(_spos->first,_spos->second,true,uCode,emType,emDoing);
		}
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------ �Ƴ�ָ���ĵ���
bool	CGameItemMgr::removeItem(uint64 uItemSN,bool bSend,uint8& uCode,_emDoingType emDoing)
{
	if (!canOperate())
		return false;

	CGameItem* pGameItem = findItem(uItemSN);
	CHECKF(pGameItem != NULL);

	//���µ��ߵĸ���
	updateItemNum(pGameItem,(int32)pGameItem->getItemNum() * -1,bSend,emDoing);
	return true;
}

//-------------------------------------------------------------
//------------------------------ �Ƿ���ʹ�õ���
bool	CGameItemMgr::canUseItem	(CGameItem* pGameItem,uint16 uItemNum,uint8& uCode)
{
	if (!canOperate())
		return false;

	CHECKF(pGameItem != NULL);
	const _stItemModel* pModel = pGameItem->getModel();
	CHECKF(pModel != NULL);

	//�Ƿ���ʹ���ڼ�
	if (!pModel->isUseTimeLimit((uint64)getTime()))
	{
		uCode = R_ResultItem_NoTimeLimit;
		return false;
	}

	//���ȼ�
	if (pModel->uLevel > m_pPlayer->getLevel())
	{
		uCode = R_ResultItem_Level_Low;
		return false;
	}

	//���CD
	if ((pModel->uCD || pModel->uGroupCD) && (m_pPlayer->getCooling().isCooling(CoolingType_Item,pModel->uItemID,pModel->uCDGroupID)))
	{
		uCode = R_ResultItem_CD;
		return false;
	}

	//������
	if (uItemNum > pGameItem->getItemNum())
	{
		uCode = R_ResultItem_NotEnough;
		return false;
	}

	//ʹ�ô�������
	if (pModel->uMaxDayUseCount || pModel->uMaxWeekUseCount || pModel->uMaxMonthUseCount)
	{
		_stItemUseCount* pUseCount = findUseCount(pModel->uItemID);
		if (pUseCount)
		{
			if (pModel->uMaxDayUseCount && pUseCount->uDayUseCount + uItemNum > pModel->uMaxDayUseCount)
			{
				uCode = R_ResultItem_NoDayUseCount;
				return false;
			}
			if (pModel->uMaxWeekUseCount && pUseCount->uWeekUseCount  + uItemNum > pModel->uMaxWeekUseCount)
			{
				uCode = R_ResultItem_NoWeekUseCount;
				return false;
			}
			if (pModel->uMaxMonthUseCount && pUseCount->uMonthUseCount + uItemNum > pModel->uMaxMonthUseCount)
			{
				uCode = R_ResultItem_NoMonthUseCount;
				return false;
			}
		}
		else
		{
			if (pModel->uMaxDayUseCount && uItemNum > pModel->uMaxDayUseCount)
			{
				uCode = R_ResultItem_NoDayUseCount;
				return false;
			}
			if (pModel->uMaxWeekUseCount && uItemNum > pModel->uMaxWeekUseCount)
			{
				uCode = R_ResultItem_NoWeekUseCount;
				return false;
			}
			if (pModel->uMaxMonthUseCount && uItemNum > pModel->uMaxMonthUseCount)
			{
				uCode = R_ResultItem_NoMonthUseCount;
				return false;
			}
		}
	}
	
	
	//ǰ������
	if (!m_pPlayer->canPreCondition(pGameItem->getModel()->dmapUseCondition,uItemNum))
	{
		uCode = R_ResultItem_PreCondition;
		return false;
	}

	//ʹ������
	switch (pModel->uUseType)
	{
	case ItemUseType_Expend:
		{
			if (pModel->vecFunValue.size() < 2)
			{
				uCode = R_ResultItem_ParamNumError;
				return false;
			}

			if (!m_pPlayer->haveState(StateType_Plane))
			{
				if (m_pPlayer->isDead())
				{
					uCode = R_ResultItem_DeadNoUse;
					return false;
				}

				if (m_pPlayer->isExpendFull(pModel->vecFunValue[0]))
				{
					uCode = R_ResultItem_ExpendFull;
					return false;
				}
			}

			if (pModel->vecFunValue[0] == ExpendType_HP &&
				 m_pPlayer->testRegion(RegionMark_CntHP))
			{
				uCode = R_ResultItem_NotUserHP;
				return false;
			}

			//��������,����������淨��,������ʹ�ü�HP��ҩˮ
			if (m_pPlayer->getGameCopymap()
				&& m_pPlayer->getGameCopymap()->getCopymapType() == CopymapType_ClimbTower
				&& pModel->vecFunValue.size()
				&& pModel->vecFunValue[0] == ExpendType_HP)
			{
				return false;
			}
		}
		break;
	case ItemUseType_Buff:
		{
			if (m_pPlayer->isDead())
			{
				uCode = R_ResultItem_DeadNoUse;
				return false;
			}
		}
		break;
	case ItemUseType_Transfer:
		{
			if (m_pPlayer->isDead())
			{
				uCode = R_ResultItem_DeadNoUse;
				return false;
			}
		}
		break;
	case ItemUseType_Treasure:
		{
			if (uItemNum != 1)
			{
				uCode = R_ResultItem_UseNumError;
				return false;
			}
		}
		break;
	case ItemUseType_Fashion:
		{
			if (uItemNum != 1)
			{
				uCode = R_ResultItem_UseNumError;
				return false;
			}

			//CPlayerFashion* pPlayerFashion = (CPlayerFashion*)m_pPlayer->getPlayerFun(GameSystemType_Fashion);
			//if (!pPlayerFashion || 1 != pModel->vecFunValue.size() )
			//	return false;

			//if (pPlayerFashion->hasFashion(pModel->vecFunValue[0]))
			//{
			//	uCode = R_ResultItem_HaveFashion;
			//	return false;
			//}
		}
	default:
		break;
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ ʹ�õ���
bool	CGameItemMgr::useItem(uint64 uItemSN,int16 uItemNum,uint8& uCode)
{
	if (!canOperate())
		return false;

	CHECKF(uItemNum > 0);
	CGameItem* pGameItem = findItem(uItemSN);
	CHECKF(pGameItem != NULL);
	const _stItemModel* pModel = pGameItem->getModel();
	CHECKF(pModel != NULL);

	//�ܷ�ʹ�õ���
	if (!canUseItem(pGameItem,uItemNum,uCode))
		return false;

	//��CD
	if (pModel->uCD || pModel->uGroupCD)
		m_pPlayer->getCooling().addCooling(CoolingType_Item,pModel->uItemID,pModel->uCD,pModel->uCDGroupID,pModel->uGroupCD);

	//�Ӵ���
	if (pModel->uMaxDayUseCount || pModel->uMaxWeekUseCount || pModel->uMaxMonthUseCount)
	{
		addUseCount(pModel,uItemNum);
	}

	//�Ӽ�����
	m_pPlayer->addPreCondition(pModel->dmapUseCondition,PreConditionType_Count,uItemNum);

	//ʹ��Ч��
	bool bResult = false;
	if (m_fnUseItem[pModel->uUseType])
	{
		bResult =  (this->*m_fnUseItem[pModel->uUseType])(pGameItem,uItemNum);
		if (!bResult)
			CLog::error("use item fail!itemid[%d],itemnum[%d]",pModel->uItemID,uItemNum);
	}

	//�۳�����
	//CPlayerQuestMgr* pQuestMgr = m_pPlayer->getPlayerQuestMgr();
	//if ( pQuestMgr )
	//	pQuestMgr->callback_useItem(pGameItem->getItemID(),uItemNum);

	m_pPlayer->getGameSkillMgr().callbackUseItem(pGameItem->getModel());

	send_UseItemAck(uItemSN,uItemNum,R_ResultItem_Succeed);

	updateItemNum(pGameItem,(int32)uItemNum * -1,true,DoingType_Item_Use);
	return bResult;
}

//-------------------------------------------------------------
//------------------------------ ����/����װ��
bool	CGameItemMgr::equipItem(uint64 uItemSN,uint8 uEquipType,uint8& uCode,bool bSend/* = true*/)
{
	if (!canOperate())
		return false;

	//���ڸ����в��ܻ�װ��
	if (m_pPlayer->isInCopymap() 
		&& m_pPlayer->getSceneMap() 
		&& m_pPlayer->getSceneMap()->getCopymapModel()
		&& m_pPlayer->getSceneMap()->getCopymapModel()->uType == CopymapType_DartRob)
	{
		uCode = R_ResultItem_DartRobCannotEquip;
		return false;
	}

	//����в��ܻ�����
	if (EquipType_Mount == uEquipType && m_pPlayer->getMountID() > 0)
	{
		uCode = R_ResultItem_MountingNoChange;
		return false;
	}

	//����װ��
	CGameItem* pEquipItem = findEquip(uEquipType);
	if (pEquipItem)
	{
		m_mapEquip.erase(uEquipType);
		insertGameItem(pEquipItem);
	}

	//����
	CGameItem* pGameItem = findItem(uItemSN);
	if (pGameItem)
	{
		const _stItemModel* pModel = pGameItem->getModel();
		CHECKF(pModel != NULL);
		CHECKF(ItemType_Equip == pModel->uType);
		CHECKF(uEquipType == pModel->uSubType);
		//���ȼ�
		if (pModel->uLevel > m_pPlayer->getLevel())
		{
			uCode = R_ResultItem_Level_Low;
			return false;
		}
		if (!m_pPlayer->canPreCondition(pModel->dmapUseCondition))
		{
			uCode = R_ResultItem_PreCondition;
			return false;
		}
		//ְҵ����
		pGameItem->getItem().setFlag(ItemFlag_Binded,true);
		delGameItem(pGameItem,false);
		m_mapEquip.insert_(uEquipType,pGameItem);
	}

	//ˢ�����Ը�����  ���ﲻ�ı�����
	if (EquipType_Mount != uEquipType)
	{
		this->calculateAttri();
		m_pPlayer->setUpdateValue(_UV_Shape);
	}

	//֪ͨ�ͻ���
	if (bSend)
		send_EquipItemAck(uItemSN,uEquipType,R_ResultItem_Succeed);

	return true;
}

//-------------------------------------------------------------
//------------------------------ ���۵���
bool	CGameItemMgr::sellItem(uint64 uItemSN,uint8& uCode)
{
	if (!canOperate())
		return false;

	CGameItem* pGameItem = findItem(uItemSN);
	CHECKF(pGameItem != NULL);
	const _stItemModel* pModel = pGameItem->getModel();
	CHECKF(pModel != NULL) ;
	if (pModel->uSellPrice <= 0)
	{
		uCode = R_ResultItem_NotSell;
		return false; 
	}

	uint64 uAddMoney = pModel->uSellPrice * pGameItem->getItemNum();

	//�ƶ����ع��б�
	delGameItem(pGameItem,false);
	m_mapSellItem.insert_(uItemSN,pGameItem);

	//��־
	//CLog::error("player[%d] sell item[%d] num[%d]",m_pPlayer->getPlayerID(),pModel->uItemID,pGameItem->getItemNum());
	//��Ǯ
	m_pPlayer->addMoney(MoneyType_BindSliver,uAddMoney,DoingType_Sell_Item);
	
	//֪ͨ�ͻ���
	send_SellItemAck(uItemSN,R_ResultItem_Succeed);
	return true;
}

//-------------------------------------------------------------
//------------------------------ �ۻص���
bool	CGameItemMgr::buyBackItem(uint64 uItemSN,uint8& uCode)
{
	if (!canOperate())
		return false;

	CGameItem* pGameItem = findSellItem(uItemSN);
	CHECKF(pGameItem != NULL);
	const _stItemModel* pModel = pGameItem->getModel();
	CHECKF(pModel != NULL && pModel->uBuyPrice > 0);

	//���Ǯ
	uint64 uNeedMoney = pModel->uSellPrice * pGameItem->getItemNum();
	if (!m_pPlayer->canAddMoney(MoneyType_BindSliver,(int32)uNeedMoney * -1))
	{
		uCode = R_ResultItem_NeedMoney;
		return false;
	}

	//��Ǯ
	m_pPlayer->addMoney(MoneyType_BindSliver,(int32)uNeedMoney * -1, DoingType_BuyBack_Item);

	//�ƶ��������б�
	m_mapBagItem.insert_(pGameItem->getItemSN(),pGameItem);
	m_mapSellItem.erase(uItemSN);

	//֪ͨ�ͻ���
	send_BuyBackItemAck(uItemSN,R_ResultItem_Succeed);
	return true;
}

//-------------------------------------------------------------
//------------------------------ �ƶ�����
bool	CGameItemMgr::moveItem	(uint64 uFromItemSN,uint64 uToItemSN,uint16 uItemNum,uint8& uCode)
{
	CGameItem* pFromGameItem = findItem(uFromItemSN);
	CGameItem* pToGameItem = findItem(uToItemSN);
	CHECKF(pFromGameItem != NULL && pToGameItem != NULL );
	CHECKF(uFromItemSN != uToItemSN);
	//�Ƿ���ͬ��ID
	CHECKF(pFromGameItem->getItemID() == pToGameItem->getItemID());

	//�Ƿ���ͬ�İ�����
	CHECKF(pFromGameItem->isBind() == pToGameItem->isBind());

	//�Ƿ���ͬ�ı�������
	CHECKF(pFromGameItem->getBingfuExp() == pToGameItem->getBingfuExp());

	//�����Ƿ��㹻
	if (pFromGameItem->getItemNum() < uItemNum)
	{
		uCode = R_ResultItem_NotEnough;
		return false;
	}

	//�Ƿ��ܵ���
	if (pToGameItem->getItemNum() + uItemNum > pToGameItem->getMaxCount())
	{
		uCode = R_ResultItem_MaxCount;
		return false;
	}

	//�۵���
	updateItemNum(pFromGameItem,(int32)uItemNum * -1,false,DoingType_Item_Move);
	updateItemNum(pToGameItem,uItemNum,false,DoingType_Item_Move);
	return true;
}

//-------------------------------------------------------------
//------------------------------  ���ӵ���
bool	CGameItemMgr::addItemPack	(uint16 uPackID,uint16 uPackNum,_emDoingType emDoing)
{
	if (!canOperate())
		return false;

	const _stItemPackModel* pPackModel = g_clItemData.findPack(uPackID);
	CHECKF(pPackModel != NULL && !pPackModel->vecItemBuild.empty() );
	auto _pos = pPackModel->vecItemBuild.begin();
	auto _end = pPackModel->vecItemBuild.end();
	switch(pPackModel->uType)
	{
	case ItemPacket_Rate:	//���ʻ��
		{
			for (;_pos != _end;++_pos)
			{
				const _stItemBuild&stItemBuild = (*_pos);
				for (uint16 i = 0;i < uPackNum;++i)
				{
					if(randRatio(stItemBuild.uRate,10000))
						addItem(stItemBuild,emDoing);
				}
			}
		}
		break;
	case ItemPacket_Mutually:		//������
		{
			static stl_vector<uint32> vecRand;
			vecRand.clear();
			for (; _pos != _end; ++_pos)
			{
				const _stItemBuild&stItemBuild = (*_pos);
				vecRand.push_back(stItemBuild.uRate);
			}

			for (uint16 i = 0;i < uPackNum;++i)
			{
				int32 iRand = getRandIndex(vecRand);
				if(iRand < 0)
					iRand = 0;
				if(uint32(iRand) >= pPackModel->vecItemBuild.size())
					iRand = pPackModel->vecItemBuild.size() - 1;

				const _stItemBuild& stItemBuild = pPackModel->vecItemBuild[iRand];
				addItem(stItemBuild,emDoing);
			}
		}
		break;
	case ItemPacket_Normal:		//�������
	default:
		{
			for (;_pos != _end;++_pos)
			{
				_stItemBuild stItemBuild = (*_pos);
				stItemBuild.uItemNum *= uPackNum;
				addItem(stItemBuild,emDoing);
			}
		}
		break;
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------  �ֽ����
bool	CGameItemMgr::splitItem	(SET_U64& setItemSN,uint8& uCode)
{
	if (!canOperate() || setItemSN.empty())
		return false;

	//Ԥ����
	stl_set<CGameItem*> setSplitItem;
	SET_U64 setSplitSN;
	MAP_U32_U16 mapBindSplitItem;
	MAP_U32_U16 mapNoBindSplitItem;
	for (CONST_ITER_SET_U64 _pos = setItemSN.begin(); _pos != setItemSN.end(); ++_pos)
	{
		uint64 uItemSN = *_pos;
		CGameItem* pGameItem = findItem(uItemSN);
		if (!pGameItem)
			continue;
		const _stItemModel* pItemModel = pGameItem->getModel();
		if (!pItemModel || !pItemModel->uSplitItemID)
			continue;
		setSplitItem.insert(pGameItem);
		setSplitSN.insert(uItemSN);
		if (pGameItem->isBind())
		{
			mapBindSplitItem[pItemModel->uSplitItemID] += pItemModel->uSplitItemNum; 
		}
		else
		{
			mapNoBindSplitItem[pItemModel->uSplitItemID] += pItemModel->uSplitItemNum; 
		}
	}

	//��װ�� һ��Ҫ�ɹ�
	for (stl_set<CGameItem*>::const_iterator _pos = setSplitItem.begin(); _pos != setSplitItem.end(); ++_pos)
	{
		CGameItem* pGameItem = *_pos;
		CHECKL(pGameItem != NULL);
		CHECKL(updateItemNum(pGameItem,(int32)pGameItem->getItemNum() * -1,false,DoingType_Item_Split));
	}

	{//�ӵ���
		ITER_MAP_U32_U16 _pos = mapBindSplitItem.begin();
		for (; _pos != mapBindSplitItem.end(); ++_pos)
		{
			CHECKL(addItem(_pos->first,_pos->second,true,true,true,DoingType_Item_Split));
		}

		_pos = mapNoBindSplitItem.begin();
		for (; _pos != mapNoBindSplitItem.end(); ++_pos)
		{
			CHECKL(addItem(_pos->first,_pos->second,false,true,true,DoingType_Item_Split));
		}
	}
	
	//֪ͨ�ͻ���
	send_SplitItemAck(setSplitSN,R_ResultItem_Succeed);
	return true;
}

//-------------------------------------------------------------
//------------------------------  ���µ��߸����������Ƴ�pGameItemΪ��
bool	CGameItemMgr::updateItemNum	(CGameItem* pGameItem,int32 nNum,bool bSend,_emDoingType emDoing)
{
	if (!canOperate())
		return false;

	CHECKF(pGameItem != NULL);
	CHECKF(!(0 == pGameItem->getItemNum() && nNum < 0));
	int16 nItemNum = pGameItem->getItemNum() + nNum;
	nItemNum = MAX((int16)0,nItemNum);
	pGameItem->setItemNum(nItemNum);

	//����־
	/*CLog::error("player[%d] additem snid[%llu] itemid[%d],addnum[%d],curnum[%d]",\
		m_pPlayer->getPlayerID(),pGameItem->getItemSN(),pGameItem->getItemID(),nNum,pGameItem->getItemNum());*/

	//��־����
	g_game_log->item_log(m_pPlayer->getPlayerID(),emDoing,pGameItem->getItem());

	//֪ͨ�ͻ���
	if (bSend)
		send_UpdateItemNumAck(pGameItem->getItemSN(),nItemNum,emDoing);

	uint32 uItemID = pGameItem->getItemID();
	//ɾ������
	if (0 == nItemNum)
		delGameItem(pGameItem);

	// ��Ȼص� (ʹ�õ���)
	const _stItemModel *pModel = pGameItem->getModel();
	if (!pModel)
		return false;

	if (nNum < 0) 
	{
		//CPlayerActivity * pActivity = m_pPlayer->getPlayerActivity();
		//if (!pActivity)
		//	return false;
		//pActivity->callback_userItem(pModel->uType);	
	}

	//CPlayerQuestMgr* pQuestMgr = m_pPlayer->getPlayerQuestMgr();
	//if(pQuestMgr)
	//{
	//	pQuestMgr->callback_updateItemNum(uItemID);
	//}
	return true;
}


//-------------------------------------------------------------
//------------------------------ 
bool	CGameItemMgr::itemUse_AddExpend	(CGameItem* pGameItem,uint16 uItemNum)
{
	if (!canOperate())
		return false;
	CHECKF(pGameItem != NULL);
	const _stItemModel* pModel = pGameItem->getModel();
	CHECKF(pModel != NULL);

	if (pModel->vecFunValue.size() < 2)
		return false;

	return m_pPlayer->addExpend(pModel->vecFunValue[0],pModel->vecFunValue[1],DoingType_Item_Use);
}

//-------------------------------------------------------------
//------------------------------ 
bool	CGameItemMgr::itemUse_AddBuff	(CGameItem* pGameItem,uint16 uItemNum)
{
	if (!canOperate())
		return false;
	CHECKF(pGameItem != NULL);
	const _stItemModel* pModel = pGameItem->getModel();
	CHECKF(pModel != NULL);

	if (pModel->vecFunValue.size() < 2)
		return false;

	return m_pPlayer->getGameBuffMgr().addBuff(pModel->vecFunValue[0],pModel->vecFunValue[1],m_pPlayer->getUnitID(),0, DoingType_ItemUseBuff);
}

//-------------------------------------------------------------
//------------------------------ 
bool	CGameItemMgr::itemUse_Tranfer	(CGameItem* pGameItem,uint16 uItemNum)
{

	return true;
}

//-------------------------------------------------------------
//------------------------------ ��ȡ���
bool	CGameItemMgr::itemUse_Packet	(CGameItem* pGameItem,uint16 uItemNum)
{
	CHECKF(pGameItem != NULL);
	const _stItemModel* pModel = pGameItem->getModel();
	CHECKF(pModel != NULL && pModel->vecFunValue.size() > 0);
	for (uint32 i = 0; i < pModel->vecFunValue.size(); ++i)
	{
		addItemPack(pModel->vecFunValue[i],uItemNum, DoingType_ItemUse_Packet);
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ ��ȡ���
bool	CGameItemMgr::itemUse_Treasure	(CGameItem* pGameItem,uint16 uItemNum)
{
	if (!canOperate() || !pGameItem || 1 != uItemNum)
		return false;

	const _stItemModel* pItemModel = pGameItem->getModel();
	CHECKF(pItemModel != NULL);

	const _stTreasureMapModel* pTreasureModel = g_clTreasureMapData.findTreasureMap(pItemModel->uSubType);
	CHECKF(pTreasureModel != NULL);

	////������
	//if (m_pPlayer->getPlayerQuestMgr())
	//	m_pPlayer->getPlayerQuestMgr()->acceptTreasureQuest(pTreasureModel->getRandQuest(),pItemModel->uItemID);
	
	return true;
}

//-------------------------------------------------------------
//------------------------------ ��ȡʱװ
bool	CGameItemMgr::itemUse_Fashion	(CGameItem* pGameItem,uint16 uItemNum)
{
	CHECKF(pGameItem != NULL);
	const _stItemModel* pModel = pGameItem->getModel();
	CHECKF(pModel != NULL && pModel->vecFunValue.size() == 1);
	//CPlayerFashion* pPlayerFashion = (CPlayerFashion*)m_pPlayer->getPlayerFun(GameSystemType_Fashion);
	//if (!pPlayerFashion )
	//	return false;

	//return pPlayerFashion->addFashion(pModel->vecFunValue[0]);
	return false;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CGameItemMgr::send_UpdateItemNumAck	(uint64 uItemSN,uint16 uItemNum,_emDoingType emDoing,uint8 uCode/* = R_ResultItem_Succeed*/)
{
	if (!canOperate())
		return false;
	PBG2CUpdateItem ack;
	ack.set_itemsnid(uItemSN);
	ack.set_itemnum(uItemNum);
	ack.set_doing(emDoing);
	return m_pPlayer->sendMsg(P_S2C_Protocol_Item,S2C_Item_UpdateNum,&ack,uCode);
}

//-------------------------------------------------------------
//------------------------------ 
bool	CGameItemMgr::send_AddItemByMailAck	(_stItem& stItem,_emDoingType emDoing,uint8 uCode/* = R_ResultItem_Succeed*/)
{
	if (!canOperate())
		return false;

	PBG2CNewItem ack;
	ack.set_isbymail(true);
	stItem.saveToProtobuf(*ack.mutable_iteminfo());
	ack.set_doing(emDoing);
	return m_pPlayer->sendMsg(P_S2C_Protocol_Item,S2C_Item_New,&ack,uCode);
}

//-------------------------------------------------------------
//------------------------------ 
bool	CGameItemMgr::send_AddItemAck	(_stItem& stItem,_emDoingType emDoing,uint8 uCode/* = R_ResultItem_Succeed*/)
{
	if (!canOperate())
		return false;

	PBG2CNewItem ack;
	stItem.saveToProtobuf(*ack.mutable_iteminfo());
	ack.set_doing(emDoing);
	return m_pPlayer->sendMsg(P_S2C_Protocol_Item,S2C_Item_New,&ack,uCode);
}

//-------------------------------------------------------------
//------------------------------ ʧ�ܲŷ���
bool	CGameItemMgr::send_DelItemAck	(uint64 uItemSN,uint8 uCode/* = R_ResultItem_Succeed*/)
{
	if (!canOperate())
		return false;

	PBG2CDelItem ack;
	ack.set_itemsnid(uItemSN);
	return m_pPlayer->sendMsg(P_S2C_Protocol_Item,S2C_Item_Del,&ack,uCode);
}

//-------------------------------------------------------------
//------------------------------ ʹ�õ���ʧ�ܲŷ���
bool	CGameItemMgr::send_UseItemAck	(uint64 uItemSN,uint16 uItemNum,uint8 uCode/* = R_ResultItem_Succeed*/)
{
	if (!canOperate())
		return false;

	PBG2CUseItem ack;
	ack.set_itemsnid(uItemSN);
	ack.set_itemnum(uItemNum);
	return m_pPlayer->sendMsg(P_S2C_Protocol_Item,S2C_Item_Use,&ack,uCode);
}

//-------------------------------------------------------------
//------------------------------ ����/����װ��ʧ�ܲŷ���
bool	CGameItemMgr::send_EquipItemAck	(uint64 uItemSN,uint8 uEquipType,uint8 uCode/* = R_ResultItem_Succeed*/)
{
	if (!canOperate())
		return false;

	PBG2CEquipItem ack;
	ack.set_itemsnid(uItemSN);
	ack.set_equiptype(uEquipType);
	CGameItem* pEquipItem = findEquip(uEquipType);
	if (pEquipItem)
		ack.set_flag(pEquipItem->getItem().uFlag);

	return m_pPlayer->sendMsg(P_S2C_Protocol_Item,S2C_Item_Equip,&ack,uCode);
}

//-------------------------------------------------------------
//------------------------------ ����װ��ʧ�ܲŷ���
bool	CGameItemMgr::send_SellItemAck	(uint64 uItemSN,uint8 uCode/* = R_ResultItem_Succeed*/)
{
	if (!canOperate())
		return false;

	PBG2CSellItem ack;
	ack.set_itemsnid(uItemSN);
	return m_pPlayer->sendMsg(P_S2C_Protocol_Item,S2C_Item_Sell,&ack,uCode);
}

//-------------------------------------------------------------
//------------------------------ �ع�ʧ�ܲŷ���
bool	CGameItemMgr::send_BuyBackItemAck	(uint64 uItemSN,uint8 uCode/* = R_ResultItem_Succeed*/)
{
	if (!canOperate())
		return false;

	PBG2CBuyBackItem ack;
	ack.set_itemsnid(uItemSN);
	return m_pPlayer->sendMsg(P_S2C_Protocol_Item,S2C_Item_BuyBack,&ack,uCode);
}

//-------------------------------------------------------------
//------------------------------ �������ʧ�ܲŷ���
bool	CGameItemMgr::send_SortItemAck	(uint8 uCode/* = R_ResultItem_Succeed*/)
{
	if (!canOperate())
		return false;

	PBG2CSortItem ack;
	return m_pPlayer->sendMsg(P_S2C_Protocol_Item,S2C_Item_Sort,&ack,uCode);
}

//-------------------------------------------------------------
//------------------------------ �ֽ���߷���
bool	CGameItemMgr::send_SplitItemAck	(SET_U64& setItemSN,uint8 uCode/* = R_ResultItem_Succeed*/)
{
	if (!canOperate())
		return false;

	PBG2CSplitItem ack;
	for (CONST_ITER_SET_U64 _pos = setItemSN.begin(); _pos != setItemSN.end(); ++_pos)
	{
		ack.add_itemsnid(*_pos);
	}
	return m_pPlayer->sendMsg(P_S2C_Protocol_Item,S2C_Item_Split,&ack,uCode);
}
//-------------------------------------------------------------
//------------------------------ ǿ������
bool	CGameItemMgr::send_EquipEnhanceAck	(uint8 uEquipType,uint8 uCode/* = R_ResultItem_Succeed*/)
{
	if (!canOperate())
		return false;

	CGameItem* pGameItem = findEquip(uEquipType);
	if (!pGameItem)
		return false;

	PBG2CEquipEnhance ack;
	ack.set_equiptype(uEquipType);
	ack.set_exp(pGameItem->getEnhanceExp());
	return m_pPlayer->sendMsg(P_S2C_Protocol_Item,S2C_Item_Enhance,&ack,uCode);
}
//-------------------------------------------------------------
//------------------------------ ��ĥ����
bool	CGameItemMgr::send_EquipForgeAck(uint8 uEquipType,uint8 uCode/* = R_ResultItem_Succeed*/)
{
	if (!canOperate())
		return false;

	CGameItem* pGameItem = findEquip(uEquipType);
	if (!pGameItem)
		return false;

	PBG2CEquipForge ack;
	ack.set_equiptype(uEquipType);
	ack.set_exp(pGameItem->getForgeExp());
	return m_pPlayer->sendMsg(P_S2C_Protocol_Item,S2C_Item_Forge,&ack,uCode);
}
//-------------------------------------------------------------
//------------------------------ �̳з���
bool	CGameItemMgr::send_EquipInheritAck	(uint64 uItemSN,uint8 uCode/* = R_ResultItem_Succeed*/)
{
	if (!canOperate())
		return false;
	CGameItem* pGameItem = findAllItem(uItemSN);
	if (!pGameItem)
		return false;
	_stItem& stItem = pGameItem->getItem();
	PBG2CEquipInherit ack;
	ack.set_itemsnid(uItemSN);
	for (uint8 i = 0; i < stItem.vecSpecialID.size(); ++i)
		ack.add_specialid(stItem.vecSpecialID[i]);
	return m_pPlayer->sendMsg(P_S2C_Protocol_Item,S2C_Item_Inherit,&ack,uCode);
}
//-------------------------------------------------------------
//------------------------------ ���췵�� ʧ�ܲŷ���
bool	CGameItemMgr::send_EquipMakeAck	(uint16 uMakeID,uint8 uMark,uint8 uCode/* = R_ResultItem_Succeed*/)
{
	if (!canOperate())
		return false;

	PBG2CEquipMake ack;
	ack.set_makeid(uMakeID);
	ack.set_mark(uMark);
	return m_pPlayer->sendMsg(P_S2C_Protocol_Item,S2C_Item_Make,&ack,uCode);
}
//-------------------------------------------------------------
//------------------------------ ��˷��� ʧ�ܲŷ���
bool	CGameItemMgr::send_MountingAck	(uint8 uCode/* = R_ResultItem_Succeed*/)
{
	if (!canOperate())
		return false;

	PBG2CMounting ack;
	ack.set_mountid(m_pPlayer->getMountID());
	return m_pPlayer->sendMsg(P_S2C_Protocol_Item,S2C_Item_Mounting,&ack,uCode);
}
//-------------------------------------------------------------
//------------------------------
bool CGameItemMgr::send_BingfuUpgrade_Ack	(uint64 uSn,uint8 uCode/* = 0*/)
{
	if (!canOperate())
		return false;

	CGameItem* pGameItem = findItem(uSn);
	if (!pGameItem)
		return false;

	PBItem pbItem;
	pGameItem->getItem().saveToProtobuf(pbItem);
	return m_pPlayer->sendMsg(P_S2C_Protocol_Item,S2C_Item_BingfuUpgrade,&pbItem,uCode);
}
//-------------------------------------------------------------
//------------------------------
bool CGameItemMgr::send_Compound_Ack	(uint32 uAddItemID,uint8 uCode/* = 0*/)
{
	if (!canOperate())
		return false;

	PBG2CItemCompoundAck ack;
	ack.set_itemid(uAddItemID);
	return m_pPlayer->sendMsg(P_S2C_Protocol_Item,S2C_Item_Compound,&ack,uCode);
}
//-------------------------------------------------------------
//------------------------------
bool CGameItemMgr::send_BingfuSplit_Ack	(uint8 uCode/* = 0*/)
{
	if (!canOperate())
		return false;

	return m_pPlayer->sendMsg(P_S2C_Protocol_Item,S2C_Item_BingfuSplit,NULL,uCode);
}
//-------------------------------------------------------------
//------------------------------
bool CGameItemMgr::send_UpdateItemAck	(CGameItem* pGameItem, uint8 uCode/* = 0*/)
{
	if (!canOperate())
		return false;
	
	PBItem pbItem;
	pGameItem->getItem().saveToProtobuf(pbItem);
	return m_pPlayer->sendMsg(P_S2C_Protocol_Item,S2C_Item_Update,&pbItem,uCode);
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameItemMgr::process(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay)
{
	if ( uTime >= m_uNextCheckExpire )
	{//����ʧЧ
		m_uNextCheckExpire = (uint32)uTime + 10;
		stl_vector<CGameItem*> vecDelItem;
		auto _pos = m_mapBagItem.begin();
		for (; _pos != m_mapBagItem.end(); ++_pos)
		{
			CGameItem* pGameItem = _pos->second;
			if (!pGameItem || !pGameItem->isExpireItem() || !pGameItem->isOutExpire(uTime))
				continue;
			vecDelItem.push_back(pGameItem);
		}

		//ɾ��
		for (uint16 i = 0; i < vecDelItem.size(); ++i)
		{
			CGameItem* pGameItem = vecDelItem[i];
			if (!pGameItem)
				continue;
			updateItemNum(pGameItem,(int32)pGameItem->getItemNum() * -1,true,DoingType_Equip_Expire);
		}
	}
}
//-------------------------------------------------------------
//------------------------------���¼�������
bool	CGameItemMgr::calculateAttri()
{
	if (!canOperate())
		return false;

	m_pItemGameAttr->initGameAttribute();

	MAP_U16_U8 mapSuit;
	ITER_MAP_EQUIP _pos = m_mapEquip.begin();
	for (; _pos != m_mapEquip.end(); ++_pos)
	{
		//δ���������
		if (EquipType_Mount == _pos->first && 0 == m_pPlayer->getMountID())
			continue;

		CGameItem* pGameItem = _pos->second;
		CHECKC(pGameItem != NULL);
		pGameItem->addGameAttri(*m_pItemGameAttr);
		uint16 uSuitID = g_clEquipData.findSuitID(pGameItem->getItemID());
		if (uSuitID)
		{
			mapSuit[uSuitID] = mapSuit[uSuitID] +1;
		}
	}
	
	//����װ
	ITER_MAP_U16_U8 _spos = mapSuit.begin();
	for (; _spos != mapSuit.end(); ++_spos)
	{
		const _stEquipSuitModel* pSuitModel = g_clEquipData.findSuitModel(_spos->first);
		if (!pSuitModel)
			continue;

		const _stGameAttribute* pGameAttri = pSuitModel->mapGameAttr.find_(_spos->second);
		if (pGameAttri)
			m_pItemGameAttr->add(pGameAttri);
	}

	m_pPlayer->setUpdateValue(_UV_AttributeBase);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
int	CGameItemMgr::sort_expenditem	(const void *p1, const void *p2)
{
	if (!p1 || !p2)
		return -1;

	const _stExpendGameItem* pExpend1 =(const _stExpendGameItem*)(p1);
	const _stExpendGameItem* pExpend2=(const _stExpendGameItem*)(p2);

	CGameItem* pGameItem1 = pExpend1->pGameItem;
	CGameItem* pGameItem2 = pExpend2->pGameItem;
	if (!pGameItem1 || !pGameItem1->getModel() || !pGameItem2 || !pGameItem2->getModel())
		return -1;

	if (pGameItem1->getModel()->uQuality > pGameItem2->getModel()->uQuality)
		return -1;

	if (pGameItem1->isBind() && !pGameItem2->isBind())
			return -1;

	return 0;
}

//-------------------------------------------------------------
//------------------------------ �̳� 
bool	CGameItemMgr::equipInherit(uint64 uItemSN,uint64 uDelItemSN,uint8& uCode)
{
	if (!canOperate())
		return false;
	
	CGameItem* pSourceItem = findAllItem(uItemSN);  
	CGameItem* pTargetItem = findAllItem(uDelItemSN);
	if (!pSourceItem || !pTargetItem)
	{
		uCode = R_ResultItem_NoItem;
		return false;
	}

	const _stItemModel* pSourceModel = pSourceItem->getModel();
	const _stItemModel* pTargetModel = pTargetItem->getModel();
	if (!pSourceModel || !pTargetModel)
	{
		uCode = R_ResultItem_NoItem;
		return false;
	}
	if (findEquip(pSourceModel->uSubType) != pSourceItem ) // ���װ��Ҫ������
	{
		uCode = R_ResultItem_NoItem; //TODO
		return false;
	}

	//ͬ�������ܼ̳�
	if (pSourceModel->uType != ItemType_Equip ||
		pTargetModel->uType != ItemType_Equip || 
		pSourceModel->uSubType != pTargetModel->uSubType)
	{
		uCode = R_ResultItem_InheritNeedSame;
		return false;
	}

	//������Ҫ��Ǯ
	uint32 uNeedMoney = g_clConstantData.Item_Inherit_Special;

	if (uNeedMoney && !m_pPlayer->canExpend(MoneyType_BindSliver,uNeedMoney))
	{
		uCode = R_ResultItem_NeedMoney;
		return false;
	}

	m_pPlayer->addExpend(MoneyType_BindSliver,(int64)uNeedMoney * -1, DoingType_Equip_Inherit);

	 _stItem stTargetItem = pTargetItem->getItem();
	updateItemNum(pTargetItem,-1,true,DoingType_Equip_Inherit);


	pSourceItem->updateSpecail(stTargetItem.vecSpecialID,DoingType_Equip_Inherit);

	//��������
	calculateAttri();

	//֪ͨ�ͻ���
	send_EquipInheritAck(uItemSN,R_ResultItem_Succeed);

	return true;
}

//-------------------------------------------------------------
//------------------------------ ����
bool	CGameItemMgr::equipMake(uint16 uMakeID,uint8 uMark,uint8& uCode)
{
	if (!canOperate())
		return false;
	const _stEquipMakeModel* pMakeModel = g_clEquipData.findMakeModel(uMakeID);
	CHECKF(pMakeModel != NULL);

	//���ǰ������
	if (!m_pPlayer->canPreCondition(pMakeModel->dmapPreCondition))
	{
		uCode = R_ResultItem_PreCondition;
		return false;
	}

	//������
	uint8 uMoneyType = MoneyType_BindSliver;
	bool bNoBind = _CHECK_BIT(uMark,_BIT32(EquipMakeMark_NoBind));
	_em_ItemBindType emBindType = ItemBindType_All;
	if (bNoBind) 
	{
		emBindType = ItemBindType_NoBind;
		uMoneyType = MoneyType_Sliver;
	}

	//�жϽ�Ǯ
	if (!m_pPlayer->canAddMoney(uMoneyType,int64(pMakeModel->uNeedMoney) * -1))
	{
		uCode = R_ResultItem_NeedMoney;
		return false;
	}

	//������
	if (!canRemoveItem(pMakeModel->mapNeedItem,emBindType))
	{
		uCode = R_ResultItem_MakeNeedItem;
		return false;
	}

	//����ֵ
	bool bUseLucky = _CHECK_BIT(uMark,_BIT32(EquipMakeMark_Lucky));
	if (bUseLucky && false == m_pPlayer->canExpend(ExpendType_EquipLucky,(int32)pMakeModel->uEquipNeedLucky))
	{
		uCode = R_ResultItem_MakeNeedLucky;
		return false;
	}

	//�������
	bool bUseSpecial = _CHECK_BIT(uMark,_BIT32(EquipMakeMark_Special));
	if (bUseSpecial && !canRemoveItem(pMakeModel->mapNeedSpecialItem,emBindType))
	{
		uCode = R_ResultItem_MakeNeedSpecial;
		return false;
	}

	//�۳����
	m_pPlayer->addMoney(uMoneyType, int64(pMakeModel->uNeedMoney) * -1, DoingType_Equip_Make);

	//�۳�����
	removeItem(pMakeModel->mapNeedItem,emBindType,DoingType_Equip_Make);

	if (bUseSpecial)
	{
		removeItem(pMakeModel->mapNeedSpecialItem,emBindType,DoingType_Equip_Make);
	}

	if (bUseLucky)
		m_pPlayer->addExpend(ExpendType_EquipLucky,(int32)pMakeModel->uEquipNeedLucky * -1, DoingType_Equip_Make);

	_stItem stItem;
	stItem.uItemID = pMakeModel->getRandEquipID(bUseSpecial);
	stItem.uItemNum = 1;
	CHECKF(stItem.uItemID > 0);
	if (!bNoBind) // ����Ӧ�ü����û��ʹ�ð󶨲��ϣ������ǿͻ���ָ����
		stItem.setFlag(ItemFlag_Binded,true);

	const _stItemModel* pEquipModel = g_clItemData.find(stItem.uItemID);
	CHECKF(pEquipModel != NULL);

	//��ȡ��������
	g_clEquipData.getSpecialModel(stItem.uItemID,bUseLucky,pEquipModel->uLevel,stItem.vecSpecialID);


	if (!canAddItem(pEquipModel,stItem.uItemNum) ) 
	{
		DMAP_NEED_ITEM dmapNeedItem;
		dmapNeedItem.insert(stItem.uItemID, stItem.uItemNum, bNoBind ? 1 : 0);
		m_pPlayer->sendMail(MailType_FullBag,&dmapNeedItem, DoingType_Equip_Make);	
		send_AddItemByMailAck(stItem, DoingType_Equip_Make);
	}
	else
	{
		//����װ����֪ͨ
		addNewItem(stItem,true, DoingType_Equip_Make);

	}


	//����luckyֵ
	if (pMakeModel->uEquipAddLucky)
		m_pPlayer->addExpend(ExpendType_EquipLucky,pMakeModel->uEquipAddLucky, DoingType_Equip_Make);

	openMakeEquipDlgAsk(uCode);

	//CPlayerQuestMgr* pQuestMgr = m_pPlayer->getPlayerQuestMgr();
	//if (pQuestMgr)
	//	pQuestMgr->callback_completeItemMake();

	return true;
}
//-------------------------------------------------------------
//------------------------------������
void	CGameItemMgr::clearBag		()
{
	stl_vector<CGameItem*> vecGameItem;
	ITER_MAP_ITEM _pos = m_mapBagItem.begin();
	for (;_pos != m_mapBagItem.end(); ++_pos)
	{
		vecGameItem.push_back(_pos->second);
	}

	for (uint16 i = 0; i < vecGameItem.size(); ++i)
	{
		CGameItem* pGameItem = vecGameItem[i];
		if (!pGameItem)
			continue;
		updateItemNum(pGameItem,(int32)pGameItem->getItemNum() * -1,true,DoingType_GM);
	}
}
//-------------------------------------------------------------
//------------------------------������
void	CGameItemMgr::clearEquip		()
{
	for (uint32 i = _emEquipType_MIN; i<= _emEquipType_MAX; ++i)
	{
		uint8 ucode = 0;
		equipItem(0, i, ucode);
	}	
}
//-------------------------------------------------------------
//------------------------------ ���
bool	CGameItemMgr::mounting(bool bMount,uint8& uCode)
{
	if (!canOperate())
		return false;

	uint16 uCurMountID = m_pPlayer->getMountID();
	//����
	if (bMount)
	{
		//�������ʱ���ӵ�buff
		m_pPlayer->getGameBuffMgr().delBuff((uint32)g_clConstantData.Relive_Buff_ID, \
			DoingType_Relive);

		CGameItem* pGameItem = findEquip(EquipType_Mount);
		if (!pGameItem)
		{
			uCode = R_ResultItem_NoItem;
			return false;
		}

		if (uCurMountID > 0)
		{
			uCode = R_ResultItem_Mounting;
			return false;
		}

		//ս���в������
		if (m_pPlayer->haveState(StateType_Fight))
		{
			uCode = R_ResultItem_FightingNoMount;
			return false;
		}

		//�����������
		if (m_pPlayer->testRegion(RegionMark_CntMounting))
		{
			uCode = R_ResultItem_RegionNoMount;
			return false;
		}

		//�����в������
		if (m_pPlayer->checkStateBuffAttr(StateBuffAttr_ForbidMount))
		{
			uCode = R_ResultItem_ControlNoMount;
			return false;
		}

		m_pPlayer->setMountID(pGameItem->getItemID());
	}
	else
	{
		if (0 == uCurMountID)
		{
			uCode = R_ResultItem_unMounting;
			return false;
		}
		m_pPlayer->setMountID(0);
	}

	//��������
	calculateAttri();

	//��������
	m_pPlayer->setUpdateValue(_UV_Shape);

	//֪ͨ�ͻ��˸������״̬
	send_MountingAck(R_ResultItem_Succeed);

	//ȡ������
	if (m_pPlayer->getPlayerCommon())
	{
		m_pPlayer->getPlayerCommon()->cancelTakeASeat();
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameItemMgr::bingfuUpgradeAsk(uint64 uSn,MAP_U64_U16& mapExpend,uint8& uCode)
{
	if (!canOperate() || mapExpend.empty())
		return false;

	CGameItem* pGameBingfu = findItem(uSn);
	if (!pGameBingfu)
	{
		uCode = R_ResultItem_NoItem;
		return false;
	}

	const _stItemModel* pModel = pGameBingfu->getModel();
	if (!pModel || ItemType_BingFu != pModel->uType)
		return false;

	//���������������
	if (pModel->uSubType == BingfuType_Exp)
	{
		uCode = R_ResultItem_ExpBingfuNoUp;
		return false;
	}

	stl_map<CGameItem*,uint16> mapCurExpend;
	//����������Ҫ�ľ��� �����������ټ�
	const _stBingfuModel* pOldModel = g_clBingfuData.findBingfuByExp(pGameBingfu->getItemID(),pGameBingfu->getBingfuExp());
	CHECKF(pOldModel != NULL);
	uint32 uMaxExp = 0;
	for (uint16 i = pOldModel->uBingfuLevel + 1; i <= g_clBingfuData.getMaxLevel(pGameBingfu->getItemID()); ++i)
	{
		const _stBingfuModel* pTmpModel = g_clBingfuData.findBingfu(pGameBingfu->getItemID(),i);
		if (!pTmpModel)
			continue;

		if (!m_pPlayer->canPreCondition(pTmpModel->dmapPreCondition) || !m_pPlayer->canDelExpend(pTmpModel->mapNeedExpend))
		{
			uMaxExp = pOldModel->uTotalExp - 1;
			break;
		}

		uMaxExp = pTmpModel->uTotalExp;
	}


	uint32 uPreCurExp = pGameBingfu->getBingfuExp(); //Ԥ�������ӵ��ľ���
	ITER_MAP_U64_U16 _pos = mapExpend.begin();
	for (; _pos != mapExpend.end(); ++_pos)
	{
		//�Ƿ��ܷ�۳� ��ʹ���˲���ɾ��
		CGameItem* pTmpGameBingfu = findItem(_pos->first);
		if (!pTmpGameBingfu)
			continue;

		const _stItemModel* pTmpModel = pTmpGameBingfu->getModel();
		if (!pTmpModel || pTmpModel->uSubType != BingfuType_Exp || !pTmpGameBingfu->getBingfuExp())
			continue;

		uint16 uExpendNum = MIN(pTmpGameBingfu->getItemNum(),_pos->second);

		//�����Ƿ����
		uint16 uNeedNum = (uint16)ceil((float32)(uMaxExp - uPreCurExp) / pTmpGameBingfu->getBingfuExp());
		uExpendNum = MIN(uNeedNum,uExpendNum);
		mapCurExpend.insert_(pTmpGameBingfu,uExpendNum);
		uPreCurExp += pTmpGameBingfu->getBingfuExp() * uExpendNum;
		uPreCurExp = MIN(uPreCurExp,uMaxExp);
		if (uNeedNum <= uExpendNum || uPreCurExp >= uMaxExp)
			break;
	}

	//�޾���ı�ֱ�ӷ���
	if (uPreCurExp == pGameBingfu->getBingfuExp())
		return false;

	//���ı���
	MAP_U64_U16 mapNewExpend;
	stl_map<CGameItem*,uint16>::iterator _exp_pos = mapCurExpend.begin();
	for (; _exp_pos != mapCurExpend.end(); ++_exp_pos)
	{
		CGameItem* pTmpGameBingfu = _exp_pos->first;
		if (!pTmpGameBingfu)
			continue;

		mapNewExpend.insert_(pTmpGameBingfu->getItemSN(),pTmpGameBingfu->getItemNum() - _exp_pos->second);
		CHECKF(this->updateItemNum(pTmpGameBingfu,(int32)_exp_pos->second * -1,true,DoingType_BingfuUpgrade));
	}

	//������
	const _stBingfuModel* pNewModel = g_clBingfuData.findBingfuByExp(pGameBingfu->getItemID(),uPreCurExp);
	CHECKF(pNewModel != NULL);
	for (uint16 i = pOldModel->uBingfuLevel + 1; i < pNewModel->uBingfuLevel; ++i )
	{
		const _stBingfuModel* pTmpModel = g_clBingfuData.findBingfu(pGameBingfu->getItemID(),i);
		if (!pTmpModel)
			continue;

		m_pPlayer->delExpend(pTmpModel->mapNeedExpend,DoingType_BingfuUpgrade);
	}

	//����仯 �����ֳ���
	if (!pGameBingfu->testFlag(ItemFlag_IsInWulue))
	{
		if (1 == pGameBingfu->getItemNum())
		{
			pGameBingfu->setFlag(ItemFlag_Binded,true);
			pGameBingfu->updateBingfuExp(uPreCurExp,DoingType_BingfuUpgrade);
		}
		else
		{
			this->updateItemNum(pGameBingfu,-1,true, DoingType_BingfuUpgrade);

			_stItem stNewBingfu = pGameBingfu->getItem();
			stNewBingfu.uBingfuExp = uPreCurExp;
			stNewBingfu.uItemNum = 1;
			stNewBingfu.uItemSN = 0;
			stNewBingfu.setFlag(ItemFlag_Binded,true);
			addBingfu(stNewBingfu,DoingType_BingfuUpgrade);
			uSn = stNewBingfu.uItemSN;
		}
	}
	else //��������Ƕ�� ������Ҫ��������
	{
		pGameBingfu->setFlag(ItemFlag_Binded,true);
		pGameBingfu->updateBingfuExp(uPreCurExp,DoingType_BingfuUpgrade);

	
		//if (pOldModel != pNewModel && m_pPlayer->getPlayerWulue())
		//{
		//	m_pPlayer->getPlayerWulue()->calculateAttri();
		//}
	}

	//CPlayerQuestMgr* pQuestMgr = m_pPlayer->getPlayerQuestMgr();
	//if (pQuestMgr)
	//	pQuestMgr->callback_completeBingfuUpgrade();
	//���ؽ��
	send_BingfuUpgrade_Ack(uSn);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameItemMgr::sort_expbingfu	(const CGameItem* pLeft, const CGameItem* pRight)
{
	if ( !pLeft || !pRight )
		return false;

	if ( pLeft->getBingfuExp() != pRight->getBingfuExp())
		return pLeft->getBingfuExp() < pRight->getBingfuExp();

	return pLeft < pRight;
}
//-------------------------------------------------------------
//------------------------------ ��һ��
bool	CGameItemMgr::bingfuUpOneLevelAsk(uint64 uSn,uint8& uCode)
{
	if (!canOperate() || m_setExpBingfu.empty())
		return false;

	CGameItem* pGameBingfu = findItem(uSn);
	if (!pGameBingfu)
	{
		uCode = R_ResultItem_NoItem;
		return false;
	}

	const _stItemModel* pModel = pGameBingfu->getModel();
	if (!pModel || ItemType_BingFu != pModel->uType)
		return false;

	//���������������
	if (pModel->uSubType == BingfuType_Exp)
	{
		uCode = R_ResultItem_ExpBingfuNoUp;
		return false;
	}

	//����������Ҫ�ľ���
	
	const _stBingfuModel* pOldModel = g_clBingfuData.findBingfuByExp(pGameBingfu->getItemID(),pGameBingfu->getBingfuExp());
	CHECKF(pOldModel != NULL);

	uint32 uMaxExp = 0;
	const _stBingfuModel* pNextModel = g_clBingfuData.findBingfu(pModel->uItemID,pOldModel->uBingfuLevel + 1);
	if (pNextModel)
		uMaxExp = pNextModel->uTotalExp;

	uint32 uPreCurExp = pGameBingfu->getBingfuExp(); //Ԥ�������ӵ��ľ���
	if (uPreCurExp >= uMaxExp)
		return false;

	uint32 uCanAddExp = uMaxExp - uPreCurExp;
	uint32 uNewAddExp = 0;

	//�����С��������
	stl_vector<CGameItem*> vecExpBingfu;
	stl_set<CGameItem*>::iterator _pos = m_setExpBingfu.begin();
	for (; _pos != m_setExpBingfu.end(); ++_pos)
		vecExpBingfu.push_back(*_pos);
	std::sort(vecExpBingfu.begin(),vecExpBingfu.end(),CGameItemMgr::sort_expbingfu);

	//��ȡ�������
	VEC_EXPENDGAMEITEM vecExpend;

	//�Ӿ����ٵĿ�ʼѡȡ��ֱ�����뵱ǰ��������ľ���
	for (uint16 i = 0; i < vecExpBingfu.size(); ++i)
	{
		CGameItem* pTmpGameItem = vecExpBingfu[i];
		if (!pTmpGameItem || uNewAddExp >= uCanAddExp)
			continue;

		uint16 uTmpNum = (uint16)ceil(float32(uCanAddExp - uNewAddExp) / pTmpGameItem->getBingfuExp());
		uint16 uTmpAddNum = MIN(uTmpNum,pTmpGameItem->getItemNum());
		uNewAddExp += uTmpAddNum* pTmpGameItem->getBingfuExp();
		_stExpendGameItem stExpendGameItem;
		stExpendGameItem.pGameItem = pTmpGameItem;
		stExpendGameItem.uItemNum = uTmpAddNum;
		vecExpend.push_back(stExpendGameItem);
		if (uNewAddExp >= uCanAddExp)
			break;
	}

	//����ѡ���ı����У�����������Ӹߵ���ѡȡ������Ͷ��ľ��������
	for (int16 i = vecExpend.size() - 1; i >= 0;)
	{
		_stExpendGameItem& stExpendGameItem = vecExpend[i];
		if (!stExpendGameItem.pGameItem || uNewAddExp < uCanAddExp + stExpendGameItem.pGameItem->getBingfuExp())
		{
			i--;
			continue;
		}

		if (0 == stExpendGameItem.uItemNum)
		{
			i--;
			continue;
		}
		stExpendGameItem.uItemNum--;
		uNewAddExp -= stExpendGameItem.pGameItem->getBingfuExp();
		if (0 == stExpendGameItem.uItemNum)
		{
			i--;
		}
	}

	uPreCurExp += uNewAddExp;
	//�������ǰ������
	const _stBingfuModel* pNewModel = g_clBingfuData.findBingfuByExp(pGameBingfu->getItemID(),uPreCurExp);
	CHECKF(pNewModel != NULL);
	if (pNewModel != pOldModel)
	{
		if (!m_pPlayer->canPreCondition(pNewModel->dmapPreCondition))
		{
			uCode = R_ResultItem_PreCondition;
			return false;
		}

		if (!m_pPlayer->canDelExpend(pNewModel->mapNeedExpend))
		{
			uCode = R_ResultItem_PreCondition;
			return false;
		}

		m_pPlayer->delExpend(pNewModel->mapNeedExpend,DoingType_BingfuUpgrade);
	}


	//��ʼ�۾������
	for (uint32 i = 0;i < vecExpend.size(); ++i)
	{
		_stExpendGameItem& stExpendGameItem = vecExpend[i];
		if (!stExpendGameItem.pGameItem || 0 ==stExpendGameItem.uItemNum)
			continue;

		CHECKF(this->updateItemNum(stExpendGameItem.pGameItem,(int32)stExpendGameItem.uItemNum * -1,true,DoingType_BingfuUpgrade));
	}

	//����仯 �����ֳ���
	if (!pGameBingfu->testFlag(ItemFlag_IsInWulue))
	{
		if (1 == pGameBingfu->getItemNum())
		{
			pGameBingfu->setFlag(ItemFlag_Binded,true);
			pGameBingfu->updateBingfuExp(uPreCurExp,DoingType_BingfuUpgrade);
		}
		else
		{
			this->updateItemNum(pGameBingfu,-1,true, DoingType_BingfuUpgrade);

			_stItem stNewBingfu = pGameBingfu->getItem();
			stNewBingfu.uBingfuExp = uPreCurExp;
			stNewBingfu.uItemNum = 1;
			stNewBingfu.uItemSN = 0;
			stNewBingfu.setFlag(ItemFlag_Binded,true);
			addBingfu(stNewBingfu,DoingType_BingfuUpgrade);
			uSn = stNewBingfu.uItemSN;
		}
	}
	else //��������Ƕ�� ������Ҫ��������
	{
		pGameBingfu->setFlag(ItemFlag_Binded,true);
		pGameBingfu->updateBingfuExp(uPreCurExp,DoingType_BingfuUpgrade);

		//if (pOldModel != pNewModel && m_pPlayer->getPlayerWulue())
		//{
		//	m_pPlayer->getPlayerWulue()->calculateAttri();
		//}
	}

	//���ؽ��
	send_BingfuUpgrade_Ack(uSn);
	return true;
}

//-------------------------------------------------------------
//------------------------------ �����ֽ�
bool	CGameItemMgr::bingfuSplitAsk(MAP_U64_U16& mapSplit,uint8& uCode)
{
	if (!canOperate() || mapSplit.empty())
		return false;

	uint64 uAddScore = 0;

	//Ԥ�������ӵ�����Ϊ
	ITER_MAP_U64_U16 _pos = mapSplit.begin();
	for (; _pos != mapSplit.end(); ++_pos)
	{
		//�Ƿ��ܷ�۳� ��ʹ���˲���ɾ��
		CGameItem* pGameBingfu = findItem(_pos->first);
		if (!pGameBingfu || pGameBingfu->testFlag(ItemFlag_IsInWulue) || pGameBingfu->getItemType() != ItemType_BingFu)
			continue;

		//�����Ƿ����
		uint16 uExpendNum = MIN(pGameBingfu->getItemNum(),_pos->second);
		uint64 uAddValue = MAX((uint64)pGameBingfu->getBingfuExp() * g_clConstantData.Item_EnhanceMoneyCount / 100,(uint64)1);
		uAddScore += (uAddValue * uExpendNum);
		CHECKC(this->updateItemNum(pGameBingfu,(int32)uExpendNum * -1,true,DoingType_Item_Split));
	}

	//�ӱ������龭��
	m_pPlayer->addExpend(MoneyType_BingfuExp,uAddScore,DoingType_Item_Split);

	//֪ͨ�ͻ��˷ֽ�ɹ�
	send_BingfuSplit_Ack(R_ResultBingfu_Succeed);
	return true;
}
//-------------------------------------------------------------
//------------------------------
bool	CGameItemMgr::addBingfu	(_stItem& stItem,_emDoingType emDoing)
{
	//�Ȳ����ܲ��ܵ���
	const _stItemModel* pItemModel = g_clItemData.find(stItem.uItemID);
	CHECKF(pItemModel != NULL);

	uint16 uLeftNum = stItem.uItemNum;
	CGameItem* pGameItem = findNoFullBingfu(stItem);
	if (pGameItem)
	{
		uint32 uCanAddItem = pGameItem->getSpareNum();
		uCanAddItem = MIN((uint32)stItem.uItemNum,uCanAddItem);
		CHECKF(updateItemNum(pGameItem,uCanAddItem,true,emDoing));
		uLeftNum -= uCanAddItem;
	}
	
	if (0 == uLeftNum)
		return true;

	const _stItemModel* pModel = g_clItemData.find(stItem.uItemID);
	CHECKF(pModel != NULL);
	while(uLeftNum > 0)
	{
		uint16 uCanAddItem = MIN(pModel->uMaxCount,uLeftNum);
		uLeftNum -= uCanAddItem;
		stItem.uItemNum = uCanAddItem;
		CHECKF(addNewItem(stItem,true,emDoing) != NULL);
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool	CGameItemMgr::equipBingfu	(CGameItem* pGameItem,bool bEquip)
{
	if (!pGameItem || pGameItem->getItemType() != ItemType_BingFu)
		return false;

	if (bEquip)
	{
		if (pGameItem->testFlag(ItemFlag_IsInWulue))
			return false;

		if (pGameItem->getItemNum() > 1)
		{
			//�ȿ� ������
			updateItemNum(pGameItem,-1,true,DoingType_Item_Move);

			_stItem stItem = pGameItem->getItem();
			stItem.setFlag(ItemFlag_IsInWulue,true);
			stItem.setFlag(ItemFlag_Binded,true);
			stItem.uItemNum = 1;
			stItem.uItemSN = 0;
			addNewItem(stItem,true,DoingType_Item_Move);
		}
		else
		{
			pGameItem->setFlag(ItemFlag_IsInWulue,true);
			pGameItem->setFlag(ItemFlag_Binded,true);

			//ͬ���ͻ���
			send_UpdateItemAck(pGameItem);
		}

	}
	else//�ѱ���
	{
		if (!pGameItem->testFlag(ItemFlag_IsInWulue))
			return false;

		pGameItem->setFlag(ItemFlag_IsInWulue,false);
		CGameItem* pAddBingfu = findNoFullBingfu(pGameItem->getItem());
		if (pAddBingfu && pAddBingfu != pGameItem)
		{
			updateItemNum(pGameItem,-1,true,DoingType_Item_Move);
			updateItemNum(pAddBingfu,1,true,DoingType_Item_Move);
		}
		else
		{
			//ͬ���ͻ���
			send_UpdateItemAck(pGameItem);
		}
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameItemMgr::compoundAsk (uint32 uRecipeID,MAP_U64_U16& mapItem,uint8& uCode)
{
	if (!canOperate() || mapItem.empty())
		return false;

	const _stItemCompoundModel* pCompoundModel = g_clItemData.findCompound(uRecipeID);
	if (!pCompoundModel)
		return false; 

	MAP_U32_U16 mapCurNum;
	stl_map<CGameItem*,uint16> mapDelItem;
	ITER_MAP_U64_U16 _pos = mapItem.begin();
	for (; _pos != mapItem.end(); ++_pos)
	{
		CGameItem* pGameItem = findItem(_pos->first);
		if (!pGameItem)
			continue;

		_pos->second = MIN(_pos->second,pGameItem->getItemNum());
		mapDelItem.insert_(pGameItem,_pos->second);
		mapCurNum[pGameItem->getItemID()] += _pos->second;
	}

	{//����䷽
		CONST_ITER_DMAP_NEED_ITEM _pos = pCompoundModel->mapNeedItem.begin();
		for (; _pos != pCompoundModel->mapNeedItem.end(); ++_pos)
		{
			const DMAP_NEED_ITEM::_sub_map& stSubmap = _pos->second;
			DMAP_NEED_ITEM::sub_const_iterator _spos = stSubmap.begin();
			for (; _spos != stSubmap.end(); ++_spos)
			{
				uint16* pNum = mapCurNum.find_(_pos->first);
				if (!pNum || *pNum != _spos->first)
				{
					uCode = R_ResultItem_NotEnoughCompound;
					return false;
				}
			}
		}
	}

	//�۵���
	bool bBind = false;
	{
		stl_map<CGameItem*,uint16>::iterator _pos = mapDelItem.begin();
		for (; _pos != mapDelItem.end(); ++_pos)
		{
			CGameItem* pGameItem = _pos->first;
			CHECKC(pGameItem != NULL);
			if (pGameItem->isBind())
				bBind = true;
			CHECKC(updateItemNum(pGameItem,(int32)_pos->second * -1,true,DoingType_Compound));
		}
	}

	//�ӵ���
	uint32 uAddItemID = pCompoundModel->findRandItem();
	addItem(uAddItemID,1,bBind,true,true,DoingType_Compound);
	send_Compound_Ack(uAddItemID);
	return true;
}

//-------------------------------------------------------------
//------------------------------ 
void CGameItemMgr::_stEquipMakeSkilled::checkUpgrade()
{
	uint32 uNextLevelNeed = g_clEquipData.getSkilledLevelPoint(uSkilledLevel + 1);
	if (uNextLevelNeed > 0 && uSkilledPoint >= uNextLevelNeed)
	{
		uSkilledLevel ++;
		uSkilledPoint -= uNextLevelNeed;
		if (g_clEquipData.getSkilledLevelPoint(uSkilledLevel + 1) == 0)
		{
			//������ܼ�������,��رջ�ȡ����ֵ.
			uSkilledPoint = 0;
		}
	}
}
//-------------------------------------------------------------
//------------------------------ 
void CGameItemMgr::_stEquipMakeSkilled::load(const PBEquipMakeSkilled& pb)
{
	uSkilledLevel = pb.skilledlevel();
	uSkilledPoint = pb.skilledpoint();
	uEquipType	  = pb.equiptype();
}
//-------------------------------------------------------------
//------------------------------ w
void CGameItemMgr::_stEquipMakeSkilled::save(PBEquipMakeSkilled& pb)
{
	pb.set_equiptype(uEquipType);
	pb.set_skilledlevel(uSkilledLevel);
	pb.set_skilledpoint(uSkilledPoint);
}
//-------------------------------------------------------------
//------------------------------ 
void CGameItemMgr::_stEquipMakeVigour::load(const PBEquipMakeVigour& pb)
{
	uVigour			= pb.vigour();
	uLastAddTime	= pb.lastaddtime();
	uVigourFrozen	= pb.vigourfrozen();
}
//-------------------------------------------------------------
//------------------------------ 
void CGameItemMgr::_stEquipMakeVigour::save(PBEquipMakeVigour & pb)
{
	pb.set_vigour(uVigour);
	pb.set_lastaddtime(uLastAddTime);
	pb.set_vigourfrozen(uVigourFrozen);
}
//-------------------------------------------------------------
//------------------------------ 
void CGameItemMgr::_stEquipMakeOEM::load(const  PBEquipMakeOEM& pb)
{
	uMakeID			= pb.makeid();
	uFee			= pb.fee();
	uCount			= pb.count();
	uCountUsed		= pb.countused();
	vcAllMakeIDs.clear();
	for (int32 i = 0; i< pb.validmakeid_size(); ++i)
	{
		vcAllMakeIDs.push_back(pb.validmakeid(i));
	}
}
//-------------------------------------------------------------
//------------------------------ 
void CGameItemMgr::_stEquipMakeOEM::save(PBEquipMakeOEM& pb)
{
	pb.set_makeid(uMakeID);
	pb.set_fee(uFee);
	pb.set_count(uCount);
	pb.set_countused(uCountUsed);
	pb.set_countreal(uCount - uCountUsed);
	for (uint32 i = 0; i < vcAllMakeIDs.size(); ++i)
	{
		pb.add_validmakeid(vcAllMakeIDs[i]);
	}
}
//-------------------------------------------------------------
//------------------------------ 
bool CGameItemMgr::openMakeEquipDlgAsk				(uint8& uCode)
{
	uCode = R_ResultItem_Succeed;
	PBG2CItemOpenEquipMakeDlgAck ack;
	calcVigour();
	ack.set_vigour(m_stMakeVigour.uVigour);
	{
		MAP_SKILLED::iterator iter = m_mpSkilled.begin();
		for (; iter!= m_mpSkilled.end(); ++iter)
		{
			PBEquipMakeSkilled * pb = ack.add_skilledinfos();
			if (pb)
			{
				iter->second.save(*pb);
			}
		}
	}
	return send_OpenMakeDlgAck(&ack, uCode);
}
//-------------------------------------------------------------
//------------------------------ 
bool CGameItemMgr::openMakeEquipOemDlgAsk			(uint8& uCode)
{
	uCode = R_ResultItem_Succeed;
	PBG2CItemOpenEquipMakeOEMDlgAck ack;
	calcVigour();
	ack.set_vigour(m_stMakeVigour.uVigour - m_stMakeVigour.uVigourFrozen);
	ack.set_vigourmax(g_clEquipData.getVigourMaxByLevel(m_pPlayer->getLevel()) - m_stMakeVigour.uVigourFrozen);
	{
		MAP_OEM::iterator iter = m_mpOEMs.begin();
		for (; iter!= m_mpOEMs.end(); ++iter)
		{
			PBEquipMakeOEM * pb = ack.add_oem();
			if (pb)
			{
				iter->second.save(*pb);
			}
		}
	}
	return send_OpenMakeOEMDlgAck(&ack, uCode);
}
//-------------------------------------------------------------
//------------------------------ 
bool CGameItemMgr::changeOemFeeAsk					(uint32 uMakeID,uint32 uFee, uint8& uCode)
{
	uCode = R_ResultItem_Succeed;
	_stEquipMakeOEM * p = findOEMByRealID(uMakeID);
	if (p)
	{//��������
		p->uFee = uFee;
		return true;
	}
	uCode = R_ResultItem_Fail;
	return false;
}

//-------------------------------------------------------------
//------------------------------ 
bool CGameItemMgr::commitMakeEquipOemAsk			(const PBC2GItemCommitEquipMakeOEMAsk & pb, uint8& uCode)
{
	uCode = R_ResultItem_Succeed;

	//�����������0,��ô��ʾȡ�����е�Ԥ��
	//uint32 uNeedVigour = 0;
	if (pb.isclearall())
	{//ɾ�����д���
		m_mpOEMs.clear();
		m_stMakeVigour.uVigourFrozen = 0;
		//ˢ�����
		//openMakeEquipOemDlgAsk(uCode);
		return true;
	}
	else if (pb.isremoved())
	{//ɾ��һ������
		_stEquipMakeOEM * p = findOEMByRealID(pb.oem().makeid());
		if (p)
		{
			MAP_OEM::iterator iter = m_mpOEMs.find(p->uMakeID);
			const _stEquipMakeModel* pModel = g_clEquipData.findMakeModel(p->uMakeID);
			if (pModel && iter != m_mpOEMs.end())
			{
				//���ض���Ļ���
				m_stMakeVigour.uVigourFrozen -= pModel->uVigourCost * (p->uCount - p->uCountUsed);
				//ɾ����¼
				m_mpOEMs.erase(iter);
				return true;
			}
		}
	}
	else
	{//�����ϼ�
		//������
		const _stEquipMakeModel* pModel = g_clEquipData.findMakeModel(pb.oem().makeid());
		if(pModel)
		{

			//���ȼ����
			_stEquipMakeSkilled*pSkilled = findSkilledByType((_emEquipType)(pModel->uProductType));
			if (!pSkilled 
				|| pModel->uSkilledLevel > pSkilled->uSkilledLevel)
			{
				//����һ������ �ȼ�����
				uCode = R_ResultItem_NoSkilled;
				return false;
			}
			_stEquipMakeOEM * p = findOEMByRealID(pb.oem().makeid());
			uint32 uReturnVigour = 0;
			if (p)
			{
				uReturnVigour = pModel->uVigourCost * (p->uCount - p->uCountUsed);
			}

			uint32 needVigourNew = pModel->uVigourCost * pb.oem().count();
			if (m_stMakeVigour.uVigour < needVigourNew + m_stMakeVigour.uVigourFrozen - uReturnVigour)
			{
				//����һ������, ��������.
				uCode = R_ResultItem_NoVigour;
				return false;
			}
			//�������
			m_stMakeVigour.uVigourFrozen += needVigourNew;
			if (p)
			{
				//��������,�ĳ�ɾ��
				//p->uFee = pb.oem().fee();
				//p->uCount += pb.oem().count();


				//���ض���Ļ���
				m_stMakeVigour.uVigourFrozen -= pModel->uVigourCost * (p->uCount - p->uCountUsed);
				//ɾ����¼
				m_mpOEMs.erase(pb.oem().makeid());

			}
			{//����
				_stEquipMakeOEM st;st.init();
				st.load(pb.oem());
				const CEquipData::VECTOR_MAKEIDS*p = g_clEquipData.getMakeIDSBySkilledLevelPart(pModel->uProductType, pModel->uSkilledLevel);
				if (p)
				{
					//�������ϴ����ȫ��Ͱ
					st.vcAllMakeIDs = *p;
				}
				m_mpOEMs[st.uMakeID] = st;
			}
			return true;
		}

	}
	//else
	//{
	//	if (pb.isremoved())
	//	{
	//	}
	//	else
	//	{
	//		//�������
	//		//for (int32 i = 0; i<pb.oem_size(); ++i)
	//		//{
	//		//	const PBEquipMakeOEM& oem = pb.oem(i);
	//		//	const _stEquipMakeModel* pModel = g_clEquipData.findMakeModel(oem.makeid());
	//		//	if (pModel)
	//		//	{
	//		//		uNeedVigour += pModel->uVigourCost * oem.count();
	//		//		//���ȼ����
	//		//		_stEquipMakeSkilled*pSkilled = findSkilledByType((_emEquipType)(pModel->uProductType));
	//		//		if (!pSkilled 
	//		//			|| pModel->uSkilledLevel > pSkilled->uSkilledLevel)
	//		//		{
	//		//			//����һ������ �ȼ�����
	//		//			uCode = R_ResultItem_NoSkilled;
	//		//			return false;
	//		//		}
	//		//	}
	//		//	else
	//		//	{
	//		//		//����һ������ û�������ʽ
	//		//		uCode = R_ResultItem_NoMakeId;
	//		//		return false;
	//		//	}
	//		//}
	//		////
	//		//if (m_stMakeVigour.uVigour < uNeedVigour + m_stMakeVigour.uVigourFrozen)
	//		//{
	//		//	//����һ������, ��������.
	//		//	uCode = R_ResultItem_NoVigour;
	//		//	return false;
	//		//}
	//		//for (int32 i = 0; i<pb.oem_size(); ++i)
	//		//{
	//		//	const PBEquipMakeOEM& oem = pb.oem(i);
	//		//	_stEquipMakeOEM st;st.init();
	//		//	st.load(oem);
	//		//	const _stEquipMakeModel* pModel = g_clEquipData.findMakeModel(st.uMakeID);
	//		//	if (pModel)
	//		//	{
	//		//		const CEquipData::VECTOR_MAKEIDS*p = g_clEquipData.getMakeIDSBySkilledLevelPart(pModel->uProductType, pModel->uSkilledLevel);
	//		//		if (p)
	//		//		{
	//		//			//�������ϴ����ȫ��Ͱ
	//		//			st.vcAllMakeIDs = *p;
	//		//		}
	//		//	}
	//		//	m_mpOEMs[st.uMakeID] = st;
	//		//}
	//		////�������
	//		//m_stMakeVigour.uVigourFrozen += uNeedVigour;

	//	}
	//}
	uCode = R_ResultItem_Fail;
	return false;
}
//-------------------------------------------------------------
//------------------------------ 
bool CGameItemMgr::buildOEMAck						(PBG2CItemVisitEquipMakeOEMAck& ack)
{
	MAP_OEM::iterator iter = m_mpOEMs.begin();
	for (; iter!= m_mpOEMs.end(); ++iter)
	{
		PBEquipMakeOEM * pb = ack.add_oem();
		if (pb)
		{
			iter->second.save(*pb);
		}
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------ 
void CGameItemMgr::newPlayerSkilled				()
{
	if (m_mpSkilled.size() == 0)
	{
		for (int32 i = EquipType_Weapon; i <= EquipType_Shoe; ++i)
		{
			if (i != EquipType_Mount)
			{
				_stEquipMakeSkilled  st;
				st.init();
				st.uEquipType = i;
				m_mpSkilled.insert_(st.uEquipType, st);
			}
		}
	}
}

//-------------------------------------------------------------
//------------------------------ 
bool CGameItemMgr::visitMakeEquipOemAsk			(uint32 uPlayerID, uint8& uCode)
{
	uCode = R_ResultItem_Succeed;

	if (uPlayerID == m_pPlayer->getPlayerID())
	{
		uCode = R_ResultItem_Fail;
		return false;
	}
	CScenePlayer* pMaker = g_pGamePlayerMgr->findPlayer(uPlayerID);
	if (! pMaker)
	{
		uCode = R_ResultItem_OEM_Not_Online;
		return false;
	}
	PBG2CItemVisitEquipMakeOEMAck ack;
	pMaker->getGameItemMgr().buildOEMAck(ack);
	send_VisitMakeOEMAck(&ack, uCode);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool CGameItemMgr::makeEquipOemAsk					(uint32 uPlayerID, uint32 uMakeIndexID, uint32 uMakeID, uint8& uCode)
{
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool CGameItemMgr::send_OpenMakeDlgAck				(PBG2CItemOpenEquipMakeDlgAck* pb, uint8 uCode)
{
	return m_pPlayer->sendMsg(P_S2C_Protocol_Item,S2C_Item_OpenEquipMakeDlg,pb,uCode);
}
//-------------------------------------------------------------
//------------------------------ 
bool CGameItemMgr::send_OpenMakeOEMDlgAck			(PBG2CItemOpenEquipMakeOEMDlgAck* pb, uint8 uCode)
{
	return m_pPlayer->sendMsg(P_S2C_Protocol_Item,S2C_Item_OpenEquipMakeOEMDlg,pb,uCode);
}
//-------------------------------------------------------------
//------------------------------ 
bool CGameItemMgr::send_CommitMakeOEMAck			(PBG2CItemCommitEquipMakeOEMAck* pb, uint8 uCode)
{
	return m_pPlayer->sendMsg(P_S2C_Protocol_Item,S2C_Item_CommitEquipMakeOEM,pb,uCode);
}
//-------------------------------------------------------------
//------------------------------ 
bool CGameItemMgr::send_VisitMakeOEMAck			(PBG2CItemVisitEquipMakeOEMAck* pb, uint8 uCode)
{
	return m_pPlayer->sendMsg(P_S2C_Protocol_Item,S2C_Item_VisitEquipMakeOEM,pb,uCode);
}
//-------------------------------------------------------------
//------------------------------ 
bool CGameItemMgr::send_MakeEquipOEMAck			(PBG2CItemEquipMakeOEMAck* pb, uint8 uCode)
{
	return m_pPlayer->sendMsg(P_S2C_Protocol_Item,S2C_Item_EquipMakeOEM,pb,uCode);
}
//-------------------------------------------------------------
//------------------------------ 
bool CGameItemMgr::send_ChangeOEMFeeAck			(PBG2CItemEquipChangeOEMFeeAck*pb, uint8 uCode )
{
	return m_pPlayer->sendMsg(P_S2C_Protocol_Item,S2C_Item_EquipChangeOEMFee,pb,uCode);
}


//-------------------------------------------------------------
//------------------------------ 
bool CGameItemMgr::calcVigour						()
{
	if (m_stMakeVigour.uLastAddTime == 0)
	{
		m_stMakeVigour.uLastAddTime = getHourBeginTime(getTime());
	}
	uint32 uDiffhour = diffTimeHour(m_stMakeVigour.uLastAddTime, getTime());
	const _stequip_make_vigourModel* pModel =	g_clEquipData.getVigourModel(m_pPlayer->getLevel());
	if (pModel)
	{
		m_stMakeVigour.uVigour += uDiffhour*pModel->uRecoverPerHour;
		if (m_stMakeVigour.uVigour > pModel->uVigourMax)
		{
			m_stMakeVigour.uVigour = pModel->uVigourMax;
		}
	}
	else
	{
		return false;
	}

	m_stMakeVigour.uLastAddTime = getHourBeginTime(getTime());
	return  true;
}

//-------------------------------------------------------------
//------------------------------ 
CGameItemMgr::_stEquipMakeSkilled* CGameItemMgr::findSkilledByType( _emEquipType uType)
{
	MAP_SKILLED::iterator iter = m_mpSkilled.find(uType);
	if (iter == m_mpSkilled.end()
		&& uType>= EquipType_Weapon
		&& uType <= EquipType_Shoe)
	{
		//û�оͲ���
		_stEquipMakeSkilled  st;
		st.init();
		st.uEquipType = uType;
		m_mpSkilled.insert_(st.uEquipType, st);
	}
	return m_mpSkilled.find_(uType);
}
//-------------------------------------------------------------
//------------------------------ 
CGameItemMgr::_stEquipMakeOEM*	 CGameItemMgr::findOEMByID		(uint32 uMakeID)
{
	return m_mpOEMs.find_(uMakeID);
}
//-------------------------------------------------------------
//------------------------------ 
CGameItemMgr::_stEquipMakeOEM*	 CGameItemMgr::findOEMByRealID	(uint32 uRealMakeID)//ͬһ������,���Դ��첻ְͬҵ��ͬһ��װ��,
{
	MAP_OEM::iterator iter = m_mpOEMs.begin();
	for (; iter != m_mpOEMs.end(); ++iter)
	{
		_stEquipMakeOEM & st = iter->second;
		for (uint32 i = 0; i < st.vcAllMakeIDs.size(); ++i)
		{
			if (st.vcAllMakeIDs[i] == uRealMakeID)
			{
				return &(iter->second);
			}
		}
	}
	return NULL;
}

//-------------------------------------------------------------
//------------------------------ 
bool CGameItemMgr::preCheckMakeSkilled					(uint32 uMakeID, uint8& uCode)
{
	const _stEquipMakeModel* pMakeModel = g_clEquipData.findMakeModel(uMakeID);
	CHECKF(pMakeModel != NULL);

	//����ֵ
	if (m_stMakeVigour.uVigour - m_stMakeVigour.uVigourFrozen < pMakeModel->uVigourCost)
	{
		uCode = R_ResultItem_NoVigour;
		return false;
	}
	//�������ж�
	_stEquipMakeSkilled* pSkilled = findSkilledByType((_emEquipType)pMakeModel->uProductType);
	if (!pSkilled)
	{
		uCode = R_ResultItem_NoSkilled;
		return false;
	}
	if ( pSkilled->uSkilledLevel >= pMakeModel->uSkilledLevel
		&& pSkilled->uSkilledPoint  >= pMakeModel->uSkilledNum)
	{
	}
	else
	{
		uCode = R_ResultItem_NoSkilled;
		return false;		
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool CGameItemMgr::equipMakeSuccessAddSkilled		(uint32 uMakeID)
{
	const _stEquipMakeModel* pMakeModel = g_clEquipData.findMakeModel(uMakeID);
	CHECKF(pMakeModel != NULL);

	//�۵�����
	m_stMakeVigour.uVigour -= pMakeModel->uVigourCost;
	//����������,����Ƿ�����. ǰ�����,һ������
	_stEquipMakeSkilled* pSkilled = findSkilledByType((_emEquipType)pMakeModel->uProductType);

	if (pSkilled)
	{
		uint32 uNextLevelNeed = g_clEquipData.getSkilledLevelPoint(pSkilled->uSkilledLevel + 1);
		if (uNextLevelNeed > 0)
		{
			pSkilled->uSkilledPoint += pMakeModel->uGetSkilled;
			pSkilled->checkUpgrade();
		}
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool CGameItemMgr::preCheckMakeOEMSkilled			(uint32 uRealMakeID, CScenePlayer* pBuyer, uint8& uCode)
{
	const _stEquipMakeModel* pMakeModel = g_clEquipData.findMakeModel(uRealMakeID);
	CHECKF(pMakeModel != NULL);
	CHECKF(pBuyer != NULL);
	//������ֵ
	const _stEquipMakeOEM* pOEM = findOEMByRealID(uRealMakeID);
	if (!pOEM)
	{
		uCode = R_ResultItem_OEM_Not_Enough;
		return false;
	}
	if (pOEM->uCount <= pOEM->uCountUsed)
	{
		uCode = R_ResultItem_OEM_Not_Enough;
		return false;
	}

	if (pOEM->uFee + pMakeModel->uNeedMoney > pBuyer->getMoney(MoneyType_Sliver))
	{
		uCode = R_ResultItem_OEM_Not_Enough_Fee;
		return false;
	}

	//����ֵ,��Ҫ�Ӷ���ֵ����۳�
	if (m_stMakeVigour.uVigourFrozen < pMakeModel->uVigourCost)
	{
		uCode = R_ResultItem_NoVigour;
		return false;
	}
	//�������ж�
	_stEquipMakeSkilled* pSkilled = findSkilledByType((_emEquipType)pMakeModel->uProductType);
	if (!pSkilled)
	{
		uCode = R_ResultItem_NoSkilled;
		return false;
	}
	if ( pSkilled->uSkilledLevel >= pMakeModel->uSkilledLevel
		&& pSkilled->uSkilledPoint  >= pMakeModel->uSkilledNum)
	{
	}
	else
	{
		uCode = R_ResultItem_NoSkilled;
		return false;		
	}
	return true;

}
//-------------------------------------------------------------
//------------------------------ 
bool CGameItemMgr::oemMakeSuccessAddSkilled		(uint32 uRealMakeID, CScenePlayer* pBuyer)
{
	const _stEquipMakeModel* pMakeModel = g_clEquipData.findMakeModel(uRealMakeID);
	CHECKF(pMakeModel != NULL);

	//�۵�����,��Ҫ�۳�����ֵ
	m_stMakeVigour.uVigour			-= pMakeModel->uVigourCost;
	m_stMakeVigour.uVigourFrozen	-= pMakeModel->uVigourCost;

	_stEquipMakeOEM* pOEM = findOEMByRealID(uRealMakeID);
	if (!pOEM)
	{
		//Ӧ�ò�������������
		return false;
	}
	pOEM->uCountUsed ++;
	//��ô�����
	pBuyer->addMoney(MoneyType_Sliver, pOEM->uFee* -1, DoingType_EquipOEM);
	m_pPlayer->addMoney(MoneyType_Sliver, pOEM->uFee, DoingType_EquipOEM);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
uint32 CGameItemMgr::getMakeVigour					()
{
	return m_stMakeVigour.uVigour;
}

//-------------------------------------------------------------
//------------------------------ 
bool CGameItemMgr::newplayerMakeVigour				()
{
	const _stequip_make_vigourModel* pModel =	g_clEquipData.getVigourModel(m_pPlayer->getLevel());
	if (pModel)
	{
		m_stMakeVigour.uVigour = pModel->uVigourMax;
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool CGameItemMgr::addMakeVigour					(int32 nAddValue,_emDoingType emDoing)
{
	const _stequip_make_vigourModel* pModel =	g_clEquipData.getVigourModel(m_pPlayer->getLevel());
	if (pModel)
	{
		int32 nT = (int32)m_stMakeVigour.uVigour;
		nT += nAddValue;
		if (nT > (int32)pModel->uVigourMax)
		{
			nT = (int32)pModel->uVigourMax;
		}

		if (nT < 0)
		{
			nT = 0;
		}
		m_stMakeVigour.uVigour = (uint32)nT;
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool CGameItemMgr::onGmsetSkilled					(_emEquipType uType, uint32 uLevel, uint32 uPoint)
{
	_stEquipMakeSkilled* p = findSkilledByType((_emEquipType)uType);
	if (p)
	{
		p->uSkilledLevel = uLevel;
		p->uSkilledPoint = uPoint;
		return true;
	}
	return false;
}
//-------------------------------------------------------------
//------------------------------ 
bool CGameItemMgr::oemCheckRemove					(uint32 uMakeID)
{
	_stEquipMakeOEM * p = findOEMByRealID(uMakeID);
	if (p && p->uCount <= p->uCountUsed)
	{//���������� ���
		MAP_OEM::iterator iter = m_mpOEMs.find(p->uMakeID);
		if (iter != m_mpOEMs.end())
		{
			//ɾ����¼
			m_mpOEMs.erase(iter);
			return true;
		}
	}
	return true;
}
