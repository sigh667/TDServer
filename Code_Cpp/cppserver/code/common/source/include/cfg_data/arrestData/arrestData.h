/*----------------- identityData.h
*
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ³ÂÁÁ
* Version      : V1.0
* Date         : 2016/9/18 17:40:54
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "cfg_data/fileData/fileData.h"
#include "arrestModel.h"
/*************************************************************/
class CArrestData	: public CFileData
{
public:
	enum
	{
		Version	= 20161114
	};

public:
	typedef stl_map<uint32,_stArrestQuestModel>					MAP_ARRESTQUEST_MODEL;
	typedef stl_map<uint32,_stArrestNpcModel>					MAP_ARRESTNPC_MODEL;

public:
	MAP_ARRESTQUEST_MODEL		m_mapArrestQuestModel;
	MAP_ARRESTNPC_MODEL			m_mapArrestNpcModel;

public:
	virtual pc_str			getFileName	()const				{	return "arrest.dat";				}
	virtual pc_str			getXmlName	()const				{	return "cs_arrest.xml";				}

public:
	CArrestData();

public:
	inline const _stArrestQuestModel*	findArrestQuest		(uint32 uID)	const	{	return m_mapArrestQuestModel.find_(uID);	}
	inline const _stArrestNpcModel*		findArrestNpc		(uint32 uID)	const	{	return m_mapArrestNpcModel.find_(uID);		}
	
public:
	const _stArrestNpcModel*			findRandNpcModel	();

protected:
	virtual bool		onLoad				(TiXmlElement&xmlRoot);

protected:
	virtual bool		onLoad				(CFileStream&clFile);
	virtual bool		onSave				(CFileStream&clFile);

private:
	bool				onLoadArrestQuest	(TiXmlElement&xmlRoot);
	bool				onLoadArrestNpc		(TiXmlElement&xmlRoot);

public:
	virtual void	onCompletioned();
};
//-------------------------------------------------------------
extern CArrestData	g_clArrestData;
