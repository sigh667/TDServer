/*----------------- singing.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����
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
	uint32			m_uSkillID;		//����ID
	uint8			m_uSkillLevel;	//���ܵȼ�
	uint32			m_uTriggerID;	//����ID
	uint32			m_uSingTick;	//����ʱ��
	_stPoint2F		m_stTargetPoint;//Ŀ���(���ܵ�)

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
	uint64			m_uBeginTick;	//������ʼʱ��
	uint64			m_uInteTick;	//�����ж�ʱ��
	uint64			m_uEndTick;		//��������ʱ��
	uint8			m_uSingState;	//����״̬
	//bool			m_bSinging;		//�Ƿ���������
private:
	CSingObject*	m_pObject;		//��������
	CSceneUnit*		m_pOwner;		//������

public:
	//--- �Ƿ�������
	inline bool		isSinging	()const					{	return (m_uEndTick != 0);		}
	inline uint8	getSingState()						{	return m_uSingState;			}

public:
	CSinging();

public:
	void	initialize		(CSceneUnit* pOwner);
	uint32	getSingTick		()const;

public:
	//--- �����ƽ�����
	bool	process			(uint64 uTick);
	//--- ��ʼ����
	bool	beginSinging	(CSingObject*pSing);
	//--- ������������
	void	doneSinging		();
	//--- �������
	void	endSinging		();
	//--- �����������
	bool	breakSinging	(bool bInter);
private:
	//--- �����㲥
	void	sendSingBroadcast		();
	//--- ��������㲥
	void	sendSingBreakBroadcast	();
};
