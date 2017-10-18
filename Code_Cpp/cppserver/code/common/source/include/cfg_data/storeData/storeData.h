/*----------------- itemData.h
*
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : êËÐñÉý
* Version      : V1.0
* Date         : 2016/5/13 14:44:58
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "cfg_data/fileData/fileData.h"
#include "storeModel.h"
/*************************************************************/
class CStoreData	: public CFileData
{
public:
	enum
	{
		Version	= 2016051301
	};

public:
	typedef stl_map<uint32, _stGoodsModel>		STORECONF;
	typedef stl_map<uint32, STORECONF>			MAPSTORECONF;

private:
	MAPSTORECONF		m_mapStore;

public:
	virtual pc_str	getFileName	()const				{	return "store.dat";					}
	virtual pc_str	getXmlName	()const				{	return "cs_store.xml";				}

public:
	CStoreData();

public:
	const	_stGoodsModel*	find(uint32 _uStoreType, uint32 uGoodsIndex)	const;

public:
	virtual bool	onLoad			(TiXmlElement&xmlRoot);
	virtual	bool	onLoadStore		(TiXmlElement&xmlRoot, uint8 uStoreType, pc_str _Tag);

protected:
	virtual bool	onLoad			(CFileStream&clFile);
	virtual bool	onSave			(CFileStream&clFile);

private:
	inline	const STORECONF*	find(uint32 _uStoreType)	const	{	return m_mapStore.find_(_uStoreType);	}
};
//-------------------------------------------------------------
extern CStoreData	g_clStoreData;
