/*----------------- skilling.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����
* Version      : V1.0
* Date         : 2016/1/26 9:54:19
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
#include "math/point2.h"
#include "gameLogic/singing/singing.h"
#include "cfg_data/skillData/skillModel.h"
#include "gameLogic/skilling/skillAnim.h"
/*************************************************************/
class CSceneUnit;
//-------------------------------------------------------------
//------------------------------ 
class CSkilling : public CSingObject
{
private:
	struct _stCompositeInfo
	{
		uint32	m_uNextSkill;	//��һ������
		uint32	m_uBaseSkill;	//��������
		uint64	m_uExpireTick;	//�ȴ�����ʱ��

		_stCompositeInfo() {	memset(this,0,sizeof(*this)); }
	};
	typedef stl_map<uint32,_stCompositeInfo> MAP_COMPOSITEINFO;

private:
	CSceneUnit*			m_pOwner;			//������
	const _stSkillModel*m_pSkillModel;		//����ģ��
	_stSkillAnimModel*	m_pSkillAnimModel;	//����ģ��

	uint8				m_uCurStage;
	uint64				m_uStartAnimTick;	//��ʼ��������
	uint64				m_uEndAnimTick;		//������������
	CSkillAnim			m_clSkillAnim;		//����֡����	
	uint8				m_uExpendCount;		//����������

	float32				m_fChargeRate;		//����ϵ��
	_stPoint2F			m_stOldPosition;	//λ��ǰ�ĵط� ˲�Ƹ��Ӽ���ר��

	uint64				m_uLastCombatTick;  //��һ��������ʱ��
	uint32				m_uNextCombatSkill;	//��һ����������ID

	MAP_COMPOSITEINFO	m_mapChildSkill;	//���Ӽ���

	uint32				m_uTarUnitID;		//��Щ������Ҫȡʵʱ����
	SET_U32				m_setChooseTarget;	//�ѱ�ѡ��Ŀ��
	SET_U32				m_setGatherTarget;	//�Ѽ�ѡ��Ŀ��

public:
	CSkilling();

public:
	inline	bool					canOperate		()		{   return m_pSkillModel != NULL && m_pOwner != NULL;	}
	inline	float32					getChargeRate	()		{   return m_fChargeRate;	}
	inline	const _stSkillModel*	getSkillModel	()const	{   return m_pSkillModel;	}
	inline	bool					haveChoose		(uint32 uUnitID)const	{   return m_setChooseTarget.is_exist(uUnitID);	}
	inline	void					addChooseTarget	(uint32 uUnitID)	{   m_setChooseTarget.insert(uUnitID);				}
	inline	void					addGatherTarget	(uint32 uUnitID)	{   m_setGatherTarget.insert(uUnitID);				}
	
public:
	void				initialize				(CSceneUnit* pOwner, const _stSkillModel*	pSkillModel);
	void				beginTime				();
	uint32				getSkillTimeByType		(const _stSkillModel* pSkillModel,_emSkillTimeType emType);
	uint32				getCurentTargetUnitId	()	{return m_uTarUnitID;}
	_stCompositeInfo*	getChildSkill			(uint32 uBaseSkillID)	{ return m_mapChildSkill.find_(uBaseSkillID);	}

	void				callbackKillTarget		(CSceneUnit* pTarget);
public:
	virtual void	onDoneSinging	(uint32 uSingingTick);
	void			onDoneSkill		(bool bBreak = false);
private:
	bool			canUse			(CSceneUnit* pOwner, const _stSkillModel* pSkillModel,uint8& uCode);
	bool			useConsume		(bool bSinging,uint8& uCode);
	bool	        useColletionSkill(uint8& nCode);
	bool	        arrestSkill		(uint8& nCode);
	bool	        activeSkill		(uint8& nCode);
	bool	        useRewardSkill	(uint8& nCode);
	bool			useSkillInnerAsk(CSceneUnit* m_pOwner, const _stSkillModel* pSkillModel,uint32 uTarUnitID,_stPoint2F&stTargetPoint,uint8& uCode);

private:
	bool			canSinging		(uint32 uTarUnitID,uint8& uCode);
	bool			singing			(uint32 uTarUnitID,_stPoint2F&stTargetPoint,uint8& uCode);
	bool			readyUseSkill	(uint32 uTarUnitID,uint8& uCode);

public:
	bool			process			(uint64 uTick);
	bool			processEndAnim	(uint64 uTick);
	void			processExpend	(uint64 uTick);

public:
	//--- ʹ�ü�������
	bool			useSkillAsk		(CSceneUnit* m_pOwner, const _stSkillModel* pSkillModel,uint32 uTarUnitID,_stPoint2F&stTargetPoint,uint8& uCode);
	//--- ��ϼ�������
	bool			breakSkillAsk	(uint32 uSkillID,bool bForce,uint8& uCode);
	//--- �ͷ�������������
	bool			useChargeAsk	(uint32 uSkillID,uint8& uCode);
	//--- ʹ���޶������� ����˲��
	static bool		useNoAnimSkill	(CSceneUnit* m_pOwner, const _stSkillModel* pSkillModel,uint32 uTarUnitID,_stPoint2F&stTargetPoint);

	

};
