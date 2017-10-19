/*----------------- wulueData.h
*
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ³ÂÁÁ
* Version      : V1.0
* Date         : 2016/7/28 16:45:36
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "cfg_data/fileData/fileData.h"
#include "wulueModel.h"
/*************************************************************/
class CWulueData	: public CFileData
{
public:
	enum
	{
		Version	= 20160728
	};

public:
	typedef stl_map<uint64,_stWulueYaodianModel>			MAP_WULUEYAODIAN_MODEL;

public:
	MAP_WULUEYAODIAN_MODEL	m_mapYaodianModel;
public:
	virtual pc_str			getFileName	()const				{	return "wulue.dat";				}
	virtual pc_str			getXmlName	()const				{	return "cs_wulue.xml";			}

public:
	CWulueData();

public:
	inline const _stWulueYaodianModel*	findZhanlueModel(uint32 uKey)	const	{ return m_mapYaodianModel.find_(uKey); }
	const _stWulueYaodianModel*			findZhanlueModel(uint16 uID,uint8 uType,uint8 uLevel)	const;
	uint32								make_wulue_key	(uint16 uID,uint8 uType,uint8 uLevel)	const;
protected:
	virtual bool		onLoad			(TiXmlElement&xmlRoot);

protected:
	virtual bool		onLoad			(CFileStream&clFile);
	virtual bool		onSave			(CFileStream&clFile);
};
//-------------------------------------------------------------
extern CWulueData	g_clWulueData;
