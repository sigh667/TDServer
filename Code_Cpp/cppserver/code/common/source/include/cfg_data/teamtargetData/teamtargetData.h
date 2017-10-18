/*----------------- teamtargetData.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : tanghaibo
* Version      : V1.0
* Date         : 2017/3/15 19:58:48
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "cfg_data/fileData/fileData.h"
#include "stl/std_map.h"
#include "teamtargetModel.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
class CTeamTargetData	: public CFileData
{
	enum
	{
		Version	= 2016022301
	};
private:
	typedef	stl_map<uint32, _stTeamTargetModel>	MAP_TEAMTARGET_MODEL;
	MAP_TEAMTARGET_MODEL						m_mapTeamTargetData;//队伍目标信息
	typedef stl_vector<uint32>					VECTOR_UNIQUE_TEAMTARGET;
	VECTOR_UNIQUE_TEAMTARGET					m_vcUniqueTeamTargetId;//队伍目标id快速索引

public:
	virtual pc_str	getFileName	()const	{	return "teamtargetData.dat";	}
	virtual pc_str	getXmlName	()const	{	return "cs_teamtarget.xml";	}
public:
	CTeamTargetData();
public:
	const _stTeamTargetModel*			findTeamTargetData		(uint32 uUniqueTeamTargetID)	const;
	uint32								randomGetUniqueTargetId	();
protected:
	virtual bool	onLoad		(TiXmlElement&xmlRoot);
private:
	bool			onLoadTeamTarget	(TiXmlElement&xmlRoot);
protected:
	virtual bool	onLoad		(CFileStream&clFile);
	virtual bool	onSave		(CFileStream&clFile);
};
//-------------------------------------------------------------
extern CTeamTargetData g_clTeamTargetData;
