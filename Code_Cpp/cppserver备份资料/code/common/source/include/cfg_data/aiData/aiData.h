/*----------------- aiData.h
*
* Copyright (C): 2016  Mokylin°§Mokyqi
* Author       : ’‘Œƒ‘¥
* Version      : V1.0
* Date         : 2016/12/21 19:20:11
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "cfg_data/fileData/fileData.h"
#include "aiModel.h"
/*************************************************************/
class CAIData	: public CFileData
{
public:
	enum
	{
		Version	= 2016122101
	};

public:
	typedef		stl_map<uint32, _stAIModel>				MAP_MODEL;
public:
	MAP_MODEL	m_mapModel;
public:
	virtual pc_str				getFileName	()const					{	return "ai.dat";						}
	virtual pc_str				getXmlName	()const					{	return "cs_ai_config.xml";				}
public:
	CAIData();

public:
	inline const _stAIModel*	find(uint32 uID)const				{	return m_mapModel.find_(uID);			}
protected:
	virtual bool				onLoad		(TiXmlElement&xmlRoot);

protected:
	virtual bool				onLoad		(CFileStream&clFile);
	virtual bool				onSave		(CFileStream&clFile);
};
//-------------------------------------------------------------
extern CAIData	g_clAIData;
