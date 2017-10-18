/*----------------- identityModel.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����
* Version      : V1.0
* Date         : 2016/9/18 16:48:25
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
#include "basic/basicTypes.h"
#include "def_struct/stlDefine.h"
#include "def_struct/gameStruct.h"
/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 
struct _stIdentityBase
{
	uint16	uID;				//���ID
	uint16	uMaxSkillLevelDiff;	//�������ĵȼ�
	
	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stIdentityOpenBase
{
	uint16	uSetID;				//��λID
	uint16	uSkillExpendCount;	//�������ı���
	bool	bMain;				//�Ƿ��������
	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stIdentityResetBase
{
	uint32	uCount;				//����
	uint32	uCoolTime;			//��ȴʱ����

	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stIdentitySkillBase
{
	uint32	uSkillID;			//����ID
	uint16	uSkillLevel;		//���ܵȼ�
	uint32	uNeedSufferValue;	//��Ҫ����ֵ
	uint32	uTotalSufferValue;	//�����˼����ܹ�����ֵ
	uint32	uDartSelfAddMoney;	//Ѻ�ڸ������ӽ�Ǯ
	uint32	uArrestAddGuard;	//����������������
	uint32	uReardAddDaobao;	//�������ӵ���С������
	uint32	uTreasureAddBox;	//�����������ӱ��伸��

	void initBase()	{   memset(this,0,sizeof(*this));	};
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stIdentityModel : _stIdentityBase
{
	DMAP_UINT8_32_32	dmapPreCondition;		//ǰ������
	stl_set<uint32>		setSkillID;				//����ID��

	void initModel()	
	{ 
		initBase();
		dmapPreCondition.clear();
		setSkillID.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stIdentityBase::read(stream)&&
			stream.read(dmapPreCondition)&&
			stream.read(setSkillID);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stIdentityBase::write(stream)&&
			stream.write(dmapPreCondition)&&
			stream.write(setSkillID);
	}
};
//-------------------------------------------------------------
//------------------------------ 
struct _stIdentityOpenModel : _stIdentityOpenBase
{
	DMAP_UINT8_32_32	dmapPreCondition;		// ǰ������
	MAP_NEED_EXPEND		mapNeedExpend;			// �۳�������
	DMAP_NEED_ITEM		mapNeedItem;			//��Ҫ�������

	void initModel()	
	{ 
		initBase();
		dmapPreCondition.clear();
		mapNeedExpend.clear();
		mapNeedItem.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stIdentityOpenBase::read(stream)&&
			stream.read(dmapPreCondition)&&
			stream.read(mapNeedExpend)&&
			stream.read(mapNeedItem);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stIdentityOpenBase::write(stream)&&
			stream.write(dmapPreCondition)&&
			stream.write(mapNeedExpend)&&
			stream.write(mapNeedItem);
	}
};
//-------------------------------------------------------------
//------------------------------ 
struct _stIdentityResetModel : _stIdentityResetBase
{
	MAP_NEED_EXPEND		mapNeedExpend;			// �۳�������
	DMAP_NEED_ITEM		mapNeedItem;		//��Ҫ�������

	void initModel()	
	{ 
		initBase();
		mapNeedExpend.clear();
		mapNeedItem.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stIdentityResetBase::read(stream)&&
			stream.read(mapNeedExpend)&&
			stream.read(mapNeedItem);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stIdentityResetBase::write(stream)&&
			stream.write(mapNeedExpend)&&
			stream.write(mapNeedItem);
	}
};
//-------------------------------------------------------------
//------------------------------ 
struct _stIdentitySkillModel : _stIdentitySkillBase
{
	typedef	stl_map<uint32,_stItemNum>		MAP_ADDITEMNUM;
	DMAP_UINT8_32_32			dmapPreCondition;		// ǰ������
	_stGameAttribute			stAttribute;			//���ӵ�����
	MAP_U32_U32					mapDartAddHonghuo;		//Ѻ�ں��������߼��� ����_������ֱ�
	MAP_U16_U32					mapCopymapAddRate;		//��������������������	����ID_������ֱ�;
	MAP_U32_U32					mapRefreshUnitAddRate;	//���Ӹ���ˢ�ּ���		����ID_������ֱ�;
	MAP_ADDITEMNUM				mapArrestAddItem;		//�������ӵ��ߵȼ���_����id_���� ע:С�ڵ��ڴ˵ȼ���
	MAP_ADDITEMNUM				mapRewardAddItem;		//�������ӵ��ߵȼ���_����id_���� ע:С�ڵ��ڴ˵ȼ���
	MAP_ADDITEMNUM				mapTreasureAddItem;		//�������ӵ��ߵȼ���_����id_���� ע:С�ڵ��ڴ˵ȼ���
	MAP_U32_U32					mapKillAddBuff;			//����ɱ�˻��buff	����ID_buffid
	MAP_U32_U32					mapBeKillAddBuff;		//������ɱ�����ӶԷ�buff ����ID_buffid;

	void initModel()	
	{ 
		initBase();
		dmapPreCondition.clear();
		stAttribute.initGameAttribute();
		mapDartAddHonghuo.clear();
		mapCopymapAddRate.clear();
		mapRefreshUnitAddRate.clear();
		mapArrestAddItem.clear();
		mapRewardAddItem.clear();
		mapTreasureAddItem.clear();
		mapKillAddBuff.clear();
		mapBeKillAddBuff.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stIdentitySkillBase::read(stream)&&
			stream.read(dmapPreCondition)&&
			stream.read(stAttribute)&&
			stream.read(mapDartAddHonghuo)&&
			stream.read(mapCopymapAddRate)&&
			stream.read(mapRefreshUnitAddRate)&&
			stream.read(mapArrestAddItem)&&
			stream.read(mapRewardAddItem)&&
			stream.read(mapTreasureAddItem)&&
			stream.read(mapKillAddBuff)&&
			stream.read(mapBeKillAddBuff);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stIdentitySkillBase::write(stream)&&
			stream.write(dmapPreCondition)&&
			stream.write(stAttribute)&&
			stream.write(mapDartAddHonghuo)&&
			stream.write(mapCopymapAddRate)&&
			stream.write(mapRefreshUnitAddRate)&&
			stream.write(mapArrestAddItem)&&
			stream.write(mapRewardAddItem)&&
			stream.write(mapTreasureAddItem)&&
			stream.write(mapKillAddBuff)&&
			stream.write(mapBeKillAddBuff);
	}
};
