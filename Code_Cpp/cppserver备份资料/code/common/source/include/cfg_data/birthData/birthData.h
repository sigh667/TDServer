/*----------------- birthData.h
*
* Copyright (C): 2016  Mokylin°§Mokyqi
* Author       : ’‘Œƒ‘¥
* Version      : V1.0
* Date         : 2016/1/7 13:33:17
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "cfg_data//fileData/fileData.h"
#include "stl/std_map.h"
#include "cfg_data/birthData/birthModel.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
class CBirthData	: public CFileData
{
	enum
	{
		Version	= 2016010701
	};
private:
	stl_map<uint8,_stBirthModel> 	m_mapBirthData;

public:
	virtual pc_str	getFileName	()const	{	return "birth.dat";		}
	virtual pc_str	getXmlName	()const	{	return "s_birth.xml";	}
public:
	CBirthData();

public:
	const	_stBirthModel*	find	(uint8 uJob)	const;

protected:
	virtual bool	onLoad		(TiXmlElement&xmlRoot);

protected:
	virtual bool	onLoad		(CFileStream&clFile);
	virtual bool	onSave		(CFileStream&clFile);
};
//-------------------------------------------------------------
extern CBirthData	g_clBirthData;