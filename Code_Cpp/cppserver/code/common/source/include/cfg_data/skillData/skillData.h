/*----------------- skillData.h
*
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ³ÂÁÁ
* Version      : V1.0
* Date         : 2016/1/21 13:41:59
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "cfg_data/fileData/fileData.h"
#include "cfg_data/skillData/skillModel.h"
/*************************************************************/
class CSkillData	: public CFileData
{
public:
	enum
	{
		Version	= 2016030401
	};

public:
	typedef double_map<uint32,uint8,_stSkillModel>			DMAP_SKILLMODEL;
	typedef stl_map<uint32,_stSkillAnimModel>				MAP_ANIM_MODEL;	
	typedef double_map<uint32,uint8,_stSkillEffectModel>	DMAP_EFFECT_MODEL;

private:
	DMAP_SKILLMODEL		m_mapSkillModel;
	MAP_ANIM_MODEL		m_mapSkillAnimModel;
	DMAP_EFFECT_MODEL	m_mapSkillEffectModel;

public:
	virtual pc_str	getFileName					()const		{	return "skill.dat";						}
	virtual pc_str	getXmlName					()const		{	return "cs_skill.xml";						}
public:
	inline _stSkillModel*			findSkill		(uint32 ID,uint8 uLevel)	{	return m_mapSkillModel.find_(ID,uLevel);		}
	inline _stSkillAnimModel*		findSkillAnim	(uint32 ID)					{	return m_mapSkillAnimModel.find_(ID);			}
	inline _stSkillEffectModel*		findSkillEffect	(uint32 ID,uint8 uLevel)	{	return m_mapSkillEffectModel.find_(ID,uLevel);	}

public:
	CSkillData();

protected:
	virtual bool	onLoad			(TiXmlElement&xmlRoot);

protected:
	bool			load_skill_model	(TiXmlElement&xmlRoot);
	bool			load_effect_model	(TiXmlElement&xmlRoot);
	bool			load_anim_model		();

protected:
	virtual bool	onLoad			(CFileStream&clFile);
	virtual bool	onSave			(CFileStream&clFile);

	void				load_skill_math	(_stSkillModel* pModel,TiXmlElement* pData);
	_stSkillModel*		alloclSkill		(uint32 ID,uint8 ulevel);

	void					load_effect_math(_stSkillEffectModel* pModel,TiXmlElement* pData);
	_stSkillEffectModel*	alloclSkillEffect(uint32 ID,uint8 ulevel);
public:
	virtual void	onCompletioned	();
};
//-------------------------------------------------------------
extern 	CSkillData	g_clSkillData;

