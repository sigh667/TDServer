/*----------------- worldBossData.h
*
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : tanghaibo
* Version      : V1.0
* Date         : 2017/05/09 14:47:14
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "cfg_data/fileData/fileData.h"
#include "worldBossModel.h"
/*************************************************************/
class CWorldBossData	: public CFileData
{
public:
	enum
	{
		Version	= 20170328
	};

public:
	typedef stl_vector<_stworldbossModel>		VECTOR_WORLDBOSS_MODEL;

	typedef stl_vector<_stworldbossrankprizeModel>		VECTOR_RANKPRIZE_MODEL;

public:
	VECTOR_WORLDBOSS_MODEL		m_vcWorldBossModel;
	VECTOR_RANKPRIZE_MODEL		m_vcRankPrizeModel;

public:
	virtual pc_str			getFileName	()const				{	return "worldboss.dat";				}
	virtual pc_str			getXmlName	()const				{	return "cs_world_boss.xml";				}

public:
	CWorldBossData();

public:
	const VECTOR_WORLDBOSS_MODEL & getAllWorldBoss()
	{
		return m_vcWorldBossModel;
	}
	uint32 findWorldBossPrizeByRank(uint32 uBossActivityId, uint32 uRank);
protected:
	virtual bool		onLoad				(TiXmlElement&xmlRoot);

protected:
	virtual bool		onLoad				(CFileStream&clFile);
	virtual bool		onSave				(CFileStream&clFile);

private:

};
//-------------------------------------------------------------
extern CWorldBossData	g_clWorldbossData;
