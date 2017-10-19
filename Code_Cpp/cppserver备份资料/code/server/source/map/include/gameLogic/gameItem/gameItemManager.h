/*----------------- gameItemManager.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����
* Version      : V1.0
* Date         : 2016/4/12 11:34:43
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "gameItem.h"
#include "memoryPool/mapPool.h"
#include "singleton/singleton.h"
#include "stream/numberStream.h"
#include "cfg_data/itemData/itemModel.h"
#include "def_struct/stlDefine.h"
#include "protocol/GameDefine.pb.h"
#include "gameLogic/gameItem/itemFactory.h"
/*************************************************************/
class CScenePlayer;
class PBPlayerEquipData;
class PBEquipMakeSkilled;
class PBEquipMakeVigour;
class PBEquipMakeOEM;
class PBC2GItemCommitEquipMakeOEMAsk;
class PBG2CItemOpenEquipMakeDlgAck;
class PBG2CItemOpenEquipMakeOEMDlgAck;
class PBG2CItemCommitEquipMakeOEMAck;
class PBG2CItemVisitEquipMakeOEMAck;
class PBG2CItemEquipMakeOEMAck;
class PBG2CItemEquipChangeOEMFeeAck;
//-------------------------------------------------------------
//------------------------------��ҵ��߹��� �������޸���
class CGameItemMgr : public CItemFactory
{
private:
	typedef CItemFactory Parent;

private:
	typedef stl_map<uint64,CGameItem*>						MAP_ITEM;
	typedef stl_map<uint64,CGameItem*>::iterator			ITER_MAP_ITEM;
	typedef stl_map<uint8,CGameItem*>						MAP_EQUIP;
	typedef stl_map<uint8,CGameItem*>::iterator				ITER_MAP_EQUIP;
	typedef stl_vector<CGameItem*>							VEC_ITEM;
	typedef stl_map<uint32,_stItemUseCount>					MAP_ITEMUSECOUNT;

	typedef double_map<uint32,uint64,CGameItem*>				DMAP_TYPE_BINGFU;//�������� key1:����id key2:����sn

	struct _stExpendGameItem
	{
		CGameItem* pGameItem;
		uint32 uItemNum;
		_stExpendGameItem()
		{
			pGameItem = NULL;
			uItemNum = 0;
		}
	};
	typedef stl_vector<_stExpendGameItem>					VEC_EXPENDGAMEITEM;
	typedef bool (CGameItemMgr::*_use_item)(CGameItem* pItem,uint32 uItemNum);

	//��������Ϣ
	struct _stEquipMakeSkilled
	{
		uint32 uEquipType;
		uint32 uSkilledLevel;
		uint32 uSkilledPoint;
		void init()
		{
			uEquipType    = 0;
			uSkilledLevel = 1;
			uSkilledPoint = 0;
		}
		void load(const PBEquipMakeSkilled& pb);
		void save(PBEquipMakeSkilled& pb);
		void checkUpgrade();
	};
	typedef stl_map<uint32, _stEquipMakeSkilled> MAP_SKILLED;

