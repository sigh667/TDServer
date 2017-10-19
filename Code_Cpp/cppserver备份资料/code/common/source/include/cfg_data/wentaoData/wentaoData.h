/*----------------- wentaoData.h
*
* Copyright (C): 2016  Mokylin°§Mokyqi
* Author       : ’‘Œƒ‘¥
* Version      : V1.0
* Date         : 2016/8/3 10:32:55
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "cfg_data/fileData/fileData.h"
#include "wentaoModel.h"
/*************************************************************/
class CWenTaoData	: public CFileData
{
public:
	enum
	{
		Version	= 20160803
	};

public:
	typedef stl_map<uint64,_stBingFaQianXiuModel>				MAP_QianXiuModel;
	typedef stl_map<uint64,_stBingFaTuPoModel>					MAP_TuPoModel;
	typedef stl_map<uint32,_stBingFaYaoDianModel>				MAP_YaoDianModel;
	typedef stl_map<uint64,_stBingFaTypeModel>					MAP_TypeModel;
	typedef stl_map<uint64,_stBingFaHoleModel>					MAP_HoleModel;
public:
	MAP_QianXiuModel		m_mapQianXiuModel;
	MAP_TuPoModel			m_mapTuPoModel;	
	MAP_YaoDianModel		m_mapYaoDianModel;
	MAP_TypeModel			m_mapTypeModel;
	MAP_HoleModel			m_mapHoleModel;
public:
	virtual pc_str			getFileName	()const				{	return "wentao.dat";				}
	virtual pc_str			getXmlName	()const				{	return "cs_wentao.xml";			}

public:
	CWenTaoData();

public:
	const _stBingFaQianXiuModel*	findFaQianXiuModel	(uint16 uBingFaID,uint16 uQianXiuLevel)	const;
	const _stBingFaTuPoModel*		findTuPoModel		(uint16 uBingFaID,uint16 uTuPoLevel)	const;
	const _stBingFaYaoDianModel*	findYaoDianModel	(uint32 uYaoDianID)const;
	const _stBingFaTypeModel*		findTypeModel		(uint16 uBingFaID)const;
	const _stBingFaHoleModel*		findHoleModel		(uint16 uHoleID)const;
protected:
	virtual bool		onLoad			(TiXmlElement&xmlRoot);

protected:
	virtual bool		onLoad			(CFileStream&clFile);
	virtual bool		onSave			(CFileStream&clFile);
private:
	bool				onLoadTypeModel		(TiXmlElement&xmlRoot);
	bool				onLoadQianXiuModel	(TiXmlElement&xmlRoot);
	bool				onLoadTuPoModel		(TiXmlElement&xmlRoot);
	bool				onLoadYaoDianModel	(TiXmlElement&xmlRoot);
	bool				onLoadHoleModel		(TiXmlElement&xmlRoot);
};
//-------------------------------------------------------------
extern CWenTaoData	g_clWenTaoData;
