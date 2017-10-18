/*----------------- questData.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 晁旭升
* Version      : V1.0
* Date         : 2016/2/29 10:23:11
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "cfg_data/fileData/fileData.h"
#include "stl/std_map.h"
#include "questModel.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
class CQuestData : public CFileData
{
	enum
	{
		Version	= 2016021801
	};

public:
	typedef		stl_vector<uint32>		VEC_QUEST_ID;
private:
	stl_map<uint32, _stQuestModel>		m_mapQuestModel	;// <任务id，任务配置>
	stl_map<uint32,_stQuestTarget>		m_mapQuestTarget;// <任务目标id，任务目标配置>
public:
	virtual pc_str	getFileName	()const	{	return "quest.dat";	}
	virtual pc_str	getXmlName	()const	{	return "cs_task.xml";	}
public:
	CQuestData();
public:
	const _stQuestModel*	findQuestModel		(uint32 uQuestId);
	const _stQuestTarget*	findQuestTarget		(uint32 uTarId);
protected:
	virtual bool			onLoad				(CFileStream&clFile);
	virtual bool			onSave				(CFileStream&clFile);
public:
	virtual bool			onLoad				(TiXmlElement&xmlRoot);

	virtual bool			onLoadQuestModel	(TiXmlElement&xmlRoot);
	virtual bool			onLoadQuestTarget	(TiXmlElement&xmlRoot);
public:
	virtual void			onCompletioned();
protected:
	void					buildPlaneQuest		();
	void					loadTargetAction	(stl_vector<_stQuestTargetAction>& vecAction,std::string&strBuffer);
};
//-------------------------------------------------------------
extern CQuestData	g_clQuestData;