	//������ֵ
	struct _stEquipMakeVigour
	{
		uint32 uVigour;
		uint64 uLastAddTime;
		uint32 uVigourFrozen;
		void init()
		{
			uVigour = 0;
			uLastAddTime = 0;
			uVigourFrozen = 0;
		}
		void load(const PBEquipMakeVigour& pb);
		void save(PBEquipMakeVigour & pb);
	};
	//������Ϣ
	struct _stEquipMakeOEM
	{
		uint32 uMakeID;
		uint32 uFee;
		uint32 uCount;
		uint32 uCountUsed;
		stl_vector<uint32> vcAllMakeIDs;//������ͬ�����ȵȼ��Ͳ�λ�Ĵ���,��������ְҵ�Ĳ�λ
		void init()
		{
			uMakeID = 0;
			uFee = 0;
			uCount = 0;
			uCountUsed = 0;
			vcAllMakeIDs.clear();
		}
		void load(const  PBEquipMakeOEM& pb);
		void save(PBEquipMakeOEM& pb);
	};
	typedef stl_map<uint32, _stEquipMakeOEM> MAP_OEM;
	
private:
	_use_item			m_fnUseItem[_emItemUseType_ARRAYSIZE];

private:
	MAP_ITEM			m_mapBagItem;		//��������
	MAP_ITEM			m_mapSellItem;		//���۵��� ���ڻع�
	MAP_EQUIP			m_mapEquip;			//װ��
	CScenePlayer*		m_pPlayer;			//������
	_stGameAttribute*	m_pItemGameAttr;	//����
	uint32				m_uNextCheckExpire;
	MAP_ITEMUSECOUNT	m_mapUseCount;		//ʹ�ô���
	DMAP_TYPE_BINGFU	m_dmapBingfu;		//�Ǿ����������
	stl_set<CGameItem*>	m_setExpBingfu;		//�����������
	MAP_SKILLED			m_mpSkilled;		//ÿ����λ��Ӧ����һ��װ���������������Ϣ.
	_stEquipMakeVigour	m_stMakeVigour;		//װ������Ļ�����Ϣ
	MAP_OEM				m_mpOEMs;			//�����Ĵ��������Ϣ

public:
	CGameItemMgr();
	~CGameItemMgr();

public:
	inline	bool			canOperate		()									{   return m_pPlayer != NULL && m_pItemGameAttr != NULL;		}
	inline CGameItem*		findItem	(uint64 uItemSN)
	{
		 CGameItem** pGameItem = m_mapBagItem.find_(uItemSN);
		return pGameItem? *pGameItem : NULL;
	}
	inline CGameItem*		findSellItem(uint64 uItemSN)
	{
		CGameItem** pGameItem = m_mapSellItem.find_(uItemSN);
		return pGameItem? *pGameItem : NULL;
	}
	inline CGameItem*		findEquip	(uint8 uEquipType)
	{
		CGameItem** pGameItem = m_mapEquip.find_(uEquipType);
		return pGameItem? *pGameItem : NULL;
	}
	inline uint32		findEquipID	(uint8 uEquipType)
	{
		CGameItem** pGameItem = m_mapEquip.find_(uEquipType);
		return pGameItem? (*pGameItem)->getItemID() : 0;
	}
	inline uint16		findEquipForgeLevel	(uint8 uEquipType)
	{
		CGameItem* pGameItem = findEquip(uEquipType);
		return pGameItem? pGameItem->getForgeLevel() : 0;
	}
	inline _stItemUseCount*		findUseCount	(uint32 uItemID)
	{
		return m_mapUseCount.find_(uItemID);
	}
	CGameItem*			findAllItem (uint64 uItemSN);
	uint16				getAllForgeMinLevel();
	bool				addUseCount(const _stItemModel* pModel,uint16 uCount);
	CGameItem*			findNoFullBingfu (const _stItem& stItem);

public:
	void	clear			();
	void	initialize		(CScenePlayer* pPlayer);
	bool	load			(const PBPlayerBag& pbBag);
	bool	save			(PBPlayerBag* pbBag);
	bool	saveToProbuff	(PBPlayerEquipData* pbEquip);
	void	cleanupDaily	();
	void	cleanupWeekly	();
	void	cleanupMonthly	();
private:
	bool		insertGameItem	(CGameItem* pGameItem);
	bool		delGameItem		(CGameItem* pGameItem,bool bRelease = true);
	bool		addPBItem		(const PBItem& pbItem,_em_ItemListType emType);
	bool		updateItemNum	(CGameItem* pGameItem,int32 nNum,bool bSend,_emDoingType emDoing );
	bool		canAddItem		(const _stItemModel* pItemModel,uint16 uItemNum);
	CGameItem* 	addNewItem		(const _stItemModel* pItemModel,uint16 uItemNum,bool bBind,bool bSend,_emDoingType emDoing);
	CGameItem* 	addNewItem		(_stItem& stItem,bool bSend,_emDoingType emDoing);
	bool		canRemoveItem	(const _stItemModel* pItemModel,uint16 uItemNum,_em_ItemBindType emType,uint8& uCode);
	bool		canUseItem		(CGameItem* pGameItem,uint16 uItemNum,uint8& uCode);
	uint32		getItemCount	(const _stItemModel* pItemModel,_em_ItemBindType emType = ItemBindType_All);
	bool		calculateAttri	();
	static int	sort_expenditem	(const void *p1, const void *p2);
	static bool	sort_expbingfu	(const CGameItem* pLeft, const CGameItem* pRight);
public: 
	bool	addItem			(const PBItem& pbItem,bool bSend,_emDoingType emDoing );
	bool	addItem			(uint32 uItemID,uint16 uItemNum,bool bBind,bool bSend, bool bMail ,_emDoingType emDoing , bool bForceMail = false, _emMailType emMailType = MailType_FullBag);
	bool	addItem			(const DMAP_NEED_ITEM * pmapItem,bool bSend,_emDoingType emDoing );
	bool	addItem			(const MAP_NEED_ITEM * pmapItem,bool bBind,bool bSend,_emDoingType emDoing );
	bool	addItem			(const _stItemBuild&stItemBuild,_emDoingType emDoing );
	bool	addEquipItem	(uint32 uItemID,_emDoingType emDoing );
	bool	removeItem		(uint32 uItemID,uint16 uItemNum,bool bSend,uint8& uCode,_em_ItemBindType emType,_emDoingType emDoing );
	bool	removeItem		(uint64 uItemSN,uint16 uItemNum,bool bSend,uint8& uCode,_em_ItemBindType emType,_emDoingType emDoing );
	bool	removeItem		(uint64 uItemSN,bool bSend,uint8& uCode,_emDoingType emDoing );
	bool	canRemoveBingfu	(const DMAP_NEED_BINGFU& dmapNeedBingfu,_em_ItemBindType emType);
	bool	canRemoveBingfu	(uint32 uBingfuID,uint16 uBingfuLevel,uint16 uBingfuNum,_em_ItemBindType emType);
	bool	removeBingfu	(uint32 uBingfuID,uint16 uBingfuLevel,uint16 uBingfuNum,_em_ItemBindType emType,_emDoingType emDoing);
	bool	removeBingfu	(const DMAP_NEED_BINGFU& dmapNeedBingfu,_em_ItemBindType emType,_emDoingType emDoing);
	bool	useItem			(uint64 uItemSN,int16 uItemNum,uint8& uCode);
	bool	equipItem		(uint64 uItemSN,uint8 uEquipType,uint8& uCode,bool bSend = true);
	bool	sellItem		(uint64 uItemSN,uint8& uCode);
	bool	buyBackItem		(uint64 uItemSN,uint8& uCode);
	bool	moveItem		(uint64 uFromItemSN,uint64 uToItemSN,uint16 uItemNum,uint8& uCode);
	bool	addItemPack		(uint16 uPackID,uint16 uPackNum,_emDoingType emDoing );
	bool	splitItem		(SET_U64& setItemSN,uint8& uCode);
	uint32	getBagSpare		();
	uint32	getBingfuBagNum	();
	uint32	getItemBagNum	();
	bool	canRemoveItem	(uint32 uItemID,uint16 uItemNum,_em_ItemBindType emType);
	bool	removeItem		(const DMAP_NEED_ITEM & dmapNeedItem,_em_ItemBindType emType,_emDoingType emDoing );
	bool	removeItem		(const MAP_NEED_ITEM & dmapNeedItem,_em_ItemBindType emType,_emDoingType emDoing );
	bool	canRemoveItem	(const DMAP_NEED_ITEM & dmapNeedItem,_em_ItemBindType emType);
	bool	canRemoveItem	(const DMAP_NEED_ITEM & dmapNeedItem);
	bool	canRemoveItem	(const MAP_NEED_ITEM & mapNeedItem,_em_ItemBindType emType = ItemBindType_All);
	uint32	getItemCount	(uint32 uItemID,_em_ItemBindType emType = ItemBindType_All);
	bool	equipInherit	(uint64 uItemSN,uint64 uDelItemSN,uint8& uCode);
	bool	equipMake		(uint16 uMakeID,uint8 uMark,uint8& uCode);
	bool	mounting		(bool bMount,uint8& uCode);
	void	clearBag		();
	void	clearEquip		();
	bool	canAddItem		(uint32 uItemID, uint16 uItemNum);
	bool	findByItemID	(uint32 uItemID, stl_vector<uint64> &vecItemSN);

public:///�������
	bool	bingfuSplitAsk			(MAP_U64_U16& mapExpend,uint8& uCode);
	bool	bingfuUpgradeAsk		(uint64 uSn,MAP_U64_U16& mapExpend,uint8& uCode);
	bool	bingfuUpOneLevelAsk		(uint64 uSn,uint8& uCode);
	bool	addBingfu				(_stItem& stItem,_emDoingType emDoing );
	bool	equipBingfu				(CGameItem* pGameItem,bool bEquip);
	bool	compoundAsk				(uint32 uRecipeID,MAP_U64_U16& mapItem,uint8& uCode);

private:
	bool	itemUse_AddExpend	(CGameItem* pGameItem,uint16 uItemNum);
	bool	itemUse_AddBuff		(CGameItem* pGameItem,uint16 uItemNum);
	bool	itemUse_Tranfer		(CGameItem* pGameItem,uint16 uItemNum);
	bool	itemUse_Packet		(CGameItem* pGameItem,uint16 uItemNum);
	bool	itemUse_Treasure	(CGameItem* pGameItem,uint16 uItemNum);
	bool	itemUse_Fashion		(CGameItem* pGameItem,uint16 uItemNum);

public:
	bool	send_UpdateItemNumAck(uint64 uItemSN,uint16 uItemNum,_emDoingType emDoing,uint8 uCode = 0);
	bool	send_AddItemByMailAck	(_stItem& stItem,_emDoingType emDoing ,uint8 uCode = 0);
	bool	send_AddItemAck		(_stItem& stItem,_emDoingType emDoing ,uint8 uCode = 0);
	bool	send_DelItemAck		(uint64 uItemSN,uint8 uCode = 0);
	bool	send_UseItemAck		(uint64 uItemSN,uint16 uItemNum,uint8 uCode = 0);
	bool	send_EquipItemAck	(uint64 uItemSN,uint8 uEquipType,uint8 uCode = 0);
	bool	send_SellItemAck	(uint64 uItemSN,uint8 uCode = 0);
	bool	send_BuyBackItemAck	(uint64 uItemSN,uint8 uCode = 0);
	bool	send_SortItemAck	(uint8 uCode = 0);
	bool	send_SplitItemAck	(SET_U64& setItemSN,uint8 uCode = 0);
	bool	send_EquipEnhanceAck(uint8 uEquipType,uint8 uCode = 0);
	bool	send_EquipForgeAck	(uint8 uEquipType,uint8 uCode = 0);
	bool	send_EquipInheritAck(uint64 uItemSN,uint8 uCode = 0);
	bool	send_EquipMakeAck	(uint16 uMakeID,uint8 uMark,uint8 uCode = 0);
	bool	send_MountingAck	(uint8 uCode = 0);
	bool	send_BingfuUpgrade_Ack(uint64 uSn,uint8 uCode = 0);
	bool	send_Compound_Ack	(uint32 uAddItemID,uint8 uCode = 0);
	bool	send_BingfuSplit_Ack(uint8 uCode = 0);
	bool	send_UpdateItemAck	(CGameItem* pGameItem, uint8 uCode = 0);

public://����������,����,�������
	bool	calcVigour						();
	_stEquipMakeSkilled* findSkilledByType  (_emEquipType uType);
	_stEquipMakeOEM*	 findOEMByID	(uint32 uMakeID);
	_stEquipMakeOEM*	 findOEMByRealID(uint32 uRealMakeID);//ͬһ������,���Դ��첻ְͬҵ��ͬһ��װ��,
	bool    preCheckMakeSkilled				(uint32 uMakeID, uint8& uCode);
	bool    equipMakeSuccessAddSkilled		(uint32 uMakeID);
	uint32	getMakeVigour					();	
	bool	addMakeVigour					(int32 nAddValue,_emDoingType emDoing);
	bool	newplayerMakeVigour				();
	bool	onGmsetSkilled					(_emEquipType uType, uint32 uLevel, uint32 uPoint);
	bool	buildOEMAck						(PBG2CItemVisitEquipMakeOEMAck& ack);
	bool    changeOemFeeAsk					(uint32 uMakeID,uint32 uFee, uint8& uCode);
	void	newPlayerSkilled				();


