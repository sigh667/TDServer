/*----------------- dazuoguajiData.h
*
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : tanghaibo
* Version      : V1.0
* Date         : 2017/03/28 14:47:14
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "cfg_data/fileData/fileData.h"
#include "dazuoguajiModel.h"
/*************************************************************/
class CDazuoguajiData	: public CFileData
{
public:
	enum
	{
		Version	= 20170328
	};

public:
	typedef stl_vector<_stDazuoguajiModel>		VECTOR_DAZUOGUAJI_MODEL;

public:
	VECTOR_DAZUOGUAJI_MODEL		m_vcDazuoguajiModel;

public:
	virtual pc_str			getFileName	()const				{	return "dazuoguaji.dat";				}
	virtual pc_str			getXmlName	()const				{	return "cs_dazuoguaji.xml";				}

public:
	CDazuoguajiData();

public:
	const _stDazuoguajiModel*	findDazuoguaji(uint32 uLevel )const ;

protected:
	virtual bool		onLoad				(TiXmlElement&xmlRoot);

protected:
	virtual bool		onLoad				(CFileStream&clFile);
	virtual bool		onSave				(CFileStream&clFile);

private:

};
//-------------------------------------------------------------
extern CDazuoguajiData	g_clDazuoguajiData;
