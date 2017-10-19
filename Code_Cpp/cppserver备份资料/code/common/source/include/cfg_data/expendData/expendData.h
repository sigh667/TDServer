/*----------------- expendData.h
*
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ³ÂÁÁ
* Version      : V1.0
* Date         : 2016/4/7 19:28:41
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "cfg_data/fileData/fileData.h"
#include "expendModel.h"
/*************************************************************/
class CExpendData	: public CFileData
{
public:
	enum
	{
		Version	= 2016040801
	};

public:
	typedef stl_map<uint16,_stExpendModel>		MAP_MODEL;

public:
	MAP_MODEL			m_mapModel;

public:
	virtual pc_str			getFileName	()const				{	return "expend.dat";				}
	virtual pc_str			getXmlName	()const				{	return "cs_expend.xml";				}

public:
	CExpendData();

public:
	inline const _stExpendModel*		find(uint16 uType) const	{	return m_mapModel.find_(uType);		}

protected:
	virtual bool	onLoad				(TiXmlElement&xmlRoot);

protected:
	virtual bool	onLoad			(CFileStream&clFile);
	virtual bool	onSave			(CFileStream&clFile);

};
//-------------------------------------------------------------
extern CExpendData	g_clExpendData;
