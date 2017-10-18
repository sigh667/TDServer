/*----------------- prizeData.h
*
* Copyright (C): 2016  Mokylin°§Mokyqi
* Author       : ’‘Œƒ‘¥
* Version      : V1.0
* Date         : 2016/4/21 14:31:36
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "cfg_data/fileData/fileData.h"
#include "prizeModel.h"
/*************************************************************/
class CPrizeData	: public CFileData
{
public:
	enum
	{
		Version	= 2016042101
	};

public:
	typedef		stl_map<uint32, _stPrizeModel>				MAP_MODEL;
	typedef		stl_vector<const _stPrizeModel*>			VEC_PRIZE_MODEL;
	typedef		double_map<uint8,uint32,VEC_PRIZE_MODEL>	DMAP_MODEL;

public:
	MAP_MODEL	m_mapModel;
	DMAP_MODEL	m_dmapModel;

public:
	virtual pc_str				getFileName	()const					{	return "prize.dat";						}
	virtual pc_str				getXmlName	()const					{	return "cs_prize.xml";					}

public:
	CPrizeData();

public:
	inline const _stPrizeModel*		find(uint32 uID)		const		{	return m_mapModel.find_(uID);				}
	inline const VEC_PRIZE_MODEL*	find(uint8 uType,uint32 uSubType)	{	return m_dmapModel.find_(uType,uSubType);	}
	const _stPrizeModel*		findQuestPrizeModel(uint32 uQuestID);

protected:
	virtual bool				onLoad		(TiXmlElement&xmlRoot);

protected:
	virtual bool				onLoad		(CFileStream&clFile);
	virtual bool				onSave		(CFileStream&clFile);
public:
	virtual void			onCompletioned	();
};
//-------------------------------------------------------------
extern CPrizeData	g_clPrizeData;
