/*----------------- topListData.h
*
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ·½Éù»Ô
* Version      : V1.0
* Date         : 2017/06/06 15:46:16
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "cfg_data/fileData/fileData.h"
#include "cfg_data/topListData/topListModel.h"
/*************************************************************/
class CTopListData	: public CFileData
{
public:
	enum
	{
		Version	= 2016030401
	};

public:
	typedef stl_map<uint32,_strTopListModel>					MAP_TOPLIST_MODEL;	
	typedef stl_map<uint32,_strTopListRewardModel>				MAP_TOPLIST_REWARD;
	typedef double_map<uint32, uint32, _strTopListRewardModel>  DMAP_TOPLIST_REWARD;

private:
	MAP_TOPLIST_MODEL		m_mapTopListModel;				// <uType, _strTopListModel>
	DMAP_TOPLIST_REWARD		m_dmapTopListReward;			// <uType, uRank, _strTopListRewardModel>
public:
	virtual pc_str	getFileName					()const		{	return "toplist.dat";							}
	virtual pc_str	getXmlName					()const		{	return "cs_toplist.xml";						}
public:
	inline const _strTopListModel*	findToplistModel	(uint16 ID)		const	{	return m_mapTopListModel.find_(ID);	}
	const _strTopListRewardModel*	findToplistReward	(uint32 uType, uint32 uRank);
	bool							hasToplistReward	(uint32 uType);

public:
	CTopListData();
protected:
	virtual bool	onLoad				(TiXmlElement&xmlRoot);

protected:
	bool			load_toplist_model	(TiXmlElement&xmlRoot);
	bool			load_toplisreward_model(TiXmlElement&xmlRoot);

protected:
	virtual bool	onLoad			(CFileStream&clFile)	override;
	virtual bool	onSave			(CFileStream&clFile)	override;
	virtual void	onCompletioned	()						override;
};
//-------------------------------------------------------------
extern 	CTopListData	g_clTopListData;

