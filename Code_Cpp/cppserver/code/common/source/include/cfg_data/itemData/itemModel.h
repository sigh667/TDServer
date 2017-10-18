/*----------------- itemModel.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����
* Version      : V1.0
* Date         : 2016/4/11 11:32:05
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/memoryFunctions.h"
#include "basic/basicTypes.h"
#include "stream/netStream.h"
#include "def_struct/stlDefine.h"
#include "timer/dateTime.h"
#include "def_struct/gameStruct.h"
#include "basic/systemFunction.h"
/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ 
struct _stSplitProb
{
	uint16		uMaterialId;		// �ֽ������ƷID
	uint16		uMaterialNum;		// �ֽ������Ʒ����
	uint16		uProb;				// �ֽ���ĸ���

	_stSplitProb()			{	dMemset(this,0,sizeof(*this));			}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stItemModelBase
{
	uint32			uItemID;
	uint8			uType;				// ��Ʒ������ _emItemType
	uint8			uSubType;			// ��ƷС���� װ������:_emEquipType ��������:_emBingfuType ����:_emMaterialType
	uint8			uUseType;			// ʹ������ _emItemUseType
	uint16			uLevel;				// ��Ʒ�ȼ�
	uint16			uMaxEnhanceLevel;	// ���ǿ���ȼ�
	uint16			uMaxForgeLevel;		// �����ĥ�ȼ�
	uint32			uMark;				// ���� _emItemMark
	uint16			uMaxCount;			// ���ѵ���
	uint32			uItemMark;			// ���߱�ʾ(λ_em_ItemMark)
	uint64			uBuyPrice;			// �̵깺��۸�
	uint64			uSellPrice;			// �����̵�۸�
	uint32			uCD;				// CDʱ�䣨���룩
	uint16			uCDGroupID;			// CD��ID
	uint32			uGroupCD;			// CD��ʱ�䣨���룩
	uint8			uQuality;			// Ʒ��		_emItemQualityType
	uint32			uDurability;		// �;�ֵ
	uint16			uSplitItemID;		// �ֽ����õ���ID
	uint16			uSplitItemNum;		// �ֽ����õ��߸���
	uint32			uExpireTime;		// ��Чʱ����
	uint32			uMaxDayUseCount;	// ÿ��ʹ�ô���
	uint32			uMaxWeekUseCount;	// ÿ��ʹ�ô���
	uint32			uMaxMonthUseCount;	// ÿ��ʹ�ô���
	uint32			uBingfuExp;			// ��������

	_stItemModelBase&	getBase		()			{	return *this;							}
	inline void			initBase	()			{	dMemset(this,0,sizeof(*this));			}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stItemBuild
{
	uint32	uItemID;			//��ƷID 
	uint32	uItemNum;			//��Ʒ���� 	
	uint16	uRate;				//����(�����ֱ�)

	_stItemBuild	()				{	initItemBuild();				}
	_stItemBuild&getItemBuild	()	{	return *this;					}
	inline void	initItemBuild	()	{	memset(this,0,sizeof(*this));	}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ ���
struct _stItemPackModelBase
{
	uint32			uPacketID;		//���ID
	uint8			uType;			//�������(_emItemPackType)

	_stItemPackModelBase&	getBase		()			{	return *this;							}
	inline void				initBase	()			{	dMemset(this,0,sizeof(*this));			}
	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ ���ߺϳ�
struct _stItemCompoundBase
{
	uint32			uRecipeID;		//�䷽

	_stItemCompoundBase&	getBase		()			{	return *this;							}
	inline void				initBase	()			{	dMemset(this,0,sizeof(*this));			}
	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
struct _stItemModel	: _stItemModelBase
{
	std::string					strScript;			// ʹ�ú󴥷��Ľű�
	stl_vector<_stSplitProb>	splitVec;			// �ֽ��б�
	stl_vector<uint32>			vecFunValue;		// ���ܲ���
	DMAP_UINT8_32_32			dmapUseCondition;	// ǰ������
	stl_vector<_stDateTime>		vecUseTimeLimit;	// ʹ��ʱ������
	stl_vector<_stGameAttribute> vecAttribute;		// �������� 
	MAP_U16_U32					mapAddEnhanceExp;	// ����ǿ������ key:�ȼ� value:����
	MAP_U16_U32					mapAddForgeExp;		// ������ĥ���� key:�ȼ� value:����
	std::string					strName;			// ��Ʒ����

	inline bool		check_mark	(uint8 _mark) const	{	return _CHECK_BIT(uMark,_BIT32(_mark));	}
	_stItemModel&	getModel	()					{	return *this;							}
	inline void		initModel	()				
	{
		initBase();
		strScript.clear();
		splitVec.clear();
		vecFunValue.clear();
		dmapUseCondition.clear();
		vecUseTimeLimit.clear();
		mapAddEnhanceExp.clear();
		mapAddForgeExp.clear();
		strName.clear();
	}
	inline bool		read			(CStreamBase& stream)
	{
		return _stItemModelBase::read(stream)&&
			stream.read(strScript)&&
			stream.read(splitVec)&&
			stream.read(vecFunValue)&&
			stream.read(dmapUseCondition)&&
			stream.read(vecAttribute)&&
			stream.read(vecUseTimeLimit)&&
			stream.read(mapAddEnhanceExp)&&
			stream.read(mapAddForgeExp)&&
			stream.read(strName);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stItemModelBase::write(stream)&&
			stream.write(strScript)&&
			stream.write(splitVec)&&
			stream.write(vecFunValue)&&
			stream.write(dmapUseCondition)&&
			stream.write(vecAttribute)&&
			stream.write(vecUseTimeLimit)&&
			stream.write(mapAddEnhanceExp)&&
			stream.write(mapAddForgeExp)&&
			stream.write(strName);
	}
	inline bool		isUseTimeLimit	(uint64 uTime)	const
	{
		if ( vecUseTimeLimit.empty() )
			return true;

		uint32	uSize 	= vecUseTimeLimit.size();
		for ( uint32 i = 0; i < uSize; ++i )
		{
			const _stDateTime& stDataTime = vecUseTimeLimit[ i ];
			if (stDataTime.toDataTime(uTime))
				return false;
		}
		return true;
	}
	inline uint32 getEnhanceExp(uint16 uLevel)	const
	{
		CONST_ITER_MAP_U16_U32 _pos = mapAddEnhanceExp.lower_bound(uLevel); //��һ�����ڵ���uLevel
		return _pos != mapAddEnhanceExp.end() ? _pos->second : 0;
	}
	inline uint32 getForgeExp(uint16 uLevel)	const
	{
		CONST_ITER_MAP_U16_U32 _pos = mapAddForgeExp.lower_bound(uLevel); //��һ�����ڵ���uLevel
		return _pos != mapAddForgeExp.end() ? _pos->second : 0;
	}
};
//-------------------------------------------------------------
//------------------------------ ���
struct _stItemPackModel : _stItemPackModelBase
{
	typedef stl_vector<_stItemBuild>	VECTOR_ITEM_BUILD;
	VECTOR_ITEM_BUILD	vecItemBuild;	//�������

	_stItemPackModel&getItemPackModel()	{	return *this;	}
	inline void	initModel()
	{
		initBase();
		vecItemBuild.clear();
	}
	inline bool		read			(CStreamBase& stream)
	{
		return _stItemPackModelBase::read(stream)&&
			stream.read(vecItemBuild);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stItemPackModelBase::write(stream)&&
			stream.write(vecItemBuild);
	}
};
//-------------------------------------------------------------
//------------------------------ ���ߺϳ�
struct _stItemCompoundModel : _stItemCompoundBase
{
	DMAP_NEED_ITEM		mapNeedItem;
	MAP_U32_U32			mapItemRate;			//���ӵ��� key:Ȩ�� value:����id

	_stItemCompoundModel&	getModel()	{	return *this;	}
	inline void	initModel()
	{
		initBase();
		mapNeedItem.clear();
		mapItemRate.clear();

	}
	inline bool		read			(CStreamBase& stream)
	{
		return _stItemCompoundBase::read(stream)&&
			stream.read(mapNeedItem)&
			stream.read(mapItemRate);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stItemCompoundBase::write(stream)&&
			stream.write(mapNeedItem)&
			stream.write(mapItemRate);
	}
	inline uint32	findRandItem() const
	{
		MAP_U32_U32::const_reverse_iterator _rpos = mapItemRate.rbegin();
		if (_rpos == mapItemRate.rend())
			return 0;
		uint32 uRandNum = randRange(_rpos->first);
		CONST_ITER_MAP_U32_U32 _spos = mapItemRate.lower_bound(uRandNum); //��һ�����ڵ���uRandNum
		return _spos != mapItemRate.end() ? _spos->second : 0;
	}
};
