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
#include "cfg_data/greatwallTrainData/greatwallTrainModel.h"
/*************************************************************/
class CGreatwallTrainData	: public CFileData
{
public:
	enum
	{
		Version	= 2016030401
	};

public:
	typedef stl_map<uint32,_stgGwTrainModel>				MAP_GWTRAIN_MODEL;	 

private:
	MAP_GWTRAIN_MODEL		m_mapGwTrainModel;				// <copymapID, _stgGwTrainModel>
	MAP_GWTRAIN_MODEL		m_mapLayerTrainModel;			// <layerID, _stgGwTrainModel>

public:
	virtual pc_str	getFileName					()const		{	return "greatwall_train.dat";							}
	virtual pc_str	getXmlName					()const		{	return "cs_greatwall_train.xml";						}
public:
	inline const _stgGwTrainModel*	findGwTrainModel	(uint16 ID)		const	{	return m_mapGwTrainModel.find_(ID);			}
	inline const _stgGwTrainModel*	findLayerTrainModel	(uint16 ID)		const	{	return m_mapLayerTrainModel.find_(ID);		}
	
public:
	CGreatwallTrainData();
protected:
	virtual bool	onLoad				(TiXmlElement&xmlRoot);

protected:
	bool			load_greatwall_train_model	(TiXmlElement&xmlRoot);

protected:
	virtual bool	onLoad			(CFileStream&clFile);
	virtual bool	onSave			(CFileStream&clFile);
};
//-------------------------------------------------------------
extern 	CGreatwallTrainData	g_clGreatwallTrainData;

