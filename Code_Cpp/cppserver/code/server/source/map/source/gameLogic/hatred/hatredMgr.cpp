/*------------- hatredMgr.cpp
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈腾
* Version      : V1.01
* Date         : 2016/02/26
*
*/ 
/***************************************************************
* 
***************************************************************/
#include "gameLogic/hatred/hatredMgr.h"
#include "stl/stl_vector.h"
#include <algorithm>
#include "gameLogic/sceneObject/sceneUnit.h"
#include "gameScene/gameSceneCell.h"
#include "gameLogic/sceneObject/sceneMonster.h"
#include "cfg_data/monsterData/monsterModel.h"
#include "cfg_data/constantData/constantData.h"
#include "cfg_data/campData/campData.h"
//#include "gameLogic/playerQuest/playerQuestMgr.h"
/*************************************************************/

static bool _sort_hatred_(CHatredMgr::_stHatred* left,CHatredMgr::_stHatred* right)
{
	if (!left || !right )
		return false;

	if (left->nLevel != right->nLevel)
	{
		return left->nLevel > right->nLevel;
	}

	if (left->nHatred != right->nHatred )
		return left->nHatred > right->nHatred;

	return left > right;
}
//-------------------------------------------------------------
//------------------------------ 
CHatredMgr::CHatredMgr()
{
	initialize(NULL);
}

//-------------------------------------------------------------
//------------------------------ 
CHatredMgr::~CHatredMgr()
{

}

