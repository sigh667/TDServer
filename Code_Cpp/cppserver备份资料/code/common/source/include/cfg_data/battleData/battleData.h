/*----------------- battleData.h
*
* Copyright (C): 2016  Mokylin°§Mokyqi
* Author       : ’‘Œƒ‘¥
* Version      : V1.0
* Date         : 2016/1/20 16:20:59
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "cfg_data//fileData/fileData.h"
#include "stl/std_map.h"
#include "cfg_data/battleData/battleModel.h"
#include "def_struct/gameStruct.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
class CBattleData	: public CFileData
{
	enum
	{
		Version	= 2016011501
	};
private:
	typedef	stl_map<uint16,_stBattleAttrModel>	MAP_MODEL;
	typedef stl_vector<_stBattleAttrModel*>		VEC_MODEL;
	MAP_MODEL	 	m_mapData;
	VEC_MODEL	 	m_vecData;
	uint8			m_uMinLayer;
	uint8			m_uMaxLayer;

public:
	virtual pc_str	getFileName	()const	{	return "battle.dat";			}
	virtual pc_str	getXmlName	()const	{	return "cs_battle_attr.xml";	}
public:
	inline	uint8	getMinLayer	()const {	return m_uMinLayer;		}
	inline	uint8	getMaxLayer	()const {	return m_uMaxLayer;		}
public:
	CBattleData();

	virtual void	onCompletioned();
public:
	const _stBattleAttrModel*	find				(uint16 uType)	const;
	int32						getAttributeRange	(uint16 uType,bool bMin) const;
	uint32						getFightPower		(const _stGameAttribute& stGameAttributeBase);

protected:
	virtual bool	onLoad		(TiXmlElement&xmlRoot);

protected:
	virtual bool	onLoad		(CFileStream&clFile);
	virtual bool	onSave		(CFileStream&clFile);
};
//-------------------------------------------------------------
extern CBattleData	g_clBattleData;