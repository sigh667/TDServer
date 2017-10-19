/*----------------- fashionData.h
*
* Copyright (C): 2017  Mokylin°§Mokyqi
* Author       : ’‘Œƒ‘¥
* Version      : V1.0
* Date         : 2017/3/30 18:10:53
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "cfg_data/fileData/fileData.h"
#include "fashionModel.h"
/*************************************************************/
class CFashionData	: public CFileData
{
public:
	enum
	{
		Version	= 20170330
	};

public:
	typedef stl_map<uint32,_stFashionModel>					MAP_FASHION_MODEL;
public:
	MAP_FASHION_MODEL		m_mapFashionModel;
public:
	virtual pc_str			getFileName	()const				{	return "fashion.dat";				}
	virtual pc_str			getXmlName	()const				{	return "cs_fashion.xml";			}

public:
	CFashionData();

public:
	inline const _stFashionModel*		findFashion(uint32 uFashionID)	const			{	return m_mapFashionModel.find_(uFashionID);			}

protected:
	virtual bool		onLoad				(TiXmlElement&xmlRoot);

protected:
	virtual bool		onLoad				(CFileStream&clFile);
	virtual bool		onSave				(CFileStream&clFile);

public:
	virtual void		onCompletioned		();

};
//-------------------------------------------------------------
extern CFashionData g_clFashionData;
