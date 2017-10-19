/*----------------- copymapData.h
*
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ³ÂÁÁ
* Version      : V1.0
* Date         : 2016/5/13 10:43:10
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "cfg_data/fileData/fileData.h"
#include "cfg_data/copymapData/copymapModel.h"
#include "stl/std_map.h"
#include "stl/std_hash_map.h"
/*************************************************************/
class CCopymapData	: public CFileData
{
public:
	enum
	{
		Version	= 2016011501
	};

public:
	typedef stl_map<uint16,_stCopymapModel>	MAP_MODEL;
	typedef double_map<uint32,uint8,_stCopymapModel*>		DMAP_TYPE_MODEL;
	typedef double_map<uint16,uint16,_stCopymapWaveModel>	DMAP_WAVE_MODEL;//KEY:¸±±¾ID key2:²¨´Î
	//typedef stl_map<uint32,_stUnitGroupModel>				MAP_GROUP_MODEL;
	typedef double_map<uint32,uint32,_stUnitGroupModel>		DMAP_GROUP_MODEL;

public:
	MAP_MODEL			m_mapModel;
	DMAP_TYPE_MODEL		m_dmapTypeModel;
	DMAP_WAVE_MODEL		m_dmapWaveModel;
	DMAP_GROUP_MODEL	m_dmapGroupModel;

public:
	virtual pc_str				getFileName		()const			{	return "copymap.dat";				}
	virtual pc_str				getXmlName		()const			{	return "cs_copymap.xml";			}
public:
	inline const _stCopymapModel*	find_model	(uint16 _id) const	{	return m_mapModel.find_(_id);		}
	uint16 findCopymap(uint32 uCopymapType,uint8 uDifficulty);
	void						findUnitGroup	(const _stCopymapWaveModel* pWaveModel,VEC_WAVE_UNITGROUP& vecGroup,const MAP_U32_U32& mapMonsterRate);
	const _stUnitGroupModel*	findGroup		(uint32 uGroupID,const MAP_U32_U32& mapMonsterRate) const;
	uint32						getMainType		(uint32 uType);

public:
	CCopymapData();

protected:
	virtual bool	onLoad				(TiXmlElement&xmlRoot);

private:
	bool			onLoadCopymap		(TiXmlElement&xmlRoot);
	bool			onLoadCopymapWave	(TiXmlElement&xmlRoot);
	bool			onLoadMonsterGroup	(TiXmlElement&xmlRoot);

protected:
	virtual bool	onLoad				(CFileStream&clFile);
	virtual bool	onSave				(CFileStream&clFile);

public:
	virtual void	onCompletioned	();
};
//-------------------------------------------------------------
extern CCopymapData	g_clCopymapData;

