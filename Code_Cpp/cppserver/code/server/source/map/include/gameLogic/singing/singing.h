/*----------------- singing.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/1/21 18:55:03
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
#include "math/point2.h"
/*************************************************************/
class CSceneUnit;
//-------------------------------------------------------------
//------------------------------ 
class CSingObject
{
private:
	uint32			m_uSkillID;		//技能ID
	uint8			m_uSkillLevel;	//技能等级
	uint32			m_uTriggerID;	//触发ID
	uint32			m_uSingTick;	//吟唱时间
	_stPoint2F		m_stTargetPoint;//目标点(技能点)

public:
	CSingObject();
	virtual~CSingObject(){}

public:
	friend class CSinging;

public:
	inline uint32		getSkillID			()	{	return m_uSkillID;		}
	inline uint8		getSkillLevel		()	{	return m_uSkillLevel;		}
	inline uint32		getSingTriggerID	()	{	return m_uTriggerID;	}
	inline uint32		getSingTick			()	{	return m_uSingTick;		}
	inline _stPoint2F&	getTargetPoint		()	{	return m_stTargetPoint;	}
	inline void			setTargetPoint		(_stPoint2F& stTargetPoint)	{ m_stTargetPoint = stTargetPoint;	}

protected:
	virtual void	initSingObject	(uint32 uSkillID,uint8 uSkillLevel,uint32 uTargetID,uint32 uTick,const _stPoint2F*pPoint);
	//virtual uint32	getSingObjectID	()	= 0;
	virtual void	onDoneSinging	(uint32 uSingingTick)	= 0;
};
//-------------------------------------------------------------
//------------------------------ 
class CSinging
{
protected:
	uint64			m_uBeginTick;	//吟唱开始时间
	uint64			m_uInteTick;	//吟唱中断时间
	uint64			m_uEndTick;		//吟唱结束时间
	uint8			m_uSingState;	//吟唱状态
	//bool			m_bSinging;		//是否吟唱技能
private:
	CSingObject*	m_pObject;		//吟唱对象
	CSceneUnit*		m_pOwner;		//归属者

public:
	//--- 是否吟唱中
	inline bool		isSinging	()const					{	return (m_uEndTick != 0);		}
	inline uint8	getSingState()						{	return m_uSingState;			}

public:
	CSinging();

public:
	void	initialize		(CSceneUnit* pOwner);
	uint32	getSingTick		()const;

public:
	//--- 吟唱推进处理
	bool	process			(uint64 uTick);
	//--- 开始吟唱
	bool	beginSinging	(CSingObject*pSing);
	//--- 吟唱正常结束
	void	doneSinging		();
	//--- 完成吟唱
	void	endSinging		();
	//--- 打断蓄力吟唱
	bool	breakSinging	(bool bInter);
private:
	//--- 吟唱广播
	void	sendSingBroadcast		();
	//--- 打断吟唱广播
	void	sendSingBreakBroadcast	();
};
