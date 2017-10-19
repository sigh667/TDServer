/*----------------- scenePlayer.cpp
*
* Copyright (C): 2011  
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/9/15 11:38:37
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#include "gameLogic/sceneObject/scenePlayer.h"
#include "serverRoot/server_define.h"
#include "basic/stringFunctions.h"

#include "packet/clientPacket.h"
#include "gameLogin/gamePlayerMgr.h"
#include "gameScene/gameSceneCell.h"
#include "gameScene/gameSceneMgr.h"
#include "gameScene/copymapBase.h"
//#include "gameScene/copymapManorLobby.h"
//#include "gameScene/copymapManorBattleField.h"
#include "log/log.h"
#include "gameSave/gameSave.h"
#include "main/gameServer.h"

#include "cfg_data/birthData/birthData.h"
#include "cfg_data/constantData/constantData.h"
#include "cfg_data/expendData/expendData.h"
#include "cfg_data/campData/campData.h"
#include "cfg_data/jobData/jobData.h"
#include "cfg_data/playerData/playerData.h"
#include "cfg_data/promotionData/promotionData.h"
#include "cfg_data/questData/questData.h"
#include "cfg_data/prizeData/prizeData.h"
#include "cfg_data/monsterData/monsterData.h"
#include "cfg_data/systemSwitchData/systemSwitchData.h"
#include "cfg_data/treasureMapData/treasureMapData.h"

#include "gameLogic/gamePlayerFun/gamePlayerFun.h"
#include "gameLogic/clientGMCommand/clientGMCommand.h"
#include "gameLogic/playerCommon/playerCommon.h"
#include "gameLogic/playerSkill/playerSkill.h"
#include "gameLogic/playerBuff/playerBuff.h"
//#include "gameLogic/playerPrize/playerPrize.h"
#include "gameLogic/playerItem/playerItem.h"
//#include "gameLogic/playerQuest/playerQuest.h"
//#include "gameLogic/playerQuest/playerQuestMgr.h"
#include "gameLogic/playerCounter/playerCounter.h"
//#include "gameLogic/playerTeam/playerTeam.h"
#include "gameLogic/playerCopymap/playerCopymap.h"
//#include "gameLogic/playerStore/playerStore.h"
//#include "gameLogic/playerAuction/playerAuction.h"
//#include "gameLogic/playerTrade/playerTrade.h"
//#include "gameLogic/playerSaleMoney/playerSaleMoney.h"
#include "gameLogic/gameMail/gameMail.h"
//#include "gameLogic/playerFaction/playerFaction.h"
//#include "gameLogic/playerTopList/playerTopList.h"
//#include "gameLogic/playerFriend/playerFriend.h"
//#include "gameLogic/playerWenTao/playerWenTao.h"
//#include "gameLogic/playerWulue/playerWulue.h"
//#include "gameLogic/playerDropItem/playerDropItem.h"
//#include "gameLogic/playerIdentity/playerIdentity.h"
#include "gameLogic/playerSystemSwitch/playerSystemSwitch.h"
//#include "gameLogic/playerColletion/playerColletion.h"
//#include "gameLogic/playerIdentity/playerIdentity.h"
//#include "gameLogic/playerWuxue/playerWuxue.h"
//#include "gameLogic/playerDart/playerDart.h"
//#include "gameLogic/playerFactionBoss/playerFactionBoss.h"
//#include "gameLogic/playerTitle/playerTitle.h"
//#include "gameLogic/playerFashion/playerFashion.h"
//#include "gameLogic/playerEquipColumn/playerEquipColumn.h"
//#include "gameLogic/playerClimbTower/playerClimbTower.h"
//#include "gameLogic/playerPvpWar/playerPvpWar.h"
//#include "gameLogic/playerGreatwallTrain/playerGreatwallTrain.h"
//#include "gameLogic/playerQuestion/playerQuestion.h"
//#include "gameLogic/playerWorldBoss/playerWorldBoss.h"
//#include "gameLogic/playerActivity/playerActivity.h"
#include "gameLogic/gameBroadcast/gameBroadcast.h"
//#include "gameLogic/globalObject/worldBossMgr.h"
#include "gameLogic/playerRelive/playerRelive.h"
//#include "gameLogic/playerCorps/playerCorps.h"
//#include "gameLogic/playerWeal/playerWeal.h"
//#include "gameLogic/playerManor/playerManor.h"
//#include "gameLogic/playerSinglePvp/playerSinglePvp.h"

#include "protocol/CASMsg.pb.h"
#include "protocol/GhostStruct.pb.h"
#include "protocol/GameDefine.pb.h"
#include "protocol/MsgResult.pb.h"
#include "protocol/Broadcast.pb.h"
#include "def_functions/protobufFun.h"
#include "json2pb.h"
#include "cfg_data/dazuoguajiData/dazuoguajiData.h"
#include "cfg_data/itemData/itemData.h"

//-------------------------------------------------------------
//------------------------------ 
CScenePlayer::CScenePlayer()
{
	m_stUnitID.uUnitType= GameObject_Player;
	m_uGhostObject		= -1 ;

	memset(m_uSaveOrder,0,sizeof(m_uSaveOrder));

	memset(m_arrNetHandler,0,sizeof(m_arrNetHandler));
	memset(m_arrPlayerFun,0,sizeof(m_arrPlayerFun));
	registerSystem(new CPlayerSkill(*this),P_C2S_Protocol_Skill );
	registerSystem(new CPlayerBuff(*this),P_C2S_Protocol_Buff );
	//registerSystem(new CPlayerFaction(*this),P_C2S_Protocol_Faction );
	//m_pPlayerPrize = new CPlayerPrize(*this) ;
	//registerSystem(m_pPlayerPrize,P_C2S_Protocol_Prize);
	registerSystem(new CPlayerItem(*this), P_C2S_Protocol_Item);

	m_pPlayerCommon = new CPlayerCommon(*this) ;
	registerSystem(m_pPlayerCommon,P_C2S_Protocol_Common);
	//m_pPlayerCounter = new CPlayerCounter(*this) ;
	//registerSystem(m_pPlayerCounter);
	//m_pPlayerQuestMgr	 = new CPlayerQuestMgr(*this) ;
	//registerSystem(m_pPlayerQuestMgr,P_C2S_Protocol_Quest);

	m_pPlayerMail = new CPlayerMail(*this);
	CHECK(m_pPlayerMail != NULL);
	registerSystem(m_pPlayerMail, P_C2S_Protocol_Mail);

	//m_pPlayerStore = new CPlayerStore(*this);
	//registerSystem(m_pPlayerStore, P_C2S_Protocol_Store);
	
	m_pPlayerCopymap = new CPlayerCopymap(*this);
	registerSystem(m_pPlayerCopymap, P_C2S_Protocol_Copymap);
	//registerSystem(new CPlayerAuction(*this), P_C2S_Protocol_Auction);
	//registerSystem(new CPlayerTrade(*this), P_C2S_Protocol_Trade);
	//registerSystem(new CPlayerSaleMoney(*this), P_C2S_Protocol_SaleMoney);
	//registerSystem(new CPlayerTopList(*this), P_C2S_Protocol_TopList);
	//registerSystem(new CPlayerFriend(*this), P_C2S_Protocol_Friend);
	//registerSystem(new CPlayerFashion(*this), P_C2S_Protocol_Fashion);
	//registerSystem(new CPlayerDropItem(*this), P_C2S_Protocol_DropItem);
	//registerSystem(new CPlayerFactionBoss(*this), P_C2S_Protocol_FactionBoss);
	//registerSystem(new CPlayerQuestion(*this), P_C2S_Protocol_AnswerQuestion);
	//m_pPlayerDart = new CPlayerDart(*this);
	//registerSystem(m_pPlayerDart, P_C2S_Protocol_Dart);

	//m_pPlayerTeam = new CPlayerTeam(*this);
	//CHECKL(m_pPlayerTeam != NULL);
	//registerSystem(m_pPlayerTeam, P_C2S_Protocol_Team);

	//m_pPlayerWulue = new CPlayerWulue(*this);
	//CHECKL(m_pPlayerWulue != NULL);
	//registerSystem(m_pPlayerWulue, P_C2S_Protocol_Wulue);

	//m_pPlayerIdentity = new CPlayerIdentity(*this) ;
	//CHECKL(m_pPlayerIdentity != NULL);
	//registerSystem(m_pPlayerIdentity,P_C2S_Protocol_Identity);

	//m_pPlayerWuxue = new CPlayerWuxue(*this) ;
	//CHECKL(m_pPlayerWuxue != NULL);
	//registerSystem(m_pPlayerWuxue,P_C2S_Protocol_Wuxue);

	m_pPlayerSystemSwitch = new CPlayerSystemSwitch(*this) ;
	registerSystem(m_pPlayerSystemSwitch);

	//m_pPlayerColleton = new CPlayerColletion(*this) ;
	//registerSystem(m_pPlayerColleton);

	//m_pPlayerTitle = new CPlayerTitle(*this) ;
	//registerSystem(m_pPlayerTitle,P_C2S_Protocol_Title);

	//m_pPlayerEquipColumn = new CPlayerEquipColumn(*this) ;
	//registerSystem(m_pPlayerEquipColumn,P_C2S_Protocol_EquipColumn);

	//m_pPlayerClimbTower = new CPlayerClimbTower(*this) ;
	//registerSystem(m_pPlayerClimbTower,P_C2S_Protocol_ClimbTower);

	//m_pPlayerPvpWar = new CPlayerPvpWar(*this) ;
	//registerSystem(m_pPlayerPvpWar,P_C2S_Protocol_PvpWar);
	//
	//m_pPlayerGreatWallTrain = new CPlayerGreatWallTrain(*this) ;
	//registerSystem(m_pPlayerGreatWallTrain,P_C2S_Protocol_GreatWallTrain);

	//m_pPlayerWorldBoss = new CPlayerWorldBoss(*this);
	//registerSystem(m_pPlayerWorldBoss, P_C2S_Protocol_WorldBoss);
	//
	//m_pPlayerCorps = new CPlayerCorps(*this) ;
	//registerSystem(m_pPlayerCorps,P_C2S_Protocol_Corps);

	//m_pPlayerActivity = new CPlayerActivity(*this) ;
	//registerSystem(m_pPlayerActivity,P_C2S_Protocol_Activity);
	//
	//m_pPlayerRelive = new CPlayerRelive(*this) ;
	//registerSystem(m_pPlayerRelive,P_C2S_Protocol_Relive);

	//m_pPlayerWentao = new CPlayerWenTao(*this) ;
	//registerSystem(m_pPlayerWentao,P_C2S_Protocol_Wentao);

	//m_pPlayerWeal = new CPlayerWeal(*this) ;
	//registerSystem(m_pPlayerWeal,P_C2S_Protocol_Weal);

	//m_pPlayerManor = new CPlayerManor(*this) ;
	//registerSystem(m_pPlayerManor,P_C2S_Protocol_Manor);

	//m_pPlayerSinglePvp = new CPlayerSinglePvp (*this) ;
	//registerSystem(m_pPlayerSinglePvp ,P_C2S_Protocol_SinglePvp);
	
	initialize();
}
//-------------------------------------------------------------
//------------------------------ 
void	CScenePlayer::registerSystem	(CGamePlayerFun* pPlayerFun,int32 uMainProctocal)
{
	if ( !pPlayerFun )
		return ;

	if ( uMainProctocal >= 0 && uMainProctocal < _emPC2S_Protocol_ARRAYSIZE )
		m_arrNetHandler[uMainProctocal] = pPlayerFun;

	if ( pPlayerFun->getFunType() < _emGameSystemType_ARRAYSIZE )
		m_arrPlayerFun[ pPlayerFun->getFunType()] = pPlayerFun;
}
//-------------------------------------------------------------
//------------------------------ 
CScenePlayer::~CScenePlayer()
{
	for ( int32 i = 0 ; i < _emGameSystemType_ARRAYSIZE ; ++i )
	{
		safe_delete(m_arrPlayerFun[i]);
	}
}
//-------------------------------------------------------------
//------------------------------ 
bool	CScenePlayer::initialize	()
{
	Parent::initialize();

	for ( int32 i = 0 ; i < _emGameSystemType_ARRAYSIZE ; ++i )
	{
		CGamePlayerFun* pFun = m_arrPlayerFun[i];
		if ( pFun )
			pFun->initialize();
	}
	m_uSaveNextTime		= (uint64)getTime() + Player_SaveTick;
	m_bBuild			= false;
	m_uBuildTime		= 0;
	m_pPacket			= NULL;
	m_pbAccount.Clear();
	m_stPlayerBase.initPlayer();
	m_mapEnermyList.clear();
	m_mapSelfEnermyList.clear();
	m_uLoginState = _GLS_Max;
	m_uCopymapState		= 0;	
	m_bUpdateParantSvr	= false;
	m_bTransfer			= false;
	m_pPlayerLevelModel = NULL;
	m_pPromotionModel = NULL;
	m_uOfflineTime = 0;
	m_uAutoReliveTime = 0;
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
void	CScenePlayer::refreshAttribute	()
{
	Parent::refreshAttribute();
}

//-------------------------------------------------------------
//------------------------------ 
void	CScenePlayer::loadDataFromProtobuf(const PBPlayerData&pbPlayer,bool bLogin)
{
	m_pbAccount.CopyFrom(pbPlayer.accountinfo());
	m_stPlayerBase.loadFromProtobuf(pbPlayer.playerbase());
	
	//读取数据到sceneunit
	m_uJob							= (uint8)m_stPlayerBase.uJob;
	m_uCampID						= (uint8)m_stPlayerBase.uCampID;
	m_pPlayerLevelModel		= g_clPlayerData.findLevel(m_stPlayerBase.uLevel);
	CHECKL(m_pPlayerLevelModel != NULL);
	m_pJobModel						= g_clJobData.findJobModel(m_uJob);
	CHECK(m_pJobModel != NULL);
	m_pPromotionModel				= g_clPromotionData.findByLevel(m_uJob, getLevel());
	CHECKL(m_pPromotionModel != NULL);
	setCampID(0);
	m_mapEnergy = m_stPlayerBase.mapEnergy;

	//读取角色信息
	for ( int32 i = 0 ; i < _emGameSystemType_ARRAYSIZE ; ++i )
	{
		CGamePlayerFun* pFun = m_arrPlayerFun[i];
		if ( pFun )
			pFun->loadFromProtobuf(pbPlayer);
	}

	m_uSaveOrder[_PInfo_Base]		= pbPlayer.playerbase().saveorder();
	m_uSaveOrder[_PInfo_System]		= pbPlayer.playersystem().saveorder();
	m_uSaveOrder[_PInfo_Bag]		= pbPlayer.playerbag().saveorder();
	m_uSaveOrder[_PInfo_Client]		= pbPlayer.clientdata().saveorder();
	m_uSaveOrder[_PInfo_Quest]		= pbPlayer.playerquest().saveorder();
	m_uSaveOrder[_PInfo_Mail]		= pbPlayer.playermail().saveorder();
	m_uSaveOrder[_PInfo_Friend]		= pbPlayer.playerfriend().saveorder();
	m_uSaveOrder[_PInfo_Faction]	= pbPlayer.playerfaction().saveorder();
}
//-------------------------------------------------------------
//------------------------------ 保存内存数据到base
void	CScenePlayer::saveMemoryToBase()
{
	if ( m_pSceneCell )
	{
		m_stPlayerBase.fPosX			= getPosition().x;
		m_stPlayerBase.fPosY			= getPosition().y;
		m_stPlayerBase.fDir				= getDirection();
		m_stPlayerBase.uSceneID			= getSceneID();
		m_stPlayerBase.uSceneCellID		= getSceneCellID();
		m_stPlayerBase.uSceneDynID		= getSceneDynID();
		m_stPlayerBase.nCurHP			= getHP();
		m_stPlayerBase.uCampID			= getCampID();
		m_stPlayerBase.mapEnergy		= m_mapEnergy;

		if ( !m_pSceneCell->getCopymapModel() )
		{
			m_stPlayerBase.uOldSceneID		= getSceneID();
			m_stPlayerBase.uOldSceneCellID	= getSceneCellID();
			m_stPlayerBase.uOldSceneDynID	= getSceneDynID();
			m_stPlayerBase.fOldPosX			= getPosition().x;
			m_stPlayerBase.fOldPosY			= getPosition().y;
			m_stPlayerBase.fOldDir			= getDirection();
		}
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CScenePlayer::saveDataToProtobuf(PBPlayerData&pbPlayer)
{
	pbPlayer.mutable_accountinfo()->CopyFrom(m_pbAccount);
	
	{
		saveMemoryToBase();
		m_stPlayerBase.saveToProtobuf(*pbPlayer.mutable_playerbase());
	}

	{//保存角色信息
		for ( int32 i = 0 ; i < _emGameSystemType_ARRAYSIZE ; ++i )
		{
			CGamePlayerFun* pFun = m_arrPlayerFun[i];
			if ( pFun )
				pFun->saveToProtobuf(pbPlayer);
		}
	}

	pbPlayer.mutable_playerbase()->set_saveorder(++m_uSaveOrder[_PInfo_Base] );
	pbPlayer.mutable_playersystem()->set_saveorder(++m_uSaveOrder[_PInfo_System] );
	pbPlayer.mutable_playerbag()->set_saveorder(++m_uSaveOrder[_PInfo_Bag] );
	pbPlayer.mutable_clientdata()->set_saveorder(++m_uSaveOrder[_PInfo_Client] );
	pbPlayer.mutable_playerquest()->set_saveorder(++m_uSaveOrder[_PInfo_Quest] );
	pbPlayer.mutable_playermail()->set_saveorder(++m_uSaveOrder[_PInfo_Mail] );
	pbPlayer.mutable_playerfriend()->set_saveorder(++m_uSaveOrder[_PInfo_Friend] );
	pbPlayer.mutable_playerfaction()->set_saveorder(++m_uSaveOrder[_PInfo_Faction] );
}
//-------------------------------------------------------------
//------------------------------ 构建角色
bool	CScenePlayer::buildPlayer	(const PBPlayerData&pbPlayer,bool bLogin)
{
	loadDataFromProtobuf(pbPlayer,bLogin);

	bool bNewOnline					= false;
	if (0 == m_stPlayerBase.uLastLoginTime)
		bNewOnline = true;

	m_uBuildTime					= (uint64)getTime();
	m_stPlayerBase.uLastLoginTime	= getTime();
	m_bBuild						= true;
	m_pPacket						= NULL;
	

	//完成回调
	onBuildComplete();

	//跨天
	int32 iOfflineDay = diffTimeDay(m_stPlayerBase.uLastLogoutTime,m_uBuildTime);
	if(iOfflineDay > 0)
		dailyCleanup(m_uBuildTime,iOfflineDay);

	//跨周
	int32 iOfflineWeek = diffTimeWeek(m_stPlayerBase.uLastLogoutTime,m_uBuildTime);
	if(iOfflineWeek > 0)
		weekCleanup(m_uBuildTime,iOfflineWeek);

	//跨月
	int32 iOfflineMonth = diffTimeMonth(m_stPlayerBase.uLastLogoutTime,m_uBuildTime);
	if(iOfflineMonth > 0)
		monthCleanup(m_uBuildTime,iOfflineMonth);

	onPassLogicDayClearup(m_stPlayerBase.uLastLogoutTime, m_uBuildTime);

	if (bLogin)
	{
		//登入处理
		onLogin();
	}

	//为了逻辑正常
	if( getLevel() >= (uint32)g_clConstantData.Game_GloBalCacheLevel)
		setUpdateParantSvr();

	//登入后刷新属性
	calculateAttributeBase();
	setHP(m_stPlayerBase.nCurHP);

	if (bNewOnline)
		newOnline();

	if ( getHP()<= 0 )
		addState(StateType_Dead);

	updateAvater();
	return true;
}

//-------------------------------------------------------------
//------------------------------ 新角色
void	CScenePlayer::newOnline()
{
	if (m_pJobModel)
	{
		for (uint8 i = 0; i < m_pJobModel->vecEnergy.size(); i++)
			registerEnergy(m_pJobModel->vecEnergy[i], m_stPlayerBase.mapEnergy[m_pJobModel->vecEnergy[i]]);
	}

	//给等级
	m_stPlayerBase.uLevel = 0;
	addLevel(1,DoingType_Birth);

	const _stBirthModel* pBirthModel = g_clBirthData.find(getJob());
	CHECK(pBirthModel != NULL);

	//给道具
	if (!pBirthModel->mapItem.empty())
	{
		CONST_ITER_MAP_U32_U16 _pos = pBirthModel->mapItem.begin();
		for (; _pos != pBirthModel->mapItem.end(); ++_pos)
		{
			getGameItemMgr().addItem(_pos->first,_pos->second,true,false,false,DoingType_Birth);
		}
	}

	//给装备
	if (!pBirthModel->mapEquip.empty())
	{
		CONST_ITER_MAP_U8_U32 _pos = pBirthModel->mapEquip.begin();
		for (; _pos != pBirthModel->mapEquip.end(); ++_pos)
		{
			getGameItemMgr().addEquipItem(_pos->second, DoingType_Birth);
		}
	}

	//给初始任务
	if (pBirthModel->uQuestID)
	{
		//CPlayerQuestMgr* pQuestMgr = getPlayerQuestMgr();
		//if ( pQuestMgr )
		//{
		//	uint8 uCode = 0;
		//	pQuestMgr->acceptQuest(pBirthModel->uQuestID,uCode);
		//}
	}

	for ( int32 i = 0 ; i < _emGameSystemType_ARRAYSIZE ; ++i )
	{
		CGamePlayerFun* pFun = m_arrPlayerFun[i];
		if ( pFun )
			pFun->newOnline();
	}

	//设置当前血上限
	setHP(getHPMax());

	//初始化装备活力值
	getGameItemMgr().newplayerMakeVigour();
	//初始化装备打造等级
	getGameItemMgr().newPlayerSkilled();

}
//-------------------------------------------------------------
//------------------------------ 
bool	CScenePlayer::logout()
{
	//登出处理
	for ( int32 i = 0 ; i < _emGameSystemType_ARRAYSIZE ; ++i )
	{
		CGamePlayerFun* pFun = m_arrPlayerFun[i];
		if ( pFun )
			pFun->onLogout();
	}

	logoutLeave();

	{
		PBG2WPlayerLogout ack;
		ack.set_playerid(getPlayerID());
		ack.set_loginsn(getLoginSN());
		sendMsgToWorld(P_G2W_Protocol_Player,G2W_Player_Logout,&ack);

	}
	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool	CScenePlayer::logoutLeave()
{
	//存离线时间
	m_stPlayerBase.uLastLogoutTime = getTime();

	//保存数据
	savePlayer((uint64)getTime());

	logoutScene();

	if(m_pPacket)
		m_pPacket->bindPlayer(NULL);
	setClientPacket(NULL);

	g_pGamePlayerMgr->onPlayerRemove(getPlayerID(),getLoginSN(),true);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
void	CScenePlayer::logoutScene			()
{
	if(m_pSceneCell)
		m_pSceneCell->playerLeaveScene(this);

	m_pSceneCell = NULL;
}
//-------------------------------------------------------------
//------------------------------ 
void	CScenePlayer::unregisterObject()
{
	m_bBuild	= false;

	Parent::unregisterObject();
}
//-------------------------------------------------------------
//------------------------------ 保存角色信息
void	CScenePlayer::savePlayer	(uint64 uTime,bool bNow)
{
	m_uSaveNextTime	= uTime + Player_SaveTick ;
	if(!m_bBuild)
	{
		CLog::error("CScenePlayer::savePlayer(%u,%d,%d)",getPlayerID(),m_bBuild,m_bRegister);
		return;
	}

	uint32 uSaveOrder =0;
	PBPlayerData pbPlayer;
	saveDataToProtobuf(pbPlayer);
	CNetStream clBuffer;

	// 保存基本信息
	clBuffer.initPosition();
	uSaveOrder = pbPlayer.playerbase().saveorder()+1;
	pbPlayer.mutable_playerbase()->set_saveorder(uSaveOrder);
	CHECK(clBuffer.SerializeToStream(pbPlayer.playerbase()));
	g_pGameSave->playerSave(getLogicWorldID(),getPlayerID(),_PInfo_Base,uSaveOrder,clBuffer.getBuffer(),clBuffer.getPosition(),bNow);

	// 保存系统信息
	clBuffer.initPosition();
	uSaveOrder = pbPlayer.playersystem().saveorder()+1;
	pbPlayer.mutable_playersystem()->set_saveorder(uSaveOrder);
	CHECK(clBuffer.SerializeToStream(pbPlayer.playersystem()));
	g_pGameSave->playerSave(getLogicWorldID(),getPlayerID(),_PInfo_System,uSaveOrder,clBuffer.getBuffer(),clBuffer.getPosition(),bNow);

	// 保存背包信息
	clBuffer.initPosition();
	uSaveOrder = pbPlayer.playerbag().saveorder()+1;
	pbPlayer.mutable_playerbag()->set_saveorder(uSaveOrder);
	CHECK(clBuffer.SerializeToStream(pbPlayer.playerbag()));
	g_pGameSave->playerSave(getLogicWorldID(),getPlayerID(),_PInfo_Bag,uSaveOrder,clBuffer.getBuffer(),clBuffer.getPosition(),bNow);

	// 保存前端信息
	clBuffer.initPosition();
	uSaveOrder = pbPlayer.clientdata().saveorder()+1;
	pbPlayer.mutable_clientdata()->set_saveorder(uSaveOrder);
	CHECK(clBuffer.SerializeToStream(pbPlayer.clientdata()));
	g_pGameSave->playerSave(getLogicWorldID(),getPlayerID(),_PInfo_Client,pbPlayer.clientdata().saveorder(),clBuffer.getBuffer(),clBuffer.getPosition(),bNow);

	// 保存任务信息
	clBuffer.initPosition();
	uSaveOrder = pbPlayer.playerquest().saveorder()+1;
	pbPlayer.mutable_playerquest()->set_saveorder(uSaveOrder);
	CHECK(clBuffer.SerializeToStream(pbPlayer.playerquest()));
	g_pGameSave->playerSave(getLogicWorldID(),getPlayerID(),_PInfo_Quest,pbPlayer.playerquest().saveorder(),clBuffer.getBuffer(),clBuffer.getPosition(),bNow);

	// 保存邮件信息
	clBuffer.initPosition();
	uSaveOrder = pbPlayer.playermail().saveorder()+1;
	pbPlayer.mutable_playermail()->set_saveorder(uSaveOrder);
	CHECK(clBuffer.SerializeToStream(pbPlayer.playermail()));
	g_pGameSave->playerSave(getLogicWorldID(),getPlayerID(),_PInfo_Mail,pbPlayer.playermail().saveorder(),clBuffer.getBuffer(),clBuffer.getPosition(),bNow);

	// 保存好友信息
	clBuffer.initPosition();
	uSaveOrder = pbPlayer.playerfriend().saveorder()+1;
	pbPlayer.mutable_playerfriend()->set_saveorder(uSaveOrder);
	CHECK(clBuffer.SerializeToStream(pbPlayer.playerfriend()));
	g_pGameSave->playerSave(getLogicWorldID(),getPlayerID(),_PInfo_Friend,pbPlayer.playerfriend().saveorder(),clBuffer.getBuffer(),clBuffer.getPosition(),bNow);

	// 保存帮会信息
	clBuffer.initPosition();
	uSaveOrder = pbPlayer.playerfaction().saveorder()+1;
	pbPlayer.mutable_playerfaction()->set_saveorder(uSaveOrder);
	CHECK(clBuffer.SerializeToStream(pbPlayer.playerfaction()));
	g_pGameSave->playerSave(getLogicWorldID(),getPlayerID(),_PInfo_Faction,pbPlayer.playerfaction().saveorder(),clBuffer.getBuffer(),clBuffer.getPosition(),bNow);
}

//-------------------------------------------------------------
//------------------------------
bool	CScenePlayer::functionNetPacket(uint8 uMainProtocol,uint8 uProtocol,uint8 uEventFlag,CNetStream& clStream)
{
	if ( uMainProtocol >= _emPC2S_Protocol_ARRAYSIZE )
		return false;

	CGamePlayerFun* pPlayerFun = m_arrNetHandler[uMainProtocol];
	if ( !pPlayerFun )
		return false;

	if (g_pServer->checkGameSwitch(GameSwitch_DebugMsg) && 
		g_pServer->checkDebugMsgSwitch(uMainProtocol) &&
		!g_pServer->checkDebugMsgFilter(uMainProtocol, uProtocol))
	{
		const std::string& sMain = _emPC2S_Protocol_Name((_emPC2S_Protocol)uMainProtocol);
		const std::string& sub = findSubProtocol_C2S(uMainProtocol, uProtocol);
		CLog::print("C2S playerid[%d] main[%s] sub[%s]", getPlayerID(), sMain.c_str(), sub.c_str());
	}
	
	if(!checkSystemSwitchOpen(uMainProtocol, uProtocol)) // 系统功能关闭
	{
		sendMsg(P_S2C_Protocol_Common,S2C_Common_SystemSwitch, NULL, R_ResultCommon_SystemClose);
		return true;
	}

	//检查跨服限制
	if (g_pServer->isBigWorldServer())
	{
		if (g_clSystemSwitchData.isBigWorldLimitProtocol(uMainProtocol, uProtocol))
		{
			sendMsg(P_S2C_Protocol_Common,S2C_Common_SystemSwitch, NULL, R_ResultCommon_BigWorldLimit);
			return true;
		}
	}

	return pPlayerFun->netPacket(uProtocol,uEventFlag,clStream);
}

//-------------------------------------------------------------
//------------------------------
bool CScenePlayer::checkSystemSwitchOpen(uint8 uMainProtocol, uint8 uProtocol)
{
	if (!m_pPlayerSystemSwitch)
		return true;

	const _stSystemSwitchModel *pModel = g_clSystemSwitchData.findSystermSwitchModel(uMainProtocol,uProtocol);
	if (!pModel)
		return true;

	//判断全局开关
	if (pModel->uSwitch > 0 )
		return false;
	
	return m_pPlayerSystemSwitch->checkSwitchStateOpen(pModel->uID);
}

//-------------------------------------------------------------
//------------------------------
bool	CScenePlayer::playerFlyScene_ByCellID	(uint16 SceneID,uint32 uCellID,uint16 uRegionID)
{
	return playerFlyScene_Imp(SceneID,0,uCellID,false,uRegionID,0.0f,0.0f);
}
//-------------------------------------------------------------
//------------------------------
bool	CScenePlayer::playerFlyScene_ByDynID	(uint16 SceneID,uint64 uDynID,uint16 uRegionID)
{
	return playerFlyScene_Imp(SceneID,uDynID,0,false,uRegionID,0.0f,0.0f);
}
//-------------------------------------------------------------
//------------------------------
bool	CScenePlayer::playerFlyScene_ByCellID	(uint16 SceneID,uint32 uCellID,float32 fPosX,float32 fPosY)
{
	return playerFlyScene_Imp(SceneID,0,uCellID,true,0,fPosX,fPosY);
}
//-------------------------------------------------------------
//------------------------------
bool	CScenePlayer::playerFlyScene_ByDynID	(uint16 SceneID,uint64 uDynID,float32 fPosX,float32 fPosY)
{
	return playerFlyScene_Imp(SceneID,uDynID,0,true,0,fPosX,fPosY);
}
//-------------------------------------------------------------
//------------------------------
bool	CScenePlayer::playerFlyScene_Imp	(uint16 SceneID,uint64 uDynID,uint32 uCellID,bool bFixPos,uint16 uRegionID,float32 fPosX,float32 fPosY)
{
	if ( m_bTransfer )
		return false;

	_stSceneMapModel* pTargetSceneModel = g_clSceneMapData.find(SceneID);
	if ( !pTargetSceneModel )
		return false;
	if ( !isWorldConneted() )
		return false;

	CGameSceneCell* pTargetCell = NULL;
	if(uDynID)
	{
		pTargetCell = g_pGameSceneMgr->findSceneCell_ByDynID( SceneID,uDynID);
	}
	else if (uCellID )
	{
		pTargetCell = g_pGameSceneMgr->findSceneCell_ByCellID( SceneID,uCellID);
	}
	else if(SceneID == getSceneID())
	{
		pTargetCell = getSceneCell();
	}

	if ( pTargetCell && (pTargetCell == getSceneCell()) )
	{
		_stPoint2F stPoint;
		if (bFixPos)
		{
			stPoint.x = fPosX;
			stPoint.y = fPosY;
		}
		else
		{
			if (!pTargetCell->getSpawnPoint(stPoint,(uint8)uRegionID))
				return false;
		}

		setPosition(stPoint.x,stPoint.y,SetPositionReason_ChangePos,true);
		return true;
	}
	{
		setTransfer(true);

		PBG2WPlayerFlyMapAsk ask;
		ask.set_playerid(getPlayerID());
		ask.set_scenedynid(uDynID);
		ask.set_sceneid(SceneID);
		ask.set_scenecellid(uCellID);
		ask.set_sceneregionid(uRegionID);
		ask.set_fixpos(bFixPos);
		ask.set_posx(fPosX);
		ask.set_posy(fPosY);
		sendMsgToWorld(P_G2W_Protocol_Player,G2W_Player_FlyMapAsk,&ask);
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------
void	CScenePlayer::playerRealFlyScene(uint16 SceneID,uint64 uDynID,uint32 uCellID,bool bFixPos,uint16 uRegionID,float32 fPosX,float32 fPosY,_stSvrInfo& stTargetGameSvrInfo,_stNetHost& stTargetHost)
{
	if ( !isWorldConneted() )
		return ;

	setPlayerMark(PlayerMark_FixPoint,bFixPos);
	//保存到base
	saveMemoryToBase();
	if ( bFixPos )
	{
		m_stPlayerBase.fPosX = fPosX;
		m_stPlayerBase.fPosY = fPosY;
	}
	else
	{
		m_stPlayerBase.uSpawnID = (uRegionID?uRegionID:1);
	}

	// 本服传送
	CGameSceneCell* pCell = g_pGameSceneMgr->findSceneCell_ByDynID( SceneID,uDynID);
	if ( pCell )
	{
		setTransfer(false);

		if ( pCell == getSceneCell())
			return ;


		this->callback_flyout();

		if ( !g_pGameSceneMgr->playerEnterScene(this,SceneID,uDynID) )
			return ;

		PBG2CSceneFlySameGameSvr info;
		info.set_sceneid(getSceneID());
		info.set_scenecellid(getSceneCellID());
		info.set_scenedynid(getSceneDynID());
		info.set_unitid(getUnitID());
		PBPoint* pbPoint = info.mutable_pbpoint();
		if (pbPoint)
		{
			pbPoint->set_posx(getPosition().x);
			pbPoint->set_posy(getPosition().y);
		}
		sendMsg(P_S2C_Protocol_Common,S2C_Common_SceneFly_SameGameSvr,&info);
		return ;
	}

	this->callback_flyout();

	// 跨游戏服务器传送
	PBPlayerData pbPlayer;
	saveDataToProtobuf(pbPlayer);

	{
		CNetStream clBuffer;	
		clBuffer.write(getPlayerID());
		clBuffer.write(uint8(_PInfo_Account));
		clBuffer.SerializeToStream(pbPlayer.accountinfo());
		sendBufferToWorld(P_G2W_Protocol_Player,G2W_Player_FlyMapSynInfo,clBuffer.getBuffer(),clBuffer.getPosition());
	}

	{
		PBPlayerBase* pbBase = pbPlayer.mutable_playerbase();
		if ( pbBase )
		{
			if (bFixPos)
			{
				pbBase->set_posx(fPosX);
				pbBase->set_posy(fPosY);
			}
			else
			{
				pbBase->set_spawnid(uRegionID?uRegionID:1);
			}
			pbBase->set_sceneid(SceneID);
			pbBase->set_cellid(uCellID);
			pbBase->set_scenedynid(uDynID);
		}

		CNetStream clBuffer;
		clBuffer.write(getPlayerID());
		clBuffer.write(uint8(_PInfo_Base));
		clBuffer.SerializeToStream(pbPlayer.playerbase());
		sendBufferToWorld(P_G2W_Protocol_Player,G2W_Player_FlyMapSynInfo,clBuffer.getBuffer(),clBuffer.getPosition());
	}

	{
		CNetStream clBuffer;
		clBuffer.write(getPlayerID());
		clBuffer.write(uint8(_PInfo_System));
		clBuffer.SerializeToStream(pbPlayer.playersystem());
		sendBufferToWorld(P_G2W_Protocol_Player,G2W_Player_FlyMapSynInfo,clBuffer.getBuffer(),clBuffer.getPosition());
	}

	{
		CNetStream clBuffer;
		clBuffer.write(getPlayerID());
		clBuffer.write(uint8(_PInfo_Bag));
		clBuffer.SerializeToStream(pbPlayer.playerbag());
		sendBufferToWorld(P_G2W_Protocol_Player,G2W_Player_FlyMapSynInfo,clBuffer.getBuffer(),clBuffer.getPosition());
	}

	{
		CNetStream clBuffer;
		clBuffer.write(getPlayerID());
		clBuffer.write(uint8(_PInfo_Client));
		clBuffer.SerializeToStream(pbPlayer.clientdata());
		sendBufferToWorld(P_G2W_Protocol_Player,G2W_Player_FlyMapSynInfo,clBuffer.getBuffer(),clBuffer.getPosition());
	}

	{
		CNetStream clBuffer;
		clBuffer.write(getPlayerID());
		clBuffer.write(uint8(_PInfo_Quest));
		clBuffer.SerializeToStream(pbPlayer.playerquest());
		sendBufferToWorld(P_G2W_Protocol_Player,G2W_Player_FlyMapSynInfo,clBuffer.getBuffer(),clBuffer.getPosition());
	}

	{
		CNetStream clBuffer;
		clBuffer.write(getPlayerID());
		clBuffer.write(uint8(_PInfo_Mail));
		clBuffer.SerializeToStream(pbPlayer.playermail());
		sendBufferToWorld(P_G2W_Protocol_Player,G2W_Player_FlyMapSynInfo,clBuffer.getBuffer(),clBuffer.getPosition());
	}

	{
		CNetStream clBuffer;
		clBuffer.write(getPlayerID());
		clBuffer.write(uint8(_PInfo_Friend));
		clBuffer.SerializeToStream(pbPlayer.playerfriend());
		sendBufferToWorld(P_G2W_Protocol_Player,G2W_Player_FlyMapSynInfo,clBuffer.getBuffer(),clBuffer.getPosition());
	}

	{
		CNetStream clBuffer;
		clBuffer.write(getPlayerID());
		clBuffer.write(uint8(_PInfo_Faction));
		clBuffer.SerializeToStream(pbPlayer.playerfaction());
		sendBufferToWorld(P_G2W_Protocol_Player,G2W_Player_FlyMapSynInfo,clBuffer.getBuffer(),clBuffer.getPosition());
	}

	{// 通知前端连接新的游戏服务器
		PBG2CSceneFlyJumpGameSvr info;
		info.set_host(stTargetHost.getHost());
		info.set_port(stTargetHost.getPort());
		sendMsg(P_S2C_Protocol_Common,S2C_Common_SceneFly_JumpGameSvr,&info);
	}

	logoutLeave();
}

void	CScenePlayer::process_move			(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay)
{
	if (haveState(StateType_Qinggong))
	{
		//
		float32 fX;
		float32 fY;
		uint32  uNextIndex;
		uint32	uMs2NextPoint;
		if (getSceneMap()
			&& getSceneMap()->findQinggongPoint(m_uQinggongID, (uint32)(uTick-m_uQinggongStartTick), uNextIndex, uMs2NextPoint, fX, fY))
		{
			setPosition(fX,fY,SetPositionReason_Qinggong,false);
		}
		else
		{
			delState(StateType_Qinggong);
		}
	}
	else
	{
		Parent::process_move(uTick, uTime, uSecond, bNewDay);
	}
}

//-------------------------------------------------------------
//------------------------------
uint16	CScenePlayer::testRegionReturnId				(uint8 uAttrType)const
{
	return m_pRegion->testMarkReturnID(uAttrType);
}

//-------------------------------------------------------------
//------------------------------ 
void	CScenePlayer::processLogic(uint64 uTick,uint64 uTime,uint32 uSecond,bool bNewDay)
{
	if ( m_stPlayerBase.uKillTime && ( uTime > m_stPlayerBase.uKillTime + g_clConstantData.Game_KillTime) )
	{
		m_stPlayerBase.uKillTime = 0;
		setGhost(_BIT32(Ghost_Pk));
	}

	Parent::processLogic(uTick,uTime,uSecond,bNewDay);
	for ( int32 i = 0 ; i < _emGameSystemType_ARRAYSIZE ; ++i )
	{
		CGamePlayerFun* pFun = m_arrPlayerFun[i];
		if ( pFun )
		{
			pFun->CTickDown::process(uTick);
		}	
	}
	
	synGhost(uTime);

	// 新的一天
	if ( bNewDay )
	{
		PBG2CCommon_TimeEvent msg;
		msg.set_newday(true);
		// 跨天
		dailyCleanup(uTime,1);

		// 跨周
		int32 iOfflineWeek = diffTimeWeek(m_stPlayerBase.uLastLogoutTime,uTime);
		if ( iOfflineWeek > 0 )
		{
			weekCleanup(uTime,iOfflineWeek);
			msg.set_newweek(true);
		}

		// 跨月
		int32 iOfflineMonth = diffTimeMonth(m_stPlayerBase.uLastLogoutTime,uTime);
		if ( iOfflineWeek > 0 )
		{
			monthCleanup(uTime,iOfflineMonth);
			msg.set_newmonth(true);
		}

		sendMsg(P_S2C_Protocol_Common,S2C_Common_TimeEvent,&msg);
	}

	static uint64 uProcessTime	= 0;
	if (!uProcessTime)
		uProcessTime	= uTime;
	
	onPassLogicDayClearup(uProcessTime, uTime);
	uProcessTime = uTime;

	//敌人列表消失时间
	processEnermyList(uTick);
	m_GameItemMgr.process(uTick,uTime,uSecond,bNewDay);
	processUpdateGlobal();
	processAutoRelive(uTime);
	if(m_uSaveNextTime <= uTime)
		 savePlayer(uTime,false);
}
//-------------------------------------------------------------
//------------------------------ 
void	CScenePlayer::processUpdateValue		(uint64 uTime)
{
	CSceneUnit::processUpdateValue(uTime);
	//更新计算外形
	if(m_bUpdateValue[_UV_Shape])
	{
		m_bUpdateValue[_UV_Shape] = false;
		updateAvater();
	}
}
//-------------------------------------------------------------
//------------------------------ 发送角色数据
void	CScenePlayer::sendPlayer()
{
	if(!m_pPacket)
		return ;

	SetGLoginState(_GLS_PlayerSend);
	PBPlayerData pbPlayer;
	saveDataToProtobuf(pbPlayer);
	sendMsg(P_S2C_Protocol_Player,	S2C_Player_BaseInfo		,	&pbPlayer.playerbase());
	sendMsg(P_S2C_Protocol_Player,	S2C_Player_SystemInfo	,	&pbPlayer.playersystem());
	sendMsg(P_S2C_Protocol_Player,	S2C_Player_BagInfo		,	&pbPlayer.playerbag());
	sendMsg(P_S2C_Protocol_Player,	S2C_Player_ClientInfo	,	&pbPlayer.clientdata());
	//sendMsg(P_S2C_Protocol_Player,S2C_Player_QuestInfo	,	&pbPlayer.playerquest()); // 应前端需求 把该消息放至前端进入场景后发送了 CPlayerQuestMgr::onClientGo
	sendMsg(P_S2C_Protocol_Player,	S2C_Player_Mail	,			&pbPlayer.playermail());
	//sendMsg(P_S2C_Protocol_Player,S2C_Player_Friend		,	&pbPlayer.playerfriend());
	sendMsg(P_S2C_Protocol_Player,	S2C_Player_Faction		,	&pbPlayer.playerfaction());
	//CPlayerFriend* pPlayerFriend = (CPlayerFriend*)getPlayerFun(GameSystemType_Friend);
	//if (pPlayerFriend)
	//{
	//	pPlayerFriend->queryFriendOnlineInfoFromGlobal();
	//}


	SetGLoginState(_GLS_PlayerSendOver);

	{
		//转发到全局服
		_stPlayerDisplay& stDisplay = m_stPlayerBase.getDisplay();
		PBPlayerDisplay msg;
		stDisplay.saveToProtobuf(msg);
		sendMsgToGlobal(P_G2GL_Protocol_Player, G2GL_Player_Syn,&msg );
	}

	{
		//转发到世界服
		_stPlayerDisplay& stDisplay = m_stPlayerBase.getDisplay();
		PBPlayerDisplay msg;
		stDisplay.saveToProtobuf(msg);
		sendMsgToWorld(P_G2W_Protocol_Player, G2W_Player_Syn,&msg );
	}
}

//-------------------------------------------------------------
//------------------------------
void CScenePlayer::SetGLoginState(uint8 uState)
{
	m_uLoginState = uState;
	CLog::print("角色[%u],名称[%s],%s",getPlayerID(),getName().c_str(),g_szGameLoginState[uState]);
}

//-------------------------------------------------------------
//------------------------------ 
//#define  _SYN_GHOST_
#ifndef _SYN_GHOST_ 
void	CScenePlayer::synGhost(uint64 uTime)
{
	if(!m_pPacket)
		return;

	if (haveState(StateType_WaitReady) )
		return ;
// 
// 	if ( haveState(StateType_Plane)  )
// 	{
// 		/*基础属性*/
// 		if(_CHECK_BIT(m_uUpdateMark,_BIT32(Ghost_Property)))
// 		{
// 			_SET_BIT32(m_uUpdateMark,Ghost_Property,false);
// 			PBG2CCommon_PlaneUpdate msg;
// 			PBGhostGameAttr* pInfo = msg.mutable_attr();
// 			if (pInfo)
// 			{
// 				for ( int32 i = 0 ; i < _emBattleAttribute_ARRAYSIZE ; ++i )
// 				{
// 					pInfo->add_attr(m_stGameAttributeBase.iAttribute[i]);
// 				}
// 			}
// 			sendMsg(P_S2C_Protocol_Common,S2C_Common_PlaneUpdate,&msg);
// 		}
// 		return ;
// 	}

	//更新自身信息
	if(m_uUpdateMark && !_CHECK_BIT(m_uUpdateMark,_BIT32(Ghost_Remove)))
	{
		PBGhostInfo pbInfo;
		if(packetGhost(uTime,true,m_uUpdateMark,pbInfo))
		{
			sendMsg(P_S2C_Protocol_Common,S2C_Common_Ghost,&pbInfo);
		}
		m_uUpdateMark = 0;
	}

	//更新周边对像信息
	MAP_GHOST::iterator _pos = m_mapGhost.begin();
	MAP_GHOST::iterator _end = m_mapGhost.end();
	while(_pos != _end)
	{
		_stGhost&stGhost = (_pos->second);
		/*无可更新内容 或者 对我隐身*/ 
		if(stGhost.updateMark == 0 
			|| (stGhost.pObject && !isVisible(stGhost.pObject))
			)
		{
			++_pos;
			continue;
		}

		/*是否被移除(隐身也视为移除)*/ 
		if(!stGhost.pObject || stGhost.testMark(Ghost_Remove) )
		{
			PBG2CCommonUnitRemove ack;
			ack.set_unittype(stGhost.stGameUnitID.uUnitType);
			ack.set_unitid(stGhost.stGameUnitID.uUnitID);
			sendMsg(P_S2C_Protocol_Common,S2C_Common_Unit_Remove,&ack);
		}
		else
		{/*打包数据*/ 
			PBGhostInfo pbInfo;
			if(stGhost.pObject->packetGhost(uTime,false,stGhost.updateMark,pbInfo) )
			{
				sendMsg(P_S2C_Protocol_Common,S2C_Common_Ghost,&pbInfo);
			}
		}

		++_pos;
		//对像不是隐身或移除时设置
		if(stGhost.pObject && !stGhost.testMark(Ghost_Remove))
			stGhost.updateMark = 0;
	}
}

