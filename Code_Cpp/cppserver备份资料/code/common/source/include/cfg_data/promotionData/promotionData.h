/*------------- promotionData.h
* Copyright (C): 2016  Mokylin°§Mokyqi
* Author       : ¡÷æ‘
* Version      : V1.01
* Date         : 2016/09/20
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "cfg_data/fileData/fileData.h"
#include "promotionModel.h"
//-------------------------------------------------------------
//------------------------------ 
class CPromotionData	: public CFileData
{
protected:
	enum
	{
		Version		= 20160920
	};

public:
	typedef double_map<uint8,uint8,_stPromotionModel>	MAP_MODEL;		// job step model

private:
	MAP_MODEL	m_mapModel;
	MAP_U8_U8	m_mapJopMaxStep;

public:
	CPromotionData();
	virtual pc_str	getFileName	()const				{	return "promotion.dat";		}
	virtual pc_str	getXmlName	()const				{	return "cs_promotion.xml";	}

public:
	inline const _stPromotionModel*	find			(uint8 uJob, uint32 uStep)	{	return m_mapModel.find_(uJob, uStep);	}
	const		_stPromotionModel*	findByLevel		(uint8 uJob, uint32 level);
	uint8							maxStep(uint8 uJob);

protected:
	virtual bool	onLoad		(TiXmlElement&xmlRoot);
	virtual bool	onLoad		(CFileStream&clFile);
	virtual bool	onSave		(CFileStream&clFile);
	virtual void	onCompletioned();
};
//-------------------------------------------------------------
extern 	CPromotionData	g_clPromotionData;
