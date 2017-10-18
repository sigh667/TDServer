/*----------------- monsterData.h
*
* Copyright (C): 2016  Mokylin°§Mokyqi
* Author       : ’‘Œƒ‘¥
* Version      : V1.0
* Date         : 2016/2/19 17:08:50
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "cfg_data/fileData/fileData.h"
#include "stl/std_map.h"
#include "monsterModel.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
class CMonsterData	: public CFileData
{
	enum
	{
		Version	= 2016042601
	};
private:
	typedef	stl_map<uint32, _stMonsterModel>			MAP_MODEL;
	typedef stl_map<uint32, _stMonsterColletionModel>	MAP_COLLETION_MODEL;
	typedef double_map<uint32,uint16,_stGameAttribute>	DMAP_LEVEATTR_MODEL;

	MAP_MODEL	 		m_mapData;
	MAP_COLLETION_MODEL m_mapColletionData;
	DMAP_LEVEATTR_MODEL m_dmapLevelAttrData;

public:
	virtual pc_str	getFileName	()const	{	return "monsterData.dat";	}
	virtual pc_str	getXmlName	()const	{	return "cs_monster.xml";	}
public:
	CMonsterData();
public:
	const _stMonsterModel*			find(uint32 uMonsterID)	const;
	const _stMonsterColletionModel*	findMonsterColletionModel(uint32 uMonsterID)	const;
	const _stGameAttribute*			findLevelAttr(uint32 uAttrIndex,uint16 uLevel)	const;

protected:
	virtual bool	onLoad		(TiXmlElement&xmlRoot);
	virtual bool	load_monster(TiXmlElement&xmlRoot);
	virtual bool    load_monster_colletion(TiXmlElement & xmlRoot);
	virtual bool	load_level_attr(TiXmlElement&xmlRoot);

	virtual bool	load_anim	();

protected:
	virtual bool	onLoad		(CFileStream&clFile);
	virtual bool	onSave		(CFileStream&clFile);
	
public:
	virtual void	onCompletioned	();
};
//-------------------------------------------------------------
extern CMonsterData	g_clMonsterData;
