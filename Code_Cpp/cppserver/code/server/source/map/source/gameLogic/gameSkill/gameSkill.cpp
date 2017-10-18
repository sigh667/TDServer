/*----------------- gameSkill.cpp
*
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ³ÂÁÁ
* Version      : V1.0
* Date         : 2016/1/20 17:31:47
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "gameLogic/gameSkill/gameSkill.h"
//-------------------------------------------------------------
//------------------------------ 
CGameSkill::CGameSkill()
{
}

//-------------------------------------------------------------
//------------------------------ 
CGameSkill::~CGameSkill()
{
}

//-------------------------------------------------------------
//------------------------------ 
void CGameSkill::initialize(const _stSkillModel* pModel,bool bTmp)
{
	m_bNormal		= false;
	m_pSkillModel	= pModel;
	m_pOwner		= NULL;
	m_bTmp			= bTmp;
}

//-------------------------------------------------------------
//------------------------------ 
bool CGameSkill::upgrade(const _stSkillModel* pModel)
{
	m_pSkillModel = pModel;
	return true;
}

