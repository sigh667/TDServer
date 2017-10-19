/*----------------- greatwallTrainData.h
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
#include "cfg_data/reliveData/reliveModel.h"
/*************************************************************/
class CReliveData	: public CFileData
{
public:
	enum
	{
		Version	= 2016030401
	};

public:
	typedef stl_map<uint32,_stReliveModel>				MAP_RELIVE_MODEL;	 
	
private:
	MAP_RELIVE_MODEL		m_mapReliveModel;				    // <ID, _stReliveModel>
	
public:
	virtual pc_str	getFileName					()const override	{	return "relive.dat";							}
	virtual pc_str	getXmlName					()const	override	{	return "cs_relive.xml";						}
public:
	 uint32			findReliveModel				(uint32 uCount); 
public:
	CReliveData();
protected:
	virtual bool	onLoad				(TiXmlElement&xmlRoot) override;

protected:
	bool	load_relive		(TiXmlElement&xmlRoot);
	

protected:
	virtual bool	onLoad			(CFileStream&clFile) override;
	virtual bool	onSave			(CFileStream&clFile) override;
	virtual void	onCompletioned	()					 override;

};
//-------------------------------------------------------------
extern 	CReliveData	g_clReliveData;

