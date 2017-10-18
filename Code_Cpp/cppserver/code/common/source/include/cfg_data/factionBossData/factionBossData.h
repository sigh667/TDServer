/*----------------- factionBossData.h
*
* Copyright (C): 2017  Mokylin°§Mokyqi
* Author       : ’‘Œƒ‘¥
* Version      : V1.0
* Date         : 2017/1/12 15:53:46
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "cfg_data/fileData/fileData.h"
#include "factionBossModel.h"
/*************************************************************/
class CFactionBossData	: public CFileData
{
public:
	enum
	{
		Version	= 2017011201
	};

public:
	typedef		double_map<uint32,uint32,_stFactionBossPrizeModel>	DMAP_MODEL;
public:
	DMAP_MODEL	m_dmapModel;

public:
	virtual pc_str				getFileName	()const					{	return "factionboss.dat";				}
	virtual pc_str				getXmlName	()const					{	return "cs_factionboss.xml";			}

public:
	CFactionBossData();
protected:
	virtual bool				onLoad		(TiXmlElement&xmlRoot);
public:
	_stFactionBossPrizeModel*	findPrize	(uint8 uType,uint32 uSubType);
protected:
	virtual bool				onLoad		(CFileStream&clFile);
	virtual bool				onSave		(CFileStream&clFile);
public:
	virtual void			onCompletioned	();
};
//-------------------------------------------------------------
extern CFactionBossData g_clFactionBossData;
