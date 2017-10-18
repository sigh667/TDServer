/*----------------- campData.h
*
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ³ÂÁÁ
* Version      : V1.0
* Date         : 2016/3/7 16:02:38
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "cfg_data/fileData/fileData.h"
/*************************************************************/
class CCampData	: public CFileData
{
public:
	enum
	{
		Version	= 2016031701
	};

#define MAX_CAMP_NUM 20

private:
	uint8 m_uArray[MAX_CAMP_NUM][MAX_CAMP_NUM];

public:
	virtual pc_str			getFileName	()const				{	return "camp.data";					}
	virtual pc_str			getXmlName	()const				{	return "cs_camp.xml";				}

public:
	CCampData();

public:
	inline uint8	findCamp		(uint8 uFirstID,uint8 uSecond)	
	{
		if (uFirstID >= MAX_CAMP_NUM || uSecond >= MAX_CAMP_NUM)
			return 0;
		return m_uArray[uFirstID][uSecond];		
	}

protected:
	virtual bool	onLoad			(TiXmlElement&xmlRoot);

protected:
	virtual bool	onLoad			(CFileStream&clFile);
	virtual bool	onSave			(CFileStream&clFile);

};
//-------------------------------------------------------------
extern CCampData	g_clCampData;
