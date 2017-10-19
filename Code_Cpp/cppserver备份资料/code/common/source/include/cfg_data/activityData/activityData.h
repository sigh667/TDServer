/*----------------- greatwallTrainData.h
*
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ·½Éù»Ô
* Version      : V1.0
* Date         : 2017/04/25 15:41:59
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "cfg_data/fileData/fileData.h"
#include "cfg_data/activityData/activityModel.h"
/*************************************************************/
class CActivityData	: public CFileData
{
public:
	enum
	{
		Version	= 2016030401
	};

public:
	typedef stl_map<uint32,_stActivityModel>				MAP_ACTIVITY_MODEL;	 
	typedef stl_map<uint32,_stLivenessModel>				MAP_LIVENESS_MODEL;	 
	typedef double_map<uint32, uint32, _stActivityModel*>	DMAP_ACTIVITY_MODEL;	 
private:
	MAP_ACTIVITY_MODEL		m_mapActivityModel;				    // <ID, _stActivityModel>
	MAP_LIVENESS_MODEL		m_mapLevenessModel;				    // <ID, _stLivenessModel>
	DMAP_ACTIVITY_MODEL	m_dmapActivityModel;				// <type, completeID, _stActivityModel>

public:
	virtual pc_str	getFileName					()const override	{	return "activity.dat";							}
	virtual pc_str	getXmlName					()const	override	{	return "cs_activity.xml";						}
public:
	inline const _stActivityModel*	findActivityModel	(uint32 ID)		const	{	return m_mapActivityModel.find_(ID);		}
	inline const _stLivenessModel*	findLevenessModel	(uint32 ID)		const	{	return m_mapLevenessModel.find_(ID);		}
	const		 _stActivityModel*	findActivityByTypeCompID(uint32 uType, uint32 uComplateID);
public:
	CActivityData();
protected:
	virtual bool	onLoad				(TiXmlElement&xmlRoot) override;

protected:
	bool	load_activity		(TiXmlElement&xmlRoot);
	bool	load_liveness		(TiXmlElement&xmlRoot);

protected:
	virtual bool	onLoad			(CFileStream&clFile) override;
	virtual bool	onSave			(CFileStream&clFile) override;
	virtual void	onCompletioned	()					 override;

};
//-------------------------------------------------------------
extern 	CActivityData	g_clActivityData;

