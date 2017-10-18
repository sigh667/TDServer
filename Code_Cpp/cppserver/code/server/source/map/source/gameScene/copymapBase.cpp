/*----------------- copymapBase.cpp
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.0
* Date         : 2016/5/11 18:21:43
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "gameScene/copymapBase.h"
#include "basic/systemFunction.h"
#include "gameScene/gameSceneCell.h"
#include "gameScene/gameSceneMgr.h"
#include "gameLogic/playerCopymap/playerCopymap.h"
//#include "gameLogic/playerTeam/playerTeam.h"
//#include "gameLogic/playerPrize/playerPrize.h"
//#include "gameLogic/playerIdentity/playerIdentity.h"
//#include "gameLogic/playerActivity/playerActivity.h"
//#include "gameLogic/playerColletion/playerColletion.h"
#include "protocol/WAGMsg.pb.h"
#include "protocol/CASMsg.pb.h"
#include "protocol/GameDefine.pb.h"
#include "protocol/CASProtocol.pb.h"
#include "protocol/PvpWar.pb.h"
#include "main/gameServer.h"
#include "cfg_data/monsterData/monsterData.h"
#include "cfg_data/trapData/trapData.h"
#include "gameLogic/gameMail/gameMail.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
CCopymapBase::CCopymapBase()
{
	PBCreateCellParam pbDefault;
	pbDefault.Clear();
	restart(NULL, pbDefault);
}

//-------------------------------------------------------------
//------------------------------ 
CCopymapBase::~CCopymapBase()
{

}
//-------------------------------------------------------------
//------------------------------ 
void	CCopymapBase::initialize()
{
	m_pSceneCell		= NULL;
	m_mapCopymapPlayer.clear();
	m_eCopymapState		= CopymapState_Ing;
	m_uCompleteTime		= 0;
	m_uStartTime		= 0;
	m_uRefreshWaveTime	= 0;
	m_uCurWaveID		= 0;
	m_uCloseTime		= 0;
	m_pCopymapModel		= NULL;
	m_uCreateNpc		= 0;
	m_pbCreator.Clear();
	m_uCash				= 0;
	m_bCashCopyMap		= false;
	m_setDropUnit.clear();
}
//-------------------------------------------------------------
//------------------------------ 
void	CCopymapBase::restart	(CGameSceneCell*pScene,const PBCreateCellParam&pbParam)
{
	m_pSceneCell		= pScene;
	m_mapCopymapPlayer.clear();
	m_setDropUnit.clear();
	m_eCopymapState		= CopymapState_Ing;
	m_uCompleteTime		= 0;
	m_uStartTime		= 0;
	m_uCurWaveID		= 0;
	m_uRefreshWaveTime	= 0;
	m_uCloseTime		= 0;
	m_uReadyStartTime	= 0;
	m_pCopymapModel		= NULL;
	m_uCreateNpc		= pbParam.npcid();
	m_uCreateLevel		= pbParam.level();
	m_pbCreator.CopyFrom(pbParam.display());
	m_uCash				= pbParam.cash();
	
	m_bCashCopyMap = m_uCash > 0 ? true: false;

	if ( pScene )
	{
		m_pCopymapModel		= pScene->getCopymapModel();
	}
	if(m_pCopymapModel)
	{
		uint32 uCurrTime= (uint32)getTime();
		m_uStartTime	= uCurrTime;
		m_uRefreshWaveTime = uCurrTime;
		m_uCloseTime	= uCurrTime + m_pCopymapModel->uSurvivalTime;
		m_uReadyStartTime	= m_pCopymapModel->uReadyTime > 0 ? uCurrTime + m_pCopymapModel->uReadyTime + 3 : 0;
	}

	startCopymap();
}
//-------------------------------------------------------------
//------------------------------
bool	CCopymapBase::isPrefect			()
{
	if (!canOperate())
		return false;

	if (m_pCopymapModel->mapPrefectCondition.empty())
		return false;

	CONST_ITER_MAP_U32_U32 _pos = m_pCopymapModel->mapPrefectCondition.begin();
	for (; _pos != m_pCopymapModel->mapPrefectCondition.end(); ++_pos)
	{
		if (CopymapPrefectType_Time == _pos->first)
		{
			if (m_uCompleteTime > _pos->second)
				return false;
		}
		else if (CopymapPrefectType_DeadNum == _pos->first)
		{
			if (getDeadNum() > _pos->second)
				return false;
		}
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
void	CCopymapBase::close				()
{
	if ( !canOperate() )
		return ;

	m_eCopymapState = CopymapState_Closing;
	synState2World();
	// 场景中的玩家踢出副本
	HASH_PLAYER::iterator _pos = m_pSceneCell->player_begin();
	HASH_PLAYER::iterator _end = m_pSceneCell->player_end();
	for ( ; _pos != _end ; ++_pos )
	{
		CScenePlayer* pPlayer = _pos->second;
		if ( !pPlayer )
			continue;

		CPlayerCopymap* pCopymap = (CPlayerCopymap*)pPlayer->getPlayerFun(GameSystemType_Copymap);
		if (!pCopymap)
			continue;

		pCopymap->leaveCopymap();
	}
	// 关闭场景 
	g_pGameSceneMgr->closeSceneCell(m_pSceneCell->getSceneID(),m_pSceneCell->getDynID());
}
//-------------------------------------------------------------
//------------------------------ 角色进入场景
void	CCopymapBase::playerEnter		(CScenePlayer*pPlayer)
{
	if ( !canOperate() || !pPlayer)
		return ;

	// 清除采集计数
	//CPlayerColletion* pColletionFun = (CPlayerColletion*)(pPlayer->getPlayerFun(GameSystemType_Colletion));
	//pColletionFun->cleanCopymapCollCount();
	//给技能
	this->giveSkill(pPlayer,true,DoingType_Copymap_Enter);

	//记录玩家信息
	_stCopymapPlayer* pCopymapPlayer = m_mapCopymapPlayer.find_(pPlayer->getPlayerID());
	if (!pCopymapPlayer)
	{
		_stCopymapPlayer stCopymapPlayer;
		stCopymapPlayer.initialize();
		stCopymapPlayer.stDisplay = pPlayer->getPlayerBase();
		stCopymapPlayer.emEnterReason = CopymapEnterReason_Normal;
		stCopymapPlayer.pPlayer = pPlayer;
		m_mapCopymapPlayer.insert_(pPlayer->getPlayerID(),stCopymapPlayer);
		pCopymapPlayer = &stCopymapPlayer;
	}
	pCopymapPlayer->emCopymapPlayerState = CopymapPlayerState_Online;
	pCopymapPlayer->pPlayer = pPlayer;

	// 赏金团的话
	if (m_bCashCopyMap && pPlayer->getPlayerID()!=m_pbCreator.playerid())
	{
		pCopymapPlayer->bNoPrize = true;
	}
}
//-------------------------------------------------------------
//------------------------------ 角色重连
void	CCopymapBase::playerReconnect		(CScenePlayer*pPlayer)
{
	if ( !canOperate() || !pPlayer)
		return ;

	//给技能
	this->giveSkill(pPlayer,true,DoingType_Copymap_Enter);

	//记录玩家信息
	_stCopymapPlayer* pCopymapPlayer = m_mapCopymapPlayer.find_(pPlayer->getPlayerID());
	if (pCopymapPlayer)
		pCopymapPlayer->emCopymapPlayerState = CopymapPlayerState_Online;
}
//-------------------------------------------------------------
//------------------------------ 角色离开场景
void	CCopymapBase::playerLeave		(CScenePlayer*pPlayer)
{
	if (!pPlayer)
		return ;

	//给技能
	this->giveSkill(pPlayer,false,DoingType_Copymap_Exit);
	_stCopymapPlayer* pCopymapPlayer = m_mapCopymapPlayer.find_(pPlayer->getPlayerID());
	if (pCopymapPlayer)
	{
		pCopymapPlayer->emCopymapPlayerState = CopymapPlayerState_Exit;
		pCopymapPlayer->stDisplay = pPlayer->getPlayerBase().getDisplay();
	}

	// 赏金团补发掉落物品 
	if (pPlayer->getPlayerID() == m_pbCreator.playerid())
		sceneItemSendMail();
	
}
//-------------------------------------------------------------
//------------------------------ 角色上线处理
void	CCopymapBase::playerOnline	(CScenePlayer*pPlayer)
{
	if ( !pPlayer )
		return ;

	//第一个进入地图后 关闭时间的重置一下 不然时间相差太大了
	_stCopymapPlayer* pCopymapPlayer = m_mapCopymapPlayer.find_(pPlayer->getPlayerID());
	if (pCopymapPlayer&&!pCopymapPlayer->bAlreadyOnline)
	{
		onFirstOnline(pPlayer);
		pCopymapPlayer->bAlreadyOnline = true;
		pCopymapPlayer->pPlayer = pPlayer;
	}

	{// 自己的进入方式 
		bool bAssist = ( getEnterType(pPlayer->getPlayerID()) == CopymapEnterReason_Assist );
		PBG2CCopymapEnterInfo msg;
		msg.set_assist(bAssist);
		pPlayer->sendMsg(P_S2C_Protocol_Copymap,S2C_Copymap_EnterInfo,&msg);
	}

	{//  副本状态
		PBG2CCopymapState msg;
		msg.set_state(m_eCopymapState);
		msg.set_endtime((uint32)m_uCloseTime);
		msg.set_completetime(m_uCompleteTime);
		msg.set_completetype(m_emCopymapCompleteType);
		pPlayer->sendMsg(P_S2C_Protocol_Copymap,S2C_Copymap_State,&msg);
	}

	
}
//-------------------------------------------------------------
//------------------------------ 
void	CCopymapBase::callback_trapLeave	(CSceneTrap*pTrap)
{
}

//-------------------------------------------------------------
//------------------------------AI关闭副本
void CCopymapBase::callback_AICloseCopymap(uint32 uTime, uint32 uChatid)
{
	m_uCloseTime = uTime;
	PBG2CCopymapCloseTip ask;
	ask.set_chatid(uChatid);
	ask.set_time(uTime);
	auto _pos = m_mapCopymapPlayer.begin();
	for ( ; _pos != m_mapCopymapPlayer.end() ; ++_pos )
	{
		CScenePlayer* pPlayer = _pos->second.pPlayer;
		if (!pPlayer)
			continue;
		pPlayer->sendMsg(P_S2C_Protocol_Copymap,S2C_Copymap_CloseTip,&ask);
	}
}

//-------------------------------------------------------------
//------------------------------
void CCopymapBase::callback_callAllPlayer(CSceneMonster* pMonster)
{
	if (!pMonster)
		return ;
	_stMonsterRegion *pRegion = pMonster->getMonsterRegion();
	if (!pRegion)
		return;

	auto _pos = pRegion->vecMonster.begin();
	auto _end = pRegion->vecMonster.end();
	
	for (; _pos!=_end; ++_pos)
	{
		if (pMonster->getMonsterID() == _pos->uMonsterID)
			break;
	}

	if (_pos->uAreaID)
	{
		auto _pos1 = m_mapCopymapPlayer.begin();
		for ( ; _pos1 != m_mapCopymapPlayer.end() ; ++_pos1 )
		{
			CScenePlayer* pPlayer = _pos1->second.pPlayer;
			if (!pPlayer)
				continue;
			
			_stPoint2F stOut(pRegion->fPosX,pRegion->fPosY);
			if (_pos->fPosX != 0 || _pos->fPosY != 0)
				stOut.set(_pos->fPosX, _pos->fPosY);
	
			m_pSceneCell->randGetPoint(stOut,pRegion->fPosX,pRegion->fPosY,pRegion->fRadius);
			
			// 清除轻功状态
			if (pPlayer->haveState(StateType_Qinggong))
				pPlayer->delState(StateType_Qinggong);

			pPlayer->setPosition(stOut, SetPositionReason_AI, true);

			m_pSceneCell->addArea(_pos->uAreaID);
		}
	}
}


//-------------------------------------------------------------
//------------------------------ 
void	CCopymapBase::callback_player_gethurt	(CScenePlayer* pPlayer, uint32 uHurt,CScenePlayer* pPlayerSource)
{

}
//-------------------------------------------------------------
//------------------------------
_emCopymapEnterReason	CCopymapBase::getEnterType	(uint32 uPlayerID)	const
{
	const _stCopymapPlayer* pCopymapPlayer = m_mapCopymapPlayer.find_(uPlayerID);
	if ( !pCopymapPlayer )
		return CopymapEnterReason_Normal;

	return pCopymapPlayer->emEnterReason;
}
//-------------------------------------------------------------
//------------------------------
void	CCopymapBase::updatePlayerInfo	(PBW2GCopymapPlayerInfo&msg)
{
	const PBPlayerDisplay& pbDisplay = msg.display();
	_stCopymapPlayer& stPlayer	= m_mapCopymapPlayer[pbDisplay.playerid()];
	stPlayer.stDisplay.loadFromProtobuf(pbDisplay);
	stPlayer.emEnterReason		= (_emCopymapEnterReason)msg.reason();
}
//-------------------------------------------------------------
//------------------------------ 
void	CCopymapBase::process	(uint64 uTick,uint64 uTime,uint32 uSecond)
{
	if ( !canOperate() )
		return ;

	switch (m_eCopymapState)
	{
	case CopymapState_Ing:
		{
			process_ing(uTick,(uint32)uTime,uSecond);
		}
		break;
	case CopymapState_Done:
		{
			process_done(uTick,(uint32)uTime,uSecond);
		}
		break;
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CCopymapBase::synState2World		()
{
	PBG2WCopymapUpdateState  msg;
	msg.set_sceneid(m_pSceneCell->getSceneID());
	msg.set_dynid(m_pSceneCell->getDynID());
	msg.set_state(m_eCopymapState);

	g_pServer->sendMsgToWorld(g_pServer->getLogicWorldID(),P_G2W_Protocol_Copymap, G2W_Copymap_UpdateState, &msg);
}
//-------------------------------------------------------------
//------------------------------ 
void	CCopymapBase::giveCach			()
{
	// 赏金团
	if (!m_bCashCopyMap || m_uCash <= 0)
		return ;

	uint32 uCashCreatePlayerID	= m_pbCreator.playerid();
	uint32 uCashPlayerCount		= 0;
	auto _pos = m_mapCopymapPlayer.begin();
	for ( ; _pos != m_mapCopymapPlayer.end() ; ++_pos )
	{
		//_stCopymapPlayer& stCopymapPlayer = _pos->second;
		CScenePlayer* pPlayer = _pos->second.pPlayer;
		if (!pPlayer)
			continue;

		if (uCashCreatePlayerID==pPlayer->getPlayerID())
			continue;

		++uCashPlayerCount;
	}
	if ( uCashPlayerCount <= 0 )
		return ;
	uint32 uGetCashCount = (uint32)(m_uCash/uCashPlayerCount);

	_pos = m_mapCopymapPlayer.begin();
	for ( ; _pos != m_mapCopymapPlayer.end() ; ++_pos )
	{
		//_stCopymapPlayer& stCopymapPlayer = _pos->second;
		CScenePlayer* pPlayer = _pos->second.pPlayer;
		if (!pPlayer)
			continue;
		if (uCashCreatePlayerID == pPlayer->getPlayerID())
			continue;

		PBMail stMail;
		stMail.set_playerid(pPlayer->getPlayerID()); 
		stMail.set_worldid(pPlayer->getLogicWorldID()); 
		stMail.set_type(MailType_CashReward); 
		stMail.set_sendtime((uint64)getTime());
		if ( uGetCashCount )
		{
			PBMoney* money = stMail.add_money();
			if ( money )
			{
				money->set_moneytype(MoneyType_BindCash);
				money->set_money(uGetCashCount);
			}
		}

		PBMailCopymapCashReward* pbInfo = stMail.mutable_copymapcash();
		if (pbInfo)
		{
			PBPlayerDisplay* pbDisplay = pbInfo->mutable_leader();
			if (pbDisplay)
			{
				pbDisplay->CopyFrom(m_pbCreator);
			}
			pbInfo->set_copymapid(getCopymapID());
			pbInfo->set_cash(uGetCashCount);
		}

		CPlayerMail::sendMail(stMail);
	}
	m_uCash = 0;
}

//-------------------------------------------------------------
//------------------------------
void CCopymapBase::sceneItemSendMail()
{
	// 赏金团
	if ( !m_bCashCopyMap )
		return ;
	
	auto _pos = m_setDropUnit.begin();
	auto _end = m_setDropUnit.end();

	for (; _pos != _end; ++_pos)
	{
		CSceneItem* pSceneItem = *_pos;
		if (pSceneItem  && pSceneItem->isCashPlayer())
		{
			_stCopymapPlayer *pCopymap = getCopymapPlayer(m_pbCreator.playerid());
			if (!pCopymap)
				continue;
			if (pCopymap->pPlayer)
				pSceneItem->awardItemBySendMail(pCopymap->pPlayer, m_pbCreator.worldid());
		}
	}
}

//-------------------------------------------------------------
//------------------------------ 
void	CCopymapBase::on_copymap_complete		(uint32 uTime,_emCopymapCompleteReson emReason )
{
	if ( !canOperate() || m_eCopymapState != CopymapState_Ing)
		return ;

	//清理怪物
	m_pSceneCell->destroyMonster();

	m_eCopymapState = CopymapState_Done;
	m_uCompleteTime		= uint32((uTime > m_uStartTime) ? (uTime - m_uStartTime) : 0);
	set_closetime(uTime); 
	if (emReason != CopymapCompleteReson_Success)
	{
		m_emCopymapCompleteType = CopymapCompleteType_Failed;
	}
	else
	{
		m_emCopymapCompleteType = CopymapCompleteType_Success;
		//是否完美
		if (isPrefect())
		{
			m_emCopymapCompleteType = CopymapCompleteType_Prefect;
		}
	}

	synState2World();

	PBG2CCopymapState msg;
	msg.set_state(m_eCopymapState);
	msg.set_endtime(m_uCloseTime);
	msg.set_completetime(m_uCompleteTime);
	msg.set_completetype(m_emCopymapCompleteType);

	CScenePlayer* pLeader = NULL;
	MAP_PLAYER::iterator _pos = m_mapCopymapPlayer.begin();
	for ( ; _pos != m_mapCopymapPlayer.end() ; ++_pos )
	{
		_stCopymapPlayer& stCopymapPlayer = _pos->second;
		CScenePlayer* pPlayer = _pos->second.pPlayer;
		if (!pPlayer)
			continue;

		if (CopymapPlayerState_Exit == stCopymapPlayer.emCopymapPlayerState)
			continue;

		msg.clear_prizeid();
		msg.clear_itemnum();
		pLeader = pPlayer;
		//奖励
		//if (pPlayer->getPlayerPrize())
		//{
		//	CPrizeData::VEC_PRIZE_MODEL vecPrizeModel;
		//	_emCopymapCompleteType emTmpType = m_emCopymapCompleteType;
		//	if (stCopymapPlayer.emCopymapCompleteType != CopymapCompleteType_None)
		//		emTmpType = stCopymapPlayer.emCopymapCompleteType;

		//	//是否有奖励
		//	if (!stCopymapPlayer.bNoPrize)
		//	{
		//		pPlayer->getPlayerPrize()->getCopymapCompletePrize(vecPrizeModel,m_pCopymapModel->uCopymapID,emTmpType,getEnterType(pPlayer->getPlayerID()),getExtraNum());
		//	}
		//	for (uint32 i = 0; i < vecPrizeModel.size(); ++i)
		//	{
		//		if (vecPrizeModel[i])
		//			msg.add_prizeid(vecPrizeModel[i]->uPrizeID);
		//	}
		//	pPlayer->getPlayerPrize()->addPrize(vecPrizeModel);

		//	//身份奖励 必须成功
		//	if (m_emCopymapCompleteType >= CopymapCompleteType_Failed )
		//	{
		//		MAP_U32_U16 mapIdentitiyItem;
		//		if (pPlayer->getPlayerIdentity())
		//			pPlayer->getPlayerIdentity()->getExtrItem(m_pCopymapModel->uMainType,mapIdentitiyItem);

		//		for (ITER_MAP_U32_U16 _tmppos = mapIdentitiyItem.begin(); _tmppos != mapIdentitiyItem.end(); ++_tmppos)
		//		{
		//			pPlayer->getGameItemMgr().addItem(_tmppos->first,_tmppos->second,false,true,true,DoingType_Copymap_Finish);
		//			PBItemNum* pItemNum = msg.add_itemnum();
		//			if (!pItemNum)
		//				continue;
		//			pItemNum->set_itemid(_tmppos->first);
		//			pItemNum->set_itemnum(_tmppos->second);
		//		}
		//	}

		//	CPlayerTeam* pPlayerTeam = pPlayer->getPlayerTeam();
		//	if (pPlayerTeam)
		//	{
		//		if (pPlayerTeam->isLeader())
		//			pLeader = pPlayer;

		//		pPlayerTeam->on_copymap_complete(m_pCopymapModel, m_emCopymapCompleteType);
		//	}
		//}

		//CPlayerActivity * pActivity = pPlayer->getPlayerActivity();
		//if (pActivity)
		//	pActivity->callback_copymap((uint32)m_pCopymapModel->uCopymapID,m_pCopymapModel->uMainType, m_pCopymapModel->uType);


		// 赏金团 分配赏金
		giveCach();
		
		//通知客户端结算面板
		pPlayer->sendMsg(P_S2C_Protocol_Copymap,S2C_Copymap_State,&msg);
		CPlayerCopymap* pCopymap  = (CPlayerCopymap*)pPlayer->getPlayerFun(GameSystemType_Copymap);
		if ( pCopymap )
			pCopymap->on_copymap_complete(m_pCopymapModel,m_emCopymapCompleteType,getEnterType(pPlayer->getPlayerID()),getExtraNum());

		//后续副本
		if (m_emCopymapCompleteType >= CopymapCompleteType_Success && pLeader && m_pCopymapModel->uNextCopymapID && ::randRatio(m_pCopymapModel->uNextCopymapIDRate) && pLeader->getPlayerCopymap())
		{
			uint8 uCode = 0;
			pLeader->getPlayerCopymap()->copymap_create(m_pCopymapModel->uNextCopymapID,m_uCreateNpc,uCode);
		}
	}
}
//-------------------------------------------------------------
//------------------------------设置关闭时间
void	CCopymapBase::set_closetime		(uint32 uTime)
{
	m_uCloseTime = uTime + m_pCopymapModel->uExtraTime;
}
//-------------------------------------------------------------
//------------------------------
void	CCopymapBase::broadCastMsg		(uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent)
{
	if ( !canOperate() )
		return ;

	HASH_PLAYER::iterator _pos = m_pSceneCell->player_begin();
	HASH_PLAYER::iterator _end = m_pSceneCell->player_end();
	for ( ; _pos != _end ; ++_pos )
	{
		CScenePlayer* pPlayer = _pos->second;
		if ( !pPlayer )
			continue;

		pPlayer->sendMsg(uMainProtocol,uProtocol,msg,uEvent);
	}
}
//-------------------------------------------------------------
//------------------------------
void	CCopymapBase::broadCastBuffer		(uint8 uMainProtocol, uint8 uProtocol, void* pBuffer,uint32 uSize, uint8 uEvent)
{
	if ( !canOperate() )
		return ;

	HASH_PLAYER::iterator _pos = m_pSceneCell->player_begin();
	HASH_PLAYER::iterator _end = m_pSceneCell->player_end();
	for ( ; _pos != _end ; ++_pos )
	{
		CScenePlayer* pPlayer = _pos->second;
		if ( !pPlayer )
			continue;

		pPlayer->sendBuffer(uMainProtocol,uProtocol,pBuffer,uSize,uEvent);
	}
}
//-------------------------------------------------------------
//------------------------------
void	CCopymapBase::process_ing			(uint64 uTick,uint32 uTime,uint32 uSecond)
{
	//波数种怪
	check_refreshWave(uTime);

	check_complete(uTime);
}
//-------------------------------------------------------------
//------------------------------
void	CCopymapBase::process_done		(uint64 uTick,uint32 uTime,uint32 uSecond)
{
	check_closeing(uTime);
}

//-------------------------------------------------------------
//------------------------------
void  CCopymapBase::clearMonsterArea(CSceneMonster* pMonster)
{
	if (!pMonster)
		return ;

	// 清除特殊区域
	_stMonsterRegion *pMonsterRegion = pMonster->getMonsterRegion();
	if (pMonsterRegion)
	{
		auto _pos = pMonsterRegion->vecMonster.begin();
		auto _end = pMonsterRegion->vecMonster.end();

		for (; _pos != _end; ++_pos)
		{
			if (_pos->uMonsterID == pMonster->getMonsterID() && _pos->uAreaID)
			{
				m_pSceneCell->delArea( _pos->uAreaID);
				break;
			}
		}
	}
}

//-------------------------------------------------------------
//------------------------------
void CCopymapBase::AddSceneItem(CSceneItem *pSceneItem)
{
	m_setDropUnit.insert(pSceneItem);
}

//-------------------------------------------------------------
//------------------------------
void	CCopymapBase::callback_killmonster(CSceneMonster* pMonster,CScenePlayer* pPlayer)
{
	if (!m_pCopymapModel || !pMonster)
		return ;
	
	clearMonsterArea(pMonster);
	if (m_pCopymapModel->uCopymapTimeType != CopymapCopymapTimeType_OneBoss)
	{
		return ;
	}
	if (m_pCopymapModel->uOneBossID != pMonster->getMonsterID())
	{
		return ;
	}
	auto _pos = m_mapCopymapPlayer.begin();
	auto _end = m_mapCopymapPlayer.end(); 
	for (; _pos != _end; ++_pos)
	{
		CScenePlayer *pPlayer = _pos->second.pPlayer;
		if (!pPlayer)
			continue;
		CPlayerCopymap* pPlayerCopymap = pPlayer->getPlayerCopymap();
		if (!pPlayerCopymap)
			continue;
	
		if ( _pos->second.emEnterReason == CopymapEnterReason_Assist) // 助战类型 赏金团
		{
			pPlayerCopymap->updateAssistCount(1);
		}
		else 
		{
			_stPlayerCopymap* pInfo = pPlayerCopymap->getInfo(getCopymapID());
			if (pInfo)
				pInfo->uEnterCount++;
		}
		
	}
}
//-------------------------------------------------------------
//------------------------------
void	CCopymapBase::callback_killplayer(CScenePlayer* pSource,CScenePlayer* pTarget)
{

}
//-------------------------------------------------------------
//------------------------------
void	CCopymapBase::callback_depotGoods(CSceneMonster* pMonster,CScenePlayer* pPlayer)
{

}

//-------------------------------------------------------------
//------------------------------
void	CCopymapBase::callback_arrestmonster(CSceneMonster* pMonster,CScenePlayer* pPlayer)
{

}
//-------------------------------------------------------------
//------------------------------
bool	CCopymapBase::unactivemonster(CSceneMonster* pMonster,CScenePlayer* pPlayer,uint8& uCode)
{
	return true;
}
//-------------------------------------------------------------
//------------------------------
void	CCopymapBase::callback_thugmonster(CSceneMonster* pMonster,CScenePlayer* pPlayer)
{

}
//-------------------------------------------------------------
//------------------------------
void	CCopymapBase::callback_npcdialog_complete(CScenePlayer* pPlayer,CSceneMonster* pNpc)
{

}
//-------------------------------------------------------------
//------------------------------
void	CCopymapBase::callback_createmonster(CSceneMonster* pMonster)
{

}
//-------------------------------------------------------------
//------------------------------

void	CCopymapBase::callback_monsterhurt	(CSceneMonster* pMonster,CSceneUnit* pSourceUnit,int32 iHurt)
{
	if (!pSourceUnit)
		return ;

	CScenePlayer* pPlayer = pSourceUnit->getOwnerPlayer();
	if (!pPlayer)
		return ;

	//记录玩家信息
	_stCopymapPlayer* pCopymapPlayer = m_mapCopymapPlayer.find_(pPlayer->getPlayerID());
	if (!pCopymapPlayer)
		return ;

	pCopymapPlayer->uHurtValue += iHurt;
	if (m_pCopymapModel&&m_pCopymapModel->uType == CopymapType_FactionBoss )
	{
		PBG2CCopymapHurtInfo msg;
		PBPlayerDisplay* display = msg.mutable_display();
		if (display)
			pCopymapPlayer->stDisplay.saveToProtobuf(*display);
		msg.set_hurtvalue(pCopymapPlayer->uHurtValue);
		if (m_pSceneCell)
			m_pSceneCell->broadcastMsg(P_S2C_Protocol_Copymap,S2C_Copymap_HurtInfo,&msg);
	}
}
//-------------------------------------------------------------
//------------------------------
void	CCopymapBase::callback_flyout			(CScenePlayer* pPlayer)
{

}
//-------------------------------------------------------------
//------------------------------
void	CCopymapBase::callback_playerdead(CScenePlayer* pPlayer,CSceneUnit* pSourceUnit)
{
	if(!pPlayer)
		return;

	_stCopymapPlayer* pCopymapPlayer = m_mapCopymapPlayer.find_(pPlayer->getPlayerID());
	if (!pCopymapPlayer)
		return;
	pCopymapPlayer->bDead = true;
}

//-------------------------------------------------------------
//------------------------------
void	CCopymapBase::callback_playerlife(CScenePlayer* pPlayer,CScenePlayer* pSource)
{
	if(!pPlayer)
		return;

	_stCopymapPlayer* pCopymapPlayer = m_mapCopymapPlayer.find_(pPlayer->getPlayerID());
	if (!pCopymapPlayer)
		return;
	pCopymapPlayer->bDead = false;
}
//-------------------------------------------------------------
//------------------------------
void	CCopymapBase::giveSkill	(CScenePlayer* pPlayer,bool bAdd,_emDoingType emDoing)
{
	if (NULL == pPlayer || m_pCopymapModel == NULL)
		return;

	{//给身份技能
		CONST_ITER_MAP_U16_I32 _pos = m_pCopymapModel->mapGiveIdentitySkill.begin();
		for (; _pos != m_pCopymapModel->mapGiveIdentitySkill.end(); ++_pos)
		{
			if (!pPlayer->haveIdentity(_pos->first))
				continue;

			if (bAdd)
			{
				pPlayer->getGameSkillMgr().addSkill(_pos->second,1,true,true,emDoing);
			}
			else
			{
				pPlayer->getGameSkillMgr().delSkill(_pos->second,false,emDoing);
			}
		}
	}
	{//给临时技能
		for (uint32 i = 0; i < m_pCopymapModel->vecGiveSkill.size(); ++i)
		{
			if (bAdd)
			{
				pPlayer->getGameSkillMgr().addSkill(m_pCopymapModel->vecGiveSkill[i],1,true,true,emDoing);
			}
			else
			{
				pPlayer->getGameSkillMgr().delSkill(m_pCopymapModel->vecGiveSkill[i],false,emDoing);
			}
		}
	}
}
//-------------------------------------------------------------
//------------------------------
void	CCopymapBase::startCopymap	()
{
	if (!m_pCopymapModel)
		return;
}

//-------------------------------------------------------------
//------------------------------ 检测副本完成
void	CCopymapBase::check_complete	(uint32 uTime)
{

}

//-------------------------------------------------------------
//------------------------------ 检测副本关闭
void	CCopymapBase::check_closeing	(uint32 uTime)
{
	if( m_eCopymapState != CopymapState_Done)
		return;

	if (!m_uCloseTime || uTime < m_uCloseTime)
		return;

	close();
}
//-------------------------------------------------------------
//------------------------------ 开始刷新波次
void	CCopymapBase::begin_wave	()
{
}
//-------------------------------------------------------------
//------------------------------ 检测刷怪
void	CCopymapBase::check_refreshWave	(uint32 uTime)
{
	if (!canOperate() || CopymapState_Ing != m_eCopymapState)
		return;

	//是否还有波数
	const _stCopymapWaveModel* pWaveModel = m_pCopymapModel->findWave(m_uCurWaveID + 1);
	if (NULL == pWaveModel)
		return;

	//检查刷新时间
	if (uTime < m_uRefreshWaveTime + pWaveModel->uWaveStepTime)
		return;

	m_uRefreshWaveTime = uTime;
	m_uCurWaveID = pWaveModel->uWaveID;
	begin_wave();

	//获取身份增加的概率
	MAP_U32_U32 mapMonsterRate;
	CScenePlayer* pLeader = getTeamLeader();
	//if (pLeader && pLeader->getPlayerIdentity())
	//{
	//	pLeader->getPlayerIdentity()->getRefreshMonsterRate(mapMonsterRate);
	//}

	//刷怪
	VEC_WAVE_UNITGROUP vecGroup;
	g_clCopymapData.findUnitGroup(pWaveModel,vecGroup,mapMonsterRate);
	for (uint32 i = 0; i < vecGroup.size(); ++i)
	{
		const _stUnitGroupModel* pGroupModel = vecGroup[i];
		if (!pGroupModel)
			continue;
		refreshUnit(pGroupModel);
	}

	//同步客户端
	//if (m_pCopymapModel->mapWaveModel.size() > 1)
		synWaveInfo();
}
//-------------------------------------------------------------
//------------------------------ 是否全部死亡
bool		CCopymapBase::refreshUnit	(const _stUnitGroupModel* pGroupModel)
{
	if (!pGroupModel)
		return false;

	//获取坐标点
	const _stPoint2F* pPoint = pGroupModel->mapBirthPostion.find_(m_uCreateNpc);
	if (!pPoint)
	{
		pPoint = pGroupModel->mapBirthPostion.find_(0);
	}

	CHECKF(pPoint != NULL);

	///刷怪
	if (GameObject_Monster == pGroupModel->uUnitType)
	{
		const _stMonsterModel*pMonsterModel = g_clMonsterData.find(pGroupModel->uUnitID);
		if (!pMonsterModel)
			return false;

		_stPoint2F stOut;
		for (uint32 i = 0; i < pGroupModel->uUnitCount; ++i)
		{
			if (m_pSceneCell->randGetPoint(stOut,pPoint->x,pPoint->y,pGroupModel->fRandDir))
			{
				m_pSceneCell->createMonster(pMonsterModel,stOut.x,stOut.y,getRandDir(),pGroupModel->uRouteID, true);
				
			}
			else
			{
				CLog::error("copymapid(%d),monsterid(%d),pos(%f,%f),curcount(%d),randdir(%f)",
					m_pCopymapModel->uCopymapID,pMonsterModel->uMonsterID,pPoint->x,pPoint->y,i,pGroupModel->fRandDir);
			}
		}
	}
	else if(GameObject_Trap == pGroupModel->uUnitType)///刷陷阱
	{
		const _stTrapModel*pTrapModel = g_clTrapData.findTrap(pGroupModel->uUnitID,1);
		if (!pTrapModel)
			return false;

		_stPoint2F stOut;
		for (uint32 i = 0; i < pGroupModel->uUnitCount; ++i)
		{
			if (m_pSceneCell->randGetPoint(stOut,pPoint->x,pPoint->y,pGroupModel->fRandDir))
			{
				CSceneTrap* pTrap = m_pSceneCell->newTrap(NULL,pTrapModel,stOut.x,stOut.y,pGroupModel->fRandDir,0,0);
				callback_newtrap(pTrap);
			}
		}
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------ 是否全部死亡
bool		CCopymapBase::isAllDead	()
{
	MAP_PLAYER::iterator _pos = m_mapCopymapPlayer.begin();
	for (; _pos != m_mapCopymapPlayer.end(); ++_pos)
	{
		_stCopymapPlayer& stCopymapPlayer = _pos->second;
		if (!stCopymapPlayer.bDead && CopymapEnterReason_Normal == stCopymapPlayer.emEnterReason)
			return false;
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------ 获取死亡玩家个数
uint32		CCopymapBase::getDeadNum	()
{
	uint32 uDeadNum = 0;
	MAP_PLAYER::iterator _pos = m_mapCopymapPlayer.begin();
	for (; _pos != m_mapCopymapPlayer.end(); ++_pos)
	{
		_stCopymapPlayer& stCopymapPlayer = _pos->second;
		if (stCopymapPlayer.bDead && CopymapEnterReason_Normal == stCopymapPlayer.emEnterReason)
			uDeadNum++;
	}
	return uDeadNum;
}
//-------------------------------------------------------------
//------------------------------ 是否全部存活
bool		CCopymapBase::isAllLife	()
{
	MAP_PLAYER::iterator _pos = m_mapCopymapPlayer.begin();
	for (; _pos != m_mapCopymapPlayer.end(); ++_pos)
	{
		_stCopymapPlayer& stCopymapPlayer = _pos->second;
		if (stCopymapPlayer.bDead && CopymapEnterReason_Normal == stCopymapPlayer.emEnterReason)
			return false;
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------ 是否全部退出
bool		CCopymapBase::isAllExit	()
{
	MAP_PLAYER::iterator _pos = m_mapCopymapPlayer.begin();
	for (; _pos != m_mapCopymapPlayer.end(); ++_pos)
	{
		_stCopymapPlayer& stCopymapPlayer = _pos->second;
		if (CopymapEnterReason_Normal != stCopymapPlayer.emEnterReason)
			continue;

		if (CopymapPlayerState_Exit != stCopymapPlayer.emCopymapPlayerState)
			return false;
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------ 是否有玩家退出
bool		CCopymapBase::isHavePlayerExit	()
{
	MAP_PLAYER::iterator _pos = m_mapCopymapPlayer.begin();
	for (; _pos != m_mapCopymapPlayer.end(); ++_pos)
	{
		_stCopymapPlayer& stCopymapPlayer = _pos->second;
		if (CopymapEnterReason_Normal != stCopymapPlayer.emEnterReason)
			continue;

		if (CopymapPlayerState_Exit == stCopymapPlayer.emCopymapPlayerState)
			return true;
	}
	return false;
}
//-------------------------------------------------------------
//------------------------------ 是否是最后一波怪
bool		CCopymapBase::isLastWave	()
{
	if (!canOperate())
		return false;

	const _stCopymapWaveModel* pWave = m_pCopymapModel->findWave(m_uCurWaveID+1);
	return pWave ? false : true;
}
//-------------------------------------------------------------
//------------------------------ 同步副本波次信息
bool		CCopymapBase::synWaveInfo	()
{
	if (!canOperate() /*|| m_pCopymapModel->mapWaveModel.size() <= 1*/)
		return false;

	PBG2CCopymapWave ack;
	ack.set_copymappid(m_pCopymapModel->uCopymapID);
	ack.set_totalwave(m_pCopymapModel->mapWaveModel.size());
	ack.set_curwave(m_uCurWaveID);

	//广播
	broadCastMsg(P_S2C_Protocol_Copymap,S2C_Copymap_SynWave,&ack);
	return true;
}

