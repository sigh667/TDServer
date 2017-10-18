/*----------------- sceneFlyData.h
*
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ·½Éù»Ô
* Version      : V1.0
* Date         : 2016/09/21 13:41:59
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "cfg_data/fileData/fileData.h"
#include "cfg_data/sceneFlyData/sceneFlyModel.h"
/*************************************************************/
class CSceneFlyData	: public CFileData
{
public:
	enum
	{
		Version	= 2016030401
	};

public:
	typedef stl_map<uint32,_stHostlerModel>				MAP_HOSTLER_MODEL;	
	typedef stl_map<uint32,_stFlyModel>					MAP_FLY_MODEL;	
	typedef stl_map<uint32,_stFlyNpcModel>				MAP_FINDNPC_MODEL;	

private:
	MAP_HOSTLER_MODEL		m_mapHostlerModel;
	MAP_FLY_MODEL			m_mapFlyModel;
	MAP_FINDNPC_MODEL		m_mapFindNpcModel;

public:
	virtual pc_str	getFileName					()const		{	return "scene_fly.dat";							}
	virtual pc_str	getXmlName					()const		{	return "cs_scene_fly.xml";						}
public:
	inline const _stHostlerModel*	findHostlerModel	(uint16 ID)		const	{	return m_mapHostlerModel.find_(ID);			}
	inline const _stFlyModel*		findFlyModel		(uint16 ID)		const	{	return m_mapFlyModel.find_(ID);				}
	inline const _stFlyNpcModel*	findFlyNpcModel		(uint16 ID)		const	{	return m_mapFindNpcModel.find_(ID);			}

public:
	CSceneFlyData();

protected:
	virtual bool	onLoad				(TiXmlElement&xmlRoot);

protected:
	bool			load_hostler_model	(TiXmlElement&xmlRoot);
	bool			load_fly_model		(TiXmlElement&xmlRoot); 
	bool			load_findnpc_model	(TiXmlElement&xmlRoot); 

protected:
	virtual bool	onLoad			(CFileStream&clFile);
	virtual bool	onSave			(CFileStream&clFile);
};
//-------------------------------------------------------------
extern 	CSceneFlyData	g_clSceneFlyData;

