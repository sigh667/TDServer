/*----------------- trapData.h
*
* Copyright (C): 2016  Mokylin°§Mokyqi
* Author       : ’‘Œƒ‘¥
* Version      : V1.0
* Date         : 2016/2/23 10:58:48
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "cfg_data/fileData/fileData.h"
#include "stl/std_map.h"
#include "trapModel.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
class CTrapData	: public CFileData
{
	enum
	{
		Version	= 2016022301
	};
private:
	typedef	double_map<uint32, uint16, _stTrapModel>	MAP_MODEL;
	MAP_MODEL	 	m_mapData;

public:
	virtual pc_str	getFileName	()const	{	return "trapData.dat";	}
	virtual pc_str	getXmlName	()const	{	return "cs_trap.xml";	}
public:
	CTrapData();
public:
	const _stTrapModel*	findTrap	(uint32 uTrapID,uint16 uLevel)	const;

protected:
	virtual bool	onLoad		(TiXmlElement&xmlRoot);
protected:
	virtual bool	onLoad		(CFileStream&clFile);
	virtual bool	onSave		(CFileStream&clFile);

private:
	_stTrapModel*	alloct		(uint32 uID,uint16 uLevel);
	void			load_trap_math(_stTrapModel* pSkillModel,TiXmlElement* pData);
	_stTrapModel*	find		(uint32 uTrapID,uint16 uLevel) { return m_mapData.find_(uTrapID,uLevel);}


};
//-------------------------------------------------------------
extern CTrapData	g_clTrapData;
