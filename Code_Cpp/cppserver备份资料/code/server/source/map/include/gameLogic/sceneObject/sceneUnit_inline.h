/*----------------- sceneUnit_inline.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2016/1/25 15:03:47
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
/*************************************************************/
#include "cfg_data/battleData/battleData.h"
//-------------------------------------------------------------
//------------------------------ 
inline int32	CSceneUnit::getHP		()const
{
	return m_iCurrHP;
}
//-------------------------------------------------------------
//------------------------------ 
inline int32	CSceneUnit::getHPMax		()const
{
	return m_iMaxHP;
}
//-------------------------------------------------------------
//------------------------------ 
inline void	CSceneUnit::setHP(int32 iHP)
{
	if(!setInt(m_iCurrHP,iHP,getHPMax()))
		return;

	setGhost(_BIT32(Ghost_HPMP));
}

//-------------------------------------------------------------
//------------------------------ 
inline void	CSceneUnit::initAttribute()
{
	m_stGameAttributeBase.initGameAttribute();
	m_stGameAttributeBuff.initGameAttribute();
	m_stGameAttribute.initGameAttribute();
	m_mapSystemAttribute.clear();

	registerAttribute(GameSystemType_Skill);
	registerAttribute(GameSystemType_Buff);
	registerAttribute(GameSystemType_Common);
}

//-------------------------------------------------------------
//------------------------------ 获取基础属性值
inline int32	CSceneUnit::getAttribute(uint8 uType)const
{
	if(uType < _emBattleAttribute_ARRAYSIZE)
		return m_stGameAttribute.iAttribute[uType];

	return 0;
}

//-------------------------------------------------------------
//------------------------------ 
inline const _stPoint2F&	CSceneUnit::getPosition()const
{
	return m_stPosition;
}

//-------------------------------------------------------------
//------------------------------ 
inline _emMoveType	CSceneUnit::getMoveType()
{
	return m_eMoveType;
}

//-------------------------------------------------------------
//------------------------------ 
inline void	CSceneUnit::setMoveType(_emMoveType eMT)
{
	m_eMoveType = eMT;
}
//-------------------------------------------------------------
//------------------------------ 
inline uint32	CSceneUnit::getEnergy (uint8 uType)
{
	int32* pValue = m_mapEnergy.find_(uType);
	return pValue ? *pValue : 0;
}
//-------------------------------------------------------------
//------------------------------ 
inline uint8	CSceneUnit::getEnergyByExpend (uint16 uExpendType)
{
	uint8 uType = 0;
	switch (uExpendType)
	{
	case ExpendType_EnergyAnger:
		uType = BattleAttribute_EnergyAnger;
		break;
	case ExpendType_EnergyVigor:
		uType = BattleAttribute_EnergyVigor;
		break;
	case ExpendType_EnergyPower:
		uType = BattleAttribute_EnergyPower;
		break;
	case ExpendType_EnergyHit:
		uType = BattleAttribute_EnergyHit;
		break;
	case ExpendType_EnergyDark:
		uType = BattleAttribute_EnergyDark;
		break;
	case ExpendType_EnergyCrystal:
		uType = BattleAttribute_EnergyCrystal;
		break;
	case ExpendType_EnergyPsych:
		uType = BattleAttribute_EnergyPsych;
		break;
	case ExpendType_Roll:
		uType = BattleAttribute_EnergyRoll;
		break;
	default:
		break;
	}
	return uType;
}
//-------------------------------------------------------------
//------------------------------ 
inline float32	CSceneUnit::getDirection()const
{
	return m_fDirection;
}

//-------------------------------------------------------------
//------------------------------ 
void	CSceneUnit::setDirection(float32 fDirection)
{
	m_fDirection = fDirection;
}

void	CSceneUnit::setDirectionAndSync	(float32 uDir)
{
	setDirection(uDir);
	setGhost(_BIT32(Ghost_Move));
}
//-------------------------------------------------------------
//------------------------------ 
inline CSinging&	CSceneUnit::getSinging()
{
	return m_clSinging;
}
//-------------------------------------------------------------
//------------------------------ 
inline CSkilling&	CSceneUnit::getSkilling()
{
	return m_clSkilling;
}
//-------------------------------------------------------------
//------------------------------ 
inline CCooling&	CSceneUnit::getCooling()
{
	return m_clCooling;
}
//-------------------------------------------------------------
//------------------------------ 
inline CGameSkillMgr&	CSceneUnit::getGameSkillMgr()
{
	return m_clGameSkillMgr;
}
//-------------------------------------------------------------
//------------------------------ 
inline CHatredMgr&		CSceneUnit::getHatredMgr()
{
	return m_clHatredMgr;
}

//-------------------------------------------------------------
//------------------------------ 
inline CGameHurtMgr&	CSceneUnit::getHurtMgr()
{
	return m_clhurtMgr;
}

//-------------------------------------------------------------
//------------------------------ 
inline CGameBuffMgr&	CSceneUnit::getGameBuffMgr()
{
	return m_clGameBuffMgr;
}
//-------------------------------------------------------------
//------------------------------ 游戏属性-结果
inline _stGameAttribute&CSceneUnit::getGameAttribute	()
{
	return m_stGameAttribute;
}
//-------------------------------------------------------------
//------------------------------ 
inline void		CSceneUnit::setUpdateValue	(uint8 uType)
{
	if(uType < _UV_Max)
		m_bUpdateValue[uType] = true;
}
