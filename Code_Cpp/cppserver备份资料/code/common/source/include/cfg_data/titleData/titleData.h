/*----------------- titleData.h
*
* Copyright (C): 2017  Mokylin¡¤Mokyqi
* Author       : ³ÂÁÁ
* Version      : V1.0
* Date         : 2017/3/15 10:50:34
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "cfg_data/fileData/fileData.h"
#include "titleModel.h"
/*************************************************************/
class CTitleData	: public CFileData
{
public:
	enum
	{
		Version	= 20170315
	};

public:
	typedef stl_map<uint32,_stTitleModel>					MAP_TITLE_MODEL;
	typedef stl_vector<const _stTitleModel*>				TYPE_TITLE_MODEL;

//	typedef double_map<uint8,uint32,_stTitleModel*>			TYPE_TITLE_MODEL;
public:
	MAP_TITLE_MODEL				m_mapTitleModel;
	TYPE_TITLE_MODEL			m_vecTypeModel[_emTitleConditionType_ARRAYSIZE];	
public:
	virtual pc_str			getFileName	()const				{	return "title.dat";				}
	virtual pc_str			getXmlName	()const				{	return "cs_title.xml";				}

public:
	CTitleData();

public:
	inline const _stTitleModel*		findTitle(uint32 uTitleID)	const			{	return m_mapTitleModel.find_(uTitleID);			}
	inline const TYPE_TITLE_MODEL&	findType(_emTitleConditionType emType)const	{	return m_vecTypeModel[emType];					}

protected:
	virtual bool		onLoad				(TiXmlElement&xmlRoot);

protected:
	virtual bool		onLoad				(CFileStream&clFile);
	virtual bool		onSave				(CFileStream&clFile);

public:
	virtual void		onCompletioned		();

};
//-------------------------------------------------------------
extern CTitleData g_clTitleData;
