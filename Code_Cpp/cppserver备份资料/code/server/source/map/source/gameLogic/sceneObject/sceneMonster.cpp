/*----------------- sceneMonster.cpp
*
* Copyright (C): 2011  
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/9/15 11:45:05
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "gameLogic/sceneObject/sceneMonster.h"
#include "protocol/GhostStruct.pb.h"
#include "gameLogic/sceneObject/sceneUnit.h"
#include "gameScene/gameSceneCell.h"
#include "cfg_data/monsterData/monsterData.h"
#include "cfg_data/constantData/constantData.h"
#include "gameLogic/sceneObject/sceneUnitAI.h"
#include "log/log.h"
//#include "gameLogic/playerQuest/playerQuestMgr.h"
//#include "gameLogic/playerTeam/playerTeam.h"
#include "def_struct/teamStruct.h"
#include "protocol/GLAGMsg.pb.h"
#include "main/gameServer.h"
#include "protocol/GLAGProtocol.pb.h"
#include "gameScene/copymapBase.h"
//#include "gameScene/copymapRewardZhenshen.h"
//#include "gameLogic/globalObject/worldBossMgr.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
CSceneMonster::CSceneMonster()
{
	m_stUnitID.uUnitType= GameObject_Monster;
	m_pMonsterModel = NULL;
	m_setBirthAddBuff.clear();
	m_uLastFightTick = 0;	// 上次战斗时间
	m_setFightAddBuff.clear();
	m_uLastSearchTime = 0;
	m_uLastLiangyiTime = 0;
	m_uParam1 = 0;
	m_uParam2 = 0;
	m_pActivePlayer = NULL;
}

//-------------------------------------------------------------
//------------------------------ 
CSceneMonster::~CSceneMonster()
{
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneMonster::packetGhost(uint64 uTime,bool bSelf,uint32& updateMark,PBGhostInfo&pbInfo)
{
	if ( !m_pMonsterModel )
		return false;

	if(_CHECK_BIT(updateMark,_BIT32(Ghost_Remove)))
		return false;

	bool bSend = Parent::packetGhost(uTime,bSelf,updateMark,pbInfo);
	PBGhostMonster& pbMonster= *pbInfo.mutable_monster();

	//基本信息
	if(_CHECK_BIT(updateMark,_BIT32(Ghost_Base)))
	{
		bSend = true;
		pbMonster.set_monsterid(m_pMonsterModel->uMonsterID);
		pbMonster.set_buildtick(m_uBuildTick);
		if (m_uParam1)
			pbMonster.set_param1(m_uParam1);
		if (m_uParam2)
			pbMonster.set_param2(m_uParam2);               
		if (getOwnerPlayerID())
			pbMonster.set_ownerplayerid(getOwnerPlayerID());
		const std::string& strName = getOwnerPlayerName();
		if (!strName.empty())
			pbMonster.set_newname(strName.c_str());

		if (m_pActivePlayer)
		{
			m_pActivePlayer->getPlayerBase().saveToProtobuf(*pbMonster.mutable_activeplayer());
		}
	}
	_SET_BIT32(updateMark,Ghost_Base,false);
	
	//霸体信息
	if(_CHECK_BIT(updateMark,_BIT32(Ghost_SuperArmor)))
	{
		bSend = true;
		pbMonster.set_superarmor(m_uSuperArmor);
		if (m_uSuperArmorTick)
			pbMonster.set_superarmortick(getSystemTick64() - m_uSuperArmorTick);
		else
			pbMonster.set_superarmortick(0);
	}
	_SET_BIT32(updateMark,Ghost_SuperArmor,false);
	
	return bSend;
}

//-------------------------------------------------------------
//------------------------------ 
void	CSceneMonster::enterScene(CGameSceneCell*pCell)
{
	Parent::enterScene(pCell);
	if ( m_pMonsterModel && m_pMonsterModel->uType == MonsterType_Monster && m_pMonsterModel->uSubType == Monster_Boss && pCell && !m_pSceneCell->isCopymap()  )
	{
		PBG2GLCommon_BossInfoUpdate msg;
		msg.set_sceneid(pCell->getSceneID());
		msg.set_cellid(pCell->getCellID());
		msg.set_monsterid(m_pMonsterModel->uMonsterID);
		msg.set_alive(true);

		g_pServer->sendMsgToGlobal(g_pServer->getLogicWorldID(),P_G2GL_Protocol_Common,GLAG_Common_BossInfoUpdate,&msg);
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneMonster::leaveScene()
{
	if ( m_pMonsterModel && m_pMonsterModel->uType == MonsterType_Monster && m_pMonsterModel->uSubType == Monster_Boss && m_pSceneCell && !m_pSceneCell->isCopymap() )
	{
		PBG2GLCommon_BossInfoUpdate msg;
		msg.set_sceneid(m_pSceneCell->getSceneID());
		msg.set_cellid(m_pSceneCell->getCellID());
		msg.set_monsterid(m_pMonsterModel->uMonsterID);
		msg.set_alive(false);

		g_pServer->sendMsgToGlobal(g_pServer->getLogicWorldID(),P_G2GL_Protocol_Common,GLAG_Common_BossInfoUpdate,&msg);
	}

	Parent::leaveScene();
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneMonster::initialize	()
{
	Parent::initialize();
	m_pMonsterModel		= NULL;
	m_uBuildTick		= 0;
	m_uDeadTick			= 0;
	m_uDeadTime			= 0;
	m_uSuperArmor		= 0;
	m_uSuperArmorTick	= 0;
	m_setBirthAddBuff.clear();
	m_setFightAddBuff.clear();
	m_uLastFightTick	= 0;
	m_uLastSearchTime	= 0;
	m_uLastLiangyiTime	= 0;
	m_uParam1			= 0;
	m_uParam2			= 0;
	m_bActivityFreezAI  = false;
	m_pActivePlayer		= NULL;

	return true;
}
//-------------------------------------------------------------
//------------------------------ 构建
bool	CSceneMonster::build	(const _stMonsterModel*pModel,_stMonsterRegion * pMonsterRegion, float32 fPointX,float32 fPointY,float32 fDir,uint32 uRouteID,uint16 uLevel)
{
	if(!pModel )
		return false;

	m_pMonsterModel	= pModel;
	m_pMonsterRegion = pMonsterRegion;

	setPosition(fPointX,fPointY,SetPositionReason_Build,false);
	setDirection(fDir);
	setCampID(pModel->uCampID);
	initAttribute();

	// 基础属性
	_stGameAttribute*pAtrr = getAttribute(GameSystemType_Common);
	if (pModel->uAttrIndex > 0)
	{
		pAtrr->add(g_clMonsterData.findLevelAttr(pModel->uAttrIndex,uLevel));
	}
	else
	{
		pAtrr->add(&pModel->stAttribute);
	}

	calculateAttributeBase();
	//生命
	setHP(getHPMax());
	
	//AI
	setRouteID(uRouteID);
	if (pModel->uAIID)
		m_pUnitAI = CSceneUnitAI::createSceneUnitAI(this, pModel->uAIID);

	m_uBuildTick		= getSystemTick64();
	m_uDeadTick			= 0;
	m_uDeadTime			= 0;
	m_uSuperArmorTick	= 0;
	m_uSuperArmor		= pModel->uSuperArmor;
	if (m_uSuperArmor)
	{
		m_clGameBuffMgr.addBuff(pModel->uSuperArmorBuffID,1,this->getUnitID(),0, DoingType_Monster);
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneMonster::exitScene()
{
	if(m_pSceneCell)
		m_pSceneCell->leaveScene(this);
}
//-------------------------------------------------------------
//------------------------------
bool	CSceneMonster::canRunAI			()
{
	if ( !m_pMonsterModel || isDead() )
		return false;

	if ( m_pMonsterModel->uBirthTick && (getSystemTick64() < m_uBuildTick + m_pMonsterModel->uBirthTick ) )
		return false;

	if (getActivityFreezeAI())
	{
		return false;
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool	CSceneMonster::canUseSkill		(CSceneUnit*pTarget)
{
	if ( !m_pMonsterModel  )
		return false;

	if (!Parent::canUseSkill(pTarget))
		return false;

	if ( pTarget && isQuestLimit(pTarget->getOwnerPlayer()) )
		return false;

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool	CSceneMonster::canUpdateHp		(int32 iHP)
{
	if (!m_pMonsterModel)
		return false;

	if (m_pMonsterModel->uType == MonsterType_RewardZhenshen && iHP < 0 )
	{
		if (m_pSceneCell&&m_pSceneCell->getGameCopymap())
		{
			//CCopymapRewardZhenshen* pCopymap = dynamic_cast<CCopymapRewardZhenshen*>(m_pSceneCell->getGameCopymap());
			//if (pCopymap&&!pCopymap->canUpdateMonsterHp())
			//	return false;
		}
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneMonster::canByUseSkill	(CSceneUnit*pAtker)
{
	if ( !m_pMonsterModel || isDead() )
		return false;

	if (!Parent::canByUseSkill(pAtker))
		return false;

	if ( m_pMonsterModel->check_mark(MonsterMark_NoBeAttack))
		return false;

	if ( m_pMonsterModel->uBirthTick && (getSystemTick64() <= m_uBuildTick + m_pMonsterModel->uBirthTick ) )
		return false;

	if ( pAtker && isQuestLimit(pAtker->getOwnerPlayer()) )
		return false;

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
uint32	CSceneMonster::getFlyTotalTick	()
{
	CHECKF(m_pMonsterModel != NULL);
	int32 uSpeed = g_clConstantData.Game_FlySpeed;
	CHECKF(uSpeed);
	uint32 uFlyTotalTick = m_pMonsterModel->uLandAnimTick +m_pMonsterModel->uClimbAnimTick 
		+ (uint32)m_pMonsterModel->fFlyHeight * 2 * 1000 * 1000 / uSpeed;
	return uFlyTotalTick;
}

//-------------------------------------------------------------
//------------------------------ 
uint32	CSceneMonster::getBakFlayTick	()
{
	CHECKF(m_pMonsterModel != NULL);
	return m_pMonsterModel->uLandAnimTick +m_pMonsterModel->uClimbAnimTick;
}

//-------------------------------------------------------------
//------------------------------ 
float32	CSceneMonster::getModelRadius	()
{
	CHECKF(m_pMonsterModel != NULL);
	return m_pMonsterModel->fModelRadius;
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneMonster::processLogic	(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay)
{
	Parent::processLogic(uTick,uTime,uSecond,bNewDay);
	if (isDead() &&  (getSystemTick64() > m_uDeadTick + m_pMonsterModel->uDeadTick ) && !(haveState(StateType_Singing) || haveState(StateType_Skiling) ) )
	{
		exitScene();
		return ;
	}

	superArmorTick();
	processBirthTick(uTick);
	processFightTick(uTick);

}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneMonster::questComplete	()
{
	if ( !m_pSceneCell || !m_pMonsterModel )
		return ;

	stl_set<CScenePlayer*> setPlayer;
	stl_map<uint32,float32>::const_iterator _pos = m_pMonsterModel->mapQuestDistribute.begin();
	for (; _pos != m_pMonsterModel->mapQuestDistribute.end(); ++_pos)
	{
		uint32 uQuestDistribute = _pos->first;
		switch (uQuestDistribute)
		{
		case MonsterDeadQuestDistribute_Damage://有伤害
			{
				auto _pos = getHurtMgr().getHurtMap().begin();
				auto _end = getHurtMgr().getHurtMap().end();
				for (; _pos != _end; ++_pos)
				{
					CScenePlayer* pTmpPlayer = m_pSceneCell->findPlayer(_pos->first);
					if (pTmpPlayer)
						setPlayer.insert(pTmpPlayer);
				}
			}
			break;
		case MonsterDeadQuestDistribute_Hate://有仇恨
			{
				getHatredMgr().getHatredPlayer(setPlayer);
			}
			break;
		case MonsterDeadQuestDistribute_Radiu://在指定半径内 参数写半径
			{
				MAP_GAME_UNIT mapObject;
				gatherSceneObject(getPosition(),_pos->second,mapObject,_BIT32(GameObject_Player));
				MAP_GAME_UNIT::iterator _tmppos = mapObject.begin();
				for (; _tmppos != mapObject.end(); ++_tmppos)
				{
					CSceneUnit* pTmpUnit = _tmppos->second;
					if (pTmpUnit && pTmpUnit->asPlayer())
						setPlayer.insert(pTmpUnit->asPlayer());
				}

			}
			break;
		default:
			break;
		}
	}

	stl_set<CScenePlayer*>::iterator _spos = setPlayer.begin();
	for (; _spos != setPlayer.end(); ++_spos)
	{
		CScenePlayer* pTmpPlayer = *_spos;
		//CPlayerQuestMgr* pQuestMgr = pTmpPlayer->getPlayerQuestMgr();
		//if(pQuestMgr)
		//	pQuestMgr->callback_killMonster(this);
	}

	if (m_pSceneCell)
	{
		m_pSceneCell->callback_killmonster(this,NULL);
	}
}
//-------------------------------------------------------------
//------------------------------ 
void CSceneMonster::callbackDead(CSceneUnit* pSource)
{
	if ( !m_pSceneCell || !m_pMonsterModel )
		return ;

	m_uDeadTick = getSystemTick64();
	m_uDeadTime = 0;

	//掉落物
	if (m_pMonsterModel->uDropID)
	{
		CScenePlayer* pDropSource = NULL;
		switch (m_pMonsterModel->uDropDistribute)
		{
		case MonsterDeadDropDistribute_MaxDamage://伤害最高
			{
				pDropSource = getHurtMgr().getHurtMaxPlayer();

			}
			break;
		case MonsterDeadDropDistribute_FirstDamage://第一刀
			{
				pDropSource = getHurtMgr().getHurtFirstPlayer();
			}
			break;
		case MonsterDeadDropDistribute_EndDamage://最后一刀
			{
				pDropSource = pSource->asPlayer();
			}
			break;
		default:
			break;
		}
		if (pDropSource)
			m_pSceneCell->dropItem(getUnitID(), m_pMonsterModel->uDropID, pDropSource, getPosition());
	}

	Parent::callbackDead(pSource);

	// 必须放到最后 因为有可能在死亡回调中会leaveScene
	questComplete();

	//
	if (getWorldBossId() && pSource && pSource->asPlayer() )
	{
		//排行榜回调
		//g_pWorldBossMgr->onBossDead(getWorldBossId()
		//	, pSource->asPlayer());
	}

}

//-------------------------------------------------------------
//------------------------------ 
void CSceneMonster::playerExp(CScenePlayer* pPlayer, uint32 iValue)
{
	if (!m_pMonsterModel || !pPlayer || iValue <= 0)
		return;

	if (m_pMonsterModel->uExpType)
		iValue *= 1;

	if (iValue <= 0)
		return;

	pPlayer->addExpend(ExpendType_Exp, iValue, DoingType_Monster);
}

//-------------------------------------------------------------
//------------------------------ 
void CSceneMonster::callbackUpdateHP(int32 iHP)
{
	Parent::callbackUpdateHP(iHP);
	
	//非破甲状态 && 扣血
	if (m_uSuperArmor && !m_uSuperArmorTick && iHP < 0 && m_pMonsterModel)
	{
		//破甲
		if (m_uSuperArmor < uint64(iHP * -1))
		{
			m_uSuperArmor = 0;
			m_uSuperArmorTick = getSystemTick64();
			m_clGameBuffMgr.delBuff(m_pMonsterModel->uSuperArmorBuffID, DoingType_Monster);
		}
		else
		{
			m_uSuperArmor += iHP;
		}
		
		setGhost(_BIT32(Ghost_SuperArmor));
	}
}
//-------------------------------------------------------------
//------------------------------ 
void CSceneMonster::callbackFight()
{
	Parent::callbackFight();
	m_uLastFightTick = getSystemTick64();
	m_setFightAddBuff.clear();
}
//-------------------------------------------------------------
//------------------------------ 
void CSceneMonster::superArmorTick()
{
	//霸体恢复中
	if (m_uSuperArmorTick && m_pMonsterModel && getSystemTick64() > m_uSuperArmorTick + m_pMonsterModel->uSuperArmorTime)
	{
		m_uSuperArmorTick	= 0;
		m_uSuperArmor		= m_pMonsterModel->uSuperArmor;
		m_clGameBuffMgr.addBuff(m_pMonsterModel->uSuperArmorBuffID,1,this->getUnitID(),0, DoingType_Monster);

		setGhost(_BIT32(Ghost_SuperArmor));
	}
}
//-------------------------------------------------------------
//------------------------------ 
void CSceneMonster::processBirthTick(uint64 uTick)
{
	if (!m_pMonsterModel)
		return;

	for (uint32 i = 0; i < m_pMonsterModel->vecBirthAddBuff.size(); ++i)
	{
		const _stDelayAddBuff& stAddBuff = m_pMonsterModel->vecBirthAddBuff[i];
		if (m_setBirthAddBuff.is_exist(stAddBuff.uBuffID))
			continue;

		if (m_uBuildTick + stAddBuff.uDelayTick < uTick)
		{
			getGameBuffMgr().addBuff(stAddBuff.uBuffID,1,this->getUnitID(),0, DoingType_Monster);
			m_setBirthAddBuff.insert(stAddBuff.uBuffID);
		}
	}
}
//-------------------------------------------------------------
//------------------------------ 
void CSceneMonster::processFightTick(uint64 uTick)
{
	if (!m_pMonsterModel || !m_uLastFightTick)
		return;

	for (uint32 i = 0; i < m_pMonsterModel->vecFightAddBuff.size(); ++i)
	{
		const _stDelayAddBuff& stAddBuff = m_pMonsterModel->vecFightAddBuff[i];
		if (m_setFightAddBuff.is_exist(stAddBuff.uBuffID))
			continue;

		if (m_uLastFightTick + stAddBuff.uDelayTick < uTick)
		{
			getGameBuffMgr().addBuff(stAddBuff.uBuffID,1,this->getUnitID(),0, DoingType_Monster);
			m_setFightAddBuff.insert(stAddBuff.uBuffID);
		}
	}
}

//-------------------------------------------------------------
//------------------------------ 
float32 CSceneMonster::getCallDist()
{
	return m_pMonsterModel ? m_pMonsterModel->fCallDist : 0.0f;
}

//-------------------------------------------------------------
//------------------------------ 
uint8 CSceneMonster::getMonsterType()
{
	return m_pMonsterModel ? m_pMonsterModel->uType : 0;		
}

//-------------------------------------------------------------
//------------------------------ 
uint32 CSceneMonster::getMonsterID()
{
	return m_pMonsterModel ? m_pMonsterModel->uMonsterID : 0;		
}
//-------------------------------------------------------------
//------------------------------ 
bool	CSceneMonster::isQuestLimit	(CScenePlayer* pPlayer)
{
	if ( !pPlayer || !m_pMonsterModel || m_pMonsterModel->vecQuestIDs.empty() )
		return false;

	//CPlayerQuestMgr* pQuestMgr = pPlayer->getPlayerQuestMgr();
	//if ( !pQuestMgr )
	//	return false;

	auto _pos = m_pMonsterModel->vecQuestIDs.begin();
	auto _end = m_pMonsterModel->vecQuestIDs.end();
	for ( ; _pos != _end ;++ _pos  )
	{
		//if (pQuestMgr->getQuest(*_pos))
		//	return false;
	}

	//if (!m_pMonsterModel->setTeamTaskVisibleID.empty() &&  pPlayer->getPlayerTeam())
	//{
		//uint32 uTeamTask =  pPlayer->getPlayerTeam()->getTeamTaskID();
		//if (m_pMonsterModel->setTeamTaskVisibleID.is_exist(uTeamTask))
		//	return false;
	//}

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool	CSceneMonster::isVisible				(CSceneUnit*pObject)
{
	if (!pObject)
		return false;

	CScenePlayer* pPlayer = pObject->getOwnerPlayer();
	if (pPlayer && isQuestLimit(pPlayer) )
		return false;

	return Parent::isVisible(pObject);
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneMonster::callbackHurt			(CSceneUnit* pUnit,int32 iHurt)
{
	Parent::callbackHurt(pUnit,iHurt);
	if (m_pSceneCell)
	{
		CCopymapBase* pCopymap = m_pSceneCell->getGameCopymap();
		if (pCopymap)
			pCopymap->callback_monsterhurt(this,pUnit,iHurt);
	}

	if (getWorldBossId() && pUnit && pUnit->asPlayer() && iHurt > 0)
	{
		//排行榜回调
		//g_pWorldBossMgr->updateHurt(getWorldBossId()
		//	,(uint32)iHurt
		//	, pUnit->asPlayer());
	}

}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneMonster::speak			(uint32 uSpeakID)
{
	if (m_pSceneCell)
	{
		PBG2CCommon_MonsterTalk ack;
		ack.set_unitid(getUnitID());
		ack.set_monsterid(getMonsterID());
		ack.set_posx(getPosition().x);
		ack.set_posy(getPosition().y);
		ack.set_talkid(uSpeakID);

		m_pSceneCell->broadcastMsg(P_S2C_Protocol_Common,S2C_Common_MonsterTalk,&ack,0);
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CSceneMonster::updateAttrByRatio(uint8 uType,uint32 uAttrRatio, std::set<int32>& setNeedChange)
{
	// 基础属性
	_stGameAttribute*pAtrr = getAttribute((_emGameSystemType)uType);
	if ( pAtrr )
	{
		//根据外面传入的倍率,将属性重新计算
		//不应该更改移动属性,需要排除 
		if (uAttrRatio > 0)
		{
			pAtrr->updateAttributeExistedAllByRatio(uAttrRatio, setNeedChange);
		}

		calculateAttributeBase();
		//生命
		setHP(getHPMax());
	}
}

//-------------------------------------------------------------
//------------------------------ 
void	CSceneMonster::setActivePlayer	(CScenePlayer* pPlayer)
{
	if (pPlayer != m_pActivePlayer)
	{
		m_pActivePlayer = pPlayer;
		setGhost(_BIT32(Ghost_Base));
	}
}
