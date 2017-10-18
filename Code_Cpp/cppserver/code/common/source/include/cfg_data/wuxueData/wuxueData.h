/*----------------- wuxueData.h
*
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ³ÂÁÁ
* Version      : V1.0
* Date         : 2016/10/27 20:47:14
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "cfg_data/fileData/fileData.h"
#include "wuxueModel.h"
/*************************************************************/
class CWuxueData	: public CFileData
{
public:
	enum
	{
		Version	= 20160722
	};

public:
	typedef double_map<uint8,uint8,_stWuxueModel>		DMAP_WUXUE_MODEL;

public:
	DMAP_WUXUE_MODEL		m_dmapWuxueModel;

public:
	virtual pc_str			getFileName	()const				{	return "wuxue.dat";				}
	virtual pc_str			getXmlName	()const				{	return "cs_wuxue.xml";				}

public:
	CWuxueData();

public:
	inline const DMAP_WUXUE_MODEL::_sub_map*	findWuxue(uint8 uJob)const {	return m_dmapWuxueModel.find_(uJob);			}

protected:
	virtual bool		onLoad				(TiXmlElement&xmlRoot);

protected:
	virtual bool		onLoad				(CFileStream&clFile);
	virtual bool		onSave				(CFileStream&clFile);

private:
	bool				onLoadWuxue		(TiXmlElement&xmlRoot);

};
//-------------------------------------------------------------
extern CWuxueData	g_clWuxueData;
