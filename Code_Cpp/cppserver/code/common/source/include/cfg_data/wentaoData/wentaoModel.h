/*----------------- wentaoModel.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2016/8/3 10:32:49
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
//------------------------------ ��������
struct _stBingFaTypeBase
{
	uint16		uBingFaID;			//����ID

	void initBase()	{   memset(this,0,sizeof(*this));	};

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ ͻ��
struct _stBingFaTuPoBase
{
	uint16		uBingFaID;			//����ID
	uint16		uTuPoLevel;			//ͻ�Ƶȼ�

	void initBase()	{   memset(this,0,sizeof(*this));	};

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------  Ǳ��
struct _stBingFaQianXiuBase
{
	uint16		uBingFaID;			//����ID
	uint16		uQianXiuLevel;		//Ǳ�޵ȼ�
	uint32		uAddAttrRadio;		//����������ֱ�

	void initBase()	{   memset(this,0,sizeof(*this));	};

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ Ҫ��
struct _stBingFaYaoDianBase
{
	uint32		uYaoDianID;			//Ҫ��ID
	uint32		uYaoDianLevelMax;	//Ҫ����ߵȼ�

	void initBase()	{   memset(this,0,sizeof(*this));	};

	READ_WRITE_STRUCT
};
//-------------------------------------------------------------
//------------------------------ ������λ
struct _stBingFaHoleBase
{
	uint16		uHoleID;			//��λID
	uint32		uEffectAttr;		//������Ч��ֱ�

	void initBase()	{   memset(this,0,sizeof(*this));	};

	READ_WRITE_STRUCT
};

//-------------------------------------------------------------
//------------------------------
struct _stBingFaHoleWulueEffect
{
	uint8	uWulueType;
	uint16	uYaodianID;
	uint32	uValue;				// Ӱ�����ֱ�

	READ_WRITE_STRUCT
};
#pragma pack(pop)
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------
struct _stBingFaQianXiuModel : _stBingFaQianXiuBase
{
	DMAP_NEED_ITEM				mapNeedItem;			// ���ĵĵ���

	void initModel()	
	{ 
		initBase();
		mapNeedItem.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stBingFaQianXiuBase::read(stream)&&
			stream.read(mapNeedItem);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stBingFaQianXiuBase::write(stream)&&
			stream.write(mapNeedItem);
	}
};
//-------------------------------------------------------------
//------------------------------
struct _stBingFaTuPoModel : _stBingFaTuPoBase
{
	DMAP_NEED_ITEM		mapNeedItem;				// ���ĵĵ���
	MAP_NEED_EXPEND		mapNeedExpend;				// ���ĵ���ֵ
	MAP_U32_U8			mapSkill;					// Я���ļ���
	VEC_U32				vecYaoDian;					// Ҫ��
	_stGameAttribute	stAttribute;				// ��ǰͻ�Ƶȼ����ӵ�����

	void initModel()	
	{ 
		initBase();
		mapNeedItem.clear();
		mapNeedExpend.clear();
		mapSkill.clear();
		vecYaoDian.clear();
		stAttribute.initGameAttribute();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stBingFaTuPoBase::read(stream)&&
			stream.read(mapNeedItem)&&
			stream.read(mapNeedExpend)&&
			stream.read(stAttribute)&&
			stream.read(vecYaoDian)&&
			stream.read(mapSkill);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stBingFaTuPoBase::write(stream)&&
			stream.write(mapNeedItem)&&
			stream.write(mapNeedExpend)&&
			stream.write(stAttribute)&&
			stream.write(vecYaoDian)&&
			stream.write(mapSkill);
	}
};
//-------------------------------------------------------------
//------------------------------
struct _stBingFaYaoDianModel : _stBingFaYaoDianBase
{
	DMAP_NEED_BINGFU	dmapNeedBingFu;				// ���ĵı��� key1:��ID KEy2:�����ȼ� key3:����
	MAP_NEED_EXPEND		mapNeedExpend;				// �۳�������
	_stGameAttribute	stAttribute;				// ��ǰҪ�����ӵ�����

	void initModel()	
	{ 
		initBase();
		dmapNeedBingFu.clear();
		mapNeedExpend.clear();
		stAttribute.initGameAttribute();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stBingFaYaoDianBase::read(stream)&&
			stream.read(dmapNeedBingFu)&&
			stream.read(mapNeedExpend)&&
			stream.read(stAttribute);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stBingFaYaoDianBase::write(stream)&&
			stream.write(dmapNeedBingFu)&&
			stream.write(mapNeedExpend)&&
			stream.write(stAttribute);
	}
};
//-------------------------------------------------------------
//------------------------------
struct _stBingFaTypeModel : _stBingFaTypeBase
{
	DMAP_NEED_ITEM						mapNeedItem;	// �������ĵĵ���
	stl_map<uint16,_stGameAttribute>	mapAttr;		// �������� ����ID ==> ��������
	void initModel()	
	{ 
		initBase();
		mapAttr.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stBingFaTypeBase::read(stream)&&
			stream.read(mapNeedItem)&&
			stream.read(mapAttr);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stBingFaTypeBase::write(stream)&&
			stream.write(mapNeedItem)&&
			stream.write(mapAttr);
	}
};
//-------------------------------------------------------------
//------------------------------
struct _stBingFaHoleModel : _stBingFaHoleBase
{
	DMAP_UINT8_32_32	dmapPreCondition;						// ǰ������
	stl_vector<_stBingFaHoleWulueEffect>	vecWulueEffect;		// ����Ӱ��ֵ  ����Ҫ�� ==�� ��ֵ
	void initModel()	
	{ 
		initBase();
		dmapPreCondition.clear();
		vecWulueEffect.clear();
	}

	inline bool		read			(CStreamBase& stream)
	{
		return _stBingFaHoleBase::read(stream)&&
			stream.read(vecWulueEffect)&&
			stream.read(dmapPreCondition);

	}
	inline bool		write			(CStreamBase& stream)
	{
		return _stBingFaHoleBase::write(stream)&&
			stream.write(vecWulueEffect)&&
			stream.write(dmapPreCondition);
	}
};