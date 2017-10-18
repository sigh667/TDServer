/*----------------- climbTowerData.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : tanghaibo
* Version      : V1.0
* Date         : 2017/06/23 14:47:14
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "cfg_data/fileData/fileData.h"
#include "manorModel.h"
/*************************************************************/
class CManorData	: public CFileData
{
public:
	enum
	{
		Version	= 20170623
	};

public:

	typedef stl_map<uint32, _stmanorModel> MAP_ID2MANOR;
	MAP_ID2MANOR m_mpID2Manor;

public:

public:
	virtual pc_str			getFileName	()const				{	return "manor.dat";				}
	virtual pc_str			getXmlName	()const				{	return "cs_manor.xml";			}

public:
	CManorData();

public:
	const MAP_ID2MANOR & getAllManorModel(){return m_mpID2Manor;}
	bool				 canApply		 (uint32 uManorID);//判断是否在报名活动时间范围内
	bool				 canFight		 (uint32 uManorID);//判断是否在领地战活动范围内
	bool				 canApply		 (_stmanorModel & model);//判断是否在报名活动时间范围内
	bool				 canFight		 (_stmanorModel & model);//判断是否在领地战活动范围内
	uint32				 calcValidFightSeconds (_stmanorModel & model);//计算得到活动总秒数
	uint32				 calcValidFightSeconds (uint32 uManorID);//计算得到活动总秒数
	bool				 canCloseSettle	 (_stmanorModel & model);//判断是否可以停止投票,结算
	_stmanorModel*		 findModel		 (uint32 uManorID){return m_mpID2Manor.find_(uManorID);}

protected:
	virtual bool		onLoad				(TiXmlElement&xmlRoot);

protected:
	virtual bool		onLoad				(CFileStream&clFile);
	virtual bool		onSave				(CFileStream&clFile);
private:

};
//-------------------------------------------------------------
extern CManorData	g_clManorData;
