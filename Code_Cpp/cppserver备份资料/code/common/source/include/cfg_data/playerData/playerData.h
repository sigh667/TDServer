/*------------- playerData.h
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ³ÂÌÚ
* Version      : V1.01
* Date         : 2016/02/18
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "cfg_data/fileData/fileData.h"
#include "stl/std_map.h"
#include "playerModel.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
class CPlayerData	: public CFileData
{
	enum
	{
		Version	= 2016021801
	};
private:
	typedef	stl_map<uint16,_stPlayerModel>	MAP_LEVEL_MODEL;
	MAP_LEVEL_MODEL	 	m_mapLevelModel;

public:
	virtual pc_str	getFileName	()const	{	return "player_level.dat";		}
	virtual pc_str	getXmlName	()const	{	return "cs_player_level.xml";	}
public:
	CPlayerData();
public:
	const _stPlayerModel*		findLevel(uint16 uLevel)	const	{	return m_mapLevelModel.find_(uLevel);	}

protected:
	virtual bool	onLoad			(TiXmlElement&xmlRoot);
	bool			onLoadLevel		(TiXmlElement&xmlRoot);

protected:
	virtual bool	onLoad		(CFileStream&clFile);
	virtual bool	onSave		(CFileStream&clFile);
};
//-------------------------------------------------------------
extern CPlayerData	g_clPlayerData;
