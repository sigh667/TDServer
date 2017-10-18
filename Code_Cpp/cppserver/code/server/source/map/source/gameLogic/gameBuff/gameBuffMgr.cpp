/*----------------- gameBuffMgr.cpp
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/1/26 14:48:49
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "gameLogic/gameBuff/gameBuffMgr.h"
#include "gameLogic/sceneObject/sceneUnit.h"
#include "main/gameServer.h"
#include "protocol/MsgResult.pb.h"
#include "cfg_data/buffData/buffData.h"
#include "protocol/CASMsg.pb.h"
#include "protocol/CASProtocol.pb.h"
#include "protocol/GameDefine.pb.h"
#include "gameLogic/sceneObject/scenePlayer.h"
#include "cfg_data/constantData/constantData.h"
#include "gameScene/gameSceneCell.h"
#include "gameScene/copymapBase.h"
//#include "gameScene/copymapManorBattleField.h"
#include "log/log.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
CGameBuffMgr::CGameBuffMgr()
{
	initialize(NULL);
}

//-------------------------------------------------------------
//------------------------------ 
CGameBuffMgr::~CGameBuffMgr()
{
}
//-------------------------------------------------------------
//------------------------------ 
void CGameBuffMgr::initialize(CSceneUnit* pOwner)
{
	m_pOwner = pOwner;
	m_uProcessTick	= 0;
	clear();
}

//-------------------------------------------------------------
//------------------------------ 
void	CGameBuffMgr::clear()
{
	m_dmapBuff.clear();
	m_poolBuff.clear();
	m_mapOneTimeBuff.clear();

	if (m_pOwner)
		m_pOwner->refreshStateBuffAttr();
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameBuffMgr::onClientGo		()
{
	//重新同步玩家所有的buff
	if (!canOperate() || !m_pOwner->asPlayer())
		return;

	m_dmapBuff.clear();
	m_mapOneTimeBuff.clear();
	PBG2CAddBuff ack;
	ack.set_targetid(m_pOwner->getUnitID());
	POOL_BUFF::iterator _pos;
	m_poolBuff.getHead(_pos);
	while (!m_poolBuff.isTail(_pos))
	{ 
		CGameBuff*	pBuff = m_poolBuff.getNext(_pos);
		if (!pBuff || pBuff->isDelete()) 
			continue;

		PBBuff* pPBBuff = ack.add_pbbuff();
		if (!pPBBuff)
			continue;

		pPBBuff->set_buffid(pBuff->getBuffID());
		pPBBuff->set_level(pBuff->getLevel());
		pPBBuff->set_lefttick(pBuff->getLeftTick());
		pPBBuff->set_unitid(pBuff->getSourceID());
		m_dmapBuff.insert(pBuff->getBuffID(),pBuff->getSourceID(),pBuff);
	}

	m_pOwner->sendBroadcast(P_S2C_Protocol_Buff,S2C_Buff_AllSyn,&ack,R_ResultBuff_Succeed);
}
//-------------------------------------------------------------
//------------------------------ 
CGameBuff*	CGameBuffMgr::find(uint32 uBuffID,uint32 uSourceID)	
{
	CGameBuff** pTmpBuff =  m_dmapBuff.find_(uBuffID,uSourceID);
	if (!pTmpBuff)
		return NULL;
	return *pTmpBuff;
}

//-------------------------------------------------------------
//------------------------------ 查找第一个buff
CGameBuff*	CGameBuffMgr::findFirst(uint32 uBuffID)	
{
	SUB_DMAP_BUFF* pSubMap =  m_dmapBuff.find_(uBuffID);
	if (!pSubMap || pSubMap->empty())
		return NULL;

	return pSubMap->begin()->second;
}
//-------------------------------------------------------------
//------------------------------获取组ID的buff,没有返回false
bool	CGameBuffMgr::getGroupBuff(uint32 uGroupID,stl_vector<CGameBuff*>& vecGameBuff)
{
	vecGameBuff.clear();
	POOL_BUFF::iterator _pos;
	m_poolBuff.getHead(_pos);
	while (!m_poolBuff.isTail(_pos))
	{
		CGameBuff* pGameBuff = m_poolBuff.getNext(_pos);
		if (!pGameBuff || pGameBuff->isDelete())
			continue;
		if (pGameBuff->getGroupID() == uGroupID)
			vecGameBuff.push_back(pGameBuff);
	}

	return !vecGameBuff.empty();
}

//-------------------------------------------------------------
//------------------------------ 
CGameBuff*	CGameBuffMgr::alloce(const _stBuffModel* pModel,uint32 uSourceID)
{
	if (!pModel)
		return NULL;

	CGameBuff* pBuff = m_poolBuff.allocate();
	if (!pBuff)
		return NULL;

	m_dmapBuff.insert(pModel->uBuffID,uSourceID,pBuff);
	return pBuff;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CGameBuffMgr::load(const PBSystemBuff& pbBuff)
{
	if (!canOperate())
		return false;

	this->clear();
	for (uint16 i = 0; i < pbBuff.pbbuff_size(); ++i)
	{
		uint32 uBuffID		= pbBuff.pbbuff(i).buffid();
		uint64 uLeftTick	= pbBuff.pbbuff(i).lefttick();
		uint64 uEndTime		= pbBuff.pbbuff(i).endtime();
		uint8  uLevel		= pbBuff.pbbuff(i).level();
		uint32 uUnitID		= pbBuff.pbbuff(i).unitid();
		uint32 uSkillID		= pbBuff.pbbuff(i).skillid();

		if (uEndTime)
			uLeftTick = MAX((uEndTime - (uint64)getTime())*1000,(uint64)0);

		_stBuffModel* pBuffModel = g_clBuffData.find(uBuffID,uLevel);
		if (!pBuffModel || pBuffModel->check_mark(BuffMark_OfflineDelete))
			continue;

		this->addBuff(uBuffID,uLevel,uLeftTick,uUnitID,uSkillID, false, DoingType_Buff);
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CGameBuffMgr::save(PBSystemBuff* pbBuff)
{
	pbBuff->clear_pbbuff();

	POOL_BUFF::iterator _pos;
	m_poolBuff.getHead(_pos);
	while (!m_poolBuff.isTail(_pos))
	{
		CGameBuff* pGameBuff = m_poolBuff.getNext(_pos);
		if (!pGameBuff || pGameBuff->isDelete())
			continue;

		uint64 uEndTime = (uint64)getTime() + pGameBuff->getLeftTick() / 1000;
		if (pGameBuff->getBuffModel()->check_mark(BuffMark_OfflineStop))
			uEndTime = 0;
		PBBuff* pbTmpBuff = pbBuff->add_pbbuff();
		if (pbTmpBuff)
		{
			pbTmpBuff->set_buffid(pGameBuff->getBuffID());
			pbTmpBuff->set_lefttick(pGameBuff->getLeftTick());
			pbTmpBuff->set_endtime(uEndTime);
			pbTmpBuff->set_level(pGameBuff->getLevel());
			pbTmpBuff->set_unitid(pGameBuff->getSourceID());
		}
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------
bool	CGameBuffMgr::addBuffAsk	(PBBuff* pBuff,uint8& uCode)
{
	return this->addBuff(pBuff->buffid(),pBuff->level(),pBuff->unitid(),pBuff->skillid(), DoingType_Buff);
}

//-------------------------------------------------------------
//------------------------------ 
bool	CGameBuffMgr::delBuffAsk	(uint32 uBuffID,uint8& uCode)
{
	return this->delBuff(uBuffID, DoingType_Buff);
}

//-------------------------------------------------------------
//------------------------------能否添加buff
bool	CGameBuffMgr::canAddBuff		(const _stBuffModel* pModel)
{
	if (!canOperate() || !pModel)
		return false;

	//判断身上buff是否已满
	uint8 uMaxBuffCount = g_clConstantData.Skill_MaxBuffCount;
	if (m_poolBuff.size() >= uMaxBuffCount)
		return false;

	//判断特例互斥
	CBuffData::DMAP_Mutual_MODEL::_sub_map* pSubMap = g_clBuffData.findIDMutual(pModel->uBuffID);
	if (pSubMap)
	{
		CBuffData::DMAP_Mutual_MODEL::_sub_map::iterator _pos = pSubMap->begin();
		CBuffData::DMAP_Mutual_MODEL::_sub_map::iterator _end = pSubMap->end();
		for (; _pos != _end; ++_pos)
		{
			if (_emBuffMutualType_DelNew != _pos->second)
				continue;
			
			if (m_dmapBuff.find_(_pos->first))
				return false;
		}
	}

	//判断组互斥
	pSubMap = g_clBuffData.findGroupMutual(pModel->uGroupID);
	if (pSubMap)
	{
		CBuffData::DMAP_Mutual_MODEL::_sub_map::iterator _pos = pSubMap->begin();
		CBuffData::DMAP_Mutual_MODEL::_sub_map::iterator _end = pSubMap->end();
		for (; _pos != _end; ++_pos)
		{
			if (_emBuffMutualType_DelNew != _pos->second)
				continue;
			
			stl_vector<CGameBuff*> vecGameBuff;
			if (getGroupBuff(_pos->first,vecGameBuff))
				return false;
		}
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CGameBuffMgr::DelOldBuff		(const _stBuffModel* pModel)
{
	if (!canOperate() || !pModel)
		return false;

	//删除互斥buff
	CBuffData::DMAP_Mutual_MODEL::_sub_map* pSubMap = g_clBuffData.findIDMutual(pModel->uBuffID);
	if (pSubMap)
	{
		CBuffData::DMAP_Mutual_MODEL::_sub_map::iterator _pos = pSubMap->begin();
		CBuffData::DMAP_Mutual_MODEL::_sub_map::iterator _end = pSubMap->end();
		for (; _pos != _end; ++_pos)
		{
			if (_emBuffMutualType_DelOld != _pos->second)
				continue;

			if (m_dmapBuff.find_(_pos->first))
			{
				delBuff(_pos->first, DoingType_Buff);
			}
		}
	}

	//判断组互斥
	pSubMap = g_clBuffData.findGroupMutual(pModel->uGroupID);
	if (pSubMap)
	{
		CBuffData::DMAP_Mutual_MODEL::_sub_map::iterator _pos = pSubMap->begin();
		CBuffData::DMAP_Mutual_MODEL::_sub_map::iterator _end = pSubMap->end();
		for (; _pos != _end; ++_pos)
		{
			if (_emBuffMutualType_DelOld != _pos->second)
				continue;

			stl_vector<CGameBuff*> vecGameBuff;
			if (!getGroupBuff(_pos->first,vecGameBuff))
				continue;

			for (uint16 i = 0; i < vecGameBuff.size(); ++i)
			{
				CGameBuff* pTmpGameBuff = vecGameBuff[i];
				if (!pTmpGameBuff || pTmpGameBuff->getBuffID() == pModel->uBuffID)
					continue;

				pTmpGameBuff->setDelete(true);
			}
		}
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CGameBuffMgr::addBuff(uint32 uBuffID,uint8 uLevel,uint32 uSourceID,uint32 uSkillID,_emDoingType emDoing)
{
	if (!canOperate())
		return false;

	const _stBuffModel* pModel = g_clBuffData.find(uBuffID,uLevel);
	if (!pModel) 
		return false;

	uint32 uContinueTime = getBuffTime(pModel);
	if ( uSourceID )
	{
		CSceneUnit* pSource = m_pOwner->findCellObject(uSourceID);
		if (pSource)
		{
			int32 nMS = pSource->getGameBuffMgr().getAtkBuffAddTime(pModel);
			updateUint32(uContinueTime,nMS);
		}
	}
	return addBuff(uBuffID,uLevel,uContinueTime,uSourceID,uSkillID,true,emDoing);
}

//-------------------------------------------------------------
//------------------------------ 
bool	CGameBuffMgr::addBuff(uint32 uBuffID,uint8 uLevel,uint64 uLeftTick,uint32 uSourceID,uint32 uSkillID,bool bSend/* = false*/,_emDoingType emDoing)
{
	if ( !m_pOwner )
		return false;

	const _stBuffModel* pModel = g_clBuffData.find(uBuffID,uLevel);
	if (!pModel)
		return false;

	//是否消失
	if (!uLeftTick && pModel->uContinueTime)
	{
		if (pModel->check_mark(BuffMark_OnlineClearBroad))
			CGameBuffMgr::send_DelBuffAck(uBuffID,uSourceID,R_ResultBuff_Succeed,true,m_pOwner);

		return true;
	}

	//能不能加上去
	if (!canAddBuff(pModel))
		return false;

	//陷阱只加一次buff
	if (pModel->check_mark(BuffMark_TrapAddBuffOneTime))
	{
		uint32 *hadBuffID = m_mapOneTimeBuff.find_(uSourceID);

		if (hadBuffID && *hadBuffID == uBuffID)
		{
			return false;
		}
		m_mapOneTimeBuff[uSourceID] = uBuffID;
	}

	//加上去互斥
	DelOldBuff(pModel);
	
	//找相同来源
	CGameBuff* pGameBuff = find(uBuffID,uSourceID);
	if (pGameBuff)
	{
		pGameBuff->repeat(pModel, uLeftTick);
	}
	else //不同来源
	{
		bool bAdd = false;

		//如果没有buff 无条件加
		if (!m_dmapBuff.find_(uBuffID))
		{
			bAdd = true;
		}
		else if (BuffDifRepeatType_Save == pModel->uDifRepeatType)
		{
			bAdd = true;
		}
		else if (BuffDifRepeatType_Drop == pModel->uDifRepeatType)
		{
			//如果有 丢弃
			if (!m_dmapBuff.find_(uBuffID))
				bAdd = true;
		}
		else if (BuffDifRepeatType_Refresh == pModel->uDifRepeatType)
		{
			//删除已有的
			CGameBuff* pOldBuff = findFirst(uBuffID);
			if (pOldBuff)
				pOldBuff->setDelete(true);
			
			bAdd = true;
		}
		if (bAdd)
		{
			pGameBuff = alloce(pModel,uSourceID);
			if (!pGameBuff)
				return false;
			
			pGameBuff->initialize(uSourceID,m_pOwner,pModel,uLeftTick);
			if (pModel->uStateType)
			{
				m_pOwner->refreshStateBuffAttr();
			}
		}
	}

	//刷新属性
	if (BuffType_Attri == pModel->uBuffType )
	{
		m_pOwner->refreshBuffAttr();
	}

	if (pGameBuff
		&& m_pOwner->getSceneCell())
	{
		CSceneUnit* pSourceUnit = m_pOwner->getSceneCell()->findObject(uSourceID);
		//回调
		if (m_pOwner->getGameCopymap()
			&& m_pOwner->getGameCopymap()->getCopymapType() == CopymapType_ManorBattleField
			&& pSourceUnit
			&& pSourceUnit->asPlayer()
			)
		{
			//CCopymapManorBattleField* p = (CCopymapManorBattleField*) (m_pOwner->getGameCopymap());
			//if (p)
			//{
			//	if (pModel->uEffectType == EffectType_Useful)
			//	{
			//		p->onAddUsefulBuff(m_pOwner->getPlayerID(), pSourceUnit->asPlayer()->getPlayerID());
			//	}
			//	else
			//	{
			//		p->onAddHarmfulBuffOrHurt(m_pOwner->getPlayerID(), pSourceUnit->asPlayer()->getPlayerID());
			//	}
			//}

		}
	}
	
	//通知客户端增加buff
	if(bSend&&pGameBuff)
	{
		if (BuffType_Move == pModel->uBuffType)
		{
			CGameBuffMgr::send_AddMoveBuffAck(uBuffID,uLevel,pGameBuff->getLeftTick(),uSourceID,uSkillID,R_ResultBuff_Succeed,true,m_pOwner);
		}
		else
		{
			CGameBuffMgr::send_AddBuffAck(uBuffID,uLevel,pGameBuff->getLeftTick(),uSourceID,uSkillID,R_ResultBuff_Succeed,true,m_pOwner);
		}
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 预删除
bool	CGameBuffMgr::delBuff(uint32 uBuffID,_emDoingType emDoing)
{
	SUB_DMAP_BUFF* pSubMap = m_dmapBuff.find_(uBuffID);
	if (!pSubMap)
		return false;

	SUB_DMAP_BUFF::iterator _pos = pSubMap->begin();
	SUB_DMAP_BUFF::iterator _end = pSubMap->end();
	for (; _pos != _end;++_pos)
	{
		CGameBuff* pBuff = _pos->second;
		if (pBuff)
			pBuff->setDelete();
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 直接删除
bool	CGameBuffMgr::delBuff(CGameBuff* pGameBuff,bool bSend,_emDoingType emDoing)
{
	if (!pGameBuff || !m_pOwner )
		return false;

	const _stBuffModel* pModel = pGameBuff->getBuffModel();

	onDelete(pGameBuff);

	//通知客户端增加技能
	if(bSend)
	{
		CGameBuffMgr::send_DelBuffAck(pGameBuff->getBuffID(),pGameBuff->getSourceID(),R_ResultBuff_Succeed,true,m_pOwner);
	}
	m_dmapBuff.erase(pGameBuff->getBuffID(),pGameBuff->getSourceID());
	m_poolBuff.release(pGameBuff);

	if (pModel&&pModel->uStateType)
	{
		m_pOwner->refreshStateBuffAttr();
	}

	//刷新属性
	if (pModel && BuffType_Attri == pModel->uBuffType )
	{
		m_pOwner->refreshBuffAttr();
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------ 刷新buff属性
bool	CGameBuffMgr::refreshAttr()
{
	POOL_BUFF::iterator _pos;
	m_poolBuff.getHead(_pos);
	while (!m_poolBuff.isTail(_pos))
	{
		CGameBuff*	pBuff = m_poolBuff.getNext(_pos);
		if (!pBuff || pBuff->isDelete())
			continue;

		const _stBuffModel* pBuffModel = pBuff->getBuffModel();
		if (!pBuffModel || BuffType_Attri != pBuffModel->uBuffType)
			continue;

		pBuff->calcAttr();
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------ 清除buff
bool	CGameBuffMgr::clearBuff(_emEffectType emEffectType,_emDoingType emDoing)
{
	if (!canOperate())
		return false;

	POOL_BUFF::iterator _pos;
	m_poolBuff.getHead(_pos);
	while (!m_poolBuff.isTail(_pos))
	{
		CGameBuff*	pBuff = m_poolBuff.getNext(_pos);
		if (!pBuff || !pBuff->getBuffModel() || pBuff->getBuffModel()->check_mark(BuffMark_NoClear))
			continue;

		if (pBuff->getBuffModel()->uEffectType != emEffectType)
			continue;

		pBuff->setDelete(true);
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------ 重置buff时间
bool	CGameBuffMgr::resetBuffTime(CSceneUnit* pSource,uint32 uBuffID)
{
	if (!canOperate())
		return false;

	CGameBuff* pGameBuff = find(uBuffID,pSource->getUnitID());
	if (!pGameBuff || !pGameBuff->getBuffModel())
		return false;

	pGameBuff->updateLeftTick(pGameBuff->getContinueTick(),true);
	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CGameBuffMgr::send_AddBuffAck	(uint32 uBuffID,uint8 uLevel,uint64 uLeftTick,uint32 uUnitID,uint32 uSkillID,uint8 uCode,bool bBroad,CSceneUnit* pOwner)
{
	if (!pOwner)
		return false;
	if (!uUnitID)
		uUnitID = pOwner->getUnitID();

	//CLog::error("CGameBuffMgr::send_AddBuffAck 增加buffid:%d,uLevel:%d,uLeftTick:%lld,unitid:%d,ownerid:%d",uBuffID,uLevel,uLeftTick,uUnitID,pOwner->getUnitID());
	PBBuff _pbBuff;
	_pbBuff.set_buffid(uBuffID);
	_pbBuff.set_level(uLevel);
	_pbBuff.set_lefttick(uLeftTick);
	_pbBuff.set_unitid(uUnitID);
	_pbBuff.set_skillid(uSkillID);
	stl_vector<PBBuff> vecPBBuff;
	vecPBBuff.push_back(_pbBuff);
	return CGameBuffMgr::send_AddBuffAck(vecPBBuff,uCode,bBroad,pOwner);
}

//-------------------------------------------------------------
//------------------------------ 
bool	CGameBuffMgr::send_AddBuffAck	(stl_vector<PBBuff>& vecPBBuff,uint8 uCode,bool bBroad,CSceneUnit* pOwner)
{
	if (!pOwner || vecPBBuff.empty())
		return false;

	PBG2CAddBuff ack;
	ack.set_targetid(pOwner->getUnitID());
	for (uint8 i = 0; i < vecPBBuff.size(); i ++)
	{
		PBBuff* pTmp = ack.add_pbbuff();
		pTmp->CopyFrom(vecPBBuff[i]);
	}

	if (bBroad)
	{
		pOwner->sendBroadcast(P_S2C_Protocol_Buff,S2C_Buff_Add,&ack,uCode);
	}
	else
	{
		CScenePlayer* pPlayer = pOwner->asPlayer();
		if (pPlayer)
			pPlayer->sendMsg(P_S2C_Protocol_Buff,S2C_Buff_Add,&ack,uCode);
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CGameBuffMgr::send_AddMoveBuffAck (uint32 uBuffID,uint8 uLevel,uint64 uLeftTick,uint32 uUnitID,uint32 uSkillID,uint8 uCode,bool bBroad,CSceneUnit* pOwner)
{
	if (!pOwner)
		return false;
	if (!uUnitID)
		uUnitID = pOwner->getUnitID();

	//CLog::error("CGameBuffMgr::send_AddMoveBuffAck 增加buffid:%d,uLevel:%d,uLeftTick:%lld,unitid:%d,ownerid:%d",uBuffID,uLevel,uLeftTick,uUnitID,pOwner->getUnitID());
	PBG2CAddMoveBuff ack;
	ack.set_targetid(pOwner->getUnitID());
	ack.mutable_pbposition()->set_posx(pOwner->getPosition().x);
	ack.mutable_pbposition()->set_posy(pOwner->getPosition().y);
	ack.mutable_pbbuff()->set_buffid(uBuffID);
	ack.mutable_pbbuff()->set_level(uLevel);
	ack.mutable_pbbuff()->set_lefttick(uLeftTick);
	ack.mutable_pbbuff()->set_unitid(uUnitID);
	ack.mutable_pbbuff()->set_skillid(uSkillID);
	if (bBroad)
	{
		pOwner->sendBroadcast(P_S2C_Protocol_Buff,S2C_Buff_AddMove,&ack,uCode);
	}
	else
	{
		CScenePlayer* pPlayer = pOwner->asPlayer();
		if (pPlayer)
			pPlayer->sendMsg(P_S2C_Protocol_Buff,S2C_Buff_AddMove,&ack,uCode);
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CGameBuffMgr::send_DelBuffAck	(uint32 uBuffID,uint32 uSourceID,uint8 uCode,bool bBroad,CSceneUnit* pOwner)
{
	if (!pOwner)
		return false;

	//CLog::error("CGameBuffMgr::send_DelBuffAck 删除buffid:%d,unitid:%d",uBuffID,pOwner->getUnitID());
	PBG2CDelBuff ack;
	ack.set_unitid(pOwner->getUnitID());
	ack.set_buffid(uBuffID);
	ack.set_souceid(uSourceID);

	if (bBroad)
	{
		pOwner->sendBroadcast(P_S2C_Protocol_Buff,S2C_Buff_Del,&ack,uCode);

	}
	else
	{
		CScenePlayer* pPlayer = pOwner->asPlayer();
		if (pPlayer)
			pPlayer->sendMsg(P_S2C_Protocol_Buff,S2C_Buff_Del,&ack,uCode);
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameBuffMgr::send_BuffHpAck	(int32 iAddHp,CSceneUnit* pOwner,CSceneUnit* pSource,_emDoingType emDoing)
{
	if (!pOwner)
		return false;

	PBG2CAddHp ack;
	ack.set_objectid(pOwner->getUnitID());
	ack.set_addvalue(iAddHp);

	//无主人的怪物加血直接广播
	if (pOwner->asMonster() && iAddHp > 0)
	{
		if (!pOwner->getOwner() || pOwner->getOwner() == pOwner )
			pOwner->sendBroadcast(P_S2C_Protocol_Common,S2C_Common_AddHp,&ack,R_ResultBuff_Succeed,true);
	}

	//buff增加才通知
	if (DoingType_Buff == emDoing || DoingType_Item_Use == emDoing)
	{
		//是玩家
		if (pOwner->asPlayer())
		{
			pOwner->asPlayer()->sendMsg(P_S2C_Protocol_Common,S2C_Common_AddHp,&ack);
		}

		//有主人
		if (pOwner->getOwner() && pOwner->getOwner() != pOwner)
		{
			CScenePlayer* pOwnerPlayer = pOwner->getOwner()->asPlayer();
			if (pOwnerPlayer)
				pOwnerPlayer->sendMsg(P_S2C_Protocol_Common,S2C_Common_AddHp,&ack);
		}

		if (pSource && pSource != pOwner && pSource->asPlayer())
		{
			pSource->asPlayer()->sendMsg(P_S2C_Protocol_Common,S2C_Common_AddHp,&ack);
		}
	}


	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGameBuffMgr::send_UpdateBuffTickAck	(uint32 uBuffID,uint32 uSouceID,uint64 uLeftTick,CSceneUnit* pOwner)
{
	if (!pOwner)
		return false;

	PBG2CUpdateBuffTick ack;
	ack.set_buffid(uBuffID);
	ack.set_lefttick(uLeftTick);
	ack.set_unitid(uSouceID);

	CScenePlayer* pPlayer = pOwner->asPlayer();
	if (pPlayer)
		pPlayer->sendMsg(P_S2C_Protocol_Buff,S2C_Buff_UpdateTick,&ack);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 删除buff
void	CGameBuffMgr::onDelete	(CGameBuff* pBuff)
{
	if(!pBuff)
		return;

	pBuff->onDelete();
}

//-------------------------------------------------------------
//------------------------------
bool	CGameBuffMgr::process	(uint64 uTick,uint64 uTime,uint32 uSecond)
{
	//100ms一次
	if(m_uProcessTick > uTick)
		return true;

	if(!m_uProcessTick)
		m_uProcessTick = uTick + 100;

	POOL_BUFF::iterator _spos;
	m_poolBuff.getHead(_spos);
	while (!m_poolBuff.isTail(_spos))
	{
		CGameBuff*	pBuff = m_poolBuff.getNext(_spos);
		if (!pBuff)
			continue;
		pBuff->process(uTick,uTime,uSecond);

		if (pBuff->isDelete())
			delBuff(pBuff,true, DoingType_Buff);
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------
void	CGameBuffMgr::callbackCritical	()
{
	POOL_BUFF::iterator _pos;
	m_poolBuff.getHead(_pos);
	while (!m_poolBuff.isTail(_pos))
	{
		CGameBuff*	pBuff = m_poolBuff.getNext(_pos);
		if (!pBuff || pBuff->isDelete() || !pBuff->getBuffModel())
			continue;

		//增加层数
		if (pBuff->getBuffModel()->check_mark(BuffMark_CriticalAddLayer))
			pBuff->addLayer(1);
	}
}

//-------------------------------------------------------------
//------------------------------
void	CGameBuffMgr::callbackSkillHit	()
{
	POOL_BUFF::iterator _pos;
	m_poolBuff.getHead(_pos);
	while (!m_poolBuff.isTail(_pos))
	{
		CGameBuff*	pBuff = m_poolBuff.getNext(_pos);
		if (!pBuff || pBuff->isDelete() || !pBuff->getBuffModel())
			continue;

		if (BuffType_AttackAddExpend == pBuff->getBuffModel()->uBuffType)
			pBuff->act();
	}
}
//-------------------------------------------------------------
//------------------------------
void	CGameBuffMgr::callbackBeSkillHit	(CSceneUnit* pUnit,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel)
{
	if (!pUnit || !pSkillModel || !pSkillEffectModel)
		return;

	POOL_BUFF::iterator _pos;
	m_poolBuff.getHead(_pos);
	while (!m_poolBuff.isTail(_pos))
	{
		CGameBuff*	pBuff = m_poolBuff.getNext(_pos);
		if (!pBuff || pBuff->isDelete() || !pBuff->getBuffModel())
			continue;

		pBuff->beSkillHit(pUnit,pSkillModel);
	}
}
//-------------------------------------------------------------
//------------------------------
void	CGameBuffMgr::callbackUseSkill	()
{
	POOL_BUFF::iterator _pos;
	m_poolBuff.getHead(_pos);
	while (!m_poolBuff.isTail(_pos))
	{
		CGameBuff*	pBuff = m_poolBuff.getNext(_pos);
		if (!pBuff || pBuff->isDelete() || !pBuff->getBuffModel())
			continue;

		//使用技能删除buff
		if (pBuff->getBuffModel()->check_mark(BuffMark_UseSkillDel))
			pBuff->setDelete(true);
	}
}
//-------------------------------------------------------------
//------------------------------
void	CGameBuffMgr::callbackUseSkillEnd	()
{
	POOL_BUFF::iterator _pos;
	m_poolBuff.getHead(_pos);
	while (!m_poolBuff.isTail(_pos))
	{
		CGameBuff*	pBuff = m_poolBuff.getNext(_pos);
		if (!pBuff || pBuff->isDelete() || !pBuff->getBuffModel())
			continue;

		//使用技能删除buff
		if (pBuff->getBuffModel()->check_mark(BuffMark_UseSkillEndDel))
			pBuff->setDelete(true);
	}
}
//-------------------------------------------------------------
//------------------------------ 玩家死亡删除所有的buff
void	CGameBuffMgr::callbackDead	()
{
	POOL_BUFF::iterator _pos;
	m_poolBuff.getHead(_pos);
	while (!m_poolBuff.isTail(_pos))
	{
		CGameBuff*	pBuff = m_poolBuff.getNext(_pos);
		if (!pBuff || pBuff->isDelete() || !pBuff->getBuffModel())
			continue;

		if (pBuff->getBuffModel()->check_mark(BuffMark_IsDeadClear))
		{
			pBuff->setDelete(true);
		}
	}
}
//-------------------------------------------------------------
//------------------------------ 跨地图
void	CGameBuffMgr::callback_flyout	()
{
	stl_vector<CGameBuff*> vecGameBuff;
	POOL_BUFF::iterator _pos;
	m_poolBuff.getHead(_pos);
	while (!m_poolBuff.isTail(_pos))
	{
		CGameBuff*	pBuff = m_poolBuff.getNext(_pos);
		if (!pBuff || !pBuff->getBuffModel())
			continue;

		if (pBuff->getBuffModel()->check_mark(BuffMark_FlyMapDelete))
		{
			pBuff->setDelete(true);
			vecGameBuff.push_back(pBuff);
		}
	}

	for (uint32 i = 0; i < vecGameBuff.size(); ++i)
	{
		CGameBuff* pBuff = vecGameBuff[i];
		if (pBuff)
			pBuff->setDelete(true);
	}
}
//-------------------------------------------------------------
//------------------------------ 
void CGameBuffMgr::callbackChangeExpend		(uint16 uExpendType,int32 iValue)
{
	if (!iValue || !m_pOwner)
		return;

	POOL_BUFF::iterator _pos;
	m_poolBuff.getHead(_pos);
	while (!m_poolBuff.isTail(_pos))
	{
		CGameBuff*	pBuff = m_poolBuff.getNext(_pos);
		if (!pBuff || pBuff->isDelete() || !pBuff->getBuffModel())
			continue;
		const _stBuffExpendAddExpend& stExpendAddExpend = pBuff->getBuffModel()->stExpendAddExpend;
		if (BuffType_ExpendAddExpend != pBuff->getBuffModel()->uBuffType || stExpendAddExpend.uExpendType1 != uExpendType || !stExpendAddExpend.iValue1)
			continue;
		int32 iNum = iValue / stExpendAddExpend.iValue1;
		if (iNum <= 0)
			continue;

		int32 iAddNum = iNum * stExpendAddExpend.iValue2;
		m_pOwner->addExpend(stExpendAddExpend.uExpendType2,iAddNum,DoingType_Buff);
	}
}
//-------------------------------------------------------------
//------------------------------
int32	CGameBuffMgr::getAtkBuffAddTime		(const _stBuffModel* pBuffModel)
{// 增加 技能增加给目标的 buff持续时间 (绝对值 毫秒)
	if ( !pBuffModel )
		return 0;

	int32 nMS = 0;
	POOL_BUFF::iterator _pos;
	m_poolBuff.getHead(_pos);
	while (!m_poolBuff.isTail(_pos))
	{
		CGameBuff*	pBuff = m_poolBuff.getNext(_pos);
		if (!pBuff || pBuff->isDelete())
			continue;

		const _stBuffModel* pModel = pBuff->getBuffModel();
		if (!pModel)
			continue;

		if (BuffType_AddSkillBuffTime != pModel->uBuffType )
			continue;

		if ( pBuffModel->uGroupID != pModel->stBuffAddSkillBuffTime.uBuffGroupID )
			continue;

		nMS += pModel->stBuffAddSkillBuffTime.nAddBuffMS;
	}
	return nMS;
}
//-------------------------------------------------------------
//------------------------------
uint32	CGameBuffMgr::getBuffTime				(const _stBuffModel* pBuffModel)
{// 减少附加的自己身上的buff时间 (万分比)
	if ( !pBuffModel )
		return 0;

	float32 fExtra = 0.f;
	POOL_BUFF::iterator _pos;
	m_poolBuff.getHead(_pos);
	while (!m_poolBuff.isTail(_pos))
	{
		CGameBuff*	pBuff = m_poolBuff.getNext(_pos);
		if (!pBuff || pBuff->isDelete())
			continue;

		const _stBuffModel* pModel = pBuff->getBuffModel();
		if (!pModel)
			continue;

		if (BuffType_AddBuffTime != pModel->uBuffType )
			continue;

		if ( pBuffModel->uGroupID != pModel->stBuffAddBuffTime.uBuffGroupID )
			continue;

		fExtra += pModel->stBuffAddBuffTime.fEffetRadio;
	}

	if ( fExtra <= -1.0f )
	{
		fExtra = -0.9f;
	}

	return pBuffModel->uContinueTime +  (uint32)(pBuffModel->uContinueTime*fExtra);
}
//-------------------------------------------------------------
//------------------------------ 增加治疗效果
float32	CGameBuffMgr::getAddCureRate	()
{
	if ( !m_pOwner || m_pOwner->getHP() <= 0 || m_pOwner->getHPMax() <= 0 )
		return 0.0f;
	
	float32 fExtra = 0.f;
	POOL_BUFF::iterator _pos;
	m_poolBuff.getHead(_pos);
	while (!m_poolBuff.isTail(_pos))
	{
		CGameBuff*	pBuff = m_poolBuff.getNext(_pos);
		if (!pBuff || pBuff->isDelete() )
			continue;

		const _stBuffModel* pModel = pBuff->getBuffModel();
		if (!pModel)
			continue;

		if (BuffType_AddCureEffet != pModel->uBuffType )
			continue;

		if ( pModel->stBuffAddCureEffet.fHPRadio >= 0.00001 )
		{
			float32 fHPRadio = (float32)m_pOwner->getHP()/(float32)m_pOwner->getHPMax();
			if ( pModel->stBuffAddCureEffet.bGreater && fHPRadio < pModel->stBuffAddCureEffet.fHPRadio )
			{
				continue;
			}
			else if ( fHPRadio > pModel->stBuffAddCureEffet.fHPRadio )
			{
				continue;
			}
		}

		fExtra += pModel->stBuffAddCureEffet.fEffetRadio;
	}

	return fExtra;
}
//-------------------------------------------------------------
//------------------------------ 吸血百分比
float32	CGameBuffMgr::getSuckHPRate	(bool bCritical)
{
	if ( !m_pOwner )
		return 0.0f;

	float32 fExtra = 0.f;
	POOL_BUFF::iterator _pos;
	m_poolBuff.getHead(_pos);
	while (!m_poolBuff.isTail(_pos))
	{
		CGameBuff*	pBuff = m_poolBuff.getNext(_pos);
		if (!pBuff || pBuff->isDelete() )
			continue;

		const _stBuffModel* pModel = pBuff->getBuffModel();
		if (!pModel)
			continue;

		if (BuffType_SuckHP != pModel->uBuffType )
			continue;

		if ( pModel->stBuffSuckHP.bNeedCritical && !bCritical )
			continue;

		fExtra += pModel->stBuffSuckHP.fEffetRadio;
	}

	return fExtra;
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameBuffMgr::getAddTargetHarm	(int32& nAddValue,float32& fAddRate)
{
	nAddValue	= 0;
	fAddRate	= 0.f;
	if ( !m_pOwner || m_pOwner->getHP() <= 0 || m_pOwner->getHPMax() <= 0 )
		return ;

	float32 fExtra = 0.f;
	POOL_BUFF::iterator _pos;
	m_poolBuff.getHead(_pos);
	while (!m_poolBuff.isTail(_pos))
	{
		CGameBuff*	pBuff = m_poolBuff.getNext(_pos);
		if (!pBuff || pBuff->isDelete() )
			continue;

		const _stBuffModel* pModel = pBuff->getBuffModel();
		if (!pModel || pModel->stBuffAddTargetHarm.fHPSep <= 0.0001f )
			continue;

		if (BuffType_AddTargetHarm != pModel->uBuffType )
			continue;

		float32 fCurHPRadio = (float32)m_pOwner->getHP() / (float32)m_pOwner->getHPMax();
		if ( fCurHPRadio > pModel->stBuffAddTargetHarm.fHPRadioMax - pModel->stBuffAddTargetHarm.fHPSep  )
			continue;

		if ( fCurHPRadio < pModel->stBuffAddTargetHarm.fHPRadioMin + pModel->stBuffAddTargetHarm.fHPSep  )
			continue;

		int32 nMulti	=	(int32) ((pModel->stBuffAddTargetHarm.fHPRadioMax - fCurHPRadio)/pModel->stBuffAddTargetHarm.fHPSep);
		nAddValue		+=	nMulti*pModel->stBuffAddTargetHarm.nAddHarmValue;
		fAddRate		+=	nMulti*pModel->stBuffAddTargetHarm.fAddHarmRadio;

		fExtra += pModel->stBuffAddHarmEffet.fEffetRadio;
	}
}
//-------------------------------------------------------------
//------------------------------ 增加伤害效果
float32	CGameBuffMgr::getAddHarmRate	(uint32 uSkillID/* = 0*/)
{
	if ( !m_pOwner || m_pOwner->getHP() <= 0 || m_pOwner->getHPMax() <= 0)
		return 0.0f;

	float32 fExtra = 0.f;
	POOL_BUFF::iterator _pos;
	m_poolBuff.getHead(_pos);
	while (!m_poolBuff.isTail(_pos))
	{
		CGameBuff*	pBuff = m_poolBuff.getNext(_pos);
		if (!pBuff || pBuff->isDelete() )
			continue;

		const _stBuffModel* pModel = pBuff->getBuffModel();
		if (!pModel)
			continue;

		if (BuffType_AddHarmEffet != pModel->uBuffType )
			continue;

		if (pModel->stBuffAddHarmEffet.uBeSkillID && pModel->stBuffAddHarmEffet.uBeSkillID != uSkillID)
			continue;

		if ( pModel->stBuffAddHarmEffet.fHPRadio >= 0.00001 )
		{
			float32 fHPRadio = (float32)m_pOwner->getHP()/(float32)m_pOwner->getHPMax();
			if ( pModel->stBuffAddHarmEffet.bGreater && fHPRadio < pModel->stBuffAddHarmEffet.fHPRadio )
			{
				continue;
			}
			else if ( fHPRadio > pModel->stBuffAddHarmEffet.fHPRadio )
			{
				continue;
			}
		}

		fExtra += pModel->stBuffAddHarmEffet.fEffetRadio;
	}

	return fExtra;
}
//-------------------------------------------------------------
//------------------------------ 增加伤害效果
float32	CGameBuffMgr::getMonsterExtraDamage	()
{
	if ( !m_pOwner  )
		return 0.0f;

	float32 fExtra = 0.f;
	POOL_BUFF::iterator _pos;
	m_poolBuff.getHead(_pos);
	while (!m_poolBuff.isTail(_pos))
	{
		CGameBuff*	pBuff = m_poolBuff.getNext(_pos);
		if (!pBuff || pBuff->isDelete() )
			continue;

		const _stBuffModel* pModel = pBuff->getBuffModel();
		if (!pModel)
			continue;

		if (BuffType_AddHarmMonster != pModel->uBuffType )
			continue;

		fExtra += pModel->fBuffAddHarmMonster;
;
	}

	return fExtra;
}
//-------------------------------------------------------------
//------------------------------
bool	CGameBuffMgr::checkCriticalConditon	(const _stBuffAddCritical& stConditon)
{
	if ( !m_pOwner )
		return false;

	bool bCheckOK = true;
	switch ( stConditon.uConditionType )
	{
	case BuffCriticalConditionType_HPPercent:
		{
			if ( m_pOwner->getHP() <= 0 || m_pOwner->getHPMax() <= 0 )
				return false;

			float32 fCurHPRadio = (float32)m_pOwner->getHP()/(float32)m_pOwner->getHPMax();

			if ( stConditon.nParam > 0 )
			{
				bCheckOK = ( fCurHPRadio >= (float32)stConditon.nParam/10000.f ) ;
			}
			else
			{
				bCheckOK = ( fCurHPRadio <= (float32)(stConditon.nParam*-1)/10000.f ) ;
			}
		}
		break;
	case BuffCriticalConditionType_BuffGroup:
		{
			POOL_BUFF::iterator _pos;
			m_poolBuff.getHead(_pos);
			while (!m_poolBuff.isTail(_pos))
			{
				CGameBuff*	pBuff = m_poolBuff.getNext(_pos);
				if (!pBuff || pBuff->isDelete() )
					continue;

				const _stBuffModel* pModel = pBuff->getBuffModel();
				if (!pModel)
					continue;

				if ( pModel->uGroupID == (uint16)stConditon.nParam )
				{
					bCheckOK = true;
					break;
				}
			}
		}
		break;
	default:
		break;
	}

	return bCheckOK;
}
//-------------------------------------------------------------
//------------------------------增加暴击概率
float32	CGameBuffMgr::getAddCriticalRate		(CSceneUnit* pTarget)
{
	if ( !m_pOwner || !pTarget || m_pOwner->getHP() <= 0 || m_pOwner->getHPMax() <= 0 )
		return 0.0f;

	float32 fExtra = 0.f;
	POOL_BUFF::iterator _pos;
	m_poolBuff.getHead(_pos);
	while (!m_poolBuff.isTail(_pos))
	{
		CGameBuff*	pBuff = m_poolBuff.getNext(_pos);
		if (!pBuff || pBuff->isDelete() )
			continue;

		const _stBuffModel* pModel = pBuff->getBuffModel();
		if (!pModel)
			continue;

		if (BuffType_AddCriticalEffet != pModel->uBuffType )
			continue;

		if ( checkCriticalConditon(pModel->stBuffAddCritical) )
		{
			fExtra += pModel->stBuffAddCritical.fEffetRadio;
		}
	}
	return fExtra;
}
//-------------------------------------------------------------
//------------------------------ 获取技能额外伤害值
uint32	CGameBuffMgr::getExtraDamage	(uint32 uSourceID,uint32 uSkillID,int32 iDamage)
{
	if (iDamage <= 0)
		return 0;

	uint32 uExtra = 0;
	POOL_BUFF::iterator _pos;
	m_poolBuff.getHead(_pos);
	while (!m_poolBuff.isTail(_pos))
	{
		CGameBuff*	pBuff = m_poolBuff.getNext(_pos);
		if (!pBuff || pBuff->isDelete())
			continue;

		const _stBuffModel* pModel = pBuff->getBuffModel();
		if (!pModel || pBuff->getSourceID() != uSourceID || BuffType_SkillExtraDamage != pModel->uBuffType)
			continue;
		
		//技能ID为0，所有的伤害都加成
		if (!pModel->stSkillExtraDamage.uSkillID || uSkillID == pModel->stSkillExtraDamage.uSkillID)
		{
			uExtra += pBuff->getExtraDamage(iDamage);
		}
	}

	return uExtra;
}
//-------------------------------------------------------------
//------------------------------ 获取减少技能消耗万分比
int32	CGameBuffMgr::getReduceSkillExpendRate	()
{
	int32 nReduce = 0;
	POOL_BUFF::iterator _pos;
	m_poolBuff.getHead(_pos);
	while (!m_poolBuff.isTail(_pos))
	{
		CGameBuff*	pBuff = m_poolBuff.getNext(_pos);
		if (!pBuff || pBuff->isDelete() )
			continue;

		const _stBuffModel* pModel = pBuff->getBuffModel();
		if (!pModel)
			continue;

		if (BuffType_SkillExpendReduce == pModel->uBuffType)
		{
			nReduce += pModel->nReduceSkillExpend;
		}
	}
	return MIN(nReduce,MAX_PROBABILITY);
}
//-------------------------------------------------------------
//------------------------------ 获取技能冷却时间
uint32	CGameBuffMgr::getSkillTimeByTime(uint8 uSkillTimeType,uint32 uSkillID,uint32 uCoolTick)
{
	if (!uCoolTick || !uSkillTimeType || uSkillTimeType >= _emSkillTimeType_ARRAYSIZE)
		return uCoolTick;

	POOL_BUFF::iterator _pos;
	m_poolBuff.getHead(_pos);
	while (!m_poolBuff.isTail(_pos))
	{
		CGameBuff*	pBuff = m_poolBuff.getNext(_pos);
		if (!pBuff || pBuff->isDelete() )
			continue;

		const _stBuffModel* pModel = pBuff->getBuffModel();
		if (!pModel || pModel->uBuffType != BuffType_ChangeSkillTime || pModel->mapSkillCoolTime.empty())
			continue;

		_stBuffModel::MAP_SKILL_COOLTIME::const_iterator _spos = pModel->mapSkillCoolTime.begin();
		for (; _spos != pModel->mapSkillCoolTime.end(); ++_spos)
		{
			const _stBuffSkillCoolTime& stCoolTime = _spos->second;
			if (stCoolTime.uSkillTimeType != uSkillTimeType)
				continue;

			if (stCoolTime.uSkillID && stCoolTime.uSkillID != uSkillID)
				continue;

			uCoolTick -= uint32((uint64)uCoolTick * stCoolTime.uRadio / MAX_PROBABILITY);
		}
	}
	return uCoolTick;
}
//-------------------------------------------------------------
//------------------------------ 
void	CGameBuffMgr::updatePassiveSkillBuff()
{
	if ( !m_pOwner )
		return ;

	{// 删除老的buff
		POOL_BUFF::iterator _pos;
		m_poolBuff.getHead(_pos);
		while (!m_poolBuff.isTail(_pos))
		{
			CGameBuff*	pBuff = m_poolBuff.getNext(_pos);
			if (!pBuff || !pBuff->getBuffModel() || pBuff->getBuffModel()->uBuffCat != BuffCat_PassiveSkill )
				continue;

			pBuff->setDelete(true);
		}
	}

	{//增加buff
		CGameSkillMgr::VEC_SKILL&	vecSkill = m_pOwner->getGameSkillMgr().m_vecSkill[SkillUseType_Passive];
		CGameSkillMgr::VEC_SKILL::iterator _pos1 = vecSkill.begin();
		CGameSkillMgr::VEC_SKILL::iterator _end1 = vecSkill.end();
		for ( ; _pos1 != _end1 ; ++_pos1 )
		{
			CGameSkill* pSkill = *_pos1;
			if ( !pSkill )
				continue;

			const _stSkillModel* pSkillModel = pSkill->getSkillModel();
			if ( !pSkillModel)
				continue;

			for (uint32 k = 0; k < (uint32)pSkillModel->vecEffectModel.size(); ++k)
			{
				const _stSkillEffectModel* pEffectModel = pSkillModel->vecEffectModel[k];
				if ( !pEffectModel )
					continue;

				_stSkillEffectModel::DMAP_SKILLADDBUFF::const_iterator _pos2 = pEffectModel->dmapAddBuff.begin();
				_stSkillEffectModel::DMAP_SKILLADDBUFF::const_iterator  _end2 = pEffectModel->dmapAddBuff.end();
				for ( ; _pos2 != _end2 ; ++_pos2 )
				{
					const _stSkillEffectModel::DMAP_SKILLADDBUFF::_sub_map& submap = _pos2->second;
					_stSkillEffectModel::DMAP_SKILLADDBUFF::_sub_map::const_iterator _spos = submap.begin();
					for (; _spos != submap.end(); ++_spos)
					{
						addBuff(_spos->second.uBuffID,pSkill->getLevel(),m_pOwner->getUnitID(),pSkillModel->uSkillID, DoingType_Buff);
					}
				}
			}
		}
	}
}
//-------------------------------------------------------------
//------------------------------ 
uint32	CGameBuffMgr::getStateBuffAttr	()
{
	uint32 uMark = 0;
	POOL_BUFF::iterator _pos;
	m_poolBuff.getHead(_pos);
	while (!m_poolBuff.isTail(_pos))
	{
		CGameBuff*	pBuff = m_poolBuff.getNext(_pos);
		if (!pBuff || pBuff->isDelete() )
			continue;

		const _stBuffModel* pModel = pBuff->getBuffModel();
		if (!pModel || !pModel->uStateType)
			continue;

		uMark |= pModel->uStateBuffAttrMark;
	}

	return uMark;
}