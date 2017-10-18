/*----------------- pvpData.h
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
#include "pvpModel.h"
/*************************************************************/
class CPvpData	: public CFileData
{
public:
	enum
	{
		Version	= 20161114
	};

public:
	typedef stl_map<uint32,_stPvpModel>						MAP_PVPMODEL;
	typedef stl_map<uint32,_stSinglePvpPrizeModel>			MAP_SINGLEPVP_PRIZEMODEL;
	typedef stl_map<uint32,_stSinglePvpDayPrizeModel>		MAP_SINGLEPVP_DAYPRIZEMODEL;

public:
	MAP_PVPMODEL					m_mapPvpModel;
	MAP_SINGLEPVP_PRIZEMODEL		m_mapSinglePrizeModel;
	MAP_SINGLEPVP_DAYPRIZEMODEL		m_mapSingleDayPrizeModel;

public:
	virtual pc_str			getFileName	()const				{	return "pvp.dat";				}
	virtual pc_str			getXmlName	()const				{	return "cs_pvp.xml";			}

public:
	CPvpData();

public:
	inline const _stPvpModel*			findPvp				(uint32 uID)	const{	return m_mapPvpModel.find_(uID);						}
	inline const _stSinglePvpPrizeModel*findSinglePvpPrize	(uint32 uScore)	const{	return m_mapSinglePrizeModel.find_lowerbound(uScore);	}
	inline const _stSinglePvpDayPrizeModel*findSinglePvpDayPrize	(uint32 uCount)	const{	return m_mapSingleDayPrizeModel.find_(uCount);	}
	uint32								getSinglePvpMaxScore();
protected:
	virtual bool		onLoad				(TiXmlElement&xmlRoot);

protected:
	virtual bool		onLoad				(CFileStream&clFile);
	virtual bool		onSave				(CFileStream&clFile);

private:
	bool				onLoadPvp				(TiXmlElement&xmlRoot);
	bool				onLoadSinglePvpPrize	(TiXmlElement&xmlRoot);
	bool				onLoadSinglePvpDayPrize	(TiXmlElement&xmlRoot);

};
//-------------------------------------------------------------
extern CPvpData	g_clPvpData;
