/*----------------- itemFactory.cpp
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/4/12 11:52:06
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "gameLogic/gameItem/itemFactory.h"
#include "gameLogic/sceneObject/scenePlayer.h"
#include "basic/systemFunction.h"
#include "cfg_data/itemData/itemData.h"
#include "log/log.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 创建一个新的道具ID
CGameItem*	CItemFactory::buildItem(CScenePlayer* pPlayer, const _stItemModel* pModel)
{
	if(!pPlayer || !pModel)
		return NULL;

	uint64 uItemSN = buildItemSN(pPlayer->getPlayerID(),pPlayer->getItemSequence());
	while(Parent::find(uItemSN))
	{
		CLog::error("生成ItemSN重复:%llu",uItemSN);
		uItemSN = this->buildItemSN(pPlayer->getPlayerID(),pPlayer->getItemSequence());
	}

	CGameItem* pItem= Parent::allocate(uItemSN);
	if(!pItem)
		return NULL;

	pItem->build(uItemSN, pModel,pPlayer);
	pItem->getItem().setFlag(ItemFlag_Sequence,true);

	//是否获得开始计时
	if (pModel->check_mark(ItemMark_GetTimeLimit))
	{
		pItem->getItem().uEndtime = getTime() + pModel->uExpireTime;
	}

	return pItem;
}

//-------------------------------------------------------------
//------------------------------ 已知属性的道具
CGameItem*	CItemFactory::buildItem(CScenePlayer* pPlayer, _stItem& stItem, const _stItemModel*pModel,_emDoingType emDoing)
{
	if(!stItem.uItemID || !pPlayer)
		return NULL;

	if(!stItem.testFlag(ItemFlag_Sequence))
		stItem.uItemSN = 0;
	uint64 uItemSN = stItem.uItemSN;
	if(!uItemSN)
	{
		uItemSN = this->buildItemSN(pPlayer->getPlayerID(),pPlayer->getItemSequence());
		while(Parent::find(uItemSN))
		{
			CLog::error("生成ItemSN重复:%llu",uItemSN);
			uItemSN = this->buildItemSN(pPlayer->getPlayerID(),pPlayer->getItemSequence());
		}
	}

	CGameItem* pGameItem= Parent::allocate(uItemSN);
	if (!pGameItem)
		return NULL;
	pGameItem->build(uItemSN,pModel,pPlayer);
	stItem.setFlag(ItemFlag_Sequence,true);
	stItem.uItemSN = uItemSN;
	pGameItem->setBuild(stItem);
	return pGameItem;
}

//-------------------------------------------------------------
//------------------------------
void CItemFactory::releaseItem(CGameItem* pItem)
{
	if (pItem)
		Parent::release(pItem->getItemSN());
}
//-------------------------------------------------------------
//------------------------------ 
uint64	CItemFactory::buildItemSN(uint32 uPlayerID, uint32& uSequence)
{
	if(!uPlayerID)
		return 0;

	static _U_NUMBER _uNumber;
	_uNumber._STREAM.uPlayerId = uPlayerID;
	_uNumber._STREAM.uSequence = ++uSequence;
	return _uNumber.uItemSN;
}
