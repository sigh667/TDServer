/*----------------- itemData.h
*
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ³ÂÁÁ
* Version      : V1.0
* Date         : 2016/4/11 14:44:58
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "cfg_data/fileData/fileData.h"
#include "stl/std_map.h"
#include "itemModel.h"
/*************************************************************/
class CItemData	: public CFileData
{
public:
	enum
	{
		Version	= 2016041101
	};

public:
	typedef stl_map<uint32, _stItemModel>			MAP_MODEL;
	typedef stl_map<uint32, _stItemPackModel>		MAP_PACTK_MODEL;
	typedef stl_map<uint32, _stItemCompoundModel>	MAP_COMPOUND_MODEL;

public:
	MAP_MODEL			m_mapItemModel;
	MAP_PACTK_MODEL		m_mapPackModel;
	MAP_COMPOUND_MODEL	m_mapCompoundModel;

public:
	virtual pc_str	getFileName	()const				{	return "item.dat";					}
	virtual pc_str	getXmlName	()const				{	return "cs_item.xml";				}
public:
	CItemData();

public:
	inline const _stItemModel*			find		(uint32 _itemID)	const	{	return m_mapItemModel.find_(_itemID);		}
	inline const _stItemPackModel*		findPack	(uint32 uPackID)	const	{	return m_mapPackModel.find_(uPackID);		}
	inline const _stItemCompoundModel*	findCompound(uint32 uRecipeID)	const	{	return m_mapCompoundModel.find_(uRecipeID);	}

	bool							searchIDByName(const std::string& name, stl_map<uint32, std::string> &itemFound);

public:
	virtual bool	onLoad			(TiXmlElement&xmlRoot);
	virtual bool	onLoadItem		(TiXmlElement&xmlRoot);
	virtual bool	onLoadPack		(TiXmlElement&xmlRoot);
	virtual bool	onLoadCompound	(TiXmlElement&xmlRoot);

protected:
	virtual bool	onLoad			(CFileStream&clFile);
	virtual bool	onSave			(CFileStream&clFile);

public:
	virtual void	onCompletioned	();
};
//-------------------------------------------------------------
extern CItemData	g_clItemData;
