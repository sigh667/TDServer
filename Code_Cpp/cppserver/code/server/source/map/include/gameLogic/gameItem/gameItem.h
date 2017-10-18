/*----------------- gameItem.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/4/13 15:44:53
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/timeFunction.h"
#include "def_struct/playerStuct.h"
#include "cfg_data/itemData/itemModel.h"
#include "protocol/PlayerStruct.pb.h"
#include "def_struct/playerDefine.h"
/*************************************************************/
class CScenePlayer;
class CGameItem
{
private:
	_stItem				m_stItem;		//道具数据
	const _stItemModel*	m_pModel;		//道具模板
	CScenePlayer*		m_pPlayer;
public:
	inline	const _stItemModel*getModel	()				{   return m_pModel;										}
	inline	_stItem&	getItem		()					{   return m_stItem;										}
	inline	uint32		getItemID	()const				{   return m_stItem.uItemID;								}
	inline	uint32		getEnhanceExp()const			{   return m_stItem.uEnhanceExp;							}
	inline	uint32		getForgeExp	()const				{   return m_stItem.uForgeExp;								}
	inline	uint64		getItemSN	()const				{   return m_stItem.uItemSN;								}
	inline	void		setItemSN	(uint64 uSN)		{   m_stItem.uItemSN = uSN;									}
	inline	uint16		getItemNum	()const				{   return m_stItem.uItemNum;								}
	inline	void		setItemNum	(uint16 uNum)		{   m_stItem.uItemNum = uNum;								}
	inline	bool		isEquip		()const				{   return m_pModel? (m_pModel->uSubType == ItemType_Equip ) : false;	}
	inline	uint16		getMaxCount	()const				{   return m_pModel? m_pModel->uMaxCount : 0;				}
	inline	bool		isBind		()const				{   return m_stItem.testFlag(ItemFlag_Binded);				}
	inline	void		setFlag		(uint8 flag,bool bSet)	{   m_stItem.setFlag(flag,bSet);						}
	inline	bool		testFlag	(uint8 flag)		{   return	m_stItem.testFlag(flag);						}
	inline	uint8		getItemType	()const				{   return m_pModel? m_pModel->uType : 0;					}
	inline	uint8		getItemSubType	()const			{   return m_pModel? m_pModel->uSubType : 0;				}
	inline	uint8		getItemUseType()const			{   return m_pModel? m_pModel->uUseType : 0;				}
	inline	uint32		getBingfuExp()const				{   return m_stItem.uBingfuExp;								}
	inline  bool		isExpireItem()const				{   return m_pModel? m_pModel->uExpireTime > 0 : false;		}
	inline	bool		isOutExpire	(uint64 uTime)		{   return uTime <= m_stItem.uEndtime;						}
	bool				addEnhanceExp(uint32 uAddExp,_emDoingType emDoing );
	bool				addForgeExp	(uint32 uAddExp,_emDoingType emDoing );
	bool				updateSpecail(stl_vector<uint32>& vecSpecial,_emDoingType emDoing);
	bool				updateBingfuExp(uint32 uExp,_emDoingType emDoing );
	uint16				getForgeLevel();
	uint32				getSpareNum();
	uint16				getBingfuLevel();

public:
	CGameItem();

public:
	void			initialize		();
	void			build			(uint64 uSN, const _stItemModel*pModel,CScenePlayer* pPlayer);
	bool			setBuild		(_stItem&stItem);
	bool			updateItem		(_stItem&stItem);
	bool			addGameAttri	(_stGameAttribute& stGameAttr);
};
