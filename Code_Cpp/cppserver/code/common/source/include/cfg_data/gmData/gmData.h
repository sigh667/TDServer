/*----------------- gmData.h
*
* Copyright (C): 2016  Mokylin°§Mokyqi
* Author       : ¡÷æ‘
* Version      : V1.0
* Date         : 2016/9/19 13:33:17
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "cfg_data/fileData/fileData.h"
#include "stl/std_map.h"
#include "cfg_data/gmData/gmModel.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
class CGMData	: public CFileData
{
	enum
	{
		Version	= 2016091916
	};
private:
	stl_map<std::string, _stGMModel> 	m_mapGMData;

public:
	virtual pc_str	getFileName	()const				{	return "gm.dat";				}
	virtual pc_str	getXmlName	()const				{	return "cs_gm.xml";				}
public:
	CGMData();

public:
	const _stGMModel*	find	(const std::string& sCmd) const;
	const stl_map<std::string, _stGMModel>&	getGMDataMap() const;

protected:
	virtual bool	onLoad		(TiXmlElement&xmlRoot);

protected:
	virtual bool	onLoad		(CFileStream&clFile);
	virtual bool	onSave		(CFileStream&clFile);
};
//-------------------------------------------------------------
extern CGMData	g_clGMData;