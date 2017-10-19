/*----------------- systemSwitchData.h
*
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ·½Éù»Ô
* Version      : V1.0
* Date         : 2016/09/28 13:41:59
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "cfg_data/fileData/fileData.h"
#include "cfg_data/systemSwitchData/systemSwitchModel.h"
/*************************************************************/
typedef stl_map<uint32,_stSystemSwitchModel*>				MAP_SYSTEMSWITCH_MODEL;
typedef double_map<int8,uint32,_stSystemSwitchModel>		DMAP_SYSTEM_SWITCH_MODEL;
class CSystemSwitchData	: public CFileData
{
public:
	enum
	{
		Version	= 2016030401
	};
	typedef double_map<int8,uint8,_stSystemSwitchModel*>		DMAP_PROTOCOL_MODEL;
	typedef double_map<uint32,uint32,_stSystemSwitchModel*>		DMAP_QUEST_MODEL;
	typedef double_map<uint16,uint32,_stSystemSwitchModel*>		DMAP_LEVEL_MODEL;
	typedef double_map<uint16,uint32,_stSystemSwitchModel*>		DMAP_VIP_MODEL;	

	typedef double_map<uint32,uint32,uint32>		DMAP_BIGWORLD_LIMIT;	

private:
	DMAP_SYSTEM_SWITCH_MODEL	m_dmapSystemSwitchModel;
	DMAP_PROTOCOL_MODEL			m_dmapProtocolModel;
	DMAP_QUEST_MODEL			m_dmapQuestModel;
	DMAP_LEVEL_MODEL			m_dmapLevelModel;
	DMAP_VIP_MODEL				m_dmapVipModel;
	DMAP_BIGWORLD_LIMIT			m_dmapBigWorldLimitModel;

public:
	virtual pc_str	getFileName					()const		{	return "system_switch.dat";							}
	virtual pc_str	getXmlName					()const		{	return "cs_system_switch.xml";						}
public:
	const	_stSystemSwitchModel*	findSystermSwitchModel	(uint8 uMainProctocal, uint8 uSubProctocal);
	bool	findQuest(uint32 uQuestID,	MAP_SYSTEMSWITCH_MODEL& mapModel);
	bool	findLevel(uint16 uLevel,	MAP_SYSTEMSWITCH_MODEL& mapModel);
	bool	findVip(uint16 uVip,		MAP_SYSTEMSWITCH_MODEL& mapModel);
	uint32	level(uint32 uModelID) const;

	DMAP_SYSTEM_SWITCH_MODEL*		getSystemSwitchMoel()						{	return &m_dmapSystemSwitchModel;	}
	inline bool	isBigWorldLimitProtocol(uint8 uMainProtocol,uint8 uSubProtol)	{	return m_dmapBigWorldLimitModel.find_(uMainProtocol,uSubProtol) != NULL;	}

public:
	CSystemSwitchData();
	~CSystemSwitchData();

protected:
	virtual bool	onLoad				(TiXmlElement&xmlRoot);

protected:
	bool			load_system_switch_model	(TiXmlElement&xmlRoot);
	bool			load_bigworld_limit_model	(TiXmlElement&xmlRoot);

protected:
	virtual bool	onLoad			(CFileStream&clFile);
	virtual bool	onSave			(CFileStream&clFile);

public:
	virtual void	onCompletioned	();
};
//-------------------------------------------------------------
extern 	CSystemSwitchData	g_clSystemSwitchData;
