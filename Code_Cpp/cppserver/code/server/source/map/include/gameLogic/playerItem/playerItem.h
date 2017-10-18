/*----------------- playerItem.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/4/12 17:16:44
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "gameLogic/sceneObject/scenePlayer.h"
#include "gameLogic/gamePlayerFun/gamePlayerFun.h"
/*************************************************************/
class CPlayerItem : public CGamePlayerFun
{
private:
	CGameItemMgr*	m_pGameItemMgr;

public:
	CPlayerItem(CScenePlayer& clPlayer);
	virtual ~CPlayerItem();
	virtual	void	initialize				();
	virtual	bool	loadFromProtobuf		(const PBPlayerData& pbPlayer);
	virtual	bool	saveToProtobuf			(PBPlayerData& pbPlayer);
	virtual	_emGameSystemType	getFunType	()const	{	return GameSystemType_Item;	}
	//--- 每日清理
	virtual void	cleanupDaily		();
	//--- 每周清理
	virtual void	cleanupWeekly		();
	//--- 每月清理
	virtual void	cleanupMonthly		();
private:
	bool	handle_DelItem_Ask			(uint8 uEventID, CNetStream& clStream);
	bool	handle_UseItem_Ask			(uint8 uEventID, CNetStream& clStream);
	bool	handle_EquipItem_Ask		(uint8 uEventID, CNetStream& clStream);
	bool	handle_SellItem_Ask			(uint8 uEventID, CNetStream& clStream);
	bool	handle_BuyBackItem_Ask		(uint8 uEventID, CNetStream& clStream);
	bool	handle_SplitItem_Ask		(uint8 uEventID, CNetStream& clStream);
	bool	handle_SortItem_Ask			(uint8 uEventID, CNetStream& clStream);
	bool	handle_EquipInherit_Ask		(uint8 uEventID, CNetStream& clStream);
	bool	handle_EquipMake_Ask		(uint8 uEventID, CNetStream& clStream);
	bool	handle_Mounting_Ask			(uint8 uEventID, CNetStream& clStream);
	bool	handle_BingfuSplit_Ask		(uint8 uEventID, CNetStream& clStream);
	bool	handle_BingfuUpgrade_Ask	(uint8 uEventID, CNetStream& clStream);
	bool	handle_BingfuUpOneLevel_Ask	(uint8 uEventID, CNetStream& clStream);
	bool	handle_Compound_Ask			(uint8 uEventID, CNetStream& clStream);
	bool	handle_OpenEquipMakeDlg_Ask			(uint8 uEventID, CNetStream& clStream);// 打开装备制造界面,返回熟练度信息							PBC2GItemOpenEquipMakeDlgAsk
	bool	handle_OpenEquipMakeOEMDlg_Ask		(uint8 uEventID, CNetStream& clStream);// 打开自己的装备代工界面									PBC2GItemOpenEquipMakeOEMDlgAsk
	bool	handle_CommitEquipMakeOEM_Ask		(uint8 uEventID, CNetStream& clStream);// 提交自己的装备代工信息,如果取消就提交一个空的列表		PBC2GItemCommitEquipMakeOEMAsk
	bool	handle_VisitEquipMakeOEM_Ask		(uint8 uEventID, CNetStream& clStream);// 打开别人的装备代工界面									PBC2GItemVisitEquipMakeOEMAsk
	bool	handle_EquipMakeOEM_Ask				(uint8 uEventID, CNetStream& clStream);// 请求别人装备代工											PBC2GItemEquipMakeOEMAsk
	bool	handle_EquipChangeOEMFee_Ask		(uint8 uEventID, CNetStream& clStream);// 更改代工费用											PBC2GItemEquipMakeOEMAsk

};
