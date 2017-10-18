/*----------------- haloData.h
*
* Copyright (C): 2015  Mokylin¡¤Mokyqi
* Author       : ³ÂÌÚ
* Version      : V1.0
* Date         : 2015/3/10 19:09
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "cfg_data/fileData/fileData.h"
#include "haloModel.h"
/*************************************************************/
class CHaloData	: public CFileData
{
public:
	enum
	{
		Version	= 2016030401
	};

public:
	typedef		stl_map<uint16, _stHaloModel>		MAP_MODEL;

public:
	MAP_MODEL	m_mapModel;

public:
	virtual pc_str				getFileName	()const					{	return "halo.dat";						}
	virtual pc_str				getXmlName	()const					{	return "cs_halo.xml";					}

public:
	CHaloData();

public:
	inline const _stHaloModel*	find		(uint16 uID)	const	{	return m_mapModel.find_(uID);			}

protected:
	virtual bool				onLoad		(TiXmlElement&xmlRoot);

protected:
	virtual bool				onLoad		(CFileStream&clFile);
	virtual bool				onSave		(CFileStream&clFile);
};
//-------------------------------------------------------------
extern CHaloData	g_clHaloData;
