/*----------------- playerItem.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����
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
	//--- ÿ������
	virtual void	cleanupDaily		();
	//--- ÿ������
	virtual void	cleanupWeekly		();
	//--- ÿ������
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
	bool	handle_OpenEquipMakeDlg_Ask			(uint8 uEventID, CNetStream& clStream);// ��װ���������,������������Ϣ							PBC2GItemOpenEquipMakeDlgAsk
	bool	handle_OpenEquipMakeOEMDlg_Ask		(uint8 uEventID, CNetStream& clStream);// ���Լ���װ����������									PBC2GItemOpenEquipMakeOEMDlgAsk
	bool	handle_CommitEquipMakeOEM_Ask		(uint8 uEventID, CNetStream& clStream);// �ύ�Լ���װ��������Ϣ,���ȡ�����ύһ���յ��б�		PBC2GItemCommitEquipMakeOEMAsk
	bool	handle_VisitEquipMakeOEM_Ask		(uint8 uEventID, CNetStream& clStream);// �򿪱��˵�װ����������									PBC2GItemVisitEquipMakeOEMAsk
	bool	handle_EquipMakeOEM_Ask				(uint8 uEventID, CNetStream& clStream);// �������װ������											PBC2GItemEquipMakeOEMAsk
	bool	handle_EquipChangeOEMFee_Ask		(uint8 uEventID, CNetStream& clStream);// ���Ĵ�������											PBC2GItemEquipMakeOEMAsk

};