#else //_SYN_GHOST 

void	CScenePlayer::synGhost(uint64 uTime)
{
	if(!m_pPacket)
		return;

	if (haveState(StateType_WaitReady) )
		return ;

	if ( haveState(StateType_Plane)  )
	{
		/*基础属性*/
		if(_CHECK_BIT(m_uUpdateMark,_BIT32(Ghost_Property)))
		{
			_SET_BIT32(m_uUpdateMark,Ghost_Property,false);
			PBG2CCommon_PlaneUpdate msg;
			PBGhostGameAttr* pInfo = msg.mutable_attr();
			if (pInfo)
			{
				for ( int32 i = 0 ; i < _emBattleAttribute_ARRAYSIZE ; ++i )
				{
					pInfo->add_attr(m_stGameAttributeBase.iAttribute[i]);
				}
			}
			sendMsg(P_S2C_Protocol_Common,S2C_Common_PlaneUpdate,&msg);
		}
		return ;
	}

	PBGhostList pbGhostList;

	//更新自身信息
	if(m_uUpdateMark && !_CHECK_BIT(m_uUpdateMark,_BIT32(Ghost_Remove)))
	{
		PBGhostInfo pbInfo;
		if(packetGhost(uTime,true,m_uUpdateMark,pbInfo))
		{
			//sendMsg(P_S2C_Protocol_Common,S2C_Common_Ghost,&pbInfo);
			PBGhostInfo *pInfo = pbGhostList.add_ghost_info();
			if (!pInfo) return ;
			pInfo->CopyFrom(pbInfo);

		}
		m_uUpdateMark = 0;
	}

	//更新周边对像信息
	MAP_GHOST::iterator _pos = m_mapGhost.begin();
	MAP_GHOST::iterator _end = m_mapGhost.end();
	while(_pos != _end)
	{
		_stGhost&stGhost = (_pos->second);
		/*无可更新内容 或者 对我隐身*/ 
		if(stGhost.updateMark == 0 
			|| (stGhost.pObject && !isVisible(stGhost.pObject))
			)
		{
			++_pos;
			continue;
		}

		/*是否被移除(隐身也视为移除)*/ 
		if(!stGhost.pObject || stGhost.testMark(Ghost_Remove) )
		{
			PBG2CCommonUnitRemove ack;
			ack.set_unittype(stGhost.stGameUnitID.uUnitType);
			ack.set_unitid(stGhost.stGameUnitID.uUnitID);
			sendMsg(P_S2C_Protocol_Common,S2C_Common_Unit_Remove,&ack);
		}
		else
		{
			// 打包数据
			PBGhostInfo pbInfo;

			if (stGhost.pObject->packetGhost(uTime,false,stGhost.updateMark, pbInfo) )
			{
				PBGhostInfo *pInfo = pbGhostList.add_ghost_info();
				if (!pInfo) return ;

				pInfo->CopyFrom(pbInfo);
				if (pbGhostList.ByteSize() >= (_IO_BUFFER_ - _IO_1_KB))
				{
					pInfo = pbGhostList.mutable_ghost_info()->ReleaseLast();
					sendMsg(P_S2C_Protocol_Common,S2C_Common_Ghost, &pbGhostList);

					pbGhostList.Clear();
					pbGhostList.mutable_ghost_info()->AddAllocated(pInfo);
				}			
			}
		}

		++_pos;
		//对像不是隐身或移除时设置
		if(stGhost.pObject && !stGhost.testMark(Ghost_Remove))
			stGhost.updateMark = 0;
	}
	if (pbGhostList.ghost_info_size() > 0 )
		sendMsg(P_S2C_Protocol_Common,S2C_Common_Ghost, &pbGhostList);
}
#endif//_SYN_GHOST_
//-------------------------------------------------------------
//------------------------------ 
bool	CScenePlayer::packetGhost(uint64 uTime,bool bSelf,uint32& updateMark,PBGhostInfo&pbInfo)
{
	if(_CHECK_BIT(updateMark,_BIT32(Ghost_Remove)))
		return false;

	bool bSend = Parent::packetGhost(uTime,bSelf,updateMark,pbInfo);
	PBGhostPlayer& pbPlayer= *pbInfo.mutable_player();

	//基本信息
	if(_CHECK_BIT(updateMark,_BIT32(Ghost_Base)))
	{
		bSend = true;

		PBPlayerAvatar* pAvatar = pbPlayer.mutable_avatar();
		if ( pAvatar )
		{
			m_stPlayerBase.saveToAvatar(*pAvatar);
		}
	}
	_SET_BIT32(updateMark,Ghost_Base,false);

	//Pk信息
	if(_CHECK_BIT(updateMark,_BIT32(Ghost_Pk)))
	{
		bSend = true;
		PBGhostPk* pPk = pbPlayer.mutable_pkinfo();
		if (pPk)
		{
			pPk->set_iskiller(m_stPlayerBase.uKillTime != 0);
			pPk->set_pkvalue(m_stPlayerBase.uPKValue);
		}
	}
	_SET_BIT32(updateMark,Ghost_Pk,false);

	//战斗力信息
	if(_CHECK_BIT(updateMark,_BIT32(Ghost_FightPower))&&bSelf)
	{
		bSend = true;
		PBGhostFightPower* pFightPower = pbPlayer.mutable_fightpower();
		if (pFightPower)
		{
			pFightPower->set_fightpower(m_stPlayerBase.uFightPower);
		}
	}
	_SET_BIT32(updateMark,Ghost_FightPower,false);

	return bSend;
}

