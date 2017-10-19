/*----------------- identityData.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/9/18 17:40:54
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "cfg_data/fileData/fileData.h"
#include "identityModel.h"
/*************************************************************/
class CIdentityData	: public CFileData
{
public:
	enum
	{
		Version	= 20160722
	};

public:
	typedef stl_map<uint16,_stIdentityModel>					MAP_IDENTITY_MODEL;
	typedef stl_map<uint16,_stIdentityOpenModel>				MAP_IDENTITY_OPEN_MODEL;
	typedef stl_map<uint32,_stIdentityResetModel>				MAP_IDENTITY_RESET_MODEL;
	typedef double_map<uint32,uint16,_stIdentitySkillModel>		DMAP_IDENTITY_SKILL_MODEL;

public:
	MAP_IDENTITY_MODEL				m_mapIdentityModel;
	MAP_IDENTITY_OPEN_MODEL			m_mapIdentityOpenModel;
	MAP_IDENTITY_RESET_MODEL		m_mapIdentityResetModel;
	DMAP_IDENTITY_SKILL_MODEL		m_dmapIdentitySkillModel;

public:
	virtual pc_str			getFileName	()const				{	return "identity.dat";				}
	virtual pc_str			getXmlName	()const				{	return "cs_identity.xml";				}

public:
	CIdentityData();

public:
	inline const _stIdentityModel*		findIdentity		(uint16 uIdentityID)	const	{	return m_mapIdentityModel.find_(uIdentityID);			}
	inline const _stIdentityOpenModel*	findOpenIdentity	(uint16 uIdentityID)	const	{	return m_mapIdentityOpenModel.find_(uIdentityID);		}
	inline const _stIdentityResetModel*	findResetIdentity	(uint32 uCount)			const	
	{	
		MAP_IDENTITY_RESET_MODEL::const_iterator _pos = m_mapIdentityResetModel.lower_bound(uCount); //返回第一个大于等于ucount的行
		return _pos != m_mapIdentityResetModel.end() ? &_pos->second : NULL;		
	}
	inline const _stIdentitySkillModel*	findIdentitySkill	(uint32 uSkillID,uint16 uLevel) const {	return m_dmapIdentitySkillModel.find_(uSkillID,uLevel);	}
	const _stIdentityOpenModel*	findMainSet		()	const;

protected:
	virtual bool		onLoad				(TiXmlElement&xmlRoot);

protected:
	virtual bool		onLoad				(CFileStream&clFile);
	virtual bool		onSave				(CFileStream&clFile);

private:
	bool				onLoadIdentity		(TiXmlElement&xmlRoot);
	bool				onLoadOpenIdentity	(TiXmlElement&xmlRoot);
	bool				onLoadResetIdentity	(TiXmlElement&xmlRoot);
	bool				onLoadIdentitySkill	(TiXmlElement&xmlRoot);

public:
	virtual void	onCompletioned();
};
//-------------------------------------------------------------
extern CIdentityData	g_clIdentityData;
