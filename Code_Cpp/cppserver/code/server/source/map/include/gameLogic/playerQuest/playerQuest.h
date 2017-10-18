/*----------------- playerQuest.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 晁旭升
* Version      : V1.0
* Date         : 2016/4/13 17:35:35
*--------------------------------------------------------------
*单个任务
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "cfg_data/questData/questModel.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------
class CPlayerQuestMgr;
class PBQuest;
struct _stQuestModel;
struct _stQuestTarget;
class CPlayerQuest
{
	enum _emQuestState
	{
		QuestState_Ing,				// 进行中
		QuestState_Done,			// 任务目标已经达成 可交付
	};
	stl_map<uint32,uint32>			m_mapCounter;		//目标完成计数 目标索引==>计数
	uint32							m_uAcceptTime;		//接任务时间点

	const _stQuestModel*			m_pModel;			//模板
	CPlayerQuestMgr*				m_pQuestMgr;		//管理器
	_emQuestState					m_eState;			//当前状态

	uint32							m_uParam;	//宝藏类型

public:
	CPlayerQuest();
	~CPlayerQuest();
public:
	void	initialize				(CPlayerQuestMgr* pQuestMgr = NULL);
	bool	load					(const PBQuest& pbQuest);
	bool	save					(PBQuest& pbQuest);
	bool	build					(const _stQuestModel* pModel);
	bool	randomQuestItem			(PBQuest& pbQuest);
public:
	inline	bool					canOperate		()				{   return m_pQuestMgr != NULL && m_pModel != NULL;	}
	inline	bool					isDone			()				{   return m_eState == QuestState_Done;				}
	inline	const _stQuestModel*	getModel		()				{   return m_pModel;								}
	inline	uint32					getQuestType	()				{   return m_pModel ? m_pModel->uType : 0;			}
	inline	uint32					getParam		()				{   return m_uParam;							}
	inline	void					setParam		(uint32 uParam) {   m_uParam = uParam;							}

public:
	// 更新任务目标计数
	void	onQuestEvent			(uint8 uType,uint32 uSubType,uint32	uCount);
	// 更新任务状态
	void	updateState				();

	uint32	getCurCount				(uint32 uTarID);
	void	setCurCount				(uint32 uTarID,uint32 uCount);

	void	onComplete				();
private:
	bool	canSerialUpdate			(uint32 uIndex);

	bool	refreshTargetCounter	(const _stQuestTarget& tar,bool bBuild);

	void	triggerTargetAction		(const stl_vector<_stQuestTargetAction>& vecAction);
};