//-------------------------------------------------------------
//------------------------------ 
CGamePlayerFun* CScenePlayer::getPlayerFun(_emGameSystemType uType)
{
	if (uType >=0 && uType < _emGameSystemType_ARRAYSIZE)
		return m_arrPlayerFun[uType];

	return NULL;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CScenePlayer::isClientConneted	()
{
	return m_pPacket != NULL;
}
//-------------------------------------------------------------
//------------------------------ 
bool CScenePlayer::sendMsg(uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent)
{
	if (!m_pPacket)
		return false;

	if (g_pServer->checkGameSwitch(GameSwitch_DebugMsg) && 
		g_pServer->checkDebugMsgSwitch(uMainProtocol) &&
		!g_pServer->checkDebugMsgFilter(uMainProtocol, uProtocol))
	{
		const std::string& sMain = _emPS2C_Protocol_Name((_emPS2C_Protocol)uMainProtocol);
		const std::string& sub = findSubProtocol_S2C(uMainProtocol, uProtocol);
		const std::string& sEvent = findResultCode(uMainProtocol, uEvent);
		std::string sMsg = msg ? pb2json(*msg) : "null";
		// 因为底层日志打印限制最大8K，所以这里做个size预警
		const size_t max_msg_len = 1024 * 7;
		const char toomany[] = "...too many!!!";
		if (sMsg.size() > max_msg_len)
			sMsg = sMsg.substr(0, max_msg_len).append(toomany);
		CLog::print("S2C playerid[%d] main[%s] sub[%s] event[%s] msg=[%s]", getPlayerID(), sMain.c_str(), sub.c_str(), sEvent.c_str(), sMsg.c_str());
	}
	return m_pPacket->sendMsg(uMainProtocol, uProtocol, msg, uEvent);
}
//-------------------------------------------------------------
//------------------------------ 
bool CScenePlayer::sendErrorMsg(uint8 uMainProtocol, uint8 uProtocol, uint8 uEventFlag,const char* pParam1,const char* pParam2)
{
	if (!m_pPacket)
		return false;

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool CScenePlayer::sendBuffer(uint8 uMainProtocol, uint8 uProtocol, void* pBuffer,uint32 uSize, uint8 uEvent)
{
	if (!m_pPacket)
		return false;

	return m_pPacket->sendBuffer(uMainProtocol, uProtocol, pBuffer, uSize,uEvent);
}
//-------------------------------------------------------------
//------------------------------ 
bool	CScenePlayer::isGlobalConneted	()
{
	return g_pServer->isGlobalConneted(getLogicWorldID());
}
//-------------------------------------------------------------
//------------------------------ 
bool CScenePlayer::sendMsgToGlobal(uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent)
{
	return g_pServer->sendMsgToGlobal(getLogicWorldID(),uMainProtocol,uProtocol, msg,uEvent);
}
//-------------------------------------------------------------
//------------------------------ 
bool CScenePlayer::sendBufferToGlobal(uint8 uMainProtocol, uint8 uProtocol, void* pBuffer,uint32 uSize, uint8 uEvent)
{
	return g_pServer->sendBufferToGlobal(getLogicWorldID(),uMainProtocol,uProtocol, pBuffer,uSize,uEvent);
}
//-------------------------------------------------------------
//------------------------------ 
bool	CScenePlayer::isWorldConneted	()
{
	return g_pServer->isWorldConneted(getLogicWorldID());
}
//-------------------------------------------------------------
//------------------------------ 
bool CScenePlayer::sendMsgToWorld(uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent)
{
	return g_pServer->sendMsgToWorld(getLogicWorldID(),uMainProtocol,uProtocol, msg,uEvent);
}
//-------------------------------------------------------------
//------------------------------ 
bool CScenePlayer::sendPlayerMsgToWorld(uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg/* = NULL*/, uint8 uEvent/* = 0*/)
{
	CNetStream stStream;
	stStream.write(getPlayerID());
	if (msg)
		stStream.SerializeToStream(*msg);
	return g_pServer->sendBufferToWorld(getLogicWorldID(),uMainProtocol,uProtocol, stStream.getBuffer(),stStream.getPosition(),uEvent);
}
//-------------------------------------------------------------
//------------------------------ 
bool CScenePlayer::sendMsgToBigWorld(uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg/* = NULL*/, uint8 uEvent/* = 0*/)
{
	CNetStream stStream;
	stStream.write(getPlayerID());
	if (msg)
		stStream.SerializeToStream(*msg);
	return g_pServer->sendBufferToWorld(getLogicWorldID(),P_G2W_Protocol_Common,G2W_Common_TransferMsg, stStream.getBuffer(),stStream.getPosition(),uEvent);
}
//-------------------------------------------------------------
//------------------------------ 
bool CScenePlayer::sendBufferToWorld(uint8 uMainProtocol, uint8 uProtocol, void* pBuffer,uint32 uSize, uint8 uEvent)
{
	return g_pServer->sendBufferToWorld(getLogicWorldID(),uMainProtocol,uProtocol, pBuffer,uSize,uEvent);
}
//-------------------------------------------------------------
//------------------------------ 
bool	CScenePlayer::checkOnlyIndex	(uint16 uIndex)
{
	CHECKF(uIndex != 0);
	uint16 uBit = uIndex / 10000;
	if (!uBit)
		uBit = 10000;
	if (uIndex <= 10000)
	{
		return getStringBit(m_stPlayerBase.strOnlyIndex,uBit);
	}
	else if (uIndex <= 20000)
	{
		return getStringBit(m_stPlayerBase.strOnlyDayIndex,uBit);
	}
	else if (uIndex <= 30000)
	{
		return getStringBit(m_stPlayerBase.strOnlyWeekIndex,uBit);
	}
	else if (uIndex <= 40000)
	{
		return getStringBit(m_stPlayerBase.strOnlyMonthIndex,uBit);
	}
	return false;
}

//-------------------------------------------------------------
//------------------------------ 
void	CScenePlayer::setOnlyIndex	(uint16 uIndex,bool bSet)
{
	CHECK(uIndex != 0);
	uint16 uBit = uIndex / 10000;
	if (!uBit)
		uBit = 10000;
	if (uIndex <= 10000)
	{
		setStringBit(m_stPlayerBase.strOnlyIndex,uBit,bSet);
	}
	else if (uIndex <= 20000)
	{
		setStringBit(m_stPlayerBase.strOnlyDayIndex,uBit,bSet);
	}
	else if (uIndex <= 30000)
	{
		setStringBit(m_stPlayerBase.strOnlyWeekIndex,uBit,bSet);
	}
	else if (uIndex <= 40000)
	{
		setStringBit(m_stPlayerBase.strOnlyMonthIndex,uBit,bSet);
	}
}

//-------------------------------------------------------------
//------------------------------ 
void	CScenePlayer::clearOnlyEvent	(_em_CleanupOnlyIndex emOnlyIndex )
{
	if (CleanupOnlyIndex_Daily == emOnlyIndex)
	{
		m_stPlayerBase.strOnlyDayIndex.clear();
	}
	else if (CleanupOnlyIndex_Weekly == emOnlyIndex)
	{
		m_stPlayerBase.strOnlyWeekIndex.clear();
	}
	else if (CleanupOnlyIndex_Monthly == emOnlyIndex)
	{
		m_stPlayerBase.strOnlyMonthIndex.clear();
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CScenePlayer::onBuildComplete()
{
	for ( int32 i = 0 ; i < _emGameSystemType_ARRAYSIZE ; ++i )
	{
		CGamePlayerFun* pFun = m_arrPlayerFun[i];
		if ( pFun )
			pFun->onBuildComplete();
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CScenePlayer::onLogin()
{
	for ( int32 i = 0 ; i < _emGameSystemType_ARRAYSIZE ; ++i )
	{
		CGamePlayerFun* pFun = m_arrPlayerFun[i];
		if ( pFun )
			pFun->onLogin();
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CScenePlayer::gmCleanDailyTime		()
{
	m_stPlayerBase.uDailyCleanTime = 0;
	m_stPlayerBase.uWeeklyCleanTime = 0;
	m_stPlayerBase.uMonthlyCleanTime = 0;
}
//-------------------------------------------------------------
//------------------------------ 
void	CScenePlayer::dailyCleanup	(uint64 uTime,int32 iOfflineDay)
{
	if (m_stPlayerBase.uDailyCleanTime == 0 
		|| diffTimeDay(m_stPlayerBase.uDailyCleanTime,uTime) > 0)
	{
		m_stPlayerBase.uDailyCleanTime = (uint32)uTime;

		m_stPlayerBase.mapCopymapDayCount.clear();
		m_stPlayerBase.mapTeamTaskDayCount.clear();
		clearOnlyEvent(CleanupOnlyIndex_Daily);
		for ( int32 i = 0 ; i < _emGameSystemType_ARRAYSIZE ; ++i )
		{
			CGamePlayerFun* pFun = m_arrPlayerFun[i];
			if ( pFun )
				pFun->cleanupDaily();
		}
	}
}

//-------------------------------------------------------------
//------------------------------
void	CScenePlayer::onPassLogicDayClearup(uint64 uProcessTime, uint64 uTime)
{
	for ( int32 i = 0 ; i < _emGameSystemType_ARRAYSIZE ; ++i )
	{
		CGamePlayerFun* pFun = m_arrPlayerFun[i];
		if ( pFun )
			pFun->onLogicDayCleanup(uProcessTime, uTime);
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CScenePlayer::weekCleanup	(uint64 uTime,int32 iOfflineWeek)
{
	if (m_stPlayerBase.uWeeklyCleanTime == 0
		 || diffTimeWeek(m_stPlayerBase.uWeeklyCleanTime,uTime) > 0)
	{
		m_stPlayerBase.uWeeklyCleanTime = (uint32)uTime;

		clearOnlyEvent(CleanupOnlyIndex_Weekly);
		for ( int32 i = 0 ; i < _emGameSystemType_ARRAYSIZE ; ++i )
		{
			CGamePlayerFun* pFun = m_arrPlayerFun[i];
			if ( pFun )
				pFun->cleanupWeekly();
		}
	}
}

//-------------------------------------------------------------
//------------------------------ 
void	CScenePlayer::monthCleanup	(uint64 uTime,int32 iOfflineMonth)
{
	if (m_stPlayerBase.uMonthlyCleanTime == 0
		|| diffTimeMonth(m_stPlayerBase.uMonthlyCleanTime,uTime) > 0)
	{
		m_stPlayerBase.uMonthlyCleanTime = (uint32)uTime;

		clearOnlyEvent(CleanupOnlyIndex_Monthly);
		for ( int32 i = 0 ; i < _emGameSystemType_ARRAYSIZE ; ++i )
		{
			CGamePlayerFun* pFun = m_arrPlayerFun[i];
			if ( pFun )
				pFun->cleanupMonthly();
		}
	}
}

//-------------------------------------------------------------
//------------------------------ 
uint32	CScenePlayer::getFlyTotalTick	()
{
	CHECKF(m_pJobModel != NULL);
	int32 uSpeed = g_clConstantData.Game_FlySpeed;
	CHECKF(uSpeed);
	uint32 uFlyTotalTick = m_pJobModel->uLandAnimTick +m_pJobModel->uClimbAnimTick 
		+ (uint32)m_pJobModel->fFlyHeight * 2 * 1000 / uSpeed;
	return uFlyTotalTick;
}

//-------------------------------------------------------------
//------------------------------ 
uint32	CScenePlayer::getBakFlayTick	()
{
	CHECKF(m_pJobModel != NULL);
	return m_pJobModel->uLandAnimTick +m_pJobModel->uClimbAnimTick;
}

//-------------------------------------------------------------
//------------------------------ 
float32	CScenePlayer::getModelRadius	()
{
	CHECKF(m_pJobModel != NULL);
	return m_pJobModel->fModelRadius;
}

//-------------------------------------------------------------
//------------------------------ 检查前提条件
bool CScenePlayer::canPreCondition(const DMAP_UINT8_32_32& dmapPreCondition, int64 nValue/* = 1*/)
{
	if (0 == dmapPreCondition.size())
		return true;

	DMAP_UINT8_32_32::const_iterator _pos = dmapPreCondition.begin();
	for (;_pos != dmapPreCondition.end(); ++_pos)
	{
		uint8 uPreType = _pos->first;
		DMAP_UINT8_32_32::_sub_map submap = _pos->second;
		DMAP_UINT8_32_32::_sub_map::iterator _spos = submap.begin();
		for (;_spos != submap.end(); ++_spos)
		{
			switch (uPreType)
			{
			case PreConditionType_OnlyIndex:
				{
					if (checkOnlyIndex(_spos->first))
						return false;
				}
				break;
			case PreConditionType_Count:
				{
					//if (m_pPlayerCounter && m_pPlayerCounter->canCounter(_spos->second,_spos->first + nValue))
					//	return false;
				}
				break;
			case PreConditionType_Job:
				{
					if (_spos->first > 0 && getJob() != _spos->first)
						return false;
				}
				break;
			case PreConditionType_Level:
				{
					if (getLevel() < _spos->first)
						return false;
				}
				break;
			case PreConditionType_SysOpen:
				{
					// 更新系统功能开关
					if (m_pPlayerSystemSwitch && !m_pPlayerSystemSwitch->isExit(_spos->first))
						return false;
						
				}
				break;
			default:
				return false;
				break;
			}
		}
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------ 增加前提条件
bool CScenePlayer::addPreCondition(const DMAP_UINT8_32_32& dmapPreCondition,uint8 uPreType,int64 nAddValue/* = 0*/)
{
	DMAP_UINT8_32_32::const_iterator _pos = dmapPreCondition.begin();
	for (;_pos != dmapPreCondition.end(); ++_pos)
	{
		uint8 uTmpPreType = _pos->first;
		if (uTmpPreType != uPreType)
			continue;
		DMAP_UINT8_32_32::_sub_map submap = _pos->second;
		DMAP_UINT8_32_32::_sub_map::iterator _spos = submap.begin();
		for (;_spos != submap.end(); ++_spos)
		{
			addPreCondition(uTmpPreType,_spos->second,_spos->first,nAddValue);
		}
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------ 记录前提条件
bool CScenePlayer::addPreCondition		(uint8 uPreType,uint32 uID,int32 nValue,int64 nAddValue/* = 0*/)
{
	CHECKF( uPreType < _emPreConditionType_ARRAYSIZE);
	if (PreConditionType_OnlyIndex == uPreType)
	{
		setOnlyIndex(uID,true);
	}
	else if (PreConditionType_Count == uPreType)
	{
		//CHECKF(m_pPlayerCounter != NULL);
		//m_pPlayerCounter->updateCounter(uID,nAddValue);
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
uint64 CScenePlayer::getMoney		(uint8 uMoneyType)
{
	CHECKF( uMoneyType < MoneyType_Max && uMoneyType > 0);
	uint64 uCurMoney = 0;
	uint64* pValue = m_stPlayerBase.mapMoney.find_(uMoneyType);
	if (pValue)
		uCurMoney = *pValue;

	return uCurMoney;
}

//-------------------------------------------------------------
//------------------------------ 是否能够更新钱
bool CScenePlayer::canAddMoney		(uint8 uMoneyType,int64 nMoney)
{
	if (nMoney >= 0 )
		return true;
	CHECKF( uMoneyType < MoneyType_Max && uMoneyType > 0 && nMoney != 0);
	int64 nCurMoney = 0;
	if (MoneyType_BindSliver == uMoneyType)
	{
		nCurMoney = getMoney(MoneyType_Sliver) + getMoney(MoneyType_BindSliver);
	}
	else if (MoneyType_BindCash == uMoneyType)
	{
		nCurMoney = getMoney(MoneyType_BindCash) + getMoney(MoneyType_Cash);
	}
	else
	{
		nCurMoney = getMoney(uMoneyType);
	}
	
	return nCurMoney + nMoney >= 0;
}
//-------------------------------------------------------------
//------------------------------ 加钱
bool CScenePlayer::addMoney		(uint8 uMoneyType,int64 nMoney,_emDoingType emDoing, PBMoneyConsume *pbConsume)
{
	CHECKF( uMoneyType < MoneyType_Max && uMoneyType > 0);

	if (nMoney > 0)
		return updateMoney(uMoneyType,nMoney,emDoing);

	//扣钱 分绑定和不绑定
	int64 nDelMoney = nMoney * -1;
	if (MoneyType_BindSliver == uMoneyType)
	{
		int64 nBingNum = (int64)getMoney(MoneyType_BindSliver);
		if (nBingNum >= nDelMoney)
		{
			if (NULL != pbConsume)
			{
				pbConsume->set_bindsilver(uint32(pbConsume->bindsilver() + nDelMoney));
			}
			return updateMoney(uMoneyType,nMoney,emDoing);
		}
		else
		{
			if (NULL != pbConsume)
			{
				pbConsume->set_bindsilver(uint32(pbConsume->bindsilver() + nBingNum));
				pbConsume->set_silver(uint32(pbConsume->silver() + (nDelMoney - nBingNum)));
			}
			updateMoney(MoneyType_BindSliver,nBingNum * -1,emDoing);
			return updateMoney(MoneyType_Sliver, nBingNum - nDelMoney,emDoing);
		}
	}
	else if (MoneyType_BindCash == uMoneyType)
	{
		int64 nBingNum = (int64)getMoney(MoneyType_BindCash);
		if (nBingNum >= nDelMoney)
		{
			if (NULL != pbConsume)
			{
				pbConsume->set_bindcash(uint32(pbConsume->bindcash() + nDelMoney));
			}
			return updateMoney(uMoneyType,nDelMoney * -1,emDoing);
		}
		else
		{
			if (NULL != pbConsume)
			{
				pbConsume->set_bindcash(uint32(pbConsume->bindcash() + nBingNum));
				pbConsume->set_cash(uint32(pbConsume->cash() + (nDelMoney - nBingNum)));
			}
			updateMoney(MoneyType_BindCash,nBingNum * -1,emDoing);
			return updateMoney(MoneyType_Cash, nBingNum - nDelMoney,emDoing);
		}
	}
	else
	{
		if (NULL != pbConsume)
		{
			if (MoneyType_Sliver == uMoneyType)
				pbConsume->set_silver(uint32(pbConsume->silver() + nDelMoney));
			else if (MoneyType_Cash == uMoneyType)
				pbConsume->set_cash(uint32(pbConsume->cash() + nDelMoney));
		}
		return updateMoney(uMoneyType,nMoney,emDoing);
	}
	return false;
}
//-------------------------------------------------------------
//------------------------------
bool	CScenePlayer::addMoney(const PBMoneyConsume &consume, _emDoingType emDoing)
{
	if (consume.silver() > 0)
		addMoney(MoneyType_Sliver, consume.silver(), emDoing);
	if (consume.bindsilver() > 0)
		addMoney(MoneyType_BindSliver, consume.bindsilver(), emDoing);
	if (consume.cash() > 0)
		addMoney(MoneyType_Cash, consume.cash(), emDoing);
	if (consume.bindcash() > 0)
		addMoney(MoneyType_BindCash, consume.bindcash(),emDoing);
	return true;
}
//-------------------------------------------------------------
//------------------------------
bool	CScenePlayer::canUpdateMoney			(uint8 uMoneyType,int64 nMoney)
{
	CHECKF( uMoneyType < MoneyType_Max && uMoneyType > 0);
	uint64 uCurMoney = getMoney(uMoneyType);
	if (nMoney > 0)
	{
		//最大值
		const _stExpendModel* pModel = g_clExpendData.find(uMoneyType);
		CHECKF(pModel != NULL);
		if ( uCurMoney + (uint64)nMoney >  pModel->uMaxValue )
			return false;
	}
	else
	{
		if (  uCurMoney < (uint64)(-1*nMoney) )
			return false;
	}

	return true;
}
//-------------------------------------------------------------
//------------------------------ 加钱
bool CScenePlayer::updateMoney		(uint8 uMoneyType,int64 nMoney,_emDoingType emDoing)
{
	CHECKF( uMoneyType < MoneyType_Max && uMoneyType > 0);
	//最大值
	const _stExpendModel* pModel = g_clExpendData.find(uMoneyType);
	CHECKF(pModel != NULL);

	uint64 uCurMoney = getMoney(uMoneyType);
	if (nMoney > 0)
	{
		uCurMoney  += nMoney;
		uCurMoney = MIN(pModel->uMaxValue,uCurMoney);
	}
	else
	{
		uCurMoney = ((int64)uCurMoney + nMoney > 0)?(uCurMoney + nMoney): 0;
	}

	m_stPlayerBase.mapMoney[uMoneyType] = uCurMoney;

	//-todo 日志
	if (pModel->uLogMinValue && pModel->uLogMinValue <= abs(nMoney))
	{
	}

	//同步给客户端
	{
		this->synExpendDataToClient(uMoneyType,uCurMoney);
	}

	// 排行 
	if (uMoneyType == MoneyType_Sliver)
		toplist_update(TopListType_Wealth, uCurMoney, 0);
	return true;
}

//-------------------------------------------------------------
//------------------------------ 经验
bool	CScenePlayer::addExp	(int32 nAddValue,_emDoingType emDoing)
{
	CHECKF(m_pPlayerLevelModel != NULL && m_pPromotionModel != NULL);
	uint64 uOldExp = m_stPlayerBase.uExp;
	const uint32 promotionLevel = m_pPromotionModel->uNeedLevel;
	if (nAddValue >= 0)
	{
		m_stPlayerBase.uExp += nAddValue;

		//达到晋升等级不升级
		uint8 i = 0;
		const _stPlayerModel* pOldModel = m_pPlayerLevelModel;
		while ( pOldModel && (getLevel() < promotionLevel)
			&& (m_stPlayerBase.uExp >= pOldModel->uLevelExp)
			&& i++ < 100)
		{
			//满级返回错误
			if (!addLevel(1, emDoing))
				break;

			m_stPlayerBase.uExp -= pOldModel->uLevelExp;
			pOldModel = m_pPlayerLevelModel;
		}
	}
	else
	{
		if (m_stPlayerBase.uExp + nAddValue >= 0)
		{
			m_stPlayerBase.uExp += nAddValue;
		}
		else
		{
			m_stPlayerBase.uExp = 0;
		}
	}

	if (m_stPlayerBase.uExp != uOldExp)
	{
		PBG2CAddExp ack;
		ack.set_expvalue(m_stPlayerBase.uExp);
		sendMsg(P_S2C_Protocol_Common, S2C_Common_BaseExp, &ack);
	}

	{
		PBG2CAddExp ack;
		ack.set_expvalue(nAddValue);
		sendMsg(P_S2C_Protocol_Common, S2C_Common_AddExp, &ack);
	}
	
	return true;
}
//-------------------------------------------------------------
//------------------------------ 提升等级
bool	CScenePlayer::addLevel(int32 nAddValue,_emDoingType emDoing)
{
	if (!nAddValue)
		return false;

	uint32 uOldLevel = m_stPlayerBase.uLevel;
	if (nAddValue > 0)
	{
		//添加技能
		for (int32 i = 1; i <= nAddValue; ++i)
		{
			const _stPlayerModel* pPlayerModel = g_clPlayerData.findLevel(m_stPlayerBase.uLevel + 1);
			if (!pPlayerModel)
				continue;

			//gm增加 更新晋升数据
			if (m_pPromotionModel && pPlayerModel->uLevel > m_pPromotionModel->uNeedLevel)
			{
				if (DoingType_GM == emDoing)
				{
					m_pPromotionModel = g_clPromotionData.find(m_pPromotionModel->uJob,m_pPromotionModel->uStep + 1);
				}
				else
				{
					break;
				}
			}

			m_pPlayerLevelModel = pPlayerModel;
			m_stPlayerBase.uLevel = pPlayerModel->uLevel;

			const _stPlayerModel::SUB_DMAP_ADD_SKILL* pSubmap = pPlayerModel->dmapAddSkill.find_(getJob());
			if (!pSubmap)
				continue;
			_stPlayerModel::SUB_DMAP_ADD_SKILL::const_iterator _pos = pSubmap->begin();
			_stPlayerModel::SUB_DMAP_ADD_SKILL::const_iterator _end = pSubmap->end();
			for (; _pos != _end; ++_pos)
				getGameSkillMgr().addSkill(_pos->first,_pos->second,false,true, emDoing);
		}
	}
	else
	{
		m_stPlayerBase.uLevel = ((int32)m_stPlayerBase.uLevel + nAddValue < 0) ? 1 : m_stPlayerBase.uLevel + nAddValue;
		const _stPlayerModel* pPlayerModel = g_clPlayerData.findLevel(m_stPlayerBase.uLevel);
		if (!pPlayerModel)
			return false;
		m_pPlayerLevelModel = pPlayerModel;
	}
	
	//等级不同才广播
	if (uOldLevel != m_stPlayerBase.uLevel)
	{
		if (m_pPlayerCommon)
			m_pPlayerCommon->calculateAttri();

		PBG2CAddLevel ack;
		ack.set_objectid(getUnitID());
		ack.set_level(m_stPlayerBase.uLevel);
		sendBroadcast(P_S2C_Protocol_Common, S2C_Common_AddLevel, &ack, 0);

		setGhost(_BIT64(Ghost_Base),true);

		//if ( m_pPlayerQuestMgr )
		//	m_pPlayerQuestMgr->callback_upLevel();

		//if ( m_pPlayerTitle )
		//	m_pPlayerTitle->callback_upLevel();

		//  更新系统功能开关
		if (m_pPlayerSystemSwitch)
			m_pPlayerSystemSwitch->callback_upLevel(m_stPlayerBase.uLevel);

		// 更新排行
		toplist_update(TopListType_Level, m_stPlayerBase.uLevel, 0);
		// 通知全局服务器
		setUpdateParantSvr();

		//先计算属性 再更新血量
		calculateAttributeBase();
		if (!isDead())
			setHP(getHPMax());
	}
	
	return true;
}
//-------------------------------------------------------------
//------------------------------ 晋升
bool	CScenePlayer::promote(uint8& uCode)
{
	if (!m_pPromotionModel /*|| !m_pPlayerQuestMgr */|| !m_pPlayerLevelModel)
		return false;

	uCode = R_ResultCommon_Succeed;
	PBG2CCommon_Promotion ack;
	uint64 begin = (uint64)g_pServer->getWorldStartTime(getLogicWorldID());
	begin = getDayBeginTime(begin);
	if (diffTimeHour(begin, (uint64)getTime()) < (int32)m_pPromotionModel->uOpenHour) {
		uCode = R_ResultCommon_PromotionTime;
	//} else if (!m_pPlayerQuestMgr->isComplete(m_pPromotionModel->uQuest))	{
	//	uCode = R_ResultCommon_PromotionQuest;
	} else if (getPromotionStep() >= g_clPromotionData.maxStep(getJob())) {
		uCode = R_ResultCommon_Fail;
	} else if (getLevel() < m_pPromotionModel->uNeedLevel) {
		uCode = R_ResultCommon_PromotionLevel;
	}
	else if (getExp() < m_pPlayerLevelModel->uLevelExp) {
		uCode = R_ResultCommon_PromotionNeedExp;
	}
	else {
		
		uCode = R_ResultCommon_Succeed;
		// 发放邮件奖励
		PBMail stMail;
		stMail.set_playerid(getPlayerID());
		stMail.set_worldid(getLogicWorldID()); 
		stMail.set_type(MailType_Promote); 
		stMail.set_sendtime((uint64)getTime());
		const _stQuestModel *pQuestModel = g_clQuestData.findQuestModel(m_pPromotionModel->uQuest);
		if (NULL != pQuestModel)
		{
			const _stPrizeModel* pModel = g_clPrizeData.findQuestPrizeModel(pQuestModel->uID);
			if (pModel)
			{
				for (CONST_ITER_MAP_NEED_EXPEND it = pModel->mapAddExpend.begin(); it != pModel->mapAddExpend.end(); ++it)
				{
					PBMoney* money = stMail.add_money();
					if ( money )
					{
						money->set_moneytype(it->first);
						money->set_money(it->second);
					}
				}
				for (CONST_ITER_MAP_U32_U32 it = pModel->mapItem.begin(); it != pModel->mapItem.end(); ++it)
				{
					PBItem* item = stMail.add_item();
					if ( item )
					{
						item->set_itemid(it->first);
						item->set_itemnum(it->second);
					}
				}
				//按照职业给道具
				{
					stl_vector<_stPrizeByJob>::const_iterator _pos = pModel->vcItemByJob.begin();
					stl_vector<_stPrizeByJob>::const_iterator _end = pModel->vcItemByJob.end();
					for ( ; _pos != _end ; ++_pos )
					{
						const _stPrizeByJob & st = *_pos;
						//检查职业
						bool bCheckJob = false;
						if (st.uJob > GameJob_All)
						{
							bCheckJob = st.uJob == getJob();
						}
						else
						{
							const _stItemModel* pModel = g_clItemData.find(st.uItemID);
							bCheckJob = pModel
								&& canPreCondition(pModel->dmapUseCondition);
						}
						if (bCheckJob)
						{
							PBItem* item = stMail.add_item();
							if ( item )
							{
								item->set_itemid(st.uItemID);
								item->set_itemnum(st.uCount);
							}
						}
					}
				}


			}
		}
		PBMailPromote* promoteInfo = stMail.mutable_promote();
		if (promoteInfo)
		{
			promoteInfo->set_step(m_pPromotionModel->uStep + 1);
		}
		
		if ( stMail.item_size() > 0 || stMail.money_size() > 0)
		{
			// 配置了奖励才会发送邮件
			 CPlayerMail::sendMail(stMail);
		}
		m_pPromotionModel = g_clPromotionData.find(getJob(), m_pPromotionModel->uStep + 1);

		addExp(0, DoingType_Promotion);
		CHECKF(m_pPromotionModel != NULL);
		//m_pPlayerQuestMgr->callback_promote();
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------ 获取晋升阶数
uint8	CScenePlayer::getPromotionStep() const
{
	return m_pPromotionModel ? m_pPromotionModel->uStep : 0;
}
//-------------------------------------------------------------
//------------------------------ 获取晋升阶数
void	CScenePlayer::setTitleID(uint32 uTitleID)
{
	if (uTitleID == m_stPlayerBase.uTitleID)
		return;
	m_stPlayerBase.uTitleID = uTitleID;
	setGhost(_BIT64(Ghost_Base),true);
}
//-------------------------------------------------------------
//------------------------------ 提升修为
bool	CScenePlayer::addWisdom	(int64 nAddValue,_emDoingType emDoing)
{
	if ((int64)m_stPlayerBase.uWisdom + nAddValue < 0)
	{
		m_stPlayerBase.uWisdom = 0;
	}
	else
	{
		m_stPlayerBase.uWisdom += nAddValue;
	}

	this->synExpendDataToClient(ExpendType_Wisdom,m_stPlayerBase.uWisdom);

	//记日志

	return true;
}
//-------------------------------------------------------------
//------------------------------
uint64	CScenePlayer::getContribution			()
{
	uint64 uValue = 0;
	//CPlayerFaction* pFaction = (CPlayerFaction*) (getPlayerFun(GameSystemType_Faction));
	//if (pFaction)
	//	uValue = pFaction->getContribution();

	return uValue;
}
//-------------------------------------------------------------
//------------------------------
bool	CScenePlayer::addContribution			(int32 nAddValue,_emDoingType emDoing)
{
	bool bRet = false;
	//CPlayerFaction* pFaction = (CPlayerFaction*) (getPlayerFun(GameSystemType_Faction));
	//if (pFaction)
	//{
	//	bRet = pFaction->updateContribution(nAddValue,emDoing);
	//}
	return bRet;
}
//-------------------------------------------------------------
//------------------------------ 提升修为
bool	CScenePlayer::addSufferValue	(int64 nAddValue,_emDoingType emDoing)
{
	if ((int64)m_stPlayerBase.uSufferValue + nAddValue < 0)
	{
		m_stPlayerBase.uSufferValue = 0;
	}
	else
	{
		m_stPlayerBase.uSufferValue += nAddValue;
	}

	this->synExpendDataToClient(ExpendType_SufferValue,m_stPlayerBase.uSufferValue);

	//记日志

	return true;
}

//-------------------------------------------------------------
//------------------------------ 增加装备吉运值
bool	CScenePlayer::addEquipLucky	(int32 nAddValue,_emDoingType emDoing)
{
	if (m_stPlayerBase.uEquipLucky + nAddValue < 0)
	{
		m_stPlayerBase.uEquipLucky = 0;
	}
	else
	{
		m_stPlayerBase.uEquipLucky += nAddValue;
	}

	//同步给客户端
	this->synExpendDataToClient(ExpendType_EquipLucky,m_stPlayerBase.uEquipLucky);

	//记日志

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool CScenePlayer::synExpendDataToClient(uint16 uType, uint64 uValue)
{
	PBG2CExpendSyn pbMsg;
	pbMsg.set_expendtype(uType);
	pbMsg.set_value(uValue);
	this->sendMsg(P_S2C_Protocol_Common,S2C_Common_ExpendSyn,&pbMsg);
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool CScenePlayer::isExpendFull(uint16 uExpendType)
{
	if (Parent::isExpendFull(uExpendType))
		return true;
	return false;
}
//-------------------------------------------------------------
//------------------------------ 
uint64 CScenePlayer::getExpend(uint16 uExpendType)
{
	CHECKF(uExpendType < _emExpendType_ARRAYSIZE && uExpendType > 0);
	
	if (uExpendType < MoneyType_Max)
	{
		if (MoneyType_BindSliver == uExpendType)
		{
			return getMoney((uint8)uExpendType) + getMoney((uint8)MoneyType_Sliver);
		}
		else if (MoneyType_BindCash == uExpendType)
		{
			return getMoney((uint8)uExpendType) + getMoney((uint8)MoneyType_Cash);
		}
		else
		{
			return getMoney((uint8)uExpendType);
		}
	}

	uint64 uValue = 0;
	switch (uExpendType)
	{
	case ExpendType_Wisdom:
		uValue = m_stPlayerBase.uWisdom;
		break;
	case ExpendType_Exp:
		uValue = m_stPlayerBase.uExp;
		break;
	case ExpendType_EquipLucky:
		uValue = m_stPlayerBase.uEquipLucky;
		break;
	case ExpendType_SufferValue:
		uValue = m_stPlayerBase.uSufferValue;
		break;
	case ExpendType_Contribution:
		uValue = getContribution();
		break;
	case ExpendType_MakeVigour:
		uValue = getGameItemMgr().getMakeVigour();
		break;
	//case ExpendType_SinglePvpScore:
	//	uValue = getPlayerSinglePvp()->getScore();
	//	break;
	default:
		uValue = CSceneUnit::getExpend(uExpendType);
		break;
	}
	return uValue;
}
//-------------------------------------------------------------
//------------------------------ 
bool CScenePlayer::canDelExpend(const MAP_NEED_EXPEND& mapExpend,uint16 uErroType/* = 0*/)
{
	CONST_ITER_MAP_NEED_EXPEND _pos = mapExpend.begin();
	CONST_ITER_MAP_NEED_EXPEND _end = mapExpend.end();
	for (; _pos != _end; ++_pos)
	{
		if (!canExpend(_pos->first,_pos->second))
		{
			uErroType = _pos->first;
			return false;
		}
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool CScenePlayer::delExpend(const MAP_NEED_EXPEND& mapExpend,_emDoingType emDoing,CSceneUnit* pSource/* = NULL*/)
{
	bool bRet = true;
	CONST_ITER_MAP_NEED_EXPEND _pos = mapExpend.begin();
	CONST_ITER_MAP_NEED_EXPEND _end = mapExpend.end();
	for (; _pos != _end; ++_pos)
	{
		bool bTmp = addExpend(_pos->first,(int64)_pos->second * -1,emDoing,pSource);
		bRet &= bTmp;
	}
	return bRet;
}
//-------------------------------------------------------------
//------------------------------ 
bool CScenePlayer::addExpend(const MAP_NEED_EXPEND& mapExpend,_emDoingType emDoing,CSceneUnit* pSource/* = NULL*/)
{
	bool bRet = true;
	CONST_ITER_MAP_NEED_EXPEND _pos = mapExpend.begin();
	CONST_ITER_MAP_NEED_EXPEND _end = mapExpend.end();
	for (; _pos != _end; ++_pos)
	{
		bool bTmp = addExpend(_pos->first,_pos->second,emDoing,pSource);
		bRet &= bTmp;
	}
	return bRet;
}
//-------------------------------------------------------------
//------------------------------ 
bool CScenePlayer::canExpend(uint16 uExpendType,uint64 uValue)
{
	CHECKF(uExpendType < _emExpendType_ARRAYSIZE && uExpendType > 0 && uValue != 0);
	return getExpend(uExpendType) >= uValue;
}

//-------------------------------------------------------------
//------------------------------ 
bool CScenePlayer::canExpend(const MAP_U16_I64& mapExpend)
{
	bool bRet = true;
	CONST_ITER_MAP_U16_I64 _pos = mapExpend.begin();
	CONST_ITER_MAP_U16_I64 _end = mapExpend.end();
	for (; _pos != _end; ++_pos)
	{
		if (_pos->second >= 0)
			continue;

		if (!canExpend(_pos->first,uint64(_pos->second * -1)))
		{
			bRet = false;
			break;
		}
	}
	return bRet;
}

//-------------------------------------------------------------
//------------------------------ 
bool CScenePlayer::canExpend(const MAP_NEED_EXPEND& mapExpend)
{
	bool bRet = true;
	CONST_ITER_MAP_NEED_EXPEND _pos = mapExpend.begin();
	CONST_ITER_MAP_NEED_EXPEND _end = mapExpend.end();
	for (; _pos != _end; ++_pos)
	{
		if (!canExpend(_pos->first,_pos->second))
		{
			bRet = false;
			break;
		}
	}

	return bRet;
}

//-------------------------------------------------------------
//------------------------------ 
bool CScenePlayer::addExpend(const MAP_U16_I64& mapExpend,_emDoingType emDoing,CSceneUnit* pSource/* = NULL*/)
{
	if (mapExpend.empty())
		return true;

	bool bRet = true;
	CONST_ITER_MAP_U16_I64 _pos = mapExpend.begin();
	CONST_ITER_MAP_U16_I64 _end = mapExpend.end();
	for (; _pos != _end; ++_pos)
	{
		bool bTmp = addExpend(_pos->first,_pos->second,emDoing,pSource);
		bRet &= bTmp;
	}

	return bRet;
}

//-------------------------------------------------------------
//------------------------------ 
bool CScenePlayer::addExpend(uint16 uExpendType,int64 nValue,_emDoingType emDoing,CSceneUnit* pSource/* = NULL*/)
{
	CHECKF(uExpendType < _emExpendType_ARRAYSIZE && uExpendType > 0 && nValue != 0);
	if (uExpendType < MoneyType_Max)
		return addMoney((uint8)uExpendType,nValue, emDoing);

	bool bRet = false;
	switch (uExpendType)
	{
	case ExpendType_Exp:
		bRet = addExp((int32)nValue, emDoing);
		break;
	case ExpendType_Level:
		bRet =  addLevel((int32)nValue, emDoing);
		break;
	case ExpendType_Wisdom:
		bRet =  addWisdom(nValue, emDoing);
		break;
	case ExpendType_EquipLucky:
		bRet =  addEquipLucky((int32)nValue, emDoing);
		break;
	case ExpendType_SufferValue:
		bRet =  addSufferValue(nValue, emDoing);
		break;
	case ExpendType_Contribution:
		bRet =  addContribution((int32)nValue, emDoing);
		break;
	case ExpendType_MakeVigour:
		bRet =  getGameItemMgr().addMakeVigour((int32)nValue, emDoing);
		break;
	//case ExpendType_SinglePvpScore:
	//	bRet = getPlayerSinglePvp()->addScore((int32)nValue);
	//	break;
	default:
		bRet = CSceneUnit::addExpend(uExpendType,nValue,emDoing,pSource);
		break;
	}

	return bRet;
}

//-------------------------------------------------------------
//------------------------------ 
bool CScenePlayer::clearAllExpend()
{
	for ( uint32 i = _emExpendType_MIN ; i< _emExpendType_ARRAYSIZE; ++i)
	{
		if (getExpend(i) != 0)
		{
			addExpend(i, -1*getExpend(i), DoingType_GM, this);
		}
	}
	return true;
}


//-------------------------------------------------------------
//------------------------------ 
bool	CScenePlayer::triggerEvent		(const _stRegionEvent*pEvent)
{
	if ( !pEvent)
		return false;

	switch ( pEvent->uEventType )
	{
	case RegionEventType_Transfer:
		{
			if ( pEvent->vecParamlist.size() < 2 )
				return false;

			uint16 uSceneID = (uint16)pEvent->vecParamlist[0];
			uint16 uRegionID = (uint16)pEvent->vecParamlist[1];
			if ( !uSceneID )
				return false;
			return playerFlyScene_ByCellID(uSceneID,0,uRegionID);
		}
		break;
	case RegionEventType_Move:
		{
			if ( pEvent->vecParamlist.size() < 1 )
				return false;

			uint16 uRegionID = (uint16)pEvent->vecParamlist[0];

			//阵营检测的跳转点
			if (pEvent->vecParamlist.size() >= 2)
			{
				if ((uint16)pEvent->vecParamlist[1] != getCampID())
				{
					return true;
				}
			}


			_stPoint2F stPoint;
			if (!m_pSceneCell || !m_pSceneCell->getSpawnPoint(stPoint,uRegionID))
				return false;

			setPosition(stPoint.x,stPoint.y,SetPositionReason_ChangePos,true);
			return true;
		}
		break;
	case RegionEventType_QinggongStart:
		{
			if ( pEvent->vecParamlist.size() < 1 )
				return false;
			breakUseSkill();
			uint32 uQinggongID = (uint32)pEvent->vecParamlist[0];
			setQinggongID(uQinggongID);
			setQinggongStartTick(getSystemTick64());
			addState(StateType_Qinggong);
			getPlayerCommon()->broadcastQinggong(uQinggongID);
			

		}
		break;
	case RegionEventType_EnterPlane:
		{
			if ( pEvent->vecParamlist.size() < 1 )
				return false;
			uint32 uPlaneID = (uint32)pEvent->vecParamlist[0];
			PBC2GCommon_RegionTriggerPlane msg;
			msg.set_plane_id(uPlaneID);
			sendMsg(P_S2C_Protocol_Common,S2C_Common_RegionTriggerPlane,&msg);
		}
		break;
	case RegionEventType_ManorEnterLine:
		{
			if ( pEvent->vecParamlist.size() < 1 )
				return false;

			//if (getGameCopymap() 
			//	&& getGameCopymap()->getCopymapType() == CopymapType_ManorLobby)
			//{
			//	CCopymapManorLobby* p = (CCopymapManorLobby*) (getGameCopymap());
			//	if (p)
			//	{
			//		p->onPlayerLineIn(this, pEvent->uRegionID);
			//	}
			//}

		}
		break;
	case RegionEventType_ManorLeaveLine:
		{
			if ( pEvent->vecParamlist.size() < 1 )
				return false;


			//if (getGameCopymap() 
			//	&& getGameCopymap()->getCopymapType() == CopymapType_ManorLobby)
			//{
			//	CCopymapManorLobby* p = (CCopymapManorLobby*) (getGameCopymap());
			//	if (p)
			//	{
			//		p->onPlayerLineOut(this, pEvent->uRegionID);
			//	}
			//}
		}
		break;
	case RegionEventType_ManorBack2Lobby:
		{
			//if (getGameCopymap() 
			//	&& getGameCopymap()->getCopymapType() == CopymapType_ManorBattleField)
			//{
			//	CCopymapManorBattleField* p = (CCopymapManorBattleField*) (getGameCopymap());
			//	if (p)
			//	{
			//		p->onPlayerBack2Lobby(this);
			//	}
			//}
		}
		break;
	case RegionEventType_ManorEnterFlag:
		{
			//if (getGameCopymap() 
			//	&& getGameCopymap()->getCopymapType() == CopymapType_ManorBattleField)
			//{
			//	CCopymapManorBattleField* p = (CCopymapManorBattleField*) (getGameCopymap());
			//	if (p)
			//	{
			//		p->onPlayerInFlag(this);
			//	}
			//}
		}
		break;
	case RegionEventType_ManorLeaveFlag:
		{
			//if (getGameCopymap() 
			//	&& getGameCopymap()->getCopymapType() == CopymapType_ManorBattleField)
			//{
			//	CCopymapManorBattleField* p = (CCopymapManorBattleField*) (getGameCopymap());
			//	if (p)
			//	{
			//		p->onPlayerOutFlag(this);
			//	}
			//}
		}
		break;
	default:
		break;
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------
void	CScenePlayer::onEnterRegion			(const _stGameRegion* pGameRegion)
{
	if ( !pGameRegion )
		return ;

	//区域限制骑乘下马
	if (testRegion(RegionMark_CntMounting))
	{
		mount(false);
	}

	//if ( m_pPlayerQuestMgr )
	//{
	//	m_pPlayerQuestMgr->callback_enterRegion(pGameRegion->uRegionID);
	//}
	//打坐挂机区域进入发送提示
	if (testRegion(RegionMark_ZaZen))
	{
		const _stDazuoguajiModel* pDazuoguaji = g_clDazuoguajiData.findDazuoguaji(getLevel());
		if (pDazuoguaji)
		{
			uint32 uTotalExpAdded = getPlayerCommon()->getDailyDazuoguajiExp() ;

			uint32 uTotalWisdomAdded = getPlayerCommon()->getDailyDazuoguajiTaolue() ;
			//返回剩余属性
			if (pDazuoguaji->uDailyTotalExp - uTotalExpAdded > 0 
				|| pDazuoguaji->uDailyTotalTaolue - uTotalWisdomAdded > 0)
			{
				uTotalExpAdded = getPlayerCommon()->getDailyDazuoguajiExp() ;
				uTotalWisdomAdded = getPlayerCommon()->getDailyDazuoguajiTaolue() ;
				getPlayerCommon()->sendDazuoguajiNtf(pDazuoguaji->uDailyTotalExp - uTotalExpAdded
					, pDazuoguaji->uDailyTotalTaolue - uTotalWisdomAdded
					, 0, 0
					, true, false);
			}
		}

	}
}
//-------------------------------------------------------------
//------------------------------
void	CScenePlayer::onLeaveRegion			(const _stGameRegion* pGameRegion)
{
	if ( !pGameRegion )
		return ;

	//打坐挂机区域进入发送提示
	if (testRegion(RegionMark_ZaZen))
	{
		//发送关闭界面消息
		getPlayerCommon()->sendDazuoguajiNtf(0
			, 0
			, 0, 0
			, false, true);

	}
}
//-------------------------------------------------------------
//------------------------------
CSceneUnit*		CScenePlayer::getOwner				()
{
	return this;
}
//-------------------------------------------------------------
//------------------------------ 获取关系
uint8	CScenePlayer::getRelation	(CSceneUnit* pUnit)
{
	uint8 uRelation = TargetCategory_Friend;
	if (!pUnit || pUnit == this)
		return TargetCategory_Self;

	//主人一样的  一定为友方
	if (this == pUnit->getOwner() )
		return TargetCategory_Friend;

	CSceneUnit* pTargetPlayer = pUnit->getOwnerPlayer();
	if (pTargetPlayer)
	{
		switch (m_stPlayerBase.uPKType)
		{
		case PKType_Peace:
			{
				return TargetCategory_Friend;
			}
			break;
		case PKType_Evil:
			{
				if (pUnit->GetPKValue() > (uint32)g_clConstantData.Game_RedPK)
					return TargetCategory_Enemy;
			}
			break;
		case PKType_Team:
			{
				if (getTeamID()&& getTeamID() != pTargetPlayer->getTeamID() )
					return TargetCategory_Enemy;
			}
			break;
		case PKType_Faction:
			{
				if (getFactionID()&& getFactionID() != pTargetPlayer->getFactionID() )
					return TargetCategory_Enemy;
			}
			break;
		case PKType_Alliance:
			{
				if (getUnionID()&& getUnionID() != pTargetPlayer->getUnionID() )
					return TargetCategory_Enemy;
			}
			break;
		case PKType_Corps:
			{
				if (getCorpsID()&& getCorpsID() != pTargetPlayer->getCorpsID() )
					return TargetCategory_Enemy;
			}
			break;
		case PKType_All:
			{
				return TargetCategory_Enemy;
			}
			break;
		case PKType_Camp:
			{
				return g_clCampData.findCamp(getCampID(),pUnit->getCampID());
			}
			break;
		default:
			break;
		}
	}
	else
	{
		//判断阵营
		uRelation = g_clCampData.findCamp(getCampID(),pUnit->getCampID());
	}

	return uRelation;
}

//-------------------------------------------------------------
//------------------------------ 
bool CScenePlayer::offlineData(uint16 uLogicWorldID, uint32 uPlayerID, uint16 uSysType, uint32 uSignType, ::google::protobuf::Message* msg)
{
	//发到世界服
	CNetStream clStream;
	clStream.write(uPlayerID);
	clStream.write(uSysType);
	clStream.write(uSignType);
	if (msg)
		clStream.SerializeToStream(*msg);

	return g_pServer->sendBufferToWorld(uLogicWorldID,P_G2W_Protocol_Player, G2W_Player_OfflineDataAdd, clStream.getBuffer(), clStream.getPosition());
}

//-------------------------------------------------------------
//------------------------------ 
bool CScenePlayer::offlineData(uint16 uLogicWorldID, uint32 uPlayerID, uint16 uSysType, uint32 uSignType, void* msg, uint16 uSize)
{
	//发到世界服
	CNetStream clStream;
	clStream.write(uPlayerID);
	clStream.write(uSysType);
	clStream.write(uSignType);
	if (msg)
		clStream.write(uSize, msg);

	return g_pServer->sendBufferToWorld(uLogicWorldID, P_G2W_Protocol_Player, G2W_Player_OfflineDataAdd, clStream.getBuffer(), clStream.getPosition());
}
//-------------------------------------------------------------
//------------------------------ 
bool	CScenePlayer::sendOpeData(uint16 uLogicWorldID, uint32 uPlayerID, uint16 uSysType, uint32 uSignType, ::google::protobuf::Message* msg)
{
	CScenePlayer* pScenePlayer = g_pGamePlayerMgr->findPlayer(uPlayerID);
	if (!pScenePlayer)
	{
		return CScenePlayer::offlineData(uLogicWorldID,uPlayerID,uSysType,uSignType,msg);
	}

	std::string strBuff;
	if (msg)
		msg->SerializeToString(&strBuff);

	CNetStream clNetStream(const_cast<char*>(strBuff.c_str()),strBuff.size());
	CGamePlayerFun* pFun = pScenePlayer->getPlayerFun((_emGameSystemType)uSysType);
	if (!pFun || !pFun->handleOfflineData(uSignType, clNetStream))
		return false;

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool CScenePlayer::sendOpeData(uint16 uLogicWorldID, uint32 uPlayerID, uint16 uSysType, uint32 uSignType, void* msg, uint16 uSize)
{
	CScenePlayer* pScenePlayer = g_pGamePlayerMgr->findPlayer(uPlayerID);
	if (!pScenePlayer)
	{
		return CScenePlayer::offlineData(uLogicWorldID,uPlayerID,uSysType,uSignType,msg,uSize);
	}

	CNetStream clNetStream((char*)msg,uSize);
	CGamePlayerFun* pFun = pScenePlayer->getPlayerFun((_emGameSystemType)uSysType);
	if (!pFun || !pFun->handleOfflineData(uSysType, clNetStream))
		return false;

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
void CScenePlayer::handleOfflineDataLoad(uint32 uOrderNum,char* pData, uint16 uSize)
{
	CNetStream clNetStream((char*)pData,uSize);
	uint32 uPlayerID	= 0;
	uint16 uFunType		= 0;
	uint32 uSignType	= 0;
	clNetStream.read(uPlayerID);
	clNetStream.read(uFunType);
	clNetStream.read(uSignType);
	if (uPlayerID != getPlayerID())
	{
		CLog::error("handleOfflineData error, player[%u]", uPlayerID);
		return ;
	}

	if (uFunType >= _emGameSystemType_ARRAYSIZE)
		return ;

	CGamePlayerFun* pFun = m_arrPlayerFun[uFunType];
	if (pFun && pFun->handleOfflineData(uSignType, clNetStream))
	{
		static char szBuff[256];
		CNetStream clBuffer(szBuff,sizeof(szBuff));
		clBuffer.write(uPlayerID);
		clBuffer.write(uOrderNum);
		sendBufferToWorld(P_G2W_Protocol_Player,G2W_Player_OfflineDataDel,clBuffer.getBuffer(),clBuffer.getPosition());
	}
}

//-------------------------------------------------------------
//------------------------------ 
bool	CScenePlayer::processEnermyList	(uint64 uTick)
{
	MAP_ENERMYLIST::iterator _pos = m_mapEnermyList.begin();
	while (_pos != m_mapEnermyList.end())
	{
		if (uTick >= _pos->second)
		{
			DelEnermy(_pos->first,false);
			m_mapEnermyList.erase(_pos++);
			continue;
		}
		++_pos;
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------ 增加一个敌人
void	CScenePlayer::AddEnermy	(CScenePlayer* pPlayer)
{
	if (!pPlayer)
		return;

	uint64 uClearTime = getSystemTick64() + g_clConstantData.Skill_EnemyClearTime;
	if(m_mapEnermyList.is_exist(pPlayer))
	{
		m_mapEnermyList[pPlayer] =  uClearTime;
		return;
	}

	m_mapEnermyList.insert_(pPlayer,uClearTime);
	pPlayer->AddSelfEnermy(this);

	//通知给客户端
	PBS2CSkillEnemyList pbMsg;
	pbMsg.set_type(ModifyType_Add);
	pbMsg.set_playerid(pPlayer->getPlayerID());
	this->sendMsg(P_S2C_Protocol_Skill,S2C_Skill_EnermyList,&pbMsg);
}

//-------------------------------------------------------------
//------------------------------ 
void	CScenePlayer::AddSelfEnermy	(CScenePlayer* pPlayer)
{
	if (pPlayer)
		m_mapSelfEnermyList.insert_(pPlayer,1);
}

//-------------------------------------------------------------
//------------------------------ 
void	CScenePlayer::DelEnermy	(CScenePlayer* pPlayer,bool bDel/* = false*/)
{
	if (!pPlayer)
		return;

	if(!m_mapEnermyList.is_exist(pPlayer))
		return;

	if (bDel)
		m_mapEnermyList.erase(pPlayer);

	pPlayer->DelSelfEnermy(this);
	uint32 uPlayerID = pPlayer->getPlayerID();
	//通知给客户端
	PBS2CSkillEnemyList pbMsg;
	pbMsg.set_type(ModifyType_Del);
	pbMsg.set_playerid(uPlayerID);
	this->sendMsg(P_S2C_Protocol_Skill,S2C_Skill_EnermyList,&pbMsg);
}

//-------------------------------------------------------------
//------------------------------ 把自己从别人列表中删除
void	CScenePlayer::DelSelfAllEnermy	()
{
	MAP_ENERMYLIST::iterator _pos = m_mapSelfEnermyList.begin();
	while ( _pos != m_mapSelfEnermyList.end())
	{
		CScenePlayer* pPlayer = _pos->first;
		++_pos;
		if (pPlayer)
			pPlayer->DelEnermy(this);
	}
}

//-------------------------------------------------------------
//------------------------------ 
void	CScenePlayer::DelSelfEnermy	(CScenePlayer* pPlayer)
{
	if (pPlayer)
		m_mapSelfEnermyList.erase(pPlayer);
}

//-------------------------------------------------------------
//------------------------------ 
void	CScenePlayer::callbackAttack		(CSceneUnit* pUnit,int32 iDamage,bool bCritical)
{
	Parent::callbackAttack(pUnit,iDamage,bCritical);
}

//-------------------------------------------------------------
//------------------------------ 
void	CScenePlayer::callbackByAttack		(CSceneUnit* pUnit,int32 iDamage)
{
	//取消打坐
	if (getPlayerCommon())
	{
		getPlayerCommon()->cancelTakeASeat();
	}
	Parent::callbackByAttack(pUnit,iDamage);
}

//-------------------------------------------------------------
//------------------------------
void	CScenePlayer::callbackCritical	()
{
	Parent::callbackCritical();
}

//-------------------------------------------------------------
//------------------------------
void	CScenePlayer::callbackSkillHit	(const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel)
{
	Parent::callbackSkillHit(pSkillModel,pSkillEffectModel);
}

//-------------------------------------------------------------
//------------------------------
void	CScenePlayer::callbackBeSkillEffect	(CSceneUnit* pUnit,const _stSkillModel*pSkillModel,const _stSkillEffectModel*pSkillEffectModel)
{

	Parent::callbackBeSkillEffect(pUnit,pSkillModel,pSkillEffectModel);

	//被敌人命中 加入敌人列表
	if (pUnit->asPlayer())
		AddEnermy(dynamic_cast<CScenePlayer*>(pUnit));
}

//-------------------------------------------------------------
//------------------------------ 
void	CScenePlayer::callbackDead		(CSceneUnit* pSource)
{
	
	//取消打坐
	if (getPlayerCommon())
	{
		getPlayerCommon()->cancelTakeASeat();
	}

	//下马
	mount(false);

	//把自己从别人列表中删除
	DelSelfAllEnermy();

	//取消匹配
	delState(StateType_Match);

	if ( pSource )
	{
		CScenePlayer* pKillerPlayer = pSource->asPlayer();
		if ( pKillerPlayer )
			pKillerPlayer->checkUpdatePKValue(this);
	}

	if (m_pSceneCell)
		m_pSceneCell->callback_playerdead(this,pSource);

	//getPlayerIdentity()->beKill(pSource);

	m_uAutoReliveTime = (uint64)(getTime() + g_clConstantData.Game_AutoReliveTime);

	Parent::callbackDead(pSource);
}
//-------------------------------------------------------------
//------------------------------ 
void	CScenePlayer::callbackKillTarget		(CSceneUnit* pTarget)
{
	//getPlayerIdentity()->killTarget(pTarget);
	Parent::callbackKillTarget(pTarget);
}
//-------------------------------------------------------------
//------------------------------ 
void	CScenePlayer::checkUpdatePKValue		(CScenePlayer* pBeKillPlayer)
{
	if ( !pBeKillPlayer )
		return ;

	if ( !canAddPKValue(pBeKillPlayer) )
		return ;

	updatePKValue((uint32)g_clConstantData.Game_AddPKValue,true);
}
//-------------------------------------------------------------
//------------------------------ 
bool	CScenePlayer::canAddPKValue			(CScenePlayer* pBeKillPlayer)
{
	if ( !pBeKillPlayer )
		return false;

	// PK区域 不增加PK值
	if ( testRegion(RegionMark_CntAddPKValue) && pBeKillPlayer->testRegion(RegionMark_CntAddPKValue) )
		return false;

	// 杀灰名 不加PK值
	if ( pBeKillPlayer->m_stPlayerBase.uKillTime )
		return false;

	// 杀PK值过大的 不增加PK值
	if ( pBeKillPlayer->m_stPlayerBase.uPKValue >= (uint32)g_clConstantData.Game_KillNotAddPK )
		return false;

	return true;
}
//-------------------------------------------------------------
//------------------------------ 
void	CScenePlayer::callbackLife		()
{
	Parent::callbackLife();
	if (m_pSceneCell)
		m_pSceneCell->callback_playerlife(this,this);
	//if (getPlayerWorldBoss())
	//{
	//	//需要能够重新打开界面
	//	getPlayerWorldBoss()->setOpenRankDialog(false);
	//}
}
//-------------------------------------------------------------
//------------------------------ 计算战斗力
void	CScenePlayer::callbackFightPower		()
{
	uint32 uOldFightPower		= m_stPlayerBase.uFightPower;
	m_stPlayerBase.uFightPower	= g_clBattleData.getFightPower(m_stGameAttributeBase);
	if (m_stPlayerBase.uFightPower > m_stPlayerBase.uMaxFightPower)
	{
		m_stPlayerBase.uMaxFightPower = m_stPlayerBase.uFightPower;
		setUpdateParantSvr();
	}

	if ( uOldFightPower != m_stPlayerBase.uFightPower )
	{// 通知全局服务器
		setUpdateParantSvr();
	}

	if (uOldFightPower < m_stPlayerBase.uFightPower)
	{
		//m_pPlayerTitle->callback_updateFightpower();
	}
	
	setGhost(_BIT32(Ghost_FightPower));
	
	//if ( m_pPlayerQuestMgr )
	//	m_pPlayerQuestMgr->callback_force();
	//更新排行
	toplist_update(TopListType_Figtht, m_stPlayerBase.uFightPower, 0);
}

//-------------------------------------------------------------
//------------------------------计算子系统战斗力
bool CScenePlayer::calculateSubFightPower		(uint8 &uCode)
{
	uCode = R_ResultCommon_Fail;
	PBG2CCommonSubFightPower ack;
	auto _pos = m_mapSystemAttribute.begin();
	auto _end = m_mapSystemAttribute.end();
	
	for (; _pos != _end; ++_pos)
	{
		if ( _emGameSystemType_MIN > _pos->first || _pos->first > _emGameSystemType_MAX)
			continue; ;
		
		uint32 uFightPower = g_clBattleData.getFightPower(_pos->second);
		//m_stPlayerBase.subFightPower[_pos->first] = uFightPower;
		
		PBG2CFightPower *pInfo = ack.add_info();
		if (pInfo)
		{
			pInfo->set_id(_pos->first);
			pInfo->set_fightpower(uFightPower);
		}
	}
	sendMsg(P_S2C_Protocol_Common,S2C_Common_GetSubFightPower,&ack,R_ResultCommon_Succeed);
	return true;
}

//-------------------------------------------------------------
//------------------------------ 进入战斗状态
void	CScenePlayer::callbackFight			()
{
	
}
//-------------------------------------------------------------
//------------------------------ 进入场景回调
void	CScenePlayer::callbackEnterScene		(CGameSceneCell* pSceneCell)
{
	if (!pSceneCell)
		return;

	//取消打坐
	if (getPlayerCommon())
	{
		getPlayerCommon()->cancelTakeASeat();
	}

	delState(StateType_Match);
	getSkilling().initialize(this,NULL);
	m_uLastActionTime = getTime();

	//g_pWorldBossMgr->sendHurtRankOnEnter(this);
}
//-------------------------------------------------------------
//------------------------------ 
void	CScenePlayer::callbackUseSkill	(const _stSkillModel*pSkillModel,uint32 uTarUnitID)
{
	Parent::callbackUseSkill(pSkillModel,uTarUnitID);
	m_uLastActionTime = getTime();
}
//-------------------------------------------------------------
//------------------------------ 位置移动
void	CScenePlayer::callbackPostionChange	()
{
	//取消打坐
	if (getPlayerCommon())
	{
		getPlayerCommon()->cancelTakeASeat();
	}
	m_uLastActionTime = getTime();
}
//-------------------------------------------------------------
//------------------------------ 进入场景
void	CScenePlayer::enterScene(CGameSceneCell*pCell)
{
	Parent::enterScene(pCell);

	setOwner(this);

	if ( pCell && pCell->getSceneModel() && (!pCell->getSceneModel()->strEnterScript.empty() ) )
		lua_tinker::call<void>(g_pServer->getLua(),pCell->getSceneModel()->strEnterScript.c_str(),this);
	
	callbackEnterScene(pCell);
}

//-------------------------------------------------------------
//------------------------------ 离开场景
void	CScenePlayer::leaveScene()
{
	//把自己从对方的敌人列表删除
	DelSelfAllEnermy();

	Parent::leaveScene();
}
//-------------------------------------------------------------
//------------------------------ 
_stTeamInfo* CScenePlayer::getTeam()
{
	return NULL;
	//_stTeamInfo* pTeam = (m_pPlayerTeam) ? m_pPlayerTeam->getTeam() : NULL;
	//return (pTeam && pTeam->uTeamID) ? pTeam : NULL;
}
//-------------------------------------------------------------
//------------------------------ 
uint32 CScenePlayer::getFactionID()
{
	//CPlayerFaction* pFaction  = (CPlayerFaction*)getPlayerFun(GameSystemType_Faction);
	//if (!pFaction)
	//	return 0;

	//return pFaction->getFactionID();
	return 0;
}
//-------------------------------------------------------------
//------------------------------ 
uint32	CScenePlayer::getUnionID				()
{
	//CPlayerFaction* pFaction  = (CPlayerFaction*)getPlayerFun(GameSystemType_Faction);
	//if (!pFaction)
	//	return 0;

	//return pFaction->getUnionID();
	return 0;
}
//-------------------------------------------------------------
//------------------------------ 
uint32	CScenePlayer::getCorpsID				()
{
	//CPlayerFaction* pFaction  = (CPlayerFaction*)getPlayerFun(GameSystemType_Faction);
	//if (!pFaction)
	//	return 0;

	//return pFaction->getCorpsID();
	return 0;
}
//-------------------------------------------------------------
//------------------------------ 
void	CScenePlayer::onEnterState			(_emGameStateType uStateType)
{
	Parent::onEnterState(uStateType);
	switch (uStateType)
	{
	case StateType_Dead:
	case StateType_Fight:
		{
			//CPlayerTrade* pPlayerTrade = (CPlayerTrade*)(getPlayerFun(GameSystemType_Trade));
			//if (pPlayerTrade)
			//{
			//	pPlayerTrade->interuptTrade();
			//}
		}
		break;
	case StateType_Rest:
		{
			//加buff
			getGameBuffMgr().addBuff(g_clConstantData.Game_DazuoBuffId, 1, this->getUnitID(), 0, DoingType_Dazuoguaji);
		
			if (testRegion(RegionMark_ZaZen))
			{
				getGameBuffMgr().addBuff(g_clConstantData.Game_DazuoGuajiBuffId, 1, this->getUnitID(), 0, DoingType_Dazuoguaji);

			}
		
		}
		break;
	default:
		break;
	}
}
//-------------------------------------------------------------
//------------------------------ 
void	CScenePlayer::onLeaveState			(_emGameStateType uStateType,bool bInterupt)
{
	Parent::onLeaveState(uStateType,bInterupt);

	switch (uStateType)
	{
		case StateType_Rest:
		{
			//去除buff
			getGameBuffMgr().delBuff(g_clConstantData.Game_DazuoBuffId, DoingType_Dazuoguaji);
			getGameBuffMgr().delBuff(g_clConstantData.Game_DazuoGuajiBuffId, DoingType_Dazuoguaji);
		}
		break;
		case StateType_Match:
			{
				uint8 uCode = 0;
				//getPlayerPvpWar()->exitMatchAsk(uCode);
			}
			break;
		default:
			break;
	}

}

//-------------------------------------------------------------
//------------------------------
void	CScenePlayer::processUpdateGlobal		()
{
	if(!m_bUpdateParantSvr)
		return;

	m_bUpdateParantSvr = false;
	if( getLevel() >= (uint32)g_clConstantData.Game_GloBalCacheLevel && isGlobalConneted() )
	{
		//更新到全局服
		PBPlayerGlobalBase msg;
		PBPlayerView* pbView = msg.mutable_view();
		if (pbView)
		{
			packetView(pbView);
		}
		

		sendMsgToGlobal(P_G2GL_Protocol_Player, G2GL_Player_Update,&msg );
	}

	if ( isWorldConneted() )
	{
		//更新到世界服
		PBG2WPlayerData msg;
		PBPlayerDisplay* pbDisplay = msg.mutable_playerdisplay();
		if (pbDisplay)
		{
			m_stPlayerBase.getDisplay().saveToProtobuf(*pbDisplay);
		}
		msg.set_fightpowr(m_stPlayerBase.uFightPower);
		sendMsgToWorld(P_G2W_Protocol_Player, G2W_Player_Cache,&msg );
	}
}

//-------------------------------------------------------------
//------------------------------
void	CScenePlayer::updateAvater ()
{
	m_stPlayerBase.stPlayerShape.uShape[PlayerShape_Weapon]				= getGameItemMgr().findEquipID(EquipType_Weapon);
	m_stPlayerBase.stPlayerShape.uShape[PlayerShape_Head]				= getGameItemMgr().findEquipID(EquipType_Head);
	m_stPlayerBase.stPlayerShape.uShape[PlayerShape_Shoulder]			= getGameItemMgr().findEquipID(EquipType_Shoulder);
	m_stPlayerBase.stPlayerShape.uShape[PlayerShape_Blouse]				= getGameItemMgr().findEquipID(EquipType_Blouse);
	m_stPlayerBase.stPlayerShape.uShape[PlayerShape_Mount]				= getMountID();//需要判断是否骑乘
	m_stPlayerBase.stPlayerShape.uShape[PlayerShape_WeaponForgeLevel]	= getGameItemMgr().findEquipForgeLevel(EquipType_Weapon);
	m_stPlayerBase.stPlayerShape.uShape[PlayerShape_EquipForgeLevel]	= getGameItemMgr().getAllForgeMinLevel();


	//CPlayerFashion* pPlayerFashion = (CPlayerFashion*)getPlayerFun(GameSystemType_Fashion);
	//if (pPlayerFashion )
	//{
	//	m_stPlayerBase.setFashion.clear();
	//	auto _pos = pPlayerFashion->m_mapEquipFashionID.begin();
	//	auto _end = pPlayerFashion->m_mapEquipFashionID.end();
	//	for (; _pos != _end; ++_pos)
	//	{
	//		m_stPlayerBase.setFashion.insert(_pos->second);
	//	}
	//}

	//军团职位大于0 同步到世界服
	//if (m_stPlayerBase.uCorpsJob > CorpsJob_Unknown)
	//{
		//getPlayerCorps()->synAvatar();
	//}

	setGhost(_BIT64(Ghost_Base),true);
}

//-------------------------------------------------------------
//------------------------------查看玩家直接显示结果
void	CScenePlayer::packetView (PBPlayerView* pbView,bool bFindPlayer/* = false*/)
{
	if (!pbView)
		return;

	PBPlayerAvatar* pbAvatar = pbView->mutable_avatar();
	if (!pbAvatar)
		return ;

	m_stPlayerBase.saveToAvatar(*pbAvatar);
	getGameItemMgr().saveToProbuff(pbView->mutable_equip());
	pbView->set_fightpower(m_stPlayerBase.uFightPower);
	pbView->set_pkvalue(m_stPlayerBase.uPKValue);

	PBGhostGameAttr* pInfo = pbView->mutable_attri();
	for ( int32 i = 0 ; i < _emBattleAttribute_ARRAYSIZE ; ++i )
	{
		if (bFindPlayer)
		{
			pInfo->add_attr(m_stGameAttribute.iAttribute[i]);
		}
		else
		{
			pInfo->add_attr(m_stGameAttributeBase.iAttribute[i]);
		}
	}

	////上阵兵法
	//if (m_pPlayerWentao)
	//	m_pPlayerWentao->packetBingfa(pbView);
	//
	//// pvp1论剑
	//if (m_pPlayerSinglePvp)
	//{
	//	m_pPlayerSinglePvp->packetSinglePvpView(pbView);
	//}

}
//-------------------------------------------------------------
//------------------------------
bool	CScenePlayer::randScene				()
{
	if ( isInCopymap() )
		return false;

	stl_vector<uint16> vecNormalMap;
	CSceneMapData::iterator _pos = g_clSceneMapData.begin();
	CSceneMapData::iterator _end = g_clSceneMapData.end();
	for ( ; _pos != _end ; ++_pos )
	{
		_stSceneMapModel& model = _pos->second;
		if (!model.isCopymap() && model.uSceneID != getSceneID() )
			vecNormalMap.push_back(model.uSceneID);
	}

	if ( vecNormalMap.empty() )
		return false;

	int32 nIndex = randRange((int32)vecNormalMap.size(),0);
	if (nIndex >= (int32)vecNormalMap.size())
	{
		nIndex = 0;
	}
	return playerFlyScene_ByCellID(vecNormalMap[nIndex],0,0);
}
//-------------------------------------------------------------
//------------------------------
bool	CScenePlayer::randPos					(uint16 uSpawnID)
{
	if (!m_pSceneCell || !m_pSceneCell->getSceneMap() )
		return false;

	_stPoint2F stPoint;
	if ( uSpawnID)
	{
		if(!m_pSceneCell->getSpawnPoint(stPoint,uSpawnID))
			return false;
	}
	else
	{
		if (!m_pSceneCell->getPathFinder().findRandomPoint(stPoint))
			return false;
	}

	setPosition(stPoint.x,stPoint.y,SetPositionReason_ChangePos,true);
	return true;
}
//-------------------------------------------------------------
//------------------------------
void	CScenePlayer::updatePKValue			(uint32 uValue,bool bAdd)
{
	if ( !uValue )
		return ;

	updateUint32_(m_stPlayerBase.uPKValue,uValue,bAdd);
	setGhost(_BIT32(Ghost_Pk));
}
//-------------------------------------------------------------
//------------------------------
void	CScenePlayer::refreshKillTime			()
{
	if ( !m_stPlayerBase.uKillTime  )
		setGhost(_BIT32(Ghost_Pk));

	m_stPlayerBase.uKillTime = getTime();
}

//-------------------------------------------------------------
//------------------------------

bool CScenePlayer::setPKType(uint8 uType,uint8& uCode)
{
	uCode = R_ResultCommon_Fail;
	if ( uType > _emPKType_MAX || uType < _emPKType_MIN)
		uType = _emPKType_MIN;

	m_stPlayerBase.uPKType = uType;

	PBG2CCommon_PKType ack;
	ack.set_pktype(m_stPlayerBase.uPKType);
	sendMsg(P_S2C_Protocol_Common,S2C_Common_PKType,&ack);

	uCode = R_ResultCommon_Succeed;
	return true;
}

//-------------------------------------------------------------
//------------------------------
uint16	CScenePlayer::getRelivePoint	()
{
	if ( !m_pSceneCell )
		return 0;

	const _stSceneMapModel* pSceneModel = m_pSceneCell->getSceneModel();
	if ( !pSceneModel)
		return 0;

	const uint16* pRegionID = pSceneModel->mapRelivePoint.find_(getCampID());
	if (!pRegionID)
	{
		pRegionID = pSceneModel->mapRelivePoint.find_(0);
	}
	
	return pRegionID ? *pRegionID : 0;
}
//-------------------------------------------------------------
//------------------------------
bool	CScenePlayer::addPrize				(uint32 uPrizeID)
{
	//CPlayerPrize* pPrize = (CPlayerPrize*)getPlayerFun(GameSystemType_Prize);
	//if ( !pPrize )
	//	return false;

	//return pPrize->addPrize(uPrizeID);
	return false;
}
//-------------------------------------------------------------
//------------------------------
void	CScenePlayer::collectTempSkill		(MAP_U16_U8& mapSkill)
{
	Parent::collectTempSkill(mapSkill);
	//收集技能
	for ( int32 i = 0 ; i < _emGameSystemType_ARRAYSIZE ; ++i )
	{
		CGamePlayerFun* pFun = m_arrPlayerFun[i];
		if ( pFun )
			pFun->collectSkill(mapSkill);
	}
}
//-------------------------------------------------------------
//------------------------------
bool	CScenePlayer::plane_enter				(uint8& uCode)
{
	addState(StateType_Plane);

	breakUseSkill();
	breakMove();
	uCode = R_ResultCommon_Succeed;
	return true;
}
//-------------------------------------------------------------
//------------------------------
bool	CScenePlayer::plane_leave				(PBC2GCommon_PlaneLeave& msg,uint8& uCode)
{
	if ( !delState(StateType_Plane) )
		return false;

	setPosition(msg.posx(),msg.posy(),SetPositionReason_ChangePos,true);
	setDirection(msg.dir());
	setHP(msg.curhp());
	for (int32 i = 0 ; i < msg.curenergy_size() ; ++i )
	{
		const PBEnergy& pbEnergy = msg.curenergy(i);
		int32 nChangeValue =  pbEnergy.value() - (int32)getEnergy(pbEnergy.type());
		if (nChangeValue)
			updateEnergy(pbEnergy.type(),nChangeValue, DoingType_Others);
	}
	resetGhost();
	//if (m_pPlayerQuestMgr)
	//{
	//	m_pPlayerQuestMgr->clearPlaneQuest();
	//}

	uCode = R_ResultCommon_Succeed;
	return true;
}
//-------------------------------------------------------------
//------------------------------
void	CScenePlayer::callbackClientGo		()
{
	if (isDead())
	{
		m_uAutoReliveTime = (uint64)(getTime() + g_clConstantData.Game_WaitAutoReliveTime);
	}

	for ( int32 i = 0 ; i < _emGameSystemType_ARRAYSIZE ; ++i )
	{
		CGamePlayerFun* pFun = m_arrPlayerFun[i];
		if ( pFun )
			pFun->onClientGo();
	}

	{//重新触发一下 进入区域事件 (前端 需要)
		triggerRegionEvent(RegionTriggerType_Enter);
	}
}
//-------------------------------------------------------------
//------------------------------
void	CScenePlayer::callback_flyout		()
{
	if (m_pSceneCell)
		m_pSceneCell->callback_flyout(this);

	//中断技能
	uint8 uCode;
	if (getSkilling().getSkillModel())
		getSkilling().breakSkillAsk(getSkilling().getSkillModel()->uSkillID,true,uCode);

	//清除buff
	getGameBuffMgr().callback_flyout();
}
//-------------------------------------------------------------
//------------------------------
void	CScenePlayer::processAutoRelive		(uint64 uTime)
{
	if ( !m_uAutoReliveTime || uTime < m_uAutoReliveTime || !isDead() )
		return ;

	uint8 uCode = 0;
	CPlayerRelive*	pRelive = getPlayerRelive()	;
	if (pRelive)
	{
		pRelive->relive(ReliveType_RelivePoint,uCode);
	}

	m_uAutoReliveTime = 0;
}
//-------------------------------------------------------------
//------------------------------
uint32	CScenePlayer::getMaxEnergy		(uint16 uType)const
{
	if (!m_pPlayerLevelModel)
		return MAX_ENERGE_NUM;

	return m_pPlayerLevelModel->stAttribute.iRadio[uType] > 0 ? m_pPlayerLevelModel->stAttribute.iRadio[uType] : MAX_ENERGE_NUM;
}

//-------------------------------------------------------------
//------------------------------
bool	CScenePlayer::sendMail	(_emMailType emType,const DMAP_NEED_ITEM* pDmapAddItem,_emDoingType emDoing,::google::protobuf::Message* msg/* = NULL*/)
{
	PBMail stMail;
	stMail.set_playerid(getPlayerID());
	stMail.set_worldid(getLogicWorldID()); 
	stMail.set_type(emType); 
	stMail.set_sendtime((uint64)getTime());
	stMail.set_doingtype(emDoing);

	if (pDmapAddItem)
	{
		CONST_ITER_DMAP_NEED_ITEM _pos = pDmapAddItem->begin();
		for (; _pos != pDmapAddItem->end(); ++_pos)
		{
			const DMAP_NEED_ITEM::_sub_map& stSubmap = _pos->second;
			DMAP_NEED_ITEM::sub_const_iterator _spos = stSubmap.begin();
			for (; _spos != stSubmap.end(); ++_spos)
			{
				PBItem* item = stMail.add_item();
				if (!item)
					continue;

				const _stItemModel* pItemModel = g_clItemData.find(_pos->first);
				if (!pItemModel)
					continue;

				item->set_itemid(_pos->first);
				item->set_itemnum(_spos->first);
				item->set_bingfuexp(pItemModel->uBingfuExp);

				uint32 uFlag = 0;
				_SET_BIT32(uFlag,ItemFlag_Binded,_spos->second);
				item->set_flag(uFlag);
			}
		}
	}
	if (msg)
	{
		switch (emType)
		{
		case MailType_Normal:
			break;
		case MailType_GM:
			break;
		case MailType_SysRemind:
			break;
		case MailType_SysAward:
			break;
		case MailType_SaleMoneyBuy:
			break;
		case MailType_SaleMoneySale:
			break;
		case MailType_SaleMoneyBuyRet:
			break;
		case MailType_SaleMoneySaleRet:
			break;
		case MailType_AuctionLockRet:
			break;
		case MailType_AuctionSaleTimeOut:
			break;
		case MailType_AuctionSaleGet:
			break;
		case MailType_AuctionBuyGet:
			break;
		case MailType_DropItem:
			break;
		case MailType_Promote:
			break;
		case MailType_FullBag:
			break;
		case MailType_Gift:
			break;
		case MailType_FactionBoss:
			break;
		case MailType_DartCarPrize:
			break;
		case MailType_ClimbTower:
			break;
		case MailType_WorldBoss:
			break;
		case MailType_FactionAlloct:
			break;
		case MailType_CashReward:
			break;
		case MailType_LeaderAway:
			break;
		case MailType_SinglePvpFirstPrize:
			stMail.mutable_singlepvpfirst()->CopyFrom(*msg);
			break;
		case MailType_SinglePvpDayPrize:
			stMail.mutable_singlepvpday()->CopyFrom(*msg);
			break;
		case MailType_SinglePvpWeekPrize:
			stMail.mutable_singlepvpweek()->CopyFrom(*msg);
			break;
		default:
			break;
		}
	}
	
	return CPlayerMail::sendMail(stMail);
}
//-------------------------------------------------------------
//------------------------------
bool	CScenePlayer::copymap_create	(uint16 uCopymapID,uint32 uParam)
{
	CPlayerCopymap* pPlayerCopymap = dynamic_cast<CPlayerCopymap*>(getPlayerFun(GameSystemType_Copymap));
	CHECKF(NULL != pPlayerCopymap);

	uint8 uCode = 0;
	return pPlayerCopymap->copymap_create(uCopymapID,uParam,uCode);
}
//-------------------------------------------------------------
//------------------------------是否有此身份
bool	CScenePlayer::haveIdentity	(uint16	uID)
{
	//CPlayerIdentity* pPlayerFun = dynamic_cast<CPlayerIdentity*>(getPlayerFun(GameSystemType_Identity));
	//CHECKF(NULL != pPlayerFun);
	//return pPlayerFun->haveIdentity(uID);
	return false;
}
//-------------------------------------------------------------
//------------------------------立即创建怪物
CSceneMonster*	CScenePlayer::createMonster	(uint32 uMonsterID,float32 fRadius/* = 1.0f*/)
{
	const _stMonsterModel*pModel = g_clMonsterData.find(uMonsterID);
	if (!pModel || !m_pSceneCell)
		return NULL;
	_stPoint2F stOut;
	if (m_pSceneCell->randGetPoint(stOut,getPosition().x,getPosition().y,fRadius))
	{
		return m_pSceneCell->createMonster(pModel,stOut.x,stOut.y,getRandDir(),0,true);
	}
	return NULL;
}
//-------------------------------------------------------------
//------------------------------获取副本日次数
uint32	CScenePlayer::getDayCopymapCount	(uint8 uCopymapMainType)
{
	uint32* pCount = m_stPlayerBase.mapCopymapDayCount.find_(uCopymapMainType);
	return pCount ? *pCount : 0;
}
//-------------------------------------------------------------
//------------------------------增加副本日次数
void	CScenePlayer::addDayCopymapCount	(uint8 uCopymapMainType,uint32 uCount/* = 1*/)
{
	uint32 uCurCount = getDayCopymapCount(uCopymapMainType);
	m_stPlayerBase.mapCopymapDayCount[uCopymapMainType] =  uCurCount + uCount;

	//通知客户端
	if (m_pPlayerCommon)
		m_pPlayerCommon->sendCopymapDayCountAck(uCopymapMainType,uCurCount + uCount);
}
//-------------------------------------------------------------
//------------------------------获取副本日次数
uint32	CScenePlayer::getDayTeamTaskCount	(uint32 uID)
{
	uint32* pCount = m_stPlayerBase.mapTeamTaskDayCount.find_(uID);
	return pCount ? *pCount : 0;
}
//-------------------------------------------------------------
//------------------------------增加副本日次数
void	CScenePlayer::addDayTeamTaskCount	(uint32 uID,uint32 uCount/* = 1*/,bool bSend/* = false*/)
{
	uint32 uCurCount = getDayTeamTaskCount(uID);
	m_stPlayerBase.mapTeamTaskDayCount[uID] =  uCurCount + uCount;

	//通知客户端
	if (m_pPlayerCommon && bSend)
		m_pPlayerCommon->sendTeamTaskDayCountAck(uID,uCurCount + uCount);
}
//-------------------------------------------------------------
//------------------------------
bool	CScenePlayer::isVisible				(CSceneUnit*pObject)
{
	if (!pObject)
		return false;

	CSceneMonster* pMonster = pObject->asMonster();
	if (pMonster && pMonster->isQuestLimit(this) )
		return false;

	return Parent::isVisible(pObject);
}
//-------------------------------------------------------------
//------------------------------
void	CScenePlayer::refreshVisibleMonster	()
{
	MAP_GHOST::iterator _pos = m_mapGhost.begin();
	MAP_GHOST::iterator _end = m_mapGhost.end();
	for (;_pos != _end;++_pos)
	{
		_stGhost&ghost = _pos->second;
		CSceneUnit* pUnit = ghost.pObject;
		if ( !pUnit )
			continue;

		CSceneMonster* pMonster = pUnit->asMonster();
		if ( !pMonster )
			continue;

		bool bVisible	= isVisible(pMonster);
		if ( !bVisible )
		{
			PBG2CCommonUnitRemove msg;
			msg.set_unittype(pMonster->getUnitType());
			msg.set_unitid(pMonster->getUnitID());
			sendMsg(P_S2C_Protocol_Common,S2C_Common_Unit_Remove,&msg);
		}
		else
		{
			ghost.updateMark = 0xffffffff;
			_SET_BIT32(ghost.updateMark,Ghost_Remove,false);
		}
	}
}
//-------------------------------------------------------------
//------------------------------
bool	CScenePlayer::mount	(bool bOnMount)
{
	uint8 uCode = 0;
	if (getMountID() > 0)
	{
		if (!bOnMount)
			return getGameItemMgr().mounting(false,uCode);
	}
	else
	{
		if (bOnMount)
			return getGameItemMgr().mounting(true,uCode);
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------
bool	CScenePlayer::toplist_update(uint32 uType,uint64 uValue,uint64 uSubValue)
{
	//CPlayerTopList* pPlayerTopList = dynamic_cast<CPlayerTopList*>(getPlayerFun(GameSystemType_TopList));
	//CHECKF(NULL != pPlayerTopList);
	//return pPlayerTopList->toplist_update(uType,uValue,uSubValue);
	return false;
}

//-------------------------------------------------------------
//------------------------------藏宝图任务完成
void	CScenePlayer::callbackCompleteTreasureQuest	(uint32 uQuestID,uint32 uItemID)
{
	const _stItemModel* pModel = g_clItemData.find(uItemID);
	if (!pModel)
		return;

	//发到世界服
	PBG2WCommonTreasurePrizeAsk ack;
	ack.set_playerid(getPlayerID());
	ack.set_treasuretype(pModel->uSubType);
	ack.set_treasureid(uItemID);
	ack.set_posx(getPosition().x);
	ack.set_posy(getPosition().y);
	sendMsgToWorld(P_G2W_Protocol_Common,G2W_Common_TreasurePrize,&ack);

	//CPlayerQuestMgr* pQuestMgr = getPlayerQuestMgr();
	//if (pQuestMgr)
	//	pQuestMgr->callback_completeTreasure();
}
//-------------------------------------------------------------
//------------------------------藏宝图任务完成
void	CScenePlayer::treasurePrize	(uint32 uTreasureID,uint32 uPrizeID,float32 fPosx,float32 fPosy)
{
	const _stItemModel* pItemModel = g_clItemData.find(uTreasureID);
	if (!pItemModel || pItemModel->uType != ItemType_Treasure)
		return;

	const _stTreasureMapModel* pTreasureModel = g_clTreasureMapData.findTreasureMap(pItemModel->uSubType);
	if (!pTreasureModel)
		return;

	const _stTreasurePrizeModel* pPrizeModel = g_clTreasureMapData.findTreasurePrize(uPrizeID);
	if (!pPrizeModel)
		return;
	uint8 uCode = 0;
	switch (pPrizeModel->uPrizeType)
	{
	case TreasurePrizeType_Item:
		{
			getGameItemMgr().addItem(&pPrizeModel->mapAddItem,true,DoingType_TreasureMap);

			//发公告
			if (pPrizeModel->uDayTreasureValue > 0)
			{
				PBG2CBroadCast_TreasurePrize msg;
				msg.set_playername(getName());
				msg.set_itemid(uTreasureID);
				g_pGameBroadcast->broadCastMsg(g_pServer->getLogicWorldID(),BroadcastChannel_World,0,\
					P_S2C_Protocol_Broadcast,S2C_BroadCast_TreasurePrize,&msg);
			}
		}
		break;
	case TreasurePrizeType_Expend:
		{
			addExpend(pPrizeModel->mapAddExpend,DoingType_TreasureMap);
		}
		break;
	case TreasurePrizeType_Buff:
		{
			getGameBuffMgr().addBuff(pPrizeModel->uAddBuffID,1,this->getUnitID(),0,DoingType_TreasureMap);
		}
		break;
	case TreasurePrizeType_EnterCopymap:
		{
			m_pPlayerCopymap->copymap_create(pPrizeModel->uEnterCopymapID,0,uCode);
		}
		break;
	case TreasurePrizeType_RefreshBoss:
		{
			for (uint32 i = 0; i < pPrizeModel->vecMonsterRefresh.size(); ++i)
			{
				const _stMonsterRefresh& stMonsterRefresh = pPrizeModel->vecMonsterRefresh[i];
				const _stMonsterModel* pMonsterModel = g_clMonsterData.find(stMonsterRefresh.uMonsterID);
				if (!pMonsterModel)
					continue;

				_stPoint2F stOut;
				for (uint32 j = 0; j < stMonsterRefresh.uMonsterNum; ++j)
				{
					if (m_pSceneCell->randGetPoint(stOut,fPosx,fPosy,stMonsterRefresh.fRandDis))
					{
						CHECKL(m_pSceneCell->createMonster(pMonsterModel,stOut.x,stOut.y,getRandDir(),0,true) != NULL);
					}
				}
			}

			//发公告
			if (pPrizeModel->uDayTreasureValue > 0)
			{
				PBG2CBroadCast_TreasurePrize_RefreshBoss msg;
				msg.set_playername(getName());
				msg.set_sceneid(getSceneID());
				g_pGameBroadcast->broadCastMsg(g_pServer->getLogicWorldID(),BroadcastChannel_World,0,\
					P_S2C_Protocol_Broadcast,S2C_BroadCast_TreasurePrize_RefreshBoss,&msg);
			}
		}
		break;
	case TreasurePrizeType_CopymapNpc:
		{
			const _stMonsterModel* pMonsterModel = g_clMonsterData.find(pPrizeModel->uCopymapNpc);
			if (!pMonsterModel)
				return;

			CSceneMonster* pMonster = m_pSceneCell->createMonster(pMonsterModel,fPosx,fPosy,getRandDir(),0u,true);
			if (!pMonster)
				return;

			pMonster->setOwner(this);
		}
		break;
	default:
		break;
	}
	
	//通知客户端
	PBG2CCommonTreasurePrize ack;
	ack.set_prizeid(uPrizeID);
	ack.set_treasureid(uTreasureID);
	sendMsg(P_S2C_Protocol_Common,S2C_Common_TreasurePrize,&ack);
}

//-------------------------------------------------------------
//------------------------------
void	CScenePlayer::addForbitSpeak	(uint32 uChannel,uint32 uAddSecond)
{
	uint32 uTime = getTime();

	uint32 uNextTime = m_stPlayerBase.mapForbitSpeak[uChannel];
	if (uNextTime < uTime)
	{
		uNextTime = uTime + uAddSecond;
	}
	else
	{
		uNextTime += uAddSecond;
	}
	m_stPlayerBase.mapForbitSpeak[uChannel] = uNextTime;
	//同步给客户端
	PBForbidSpeak ack;
	ack.set_channel(uChannel);
	ack.set_nexttime(uNextTime);
	sendMsg(P_S2C_Protocol_Common,S2C_Common_SynForbitSpeak,&ack);
}
//-------------------------------------------------------------
//------------------------------
void	CScenePlayer::setCorpsJob				(uint32 uJob)
{
	m_stPlayerBase.uCorpsJob = uJob;	

	//同步客户端
	PBG2CCommonSetCorpsJob ack;
	ack.set_corpsjob(uJob);
	sendMsg(P_S2C_Protocol_Common,S2C_Common_SynForbitSpeak,&ack);
}
//-------------------------------------------------------------
//------------------------------
void	CScenePlayer::setCampID		(uint8 uCampID)
{
	m_stPlayerBase.uCampID = uCampID;
	Parent::setCampID(uCampID);
}