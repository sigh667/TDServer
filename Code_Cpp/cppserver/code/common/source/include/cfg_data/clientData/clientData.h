/*----------------- clientData.h
*
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ³ÂÁÁ
* Version      : V1.0
* Date         : 2016/5/20 17:24:21
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "cfg_data/fileData/fileData.h"
/*************************************************************/
class CClientData
{
public:
	enum
	{
		Version	= 2016040801
	};
	struct _stClientData
	{
		uint32 uCrc;
		std::string strData;

		_stClientData()
		{
			uCrc = 0;
			strData.clear();
		}
	};
public:
	typedef stl_map<std::string,_stClientData>		MAP_CLENT_DATA;

private:
	MAP_CLENT_DATA			m_mapData;

public:
	virtual pc_str			getFileName	()const				{	return "expend.dat";				}
	virtual pc_str			getXmlName	()const				{	return "cs_expend.xml";				}

public:
	CClientData();

public:
	inline const _stClientData*	find (const std::string& strFileName)	const	{	return m_mapData.find_(strFileName); }

public:
	bool	reLoad			(std::string&	strClientDataPath);
	
private:
	bool	onLoad			(std::string&	strClientDataPath,std::string&	strFileName);

};
//-------------------------------------------------------------
extern CClientData	g_clClientData;
