/*----------------- equipData.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/8/15 20:23:10
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "cfg_data/fileData/fileData.h"
#include "equipModel.h"
#include "equipModelAppend.h"
/*************************************************************/
class CEquipData	: public CFileData
{
public:
	enum
	{
		Version	= 20160728
	};

public:
	typedef double_map<uint8,uint32,_stEquipEnhanceModel>		DMAP_EQUIP_ENHANCE;			//KEY1:装备类型 key2:总经验值
	typedef double_map<uint8,uint16,_stEquipEnhanceModel*>		DMAP_EQUIP_ENHANCE_LEVEL;	//KEY1:装备类型 key2:等级
	typedef double_map<uint8,uint32,_stEquipForgeModel>			DMAP_EQUIP_FORGE;			//KEY1:装备类型 key2:总经验值
	typedef double_map<uint8,uint16,_stEquipForgeModel*>		DMAP_EQUIP_FORGE_LEVEL;		//KEY1:装备类型 key2:等级
	typedef stl_map<uint32,_stEquipSpecailAttrModel>			MAP_EQUIP_SPECIAL;			//key:ID
	typedef stl_map<uint16,_stEquipMakeModel>					MAP_EQUIP_MAKE;
	typedef double_map<uint16,uint16,uint32>					DMAP_EQUIP_GROUP;			//key1:组ID  key2:极品类型 key3:权重
	typedef stl_map<uint32,_stEquipMakeGroupModel>				MAP_EQUIP_MAKEGROUP;		//KEY1:装备ID
	typedef double_map<uint16,uint16,_stEquipSpecailAttrModel*>	DMAP_EQUIP_SPECIAL_FIND;	//key1:极品类型  key2:等级 key3:模板
	typedef stl_map<uint16,_stEquipSuitModel>					MAP_SUIT_MODEL;				//key1:套装ID
	typedef stl_map<uint16,uint16>								MAP_EQUIP_SUIT_MODEL;		//key1:装备ID，套装ID
	typedef stl_map<uint32,uint32>								MAP_EQUIP_SKILLED_LEVEL_MODEL;//key1:等级level，升级等级所需要的熟练度点数
	typedef stl_map<uint32,_stequip_make_vigourModel>			MAP_EQUIP_VIGOUR_MODEL;		//key1:等级level，相对应等级的活力信息
	typedef stl_vector<uint32>									VECTOR_MAKEIDS;
	typedef stl_map<uint32, VECTOR_MAKEIDS>						MAP_SKILLED_LEVEL_PART_2_MAKEIDS;	//key skilledLevel * 100 + part, value:对应的打造公式id集合

public:
	DMAP_EQUIP_ENHANCE		m_dmapEnhanceModel;
	DMAP_EQUIP_ENHANCE_LEVEL m_dmapLevelEnhanceModel;
	DMAP_EQUIP_FORGE		m_dmapForgeModel;
	DMAP_EQUIP_FORGE_LEVEL	m_dmapLevelForgeModel;
	MAP_EQUIP_SPECIAL		m_mapSpecialModel;
	MAP_EQUIP_MAKE			m_mapMakeModel;
	MAP_EQUIP_MAKEGROUP		m_mapMakeGroupModel;
	DMAP_EQUIP_GROUP		m_dmapGroup;
	DMAP_EQUIP_SPECIAL_FIND	m_dmapSpecialFind;
	MAP_EQUIP_SUIT_MODEL	m_mapEquipSuitModel;
	MAP_SUIT_MODEL			m_mapSuitModel;
	MAP_EQUIP_SKILLED_LEVEL_MODEL   m_mpSkilledLevelModel;
	MAP_EQUIP_VIGOUR_MODEL	m_mpVigourModel;
	MAP_SKILLED_LEVEL_PART_2_MAKEIDS	m_mpSkilledLevelPart2IDs;
public:
	virtual pc_str			getFileName	()const				{	return "equip.dat";				}
	virtual pc_str			getXmlName	()const				{	return "cs_equip.xml";			}

public:
	CEquipData();

public:
	inline const _stEquipSpecailAttrModel*	findSpecailAttrModel(uint32 uID)		const	{ return m_mapSpecialModel.find_(uID);		}
	inline const _stEquipMakeModel*			findMakeModel		(uint16 uMakeEquipID)const	{ return m_mapMakeModel.find_(uMakeEquipID);	}
	inline const _stEquipSuitModel*			findSuitModel		(uint16 uEquipID)	const	{ return m_mapSuitModel.find_(uEquipID);	}
	uint16								findSuitID				(uint16 uEquipID);
	const _stEquipEnhanceModel*			findEnhanceModel		(uint8 uEquipType,uint32 uExp) const;
	_stEquipEnhanceModel*				findEnhanceModelByLevel	(uint8 uEquipType,uint16 uLevel) ;
	uint32								findEnhanceExpByLevel	(uint8 uEquipType,uint16 uLevel);
	uint16								getEnhanceLevel			(uint8 uEquipType,uint32 uExp);
	const _stEquipEnhanceModel*			findNextEnhanceModel	(uint8 uEquipType,uint32 uExp) const;
	_stEquipForgeModel*					findForgeModelByLevel	(uint8 uEquipType,uint16 uLevel) ;
	uint32								getMaxEnhanceExp		(uint8 uEquipType);
	const _stEquipForgeModel*			findForgeModel			(uint8 uEquipType,uint32 uExp) const;
	const _stEquipForgeModel*			findNextForgeModel		(uint8 uEquipType,uint32 uExp) const;
	uint16								getForgeLevel			(uint8 uEquipType,uint32 uExp);
	uint32								getMaxForgeExp			(uint8 uEquipType);
	uint32								findForgeExpByLevel		(uint8 uEquipType,uint16 uLevel);
	bool								getSpecialModel			(uint32 uAddEquipID,bool bUseLucky,uint16 uEquipLevel,VEC_U32& vecSpecialID);
	uint16								getEnhanceMaxLevel		(uint8 uEquipType);
	uint16								getForgeMaxLevel		(uint8 uEquipType);
	uint32								getSkilledLevelPoint	(uint32 uSkilledLevel);
	const _stequip_make_vigourModel*	getVigourModel			(uint32 uPlayerLevel);
	uint32								getVigourMaxByLevel		(uint32 uPlayerLevel);
	const VECTOR_MAKEIDS*				getMakeIDSBySkilledLevelPart(uint32 uEquipType, uint32 uSkilledLevel);

protected:
	virtual bool		onLoad			(TiXmlElement&xmlRoot);

protected:
	virtual bool		onLoad			(CFileStream&clFile);
	virtual bool		onSave			(CFileStream&clFile);
	virtual	void		onCompletioned	();
private:
	bool				onLoadEnhance		(TiXmlElement&xmlRoot);
	bool				onLoadSpecailAttr	(TiXmlElement&xmlRoot);
	bool				onLoadForge			(TiXmlElement&xmlRoot);
	bool				onLoadMake			(TiXmlElement&xmlRoot);
	bool				onLoadMakeGroup		(TiXmlElement&xmlRoot);
	bool				onLoadGroup			(TiXmlElement&xmlRoot);
	bool				onLoadSuit			(TiXmlElement&xmlRoot);
	bool				onLoadSkilledLevelModel	(TiXmlElement&xmlRoot);
	bool				onLoadVigourModel	(TiXmlElement&xmlRoot);

};
//-------------------------------------------------------------
extern CEquipData	g_clEquipData;