//-------------------------------------------------------------
//------------------------------ 传送至出生点
bool		CCopymapBase::transToBirthRegion	()
{
	if (!canOperate())
		return false;
	
	const _stSceneMapModel* pSceneModel = m_pSceneCell->getSceneModel();
	if (!pSceneModel)
		return false;

	HASH_PLAYER::iterator _pos = m_pSceneCell->player_begin();
	HASH_PLAYER::iterator _end = m_pSceneCell->player_end();
	for ( ; _pos != _end ; ++_pos )
	{
		CScenePlayer* pPlayer = _pos->second;
		if ( !pPlayer )
			continue;

		pPlayer->playerFlyScene_ByDynID(pSceneModel->uSceneID,m_pSceneCell->getDynID(),pSceneModel->uBirthRegionID);
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
void	CCopymapBase::onFirstOnline		(CScenePlayer*pPlayer)
{
	if (!pPlayer)
		return ;

	bool bFirstOnlinePlayer = true;
	auto _pos = m_mapCopymapPlayer.begin();
	for ( ; _pos!=m_mapCopymapPlayer.end() ; ++_pos )
	{
		if (_pos->second.bAlreadyOnline)
		{
			bFirstOnlinePlayer = false;
			break;
		}
	}

	if (bFirstOnlinePlayer)
	{
		m_uCloseTime +=(getTime() - m_uStartTime);
	}
}
//------------------------------ 
void	CCopymapBase::onClientGo		(CScenePlayer*pPlayer)
{
	if (!pPlayer)
		return ;

	{//同步开场准备时间 有时间才通知
		if (getTime() < m_uReadyStartTime)
		{
			PBG2CReadyStart msg;
			msg.set_leftscond(m_uReadyStartTime - getTime());
			pPlayer->sendMsg(P_S2C_Protocol_Copymap,S2C_Copymap_SynReadStartTime,&msg);
		}
	}
}
//-------------------------------------------------------------
//------------------------------ 
CScenePlayer*	CCopymapBase::getTeamLeader	()
{
	MAP_PLAYER::iterator _pos = m_mapCopymapPlayer.begin();
	for (; _pos != m_mapCopymapPlayer.end(); ++_pos)
	{
		CScenePlayer* pPlayer = _pos->second.pPlayer;
		if (pPlayer && pPlayer->getTeam() && pPlayer->getTeam()->uLeaderID == pPlayer->getPlayerID())
			return pPlayer;
	}

	return NULL;
}
void	CCopymapBase::_stCopymapPlayer::saveToProtobuf		(PBPvpBannerPlayerInfo* pbData)
{
	if (!pbData)
		return;
	pbData->Clear();
	if (pPlayer)
	{
		pPlayer->getPlayerBase().saveToProtobuf(*pbData->mutable_display());
	}
	else
	{
		stDisplay.saveToProtobuf(*pbData->mutable_display());
	}
	pbData->set_deadcount(uDeadCount);
	pbData->set_killcount(uKillCount);
	pbData->set_contri(uContri);
	pbData->set_assistkillcount(uAssistKillCount);
	pbData->set_godlikecount(uGodlikeCount);
	pbData->set_megakillcount(uMegakillCount);
	pbData->set_ismvp(uMvpCount > 0);
}
