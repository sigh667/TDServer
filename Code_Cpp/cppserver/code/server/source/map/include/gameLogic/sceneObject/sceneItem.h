/*------------- sceneItem.h
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����
* Version      : V1.01
* Date         : 2016/03/09
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "sceneUnit.h"
#include "cfg_data/dropData/dropData.h"
#include "def_struct/teamStruct.h"
#include "protocol/CASMsg.pb.h"
#include "stl/std_map.h"
#include "stl/stl_vector.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
// ����ϵͳ ������Ϣ
struct _stDropItemDropCell
{
	uint16	uIndex;					;	// ����
	uint32	uItemID					;	// ����ID
	uint16	uItemNum				;	// ��������
	uint32	uOwnerID				;	// �������Ϣ
	bool	bBind					;	// �Ƿ��
	stl_map<uint32,uint32>	mapDice	;	// ��ԱID ==> ��������

	_stDropItemDropCell()
	{
		init();
	}
	void	init()
	{
		uIndex		= 0;
		uItemID		= 0;
		uItemNum	= 0;
		uOwnerID	= 0;
		bBind		= false;
		mapDice.clear();
	}
	void		loadFromProtobuf(const PBDropItem_DropCell&pbInfo);
	void		saveToProtobuf	 (PBDropItem_DropCell&pbInfo);
};
//-------------------------------------------------------------
//------------------------------
class PBC2DropItem_Info;
class CSceneItem	: public CSceneUnit
{
private:
	typedef CSceneUnit Parent;
	friend	class CPlayerDropItem;
private:
	bool							m_bOpen;
	uint16							m_uDropID;			//����ID
	uint32							m_uDeadID;			//��������ID
	uint32							m_uOwnerPlayerID;	//������
	uint64							m_uBirthTime;		//����ʱ��
	uint32							m_uCurAllotType;	//��ǰ����ģʽ m_uCurAllotType
	uint32							m_uAllotTypeEndTime;//��ǰ����ģʽ����ʱ���
	uint32							m_uExistTime;		//��ʧʱ��
	_stTeamInfo						m_stTeamInfo;		//������Ϣ
	stl_vector<_stDropItemDropCell>	m_vecItem;			//������Ϣ
	bool							m_bCashPlayer;		//�Ƿ����ͽ���					
public:
	CSceneItem();
public:
	virtual bool	initialize		();
	virtual uint32	getID			();
	virtual CSceneItem*	asSceneItem	()	{	return this;		}

public:
	virtual	bool	packetGhost		(uint64 uTime,bool bSelf,uint32& updateMark,PBGhostInfo&pbInfo);
	/*--->[ ���� ]*/
	virtual void	processLogic	(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay);
public:
	//--- ����
	bool	build					(CGameSceneCell* pCell,uint32 uDeadID, uint16 uDropID,const VEC_DROP_CELL&vecDropGroup, CScenePlayer* pOwner);

	bool	autoDiceAward			(_stDropItemDropCell& cell);

	bool	diceItem				(_stDropItemDropCell& cell,_stTeamMember* pMember,bool bAbandon,bool bSynDice = true);
	bool	awardItem				(_stDropItemDropCell& cell,_stPlayerDisplay* pDisplay);
	bool	awardItemBySendMail		(CScenePlayer * pPlayer, uint32 uLogicWorldID);
	bool	isOpen					();
	bool	open					();

	inline  bool  isCashPlayer		()	{	return m_bCashPlayer;	}

	bool	isItemOwner				(uint32 uPlayerID);
	bool	haveItem				();

	void	packPB					(PBC2DropItem_Info& ack);
public:
	void			broadCastMsg	(uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent = 0);
};
