/*----------------- buffData.h
*
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ³ÂÁÁ
* Version      : V1.0
* Date         : 2016/2/3 11:19:39
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "cfg_data/fileData/fileData.h"
#include "buffModel.h"
/*************************************************************/
class CBuffData	: public CFileData
{
public:
	enum
	{
		Version	= 2016111701
	};

public:
	typedef double_map<uint32,uint8,_stBuffModel>		DMAP_MODEL;
	typedef double_map<uint32,uint32,uint8>				DMAP_Mutual_MODEL;//value _emBuffMutualType
	typedef stl_map<uint32,_stBuffStateAttrModel>		MAP_BuffStateAttrModel;

public:
	DMAP_MODEL				m_mapModel;
	DMAP_Mutual_MODEL		m_dmapGroupModel;
	DMAP_Mutual_MODEL		m_dmapIDModel;
	MAP_BuffStateAttrModel	m_mapBuffStateAttrModel;

public:
	virtual pc_str			getFileName	()const				{	return "buff.dat";					}
	virtual pc_str			getXmlName	()const				{	return "cs_buff.xml";				}

public:
	CBuffData();

public:
	inline _stBuffModel*				find(uint32 _buffID,uint8 uLevel)	{	return m_mapModel.find_(_buffID,uLevel);	}
	inline DMAP_Mutual_MODEL::_sub_map*	findGroupMutual	(uint32 uGroupID)	{	return m_dmapGroupModel.find_(uGroupID);}
	inline DMAP_Mutual_MODEL::_sub_map*	findIDMutual	(uint32 uBuffID)	{	return m_dmapIDModel.find_(uBuffID);	}

protected:
	virtual bool			onLoad				(TiXmlElement&xmlRoot);
	virtual bool			onLoadBuff			(TiXmlElement&xmlRoot);
	virtual bool			onLoadGroupMutual	(TiXmlElement&xmlRoot);
	virtual bool			onLoadIDMutual		(TiXmlElement&xmlRoot);
	virtual bool			onLoadState			(TiXmlElement&xmlRoot);

protected:
	virtual bool	onLoad			(CFileStream&clFile);
	virtual bool	onSave			(CFileStream&clFile);

protected:
	void			load_buff_math	(_stBuffModel* pModel,TiXmlElement* pData);
	_stBuffModel*	alloclBuff		(uint32 ID,uint8 ulevel);
protected:
	virtual void	onCompletioned	();

};
//-------------------------------------------------------------
extern CBuffData	g_clBuffData;
