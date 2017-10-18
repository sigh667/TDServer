/*----------------- sceneMapData.h
*
* Copyright (C): 2011  
* Author       : утндт╢
* Version      : V1.0
* Date         : 2012/3/5 15:09:49
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "cfg_data/fileData/fileData.h"
#include "sceneMapModel.h"
/*************************************************************/
class CSceneMapData	: public CFileData
{
public:
	enum
	{
		Version	= 2013102501
	};

public:
	typedef stl_map<uint16,_stSceneMapModel>		MAP_SCENE;
	typedef MAP_SCENE::iterator						iterator;

public:
	MAP_SCENE			m_hashScene;

public:
	virtual pc_str	getFileName	()const	{	return "sceneMap.dat";	}
	virtual pc_str	getXmlName	()const	{	return "cs_scene.xml";	}

public:
	CSceneMapData();

public:
	inline const _stSceneMapModel*	find(uint16 uSceneID)	const	{	return m_hashScene.find_(uSceneID);		}
	inline		 _stSceneMapModel*	find(uint16 uSceneID)			{	return m_hashScene.find_(uSceneID);		}

	inline	iterator			begin	()					{	return m_hashScene.begin();					}
	inline	iterator			end		()					{	return m_hashScene.end();					}
protected:
	virtual bool	onLoad		(TiXmlElement&xmlRoot);

protected:
	virtual bool	onSave		(CFileStream&clFile);
	virtual bool	onLoad		(CFileStream&clFile);
public:
	virtual void	onCompletioned();
};
//-------------------------------------------------------------
extern CSceneMapData	g_clSceneMapData;
