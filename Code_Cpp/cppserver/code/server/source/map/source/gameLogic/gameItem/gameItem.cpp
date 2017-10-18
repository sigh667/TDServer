/*----------------- gameItem.cpp
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/4/13 15:47:53
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "gameLogic/gameItem/gameItem.h"
#include "basic/systemFunction.h"
#include "basic/timeFunction.h"
#include "basic/basicTypes.h"
#include "log/log.h"
#include "cfg_data/equipData/equipData.h"
#include "cfg_data/bingfuData/bingfuData.h"
#include "game_log.h"
#include "gameLogic/sceneObject/scenePlayer.h"
#include "gameLogic/playerEquipColumn/playerEquipColumn.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
CGameItem::CGameItem()
{
	build(0,NULL,NULL);
}

//-------------------------------------------------------------
//------------------------------ 
void	CGameItem::initialize()
{
	m_stItem.initialize();
	m_pModel = NULL;
	m_pPlayer = NULL;
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameItem::build(uint64 uSN, const _stItemModel*pModel,CScenePlayer* pPlayer)
{
	m_pModel = pModel;
	m_stItem.initialize();
	if (!m_pModel || !pPlayer)
		return;
	m_pPlayer = pPlayer;
	m_stItem.uItemSN	= uSN;				/*流水号*/ 
	m_stItem.uItemID	= pModel->uItemID;	/*物品ID*/ 
	m_stItem.uItemNum	= 0;				/*物品数量*/ 
	m_stItem.uBingfuExp = pModel->uBingfuExp;//兵符经验
}

//-------------------------------------------------------------
//------------------------------ 
bool	CGameItem::setBuild	(_stItem&stItem)
{
	m_stItem.getItem() = stItem.getItem();
	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CGameItem::updateItem(_stItem&stItem)
{
	CHECKF(m_pModel != NULL);
	if ( m_pModel->uItemID != stItem.uItemID  )
		return false;

	m_stItem.getItem()	= stItem.getItem();
	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CGameItem::addGameAttri(_stGameAttribute& stGameAttr)
{
	if (!m_pModel || m_pModel->vecAttribute.empty())
		return false;

	//基础属性
	for(stl_vector<_stGameAttribute>::size_type i=0; i<m_pModel->vecAttribute.size(); ++i)
	{
		stGameAttr += m_pModel->vecAttribute[i];
	}

	//极品属性
	for (uint8 i = 0; i < m_stItem.vecSpecialID.size(); ++i)
	{
		const _stEquipSpecailAttrModel* pModel = g_clEquipData.findSpecailAttrModel(m_stItem.vecSpecialID[i]);
		;
		if (pModel 
			&& m_pPlayer->getPlayerEquipColumn()
			/*&&getForgeLevel()*/
			&& m_pPlayer->getPlayerEquipColumn()->getForgeLevel(m_pModel->uSubType)
				>= pModel->uNeedForgeLevel
			)
			stGameAttr += pModel->stAttri;
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameItem::addEnhanceExp(uint32 uAddExp,_emDoingType emDoing)
{
	if (!m_pModel || !m_pPlayer)
		return false;

	//等级最大经验
	uint32 uMaxItemEnhanceExp = g_clEquipData.findEnhanceExpByLevel(m_pModel->uSubType,m_pModel->uMaxEnhanceLevel);

	//获取最大经验
	uint32 uMaxTmpExp = g_clEquipData.getMaxEnhanceExp(m_pModel->uSubType);
	uint32 uMaxExp = MIN(uMaxTmpExp,uMaxItemEnhanceExp);
	m_stItem.uEnhanceExp = MIN(uMaxExp,m_stItem.uEnhanceExp + uAddExp);


	//日志流向
	g_game_log->item_log(m_pPlayer->getPlayerID(),emDoing,m_stItem);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameItem::addForgeExp(uint32 uAddExp,_emDoingType emDoing)
{
	if (!m_pModel)
		return false;

	//等级最大经验
	uint32 uMaxItemForgeExp = g_clEquipData.findForgeExpByLevel(m_pModel->uSubType,m_pModel->uMaxForgeLevel);

	//获取最大经验
	uint32 uMaxExp = MIN(g_clEquipData.getMaxForgeExp(m_pModel->uSubType),uMaxItemForgeExp);
	m_stItem.uForgeExp = MIN(uMaxExp,m_stItem.uForgeExp + uAddExp);
	//日志流向
	g_game_log->item_log(m_pPlayer->getPlayerID(),emDoing,m_stItem);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameItem::updateSpecail(stl_vector<uint32>& vecSpecial,_emDoingType emDoing)
{
	m_stItem.vecSpecialID = vecSpecial;
	//日志流向
	g_game_log->item_log(m_pPlayer->getPlayerID(),emDoing,m_stItem);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameItem::updateBingfuExp(uint32 uExp,_emDoingType emDoing)
{
	if (m_stItem.uBingfuExp == uExp)
		return false;

	m_stItem.uBingfuExp = uExp;

	//日志流向
	g_game_log->item_log(m_pPlayer->getPlayerID(),emDoing,m_stItem);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
uint16	CGameItem::getForgeLevel()
{
	if (!m_pModel)
		return 0;

	const _stEquipForgeModel* pModel = g_clEquipData.findForgeModel(m_pModel->uSubType, m_stItem.uForgeExp);
	return pModel ? pModel->uLevel : 0;
}
//-------------------------------------------------------------
//------------------------------ 
uint16	CGameItem::getBingfuLevel()
{
	if (!m_pModel)
		return 0;

	const _stBingfuModel* pModel = g_clBingfuData.findBingfuByExp(m_stItem.uItemID,m_stItem.uBingfuExp);
	return pModel != NULL ? pModel->uBingfuLevel : 0;
}

//-------------------------------------------------------------
//------------------------------ 
uint32	CGameItem::getSpareNum()
{
	if (!m_pModel)
		return 0;

	return m_stItem.uItemNum <= m_pModel->uMaxCount ? m_pModel->uMaxCount - m_stItem.uItemNum: 0;
}
