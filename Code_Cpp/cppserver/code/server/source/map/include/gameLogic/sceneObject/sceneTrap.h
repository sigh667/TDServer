/*----------------- sceneTrap.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2016/2/23 10:30:19
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "sceneUnit.h"
#include "cfg_data/trapData/trapModel.h"
/*************************************************************/
class CSceneTrap	: public CSceneUnit
{
	enum _emCurMoveState
	{
		CurMoveState_None,

		CurMoveState_Move,			// �ƶ�
		CurMoveState_Stop,			// ֹͣ
	};
	enum _emCurMoveMethod
	{
		CurMoveMethod_None,
		CurMoveMethod_Straight,		// ֱ��
		CurMoveMethod_Track,		// ׷��
		CurMoveMethod_Spiral,		// ����˳ʱ��
		CurMoveMethod_SpiralRevert,	// ������ʱ��

	};
	struct _stTrapSkill
	{
		_stTrapSkill()
		{
			initTrap();
		}
		void	initTrap()				{	memset(this,0,sizeof(*this));	}


		const _stTrapSkillModel* pModel;// ����ģ��
		uint64	uNextTrigerTick;		// ��һ�δ�������ʱ��
		uint32	uTrigerCount;			// �Ѿ������Ĵ���
	};
	typedef	stl_vector<_stTrapSkill>	VEC_TRAP_SKILL;
	typedef	stl_set	<uint32>			SET_UNIT_ID;
private:
	typedef CSceneUnit Parent;
private:
	uint64				m_uBuildTick;	// ����ʱ��
	uint64				m_uExitTick;	// ��ʧʱ���
	uint64				m_uDestroyTick;	// ����ʱ��� �����뿪����

	bool				m_bDelete;		// �Ƿ�ɾ��
	const _stTrapModel*	m_pTrapModel;	// ����ģ��

	// �������
	VEC_TRAP_SKILL		m_vecTrapSkill;	// ��ǰ����
	SET_UNIT_ID			m_setUnitID;	// �ѱ����й���Ŀ��

	// �ƶ����
	uint64				m_uLastMoveTick;// ��һ���ƶ�ʱ��
	uint64				m_uStopTick;	// ֹͣ��ʱ���
	_stPoint2F			m_stStartPoint;	// ��ʼ����
	_stPoint2F			m_stEndPoint;	// ��ֹĿ������
	_stPoint2F			m_stPrePoint;	// ��һ�ε�����
	uint64				m_uStartTick;	// ��ʼ�ƶ�ʱ��
	float32				m_fStartDir;	// ��ʼ����
	float32				m_fDistance;	// ��Զ���о���
	float32				m_fHigh;		// �߶�
	uint32				m_uTarUnitID;	// Ŀ��ID
	_emCurMoveState		m_eMoveState;	// ��ǰ�ƶ�״̬
	_emCurMoveMethod	m_eMoveMethod;	// ��ǰ�ƶ���ʽ
	uint32				m_uMoveStage;	// ���й켣�Ľ׶�
	bool				m_bNoUsekill;	// �Ƿ���ʹ�ü���
public:
	CSceneTrap();
	virtual ~CSceneTrap();
public:
	virtual bool		initialize	();

public:
	virtual CSceneTrap*	asTrap			()	{	return this;	}
	virtual	bool		packetGhost		(uint64 uTime,bool bSelf,uint32& updateMark,PBGhostInfo&pbInfo);

	virtual void		processLogic	(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay);

	virtual	bool		canUseSkill		(CSceneUnit*pTarget);
	virtual	bool		canByUseSkill	(CSceneUnit*pAtker);
	virtual bool		isVisible		(CSceneUnit*pObject);	// Ŀ����Լ��Ƿ�ɼ� 

	virtual void		callbackAttack	(CSceneUnit* pUnit,int32 iDamage,bool bCritical);
	virtual void		callbackSkillHitTarget	(CSceneUnit* pUnit,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	virtual void		callbackSkillHit(const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	virtual void		process_move	(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay);
public:
	/*--->[ ���� ]*/
	bool				build			(CSceneUnit* pOwner,const _stTrapModel*pModel,const _stPoint2F&stBuildPos,float32 fDir,float32 fHigh,_stPoint2F*const pTargetPos = NULL,uint32 uTarUnitID = 0);
private:
	void				processMove		(uint64 uTick,uint64 uTime,uint32 uSecond);

	void				processMove_Move(uint64 uTick,uint64 uTime,uint32 uSecond);
	void				processMove_Stop(uint64 uTick,uint64 uTime,uint32 uSecond);
	float32				getFlyDistance	();
	void				onExit			();
	void				destroy			();


public:
	_stPoint2F			getEndPoint()	{return m_stEndPoint;}
};