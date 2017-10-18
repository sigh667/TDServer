/*----------------- bingfuData.h
*
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ³ÂÁÁ
* Version      : V1.0
* Date         : 2016/8/1 12:05:17
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "cfg_data/fileData/fileData.h"
#include "bingfuModel.h"
/*************************************************************/
class CBingfuData	: public CFileData
{
public:
	enum
	{
		Version	= 20160728
	};

public:
	typedef double_map<uint32,uint16,_stBingfuModel>		DMAP_BINGFU_MODEL;
	typedef double_map<uint32,uint32,_stBingfuModel*>		DMAP_BINGFU_EXPMODEL;
	typedef stl_map<uint32,_stBingfuModel*>					MAP_EXPBINGFU_MDOL;	//¾­Ñé±ø·û

public:
	DMAP_BINGFU_MODEL			m_dmapBingfuModel;
	DMAP_BINGFU_EXPMODEL		m_dmapExpBingfuModel;
	MAP_EXPBINGFU_MDOL			m_mapExpBingfuModel;

public:
	virtual pc_str			getFileName	()const				{	return "bingfu.dat";			}
	virtual pc_str			getXmlName	()const				{	return "cs_bingfu.xml";			}

public:
	CBingfuData();

public:
	const _stBingfuModel*		findBingfu			(uint32 uID,uint16 uLevel)	const	{	return m_dmapBingfuModel.find_(uID,uLevel);		}
	uint32						findBingfuExp		(uint32 uID,uint16 uLevel);
	uint32						getMaxLevelExp		(uint32 uID);
	uint16						getMaxLevel			(uint32 uID);
	const _stBingfuModel*		findBingfuByExp		(uint32 uID,uint32 uCurExp)	const;
	bool						getExpBinfu			(uint32 uBingfuExp,stl_map<_stBingfuModel*,uint32>& mapExpModel);

protected:
	virtual bool		onLoad			(TiXmlElement&xmlRoot);

protected:
	virtual bool		onLoad			(CFileStream&clFile);
	virtual bool		onSave			(CFileStream&clFile);

public:
	virtual	void		onCompletioned	();
};
//-------------------------------------------------------------
extern CBingfuData	g_clBingfuData;
