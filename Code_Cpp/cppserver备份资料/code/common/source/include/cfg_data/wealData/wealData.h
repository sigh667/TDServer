/*----------------- wealData.h
*
* Copyright (C): 2017  Mokylin¡¤Mokyqi
* Author       : ³ÂÁÁ
* Version      : V1.0
* Date         : 2017/6/15 16:33:24
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "cfg_data/fileData/fileData.h"
#include "wealModel.h"
/*************************************************************/
class CWealData	: public CFileData
{
public:
	enum
	{
		Version	= 20170609
	};

public:
	typedef stl_map<uint32,_stLoginWealModel>					MAP_LOGINMODEL;
	typedef double_map<uint32,uint32,_stPrizeWealModel>			DMAP_PRIZEMODEL;

public:
	MAP_LOGINMODEL		m_mapLoginModel;
	DMAP_PRIZEMODEL		m_dmapPrizeModel;

public:
	virtual pc_str			getFileName	()const				{	return "weal.dat";				}
	virtual pc_str			getXmlName	()const				{	return "cs_weal.xml";			}

public:
	CWealData();

public:
	inline const _stLoginWealModel*		findLoginWeal	(uint32 uID)	const			{	return m_mapLoginModel.find_(uID);			}
	inline const _stPrizeWealModel*		findPrizeWeal	(uint32 uID,uint32 uDay)const	{	return m_dmapPrizeModel.find_(uID,uDay);	}


protected:
	virtual bool		onLoad				(TiXmlElement&xmlRoot);

protected:
	virtual bool		onLoad				(CFileStream&clFile);
	virtual bool		onSave				(CFileStream&clFile);

private:
	bool				onLoadLoginWeal		(TiXmlElement&xmlRoot);
	bool				onLoadPrizeWeal		(TiXmlElement&xmlRoot);

};
//-------------------------------------------------------------
extern CWealData	g_clWealData;
