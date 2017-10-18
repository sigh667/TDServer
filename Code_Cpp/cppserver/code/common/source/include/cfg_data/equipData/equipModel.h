/*----------------- equipModel.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����
* Version      : V1.0
* Date         : 2016/8/15 19:55:48
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
#include "basic/basicTypes.h"
#include "def_struct/stlDefine.h"
#include "def_struct/gameStruct.h"
#include "basic/systemFunction.h"
/*************************************************************/
/*************************************************************/
#pragma pack(push,1)
//-------------------------------------------------------------
//------------------------------ װ��ǿ��
struct _stEquipEnhanceBase
{
	uint16		uLevel;				//ǿ���ĵȼ�
	uint8		uEquipType;			//��������  _emEquipType
	uint32		uTotalExp;			//�����˼��������

	void initBase()	{   memset(this,0,sizeof(*this));	};

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ װ����ĥ
struct _stEquipForgeBase
{
	uint16		uLevel;				//ǿ���ĵȼ�
	uint32		uTotalExp;			//�����˼��������
	uint8		uEquipType;			//��������  _emEquipType
	uint32		uUpgradeRate;		//�����ӵ���ֱ�
	uint32		uEnhanceLevel;
	void initBase()	{   memset(this,0,sizeof(*this));	};

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ װ����Ʒ����
struct _stEquipSpecailAttrBase
{
	uint32		uID;				//Ψһ
	uint16		uType;				//��������
	uint32		uRate;				//Ȩ��
	uint16		uLevel;				//�ȼ�
	uint16		uNeeEquipLevel;		//װ���ȼ�
	uint16		uNeedForgeLevel;	//��Ҫ��ĥ�ȼ�

	void initBase()	{   memset(this,0,sizeof(*this));	};

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ װ��������
struct _stEquipMakeGroupBase
{
	uint32		uEquipID;			//װ��ID

	void initBase()	{   memset(this,0,sizeof(*this));	};

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ װ������
struct _stEquipMakeBase
{
	uint16		uFormulaID;					//����װ����ʽID
	uint16		uEquipLuckyType;			//װ������ֵ����
	uint32		uEquipNeedLucky;			//װ������ֵ
	uint32		uEquipAddLucky;				//װ����������ļ���ֵ
	uint32		uNeedMoney;					//��Ҫ���
	uint32		uProductType;				//��λ���� _emEquipType
	uint32		uJobType;					//ְҵ����
	uint32      uSkilledLevel;				//�����ȵȼ�Ҫ��
	uint32		uSkilledNum;				//��Ҫ����������ֵ
	uint32		uGetSkilled;				//����ɹ���õ�������
	uint32		uVigourCost;				//����ֵ����


	void initBase()	{   memset(this,0,sizeof(*this));	}

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ 
struct _stIDAndRate
{
	uint16	uID;
	uint32	uRate;
	_stIDAndRate()	{   memset(this,0,sizeof(*this));	}

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ װ������
struct _stEquipSuitBase
{
	uint16		uSuitID;			//��װID
	uint16		uSuitLevel;			//װ���ȼ�

	void initBase()	{   memset(this,0,sizeof(*this));	}

	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ װ����ǿ��
struct _stEquipEnhanceModel : _stEquipEnhanceBase
{
	DMAP_UINT8_32_32		dmapPreCondition;			// ǰ������
	MAP_NEED_EXPEND			mapNeedExpend;				// �۳�������
	_stGameAttribute		stAttribute;				//���ӵ�����
	stl_set<uint8>			setMaterialType;			//��Ҫ��������
	MAP_U64_U16				mapNeedMaterial;			// ��Ҫ����
	void initModel()	
	{ 
		initBase();
		dmapPreCondition.clear();
		mapNeedExpend.clear();
		stAttribute.initGameAttribute();
		setMaterialType.clear();
		mapNeedMaterial.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stEquipEnhanceBase::read(stream)&&
			stream.read(dmapPreCondition)&&
			stream.read(mapNeedExpend)&&
			stream.read(stAttribute)&&
			stream.read(setMaterialType)&&
			stream.read(mapNeedMaterial);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stEquipEnhanceBase::write(stream)&&
			stream.write(dmapPreCondition)&&
			stream.write(mapNeedExpend)&&
			stream.write(stAttribute)&&
			stream.write(setMaterialType)&&
			stream.write(mapNeedMaterial);
	}

	inline bool		isMaterialType(uint8 uType)	const
	{
		return setMaterialType.find(uType) != setMaterialType.end();
	}
};
//-------------------------------------------------------------
//------------------------------װ������ĥ
struct _stEquipForgeModel : _stEquipForgeBase
{
	DMAP_UINT8_32_32	dmapPreCondition;			// ǰ������
	MAP_NEED_EXPEND		mapNeedExpend;				// �۳�������
	stl_set<uint8>		setMaterialType;			// ��Ҫ��������
	MAP_U64_U16			mapNeedMaterial;			// ��Ҫ����

