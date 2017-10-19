/*----------------- treasureMapData.h
*
* Copyright (C): 2017  Mokylin¡¤Mokyqi
* Author       : ³ÂÁÁ
* Version      : V1.0
* Date         : 2017/4/21 16:23:29
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "cfg_data/fileData/fileData.h"
#include "treasureMapModel.h"
/*************************************************************/
class CTreasureMapData	: public CFileData
{
public:
	enum
	{
		Version	= 20170421
	};

public:
	typedef stl_map<uint32,_stTreasureMapModel>				MAP_TREASUREMAPMODEL;
	typedef stl_map<uint32,_stTreasurePrizeModel>			MAP_TREASUREPRIZEMODEL;

public:
	MAP_TREASUREMAPMODEL		m_mapTreasureMapModel;
	MAP_TREASUREPRIZEMODEL		m_mapTreasurepPrizeModel;

public:
	virtual pc_str			getFileName	()const				{	return "treasure_map.dat";				}
	virtual pc_str			getXmlName	()const				{	return "cs_treasure_map.xml";			}

public:
	CTreasureMapData();

public:
	inline const _stTreasureMapModel*		findTreasureMap		(uint32 uTreasureType)	const	{	return m_mapTreasureMapModel.find_(uTreasureType);		}
	inline const _stTreasurePrizeModel*		findTreasurePrize	(uint32 uPrizeID)		const	{	return m_mapTreasurepPrizeModel.find_(uPrizeID);		}

protected:
	virtual bool		onLoad				(TiXmlElement&xmlRoot);

protected:
	virtual bool		onLoad				(CFileStream&clFile);
	virtual bool		onSave				(CFileStream&clFile);

private:
	bool				onLoadTreasureMap		(TiXmlElement&xmlRoot);
	bool				onLoadTreasurePrize		(TiXmlElement&xmlRoot);

public:
	virtual void		onCompletioned();
};
//-------------------------------------------------------------
extern CTreasureMapData	g_clTreasureMapData;
