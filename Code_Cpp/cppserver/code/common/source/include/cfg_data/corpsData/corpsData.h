/*----------------- corpsData.h
*
* Copyright (C): 2017  Mokylin¡¤Mokyqi
* Author       : ³ÂÁÁ
* Version      : V1.0
* Date         : 2017/6/9 16:54:50
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "cfg_data/fileData/fileData.h"
#include "corpsModel.h"
/*************************************************************/
class CCorpsData	: public CFileData
{
public:
	enum
	{
		Version	= 20170609
	};

public:
	typedef stl_map<uint32,_stCorpsJobModel>				MAP_CORPSJOBMODEL;


public:
	MAP_CORPSJOBMODEL		m_mapCorpsJobModel;

public:
	virtual pc_str			getFileName	()const				{	return "corps.dat";				}
	virtual pc_str			getXmlName	()const				{	return "cs_corps.xml";			}

public:
	CCorpsData();

public:
	inline const _stCorpsJobModel*		findCorpsJob		(uint32 uID)	const	{	return m_mapCorpsJobModel.find_(uID);		}

protected:
	virtual bool		onLoad				(TiXmlElement&xmlRoot);

protected:
	virtual bool		onLoad				(CFileStream&clFile);
	virtual bool		onSave				(CFileStream&clFile);

private:
	bool				onLoadCorpsJob		(TiXmlElement&xmlRoot);
};
//-------------------------------------------------------------
extern CCorpsData	g_clCorpsData;
