/*----------------- gameBuff.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/1/26 14:49:18
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stream/bufferStream.h"
#include "def_struct/gameStruct.h"
#include "cfg_data/buffData/buffData.h"
/*************************************************************/
class CSceneUnit;
class CScenePlayer;
class CGameBuff
{
private:
	const _stBuffModel*		m_pBuffModel;		//buff模板
	CSceneUnit*				m_pOwner;			//所有者
	uint32					m_uSourceID;		//施加者ID
	uint64					m_uBeginTick;		//开始时间
	uint64					m_uContinueTick;	//持续时间 0表示无限时间
	uint64					m_uNextTick;		//下次执行时间

	uint8					m_uLayer;			//当前层数
	bool					m_bDelete;			//是否删除
public:
	CGameBuff();
	~CGameBuff();

public:
	inline	const _stBuffModel*	getBuffModel()	const	{   return m_pBuffModel;								}
	inline	bool				canOperate	()			{   return m_pOwner != NULL && m_pBuffModel != NULL;	}
	inline	uint32				getBuffID	()			{   return m_pBuffModel ? m_pBuffModel->uBuffID : 0;	}
	inline	uint32				getGroupID	()			{   return m_pBuffModel ? m_pBuffModel->uGroupID : 0;	}
	inline	bool				isDelete	()			{   return m_bDelete;									}
	inline	uint8				getLevel	()			{   return m_pBuffModel ? m_pBuffModel->uLevel : 0;		}
	inline	uint64				getContinueTick	()		{   return m_uContinueTick;								}
	inline	uint32				getSourceID()			{   return m_uSourceID;								}

public:
	uint64	getLeftTick		();
	void	setDelete		(bool bSet = true);
	void	onDelete		();
	
public:
	uint32	getExtraDamage	(int32 iDamage);

public:
	void	initialize		(uint32 uSourceID = 0, CSceneUnit* pOwner = NULL, const _stBuffModel* pModel = NULL, uint64 uLeftTick = 0);
	void	start			(uint32 uSourceID,uint64 uLeftTick,int8 nAddLayer = 0);
	void	repeat			(const _stBuffModel* pModel,uint64 uLeftTick);
	void	act				();
	void	onUseSkill		(uint32 uSkillID);
	void	beSkillHit		(CSceneUnit* pUnit,const _stSkillModel*pSkillModel);
	void	addLayer		(uint8 uAddNum = 1);
	bool	addMove			(_stPoint2F& stTarget,float32 fSkillDir);
	bool	updateLeftTick	(uint64 uContinueTick,bool bSend);
	void	calcAttr		();
public:
	bool	process			(uint64 uTick,uint64 uTime,uint32 uSecond);
	bool	processContinue	(uint64 uTick,uint64 uTime,uint32 uSecond);


};