//-------------------------------------------------------------
//------------------------------ 
void CHatredMgr::initialize(CSceneUnit* pUnit)
{
	m_pOwner			= pUnit;
	m_pFirstHatred		= NULL;
	m_mapHatred.clear();
	m_setBeHatred.clear();
}
//-------------------------------------------------------------
//------------------------------ 
bool	CHatredMgr::canHatred				(CSceneUnit* pUnit)
{
	if ( !m_pOwner || !pUnit  )
		return false;

	if (!m_pOwner->getAI())
		return false;

	// 只有怪物才有仇恨
	if (!m_pOwner->asMonster())
		return false;

	if ( !m_pOwner->isVisible(pUnit) || m_pOwner->isDead()  || pUnit->isDead() )
		return false;

	//敌对阵营才加 仇恨
	if (TargetCategory_Enemy != g_clCampData.findCamp(m_pOwner->getCampID(),pUnit->getCampID()))
		return false;

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
void CHatredMgr::updateHatred(CSceneUnit* pUnit, int32 nHatred, bool bCall)
{
	//隐含的规则   
	//pUnit 应该是玩家，或者是npc
	//m_pOwner 应该是怪物,或者是npc。
	if ( !m_pOwner || !pUnit || !nHatred || m_pOwner == pUnit )
		return;

	//陷阱处理
	if(pUnit->asTrap())
	{
		pUnit = pUnit->asTrap()->getOwner();
		if (!pUnit)
			return;
	}

	if ( !canHatred(pUnit) )
		return ;
	
	// 进入战斗求援
	/*if (bCall && m_mapHatred.empty() && nHatred > 0)
	{ 
		CGameSceneCell* pCell = m_pOwner->getSceneCell();
		CSceneMonster*	pMonster = m_pOwner->asMonster();
		if (pMonster && pCell && pMonster->getCallDist() > 0.01)
		{
			static MAP_GAME_UNIT	mapObject;
			mapObject.clear();
			m_pOwner->gatherSceneObject(m_pOwner->getPosition(), pMonster->getCallDist(), mapObject, g_uBattleObject);

			MAP_GAME_UNIT::iterator _pos = mapObject.begin();
			MAP_GAME_UNIT::iterator _end = mapObject.end();
			for(;_pos != _end;++_pos)
			{
				CSceneUnit* pTemp = _pos->second;
				if (!pTemp || pTemp == m_pOwner || pTemp->isFighting())
					continue;

				if (pTemp->getRelation(m_pOwner) != TargetCategory_Friend)
					continue;

				CHatredMgr& stHatredMgr = pTemp->getHatredMgr();
				if (!stHatredMgr.hasHatred())
					stHatredMgr.updateHatred(pUnit, 1, false);
			}
		}
	}*/

	_stHatred* pHatred	= m_mapHatred.find(pUnit->getUnitID());
	if (pHatred)
	{
		pHatred->nHatred += nHatred;
		if ( pHatred->nHatred <= 0 )
		{
			deleteHatred(pUnit);
			return ;
		}

		if ( pHatred == m_pFirstHatred )
		{
			if ( nHatred < 0 )
				updateFirstHatred();
		}
		else
		{
			if ( !m_pFirstHatred || _sort_hatred_(pHatred, m_pFirstHatred) )
			{
				m_pFirstHatred = pHatred;
			}
		}
	}
	else
	{
		if ( nHatred <= 0 )
			return ;

		pHatred = newHatred(pUnit,nHatred);
		if (!pHatred)
			return ;

		if ( !m_pFirstHatred || _sort_hatred_(pHatred, m_pFirstHatred) )
		{
			m_pFirstHatred = pHatred;
		}
	}
}
//-------------------------------------------------------------
//------------------------------ 
CHatredMgr::_stHatred*	CHatredMgr::newHatred				(CSceneUnit* pUnit, int32 nHatred)
{
	if (!m_pOwner || !pUnit)
		return NULL;

	_stHatred* pHatred = m_mapHatred.allocate(pUnit->getUnitID());
	if (!pHatred)
		return NULL;
	pHatred->uUnitID	= pUnit->getUnitID();
	pHatred->nHatred	= nHatred;
	pUnit->getHatredMgr().m_setBeHatred.insert(m_pOwner->getUnitID());
	return pHatred;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CHatredMgr::isHatred				(uint32 uUnitID)
{
	_stHatred* pHatred = m_mapHatred.find(uUnitID);
	return pHatred != NULL;
}
//-------------------------------------------------------------
//------------------------------ 
void	CHatredMgr::updateFirstHatred		()
{
	m_pFirstHatred = NULL;

	MAP_HATRED::iterator _pos;
	m_mapHatred.getHead(_pos);
	while ( !m_mapHatred.isTail(_pos))
	{
		_stHatred* pHatred = m_mapHatred.getNext(_pos);
		if (!pHatred)
			continue;

		if ( !m_pFirstHatred || _sort_hatred_(pHatred, m_pFirstHatred))
			m_pFirstHatred = pHatred;
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CHatredMgr::deleteHatred(CSceneUnit* pUnit)
{
	if (!m_pOwner || !pUnit)
		return;

	_stHatred* pHatred = m_mapHatred.find(pUnit->getUnitID());
	if (!pHatred)
		return ;

	m_mapHatred.release(pUnit->getUnitID());
	pUnit->getHatredMgr().m_setBeHatred.erase(m_pOwner->getUnitID());

	if ( pHatred == m_pFirstHatred )
	{
		updateFirstHatred();
	}
}
//-------------------------------------------------------------
//------------------------------ 
uint32	CHatredMgr::getFirstHatredID()
{
	return m_pFirstHatred?m_pFirstHatred->uUnitID:0;
}
//-------------------------------------------------------------
//------------------------------ 
uint32	CHatredMgr::getOrderHatredID		(uint32 uOrder)
{
	if ( uOrder <= 1 || m_mapHatred.size() <= 1 )
		return getFirstHatredID();

	static stl_vector<_stHatred*> vecHatred;
	vecHatred.clear();
	MAP_HATRED::iterator _pos;
	m_mapHatred.getHead(_pos);
	while ( !m_mapHatred.isTail(_pos))
	{
		_stHatred* pHatred = m_mapHatred.getNext(_pos);
		if (!pHatred)
			continue;

		vecHatred.push_back(pHatred);
	}

	if (vecHatred.empty())
		return 0;

	std::sort(vecHatred.begin(),vecHatred.end(),_sort_hatred_);

	int32 nIndex = uOrder - 1;
	if ( nIndex >= (int32)vecHatred.size()  )
		nIndex = vecHatred.size()-1;

	return vecHatred[nIndex]->uUnitID;
}
//-------------------------------------------------------------
//------------------------------ 
uint32 CHatredMgr::getRandomHatredID	()
{
	if (!m_pOwner || m_mapHatred.empty() )
		return 0;

	uint32 uIndex = randRange((uint32)m_mapHatred.size()-1,uint32(0));
	MAP_HATRED::iterator _pos;
	m_mapHatred.getHead(_pos);
	for ( uint32 nCurIndex = 0; !m_mapHatred.isTail(_pos) ; ++nCurIndex)
	{
		_stHatred* pHatred = m_mapHatred.getNext(_pos);
		if (!pHatred)
			continue;
		if (nCurIndex == uIndex )
			return pHatred->uUnitID;
	}
	return 0;
}
//-------------------------------------------------------------
//------------------------------ 
void CHatredMgr::clearHatred()
{
	m_pFirstHatred = NULL;
	if (!m_pOwner || m_mapHatred.empty())
		return;

	uint32 uOwnerID			= m_pOwner->getUnitID();
	CGameSceneCell* pCell	= m_pOwner->getSceneCell();
	if (pCell)
	{
		auto _pos	= m_mapHatred.begin();
		auto _end	= m_mapHatred.end();
		for (; _pos != _end; ++_pos)
		{
			CSceneUnit* pUnit = pCell->findObject(_pos->first);
			if (pUnit)
				pUnit->getHatredMgr().m_setBeHatred.erase(uOwnerID);
		}
	}
	m_mapHatred.clear();
}

//-------------------------------------------------------------
//------------------------------ 
void CHatredMgr::clearBeHatred()
{
	if (!m_pOwner || m_setBeHatred.empty())
		return;

	//uint32 uOwnerID			= m_pOwner->getUnitID();
	CGameSceneCell* pCell	= m_pOwner->getSceneCell();
	if (pCell) 
	{
		auto _pos	= m_setBeHatred.begin();
		while  ( _pos != m_setBeHatred.end() )
		{
			auto _itr = _pos++;
			CSceneUnit* pUnit = pCell->findObject(*_itr);
			if (pUnit)
				pUnit->getHatredMgr().deleteHatred(m_pOwner);
		}
	}
	m_setBeHatred.clear();
}

//-------------------------------------------------------------
//------------------------------ 
void CHatredMgr::onDead()
{
	clear();
}

//-------------------------------------------------------------
//------------------------------ 
void CHatredMgr::onLeaveScene()
{
	clear();
}
//-------------------------------------------------------------
//------------------------------ 
void	CHatredMgr::onRefreshVisible		()
{
	if ( !m_pOwner )
		return ;

	CGameSceneCell* pCell	= m_pOwner->getSceneCell();
	if (!pCell) 
		return ;

	for ( auto _pos	= m_setBeHatred.begin() ; _pos != m_setBeHatred.end(); )
	{
		auto _itr = _pos++;
		CSceneUnit* pUnit = pCell->findObject(*_itr);
		if (pUnit && !pUnit->isVisible(m_pOwner) )
			pUnit->getHatredMgr().deleteHatred(m_pOwner);
	}
}
//-------------------------------------------------------------
//------------------------------ 
void CHatredMgr::clear()
{
	if (!m_pOwner)
		return;
	//怪物，npc执行，
	clearHatred();
	//玩家，npc执行
	clearBeHatred();
}
//-------------------------------------------------------------
//------------------------------ 
void	CHatredMgr::onByDamage_Skill			(CSceneUnit* pSource, int32 nHP,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel)
{
	if ( !m_pOwner || !pSource )
		return ;

	if ( nHP < 0 || !pSkillModel || !pSkillEffectModel )
		return ;

	int32 nHatred = nHP + pSkillEffectModel->nHatredValue;
	updateHatred(pSource, MAX(nHatred,1));
}
//-------------------------------------------------------------
//------------------------------ 
void	CHatredMgr::onByDamage_Buff			(CSceneUnit* pSource, int32 nHP)
{
	if ( !m_pOwner || !pSource )
		return ;

	if ( nHP <= 0 )
		return ;

	int32 nRate = g_clConstantData.Game_HatredBuffHurt;
	if (!nRate)
		return ;

	int32 nHatred = nHP * nRate/10000;
	updateHatred(pSource, MAX(nHatred,1));
}
//-------------------------------------------------------------
//------------------------------ 
void	CHatredMgr::onByCure_Skill			(CSceneUnit* pSource, int32 nHP,const _stSkillModel* pSkillModel,const _stSkillEffectModel*pSkillEffectModel)
{
	if ( !m_pOwner || !pSource )
		return ;

	if ( nHP <= 0 || !pSkillModel || !pSkillEffectModel )
		return ;

	CGameSceneCell* pCell	= m_pOwner->getSceneCell();
	if (!pCell)
		return ;

	int32 nHatred = pSkillEffectModel->nHatredValue;
	//只有玩家被治疗，治疗者添加到被治疗者的仇恨列表里面去。
	int32 nRate = g_clConstantData.Game_HatredSkillCure;
	if (nRate)
	{
		nHatred += nHP * nRate/10000;
	}

	for ( auto _pos = m_setBeHatred.begin(); _pos != m_setBeHatred.end(); ++_pos )
	{
		CSceneUnit* pUnit1 = pCell->findObject(*_pos);
		if (!pUnit1)
			continue;

		pUnit1->getHatredMgr().updateHatred(pSource, MAX(nHatred,1) );
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CHatredMgr::onByCure_Buff			(CSceneUnit* pSource, int32 nHP)
{
	if ( !m_pOwner || !pSource )
		return ;

	int32 nRate = g_clConstantData.Game_HatredBuffCure;
	if (!nRate)
		return ;

	CGameSceneCell* pCell	= m_pOwner->getSceneCell();
	if (!pCell)
		return ;

	int32 nHatred = nHP * nRate/10000;
	for ( auto _pos = m_setBeHatred.begin(); _pos != m_setBeHatred.end(); ++_pos )
	{
		CSceneUnit* pUnit1 = pCell->findObject(*_pos);
		if (!pUnit1)
			continue;

		pUnit1->getHatredMgr().updateHatred(pSource, MAX(nHatred,1) );
	}
}
//-------------------------------------------------------------
//------------------------------
void	CHatredMgr::onEnterFightDis			(CSceneUnit* pSource)
{
	if ( !m_pOwner || !pSource )
		return ;

	if (m_pOwner->getRelation(pSource) != TargetCategory_Enemy )
		return ;

	CSceneMonster* pMonster = m_pOwner->asMonster();
	if (!pMonster )
		return ;

	const _stMonsterModel* pMonsterModel = pMonster->getMonsterModel();
	if (!pMonsterModel)
		return ;

	switch (pMonsterModel->uHatredType)
	{
	case HatredType_Proactive:
		{

		}
		break;
	case HatredType_Task:
		{
			CScenePlayer* pPlayer = pSource->getOwnerPlayer();
			if (pPlayer )
			{
				//CPlayerQuestMgr* pQuestMgr = pPlayer->getPlayerQuestMgr();
				//if (!pQuestMgr) 
				//	return ;

				//if ( !pQuestMgr->haveQuestEvent(QuestTargetType_KillMonster, pMonsterModel->uMonsterID)  && !pQuestMgr->haveQuestEvent(QuestTargetType_MonsterDropGoods, pMonsterModel->uMonsterID) )
				//	return ;
			}
		}
		break;
	default:
		return ;
	}
	_stHatred* pHatred	= m_mapHatred.find(pSource->getUnitID());
	if (!pHatred)
	{
		updateHatred(pSource,1);
	}
}
//-------------------------------------------------------------
//------------------------------
bool	CHatredMgr::hasHatred		()
{		
	return !m_mapHatred.empty();	
}
//-------------------------------------------------------------
//------------------------------
bool	CHatredMgr::hasBeHatred		()
{
	return !m_setBeHatred.empty();
}
//-------------------------------------------------------------
//------------------------------
bool	CHatredMgr::getHatredPlayer	(stl_set<CScenePlayer*>& setPlayer)
{
	if (!m_pOwner)
		return false;

	MAP_HATRED::iterator _pos;
	m_mapHatred.getHead(_pos);
	while ( !m_mapHatred.isTail(_pos))
	{
		_stHatred* pHatred = m_mapHatred.getNext(_pos);
		if (!pHatred)
			continue;

		CSceneUnit* pUnit = m_pOwner->findCellObject(pHatred->uUnitID);
		if (pUnit && pUnit->asPlayer())
			setPlayer.insert(pUnit->asPlayer());
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------简单提升仇恨等级,不涉及从0开始增加仇恨
void	CHatredMgr::addHatredLevel			(CSceneUnit* pUnit, int32 nAddHatredLevel)
{
	if (pUnit)
	{
		_stHatred* pHatred	= m_mapHatred.find(pUnit->getUnitID());
		if (pHatred)
		{
			pHatred->nLevel += nAddHatredLevel;
			updateFirstHatred();
		}
	}
}
