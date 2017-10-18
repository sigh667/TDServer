/*----------------- identityQuestData.h
*
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ³ÂÁÁ
* Version      : V1.0
* Date         : 2016/12/7 18:23:34
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "cfg_data/fileData/fileData.h"
#include "identityQuestModel.h"
/*************************************************************/
class CIdentityQuestData	: public CFileData
{
public:
	enum
	{
		Version	= 20161114
	};

public:
	typedef stl_map<uint32,_stIdentityQuestModel>				MAP_IDENTITYQUEST_MODEL;
	typedef stl_map<uint32,_stIdentityWenquPoemModel>			MAP_IDENTITYWENQUPOEM_MODEL;
	typedef stl_map<uint32,_stIdentityRewardZhenshenModel>		MAP_IDENTITYZHENSHEN_MODEL;

public:
	MAP_IDENTITYQUEST_MODEL		m_mapQuestModel;
	MAP_IDENTITYWENQUPOEM_MODEL	m_mapWenquPoemModel;
	MAP_IDENTITYZHENSHEN_MODEL	m_mapZhenshenModel;

public:
	virtual pc_str			getFileName	()const				{	return "identityquest.dat";				}
	virtual pc_str			getXmlName	()const				{	return "cs_identity_quest.xml";				}

public:
	CIdentityQuestData();

public:
	inline const _stIdentityQuestModel*		findQuest		(uint32 uID)	const	{	return m_mapQuestModel.find_(uID);		}
	inline const _stIdentityWenquPoemModel*	findWenquPoem	(uint32 uID)	const	{	return m_mapWenquPoemModel.find_(uID);		}
	const _stIdentityWenquPoemModel*		findRandPoem	();
	const _stIdentityRewardZhenshenModel*	findRandZhenshen();
protected:
	virtual bool		onLoad				(TiXmlElement&xmlRoot);

protected:
	virtual bool		onLoad				(CFileStream&clFile);
	virtual bool		onSave				(CFileStream&clFile);

private:
	bool				onLoadIdentityQuest				(TiXmlElement&xmlRoot);
	bool				onLoadIdentityWenquPoem			(TiXmlElement&xmlRoot);
	bool				onLoadIdentityRewardZhenshen	(TiXmlElement&xmlRoot);

public:
	virtual void	onCompletioned();
};
//-------------------------------------------------------------
extern CIdentityQuestData	g_clIdentityQuestData;
