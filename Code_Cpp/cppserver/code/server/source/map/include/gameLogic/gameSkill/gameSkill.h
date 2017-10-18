/*----------------- gameSkill.h
*
* Copyright (C): 2016  Mokylin��Mokyqi
* Author       : ����
* Version      : V1.0
* Date         : 2016/1/20 17:31:16
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "math/point2.h"
#include "math/rect.h"
#include "gameLogic/singing/singing.h"
#include "cfg_data/skillData/skillModel.h"
/*************************************************************/
class CSceneUnit;
class CGameSkill
{
private:
	bool					m_bNormal;			//�Ƿ��������
	const _stSkillModel*	m_pSkillModel;		//����ģ��
	CSceneUnit*				m_pOwner;			//������
	bool					m_bTmp;				//��ʱ����

public:
	CGameSkill();
	~CGameSkill();

public:
	inline	bool		canOperate		()				{   return m_pSkillModel != NULL && m_pOwner != NULL;	}
	inline	uint8		getLevel		()				{   return m_pSkillModel ? m_pSkillModel->uLevel : 0;	}
	inline	uint32		getSkillID		()				{   return m_pSkillModel ? m_pSkillModel->uSkillID : 0;	}
	inline	bool		isTempSkill		()				{   return m_bTmp;										}

	inline	const _stSkillModel*	getSkillModel()		{   return m_pSkillModel;								}

public:
	void	initialize		(const _stSkillModel* pModel = NULL, bool bTmp = false);
	bool	upgrade			(const _stSkillModel* pModel);
};