	void initModel()	
	{ 
		initBase();
		dmapPreCondition.clear();
		mapNeedExpend.clear();
		setMaterialType.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stEquipForgeBase::read(stream)&&
			stream.read(dmapPreCondition)&&
			stream.read(mapNeedExpend)&&
			stream.read(setMaterialType)&&
			stream.read(mapNeedMaterial);


	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stEquipForgeBase::write(stream)&&
			stream.write(dmapPreCondition)&&
			stream.write(mapNeedExpend)&&
			stream.write(setMaterialType)&&
				stream.write(mapNeedMaterial);
	}

	inline bool		isMaterialType(uint8 uType) const
	{
		return setMaterialType.find(uType) != setMaterialType.end();
	}
};
//-------------------------------------------------------------
//------------------------------ װ������
struct _stEquipMakeModel : _stEquipMakeBase
{
	MAP_U32_U32			mapAddEquipRate;		// ���ɵ��߸���
	DMAP_NEED_ITEM		mapNeedItem;			//��Ҫ����
	DMAP_NEED_ITEM		mapNeedSpecialItem;		//��Ҫ�������
	DMAP_UINT8_32_32	dmapPreCondition;		//ǰ������
	MAP_U32_U32			mapGetSkilledDecay;		//��������ȵݼ�����,key�ǵȼ���,value�ǰٷֱ�

	void initModel()	
	{ 
		initBase();
		mapAddEquipRate.clear();	
		mapNeedItem.clear();		
		mapNeedSpecialItem.clear();	
		dmapPreCondition.clear();	
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stEquipMakeBase::read(stream)&&
			stream.read(mapAddEquipRate)&&
			stream.read(mapNeedItem)&&
			stream.read(mapNeedSpecialItem)&&
			stream.read(dmapPreCondition);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stEquipMakeBase::write(stream)&&
			stream.write(mapAddEquipRate)&&
			stream.write(mapNeedItem)&&
			stream.write(mapNeedSpecialItem)&&
			stream.write(dmapPreCondition);
	}

	uint32		getRandEquipID			(bool bMaxQuality)	const
	{
		stl_vector<uint32> vecRand;
		uint32 uMin = 0;
		int32 iRand = 0;
		CONST_ITER_MAP_U32_U32 _pos = mapAddEquipRate.begin();
		for (; _pos != mapAddEquipRate.end(); ++_pos)
		{
			vecRand.push_back(_pos->first);
			if (!uMin || _pos->first < uMin)
			{
				uMin = _pos->first;
				iRand = vecRand.size()-1;
			}
		}

		if (!bMaxQuality)
			iRand = getRandIndex(vecRand);
		_pos = mapAddEquipRate.begin();
		if ((uint32)iRand > mapAddEquipRate.size())
			return 0;

		advance(_pos,iRand);
		return _pos != mapAddEquipRate.end() ? _pos->second : 0;
	}
};
//-------------------------------------------------------------
//------------------------------ װ��������
struct _stEquipMakeGroupModel : _stEquipMakeGroupBase
{
	stl_vector<_stIDAndRate>			vecNormalGroup;		// ��ͨ���
	stl_vector<_stIDAndRate>			vecAdvancedGroup;	// �߼����

	void initModel()	
	{ 
		initBase();
		vecNormalGroup.clear();
		vecAdvancedGroup.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stEquipMakeGroupBase::read(stream)&&
			stream.read(vecNormalGroup)&&
			stream.read(vecAdvancedGroup);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stEquipMakeGroupBase::write(stream)&&
			stream.write(vecNormalGroup)&&
			stream.write(vecAdvancedGroup);
	}
};
//-------------------------------------------------------------
//------------------------------ װ����Ʒ����
struct _stEquipSpecailAttrModel : _stEquipSpecailAttrBase
{
	_stGameAttribute stAttri;

	void initModel()	
	{ 
		initBase();
		stAttri.initGameAttribute();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stEquipSpecailAttrBase::read(stream)&&
			stream.read(stAttri);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stEquipSpecailAttrBase::write(stream)&&
			stream.write(stAttri);
	}
};
//-------------------------------------------------------------
//------------------------------ ��װ
struct _stEquipSuitModel : _stEquipSuitBase
{
	stl_set<uint16>						setEquipID;			//��װID
	stl_map<uint8,_stGameAttribute>		mapGameAttr;		//��װ���� key:����
	void initModel()	
	{ 
		initBase();
		setEquipID.clear();
		mapGameAttr.clear();
	}
	inline bool		read			(CStreamBase& stream)
	{
		return _stEquipSuitBase::read(stream)&&
			stream.read(setEquipID)&&
			stream.read(mapGameAttr);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stEquipSuitBase::write(stream)&&
			stream.write(setEquipID)&&
			stream.write(mapGameAttr);
	}
};