	bool    preCheckMakeOEMSkilled			(uint32 uMakeID, CScenePlayer* pBuyer, uint8& uCode);
	bool    oemMakeSuccessAddSkilled		(uint32 uMakeID, CScenePlayer* pBuyer);
	bool    oemCheckRemove					(uint32 uMakeID);


	bool	openMakeEquipDlgAsk				(uint8& uCode);
	bool	openMakeEquipOemDlgAsk			(uint8& uCode);
	bool	commitMakeEquipOemAsk			(const PBC2GItemCommitEquipMakeOEMAsk & pb, uint8& uCode);
	bool	visitMakeEquipOemAsk			(uint32 uPlayerID, uint8& uCode);
	bool	makeEquipOemAsk					(uint32 uPlayerID, uint32 uMakeIndexID, uint32 uMakeID, uint8& uCode);

	bool	send_OpenMakeDlgAck				(PBG2CItemOpenEquipMakeDlgAck* , uint8 uCode = 0);
	bool	send_OpenMakeOEMDlgAck			(PBG2CItemOpenEquipMakeOEMDlgAck* , uint8 uCode = 0);
	bool	send_CommitMakeOEMAck			(PBG2CItemCommitEquipMakeOEMAck* , uint8 uCode = 0);
	bool	send_VisitMakeOEMAck			(PBG2CItemVisitEquipMakeOEMAck* , uint8 uCode = 0);
	bool	send_MakeEquipOEMAck			(PBG2CItemEquipMakeOEMAck* , uint8 uCode = 0);
	bool	send_ChangeOEMFeeAck			(PBG2CItemEquipChangeOEMFeeAck*, uint8 uCode = 0);


public:
	void	process				(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay);

};

