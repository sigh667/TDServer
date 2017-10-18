/*------------- factionData.h
* Copyright (C): 2016  Mokylin°§Mokyqi
* Author       : ¡÷æ‘
* Version      : V1.01
* Date         : 2016/10/11
*
*/ 
/***************************************************************
* 
***************************************************************/
#include "cfg_data/fileData/fileData.h"
#include "factionModel.h"
//-------------------------------------------------------------
//------------------------------ 
class CFactionData	: public CFileData
{
protected:
	enum
	{
		Version		= 20160223
	};

public:
	typedef stl_map<uint64, _stFactionSkillModel>				MAP_Skill_MODEL;
	typedef stl_set<uint32>										SET_SkillID;
	typedef stl_map<uint64, _stFactionDonateSliverModel>		MAP_DonateSliver_MODEL;
	typedef stl_map<uint32, _stFactionTaskModel>				MAP_Task_MODEL;
	typedef stl_map<uint32, _stFactionLevelModel>				MAP_Level_MODEL;
	typedef stl_map<uint32, _stFactionJobModel>					MAP_Job_MODEL;
	typedef stl_map<uint64, _stFactionDrinkWineModel>			MAP_DrinkWine_MODEL;
public:
	MAP_Skill_MODEL			m_mapSkillModel;
	SET_SkillID				m_setSkillID;
	MAP_DonateSliver_MODEL	m_mapDonateSliverModel;
	MAP_Task_MODEL			m_mapTaskModel;
	MAP_Level_MODEL			m_mapLevelModel;
	MAP_Job_MODEL			m_mapJobModel;
	MAP_DrinkWine_MODEL		m_mapDrinkWineModel;
public:
	_stFactionDrinkWineConstantModel	m_clDrinkWineConstantModel;

public:
	CFactionData();
	virtual pc_str	getFileName	()const				{	return "faction.dat";						}
	virtual pc_str	getXmlName	()const				{	return "cs_faction.xml";					}

public:
	const _stFactionSkillModel*			findFactionSkillModel		(uint32 uSkllID,uint32 uLevel)const;
	const _stFactionDonateSliverModel*	findFactionDonateSliverModel(uint32 uID)const						{	return m_mapDonateSliverModel.find_(uID);		}
	const _stFactionDrinkWineModel*		findFactionDrinkWineModel	(uint32 uID)const						{	return m_mapDrinkWineModel.find_(uID);			}
	const _stFactionTaskModel*			findFactionTaskModel		(uint32 uID)const						{	return m_mapTaskModel.find_(uID);				}
	const _stFactionLevelModel*			findFactionLevelModel		(uint32 uLevel)const					{	return m_mapLevelModel.find_(uLevel);			}
	const _stFactionJobModel*			findFactionJobModel			(uint32 uJob)const						{	return m_mapJobModel.find_(uJob);				}
	const _stFactionDrinkWineConstantModel&		getDrinkWineConstant()const									{	return m_clDrinkWineConstantModel;				}

public:
	const _stFactionTaskModel*			randGetFactionTaskModel		(uint32 uFactionLevel)const;
protected:
	virtual bool	onLoad					(TiXmlElement&xmlRoot);
	virtual bool	load_skill				(TiXmlElement&xmlRoot);
	virtual bool	load_donate_sliver		(TiXmlElement&xmlRoot);
	virtual bool	load_drink_wine			(TiXmlElement&xmlRoot);
	virtual bool	load_task				(TiXmlElement&xmlRoot);
	virtual bool	load_level				(TiXmlElement&xmlRoot);
	virtual bool	load_job				(TiXmlElement&xmlRoot);

protected:
	virtual bool	onLoad		(CFileStream&clFile);
	virtual bool	onSave		(CFileStream&clFile);
public:
	virtual	void		onCompletioned	();
};
//-------------------------------------------------------------
extern 	CFactionData	g_clFactionData;
