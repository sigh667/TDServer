/*----------------- playerCommon.cpp
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2016/1/8 15:44:53
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "basic/stringFunctions.h"
#include "basic/timeFunction.h"
#include "log/log.h"
//------------------------------ 
#include "protocol/CASMsg.pb.h"
#include "protocol/GhostStruct.pb.h"
#include "protocol/MsgResult.pb.h"
#include "protocol/GLAGProtocol.pb.h"
#include "protocol/CASProtocol.pb.h"
#include "protocol/WAGProtocol.pb.h"
#include "protocol/GLAGMsg.pb.h"
//------------------------------ 
#include "main/gameServer.h"
#include "packet/clientPacket.h"
#include "gameLogic/playerCommon/playerCommon.h"
#include "gameLogic/sceneObject/scenePlayer.h"
#include "gameLogic/cooling/cooling.h"
//#include "gameLogic/playerQuest/playerQuestMgr.h"
//#include "gameLogic/playerSystemSwitch/playerSystemSwitch.h"
//#include "gameLogic/playerActivity/playerActivity.h"
#include "gameLogin/gamePlayerMgr.h"
#include "gameScene/copymapBase.h"
#include "gameScene/gameSceneCell.h"
#include "cfg_data/playerData/playerData.h"
#include "cfg_data/clientData/clientData.h"
#include "cfg_data/constantData/constantData.h"
#include "cfg_data/strongerData/strongerData.h"
#include "cfg_data/sceneFlyData/sceneFlyData.h"
#include "cfg_data/promotionData/promotionData.h"
#include "cfg_data/questData/questData.h"
#include "game_log.h"
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
CPlayerCommon::CPlayerCommon(CScenePlayer& clPlayer):CGamePlayerFun(clPlayer),m_clGMCommand(clPlayer)
{
	registerHandler(C2S_Common_Go,(CGamePlayerFun::_handleSubMsg)&CPlayerCommon::handle_go	);
	registerHandler(C2S_Common_ClientSave,(CGamePlayerFun::_handleSubMsg)&CPlayerCommon::handle_client_save	);
	registerHandler(C2S_Common_Move,(CGamePlayerFun::_handleSubMsg)&CPlayerCommon::handle_move	);
	registerHandler(C2S_Common_MoveStop,(CGamePlayerFun::_handleSubMsg)&CPlayerCommon::handle_move_stop	);
	registerHandler(C2S_Common_Turn,(CGamePlayerFun::_handleSubMsg)&CPlayerCommon::handle_turn	);
	registerHandler(C2S_Common_GMCmd,(CGamePlayerFun::_handleSubMsg)&CPlayerCommon::handle_gm_cmd	);
	registerHandler(C2S_Common_Pickup,(CGamePlayerFun::_handleSubMsg)&CPlayerCommon::handle_pickup	);
	registerHandler(C2S_Common_GetClientData,(CGamePlayerFun::_handleSubMsg)&CPlayerCommon::handle_get_client_data	);
	registerHandler(C2S_Common_FindPlayer,(CGamePlayerFun::_handleSubMsg)&CPlayerCommon::handle_find_player	);
	registerHandler(C2S_Common_Promotion,(CGamePlayerFun::_handleSubMsg)&CPlayerCommon::handle_promotion);
	registerHandler(C2S_Common_PlaneEnter,(CGamePlayerFun::_handleSubMsg)&CPlayerCommon::handle_planeenter);
	registerHandler(C2S_Common_PlaneLeave,(CGamePlayerFun::_handleSubMsg)&CPlayerCommon::handle_planeleave);
	registerHandler(C2S_Common_BossList,(CGamePlayerFun::_handleSubMsg)&CPlayerCommon::handle_bosslist);
	registerHandler(C2S_Common_Fly, (CGamePlayerFun::_handleSubMsg)&CPlayerCommon::handle_fly);
	registerHandler(C2S_Common_SearchPlayer, (CGamePlayerFun::_handleSubMsg)&CPlayerCommon::handle_search_player);
	registerHandler(C2S_Common_NpcDialogComplete, (CGamePlayerFun::_handleSubMsg)&CPlayerCommon::handle_npcdialog_complete);
	registerHandler(C2S_Common_GetBattleAttrDetail, (CGamePlayerFun::_handleSubMsg)&CPlayerCommon::handle_get_battleattr_detail); 
	registerHandler(C2S_Common_PKType, (CGamePlayerFun::_handleSubMsg)&CPlayerCommon::handle_pktype);
	registerHandler(C2S_Common_TakeASeat, (CGamePlayerFun::_handleSubMsg)&CPlayerCommon::handle_takeaseat);
	registerHandler(C2S_Common_TaskFly, (CGamePlayerFun::_handleSubMsg)&CPlayerCommon::handle_task_fly);
	registerHandler(C2S_Common_ForbitSpeak, (CGamePlayerFun::_handleSubMsg)&CPlayerCommon::handle_forbit_speak);
	registerHandler(C2S_Common_StrongerGetReward, (CGamePlayerFun::_handleSubMsg)&CPlayerCommon::handle_stronger_reward);
	registerHandler(C2S_Common_GetSubFightPower, (CGamePlayerFun::_handleSubMsg)&CPlayerCommon::handle_sub_fight_power);
}

//-------------------------------------------------------------
//------------------------------ 
CPlayerCommon::~CPlayerCommon()
{
}
//-------------------------------------------------------------
//------------------------------ 
void	CPlayerCommon::initialize			()
{
	CGamePlayerFun::initialize();
	m_pbClientData.Clear();
}
//-------------------------------------------------------------
//------------------------------ 
bool	CPlayerCommon::loadFromProtobuf	(const PBPlayerData&pbPlayer)
{
	m_pbClientData.CopyFrom(pbPlayer.clientdata());
	m_clPlayer.getCooling().loadData(pbPlayer.playersystem().cooling());

	m_uDailyDazuoguajiaExp = pbPlayer.playersystem().dazuoguaji().todaydazuoguajiexpcount();
	m_uDailyDazuoguajiaWisdom = pbPlayer.playersystem().dazuoguaji().todaydazuoguajiwisdomcount();

	this->calculateAttri();

	const PBPlayerCommon & pbCommon = pbPlayer.playersystem().common();
	for (int32 i = 0; i < pbCommon.strongerid_size(); ++i)
	{
		m_uStrongerFightID.insert(pbCommon.strongerid(i));
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CPlayerCommon::saveToProtobuf		(PBPlayerData&pbPlayer)
{
	PBClientData* pbClientData = pbPlayer.mutable_clientdata();
	if (pbClientData)
		pbClientData->CopyFrom(m_pbClientData);

	PBPlayerSystem* pbSystem = pbPlayer.mutable_playersystem();
	if ( pbSystem )
	{
		PBCooling* pbCooling = pbSystem->mutable_cooling();
		if (pbCooling)
			m_clPlayer.getCooling().saveData(*pbCooling);
		PBPlayerDazuoguaji* pbDazuoguaji = pbSystem->mutable_dazuoguaji();
		if (pbDazuoguaji)
		{
			pbDazuoguaji->set_todaydazuoguajiexpcount(m_uDailyDazuoguajiaExp);
			pbDazuoguaji->set_todaydazuoguajiwisdomcount(m_uDailyDazuoguajiaWisdom);
		}

		PBPlayerCommon	*pCommon = pbSystem->mutable_common();
		if (pCommon)
		{
			auto _pos	= m_uStrongerFightID.begin();
			auto _end	= m_uStrongerFightID.end();	
			for (; _pos != _end; ++_pos)
				pCommon->add_strongerid(*_pos);
		}
	}

	return true;
}
//------------------------------------------------------------------------------
//------------------------------------- 
bool	CPlayerCommon::handle_go		(uint8 uEventID,CNetStream& clStream)
{
	if ( !m_clPlayer.delState(StateType_WaitReady))
		return false;

	m_clPlayer.resetGhost();

	m_clPlayer.callbackClientGo();

	CGameSceneCell* pCell = m_clPlayer.getSceneCell();
	if (pCell )
		pCell->player_syn(&m_clPlayer);

	return true;
}
//------------------------------------------------------------------------------
//------------------------------------- 
bool	CPlayerCommon::handle_client_save		(uint8 uEventID,CNetStream& clStream)
{
	
	CHECKF(clStream.ParseFromStream(m_pbClientData) );

	return true;
}
//------------------------------------------------------------------------------
//------------------------------------- 
bool	CPlayerCommon::handle_move		(uint8 uEventID,CNetStream& clStream)
{
	PBC2GCommonMove move;
	CHECKF(clStream.ParseFromStream(move) );

	CGameSceneCell* pCell = m_clPlayer.getSceneCell();
	if (pCell == NULL) return true;

	m_clPlayer.getGameSkillMgr().clearSkillCmd();
	//清除复活时所加的buff
	m_clPlayer.getGameBuffMgr().delBuff((uint32)g_clConstantData.Relive_Buff_ID, DoingType_Relive);
	//uint8 uCode = R_ResultMove_Fail;
	_stPoint2F tar;
	tar.x	  = move.posx();
	tar.y	  = move.posy();
	if ( !m_clPlayer.beginMove((uint8)move.type(),tar.x,tar.y,move.dir())  )
	{
		PBGhostMove ack;
		m_clPlayer.packetMove(ack);
		m_clPlayer.sendMsg(P_S2C_Protocol_Common,S2C_Common_Move,&ack);
	}
	return true;
}
//------------------------------------------------------------------------------
//------------------------------------- 
bool	CPlayerCommon::handle_move_stop		(uint8 uEventID,CNetStream& clStream)
{
	PBC2GCommonMove move;
	CHECKF(clStream.ParseFromStream(move) );

	CGameSceneCell* pCell = m_clPlayer.getSceneCell();
	if(pCell == NULL) return true;

	uint8 uCode = R_ResultMove_Fail;
	
	_stPoint2F tar;
	tar.x	  = move.posx();
	tar.y	  = move.posy();
	if ( !m_clPlayer.endMove(tar.x,tar.y,move.dir()) )
	{
		PBGhostMove ack;
		m_clPlayer.packetMove(ack);
		m_clPlayer.sendMsg(P_S2C_Protocol_Common,S2C_Common_MoveStop,&ack,uCode);
	}

	return true;
}
//------------------------------------------------------------------------------
//------------------------------------- 
bool	CPlayerCommon::handle_turn		(uint8 uEventID,CNetStream& clStream)
{
	PBC2GCommonMove move;
	CHECKTR(clStream.ParseFromStream(move) );

	uint8 uCode = R_ResultMove_Fail;
	if ( !m_clPlayer.changeDiretion(move.posx(),move.posy(),move.dir()) )
	{
		PBGhostMove ack;
		m_clPlayer.packetMove(ack);
		m_clPlayer.sendMsg(P_S2C_Protocol_Common,S2C_Common_Turn,&ack,uCode);
	}

	return true;
}
//------------------------------------------------------------------------------
//------------------------------------- 
bool	CPlayerCommon::handle_gm_cmd				(uint8 uEventID,CNetStream& clStream)
{
	return m_clGMCommand.handle_txt_cmd(uEventID,clStream);
}

//------------------------------------------------------------------------------
//------------------------------------- 
void CPlayerCommon::onLogin()
{
	const _stPromotionModel* promotionModel = m_clPlayer.getPromotionModel();
	CHECK(promotionModel != NULL);
	
	//CPlayerQuestMgr* questMgr = m_clPlayer.getPlayerQuestMgr();
	//CHECK(questMgr != NULL);
	//
	//// 如果身上没有晋升任务，做个容错
	//CPlayerQuest *pQuest = questMgr->getQuest(promotionModel->uQuest);
	//if (pQuest == NULL) return ;
	//
	//uint8 uCode = 0;
	//questMgr->acceptQuest(promotionModel->uQuest, uCode);
}

//------------------------------------------------------------------------------
//------------------------------------- 
bool CPlayerCommon::handle_pickup(uint8 uEventID,CNetStream& clStream)
{
	return true;
}

//------------------------------------------------------------------------------
//------------------------------------- 
bool CPlayerCommon::handle_get_client_data(uint8 uEventID,CNetStream& clStream)
{
	PBC2GGetClientData ask;
	CHECKF(clStream.ParseFromStream(ask));

	for (int i = 0; i < ask.clientdata_size(); ++i)
	{
		const PBVerClientData& stClientData = ask.clientdata(i);
		const CClientData::_stClientData* pClientData = g_clClientData.find(stClientData.filename());
		CHECKC(pClientData != NULL);
		if (pClientData->uCrc != stClientData.crc())
		{
			PBG2CGetClientData ack;
			ack.set_crc(pClientData->uCrc);
			ack.set_filename(stClientData.filename());
			ack.set_filedata(pClientData->strData);
			m_clPlayer.sendMsg(P_S2C_Protocol_Common,S2C_Common_GetClientData,&ack,uEventID);
		}
	}

	PBG2CClientDataOver ack;
	m_clPlayer.sendMsg(P_S2C_Protocol_Common,S2C_Common_ClientDataOver,&ack,uEventID);

	return true;
}

//-------------------------------------------------------------
//------------------------------
bool CPlayerCommon::handle_find_player		(uint8 uEventID,CNetStream& clStream)
{
	PBC2GFindPlayer ask;
	CHECKF(clStream.ParseFromStream(ask));
	
	CScenePlayer* pPlayer = g_pGamePlayerMgr->findPlayer(ask.playerid());
	if (pPlayer)
	{
		PBG2CFindPlayer ack;
		pPlayer->packetView(ack.mutable_pbview(),true);

		//通知
		if (pPlayer->getSceneCell() == m_clPlayer.getSceneCell() && pPlayer->getPlayerCommon() && ask.playerid() != pPlayer->getPlayerID())
			pPlayer->getPlayerCommon()->sendBeFindNoticeAck();
		m_clPlayer.sendMsg(P_S2C_Protocol_Common,S2C_Common_FindPlayer,&ack);
	}
	else
	{
		//转发到全局服
		CHECKTR(m_clPlayer.isGlobalConneted());
		
		PBGLAGFindPlayer msg;
		msg.set_sourceid(m_clPlayer.getPlayerID());
		msg.set_targetid(ask.playerid());
		msg.set_worldid(m_clPlayer.getLogicWorldID());
		m_clPlayer.sendMsgToGlobal(P_G2GL_Protocol_Common,GLAG_Common_FindPlayer,&msg);
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------
bool CPlayerCommon::handle_promotion(uint8 uEventID,CNetStream& clStream)
{
	uint8 uCode = R_ResultCommon_Fail;
	m_clPlayer.promote(uCode);
	PBG2CCommon_Promotion ack;
	ack.set_step(m_clPlayer.getPromotionStep());
	m_clPlayer.sendMsg(P_S2C_Protocol_Common, S2C_Common_Promotion, &ack, uCode);
	return true;
}

//-------------------------------------------------------------
//------------------------------
bool CPlayerCommon::handle_planeenter		(uint8 uEventID,CNetStream& clStream)
{
	uint8 uCode = R_ResultCommon_Fail;
	PBC2GCommon_PlaneEnter ask;
	clStream.ParseFromStream(ask);
	m_clPlayer.plane_enter(uCode);
	{
		CLog::error("plane enter:%d,size:%d",ask.param(),ask.ByteSize());
		m_clPlayer.sendMsg(P_S2C_Protocol_Common,S2C_Common_PlaneEnter_Ack,&ask,uCode);
		//return false;
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerCommon::handle_planeleave		(uint8 uEventID,CNetStream& clStream)
{
	uint8 uCode = R_ResultCommon_Fail;
	PBC2GCommon_PlaneLeave ask;
	clStream.ParseFromStream(ask);
	m_clPlayer.plane_leave(ask,uCode);
	{
		CLog::error("plane leave:%f,%f,size:%d",ask.posx(),ask.posy(),ask.ByteSize());
		m_clPlayer.sendMsg(P_S2C_Protocol_Common,S2C_Common_PlaneLeave_Ack,&ask,uCode);
		//return false;
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerCommon::handle_bosslist		(uint8 uEventID,CNetStream& clStream)
{
	CHECKTR(m_clPlayer.isGlobalConneted() );

	PBG2GLCommon_BossListAsk msg;
	msg.set_playerid(m_clPlayer.getPlayerID());
	m_clPlayer.sendMsgToGlobal(P_G2GL_Protocol_Common, GLAG_Common_BossListAsk,&msg);
	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerCommon::handle_fly (uint8 uEventID,CNetStream& clStream)
{
	uint8 uCode = R_ResultFly_Fail;
	
	PBC2GCommon_FlyAsk ask;
    CHECKF(clStream.ParseFromStream(ask));

	PBG2CCommon_FlyAck ack;
	do{
	
	uint16 uSceneID		= 0;
	uint16 uRegionID	= 0;
	_stPoint2F stTargetPos;
	// 御风神行
	if (ask.flytype()== FlyType_FlyByWind) 	
	{
		if (m_clPlayer.getCooling().isCooling(CoolingType_Fly,FlyType_FlyByWind))
		{
			uCode = R_ResultFly_Cooling;
			break;
		}			
		
		const _stFlyModel *pFlyModel	= g_clSceneFlyData.findFlyModel(ask.id());
		if (!pFlyModel)
		{
			uCode = R_ResultFly_Fail;
			break;
		}		
		uSceneID					= pFlyModel->uEndSceneID;
		uRegionID					= pFlyModel->uRegionID;
		if (!uRegionID)
			stTargetPos = pFlyModel->stPos;

		uint32 uCoolingMS = g_clConstantData.Game_FlyByWind;
		m_clPlayer.getCooling().addCooling(CoolingType_Fly,FlyType_FlyByWind, uCoolingMS * 1000);
	}
	else if (ask.flytype()== FlyType_FlyByHorse) // 马夫传送
	{
		const _stHostlerModel *pHosterModel	= g_clSceneFlyData.findHostlerModel(ask.id());
		if(!pHosterModel)
		{
			uCode = R_ResultFly_Fail;
			break;
		}

		uSceneID					= pHosterModel->uSceneID;
		uRegionID					= pHosterModel->uRegionID;
		if (!uRegionID)
			stTargetPos = pHosterModel->stPos;
	}
	else if (ask.flytype()== FlyType_FindNpc) // 传送到NPC
	{
		const _stFlyNpcModel* pModel = g_clSceneFlyData.findFlyNpcModel(ask.id());
		if(!pModel)
		{
			uCode = R_ResultFly_Fail;
			break;
		}

		uSceneID					= pModel->uSceneID;
		uRegionID					= pModel->uRegionID;
		if (!uRegionID)
			stTargetPos = pModel->stPos;
	}
	else
	{
		uCode = R_ResultFly_Type;
		break;
	}

	if (stTargetPos.isZero())
	{
		if (!m_clPlayer.playerFlyScene_ByCellID(uSceneID, 0, uRegionID))
		{
			if (ask.flytype()== FlyType_FlyByWind)
			{
				m_clPlayer.getCooling().clearCooling(CoolingType_Fly,FlyType_FlyByWind);
				uCode = R_ResultFly_Fail;
				break;
			}	
		}
	}
	else
	{
		if (!m_clPlayer.playerFlyScene_ByCellID(uSceneID, 0, stTargetPos.x,stTargetPos.y))
		{
			if (ask.flytype()== FlyType_FlyByWind)
			{
				m_clPlayer.getCooling().clearCooling(CoolingType_Fly,FlyType_FlyByWind);
				uCode = R_ResultFly_Fail;
				break;
			}	
		}
	}

	uCode = R_ResultFly_Succeed;
	}while (false);
	
	m_clPlayer.sendMsg(P_S2C_Protocol_Common,S2C_Common_Fly_Ack,&ack,uCode);

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool	CPlayerCommon::handle_task_fly			(uint8 uEventID,CNetStream& clStream)
{
	uint8 uCode = R_ResultFly_Fail;

	PBC2GCommon_TaskFlyAsk ask;
	CHECKF(clStream.ParseFromStream(ask));

	PBG2CCommon_TaskFlyAck ack;
	do{

	if (!m_clPlayer.playerFlyScene_ByCellID(ask.sceneid(), 0,ask.regionid()))
	{	
		break;
	}	

	uCode = R_ResultFly_Succeed;
	}while (false);

	m_clPlayer.sendMsg(P_S2C_Protocol_Common,S2C_Common_TaskFly,&ack,uCode);

	return true;
}

//-------------------------------------------------------------
//------------------------------
bool CPlayerCommon::handle_search_player		(uint8 uEventID,CNetStream& clStream)
{
	PBC2GCommon_SearchPlayer ask;
	CHECKF(clStream.ParseFromStream(ask));

	CHECKTR(m_clPlayer.isGlobalConneted());

	CNetStream stStream;
	stStream.write(m_clPlayer.getPlayerID());
	stStream.SerializeToStream(ask);
	//转发到全局服
	m_clPlayer.sendBufferToGlobal(P_G2GL_Protocol_Common, GLAG_Common_SearchPlayer,stStream.getBuffer(), stStream.getPosition());
	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerCommon::handle_npcdialog_complete		(uint8 uEventID,CNetStream& clStream)
{
	PBC2GCommon_NpcDialogComplete ask;
	CHECKF(clStream.ParseFromStream(ask));

	CGameSceneCell* pGameScell = m_clPlayer.getSceneCell();
	CHECKTR(pGameScell != NULL);

	pGameScell->npcdialog(ask.unitid(),&m_clPlayer);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
void	CPlayerCommon::onBuildComplete			()
{
	const auto *pModel	= g_clPromotionData.findByLevel(m_clPlayer.getJob(), m_clPlayer.getLevel());
	if (!pModel)
		return ;

	const auto *pQuestModel = g_clQuestData.findQuestModel(pModel->uQuest);
	//if (NULL != pModel && NULL != pQuestModel && NULL == m_clPlayer.getPlayerQuestMgr()->getQuest(pModel->uQuest))
	{
		//m_clPlayer.getPlayerQuestMgr()->acceptQuest(pQuestModel);
	}
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerCommon::calculateAttri()
{
	_stGameAttribute*pAtrr = m_clPlayer.getAttribute(GameSystemType_Common);
	CHECKN( pAtrr != NULL );

	pAtrr->initGameAttribute();
	if (m_clPlayer.getLevelModel())
		*pAtrr += m_clPlayer.getLevelModel()->stAttribute;
	
	m_clPlayer.setUpdateValue(_UV_AttributeBase);

	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerCommon::sendBeFindNoticeAck()
{
	PBG2CCommon_BeFindNotice ack;
	return m_clPlayer.sendMsg(P_S2C_Protocol_Common,S2C_Common_BeFindNotice,&ack);
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerCommon::sendCopymapDayCountAck(uint8 uCopymapMainType,uint32 uCount)
{
	PBG2CCommon_DayCopymapCount ack;
	ack.set_type(uCopymapMainType);
	ack.set_count(uCount);
	return m_clPlayer.sendMsg(P_S2C_Protocol_Common,S2C_Common_SynTypeDayCopymapCount,&ack);
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerCommon::sendTeamTaskDayCountAck(uint32 uID,uint32 uCount)
{
	PBTeamTaskDayCount ack;
	ack.set_id(uID);
	ack.set_count(uCount);
	return m_clPlayer.sendMsg(P_S2C_Protocol_Common,S2C_Common_SynDayTeamTaskCount,&ack);
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerCommon::broadcastQinggong		(uint32 uQinggongID)
{
	PBS2CQinggongStartNtf ntf;
	ntf.Clear();
	ntf.set_unitid(m_clPlayer.getUnitID());
	ntf.set_qinggongid(uQinggongID);

	m_clPlayer.getSceneCell()->broadcastMsg(P_S2C_Protocol_Common, S2C_Common_QinggongStart_Ntf, &ntf);
	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerCommon::handle_get_battleattr_detail		(uint8 uEventID,CNetStream& clStream)
{
	PBC2GCommonGetBattleAttrDetailAsk ask;
	CHECKF(clStream.ParseFromStream(ask));

	PBS2CGetBattleAttrDetailAck ack;
	ack.Clear();
	uint8 uCode = R_ResultCommon_Succeed;
	if (!onGetBattleAttrDetail(ask.type(), ack, uCode))
	{
		sendBattleAttrDetail(&ack, uCode);
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerCommon::sendBattleAttrDetail	(const PBS2CGetBattleAttrDetailAck *ack, uint8 uCode)
{
	return m_clPlayer.sendMsg(P_S2C_Protocol_Common,S2C_Common_GetBattleAttrDetail,ack,uCode);
}
//-------------------------------------------------------------
//------------------------------
bool CPlayerCommon::onGetBattleAttrDetail	(uint32 uType, PBS2CGetBattleAttrDetailAck & ack, uint8& uCode)
{
	ack.set_type(uType);
	if (uType > BattleAttribute_Murder)
	{
		uCode = R_ResultCommon_BattleAttrNotFound;
		return false;
	}
	uint32 uSystemType = 0;
	uint32 uTotal = 0;
	//基础
	{
		uSystemType = GameSystemType_Common;
		uint32 uFromType = GameBattleAttrFromType_Base;
		int32  uValue = m_clPlayer.getAttributeSingle((_emGameSystemType)uSystemType, uType);
		PBBattleAttrDetail * pbBAD = ack.add_detail();
		if (pbBAD)
		{
			pbBAD->set_sourcetype(uFromType);
			pbBAD->set_value(uValue);
		}
		uTotal += uValue;
	}
	//装备
	{
		uSystemType = GameSystemType_Item;
		uint32 uFromType = GameBattleAttrFromType_Equip;
		int32  uValue = m_clPlayer.getAttributeSingle((_emGameSystemType)uSystemType, uType);
		PBBattleAttrDetail * pbBAD = ack.add_detail();
		if (pbBAD)
		{
			pbBAD->set_sourcetype(uFromType);
			pbBAD->set_value(uValue);
		}
		uTotal += uValue;
	}
	//帮派技能
	{
		uSystemType = GameSystemType_Skill;
		uint32 uFromType = GameBattleAttrFromType_FactionSkill;
		int32  uValue = m_clPlayer.getAttributeSingle((_emGameSystemType)uSystemType, uType);
		PBBattleAttrDetail * pbBAD = ack.add_detail();
		if (pbBAD)
		{
			pbBAD->set_sourcetype(uFromType);
			pbBAD->set_value(uValue);
		}
		uTotal += uValue;
	}
	//文韬
	{
		uSystemType = GameSystemType_WenTao;
		uint32 uFromType = GameBattleAttrFromType_Wentao;
		int32  uValue = m_clPlayer.getAttributeSingle((_emGameSystemType)uSystemType, uType);
		PBBattleAttrDetail * pbBAD = ack.add_detail();
		if (pbBAD)
		{
			pbBAD->set_sourcetype(uFromType);
			pbBAD->set_value(uValue);
		}
		uTotal += uValue;
	}
	//武略
	{
		uSystemType = GameSystemType_Wulue;
		uint32 uFromType = GameBattleAttrFromType_Wulue;
		int32  uValue = m_clPlayer.getAttributeSingle((_emGameSystemType)uSystemType, uType);
		PBBattleAttrDetail * pbBAD = ack.add_detail();
		if (pbBAD)
		{
			pbBAD->set_sourcetype(uFromType);
			pbBAD->set_value(uValue);
		}
		uTotal += uValue;
	}
	ack.set_total(uTotal);
	sendBattleAttrDetail(&ack, uCode);
	return true;
}

//-------------------------------------------------------------
//------------------------------设置PK模式
bool CPlayerCommon::handle_pktype		(uint8 uEventID,CNetStream& clStream)
{
	uint8 uCode = R_ResultCommon_Fail;
	PBC2GCommon_PKType ask;
	CHECKF(clStream.ParseFromStream(ask));
	
	if (!m_clPlayer.setPKType(ask.pktype(),uCode))
	{
		m_clPlayer.sendMsg(P_S2C_Protocol_Common,S2C_Common_PKType,NULL,uCode);
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------打坐命令
bool CPlayerCommon::handle_forbit_speak		(uint8 uEventID,CNetStream& clStream)
{
	CNetStream stStream;
	stStream.write(m_clPlayer.getPlayerID());
	stStream.write(clStream.getSpareSize(),clStream.getSpareBuffer());
	return m_clPlayer.sendBufferToWorld(P_G2W_Protocol_Common, G2W_Common_ForbitSpeak, stStream.getBuffer(), stStream.getPosition());
}

//-------------------------------------------------------------
//------------------------------
bool			CPlayerCommon::handle_stronger_reward	(uint8 uEventID,CNetStream& clStream)
{
	uint8 uCode = R_ResultCommon_Fail;
	PBC2GCommonStrongerReWard ask;
	CHECKF(clStream.ParseFromStream(ask));

	if (!strongerGetReWard(ask.uid(),uCode))
	{
		m_clPlayer.sendMsg(P_S2C_Protocol_Common,S2C_Common_StrongerGetReward,NULL,uCode);
	}
	return true;
}

//-------------------------------------------------------------
//-----------------------------
bool			CPlayerCommon::handle_sub_fight_power	(uint8 uEventID,CNetStream& clStream)
{
	uint8 uCode = R_ResultCommon_Fail;
	PBC2GCommonSubFightPower ask;
	CHECKF(clStream.ParseFromStream(ask));

	if (!m_clPlayer.calculateSubFightPower(uCode))
	{
		m_clPlayer.sendMsg(P_S2C_Protocol_Common,S2C_Common_GetSubFightPower,NULL,uCode);
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------打坐命令
bool CPlayerCommon::handle_takeaseat		(uint8 uEventID,CNetStream& clStream)
{
	uint8 uCode = R_ResultCommon_Succeed;

	if (!onTakeASeat(uCode))
	{
		m_clPlayer.sendMsg(P_S2C_Protocol_Common,S2C_Common_TakeASeat,NULL,uCode);
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------执行打坐
bool CPlayerCommon::onTakeASeat				(uint8& uCode)
{

	if (m_clPlayer.haveState(StateType_Singing) 
		&& m_clPlayer.getSkilling().getSkillModel() 
		&& m_clPlayer.getSkilling().getSkillModel()->uType == SkillType_MonsterCollletion
		)
	{
		uCode = R_ResultCommon_Cant_SitDown_Collect;
		return false;
	}
	else if (m_clPlayer.isFighting())
	{
		uCode = R_ResultCommon_Cant_SitDown_Fight;
		return false;
	}
	else if (m_clPlayer.getMountID() > 0)
	{
		uCode = R_ResultCommon_Cant_SitDown_Mount;
		return false;			
	}
	//自动战斗判断


	PBS2GCommon_TakeASeatAck ack;
	ack.Clear();
	if (m_clPlayer.haveState(StateType_Rest))
	{
		ack.set_issitdown(false);
		//站起来	
		//离开状态,减buff,
		m_clPlayer.delState(StateType_Rest);
	}
	else
	{
		ack.set_issitdown(true);
		//坐下
		//进入状态,加buff
		m_clPlayer.addState(StateType_Rest);
	}

	//返回消息
	if (m_clPlayer.testRegion(RegionMark_ZaZen))
	{
		//CPlayerActivity * pActivity = m_clPlayer.getPlayerActivity();
		//if (pActivity)
		//	pActivity->callback_takeASeat();
		//
		//CPlayerQuestMgr* pQuestMgr = m_clPlayer.getPlayerQuestMgr();
		//if (pQuestMgr)
		//	pQuestMgr->callback_completeTakeASeat();
	}
	
	m_clPlayer.sendMsg(P_S2C_Protocol_Common,S2C_Common_TakeASeat,&ack,uCode);

	return true;
}
//-------------------------------------------------------------
//------------------------------取消打坐
bool CPlayerCommon::cancelTakeASeat			()
{
	if (m_clPlayer.haveState(StateType_Rest))
	{
		PBS2GCommon_TakeASeatAck ack;
		ack.Clear();
		ack.set_issitdown(false);
		//站起来
		//离开状态,减buff,
		m_clPlayer.delState(StateType_Rest);
		m_clPlayer.sendMsg(P_S2C_Protocol_Common,S2C_Common_TakeASeat,&ack,R_ResultCommon_Succeed);
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------每日清零
void CPlayerCommon::cleanupDaily()
{
	//todo 记录日志
	g_game_log->doing_log(g_pServer->getLogicWorldID(), 
		m_clPlayer.getSceneID(),m_clPlayer.getSceneCellID(),DoingType_Dazuoguaji,ExpendType_Exp,
		m_clPlayer.getPlayerID(),0,0,0,0, m_uDailyDazuoguajiaExp);

	g_game_log->doing_log(g_pServer->getLogicWorldID(), 
		m_clPlayer.getSceneID(),m_clPlayer.getSceneCellID(),DoingType_Dazuoguaji,ExpendType_Wisdom,
		m_clPlayer.getPlayerID(),0,0,0,0, m_uDailyDazuoguajiaWisdom);

	m_uDailyDazuoguajiaExp = 0;
	m_uDailyDazuoguajiaWisdom = 0;
}
//-------------------------------------------------------------
//------------------------------发送剩余的打坐挂机收益
bool CPlayerCommon::sendDazuoguajiNtf		(uint32 uExpLeft, uint32 uWisdomLeft, uint32 uExpAdded, uint32 uWisdomAdded, bool bShowPrompt,bool  bClosePanel)
{
	PBG2CCommon_DazuoguajiTotalNtf ntf;
	ntf.Clear();
	ntf.set_dailydazuoguajiexpleft(uExpLeft);
	ntf.set_dailydazuoguajiwisdomleft(uWisdomLeft);
	ntf.set_wisdomadded(uWisdomAdded);
	ntf.set_expadded(uExpAdded);
	ntf.set_showprompt(bShowPrompt);
	ntf.set_closepanel(bClosePanel);
	m_clPlayer.sendMsg(P_S2C_Protocol_Common,S2C_Common_DazuoguajiTotalNtf,&ntf,R_ResultCommon_Succeed);
	return true;
}

//-------------------------------------------------------------
//------------------------------
bool CPlayerCommon::strongerGetReWard(uint32 uID,	uint8 &uCode)
{
	if (m_uStrongerFightID.is_exist(uID))
	{
		uCode = R_ResultCommon_StrongerReward_Exist;
		return false;
	}

	_stStrongerModel * pModel = g_clStrongerData.getStrongerModel(uID);
	if (!pModel)
	{
		uCode = R_ResultCommon_Fail;
		return false;
	}

	if (m_clPlayer.getFightPower() < pModel->uFightPower)
	{
		uCode = R_ResultCommon_StrongerReward_PowerLess;
		return false;
	}

	auto _pos = pModel->mapItem.begin();
	auto _end =  pModel->mapItem.end();

	for (; _pos != _end; ++_pos)
	{
		if (! m_clPlayer.getGameItemMgr().addItem(_pos->first, _pos->second, true, true, true,DoingType_Stronger))
		{
			return R_ResultCommon_Fail;
		}
	}

	m_uStrongerFightID.insert(uID);
	PBG2CCommonStrongerReWard ack;
	ack.set_uid(uID);
	m_clPlayer.sendMsg(P_S2C_Protocol_Common,S2C_Common_StrongerGetReward,&ack,R_ResultCommon_Succeed);

	return true;
}
