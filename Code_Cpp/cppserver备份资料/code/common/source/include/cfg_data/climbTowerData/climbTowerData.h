/*----------------- climbTowerData.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : tanghaibo
* Version      : V1.0
* Date         : 2017/03/28 14:47:14
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "cfg_data/fileData/fileData.h"
#include "climbTowerModel.h"
/*************************************************************/
class CClimbTowerData	: public CFileData
{
public:
	enum
	{
		Version	= 20170328
	};

public:
	stl_vector<uint32>		m_vcEasyOrder;
	stl_vector<uint32>		m_vcHardOrder;
	stl_vector<uint32>		m_vcEliteOrder;

	typedef stl_map<uint32, _stClimbPrizeModel> MAP_ID2CLIMBPRIZE;
	MAP_ID2CLIMBPRIZE m_mpID2ClimbPrize;

	typedef stl_vector<_stClimbLuckDrawModel> VECTOR_CLIMBLUCKDRAWS;
	typedef stl_map<uint32, VECTOR_CLIMBLUCKDRAWS> MAP_ID2LUCKDRAW;
	MAP_ID2LUCKDRAW m_mpID2LuckDraw;//奖励的细节
	typedef stl_map<uint32, _stClimbLuckDrawModel> MAP_CLIMBLUCKDRAWS;
	MAP_CLIMBLUCKDRAWS	m_mpID2LuckDrawItem;	//一个冗余的备份,可以通过流水号快速查询奖励

public:

public:
	virtual pc_str			getFileName	()const				{	return "climbtower.dat";				}
	virtual pc_str			getXmlName	()const				{	return "cs_climb_tower.xml";			}

public:
	CClimbTowerData();

public:
	bool				findDrawByPrizeID(uint32 uPrizeID, VECTOR_CLIMBLUCKDRAWS& v);
	bool				randomPrizeIDByCopymapID(uint32 uCopymapID, uint32 & uPrizeID);
	bool				findClimbPrizeModelByCopymapID(uint32 uCopymapID, _stClimbPrizeModel&);
	const stl_vector<uint32>* getOrderByClass(uint32 uClass);
	uint32				getNextClass(uint32 uClass);
	bool				getNextClimbTower(uint32 uCopymapID, uint32 uClass, uint32& uNextCopymapID, uint32& uNextClass);
	const _stClimbLuckDrawModel* findDrawItemByID(uint32 uDrawID);
protected:
	virtual bool		onLoad				(TiXmlElement&xmlRoot);

protected:
	virtual bool		onLoad				(CFileStream&clFile);
	virtual bool		onSave				(CFileStream&clFile);


private:

};
//-------------------------------------------------------------
extern CClimbTowerData	g_clClimbTowerData;
