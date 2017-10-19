/*----------------- teamTaskData.h
*
* Copyright (C): 2017  Mokylin¡¤Mokyqi
* Author       : ³ÂÁÁ
* Version      : V1.0
* Date         : 2017/4/1 11:20:14
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "cfg_data/fileData/fileData.h"
#include "teamTaskModel.h"
/*************************************************************/
class CTeamTaskData	: public CFileData
{
public:
	enum
	{
		Version	= 20161114
	};

public:
	typedef stl_map<uint32,_stTeamTaskModel>				MAP_TEAMTASKMODEL;
	typedef double_map<uint64,uint16,_stTeamTaskPrizeModel>	DMAP_TEAMTASKPRIZEMODEL;


public:
	MAP_TEAMTASKMODEL		m_mapTeamTaskModel;
	DMAP_TEAMTASKPRIZEMODEL	m_dmapPrizeModel;

public:
	virtual pc_str			getFileName	()const				{	return "teamtask.dat";				}
	virtual pc_str			getXmlName	()const				{	return "cs_team_task.xml";				}

public:
	CTeamTaskData();

public:
	inline const _stTeamTaskModel*		findTeamTask		(uint32 uID)	const	{	return m_mapTeamTaskModel.find_(uID);		}
	const _stTeamTaskPrizeModel*		findTeamTaskPrize	(uint32 uID,uint32 uCount,uint16 uLevel)	const;

protected:
	virtual bool		onLoad				(TiXmlElement&xmlRoot);

protected:
	virtual bool		onLoad				(CFileStream&clFile);
	virtual bool		onSave				(CFileStream&clFile);

private:
	bool				onLoadTeamTask		(TiXmlElement&xmlRoot);
	bool				onLoadTeamTaskPrize	(TiXmlElement&xmlRoot);
};
//-------------------------------------------------------------
extern CTeamTaskData	g_clTeamTaskData;
