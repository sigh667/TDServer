/*----------------- strongerData.h
*
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ·½Éù»Ô
* Version      : V1.0
* Date         : 2017/04/25 15:41:59
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "cfg_data/fileData/fileData.h"
#include "cfg_data/strongerData/strongerModel.h"
/*************************************************************/
class CStrongerData	: public CFileData
{
public:
	enum
	{
		Version	= 2016030401
	};

public:
	typedef stl_map<uint32,_stStrongerModel>				MAP_ACTIVITY_MODEL;	 

private:
	MAP_ACTIVITY_MODEL		m_mapStrongerModel;				    // <ID, _stStrongerModel>

public:
	virtual pc_str	getFileName					()const override	{	return "stronger.dat";							}
	virtual pc_str	getXmlName					()const	override	{	return "cs_stronger.xml";						}
public:
	_stStrongerModel * getStrongerModel			(uint32 uID)		{	return m_mapStrongerModel.find_(uID);			}
public:
	CStrongerData();
protected:
	virtual bool	onLoad				(TiXmlElement&xmlRoot) override;

protected:
	bool	load_stronger		(TiXmlElement&xmlRoot);

protected:
	virtual bool	onLoad			(CFileStream&clFile) override;
	virtual bool	onSave			(CFileStream&clFile) override;
	virtual void	onCompletioned	()					 override;

};
//-------------------------------------------------------------
extern 	CStrongerData	g_clStrongerData;

