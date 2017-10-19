/*----------------- dropData.h
*
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ³ÂÁÁ
* Version      : V1.0
* Date         : 2016/7/22 10:32:32
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "cfg_data/fileData/fileData.h"
#include "dropModel.h"
/*************************************************************/
class CDropData	: public CFileData
{
public:
	enum
	{
		Version	= 20160722
	};

public:
	typedef double_map<uint16,uint16,_stDropCellModel>		DMAP_GROUP_MODEL;
	typedef stl_map<uint16,_stDropModel>					MAP_DROP_MODEL;

public:
	DMAP_GROUP_MODEL			m_mapGroupModel;
	MAP_DROP_MODEL				m_mapDropModel;

public:
	virtual pc_str			getFileName	()const				{	return "drop.dat";				}
	virtual pc_str			getXmlName	()const				{	return "cs_drop.xml";				}

public:
	CDropData();

public:
	bool				find			(uint16 uDropID,VEC_DROP_CELL& vecDropGroup);


protected:
	virtual bool		onLoad			(TiXmlElement&xmlRoot);

protected:
	virtual bool		onLoad			(CFileStream&clFile);
	virtual bool		onSave			(CFileStream&clFile);

private:
	bool					onLoadGroup			(TiXmlElement&xmlRoot);
	bool					onLoadDrop			(TiXmlElement&xmlRoot);
	const _stDropCellModel*	findGroup			(uint16 uGroupID) const;
};
//-------------------------------------------------------------
extern CDropData	g_clDropData;
