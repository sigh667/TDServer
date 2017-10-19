/*----------------- dartData.h
*
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : tanghaibo
* Version      : V1.0
* Date         : 2016/2/23 10:58:48
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "cfg_data/fileData/fileData.h"
#include "stl/std_map.h"
#include "dartModel.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
class CDartData	: public CFileData
{
	enum
	{
		Version	= 2016022301
	};
private:
	typedef	stl_map<uint32, _stDartModel>	MAP_NORMALDART_MODEL;
	MAP_NORMALDART_MODEL	 	m_mapNormalDartData;

	typedef	stl_map<uint32, _stDartBuffModel>	MAP_DARTBUFF_MODEL;
	MAP_DARTBUFF_MODEL	 	m_mapDartBuffData;

	typedef	stl_map<uint32, _stRobMonsterModel>	MAP_DARTROB_MODEL;
	MAP_DARTROB_MODEL	 	m_mapDartRobMonsterData;


public:
	virtual pc_str	getFileName	()const	{	return "dartData.dat";	}
	virtual pc_str	getXmlName	()const	{	return "cs_dart.xml";	}
public:
	CDartData();
public:
	const _stDartModel*			findNormalDartData		(uint32 uNormalDartID)	const;
	const _stDartBuffModel*		findDartBuffData		(uint32 uDartBuffID)	const;
	const _stRobMonsterModel*	findRobMonsterData		(uint32 uNormalDartID)	const;
protected:
	virtual bool	onLoad		(TiXmlElement&xmlRoot);
private:
	bool			onLoadDart	(TiXmlElement&xmlRoot);
	bool			onLoadDartBuff(TiXmlElement&xmlRoot);
	bool			onLoadMonster(TiXmlElement&xmlRoot);
protected:
	virtual bool	onLoad		(CFileStream&clFile);
	virtual bool	onSave		(CFileStream&clFile);
};
//-------------------------------------------------------------
extern CDartData	g_clDartData;
