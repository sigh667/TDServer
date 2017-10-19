/*----------------- skillAnim.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����
* Version      : V1.0
* Date         : 2016/1/27 19:58:58
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
#include "math/point2.h"
#include "cfg_data/skillData/skillModel.h"
#include "gameLogic/ghostObject/ghostObject.h"
/*************************************************************/
class CSceneUnit;
//-------------------------------------------------------------
//------------------------------ 
class CSkillAnim 
{
private:
	CSceneUnit*					m_pOwner;			//������
	const _stSkillModel*		m_pSkillModel;		//����ģ��
	const _stSkillAnimModel*	m_pSkillAnimModel;	//��ǰ����ģ��
	uint64						m_uStartAnimTick;	//��ʼ��������
	uint64						m_uEndAnimTick;		//������������
	bool						m_bBreak;			//�Ƿ��ж�

	_stAttEffect		m_stAttEffect;		//��ǰ��Ч��֡
	uint8				m_uCurEffect;		//��ǰ��Ч��

public:
	CSkillAnim();

public:
	inline	bool	canOperate	()				{   return m_pSkillModel != NULL && m_pOwner != NULL && m_pSkillAnimModel != NULL && !m_bBreak;	}
	inline	bool	isBreak		()				{   return m_bBreak;	}

public:
	void			initialize	(CSceneUnit* pOwner,uint64 uStartTick,const _stSkillModel* pSkillModel,const _stSkillAnimModel* pSkillAnimModel);
	void			start		();
	void			end			();
public:
	static bool		canChooseTo	(CSceneUnit* pOwner,CSceneUnit*pTarget,const _stSkillModel* pModel);
	static bool		gatherObject(CSceneUnit* pOwner,CSceneUnit*pTarget,const _stPoint2F&stTargetPoint,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel,MAP_GAME_UNIT&mapTarget,float32 fMoveDis);
	static bool		gatherObject(CSceneUnit* pOwner,const _stPoint2F&stTargetPoint,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel,VECTOR_RANGE_UNIT&vecRangeUnit,float32 fMoveDis);
	//--- ��ʼ�˺�
	static bool		beginDamage	(CSceneUnit* pOwner,CSceneUnit* pTarget,const _stPoint2F&stTargetPoint,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel,float32 fMoveDis = 0);
	//--- ��ʱ����
	static bool		delayDamage	(CSceneUnit* pOwner,CSceneUnit* pTarget,const _stPoint2F&stTargetPoint,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	//--- �����˺�
	static bool		realDamage	(CSceneUnit* pOwner,MAP_GAME_UNIT&mapTarget,const _stPoint2F&stTargetPoint,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	//--- ����Ч��
	static bool		skillAllEffect(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stPoint2F&stTargetPoint,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	//--- ����
	static bool		skillCure	(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stPoint2F&stTargetPoint,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	//--- ����
	static bool		skillAttack	(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stPoint2F&stTargetPoint,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	//--- �����˺�
	static bool		attackDamage(CSceneUnit* pOwner,CSceneUnit*pTarget,const _stPoint2F&stTargetPoint,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel,uint8 uAttackNum,uint8 uTargetNum);
	//--- ���ܼ�buff
	static bool		skillAddBuff(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel,const _stSkillEffectType& stSkillEffectType);
	//--- ���ܼӳ�������
	static bool		skillAddSceneObject(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stPoint2F&stTargetPoint,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	//--- ������
	static bool		skillAddExpend(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel,const _stSkillEffectType& stSkillEffectType);
	//--- �������buff
	static bool		skillClearBuff(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel,const _stSkillEffectType& stSkillEffectType);
	//--- ���ܼ�λ��
	static bool		skillAddMove(CSceneUnit* pOwner,const _stPoint2F&stTargetPoint,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel,const _stSkillEffectType& stSkillEffectType);
	//--- ����Ŀ��ʹ�ü���
	static bool		skillUseSkill(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel,const _stSkillEffectType& stSkillEffectType);
	//--- ����Ŀ��ʹ�ü���
	static bool		skillOutRingAddBuff(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel,const _stSkillEffectType& stSkillEffectType);
	//--- ����buffʱ��
	static bool		skillResetBuffTime(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel,const _stSkillEffectType& stSkillEffectType);
	//--- �����������
	static bool		skillDamageOver(CSceneUnit* pOwner,MAP_GAME_UNIT&mapObject,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel);
	
public:
	bool			process			(uint64 uTick);
	bool			processAttack	(uint64 uTick);

};
