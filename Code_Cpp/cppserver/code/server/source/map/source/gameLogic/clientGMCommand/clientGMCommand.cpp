/*----------------- clientGMCommand.cpp
*
* Copyright (C): 2014 Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2014/22/10 9:21
*--------------------------------------------------------------
* 
*------------------------------------------------------------*/
#include "basic/stringFunctions.h"
#include "log/log.h"
#include <sstream>
#include<memory>
//------------------------------ 
#include "protocol/CASProtocol.pb.h"
#include "protocol/WAGMsg.pb.h"
#include "protocol/Team.pb.h"
#include "protocol/MsgResult.pb.h"
#include "protocol/CASMsg.pb.h"
#include "protocol/GLAGProtocol.pb.h"
#include "google/protobuf/descriptor.h"
//------------------------------ 
#include "main/gameServer.h"
#include "packet/clientPacket.h"
#include "json_c/include/json_object.h"
#include "json2pb.h"
#include "HttpClient.h"
#include "gameScene/gameScene.h"
#include "gameScene/gameSceneMgr.h"
#include "def_functions/protobufFun.h"
//------------------------------ 
#include "cfg_data/loadData/loadData.h"
#include "cfg_data/trapData/trapData.h"
#include "cfg_data/monsterData/monsterData.h"
#include "cfg_data/sceneFlyData/sceneFlyData.h"
#include "cfg_data/gmData/gmData.h"
#include "cfg_data/copymapData//copymapData.h"
#include "cfg_data/itemData/itemData.h"
#include "cfg_data/questData/questData.h"
#include "cfg_data/systemSwitchData/systemSwitchData.h"
#include "cfg_data/teamtargetData/teamtargetData.h"
//------------------------------ 
#include "gameLogic/clientGMCommand/clientGMCommand.h"
#include "gameLogic/sceneObject/scenePlayer.h"
#include "gameLogic/gameItem/gameItemManager.h"
#include "gameLogin/gamePlayerMgr.h"
//#include "gameLogic/playerQuest/playerQuestMgr.h"
//#include "gameLogic/playerTrade/playerTrade.h"
//#include "gameLogic/playerSaleMoney/playerSaleMoney.h"
//#include "gameLogic/playerAuction/playerAuction.h"
//#include "gameLogic/playerTopList/playerTopList.h"
//#include "gameLogic/playerFriend/playerFriend.h"
//#include "gameLogic/playerWenTao/playerWenTao.h"
//#include "gameLogic/playerWulue/playerWulue.h"
//#include "gameLogic/playerTeam/playerTeam.h"
//#include "gameLogic/playerDropItem/playerDropItem.h"
#include "gameLogic/gameBroadcast/gameBroadcast.h"
#include "gameLogic/playerCopymap/playerCopymap.h"
#include "gameLogic/playerCommon/playerCommon.h"
//#include "gameLogic/playerIdentity/playerIdentity.h"
//#include "gameLogic/playerWuxue/playerWuxue.h"
//#include "gameLogic/playerSystemSwitch/playerSystemSwitch.h"
#include "gameLogic/sceneObject/sceneUnitAI.h"
//#include "gameLogic/playerDart/playerDart.h"
//#include "gameLogic/playerFactionBoss/playerFactionBoss.h"
#include "gameLogic/gameMail/gameMail.h"
//#include "gameLogic/playerTitle/playerTitle.h"
//#include "gameLogic/playerFashion/playerFashion.h"
//#include "gameLogic/playerEquipColumn/playerEquipColumn.h"
//#include "gameLogic/playerColletion/playerColletion.h"
//#include "gameLogic/playerPvpWar/playerPvpWar.h"
//#include "gameLogic/playerWorldBoss/playerWorldBoss.h"
//#include "gameLogic/globalObject/worldBossMgr.h"
//#include "gameLogic/playerActivity/playerActivity.h"
#include "gameLogic/playerRelive/playerRelive.h"
//#include "gameLogic/playerWeal/playerWeal.h"
//#include "gameLogic/playerFaction/playerFaction.h"
//#include "gameLogic/playerClimbTower/playerClimbTower.h"
//#include "gameLogic/playerSinglePvp/playerSinglePvp.h"
/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
CClientGMCommand::CClientGMCommand(CScenePlayer& clPlayer):m_clPlayer(clPlayer)
{
	m_mapCmd.clear();
#define _bind_gm(cmd)	addCMD(#cmd, (_cmdTextMsg)&CClientGMCommand::cmd, "")
	//_bind_gm(help);
	//_bind_gm(cleanday);
	//_bind_gm(trap);
	//_bind_gm(pathobj);
	//_bind_gm(path);
	//_bind_gm(ray);
	//_bind_gm(move);
	//_bind_gm(exp);
	//_bind_gm(level);
	//_bind_gm(promote);
	//_bind_gm(fly);
	//_bind_gm(flypos);
	//_bind_gm(fly1);
	//_bind_gm(baseinfo);
	//_bind_gm(useskill);
	//_bind_gm(addskill);
	//_bind_gm(upgradeskill);
	//_bind_gm(breakskill);
	//_bind_gm(monster);
	//_bind_gm(dartcar);
	//_bind_gm(openmp);
	//_bind_gm(closemp);
	//_bind_gm(addbuff);
	//_bind_gm(delbuff);
	//_bind_gm(addattribute);
	//_bind_gm(addexpend);
	//_bind_gm(additem);
	//_bind_gm(delitem);
	//_bind_gm(sellitem);
	//_bind_gm(equipitem);
	//_bind_gm(moveitem);
	//_bind_gm(buybackitem);
	//_bind_gm(useitem);
	//_bind_gm(splititem);
	//_bind_gm(clearbag);
	//_bind_gm(getposition);
	//_bind_gm(clearmonster);
	//_bind_gm(offlinedata);
	//_bind_gm(sendmail);
	//_bind_gm(gmmail);
	//_bind_gm(questaccept);
	//_bind_gm(questgiveup);
	//_bind_gm(questcomplete);
	//_bind_gm(questupdate);
	//_bind_gm(questfire);
	//_bind_gm(questauto);
	//_bind_gm(questreset);
	//_bind_gm(reloaddata);
	//_bind_gm(copymapenter);
	//_bind_gm(copymapcreate);
	//_bind_gm(copymapexit);
	//_bind_gm(copymapinvite);
	//_bind_gm(copymapacceptinvite);
	//_bind_gm(copymapteammemberinfo);
	//_bind_gm(copymapclear);
	//_bind_gm(copymapcomplete);	
	//_bind_gm(tradeapply);
	//_bind_gm(tradeanswer);
	//_bind_gm(tradeadditem);
	//_bind_gm(tradedelitem);
	//_bind_gm(tradesetmoney);
	//_bind_gm(tradelock);
	//_bind_gm(tradeconfirm);
	//_bind_gm(tradecancel);
	//_bind_gm(salemoneysale);
	//_bind_gm(salemoneysalecancel);
	//_bind_gm(salemoneylist);
	//_bind_gm(auctionsale);
	//_bind_gm(auctionsalecancel);
	//_bind_gm(auctionbuy);
	//_bind_gm(auctionlist);
	//_bind_gm(auctionave);
	//_bind_gm(toplistlist);
	//_bind_gm(toplistplayer);
	//_bind_gm(toplistupdate);
	//_bind_gm(toplistrand);
	//_bind_gm(toplist_resettime);
	//_bind_gm(randpos);
	//_bind_gm(call);
	//_bind_gm(find);
	//_bind_gm(go);
	//_bind_gm(pkupdate);
	//_bind_gm(friendadd);
	//_bind_gm(frienddel);
	//_bind_gm(friendanswer);
	//_bind_gm(friendvalue);
	//_bind_gm(zisha);
	//_bind_gm(fuhuo);
	//_bind_gm(splitbingfu);
	//_bind_gm(upgradebingfu);
	//_bind_gm(upgradewulue);
	//_bind_gm(setbingfu);
	//_bind_gm(wentaosetscheme);
	//_bind_gm(wentaoqianxiu);
	//_bind_gm(wentaotupo);
	//_bind_gm(wentaoupgrate);
	//_bind_gm(wentaosetbingfu);
	//_bind_gm(wentaoreset);
	//_bind_gm(wentaoactivescheme);
	//_bind_gm(wentaoactivebingfa);
	//_bind_gm(teaminvite);
	//_bind_gm(teamacceptinvite);
	//_bind_gm(teamapply);
	//_bind_gm(teamacceptapply);
	//_bind_gm(teamleave);
	//_bind_gm(teamchangeleader);
	//_bind_gm(teampublishask);
	//_bind_gm(teampublishcancel);
	//_bind_gm(teampublishinvite);
	//_bind_gm(teampublishacceptinvite);
	//_bind_gm(teampublishlist);
	//_bind_gm(teamkick);
	//_bind_gm(teamkickvote);
	//_bind_gm(teamsetallot);
	//_bind_gm(teamsetallotvote);
	//_bind_gm(teamnearplayerlist);
	//_bind_gm(teamnearteamlist);
	//_bind_gm(dropitem);
	//_bind_gm(dropiteminfo);
	//_bind_gm(dropitemdice);
	//_bind_gm(dropitemassign);
	//_bind_gm(dropitempickup);
	//_bind_gm(equipenhance);
	//_bind_gm(equipforge);
	//_bind_gm(equipinherit);
	//_bind_gm(equipmake);
	//_bind_gm(mount);
	//_bind_gm(skillcmd);
	//_bind_gm(broadcast);
	//_bind_gm(httpstart);
	//_bind_gm(httpstop);
	//_bind_gm(httprequest);
	//_bind_gm(httpresponse);
	//_bind_gm(bosslist);
	//_bind_gm(identity_open);
	//_bind_gm(identity_learn);
	//_bind_gm(identity_reset);
	//_bind_gm(identity_upskill);
	//_bind_gm(addliveblock);
	//_bind_gm(callfollower);
	//_bind_gm(clearskillcooltime);
	//_bind_gm(clearskillexpend);
	//_bind_gm(supply);
	//_bind_gm(supplyextra);
	//_bind_gm(save);
	//_bind_gm(worldstarttime);
	//_bind_gm(itemlike);
	//_bind_gm(buy);
	//_bind_gm(debugmsg);
	//_bind_gm(wuxuenew);
	//_bind_gm(wuxuedel);
	//_bind_gm(wuxuechange);
	//_bind_gm(wuxuelearnskill);
	//_bind_gm(sysopen);
	//_bind_gm(randscene);
	//_bind_gm(callup);
	//_bind_gm(acceptidentity);
	//_bind_gm(factionbosspersonaltop);
	//_bind_gm(factionbossfactiontop);
	//_bind_gm(factionbossuniontop);
	//_bind_gm(qinggong);
	//_bind_gm(battleattrdetail);
	//_bind_gm(setdtime);
	//_bind_gm(getdtime);
	//_bind_gm(resumedtime);
	//_bind_gm(benew);
	//_bind_gm(enterplane);
	//_bind_gm(dazuo);
	//_bind_gm(addtitle);
	//_bind_gm(deltitle);
	//_bind_gm(teamcreatebytargetrandom);
	//_bind_gm(teamapplybytarget);
	//_bind_gm(fasionadd);
	//_bind_gm(fasiondel);
	//_bind_gm(clientpbmsg);
	//_bind_gm(setpkmode);
	//_bind_gm(acceptteamtask);
	//_bind_gm(enterteamtaskcopymap);
	//_bind_gm(completeamtask);  
	//_bind_gm(getitemsn);
	//_bind_gm(addcontribution);
	//_bind_gm(entermatch);
	//_bind_gm(exitmatch);
	//_bind_gm(enterready);
	//_bind_gm(exitready);
	//_bind_gm(openworldbossdlg);
	//_bind_gm(runworldbossevent);
	//_bind_gm(testworldbossbroadcast);
	//_bind_gm(activityreset);
	//_bind_gm(relive);
	//_bind_gm(getdropfromwb);
	//_bind_gm(freezewb);
	//_bind_gm(teamconfirm);
	//_bind_gm(setmakeskilled);
	//_bind_gm(weallogin);
	//_bind_gm(factioncreate);
	//_bind_gm(factionapply);
	//_bind_gm(stronger);

	//_bind_gm(MEOpen);
	//_bind_gm(MEOpenoem);
	//_bind_gm(MECommitoem);
	//_bind_gm(MEVisit);
	//_bind_gm(MEOem);


	//_bind_gm(setCT);


	//_bind_gm(MANORopenall);
	//_bind_gm(MANORopenfight);
	//_bind_gm(MANORapply);
	//_bind_gm(MANORenterlobby);
	//_bind_gm(MANORmvpvote);
	//_bind_gm(MANORenterbf);
	//_bind_gm(KSSQ);
	//_bind_gm(GBSQ);
	//_bind_gm(KSLDZ);
	//_bind_gm(GBLDZ);


	//_bind_gm(entersinglematch);
	//_bind_gm(exitsinglematch);
	//_bind_gm(singledayprize);

#undef _bind_gm
}

//-------------------------------------------------------------
//------------------------------ 
CClientGMCommand::~CClientGMCommand()
{

}
//------------------------------------------------------------------------------
//------------------------------------- 
bool	CClientGMCommand::handle_txt_cmd		(uint8 uEventID,CNetStream& clStream)
{
	if(!g_pServer->checkGameSwitch(GameSwitch_ClientGM))
		return true;

	PBC2GGMCmdTxtCmd cmd;
	if ( !clStream.ParseFromStream(cmd))
		return false;

	std::string strParam = cmd.param();
	_convertUTF8toANSI(strParam, strParam);
	std::string strCmd;
	getSubString(strParam, strCmd, ' ');
	strParam = trim(strParam);

	PBG2CGMCmdTxtCmd ack;
	ack.set_callbackid(cmd.callbackid());
	auto _pos = m_mapCmd.find(strCmd);
	if ( _pos != m_mapCmd.end()  )
	{
		_cmdTextMsg func = _pos->second.func;
		std::string outMsg;
		bool bRet = false;
		if (func)
			bRet = (this->*func)(strParam,outMsg);
		else
			outMsg = strParam + " 指令未指定处理函数";

		//转会编码;
		std::string utf8Msg;
		_convertANSItoUTF8(outMsg, utf8Msg);
		ack.set_result(bRet);
		ack.set_msg(utf8Msg);
		//CLog::print("gm[%s] ret=%s", strCmd.c_str(), outMsg.c_str());
		m_clPlayer.sendMsg(P_S2C_Protocol_Common,S2C_Common_GMCmd, &ack);
	}
	else
	{	
		CNetStream stStream;
		stStream.write(m_clPlayer.getPlayerID());
		stStream.SerializeToStream(cmd);

		m_clPlayer.sendBufferToWorld(P_G2W_Protocol_GM, G2W_Protocal_GMCmd, \
			stStream.getBuffer(), stStream.getPosition());
	}

	return true;
}

//------------------------------------------------------------------------------
//------------------------------------- 
void CClientGMCommand::addCMD(const char* pCmd, _cmdTextMsg pFun, const char* pDesc)
{
	m_mapCmd.insert_(pCmd, _stGMCommand(pFun, pDesc));
}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::help		(std::string&strParam,std::string&outMsg)
//{
//	std::stringstream sstr;
//	const _stGMModel* gmModel = g_clGMData.find(strParam);
//	if (gmModel)
//	{
//		sstr << strParam << ":\r\n";
//		sstr << "[desc=" << gmModel->sDesc << "]\r\n";
//		if (!gmModel->sRemark.empty())
//		{
//			sstr << "[remark=" << gmModel->sRemark << "]\r\n";
//		}
//		if (!gmModel->sP1type.empty() && !gmModel->sP1desc.empty())
//		{
//			sstr << "[1=" << gmModel->sP1type << ":" << gmModel->sP1desc << "]" << "\r\n";
//		}
//		if (!gmModel->sP2type.empty() && !gmModel->sP2desc.empty())
//		{
//			sstr << "[2=" << gmModel->sP2type << ":" << gmModel->sP2desc << "]" << "\r\n";
//		}
//		if (!gmModel->sP3type.empty() && !gmModel->sP3desc.empty())
//		{
//			sstr << "[3=" << gmModel->sP3type << ":" << gmModel->sP3desc << "]" << "\r\n";
//		}
//		if (!gmModel->sP4type.empty() && !gmModel->sP4desc.empty())
//		{
//			sstr << "[4=" << gmModel->sP4type << ":" << gmModel->sP4desc << "]";
//		}
//	}
//	outMsg = sstr.str();
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::move				(std::string&strParam,std::string&outMsg)
//{
//	float32 uPosX	= (float32)readInt64FromString(strParam,' ');
//	float32 uPosY	= (float32)readInt64FromString(strParam,' ');
//	if (m_clPlayer.getSceneCell()->canMove(uPosX, uPosY))
//	{
//		//int anchor = 0;
//	}
//	//CGameSceneCell* pell = m_clPlayer.getSceneCell();
//	//if (pell&&pell->canMove(uPosX,uPosY))
//	{
//		m_clPlayer.setPosition(uPosX,uPosY,SetPositionReason_Gm,true);
//	}
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::cleanday				(std::string&strParam,std::string&outMsg)
//{
//	uint32 uDays		= (uint32)readInt64FromString(strParam,' ');
//	uDays = MAX((uint32)1,uDays);
//	PBG2CCommon_TimeEvent msg;
//	msg.set_newday(true);
//	m_clPlayer.gmCleanDailyTime();
//	m_clPlayer.dailyCleanup(0,uDays);
//	if (uDays/7 > 0)
//	{
//		m_clPlayer.weekCleanup(0,uDays/7);
//		msg.set_newweek(true);
//	}
//	if (uDays/30 > 0)
//	{
//		m_clPlayer.monthCleanup(0,uDays/7);
//		msg.set_newmonth(true);
//	}
//	m_clPlayer.sendMsg(P_S2C_Protocol_Common,S2C_Common_TimeEvent,&msg);
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::trap				(std::string&strParam,std::string&outMsg)
//{
//	uint32 uTrapID		= (uint32)readInt64FromString(strParam,' ');
//	uint16 uLevel		= (uint16)readInt64FromString(strParam,' ');
//	CGameSceneCell* pCell = m_clPlayer.getSceneCell();
//	if (!pCell)
//		return false;
//
//	const _stTrapModel*pModel = g_clTrapData.findTrap(uTrapID,uLevel);
//	if (!pModel)
//	{
//		outMsg = "not found trap model!";
//		return false;
//	}
//
//	pCell->newTrap( &m_clPlayer,pModel,m_clPlayer.getPosition().x,m_clPlayer.getPosition().y,m_clPlayer.getDirection(),0.0f,0);
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::pathobj				(std::string&strParam,std::string&outMsg)
//{
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::path				(std::string&strParam,std::string&outMsg)
//{
//	CGameSceneCell* pScene = m_clPlayer.getSceneCell();
//	if (!pScene )
//		return false;
//
//	_stPoint2F start;
//	_stPoint2F end;
//
//	start.x =  (float32)readfloat64FromString(strParam);
//	start.y =  (float32)readfloat64FromString(strParam);
//
//	end.x =  (float32)readfloat64FromString(strParam);
//	end.y =  (float32)readfloat64FromString(strParam);
//
//
//	stl_vector<_stPoint2F> vecPath;
//	if (!pScene->findPath(start,end,vecPath))
//	{
//		outMsg = "not found!";
//		return false;
//	}
//
//	char szBuff[256];
//	for ( stl_vector<_stPoint2F>::iterator _pos = vecPath.begin(); _pos != vecPath.end() ; ++_pos )
//	{
//		dSprintf(szBuff,sizeof(szBuff),"%f %f\n",_pos->x,_pos->y);
//		outMsg += szBuff;
//	}
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::ray				(std::string&strParam,std::string&outMsg)
//{
//	CGameSceneCell* pScene = m_clPlayer.getSceneCell();
//	if (!pScene )
//		return false;
//
//	_stPoint2F start;
//	_stPoint2F end;
//	start.x =  (float32)readfloat64FromString(strParam);
//	start.y =  (float32)readfloat64FromString(strParam);
//	end.x =  (float32)readfloat64FromString(strParam);
//	end.y =  (float32)readfloat64FromString(strParam);
//
//	_stPoint2F target;
//	if (!pScene->getTargetPoint(start,end,target))
//	{
//		outMsg = "error position!";
//		return false;
//	}
//
//	char szBuff[256];
//	dSprintf(szBuff,sizeof(szBuff),"%f %f\n",target.x,target.y);
//	outMsg += szBuff;
//
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::exp(std::string&strParam,std::string&outMsg)
//{
//	int32 n = (int32)readInt64FromString(strParam,' ');
//	return m_clPlayer.addExp(n, DoingType_GM);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::level(std::string&strParam,std::string&outMsg)
//{
//	int32 n = (int32)readInt64FromString(strParam, ' ');
//	return m_clPlayer.addLevel(n, DoingType_GM);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::promote(std::string&strParam,std::string&outMsg)
//{
//	uint8 uCode = 0;
//	return m_clPlayer.promote(uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::fly		(std::string&strParam,std::string&outMsg)
//{
//	uint16 uScene			= (uint16)readInt64FromString(strParam,' ');
//	uint32 uSceneCellID		= (uint32)readInt64FromString(strParam,' ');
//	uint32 uSceneRegionID	 = (uint32)readInt64FromString(strParam,' ');
//
//	return m_clPlayer.playerFlyScene_ByCellID(uScene,uSceneCellID,uSceneRegionID);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::flypos		(std::string&strParam,std::string&outMsg)
//{
//	uint16 uScene			= (uint16)readInt64FromString(strParam,' ');
//	uint32 x				= (uint32)readInt64FromString(strParam,' ');
//	uint32 y				= (uint32)readInt64FromString(strParam,' ');
//
//	return m_clPlayer.playerFlyScene_ByCellID(uScene,m_clPlayer.getSceneCellID(),(float32)x, (float32)y);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::fly1		(std::string&strParam,std::string&outMsg)
//{
//	uint16 uScene		= (uint16)readInt64FromString(strParam,' ');
//	uint64 uSceneDynID = (uint64)readInt64FromString(strParam,' ');
//	uint32 uSceneRegionID	 = (uint32)readInt64FromString(strParam,' ');
//	return m_clPlayer.playerFlyScene_ByDynID(uScene,uSceneDynID,uSceneRegionID);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::baseinfo				(std::string&strParam,std::string&outMsg)
//{
//	PBPlayerData pbPlayer;
//	m_clPlayer.saveDataToProtobuf(pbPlayer);
//	outMsg = pb2json(pbPlayer.playerbase());
//	return true;
//}
//
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::useskill				(std::string&strParam,std::string&outMsg)
//{
//	uint32 uSkillID		= (uint32)readInt64FromString(strParam,' ');
//
//	uint8 uCode = R_ResultSkill_Fail;
//	_stPoint2F stPoint = m_clPlayer.getPosition();
//	m_clPlayer.getGameSkillMgr().useSkillAsk(uSkillID,m_clPlayer.getDirection(),(uint32)0,stPoint,uCode);
//	return true;
//}
//
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::addskill				(std::string&strParam,std::string&outMsg)
//{
//	uint32 uSkillID		= (uint16)readInt64FromString(strParam,' ');
//	uint8 uLevel		= 1;
//
//	uint8 uCode = R_ResultSkill_Fail;
//	m_clPlayer.getGameSkillMgr().addSkillAsk(uSkillID,uLevel,uCode);
//	return true;
//}
//
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::upgradeskill				(std::string&strParam,std::string&outMsg)
//{
//	uint32 uSkillID		= (uint32)readInt64FromString(strParam,' ');
//	uint8 uLevel		=  (uint8)readInt64FromString(strParam,' ');
//
//	uint8 uCode = R_ResultSkill_Fail;
//	m_clPlayer.getGameSkillMgr().upgradeSkillAsk(uSkillID,uLevel,uCode);
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::breakskill				(std::string&strParam,std::string&outMsg)
//{
//	uint32 uSkillID		= (uint32)readInt64FromString(strParam,' ');
//
//	uint8 uCode = R_ResultSkill_Fail;
//	m_clPlayer.getSkilling().breakSkillAsk(uSkillID,true,uCode);
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::monster				(std::string&strParam,std::string&outMsg)
//{
//	uint32 uMonsterID		= (uint32)readInt64FromString(strParam,' ');
//	uint32 uCount			= (uint32)readInt64FromString(strParam,' ');
//	float32 fRadius			= (float32)readfloat64FromString(strParam,' ');
//	uint16 uLevel			= (uint16)readInt64FromString(strParam,' ');
//
//	CGameSceneCell* pCell = m_clPlayer.getSceneCell();
//	if (!pCell)
//		return false;
//
//	uCount = MAX((uint32)1, uCount);
//	if ( uCount > 1000 )
//		uCount = 1000;
//
//	if ( fRadius < 1.0f )
//		fRadius = 1.0f;
//
//	const _stMonsterModel*pModel = g_clMonsterData.find(uMonsterID);
//	if (!pModel)
//	{
//		outMsg = "not found monster model!";
//		return false;
//	}
//
//	_stPoint2F stOut;
//	for ( uint32 i = 0 ; i < uCount ;++i )
//	{
//		if (pCell->randGetPoint(stOut,m_clPlayer.getPosition().x,m_clPlayer.getPosition().y,fRadius))
//		{
//			CSceneMonster *p = pCell->createMonster(pModel,stOut.x,stOut.y,getRandDir(), 0, true,uLevel);
//			CHECKF(p != NULL);
//		}
//	}
//	return true;
//}
//
//
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::openmp				(std::string&strParam,std::string&outMsg)
//{
//	//uint32 mpID		= (uint32)readInt64FromString(strParam,' ');
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::closemp				(std::string&strParam,std::string&outMsg)
//{
//	//uint32 mpID		= (uint32)readInt64FromString(strParam,' ');
//	//m_clPlayer.delState(StateType_Multi_Plane);
//	return true;
//}
//
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::dartcar				(std::string&strParam,std::string&outMsg)
//{
//	uint32 uDartID					= (uint32)readInt64FromString(strParam,' ');
//	uint32 uRobTriggleTime			= (uint32)readInt64FromString(strParam,' ');
//	uint32 uCount			= 1;
//	float32 fRadius			= 1;
//
//	CGameSceneCell* pCell = m_clPlayer.getSceneCell();
//	if (!pCell)
//		return false;
//
//	uCount = MAX((uint32)1, uCount);
//	if ( uCount > 1000 )
//		uCount = 1000;
//
//	if ( fRadius < 1.0f )
//		fRadius = 1.0f;
//
//	uint8 uCode = 0;
//	m_clPlayer.getPlayerDart()->onStartDart(uDartID, 0, uCode, uRobTriggleTime, true);
//	return true;
//}
//
//
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::addbuff				(std::string&strParam,std::string&outMsg)
//{
//	uint32 uBuffID		= (uint32)readInt64FromString(strParam,' ');
//	uint8 uLevel		= (uint8)readInt64FromString(strParam,' ');
//
//	m_clPlayer.getGameBuffMgr().addBuff(uBuffID,uLevel,m_clPlayer.getUnitID(),0, DoingType_GM);
//	return true;
//}
//
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::delbuff				(std::string&strParam,std::string&outMsg)
//{
//	uint32 uBuffID		= (uint32)readInt64FromString(strParam,' ');
//	//uint8 uCode = R_ResultBuff_Fail;
//	m_clPlayer.getGameBuffMgr().delBuff(uBuffID, DoingType_GM);
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::addattribute				(std::string&strParam,std::string&outMsg)
//{
//	uint8 uAttriType	= (uint8)readInt64FromString(strParam,' ');
//	int32 iValue		= (int32)readInt64FromString(strParam,' ');
//	m_clPlayer.addAttribute(GameSystemType_Common,uAttriType,iValue,0);
//	m_clPlayer.setUpdateValue(_UV_AttributeBase);
//	_stGameAttribute* pAttr = m_clPlayer.getAttribute (GameSystemType_Common);
//	CHECKF(pAttr != NULL);
//	char szTmp[1024] = "";
//	dSprintf(szTmp,sizeof(szTmp),"value=%d",pAttr->getAttribute(uAttriType));
//	outMsg.append(szTmp);
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::addexpend				(std::string&strParam,std::string&outMsg)
//{
//	uint16 uExpendType	= (uint16)readInt64FromString(strParam,' ');
//	int64 iValue		= (int64)readInt64FromString(strParam,' ');
//	return m_clPlayer.addExpend(uExpendType,iValue, DoingType_GM);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::additem(std::string&strParam,std::string&outMsg)
//{
//	CGameItemMgr& clGameItemMgr =  m_clPlayer.getGameItemMgr();
//	uint32 uItemID			= (uint32)readInt64FromString(strParam,' ');
//	uint32 uCount			= (uint32)readInt64FromString(strParam,' ');
//	bool bIsBind			= readInt64FromString(strParam,' ') > 0;
//	return clGameItemMgr.addItem(uItemID,uCount,bIsBind,true, true, DoingType_GM);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::delitem(std::string&strParam,std::string&outMsg)
//{
//	CGameItemMgr& clGameItemMgr =  m_clPlayer.getGameItemMgr();
//	uint32 uItemID			= (uint32)readInt64FromString(strParam,' ');
//	uint32 uCount			= (uint32)readInt64FromString(strParam,' ');
//	uint8 uCode = 0;
//	return clGameItemMgr.removeItem(uItemID,uCount,true,uCode,ItemBindType_All, DoingType_GM);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::sellitem(std::string&strParam,std::string&outMsg)
//{
//	CGameItemMgr& clGameItemMgr =  m_clPlayer.getGameItemMgr();
//	uint64 uItemSN	= (uint64)readInt64FromString(strParam,' ');
//	uint8 uCode = 0;
//	return clGameItemMgr.sellItem(uItemSN,uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::equipitem(std::string&strParam,std::string&outMsg)
//{
//	CGameItemMgr& clGameItemMgr =  m_clPlayer.getGameItemMgr();
//	uint64 uItemSN		= (uint64)readInt64FromString(strParam,' ');
//	uint8 uEquipType	= (uint8)readInt64FromString(strParam,' ');
//	uint8 uCode = 0;
//	return clGameItemMgr.equipItem(uItemSN,uEquipType,uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::moveitem(std::string&strParam,std::string&outMsg)
//{
//	CGameItemMgr& clGameItemMgr =  m_clPlayer.getGameItemMgr();
//	uint64 uFromItemSN	= (uint64)readInt64FromString(strParam,' ');
//	uint64 uToItemSN	= (uint64)readInt64FromString(strParam,' ');
//	uint16 uNum			= (uint16)readInt64FromString(strParam,' ');
//	uint8 uCode = 0;
//	return clGameItemMgr.moveItem(uFromItemSN,uToItemSN,uNum,uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::buybackitem(std::string&strParam,std::string&outMsg)
//{
//	CGameItemMgr& clGameItemMgr =  m_clPlayer.getGameItemMgr();
//	uint64 uItemSN	= (uint64)readInt64FromString(strParam,' ');
//	uint8 uCode = 0;
//	return clGameItemMgr.buyBackItem(uItemSN,uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::useitem(std::string&strParam,std::string&outMsg)
//{
//	CGameItemMgr& clGameItemMgr =  m_clPlayer.getGameItemMgr();
//	uint64 uItemSN		= (uint64)readInt64FromString(strParam,' ');
//	uint16 uNum			= (uint16)readInt64FromString(strParam,' ');
//	uint8 uCode = 0;
//	return clGameItemMgr.useItem(uItemSN,uNum,uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::splititem(std::string&strParam,std::string&outMsg)
//{
//	CGameItemMgr& clGameItemMgr =  m_clPlayer.getGameItemMgr();
//	SET_U64 setItemSN;
//	while (!strParam.empty())
//	{
//		uint64 uItemSN	= (uint64)readInt64FromString(strParam,';');
//		setItemSN.insert(uItemSN);
//	}
//	uint8 uCode = 0;
//	return clGameItemMgr.splitItem(setItemSN,uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::clearbag(std::string&strParam,std::string&outMsg)
//{
//	m_clPlayer.getGameItemMgr().clearBag();
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::getposition				(std::string&strParam,std::string&outMsg)
//{
//	uint32 uUnitID = (uint32)readInt64FromString(strParam,' ');
//	if (!uUnitID)
//		uUnitID = m_clPlayer.getUnitID();
//	CGameSceneCell* pCell = m_clPlayer.getSceneCell();
//	if (!pCell)
//		return false;
//	CSceneUnit* pUnit= pCell->findObject(uUnitID);
//	if (!pUnit)
//		return false;
//	char szTmp[1024] = "";
//	dSprintf(szTmp,sizeof(szTmp),"sceneid=%d,cellid=%d,x=%f,y=%f,dir=%f",pCell->getSceneID(),pCell->getCellID(), pUnit->getPosition().x,pUnit->getPosition().y,pUnit->getDirection());
//	outMsg.append(szTmp);
//	return true;
//}
//
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::clearmonster				(std::string&strParam,std::string&outMsg)
//{
//	CGameSceneCell* pScene = m_clPlayer.getSceneCell();
//	if (!pScene )
//		return false;
//
//	pScene->destroyMonster();
//	pScene->destroyAllDartcar();
//	pScene->forbidBirth(true);
//	return true;
//}
//
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::offlinedata					(std::string&strParam,std::string&outMsg)
//{
//	uint16 uSysType		= (uint16)readInt64FromString(strParam,' ');
//	uint32 uSignType	= (uint32)readInt64FromString(strParam,' ');
//
//	m_clPlayer.offlineData(m_clPlayer.getLogicWorldID(), m_clPlayer.getPlayerID(), uSysType, uSignType, NULL);
//
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::questaccept					(std::string&strParam,std::string&outMsg)
//{
//	uint32 uQuestID		= (uint32)readInt64FromString(strParam,' ');
//	bool bForce		= ( 0 == (uint32)readInt64FromString(strParam,' ') );
//
//	CPlayerQuestMgr* pQuestMgr = m_clPlayer.getPlayerQuestMgr();
//	if (!pQuestMgr)
//		return false;
//
//	if (bForce)
//	{
//		if ( !pQuestMgr->acceptQuest(g_clQuestData.findQuestModel(uQuestID)) )
//			return false;
//	}
//	else
//	{
//		uint8 uCode = 0;
//		if ( !pQuestMgr->acceptQuest(uQuestID,uCode) )
//			return false;
//	}
//
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::questgiveup					(std::string&strParam,std::string&outMsg)
//{
//	uint32 uQuestID		= (uint32)readInt64FromString(strParam,' ');
//	CPlayerQuestMgr* pQuestMgr = m_clPlayer.getPlayerQuestMgr();
//	if (!pQuestMgr)
//		return false;
//
//	uint8 uCode = 0;
//	if ( !pQuestMgr->giveupQuest(uQuestID,uCode) )
//		return false;
//
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::questcomplete					(std::string&strParam,std::string&outMsg)
//{
//	uint32 uQuestID		= (uint32)readInt64FromString(strParam,' ');
//
//	CPlayerQuestMgr* pQuestMgr = m_clPlayer.getPlayerQuestMgr();
//	if (!pQuestMgr)
//		return false;
//
//	uint8 uCode = 0;
//	if ( !pQuestMgr->completeQuest(uQuestID,uCode) )
//		return false;
//
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::questupdate					(std::string&strParam,std::string&outMsg)
//{
//	uint32 uQuestID		= (uint32)readInt64FromString(strParam,' ');
//	uint32 uTarIndex	= (uint32)readInt64FromString(strParam,' ');
//	uint32 uCount		= (uint32)readInt64FromString(strParam,' ');
//
//	CPlayerQuestMgr* pQuestMgr = m_clPlayer.getPlayerQuestMgr();
//	if (!pQuestMgr)
//		return false;
//
//	uint8 uCode = 0;
//	if ( !pQuestMgr->updateQuestItem(uQuestID,uTarIndex,uCount,uCode) )
//		return false;
//
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::questfire					(std::string&strParam,std::string&outMsg)
//{
//	uint8 uType		= (uint8)readInt64FromString(strParam,' ');
//	uint32 uSubType	= (uint32)readInt64FromString(strParam,' ');
//	uint32 uCount	= (uint32)readInt64FromString(strParam,' ');
//
//	CPlayerQuestMgr* pQuestMgr = m_clPlayer.getPlayerQuestMgr();
//	if (!pQuestMgr)
//		return false;
//
//	pQuestMgr->fireQuestEvent(uType,uSubType,uCount);
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::questauto					(std::string&strParam,std::string&outMsg)
//{
//	CPlayerQuestMgr* pQuestMgr = m_clPlayer.getPlayerQuestMgr();
//	if (!pQuestMgr)
//		return false;
//
//	uint32 uQuestID		= (uint32)readInt64FromString(strParam,' ');
//	pQuestMgr->gmAccept(uQuestID);
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::questreset					(std::string&strParam,std::string&outMsg)
//{
//	CPlayerQuestMgr* pQuestMgr = m_clPlayer.getPlayerQuestMgr();
//	if (!pQuestMgr)
//		return false;
//
//	CPlayerColletion *pColletion = m_clPlayer.getPlayerColletion();
//	if (!pColletion)
//		return false;
//	
//	pColletion->gmClear();
//	
//	return pQuestMgr->gmReset();
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::reloaddata					(std::string&strParam,std::string&outMsg)
//{
//	uint8 uCode = 0;
//	return m_clPlayer.sendMsgToWorld(P_G2W_Protocol_Common, G2W_Common_GmReloadData, NULL, uCode);
//}
//
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::copymapcreate					(std::string&strParam,std::string&outMsg)
//{
//	CPlayerCopymap* pPlayerCopymap = dynamic_cast<CPlayerCopymap*>(m_clPlayer.getPlayerFun(GameSystemType_Copymap));
//	CHECKF(NULL != pPlayerCopymap);
//
//	uint16 uCopymapID		= (uint16)readInt64FromString(strParam,' ');
//	uint32 uParam		= (uint32)readInt64FromString(strParam,' ');
//	uint8 uCode = 0;
//	pPlayerCopymap->copymap_create(uCopymapID,uParam,uCode);
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::copymapenter					(std::string&strParam,std::string&outMsg)
//{
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::copymapexit					(std::string&strParam,std::string&outMsg)
//{
//	CPlayerCopymap* pPlayerCopymap = dynamic_cast<CPlayerCopymap*>(m_clPlayer.getPlayerFun(GameSystemType_Copymap));
//	CHECKF(NULL != pPlayerCopymap);
//
//	uint16 uCopymapID		= (uint16)readInt64FromString(strParam,' ');
//	uint8 uCode = 0;
//	pPlayerCopymap->copymap_exit(uCopymapID,uCode);
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::copymapinvite		(std::string&strParam,std::string&outMsg)
//{
//	CPlayerCopymap* pPlayerCopymap = dynamic_cast<CPlayerCopymap*>(m_clPlayer.getPlayerFun(GameSystemType_Copymap));
//	CHECKF(NULL != pPlayerCopymap);
//
//	uint16 uCopymapID		= (uint16)readInt64FromString(strParam,' ');
//	uint32 uTargetID		= (uint32)readInt64FromString(strParam,' ');
//	uint8 uCode = 0;
//	pPlayerCopymap->copymap_invite(uCopymapID,uTargetID,uCode);
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::copymapacceptinvite(std::string&strParam,std::string&outMsg)
//{
//	CPlayerCopymap* pPlayerCopymap = dynamic_cast<CPlayerCopymap*>(m_clPlayer.getPlayerFun(GameSystemType_Copymap));
//	CHECKF(NULL != pPlayerCopymap);
//
//	uint16 uCopymapID		= (uint16)readInt64FromString(strParam,' ');
//	uint32 uTargetID		= (uint32)readInt64FromString(strParam,' ');
//	bool	uOk				= 0 != (uint32)readInt64FromString(strParam,' ');
//	uint8 uCode = 0;
//	pPlayerCopymap->copymap_acceptinvite(uCopymapID,uTargetID,uOk,uCode);
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::copymapteammemberinfo(std::string&strParam,std::string&outMsg)
//{
//	CPlayerCopymap* pPlayerCopymap = dynamic_cast<CPlayerCopymap*>(m_clPlayer.getPlayerFun(GameSystemType_Copymap));
//	CHECKF(NULL != pPlayerCopymap);
//
//	uint16 uCopymapID		= (uint16)readInt64FromString(strParam,' ');
//	uint8 uCode = 0;
//	pPlayerCopymap->copymap_teammemberinfo(uCopymapID,uCode);
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::copymapclear		(std::string&strParam,std::string&outMsg)
//{
//	CPlayerCopymap* pPlayerCopymap = dynamic_cast<CPlayerCopymap*>(m_clPlayer.getPlayerFun(GameSystemType_Copymap));
//	CHECKF(NULL != pPlayerCopymap);
//
//	uint16 uCopymapID		= (uint16)readInt64FromString(strParam,' ');
//	pPlayerCopymap->copymap_clear(uCopymapID);
//	return true;
//}
//
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::copymapcomplete		(std::string&strParam,std::string&outMsg)
//{
//	CPlayerCopymap* pPlayerCopymap = dynamic_cast<CPlayerCopymap*>(m_clPlayer.getPlayerFun(GameSystemType_Copymap));
//	CHECKF(NULL != pPlayerCopymap);
//	uint16 uCopymapID		= (uint16)readInt64FromString(strParam, ' ');
//	uint8 uType 			= (uint8)readInt64FromString(strParam, ' ');
//	if (!uType)
//		uType = CopymapCompleteType_Success;
//	uint32 uExtraNum 			= (uint16)readInt64FromString(strParam, ' ');
//	const _stCopymapModel *pCopymapModel = g_clCopymapData.find_model(uCopymapID);
//	CHECKF(NULL != pCopymapModel);
//	pPlayerCopymap->on_copymap_complete(pCopymapModel,(_emCopymapCompleteType)uType,CopymapEnterReason_Normal,uExtraNum);
//	return true;
//}
//
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::sendmail(std::string&strParam,std::string&outMsg)
//{
//	PBMail stMail;
//	stMail.set_worldid(m_clPlayer.getLogicWorldID());
//	stMail.set_playerid(m_clPlayer.getPlayerID());
//	stMail.set_type(MailType_GM);
//
//	std::string stmp;
//	getSubString(strParam, stmp, ' ');
//	stMail.set_title(stmp);
//	getSubString(strParam, stmp, ' ');
//	stMail.set_text(stmp);
//	getSubString(strParam, stmp, ' ');
//	stMail.set_sender(stmp);
//
//	uint32 uSliver		= (uint32)readInt64FromString(strParam, ' ');
//	uint32 uItemID		= (uint32)readInt64FromString(strParam, ' ');
//	uint32 uItemCount	= (uint32)readInt64FromString(strParam, ' ');
//
//	if ( uSliver )
//	{
//		PBMoney* money = stMail.add_money();
//		if (money)
//		{
//			money->set_moneytype(MoneyType_Sliver);
//			money->set_money(uSliver);
//		}
//	}
//
//	if ( uItemID && uItemCount )
//	{
//		if (g_clItemData.find(uItemID))
//		{
//			PBItem* itm = stMail.add_item();
//			if (itm)
//			{
//				itm->set_itemid(uItemID);
//				itm->set_itemnum(uItemCount);
//			}
//		}
//	}
//	return CPlayerMail::sendMail(stMail);
//}
//
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::gmmail(std::string&strParam,std::string&outMsg)
//{
//	uint32 itemID		= (uint32)readInt64FromString(strParam, ' ');
//	uint32 itemNum		= (uint32)readInt64FromString(strParam, ' ');
//	uint32 mailNum		= (uint32)readInt64FromString(strParam, ' ');
//	// 发邮件
//	for(uint32 i = 0; i < mailNum; ++i)
//	{
//		PBMail stMail;
//		stMail.set_playerid(m_clPlayer.getPlayerID());
//		stMail.set_senderid(m_clPlayer.getPlayerID());
//		stMail.set_sender(m_clPlayer.getPlayerBase().szPlayerName);
//		stMail.set_worldid(m_clPlayer.getLogicWorldID()); 
//		stMail.set_type(MailType_GM); 
//		stMail.set_sendtime((uint64)getTime()); 
//
//		PBItem* item = stMail.add_item();
//		if ( item )
//		{
//			item->set_itemid(itemID);
//			item->set_itemnum(itemNum);
//		}
//
//		CPlayerMail* pMailFun = dynamic_cast<CPlayerMail*>(m_clPlayer.getPlayerFun(GameSystemType_Mail));
//		if (NULL != pMailFun)
//		{
//			pMailFun->sendMail(stMail);
//		}
//	}
//	return true;
//}
//
//
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::tradeapply		(std::string&strParam,std::string&outMsg)
//{
//	CPlayerTrade* pPlayerTrade = dynamic_cast<CPlayerTrade*>(m_clPlayer.getPlayerFun(GameSystemType_Trade));
//	CHECKF(NULL != pPlayerTrade);
//
//	uint8 uCode = 0;
//	uint32 uTargetID = (uint32)readInt64FromString(strParam,' ');
//	return pPlayerTrade->trade_apply(uTargetID,uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::tradeanswer		(std::string&strParam,std::string&outMsg)
//{
//	CPlayerTrade* pPlayerTrade = dynamic_cast<CPlayerTrade*>(m_clPlayer.getPlayerFun(GameSystemType_Trade));
//	CHECKF(NULL != pPlayerTrade);
//
//	uint8 uCode = 0;
//	uint32 uApplyerID		= (uint32)readInt64FromString(strParam,' ');
//	bool bOk = (readInt64FromString(strParam,' ')!=0);
//	return pPlayerTrade->trade_answer(uApplyerID,bOk,uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::tradeadditem		(std::string&strParam,std::string&outMsg)
//{
//	CPlayerTrade* pPlayerTrade = dynamic_cast<CPlayerTrade*>(m_clPlayer.getPlayerFun(GameSystemType_Trade));
//	CHECKF(NULL != pPlayerTrade);
//
//	uint8 uCode = 0;
//	uint64 uItemSN		= (uint64)readInt64FromString(strParam,' ');
//	uint32 uCount		= (uint32)readInt64FromString(strParam,' ');
//	return pPlayerTrade->trade_addItem(uItemSN,uCount,uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::tradedelitem		(std::string&strParam,std::string&outMsg)
//{
//	CPlayerTrade* pPlayerTrade = dynamic_cast<CPlayerTrade*>(m_clPlayer.getPlayerFun(GameSystemType_Trade));
//	CHECKF(NULL != pPlayerTrade);
//
//	uint8 uCode = 0;
//	uint64 uItemSN		= (uint64)readInt64FromString(strParam,' ');
//	return pPlayerTrade->trade_delItem(uItemSN,uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::tradesetmoney		(std::string&strParam,std::string&outMsg)
//{
//	CPlayerTrade* pPlayerTrade = dynamic_cast<CPlayerTrade*>(m_clPlayer.getPlayerFun(GameSystemType_Trade));
//	CHECKF(NULL != pPlayerTrade);
//
//	uint8 uCode = 0;
//	uint8 uMoneyType		= (uint8)readInt64FromString(strParam,' ');
//	uint64 uMoney			= (uint64)readInt64FromString(strParam,' ');
//	return pPlayerTrade->trade_setMoney(uMoneyType,uMoney,uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::tradelock		(std::string&strParam,std::string&outMsg)
//{
//	CPlayerTrade* pPlayerTrade = dynamic_cast<CPlayerTrade*>(m_clPlayer.getPlayerFun(GameSystemType_Trade));
//	CHECKF(NULL != pPlayerTrade);
//
//	uint8 uCode = 0;
//	return pPlayerTrade->trade_lock(uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::tradeconfirm		(std::string&strParam,std::string&outMsg)
//{
//	CPlayerTrade* pPlayerTrade = dynamic_cast<CPlayerTrade*>(m_clPlayer.getPlayerFun(GameSystemType_Trade));
//	CHECKF(NULL != pPlayerTrade);
//
//	uint8 uCode = 0;
//	return pPlayerTrade->trade_confirm(uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::tradecancel		(std::string&strParam,std::string&outMsg)
//{
//	CPlayerTrade* pPlayerTrade = dynamic_cast<CPlayerTrade*>(m_clPlayer.getPlayerFun(GameSystemType_Trade));
//	CHECKF(NULL != pPlayerTrade);
//
//	uint8 uCode = 0;
//	return pPlayerTrade->trade_cancel(uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::salemoneysale		(std::string&strParam,std::string&outMsg)
//{
//	CPlayerSaleMoney* pPlayerSaleMoney = dynamic_cast<CPlayerSaleMoney*>(m_clPlayer.getPlayerFun(GameSystemType_SaleMoney));
//	CHECKF(NULL != pPlayerSaleMoney);
//
//	uint8 uCode = 0;
//	uint32 uType		= (uint32)readInt64FromString(strParam,' ');
//	uint32 uCash		= (uint32)readInt64FromString(strParam,' ');
//	uint32 uSliver		= (uint32)readInt64FromString(strParam,' ');
//	return pPlayerSaleMoney->salemoney_sale(uType,uCash,uSliver,uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::salemoneysalecancel		(std::string&strParam,std::string&outMsg)
//{
//	CPlayerSaleMoney* pPlayerSaleMoney = dynamic_cast<CPlayerSaleMoney*>(m_clPlayer.getPlayerFun(GameSystemType_SaleMoney));
//	CHECKF(NULL != pPlayerSaleMoney);
//
//	uint8 uCode = 0;
//	uint64 uOrderID		= (uint64)readInt64FromString(strParam,' ');
//	return pPlayerSaleMoney->salemoney_salecancel(uOrderID,uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::salemoneylist		(std::string&strParam,std::string&outMsg)
//{
//	CPlayerSaleMoney* pPlayerSaleMoney = dynamic_cast<CPlayerSaleMoney*>(m_clPlayer.getPlayerFun(GameSystemType_SaleMoney));
//	CHECKF(NULL != pPlayerSaleMoney);
//
//	uint8 uCode = 0;
//	return pPlayerSaleMoney->salemoney_list(uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::auctionsale		(std::string&strParam,std::string&outMsg)
//{
//	CPlayerAuction* pPlayerAuction = dynamic_cast<CPlayerAuction*>(m_clPlayer.getPlayerFun(GameSystemType_Auction));
//	CHECKF(NULL != pPlayerAuction);
//
//	uint8 uCode = 0;
//	uint64 uItemSN		= (uint64)readInt64FromString(strParam,' ');
//	uint32 uSaleCount	= (uint32)readInt64FromString(strParam,' ');
//	uint32 uUnitSliver	= (uint32)readInt64FromString(strParam,' ');
//	return pPlayerAuction->auction_sale(uItemSN,uSaleCount,uUnitSliver,uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::auctionsalecancel		(std::string&strParam,std::string&outMsg)
//{
//	CPlayerAuction* pPlayerAuction = dynamic_cast<CPlayerAuction*>(m_clPlayer.getPlayerFun(GameSystemType_Auction));
//	CHECKF(NULL != pPlayerAuction);
//
//	uint8 uCode = 0;
//	uint64 uSaleOrderID		= (uint64)readInt64FromString(strParam,' ');
//	return pPlayerAuction->auction_salecancel(uSaleOrderID,uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::auctionbuy		(std::string&strParam,std::string&outMsg)
//{
//	CPlayerAuction* pPlayerAuction = dynamic_cast<CPlayerAuction*>(m_clPlayer.getPlayerFun(GameSystemType_Auction));
//	CHECKF(NULL != pPlayerAuction);
//
//	uint8 uCode = 0;
//	uint64 uSaleOrderID	= (uint64)readInt64FromString(strParam,' ');
//	uint32 uUnitSliver	= (uint32)readInt64FromString(strParam,' ');
//	uint32 uBuyCount	= (uint32)readInt64FromString(strParam,' ');
//	return pPlayerAuction->auction_buy(uSaleOrderID,uUnitSliver,uBuyCount,uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::auctionlist		(std::string&strParam,std::string&outMsg)
//{
//	CPlayerAuction* pPlayerAuction = dynamic_cast<CPlayerAuction*>(m_clPlayer.getPlayerFun(GameSystemType_Auction));
//	CHECKF(NULL != pPlayerAuction);
//
//	uint8 uCode = 0;
//	PBC2GAuction_list msg;
//	return pPlayerAuction->auction_list(msg,uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::auctionave		(std::string&strParam,std::string&outMsg)
//{
//	CPlayerAuction* pPlayerAuction = dynamic_cast<CPlayerAuction*>(m_clPlayer.getPlayerFun(GameSystemType_Auction));
//	CHECKF(NULL != pPlayerAuction);
//
//	uint8 uCode = 0;
//	uint32 uItemID	= (uint32)readInt64FromString(strParam,' ');
//	return pPlayerAuction->auction_ave(uItemID,uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool			CClientGMCommand::toplistupdate		(std::string&strParam,std::string&outMsg)
//{
//	CPlayerTopList* pPlayerTopList = dynamic_cast<CPlayerTopList*>(m_clPlayer.getPlayerFun(GameSystemType_TopList));
//	CHECKF(NULL != pPlayerTopList);
//
//	uint32 uType		= (uint32)readInt64FromString(strParam,' ');
//	uint32 uValue		= (uint32)readInt64FromString(strParam,' ');
//	uint32 uSubValue	= (uint32)readInt64FromString(strParam,' ');
//
//	return pPlayerTopList->toplist_update(uType,uValue,uSubValue);
//}
//
////------------------------------------------------------------------------------
////------------------------------------- 排行测试用
//bool			CClientGMCommand::toplistrand		(std::string&strParam,std::string&outMsg)
//{
//	CPlayerTopList* pPlayerTopList = dynamic_cast<CPlayerTopList*>(m_clPlayer.getPlayerFun(GameSystemType_TopList));
//	CHECKF(NULL != pPlayerTopList);
//
//	uint32 uType		= (uint32)readInt64FromString(strParam,' ');
//	uint32 uValue		= 0;
//	uint32 uSubValue	= 0;
//
//	if (uType == TopListType_Figtht)
//	{
//		uValue = randRange(999999, 15);
//		return m_clPlayer.toplist_update(uType,uValue,uSubValue);
//		return true;
//	}
//	else if (uType == TopListType_Level)
//	{
//		uValue = randRange(500, 15);
//		m_clPlayer.addLevel(uValue, DoingType_GM);
//		return true;
//	}
//	else if (uType == TopListType_Wealth)
//	{
//		uValue = randRange(99999, 15);
//		m_clPlayer.addExpend(MoneyType_Sliver,uValue, DoingType_GM);
//		return true;
//	}
//	else if (uType ==  TopListType_SinglePvp)
//	{
//		uValue = randRange(99999, 15);
//		//m_clPlayer.getPlayerSinglePvp()->addScore(uValue);
//		m_clPlayer.toplist_update(TopListType_SinglePvp, uValue, 0);
//		return true;
//	}
//	else if (uType == TopListType_ClimbTower)
//	{
//		uValue = randRange(90, 1);
//		uint32 uTime = randRange(90, 4);
//		m_clPlayer.toplist_update(uType,uValue,uTime);
//		return true;
//	}
//	else if  (uType == TopListType_CopymapGW)
//	{
//		uValue = randRange(90, 1);
//		uSubValue = randRange(90, 4);
//		 m_clPlayer.toplist_update(uType,uValue,uSubValue);
//		return true;
//	}
//	else if (uType == TopListType_Faction)
//	{
//		uValue = randRange(90, 1);
//		uSubValue = randRange(90, 4);
//		m_clPlayer.toplist_update(uType,uValue,uSubValue);
//	}
//	return true;
//}
//
////-------------------------------------------------------------
////------------------------------ 
//bool			CClientGMCommand::toplist_resettime		(std::string&strParam,std::string&outMsg)
//{
//	CPlayerTopList* pPlayerTopList = dynamic_cast<CPlayerTopList*>(m_clPlayer.getPlayerFun(GameSystemType_TopList));
//	CHECKF(NULL != pPlayerTopList);
//	uint32 uType		= (uint32)readInt64FromString(strParam,' ');
//
//	uint32 AddTime	= getTime() ;
//	if (uType == TopListTimeResetType_Day)
//	{
//		AddTime -= AddTime + 3600 * 24;
//	}
//	else if  (uType == TopListTimeResetType_Week)
//	{
//		AddTime -= AddTime + 3600 * 24 * 7 ;
//	}
//	else if  (uType ==TopListTimeResetType_Month)
//	{ 
//		AddTime -= AddTime + 3600 * 24 * 7 ;
//	}
//
//	pPlayerTopList->gm_TopListReSetTime(AddTime);
//	return true;
//}
//
////------------------------------------------------------------------------------
////------------------------------------- 
//bool			CClientGMCommand::toplistlist		(std::string&strParam,std::string&outMsg)
//{
//	CPlayerTopList* pPlayerTopList = dynamic_cast<CPlayerTopList*>(m_clPlayer.getPlayerFun(GameSystemType_TopList));
//	CHECKF(NULL != pPlayerTopList);
//
//	uint32 uType		= (uint32)readInt64FromString(strParam,' ');
//	uint32 uBeginOrder	= (uint32)readInt64FromString(strParam,' ');
//	uint32 uCount		= (uint32)readInt64FromString(strParam,' ');
//
//	//转发到全局服
//	PBC2GTopListList ask;
//	PBTopListSortType*pSortType = ask.mutable_sorttype();
//	if (pSortType)
//	{
//		pSortType->set_type(uType);
//	}
//
//	ask.set_beginorder(uBeginOrder);
//	ask.set_count(uCount);
//
//
//	uint8 uCode;
//	return pPlayerTopList->toplist_list(ask,uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool			CClientGMCommand::toplistplayer		(std::string&strParam,std::string&outMsg)
//{
//	CPlayerTopList* pPlayerTopList = dynamic_cast<CPlayerTopList*>(m_clPlayer.getPlayerFun(GameSystemType_TopList));
//	CHECKF(NULL != pPlayerTopList);
//
//	uint32 uTargetID		= (uint32)readInt64FromString(strParam,' ');
//	if (!uTargetID)
//		uTargetID = m_clPlayer.getPlayerID();
//
//	PBC2GTopListOne ask;
//	ask.set_targetid(uTargetID);
//
//	uint8 uCode;
//	return pPlayerTopList->toplist_player(ask,uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool			CClientGMCommand::randpos				(std::string&strParam,std::string&outMsg)
//{
//	bool bAllPlayer = ( 0 != readInt64FromString(strParam,' '));
//	if ( !bAllPlayer )
//	{
//		return m_clPlayer.randPos(0);
//	}
//
//	CGamePlayerMgr::iterator _pos = g_pGamePlayerMgr->begin();
//	CGamePlayerMgr::iterator _end = g_pGamePlayerMgr->end();
//	for ( ; _pos != _end ; ++_pos )
//	{
//		CScenePlayer* pPlayer = _pos->second;
//		if ( !pPlayer )
//			continue;
//
//		pPlayer->randPos(0);
//	}
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool			CClientGMCommand::call				(std::string&strParam,std::string&outMsg)
//{
//	//转发到全局服
//	if (!m_clPlayer.isGlobalConneted())
//		return false;
//
//	PBGLAGCallPlayer msg;
//	msg.set_tarname(strParam);
//	msg.set_sceneid(m_clPlayer.getSceneID());
//	msg.set_scenecellid(m_clPlayer.getSceneCellID());
//	msg.set_scenedynid(m_clPlayer.getSceneDynID());
//	msg.set_posx(m_clPlayer.getPosition().x);
//	msg.set_posy(m_clPlayer.getPosition().y);
//
//	m_clPlayer.sendMsgToGlobal(P_G2GL_Protocol_Common,GLAG_Common_CallPlayer,&msg);
//
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool			CClientGMCommand::find				(std::string&strParam,std::string&outMsg)
//{
//	//转发到全局服
//	if (!m_clPlayer.isGlobalConneted())
//		return false;
//
//	PBGLAGLocatePlayer msg;
//	msg.set_playerid(m_clPlayer.getPlayerID());
//	msg.set_reason(LocatePlayerReason_GMQuery);
//	msg.set_tarname(strParam);
//
//	m_clPlayer.sendMsgToGlobal(P_G2GL_Protocol_Common,GLAG_Common_LocatePlayer,&msg);
//
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//void	CClientGMCommand::on_find_result		(PBGLAGLocatePlayerAck& ack)
//{
//	PBG2CGMCmdTxtCmd msg;
//	msg.set_result(true);
//	if ( !ack.suc() )
//	{
//		msg.set_msg("player not found!");
//	}
//	else
//	{
//		const char* szSceneName = "";
//		_stSceneMapModel* pModel = g_clSceneMapData.find((uint16)ack.sceneid());
//		if (pModel)
//		{
//			szSceneName = pModel->strName.c_str();
//		}
//		char szBuff[256];
//		dSprintf(szBuff,sizeof(szBuff),"%s(%d)_%f_%f\n",szSceneName,ack.scenecellid(),ack.posx(),ack.posy());
//
//		std::string utf8Str;
//		_convertANSItoUTF8(szBuff, utf8Str);
//		msg.set_msg(utf8Str);
//	}
//	m_clPlayer.sendMsg(P_S2C_Protocol_Common,S2C_Common_GMCmd, &msg);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool			CClientGMCommand::go				(std::string&strParam,std::string&outMsg)
//{
//	//转发到全局服
//	if (!m_clPlayer.isGlobalConneted())
//		return false;
//
//	PBGLAGLocatePlayer msg;
//	msg.set_playerid(m_clPlayer.getPlayerID());
//	msg.set_reason(LocatePlayerReason_GMQueryGOTO);
//	msg.set_tarname(strParam);
//
//	m_clPlayer.sendMsgToGlobal(P_G2GL_Protocol_Common,GLAG_Common_LocatePlayer,&msg);
//
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//void	CClientGMCommand::on_go_result		(PBGLAGLocatePlayerAck& ack)
//{
//	if ( !ack.suc() )
//	{
//		PBG2CGMCmdTxtCmd msg;
//		msg.set_result(true);
//		msg.set_msg("player not found!");
//		m_clPlayer.sendMsg(P_S2C_Protocol_Common,S2C_Common_GMCmd, &msg);
//	}
//	else
//	{
//		m_clPlayer.playerFlyScene_ByDynID(ack.sceneid(),ack.scenedynid(),ack.posx(),ack.posy());
//	}
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool			CClientGMCommand::pkupdate				(std::string&strParam,std::string&outMsg)
//{
//	int32 nValue		= (uint32)readInt64FromString(strParam,' ');
//	if (!nValue)
//		return true;
//
//	bool bAdd = (nValue>0);
//	uint32 uValue = (uint32)abs(nValue);
//	m_clPlayer.updatePKValue(uValue,bAdd);
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::friendadd			(std::string&strParam,std::string&outMsg)
//{
//	CPlayerFriend* pPlayerFriend = dynamic_cast<CPlayerFriend*>(m_clPlayer.getPlayerFun(GameSystemType_Friend));
//	CHECKF(NULL != pPlayerFriend);
//
//	uint32 uType		= (uint32)readInt64FromString(strParam,' ');
//	uint32 uTargetID	= (uint32)readInt64FromString(strParam,' ');
//
//	uint8 uCode;
//	pPlayerFriend->friend_add_ask(uType,uTargetID,uCode);
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::frienddel			(std::string&strParam,std::string&outMsg)
//{
//	CPlayerFriend* pPlayerFriend = dynamic_cast<CPlayerFriend*>(m_clPlayer.getPlayerFun(GameSystemType_Friend));
//	CHECKF(NULL != pPlayerFriend);
//
//	uint32 uType		= (uint32)readInt64FromString(strParam,' ');
//	uint32 uTargetID	= (uint32)readInt64FromString(strParam,' ');
//
//	uint8 uCode;
//	pPlayerFriend->friend_del_ask(uType,uTargetID,uCode);
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::friendanswer		(std::string&strParam,std::string&outMsg)
//{
//	CPlayerFriend* pPlayerFriend = dynamic_cast<CPlayerFriend*>(m_clPlayer.getPlayerFun(GameSystemType_Friend));
//	CHECKF(NULL != pPlayerFriend);
//
//	uint32 uType		= (uint32)readInt64FromString(strParam,' ');
//	uint32 uApplyerID	= (uint32)readInt64FromString(strParam,' ');
//
//	uint8 uCode;
//	pPlayerFriend->friend_answer(uType,uApplyerID,uCode);
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::friendvalue		(std::string&strParam,std::string&outMsg)
//{
//	CPlayerFriend* pPlayerFriend = dynamic_cast<CPlayerFriend*>(m_clPlayer.getPlayerFun(GameSystemType_Friend));
//	CHECKF(NULL != pPlayerFriend);
//
//	uint32 uType		= (uint32)readInt64FromString(strParam,' ');
//	uint32 uTargetID	= (uint32)readInt64FromString(strParam,' ');
//	int32 nValue		= (uint32)readInt64FromString(strParam,' ');
//
//	bool bAdd = (nValue >= 0);
//	uint32 uValue = abs(nValue);
//	pPlayerFriend->update_friend_value( uType , uTargetID ,uValue,bAdd );
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::zisha		(std::string&strParam,std::string&outMsg)
//{
//	int32 nHp = m_clPlayer.getHP();
//	m_clPlayer.updateHP(-1* nHp,&m_clPlayer, DoingType_GM);
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::fuhuo		(std::string&strParam,std::string&outMsg)
//{
//	m_clPlayer.updateHP(m_clPlayer.getHPMax(),&m_clPlayer, DoingType_GM);
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::splitbingfu		(std::string&strParam,std::string&outMsg)
//{
//	uint64 uBingfuSn		= (uint32)readInt64FromString(strParam,' ');
//	uint16 uNum	= (uint16)readInt64FromString(strParam,' ');
//	MAP_U64_U16 mapExpend;
//	mapExpend.insert_(uBingfuSn,uNum);
//	uint8 uCode = 0;
//	return m_clPlayer.getGameItemMgr().bingfuSplitAsk(mapExpend,uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::upgradebingfu	(std::string&strParam,std::string&outMsg)
//{
//	uint64 uBingfuSn		= (uint32)readInt64FromString(strParam,' ');
//	uint64 uExpendBingfuSn		= (uint32)readInt64FromString(strParam,' ');
//	uint16 uNum	= (uint16)readInt64FromString(strParam,' ');
//	MAP_U64_U16 mapExpend;
//	mapExpend.insert_(uExpendBingfuSn,uNum);
//	uint8 uCode = 0;
//	return m_clPlayer.getGameItemMgr().bingfuUpgradeAsk(uBingfuSn,mapExpend,uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::upgradewulue	(std::string&strParam,std::string&outMsg)
//{
//	uint8 uWulueType		= (uint8)readInt64FromString(strParam,' ');
//	uint16 uYaodianID		= (uint16)readInt64FromString(strParam,' ');
//	uint8 uCode = 0;
//	return m_clPlayer.getPlayerWulue()->upgradeZhanlueAsk(uYaodianID,uWulueType,uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::setbingfu	(std::string&strParam,std::string&outMsg)
//{
//	uint8 uWulueType	= (uint8)readInt64FromString(strParam,' ');
//	uint16 uYaodianID	= (uint16)readInt64FromString(strParam,' ');
//	uint32 uBingfuSn	= (uint32)readInt64FromString(strParam,' ');
//	uint8 uCode = 0;
//	return m_clPlayer.getPlayerWulue()->setBingfuAsk(uYaodianID,uWulueType,uBingfuSn,uCode);
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::wentaosetscheme	(std::string&strParam,std::string&outMsg)
//{
//	CPlayerWenTao* pWenTao = dynamic_cast<CPlayerWenTao*>(m_clPlayer.getPlayerFun(GameSystemType_WenTao));
//	CHECKF(NULL != pWenTao);
//
//	uint32 uScheme		= (uint32)readInt64FromString(strParam,' ');
//	if ( uScheme <= 0 )
//		return false;
//
//	uint16 uIndex		= (uint16)readInt64FromString(strParam,' ');
//	uint16 uBingFaID	= (uint16)readInt64FromString(strParam,' ');
//	uint8 uCode;
//	pWenTao->setscheme_ask(uIndex,uBingFaID,uCode);
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::wentaoqianxiu	(std::string&strParam,std::string&outMsg)
//{
//	CPlayerWenTao* pWenTao = dynamic_cast<CPlayerWenTao*>(m_clPlayer.getPlayerFun(GameSystemType_WenTao));
//	CHECKF(NULL != pWenTao);
//
//	uint16 uBingFaID	= (uint16)readInt64FromString(strParam,' ');
//
//	uint8 uCode;
//	pWenTao->qianxiu_ask(uBingFaID,uCode);
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::wentaotupo	(std::string&strParam,std::string&outMsg)
//{
//	CPlayerWenTao* pWenTao = dynamic_cast<CPlayerWenTao*>(m_clPlayer.getPlayerFun(GameSystemType_WenTao));
//	CHECKF(NULL != pWenTao);
//
//	uint16 uBingFaID	= (uint16)readInt64FromString(strParam,' ');
//
//	uint8 uCode;
//	pWenTao->tupo_ask(uBingFaID,uCode);
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::wentaoupgrate	(std::string&strParam,std::string&outMsg)
//{
//	CPlayerWenTao* pWenTao = dynamic_cast<CPlayerWenTao*>(m_clPlayer.getPlayerFun(GameSystemType_WenTao));
//	CHECKF(NULL != pWenTao);
//
//	uint16 uBingFaID	= (uint16)readInt64FromString(strParam,' ');
//	uint16 uYaoDianID	= (uint16)readInt64FromString(strParam,' ');
//
//	uint8 uCode;
//	pWenTao->upgrate_ask(uBingFaID,uYaoDianID,uCode);
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::wentaosetbingfu	(std::string&strParam,std::string&outMsg)
//{
//// 	CPlayerWenTao* pWenTao = dynamic_cast<CPlayerWenTao*>(m_clPlayer.getPlayerFun(GameSystemType_WenTao));
//// 	CHECKF(NULL != pWenTao);
//// 
//// 	uint16 uBingFaID	= (uint16)readInt64FromString(strParam,' ');
//// 	uint16 uYaoDianID	= (uint16)readInt64FromString(strParam,' ');
//// 	uint32 uBingFuSN	= (uint32)readInt64FromString(strParam,' ');
//// 
//// 	uint8 uCode;
//// 	//pWenTao->setbingfu_ask(uBingFaID,uYaoDianID,uBingFuSN,uCode);
// 	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::wentaoreset	(std::string&strParam,std::string&outMsg)
//{
//	CPlayerWenTao* pWenTao = dynamic_cast<CPlayerWenTao*>(m_clPlayer.getPlayerFun(GameSystemType_WenTao));
//	CHECKF(NULL != pWenTao);
//
//	uint16 uBingFaID	= (uint16)readInt64FromString(strParam,' ');
//
//	uint8 uCode;
//	pWenTao->reset_ask(uBingFaID,uCode);
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::wentaoactivescheme	(std::string&strParam,std::string&outMsg)
//{
//	CPlayerWenTao* pWenTao = dynamic_cast<CPlayerWenTao*>(m_clPlayer.getPlayerFun(GameSystemType_WenTao));
//	CHECKF(NULL != pWenTao);
//
//	uint16 uSchemeID	= (uint16)readInt64FromString(strParam,' ');
//
//	uint8 uCode;
//	pWenTao->activescheme_ask(uSchemeID,uCode);
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::wentaoactivebingfa	(std::string&strParam,std::string&outMsg)
//{
//	CPlayerWenTao* pWenTao = dynamic_cast<CPlayerWenTao*>(m_clPlayer.getPlayerFun(GameSystemType_WenTao));
//	CHECKF(NULL != pWenTao);
//
//	uint16 uBingFaID	= (uint16)readInt64FromString(strParam,' ');
//
//	uint8 uCode;
//	pWenTao->activebingfa_ask(uBingFaID,uCode);
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::teaminvite			(std::string&strParam,std::string&outmsg)
//{
//	CPlayerTeam* pPlayerTeam = dynamic_cast<CPlayerTeam*>(m_clPlayer.getPlayerFun(GameSystemType_Team));
//	CHECKF(NULL != pPlayerTeam);
//
//	uint32 uTargetID	= (uint32)readInt64FromString(strParam,' ');
//
//	PBC2GTeamInvite ask;
//	ask.set_playerid(uTargetID);
//	CNetStream stStream;
//	stStream.write(m_clPlayer.getPlayerID());
//	stStream.SerializeToStream(ask);
//	m_clPlayer.sendBufferToWorld( P_G2W_Protocol_Team, C2S_Team_Invite, stStream.getBuffer(), stStream.getPosition());
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::teamacceptinvite			(std::string&strParam,std::string&outmsg)
//{
//	CPlayerTeam* pPlayerTeam = dynamic_cast<CPlayerTeam*>(m_clPlayer.getPlayerFun(GameSystemType_Team));
//	CHECKF(NULL != pPlayerTeam);
//
//	uint32 uTargetID	= (uint32)readInt64FromString(strParam,' ');
//	bool bAccept		=  0 != (uint32)readInt64FromString(strParam,' ');
//
//	PBC2GTeamAcceptInvite ask;
//	ask.set_playerid(uTargetID);
//	ask.set_accept(bAccept);
//	CNetStream stStream;
//	stStream.write(m_clPlayer.getPlayerID());
//	stStream.SerializeToStream(ask);
//	m_clPlayer.sendBufferToWorld(P_G2W_Protocol_Team, C2S_Team_AcceptInvite, stStream.getBuffer(), stStream.getPosition());
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::teamapply			(std::string&strParam,std::string&outmsg)
//{
//	CPlayerTeam* pPlayerTeam = dynamic_cast<CPlayerTeam*>(m_clPlayer.getPlayerFun(GameSystemType_Team));
//	CHECKF(NULL != pPlayerTeam);
//
//	uint32 uTargetID	= (uint32)readInt64FromString(strParam,' ');
//
//	PBC2GTeamApply ask;
//	ask.set_playerid(uTargetID);
//	CNetStream stStream;
//	stStream.write(m_clPlayer.getPlayerID());
//	stStream.SerializeToStream(ask);
//	m_clPlayer.sendBufferToWorld(P_G2W_Protocol_Team, C2S_Team_Apply, stStream.getBuffer(), stStream.getPosition());
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::teamacceptapply			(std::string&strParam,std::string&outmsg)
//{
//	CPlayerTeam* pPlayerTeam = dynamic_cast<CPlayerTeam*>(m_clPlayer.getPlayerFun(GameSystemType_Team));
//	CHECKF(NULL != pPlayerTeam);
//
//	uint32 uTargetID	= (uint32)readInt64FromString(strParam,' ');
//	bool bAccept		=  0 != (uint32)readInt64FromString(strParam,' ');
//
//	PBC2GTeamAcceptApply ask;
//	ask.set_playerid(uTargetID);
//	ask.set_accept(bAccept);
//	CNetStream stStream;
//	stStream.write(m_clPlayer.getPlayerID());
//	stStream.SerializeToStream(ask);
//	m_clPlayer.sendBufferToWorld( P_G2W_Protocol_Team, C2S_Team_AcceptApply, stStream.getBuffer(), stStream.getPosition());
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::teamleave			(std::string&strParam,std::string&outmsg)
//{
//	CPlayerTeam* pPlayerTeam = dynamic_cast<CPlayerTeam*>(m_clPlayer.getPlayerFun(GameSystemType_Team));
//	CHECKF(NULL != pPlayerTeam);
//
//	PBC2GTeamLeave ask;
//	CNetStream stStream;
//	stStream.write(m_clPlayer.getPlayerID());
//	stStream.SerializeToStream(ask);
//	m_clPlayer.sendBufferToWorld(P_G2W_Protocol_Team, C2S_Team_Leave, stStream.getBuffer(), stStream.getPosition());
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::teamchangeleader			(std::string&strParam,std::string&outmsg)
//{
//	CPlayerTeam* pPlayerTeam = dynamic_cast<CPlayerTeam*>(m_clPlayer.getPlayerFun(GameSystemType_Team));
//	CHECKF(NULL != pPlayerTeam);
//
//	uint32 uTargetID	= (uint32)readInt64FromString(strParam,' ');
//
//
//	PBC2GTeamChangeLeader ask;
//	ask.set_playerid(uTargetID);
//	CNetStream stStream;
//	stStream.write(m_clPlayer.getPlayerID());
//	stStream.SerializeToStream(ask);
//	m_clPlayer.sendBufferToWorld(P_G2W_Protocol_Team, C2S_Team_ChangeLeader, stStream.getBuffer(), stStream.getPosition());
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::teampublishask			(std::string&strParam,std::string&outmsg)
//{
//	CPlayerTeam* pPlayerTeam = dynamic_cast<CPlayerTeam*>(m_clPlayer.getPlayerFun(GameSystemType_Team));
//	CHECKF(NULL != pPlayerTeam);
//
//	uint16 uCopymapID	= (uint16)readInt64FromString(strParam,' ');
//
//
//	PBG2WTeamPublishAsk ask;
//	PBC2GTeamPublishAsk* pClietMsg = ask.mutable_clientmsg();
//	if (pClietMsg)
//	{
//		pClietMsg->set_copymapid(uCopymapID);
//	}
//	CNetStream stStream;
//	stStream.write(m_clPlayer.getPlayerID());
//	stStream.SerializeToStream(ask);
//	m_clPlayer.sendBufferToWorld(P_G2W_Protocol_Team, C2S_Team_Publish_Ask, stStream.getBuffer(), stStream.getPosition());
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::teampublishcancel			(std::string&strParam,std::string&outmsg)
//{
//	CPlayerTeam* pPlayerTeam = dynamic_cast<CPlayerTeam*>(m_clPlayer.getPlayerFun(GameSystemType_Team));
//	CHECKF(NULL != pPlayerTeam);
//
//	uint16 uCopymapID	= (uint16)readInt64FromString(strParam,' ');
//
//
//	PBC2GTeamPublishCancel ask;
//	ask.set_copymapid(uCopymapID);
//	CNetStream stStream;
//	stStream.write(m_clPlayer.getPlayerID());
//	stStream.SerializeToStream(ask);
//	m_clPlayer.sendBufferToWorld(P_G2W_Protocol_Team, C2S_Team_Publish_Cancel, stStream.getBuffer(), stStream.getPosition());
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::teampublishinvite			(std::string&strParam,std::string&outmsg)
//{
//	CPlayerTeam* pPlayerTeam = dynamic_cast<CPlayerTeam*>(m_clPlayer.getPlayerFun(GameSystemType_Team));
//	CHECKF(NULL != pPlayerTeam);
//
//	uint16 uCopymapID	= (uint16)readInt64FromString(strParam,' ');
//	uint32 uTargetID	= (uint32)readInt64FromString(strParam,' ');
//
//	PBC2GTeamPublishInvite ask;
//	ask.set_copymapid(uCopymapID);
//	ask.set_playerid(uTargetID);
//	CNetStream stStream;
//	stStream.write(m_clPlayer.getPlayerID());
//	stStream.SerializeToStream(ask);
//	m_clPlayer.sendBufferToWorld(P_G2W_Protocol_Team, C2S_Team_Publish_Invite, stStream.getBuffer(), stStream.getPosition());
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::teampublishacceptinvite			(std::string&strParam,std::string&outmsg)
//{
//	CPlayerTeam* pPlayerTeam = dynamic_cast<CPlayerTeam*>(m_clPlayer.getPlayerFun(GameSystemType_Team));
//	CHECKF(NULL != pPlayerTeam);
//
//	uint16 uCopymapID	= (uint16)readInt64FromString(strParam,' ');
//	uint32 uTargetID	= (uint32)readInt64FromString(strParam,' ');
//	bool bAccept		=  0 != (uint32)readInt64FromString(strParam,' ');
//
//	PBC2GTeamPublishAcceptInvite ask;
//	ask.set_copymapid(uCopymapID);
//	ask.set_playerid(uTargetID);
//	ask.set_accept(bAccept);
//	CNetStream stStream;
//	stStream.write(m_clPlayer.getPlayerID());
//	stStream.SerializeToStream(ask);
//	m_clPlayer.sendBufferToWorld(P_G2W_Protocol_Team, C2S_Team_Publish_AcceptInvite, stStream.getBuffer(), stStream.getPosition());
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::teampublishlist			(std::string&strParam,std::string&outmsg)
//{
//	CPlayerTeam* pPlayerTeam = dynamic_cast<CPlayerTeam*>(m_clPlayer.getPlayerFun(GameSystemType_Team));
//	CHECKF(NULL != pPlayerTeam);
//
//	uint16 uCopymapID	= (uint16)readInt64FromString(strParam,' ');
//	uint32 uBeginIdx	= (uint32)readInt64FromString(strParam,' ');
//	uint32 uEndIdx		= (uint32)readInt64FromString(strParam,' ');
//
//	PBC2GTeamPublishList ask;
//	ask.set_copymapid(uCopymapID);
//	ask.set_beginidx(uBeginIdx);
//	ask.set_endidx(uEndIdx);
//	CNetStream stStream;
//	stStream.write(m_clPlayer.getPlayerID());
//	stStream.SerializeToStream(ask);
//	m_clPlayer.sendBufferToWorld( P_G2W_Protocol_Team, C2S_Team_Publish_List, stStream.getBuffer(), stStream.getPosition());
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::teamkick			(std::string&strParam,std::string&outmsg)
//{
//	CPlayerTeam* pPlayerTeam = dynamic_cast<CPlayerTeam*>(m_clPlayer.getPlayerFun(GameSystemType_Team));
//	CHECKF(NULL != pPlayerTeam);
//
//	uint32 uTargetID	= (uint32)readInt64FromString(strParam,' ');
//
//	PBC2GTeamKick ask;
//	ask.set_playerid(uTargetID);
//	CNetStream stStream;
//	stStream.write(m_clPlayer.getPlayerID());
//	stStream.SerializeToStream(ask);
//	m_clPlayer.sendBufferToWorld( P_G2W_Protocol_Team, C2S_Team_Kick, stStream.getBuffer(), stStream.getPosition());
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::teamkickvote			(std::string&strParam,std::string&outmsg)
//{
//	CPlayerTeam* pPlayerTeam = dynamic_cast<CPlayerTeam*>(m_clPlayer.getPlayerFun(GameSystemType_Team));
//	CHECKF(NULL != pPlayerTeam);
//
//	uint32 uTargetID	= (uint32)readInt64FromString(strParam,' ');
//	bool bAgree			=  0 != (uint32)readInt64FromString(strParam,' ');
//
//
//	PBC2GTeamKickVote ask;
//	ask.set_playerid(uTargetID);
//	ask.set_agree(bAgree);
//	CNetStream stStream;
//	stStream.write(m_clPlayer.getPlayerID());
//	stStream.SerializeToStream(ask);
//	m_clPlayer.sendBufferToWorld(P_G2W_Protocol_Team, C2S_Team_Kick_Vote, stStream.getBuffer(), stStream.getPosition());
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::teamsetallot			(std::string&strParam,std::string&outmsg)
//{
//	CPlayerTeam* pPlayerTeam = dynamic_cast<CPlayerTeam*>(m_clPlayer.getPlayerFun(GameSystemType_Team));
//	CHECKF(NULL != pPlayerTeam);
//
//	uint32 uAllotType	= (uint32)readInt64FromString(strParam,' ');
//
//
//	PBTeamSetAllot ask;
//	ask.set_type(uAllotType);
//	CNetStream stStream;
//	stStream.write(m_clPlayer.getPlayerID());
//	stStream.SerializeToStream(ask);
//	m_clPlayer.sendBufferToWorld(P_G2W_Protocol_Team, C2S_Team_SetAllot, stStream.getBuffer(), stStream.getPosition());
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::teamsetallotvote			(std::string&strParam,std::string&outmsg)
//{
//	CPlayerTeam* pPlayerTeam = dynamic_cast<CPlayerTeam*>(m_clPlayer.getPlayerFun(GameSystemType_Team));
//	CHECKF(NULL != pPlayerTeam);
//
//	uint32 uAllotType	= (uint32)readInt64FromString(strParam,' ');
//	bool bAgree			=  0 != (uint32)readInt64FromString(strParam,' ');
//
//	PBTeamSetAllotVote ask;
//	ask.set_type(uAllotType);
//	ask.set_agree(bAgree);
//	CNetStream stStream;
//	stStream.write(m_clPlayer.getPlayerID());
//	stStream.SerializeToStream(ask);
//	m_clPlayer.sendBufferToWorld(P_G2W_Protocol_Team, C2S_Team_SetAllot_Vote, stStream.getBuffer(), stStream.getPosition());
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::teamnearplayerlist			(std::string&strParam,std::string&outmsg)
//{
//	CPlayerTeam* pPlayerTeam = dynamic_cast<CPlayerTeam*>(m_clPlayer.getPlayerFun(GameSystemType_Team));
//	CHECKF(NULL != pPlayerTeam);
//
//	uint32 uBeginIdx	= (uint32)readInt64FromString(strParam,' ');
//	uint32 uEndIdx		= (uint32)readInt64FromString(strParam,' ');
//
//	uint8 uCode;
//	pPlayerTeam->team_nearplayerlist(uBeginIdx,uEndIdx,uCode);
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::teamnearteamlist			(std::string&strParam,std::string&outmsg)
//{
//	CPlayerTeam* pPlayerTeam = dynamic_cast<CPlayerTeam*>(m_clPlayer.getPlayerFun(GameSystemType_Team));
//	CHECKF(NULL != pPlayerTeam);
//
//	uint32 uBeginIdx	= (uint32)readInt64FromString(strParam,' ');
//	uint32 uEndIdx		= (uint32)readInt64FromString(strParam,' ');
//
//	uint8 uCode;
//	pPlayerTeam->team_nearteamlist(uBeginIdx,uEndIdx,uCode);
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::dropitem				(std::string&strParam,std::string&outMsg)
//{
//	CGameSceneCell* pCell	= m_clPlayer.getSceneCell();
//	if (!pCell)
//	{
//		outMsg = "scene is null!";
//		return false;
//	}
//
//	uint16 uDropID			=	(uint16)readInt64FromString(strParam,' ');
//	CSceneItem* pSceneItem	=	pCell->dropItem(0, uDropID, &m_clPlayer, m_clPlayer.getPosition());
//	uint32 uDropUnitID		=	0;
//	if ( pSceneItem )
//		uDropUnitID			= pSceneItem->getUnitID();
//
//	char szBuff[256];
//	dSprintf(szBuff,sizeof(szBuff),"%u",uDropUnitID);
//	outMsg = szBuff;
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::dropiteminfo				(std::string&strParam,std::string&outMsg)
//{
//	CPlayerDropItem* pPlayerDropItem = dynamic_cast<CPlayerDropItem*>(m_clPlayer.getPlayerFun(GameSystemType_DropItem));
//	CHECKF(NULL != pPlayerDropItem);
//
//	uint32 uDropUnitID			= (uint32)readInt64FromString(strParam,' ');
//
//	uint8 uCode;
//	pPlayerDropItem->dropitem_info_ask(uDropUnitID,uCode);
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::dropitemdice				(std::string&strParam,std::string&outMsg)
//{
//	CPlayerDropItem* pPlayerDropItem = dynamic_cast<CPlayerDropItem*>(m_clPlayer.getPlayerFun(GameSystemType_DropItem));
//	CHECKF(NULL != pPlayerDropItem);
//
//	uint32 uDropUnitID			= (uint32)readInt64FromString(strParam,' ');
//	uint32 uItemIdx				= (uint32)readInt64FromString(strParam,' ');
//	bool	bAbandon			= 0 != (uint32)readInt64FromString(strParam,' ');
//
//	uint8 uCode;
//	pPlayerDropItem->dropitem_dice_ask(uDropUnitID,uItemIdx,bAbandon,uCode);
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::dropitemassign				(std::string&strParam,std::string&outMsg)
//{
//	CPlayerDropItem* pPlayerDropItem = dynamic_cast<CPlayerDropItem*>(m_clPlayer.getPlayerFun(GameSystemType_DropItem));
//	CHECKF(NULL != pPlayerDropItem);
//
//	uint32 uDropUnitID			= (uint32)readInt64FromString(strParam,' ');
//	uint32 uItemIdx				= (uint32)readInt64FromString(strParam,' ');
//	uint32 uTargetID				= (uint32)readInt64FromString(strParam,' ');
//
//	uint8 uCode;
//	pPlayerDropItem->dropitem_assign_ask(uDropUnitID,uItemIdx,uTargetID,uCode);
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::dropitempickup				(std::string&strParam,std::string&outMsg)
//{
//	CPlayerDropItem* pPlayerDropItem = dynamic_cast<CPlayerDropItem*>(m_clPlayer.getPlayerFun(GameSystemType_DropItem));
//	CHECKF(NULL != pPlayerDropItem);
//
//	uint32 uDropUnitID			= (uint32)readInt64FromString(strParam,' ');
//	uint32 uItemIdx				= (uint32)readInt64FromString(strParam,' ');
//
//	uint8 uCode;
//	pPlayerDropItem->dropitem_pickup_ask(uDropUnitID,uItemIdx,uCode);
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::equipenhance		(std::string&strParam,std::string&outMsg)
//{
//	uint8 uEquipType			= (uint8)readInt64FromString(strParam,' ');
//
//	stl_map<uint64, _stExpendItem> mapExpend;
//	while (!strParam.empty())
//	{
//
//		uint64 uItemSN	= (uint64)readInt64FromString(strParam,' ');
//		if (!uItemSN)
//			break;
//		uint32 uItemNum	= (uint16)readInt64FromString(strParam,' ');
//		_stExpendItem &stItem = mapExpend[uItemSN];
//		stItem.uItemSN = uItemSN;
//		stItem.uItemNum = uItemNum;
//	}
//
//	uint8 uCode = 0;
//
//	//return m_clPlayer.getGameItemMgr().equipEnhance(uEquipType,vecExpendItem,uCode);
//	CPlayerEquipColumn* pPlayerEquipColumn = dynamic_cast<CPlayerEquipColumn*>(m_clPlayer.getPlayerFun(GameSystemType_EqiupColumn));
//	CHECKF(NULL != pPlayerEquipColumn);
//	return pPlayerEquipColumn->equipColumnEnhance(uEquipType,mapExpend,uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::equipforge	(std::string&strParam,std::string&outMsg)
//{
//	uint8 uEquipType = (uint8)readInt64FromString(strParam,' ');
//	stl_map<uint64, _stExpendItem> mapExpend;
//	while (!strParam.empty())
//	{
//
//		uint64 uItemSN	= (uint64)readInt64FromString(strParam,' ');
//		if (!uItemSN)
//			break;
//		uint32 uItemNum	= (uint16)readInt64FromString(strParam,' ');
//		_stExpendItem &stItem = mapExpend[uItemSN];
//		stItem.uItemSN = uItemSN;
//		stItem.uItemNum = uItemNum;
//	}
//	uint8 uCode = 0;
//	//return m_clPlayer.getGameItemMgr().equipForge(uEquipType,vecExpendItem,uCode);
//	CPlayerEquipColumn* pPlayerEquipColumn = dynamic_cast<CPlayerEquipColumn*>(m_clPlayer.getPlayerFun(GameSystemType_EqiupColumn));
//	CHECKF(NULL != pPlayerEquipColumn);
//	return pPlayerEquipColumn->equipColumnForge(uEquipType,mapExpend,uCode);
//}
//
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::equipinherit				(std::string&strParam,std::string&outMsg)
//{
//	uint64 uItemSN			= (uint64)readInt64FromString(strParam,' ');
//	uint64 uDelItemSN		= (uint64)readInt64FromString(strParam,' ');
//
//	uint8 uCode = 0;
//	return m_clPlayer.getGameItemMgr().equipInherit(uItemSN,uDelItemSN,uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::equipmake				(std::string&strParam,std::string&outMsg)
//{
//	uint16 uMakeID			= (uint16)readInt64FromString(strParam,' ');
//	uint8 uMark				= (uint8)readInt64FromString(strParam,' ');
//
//	uint8 uCode = 0;
//	return m_clPlayer.getGameItemMgr().equipMake(uMakeID,uMark,uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::mount				(std::string&strParam,std::string&outMsg)
//{
//	uint8 uCode = 0;
//	return m_clPlayer.getGameItemMgr().mounting(0 == m_clPlayer.getMountID(),uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::skillcmd				(std::string&strParam,std::string&outMsg)
//{
//	uint32 uSkillID			= (uint32)readInt64FromString(strParam,' ');
//
//	uint8 uCode = 0;
//	_stPoint2F stTargetPoint = m_clPlayer.getPosition();
//	m_clPlayer.getGameSkillMgr().addSkillUseCmd(uSkillID,m_clPlayer.getDirection(),0,stTargetPoint,stTargetPoint,uCode);
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::broadcast			(std::string&strParam,std::string&outMsg)
//{
//	uint8 uChannel			= (uint8)readInt64FromString(strParam,' ');
//	uint64 ID				= (uint64)readInt64FromString(strParam,' ');
//	PBC2DropItem_Info ack;
//	g_pGameBroadcast->broadCastMsg(m_clPlayer.getLogicWorldID(),(_emBroadcast_Channel)uChannel,ID,P_S2C_Protocol_DropItem,S2C_DropItem_Info,&ack);
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//typedef	void (* http_callback)(HttpRequest*pRequest,HttpResponse*pResponse);
//class CHttpNetwork :	public HttpClient
//{
//	struct MyHttpRequest : public HttpRequest
//	{
//		http_callback callback;
//
//		MyHttpRequest()
//		{
//			callback = NULL;
//		}
//	};
//protected:
//	virtual void	processHttpResponse	(HttpResponse*pResponse)
//	{
//		MyHttpRequest*pOriginRequest = (MyHttpRequest*)pResponse->getHttpRequest();
//		if ( !pOriginRequest )
//			return ;
//
//		CLog::print("http request url：");
//		CLog::print(pOriginRequest->getUrl());
//		CLog::print("http response result：");
//		CLog::print("%u",pResponse->getResponseCode());
//		CLog::print("http response data：");
//		std::vector<char>* pData = pResponse->getResponseData();
//		if (pData)
//		{
//			if ( pData->size() > 1024 )
//			{
//				(*pData)[1024] =  '\0';
//			}
//			else
//			{
//				pData->push_back('\0');
//			}
//			CLog::print("%s",&(*pData->begin()));
//		}
//		if ( pOriginRequest->callback )
//		{
//			(*pOriginRequest->callback)(pOriginRequest,pResponse);
//		}
//
//		safe_delete(pOriginRequest);
//	}
//public:
//	bool			sendHttpRequest		(const std::string& url, HttpRequest::_emRequestType type,const std::string& data,const std::vector<std::string>& header,const std::string& cookie,http_callback callback = NULL)
//	{
//		MyHttpRequest* request =  new MyHttpRequest;
//		request->setUrl(url.c_str());
//		request->setRequestType(type);
//		request->setRequestData(data.c_str(), data.length());
//		request->setHeaders(header);
//		request->setCookieData(cookie.c_str(), cookie.length());
//		request->callback	= callback;
//		if ( !postHttpRequest(request) )
//		{
//			safe_delete(request);
//			return false;
//		}
//		return true;
//	}
//};
//static CHttpNetwork s_clInstane;
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::httpstart			(std::string&strParam,std::string&outMsg)
//{
//	s_clInstane.start();
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::httpstop			(std::string&strParam,std::string&outMsg)
//{
//	s_clInstane.shutdown();
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::httprequest			(std::string&strParam,std::string&outMsg)
//{
//	std::string url;// = "https://api.gotye.com.cn/api/accessToken";
//	std::string header;
//	std::string data;// = "{ \"grant_type\": \"client_credentials\", \"client_id\": \"8b642770-ea44-4048-8a4e-b99925a7162f\", \"client_secret\": \"e71507f243724d3e8bbb5c80dcae8ca1\"}";
//	std::string cookie;
//
//	getSubString(strParam,url);
//	getSubString(strParam,header);
//	getSubString(strParam,data);
//	getSubString(strParam,cookie);
//
//	std::vector<std::string> headers;
//	if (!header.empty())
//		headers.push_back(header);
//
//	s_clInstane.sendHttpRequest(url,HttpRequest::POST,data,headers,cookie);
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::httpresponse			(std::string&strParam,std::string&outMsg)
//{
//	s_clInstane.process();
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::bosslist			(std::string&strParam,std::string&outMsg)
//{
//	PBG2GLCommon_BossListAsk msg;
//	msg.set_playerid(m_clPlayer.getPlayerID());
//	m_clPlayer.sendMsgToGlobal(P_G2GL_Protocol_Common, GLAG_Common_BossListAsk,&msg);
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::identity_open			(std::string&strParam,std::string&outMsg)
//{
//	CPlayerIdentity* pPlayerIdentity = dynamic_cast<CPlayerIdentity*>(m_clPlayer.getPlayerFun(GameSystemType_Identity));
//	CHECKF(NULL != pPlayerIdentity);
//
//	uint16 uSetID			= (uint16)readInt64FromString(strParam,' ');
//	uint8 uCode = 0;
//	return pPlayerIdentity->openAsk(uSetID,uCode);
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::identity_learn			(std::string&strParam,std::string&outMsg)
//{
//	CPlayerIdentity* pPlayerIdentity = dynamic_cast<CPlayerIdentity*>(m_clPlayer.getPlayerFun(GameSystemType_Identity));
//	CHECKF(NULL != pPlayerIdentity);
//
//	uint16 uSetID			= (uint16)readInt64FromString(strParam,' ');
//	uint16 uIdentityID		= (uint16)readInt64FromString(strParam,' ');
//
//	uint8 uCode = 0;
//	return pPlayerIdentity->learnAsk(uSetID,uIdentityID,uCode);
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::identity_reset			(std::string&strParam,std::string&outMsg)
//{
//	CPlayerIdentity* pPlayerIdentity = dynamic_cast<CPlayerIdentity*>(m_clPlayer.getPlayerFun(GameSystemType_Identity));
//	CHECKF(NULL != pPlayerIdentity);
//
//	uint16 uSetID			= (uint16)readInt64FromString(strParam,' ');
//	uint8 uCode = 0;
//	return pPlayerIdentity->resetAsk(uSetID,uCode);
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::identity_upskill			(std::string&strParam,std::string&outMsg)
//{
//	CPlayerIdentity* pPlayerIdentity = dynamic_cast<CPlayerIdentity*>(m_clPlayer.getPlayerFun(GameSystemType_Identity));
//	CHECKF(NULL != pPlayerIdentity);
//
//	uint16 uSetID			= (uint16)readInt64FromString(strParam,' ');
//	uint32 uSkillID			= (uint16)readInt64FromString(strParam,' ');
//	uint8 uCode = 0;
//	return pPlayerIdentity->upSkillAsk(uSetID,uSkillID,uCode);
//}
//
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::addliveblock				(std::string&strParam,std::string&outMsg)
//{
//	uint32 uMonsterID		= (uint32)readInt64FromString(strParam,' ');
//	uint32 uCount			= (uint32)readInt64FromString(strParam,' ');
//	float32 fRadius			= (float32)readfloat64FromString(strParam,' ');
//	float32 fBlockSize		= (float32)readfloat64FromString(strParam,' ');
//
//	CGameSceneCell* pCell = m_clPlayer.getSceneCell();
//	if (!pCell)
//		return false;
//
//	uCount = MAX((uint32)1, uCount);
//	if ( uCount > 1000 )
//		uCount = 1000;
//
//	if ( fRadius < 1.0f )
//		fRadius = 1.0f;
//
//	const _stMonsterModel*pModel = g_clMonsterData.find(uMonsterID);
//	if (!pModel)
//	{
//		outMsg = "not found monster model!";
//		return false;
//	}
//	if (fBlockSize > 5)
//	{
//		fBlockSize = 5;
//	}
//	if ( fBlockSize < 1.0f )
//	{
//		fBlockSize = 1.0f;
//	}
//
//	_stPoint2F stOut;
//	for ( uint32 i = 0 ; i < uCount ;++i )
//	{
//		if (pCell->randGetPoint(stOut,m_clPlayer.getPosition().x,m_clPlayer.getPosition().y,fRadius))
//		{
//
//			pCell->createMonster(pModel,stOut.x,stOut.y,getRandDir());
//			//新增阻挡点，并且在阻挡点放怪物。
//			_stSceneArea _stSA ;
//			_stSA.initSceneArea();
//			_stSA.fHeight = 20;
//			_stSA.isDynamic = true;
//			static uint8 s_uAreaID = 0;
//			_stSA.uAreaID = ++s_uAreaID;
//			_stSA.uType = SceneAreaType_Door; 
//			_stSA.vecVerts.push_back(stOut.x - fBlockSize);_stSA.vecVerts.push_back(0);_stSA.vecVerts.push_back(stOut.y + fBlockSize);
//			_stSA.vecVerts.push_back(stOut.x + fBlockSize);_stSA.vecVerts.push_back(0);_stSA.vecVerts.push_back(stOut.y + fBlockSize);
//			_stSA.vecVerts.push_back(stOut.x + fBlockSize);_stSA.vecVerts.push_back(0);_stSA.vecVerts.push_back(stOut.y - fBlockSize);
//			_stSA.vecVerts.push_back(stOut.x - fBlockSize);_stSA.vecVerts.push_back(0);_stSA.vecVerts.push_back(stOut.y - fBlockSize);
//			pCell->getPathFinder().addArea(&_stSA,true);
//		}
//	}
//	return true;
//}
//
////------------------------------------------------------------------------------
////------------------------------------- 
//bool	CClientGMCommand::callfollower				(std::string&strParam,std::string&outMsg)
//{
//	uint32 uMonsterID		= (uint32)readInt64FromString(strParam,' ');
//	//uint32 uSecond			= (uint32)readInt64FromString(strParam,' ');
//	float32 fRadius			= (float32)readfloat64FromString(strParam,' ');
//        
//	CGameSceneCell* pCell = m_clPlayer.getSceneCell();
//	if (!pCell)
//		return false;
//	MAP_GAME_UNIT _map;
//	_map.clear();
//	_stPoint2F _st = m_clPlayer.getPosition();
//	pCell->gatherObject(_st, fRadius, _BIT32(GameObject_Monster), _map);
//	MAP_GAME_UNIT::iterator iter = _map.begin();
//	MAP_GAME_UNIT::iterator iter_end = _map.end();
//
//	for (;iter != iter_end; ++iter)
//	{
//		CSceneUnit* pUnit = iter->second;
//		if (pUnit!= NULL 
//			&& pUnit->getAI() != NULL
//			&& pUnit->isDead()==false 
//			&& pUnit->asMonster() 
//			&& pUnit->asMonster()->getMonsterModel()->uMonsterID == uMonsterID)
//		{
//			//设置怪物变成跟随者。
//			//pUnit->getAI()->changeState2Follow(m_clPlayer.getUnitID(), uSecond);
//		}
//	} 
//	return true;
//}
//
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::clearskillcooltime			(std::string&strParam,std::string&outMsg)
//{
//	uint32 uMark		= (uint32)readInt64FromString(strParam,' ');
//	if (uMark > 0)
//	{
//		m_clPlayer.delete_mark(SceneUnitMark_IgnoreSkillCD);
//	}
//	else
//	{
//		m_clPlayer.update_mark(SceneUnitMark_IgnoreSkillCD);
//
//	}
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::clearskillexpend			(std::string&strParam,std::string&outMsg)
//{
//	uint32 uMark		= (uint32)readInt64FromString(strParam,' ');
//	if (uMark > 0)
//	{
//		m_clPlayer.delete_mark(SceneUnitMark_IgnoreSkillExpend);
//	}
//	else
//	{
//		m_clPlayer.update_mark(SceneUnitMark_IgnoreSkillExpend);
//	}
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::supply			(std::string&strParam,std::string&outMsg)
//{
//	m_clPlayer.sendBufferToGlobal(P_G2GL_Protocol_SaleMoney, G2GL_SaleMoney_supply, NULL, 0);
//	return true;
//}
//
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::supplyextra		(std::string&strParam,std::string&outMsg)
//{
//	m_clPlayer.sendBufferToGlobal(P_G2GL_Protocol_SaleMoney, G2GL_SaleMoney_supplyextra, NULL, 0);
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::save		(std::string&strParam,std::string&outMsg)
//{
//	return true;
//}
//
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::worldstarttime		(std::string&strParam,std::string&outMsg)
//{	
//	uint64 uTime = readDateTimeFromString(strParam);
//	if (uTime <= 0)
//		return false;
//	uint8 uCode = 0;
//	PBG2WGMWorldStartTime stInfo;
//	stInfo.set_stamp(uTime);
//	stInfo.set_logicworldid(g_pServer->getLogicWorldID());
//	return m_clPlayer.sendMsgToWorld(P_G2W_Protocol_Common, G2W_Common_GmWorldStartTime, &stInfo, uCode);
//}
//
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::itemlike(std::string &strParam, std::string &outMsg)
//{
//	stl_map<uint32, std::string> itemFound;	
//	std::stringstream sstr;
//	if ( g_clItemData.searchIDByName(strParam, itemFound))
//		sstr << "Result[limit 15]:" << "\r\n";
//	else
//	{
//		sstr << "Only used Debug V"  << "\r\n"; 
//		return true;
//	}
//
//	stl_map<uint32, std::string>::iterator _pos = itemFound.begin();
//	stl_map<uint32, std::string>::iterator end  = itemFound.end();
//	uint32 uCount = 0;
//	for (; _pos != end; ++_pos)
//	{
//		if (uCount == 15)
//		{
//			break;
//		}
//		sstr << _pos->first << "\r:" << _pos->second << "\r\n"; 
//		uCount++;
//	}
//	
//	outMsg = sstr.str();
//	return true;
//}
//
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::buy(std::string &strParam, std::string &outMsg)
//{
//	uint32 uType		= (uint32)readInt64FromString(strParam,' ');
//	uint32 uIndex		= (uint32)readInt64FromString(strParam,' ');
//	uint32 uCount		= (uint32)readInt64FromString(strParam,' ');
//	uint32 friendID		= (uint32)readInt64FromString(strParam,' ');
//	CPlayerStore *pPlayerStore = m_clPlayer.getPlayerStore();
//
//
//	if(pPlayerStore)
//	{
//		pPlayerStore->buyItem(uType, uIndex, uCount, friendID);
//		//pPlayerStore->buyItem(uType, uIndex, uCount);
//	}
//	return true;
//}
//
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::debugmsg(std::string &strParam, std::string &outMsg)
//{
//	int32 iCode	= (int32)readInt64FromString(strParam,' ');
//	int8 iSwitch		= (int8)readInt64FromString(strParam,' ');
//	if (iCode < 0)
//	{
//		for (uint8 i = _emPC2S_Protocol_MIN; i <= _emPC2S_Protocol_MAX; ++i)
//		{
//			g_pServer->m_stDebugMsgCfg.setSwitch(i, iSwitch > 0);
//		}
//	}
//	else if (iCode < _emPC2S_Protocol_ARRAYSIZE)
//	{
//		g_pServer->m_stDebugMsgCfg.setSwitch(iCode, iSwitch > 0);
//	}
//	return true;
//}
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::wuxuenew(std::string &strParam, std::string &outMsg)
//{
//	CPlayerWuxue* pPlayerWuxue = m_clPlayer.getPlayerWuxue();
//	if (!pPlayerWuxue)
//		return false;
//	uint8 uCode = 0;
//	return pPlayerWuxue->newSchemeAsk(uCode);
//}
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::wuxuedel(std::string &strParam, std::string &outMsg)
//{
//	uint8 uSchemeNum	= (uint8)readInt64FromString(strParam,' ');
//	CPlayerWuxue* pPlayerWuxue = m_clPlayer.getPlayerWuxue();
//	if (!pPlayerWuxue)
//		return false;
//	uint8 uCode = 0;
//	return pPlayerWuxue->delSchemeAsk(uSchemeNum,uCode);
//}
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::wuxuechange(std::string &strParam, std::string &outMsg)
//{
//	CPlayerWuxue* pPlayerWuxue = m_clPlayer.getPlayerWuxue();
//	if (!pPlayerWuxue)
//		return false;
//	uint8 uCode = 0;
//	return pPlayerWuxue->changeSchemeAsk(uCode);
//}
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::wuxuelearnskill(std::string &strParam, std::string &outMsg)
//{
//	uint8 uSchemeNum	= (uint8)readInt64FromString(strParam,' ');
//	uint8 uPage			= (uint8)readInt64FromString(strParam,' ');
//	uint16 uOldIndex	= (uint16)readInt64FromString(strParam,' ');
//	uint32 uSkillID		= (uint32)readInt64FromString(strParam,' ');
//
//
//	CPlayerWuxue* pPlayerWuxue = m_clPlayer.getPlayerWuxue();
//	if (!pPlayerWuxue)
//		return false;
//	uint8 uCode = 0;
//	return pPlayerWuxue->learnSkillAsk(uSchemeNum,uPage,uOldIndex,uSkillID,uCode);
//}
//
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::sysopen(std::string &strParam, std::string &outMsg)
//{
//	uint32 uID		= (uint32)readInt64FromString(strParam,' ');
//	if (uID > _emGameSystemType_MAX)
//		return false;
//
//	
//	CPlayerSystemSwitch *pPlayerSys = m_clPlayer.getPlayerSystemSwitch();
//	if (pPlayerSys)
//	{
//		if (uID == 0) //打开全部
//		{
//			DMAP_SYSTEM_SWITCH_MODEL *pModel = g_clSystemSwitchData.getSystemSwitchMoel();
//			if (!pModel)
//				return false;
//			DMAP_SYSTEM_SWITCH_MODEL::iterator _pos = pModel->begin();
//			DMAP_SYSTEM_SWITCH_MODEL::iterator end  = pModel->end();
//			for (; _pos != end; ++_pos)
//			{
//				DMAP_SYSTEM_SWITCH_MODEL::_sub_map& subMap = _pos->second;
//				DMAP_SYSTEM_SWITCH_MODEL::sub_iterator _sub_pos = subMap.begin();
//				for (; _sub_pos != subMap.end(); ++_sub_pos)
//				{
//					_stSystemSwitchModel& stModel = _sub_pos->second;
//					if (!pPlayerSys->checkSwitchStateOpen(stModel.uID))
//					{
//						pPlayerSys->openSystemSwitch(stModel.uID);
//						pPlayerSys->send_SystemSwitchOpen_Ack(stModel.uID);
//					}	
//				}
//			}	
//		}
//		else 
//		{
//			if (!pPlayerSys->checkSwitchStateOpen(uID))
//			{
//				pPlayerSys->openSystemSwitch(uID);
//				pPlayerSys->send_SystemSwitchOpen_Ack(uID);
//			}
//
//		}
//		
//	}
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool			CClientGMCommand::randscene				(std::string&strParam,std::string&outMsg)
//{
//	bool bAllPlayer = ( 0 != readInt64FromString(strParam,' '));
//	if ( !bAllPlayer )
//	{
//		return m_clPlayer.randScene();
//	}
//
//	CGamePlayerMgr::iterator _pos = g_pGamePlayerMgr->begin();
//	CGamePlayerMgr::iterator _end = g_pGamePlayerMgr->end();
//	for ( ; _pos != _end ; ++_pos )
//	{
//		CScenePlayer* pPlayer = _pos->second;
//		if ( !pPlayer )
//			continue;
//
//		pPlayer->randScene();
//	}
//	return true;
//}
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::callup(std::string &strParam, std::string &outMsg)
//{
//	CPlayerTeam* pPlayerTeam = (CPlayerTeam*)m_clPlayer.getPlayerFun(GameSystemType_Team);
//	if (!pPlayerTeam)
//		return false;
//
//	uint32 uID	= (uint32)readInt64FromString(strParam,' ');
//	uint8 uCode = 0;
//	return pPlayerTeam->callup(uID,uCode);
//}
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::acceptidentity(std::string &strParam, std::string &outMsg)
//{
//	uint32 uID	= (uint32)readInt64FromString(strParam,' ');
//	uint32 uNpcUnitID	= (uint32)readInt64FromString(strParam,' ');
//	CPlayerTeam* pPlayerTeam = (CPlayerTeam*)m_clPlayer.getPlayerFun(GameSystemType_Team);
//	if (!pPlayerTeam)
//		return false;
//	uint8 uCode = 0;
//	return pPlayerTeam->accept_identity(uID,uNpcUnitID,uCode);
//}
//
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::hide(std::string &strParam, std::string &outMsg)
//{
//	uint32 hide	= (uint32)readInt64FromString(strParam,' ');
//	if(hide > 0)
//	{
//		m_clPlayer.addState(StateType_Hide);
//	}
//	else
//	{		
//		m_clPlayer.delState(StateType_Hide);
//	}
//	return true;
//
//}
//
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::factionbosspersonaltop(std::string &strParam, std::string &outMsg)
//{
//	uint32 uBeginOrder		= (uint32)readInt64FromString(strParam,' ');
//	uint32 uCount		= (uint32)readInt64FromString(strParam,' ');
//
//	CPlayerFactionBoss* pPlayerFactionBoss = (CPlayerFactionBoss*)m_clPlayer.getPlayerFun(GameSystemType_FactionBoss);
//	if (!pPlayerFactionBoss)
//		return false;
//
//	uint8 uCode;
//	pPlayerFactionBoss->FactionBoss_PersonalTop_Ask(uBeginOrder,uCount,uCode);
//	return true;
//}
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::factionbossfactiontop(std::string &strParam, std::string &outMsg)
//{
//	uint32 uBeginOrder		= (uint32)readInt64FromString(strParam,' ');
//	uint32 uCount		= (uint32)readInt64FromString(strParam,' ');
//
//	CPlayerFactionBoss* pPlayerFactionBoss = (CPlayerFactionBoss*)m_clPlayer.getPlayerFun(GameSystemType_FactionBoss);
//	if (!pPlayerFactionBoss)
//		return false;
//
//	uint8 uCode;
//	pPlayerFactionBoss->FactionBoss_FactionTop_Ask(uBeginOrder,uCount,uCode);
//	return true;
//}
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::factionbossuniontop(std::string &strParam, std::string &outMsg)
//{
//	uint32 uBeginOrder		= (uint32)readInt64FromString(strParam,' ');
//	uint32 uCount		= (uint32)readInt64FromString(strParam,' ');
//
//	CPlayerFactionBoss* pPlayerFactionBoss = (CPlayerFactionBoss*)m_clPlayer.getPlayerFun(GameSystemType_FactionBoss);
//	if (!pPlayerFactionBoss)
//		return false;
//
//	uint8 uCode;
//	pPlayerFactionBoss->FactionBoss_UnionTop_Ask(uBeginOrder,uCount,uCode);
//	return true;
//}
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::qinggong(std::string &strParam, std::string &outMsg)
//{
//	uint32 uQinggongID		= (uint32)readInt64FromString(strParam,' ');
//	m_clPlayer.setQinggongID(uQinggongID);
//	m_clPlayer.setQinggongStartTick(getSystemTick64());
//	m_clPlayer.addState(StateType_Qinggong);
//	m_clPlayer.getPlayerCommon()->broadcastQinggong(uQinggongID);
//	return true;
//}
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::battleattrdetail(std::string &strParam, std::string &outMsg)
//{
//	uint32 uType		= (uint32)readInt64FromString(strParam,' ');
//	PBS2CGetBattleAttrDetailAck ack;
//	uint8 uCode = 0;
//	m_clPlayer.getPlayerCommon()->onGetBattleAttrDetail(uType, ack, uCode);
//	return true;
//}
////-------------------------------------------------------------
////------------------------------YYYY-MM-DD HH:MM:SS  设置宿主机的时间
//bool CClientGMCommand::setdtime(std::string &strParam, std::string &outMsg)
//{
//#ifdef WIN32
//#include <Windows.h>
//	SYSTEMTIME sysTime = {};
//
//	sysTime.wYear	= (int32)readInt64FromString(strParam,'-');
//	//月
//	sysTime.wMonth	=  (int32)readInt64FromString(strParam,'-');
//
//	//日
//	sysTime.wDay	=  (int32)readInt64FromString(strParam,' ');
//
//	//时
//	sysTime.wHour	=  (int32)readInt64FromString(strParam,':');
//
//	//分
//	sysTime.wMinute	=  (int32)readInt64FromString(strParam,':');
//
//	//秒
//	sysTime.wSecond	=  (int32)readInt64FromString(strParam);
//
//	SetLocalTime(&sysTime);
//#endif // WIN32
//	time_format(getTime(), outMsg);
//	return true;
//}
////-------------------------------------------------------------
////------------------------------YYYY-MM-DD HH:MM:SS  设置宿主机的时间
//bool CClientGMCommand::getdtime(std::string &strParam, std::string &outMsg)
//{
//	if (time_format(getTime(), outMsg))
//	{
//	}
//	return true;
//}
////-------------------------------------------------------------
////------------------------------从网络获取时间  设置宿主机的时间
//#ifdef WIN32
//#include "http/curl_src/include/curl/curl.h"
//#endif // WIN32
//size_t writeToString(void *ptr, size_t size, size_t count, void *stream)
//{
//	((std::string*)stream)->append((char*)ptr, 0, size* count);
//	return size* count;
//}
//bool CClientGMCommand::resumedtime(std::string &strParam, std::string &outMsg)
//{
//#ifdef WIN32
//	CURL *curl;
//	CURLcode res;
//	std::string s;
//
//	curl = curl_easy_init();
//	if(curl) 
//	{
//		curl_easy_setopt(curl, CURLOPT_URL, "http://www.163.com/");
//		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, writeToString);
//		curl_easy_setopt(curl, CURLOPT_HEADERDATA, &s);
//		res = curl_easy_perform(curl);
//
//		if(CURLE_OK == res) 
//		{
//			//返回的http头部
//			//HTTP/1.0 302 Moved Temporarily
//			//Server: Cdn Cache Server V2.0
//			//Date: Mon, 20 Feb 2017 03:45:26 GMT
//			//	  Content-Length: 0
//			//Location: http://3g.163.com
//
//			std::size_t uBegin = s.find("Date: ");
//			if (uBegin != std::string::npos)
//			{
//				std::size_t uEnd = s.find("\r", uBegin + 6);
//				if (uEnd != std::string::npos)
//				{
//					std::string date = s.substr(uBegin+6, uEnd);
//					time_t t = curl_getdate(date.c_str(), NULL); 
//					if (t > 0)
//					{
//						time_format(t, outMsg);
//						setdtime(outMsg, outMsg);
//					}
//				}
//			}
//		}
//		/* always cleanup */ 
//		curl_easy_cleanup(curl);
//	}
//#endif // WIN32
//	return true;
//}
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::benew(std::string &strParam, std::string &outMsg)
//{
//	m_clPlayer.clearAllExpend();
//	m_clPlayer.getPlayerCommon()->initialize();
//	m_clPlayer.getPlayerQuestMgr()->initialize();
//	m_clPlayer.getGameItemMgr().clearEquip();
//	m_clPlayer.getGameItemMgr().clearBag();
//	m_clPlayer.newOnline();
//	m_clPlayer.resetGhost();
//	m_clPlayer.dailyCleanup(0,0);
//	m_clPlayer.weekCleanup(0,0);
//	m_clPlayer.monthCleanup(0,0);
//	return true;
//}
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::enterplane(std::string &strParam, std::string &outMsg)
//{
//	uint32 uIsEnter		= (uint32)readInt64FromString(strParam,' ');
//	uint8 uCode;
//	if (uIsEnter)
//	{
//		PBC2GCommon_PlaneEnter ask;
//		ask.set_param(1);
//		m_clPlayer.plane_enter(uCode);
//		{
//			m_clPlayer.sendMsg(P_S2C_Protocol_Common,S2C_Common_PlaneEnter_Ack,&ask,uCode);
//		}
//	}
//	else
//	{
//		PBC2GCommon_PlaneLeave ask;
//		ask.set_posx(m_clPlayer.getPosition().x);
//		ask.set_posy(m_clPlayer.getPosition().y);
//		ask.set_dir(m_clPlayer.getDirection());
//		ask.set_curhp(m_clPlayer.getHP());
//		m_clPlayer.plane_leave(ask,uCode);
//		{
//			m_clPlayer.sendMsg(P_S2C_Protocol_Common,S2C_Common_PlaneLeave_Ack,&ask,uCode);
//		}
//	}
//	
//	return true;
//}
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::dazuo(std::string &strParam, std::string &outMsg)
//{
//	uint8 uCode=0;
//	m_clPlayer.getPlayerCommon()->onTakeASeat(uCode);
//	return true;
//}
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::addtitle(std::string &strParam, std::string &outMsg)
//{
//	uint32 uID		= (uint32)readInt64FromString(strParam,' ');
//	m_clPlayer.getPlayerTitle()->addTitle(uID);
//	return true;
//}
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::deltitle(std::string &strParam, std::string &outMsg)
//{
//	uint32 uID		= (uint32)readInt64FromString(strParam,' ');
//	m_clPlayer.getPlayerTitle()->delTitle(uID);
//	return true;
//}
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::teamcreatebytargetrandom(std::string &strParam, std::string &outMsg)
//{
//	CPlayerTeam* pPlayerTeam = dynamic_cast<CPlayerTeam*>(m_clPlayer.getPlayerFun(GameSystemType_Team));
//	CHECKF(NULL != pPlayerTeam);
//
//	PBTeamCreateAsk ask;
//	PBTeamTarget* pbTarget = ask.mutable_target();
//	if (pbTarget)
//	{
//		pbTarget->set_targetid(g_clTeamTargetData.randomGetUniqueTargetId());
//	}
//	//随机设定targetid
//	CNetStream stStream;
//	stStream.write(m_clPlayer.getPlayerID());
//	stStream.SerializeToStream(ask);
//	m_clPlayer.sendBufferToWorld(P_G2W_Protocol_Team, C2S_Team_Create, stStream.getBuffer(), stStream.getPosition());
//	return true;
//}
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::teamapplybytarget(std::string &strParam, std::string &outMsg)
//{
//	uint64 uTeamId		= (uint64)readInt64FromString(strParam,' ');
//
//	CPlayerTeam* pPlayerTeam = dynamic_cast<CPlayerTeam*>(m_clPlayer.getPlayerFun(GameSystemType_Team));
//	CHECKF(NULL != pPlayerTeam);
//
//	PBC2GApplyTeamByTargetAsk ask;
//	ask.set_teamid(uTeamId);
//	//随机设定targetid
//	CNetStream stStream;
//	stStream.write(m_clPlayer.getPlayerID());
//	stStream.SerializeToStream(ask);
//	m_clPlayer.sendBufferToWorld(P_G2W_Protocol_Team, C2S_Team_ApplyTeamByTarget, stStream.getBuffer(), stStream.getPosition());
//	return true;
//}
//
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::fasionadd(std::string &strParam, std::string &outMsg)
//{
//	uint32 uFashionID		= (uint32)readInt64FromString(strParam,' ');
//
//	CPlayerFashion* pPlayerFashion = (CPlayerFashion*)m_clPlayer.getPlayerFun(GameSystemType_Fashion);
//	if (!pPlayerFashion )
//		return false;
//
//	pPlayerFashion->addFashion(uFashionID);
//	return true;
//}
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::fasiondel(std::string &strParam, std::string &outMsg)
//{
//	uint32 uFashionID		= (uint32)readInt64FromString(strParam,' ');
//
//	CPlayerFashion* pPlayerFashion = (CPlayerFashion*)m_clPlayer.getPlayerFun(GameSystemType_Fashion);
//	if (!pPlayerFashion )
//		return false;
//
//	pPlayerFashion->delFashion(uFashionID);
//	return true;
//}
//
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::clientpbmsg(std::string &strParam, std::string &outMsg)
//{
//	uint8 uMainProto		= (uint8)readInt64FromString(strParam,' ');
//	uint8 uSubProto			= (uint8)readInt64FromString(strParam,' ');
//
//	char szBuff[256];
//	CNetStream clBuffer(szBuff,sizeof(szBuff));
//
//	std::string strPBName;
//	getSubString(strParam,strPBName,' ');
//	if (!strPBName.empty())
//	{
//		std::auto_ptr<google::protobuf::Message> msg(createMessage(strPBName));
//		if ( !msg.get() )
//		{
//			outMsg = "not find msg [";
//			outMsg += strPBName;
//			outMsg += "]";
//			return false;
//		}
//
//		auto pDescriptor	= msg->GetDescriptor();
//		auto pReflection	= msg->GetReflection();
//		if (!pDescriptor || !pReflection)
//			return false;
//
//		while (!strParam.empty())
//		{
//			std::string strKey;
//			std::string strValue;
//			getSubString(strParam,strKey,'=');
//			getSubString(strParam,strValue,' ');
//
//			auto pFieldDescriptor = pDescriptor->FindFieldByName(strKey);
//			if (!pFieldDescriptor)
//			{
//				outMsg = "not find field[";
//				outMsg += strKey;
//				outMsg += "]";
//				outMsg += "in msg [";
//				outMsg += strPBName;
//				outMsg += "]";
//				return false;
//			}
//			
//			if (pFieldDescriptor->is_repeated())
//			{
//				outMsg = "not supported repeated field[";
//				outMsg += strKey;
//				outMsg += "]";
//				outMsg += "in msg [";
//				outMsg += strPBName;
//				outMsg += "]";
//				return false;
//			}
//
//			std::string strTmp = strValue;
//			int64 nValue	   = readInt64FromString(strTmp,' ');
//			switch (pFieldDescriptor->cpp_type())
//			{
//			case  google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
//				{
//					outMsg = "not supported message field[";
//					outMsg += strKey;
//					outMsg += "]";
//					outMsg += "in msg [";
//					outMsg += strPBName;
//					outMsg += "]";
//					return false;
//				}
//				break;
//#define SET_FIELD(CPPTYPE, METHOD, VALUE_EXP)										\
//			case google::protobuf::FieldDescriptor::CPPTYPE_##CPPTYPE:				\
//				{																	\
//					pReflection->Set##METHOD(msg.get(), pFieldDescriptor, (VALUE_EXP));	\
//				}																	\
//				break;
//			SET_FIELD( INT32,	Int32,	(int32)nValue);
//			SET_FIELD( INT64,	Int64,	(int64)nValue);
//			SET_FIELD(UINT32,	UInt32,	(uint32)nValue);
//			SET_FIELD(UINT64,	UInt64,	(uint64)nValue);
//			SET_FIELD( FLOAT,	Float,	(float)nValue);
//			SET_FIELD(DOUBLE,	Double, (double)nValue);
//			SET_FIELD(BOOL,		Bool,	nValue != 0 );
//#undef SET_FIELD
//			case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
//				{
//					pReflection->SetString(msg.get(), pFieldDescriptor, strValue);														
//				}
//				break;
//			case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
//				{
//					const auto *enumDes = pFieldDescriptor->enum_type();
//					CHECKC(NULL != enumDes);
//					pReflection->SetEnum(msg.get(), pFieldDescriptor, enumDes->value((int32)nValue));
//				}
//				break;
//			default:
//				break;
//			}
//		}
//		clBuffer.SerializeToStream(*msg);
//		clBuffer.initBufferStream(clBuffer.getBuffer(),clBuffer.getPosition());
//	}
//	else
//	{
//		clBuffer.setPosition(clBuffer.getStreamSize());
//	}
//
//	m_clPlayer.functionNetPacket(uMainProto,uSubProto,0,clBuffer);
//	return true;
//}
//
//
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::setpkmode(std::string &strParam, std::string &outMsg)
//{
//	uint8 uPkMode		= (uint8)readInt64FromString(strParam,' ');
//
//	uint8 uCode;
//	m_clPlayer.setPKType(uPkMode,uCode);
//	return true;
//}
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::acceptteamtask(std::string &strParam, std::string &outMsg)
//{
//	if (!m_clPlayer.getPlayerTeam())
//		return false;
//	uint32 uID		= (uint32)readInt64FromString(strParam,' ');
//	uint32 uNpcUnitID		= (uint32)readInt64FromString(strParam,' ');
//	uNpcUnitID = MAX((uint32)1,uNpcUnitID);
//	uint8 uCode;
//	return 	m_clPlayer.getPlayerTeam()->accept_teamtask(uID,uNpcUnitID,uCode);
//}
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::enterteamtaskcopymap(std::string &strParam, std::string &outMsg)
//{
//	if (!m_clPlayer.getPlayerTeam())
//		return false;
//	uint32 uNpcUnitID		= (uint32)readInt64FromString(strParam,' ');
//	uNpcUnitID = MAX((uint32)1,uNpcUnitID);
//	uint8 uCode;
//	return 	m_clPlayer.getPlayerTeam()->enter_teamtask_copymap(uNpcUnitID,uCode);
//}
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::completeamtask(std::string &strParam, std::string &outMsg)
//{
//	if (!m_clPlayer.getPlayerTeam())
//		return false;
//	uint32 uNpcUnitID		= (uint32)readInt64FromString(strParam,' ');
//	uNpcUnitID = MAX((uint32)1,uNpcUnitID);
//	uint8 uCode;
//	return 	m_clPlayer.getPlayerTeam()->complete_teamtask(uNpcUnitID,uCode);
//}
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::getitemsn(std::string &strParam, std::string &outMsg)
//{
//	uint32 uItemID		= (uint32)readInt64FromString(strParam,' ');
//
//	CGameItemMgr& clGameItemMgr =  m_clPlayer.getGameItemMgr();
//	
//	std::stringstream sstr;
//
//	stl_vector<uint64> vecItemSN;
//	clGameItemMgr.findByItemID(uItemID, vecItemSN);
//	uint32 uCount = 0;
//	for (uint32 _pos =0; _pos < (uint32)vecItemSN.size() ; ++_pos)
//	{
//		if (uCount == 15)
//		{
//			break;
//		}
//		sstr << "itemSN" << ":" << vecItemSN[_pos] << "\r\n"; 
//		uCount++;
//	}
//
//	outMsg = sstr.str();
//	CLog::error("gm[getitemsn] ret=%s", outMsg.c_str());
//	return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::addcontribution				(std::string&strParam,std::string&outMsg)
//{
//	int32 iValue		= (int32)readInt64FromString(strParam,' ');
//	return m_clPlayer.addContribution(iValue,DoingType_GM);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::entermatch				(std::string&strParam,std::string&outMsg)
//{
//	uint32 uID		= (uint32)readInt64FromString(strParam,' ');
//	uint8 uCode = 0;
//	return m_clPlayer.getPlayerPvpWar()->enterMatchAsk(uID,uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::exitmatch				(std::string&strParam,std::string&outMsg)
//{
//	uint8 uCode = 0;
//	return m_clPlayer.getPlayerPvpWar()->exitMatchAsk(uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::enterready				(std::string&strParam,std::string&outMsg)
//{
//	uint8 uCode = 0;
//	return m_clPlayer.getPlayerPvpWar()->enterReadyAsk(uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::exitready				(std::string&strParam,std::string&outMsg)
//{
//	uint8 uCode = 0;
//	return m_clPlayer.getPlayerPvpWar()->exitReadyAsk(uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::openworldbossdlg				(std::string&strParam,std::string&outMsg)
//{
//	uint8 uCode = 0;
//	return m_clPlayer.getPlayerWorldBoss()->onGetInfo(uCode);
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::runworldbossevent				(std::string&strParam,std::string&outMsg)
//{
//	//直接执行时间片,不等时间片回调
//	 g_pWorldBossMgr->timerProcess();
//	 return true;
//}
////------------------------------------------------------------------------------
////------------------------------------- 
//bool CClientGMCommand::testworldbossbroadcast				(std::string&strParam,std::string&outMsg)
//{
//	//直接执行时间片,不等时间片回调
//	PBG2CBroadCast_WorldBoss msg;
//	msg.set_bossactivityid(1);
//	g_pGameBroadcast->broadCastMsg(g_pServer->getLogicWorldID(),BroadcastChannel_World,0,\
//		P_S2C_Protocol_Broadcast,S2C_BroadCast_WorldBoss,&msg);
//	return true;
//}
//
////-------------------------------------------------------------
////------------------------------activityreset
//bool CClientGMCommand::activityreset				(std::string&strParam,std::string&outMsg)
//{
//	CPlayerActivity * pActivity = m_clPlayer.getPlayerActivity();
//	if (pActivity)
//		pActivity->gm_clear();	
//	return true;
//}
//
////-------------------------------------------------------------
////------------------------------复活
//bool CClientGMCommand::relive(std::string&strParam,std::string&outMsg)
//{
//	uint32 uType = (uint32)readInt64FromString(strParam,' ');
//	CPlayerRelive * pRelive = m_clPlayer.getPlayerRelive();
//	uint8 uCode = 0;
//	if (pRelive)
//		pRelive->relive(uType, uCode);	
//	return true;
//}
//
////-------------------------------------------------------------
////------------------------------测试领取世界boss掉落奖励
//bool CClientGMCommand::getdropfromwb(std::string&strParam,std::string&outMsg)
//{
//	uint32 uDropID = (uint32)readInt64FromString(strParam,' ');
//
//	_stWorldBossInfo st;
//	st.onGetRankPrize(uDropID, &m_clPlayer, 1);
//	return true;
//}
//
////-------------------------------------------------------------
////------------------------------测试领取世界boss,  0 直接冻结,非0 解冻
//bool CClientGMCommand::freezewb(std::string&strParam,std::string&outMsg)
//{
//	uint32 uIsFreeze	= (uint32)readInt64FromString(strParam,' ');
//	if (uIsFreeze == 0)
//	{
//		g_pWorldBossMgr->freezeBoss(m_clPlayer.getSceneID(), true);
//	}
//	else
//	{
//		g_pWorldBossMgr->freezeBoss(m_clPlayer.getSceneID(), false);
//	}
//	return true;
//}
//
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::teamconfirm				(std::string&strParam,std::string&outMsg)
//{
//	uint32 uType  = (uint32)readInt64FromString(strParam,' ') ;
//	uint32 uID  =	(uint32)readInt64FromString(strParam,' ') ;
//	bool bConfirm = (uint32)readInt64FromString(strParam,' ') > 0;
//	uint8 uCode = 0;
//	return m_clPlayer.getPlayerTeam()->confirm(uID,uType, bConfirm,uCode);
//}
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::setmakeskilled				(std::string&strParam,std::string&outMsg)
//{
//	uint32 uEquiptype = (uint32)readInt64FromString(strParam,' ') ;
//	uint32 uNewLevel = (uint32)readInt64FromString(strParam,' ') ;
//	uint32 uNewPoint = (uint32)readInt64FromString(strParam,' ') ;
//	
//	return m_clPlayer.getGameItemMgr().onGmsetSkilled((_emEquipType)uEquiptype, uNewLevel, uNewPoint);
//}
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::weallogin				(std::string&strParam,std::string&outMsg)
//{
//	uint32 uID = (uint32)readInt64FromString(strParam,' ') ;
//	uint32 uDay = (uint32)readInt64FromString(strParam,' ') ;
//	uint8 uCode = 0;
//	return m_clPlayer.getPlayerWeal() ? m_clPlayer.getPlayerWeal()->loginPrize(uID, uDay, uCode):false;
//}
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::MEOpen				(std::string&strParam,std::string&outMsg)
//{
//	uint8 uCode = 0;
//	return m_clPlayer.getGameItemMgr().openMakeEquipDlgAsk(uCode);
//}
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::MEOpenoem				(std::string&strParam,std::string&outMsg)
//{
//	uint8 uCode = 0;
//	return m_clPlayer.getGameItemMgr().openMakeEquipOemDlgAsk(uCode);
//}
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::MECommitoem				(std::string&strParam,std::string&outMsg)
//{
//	uint8 uCode = 0;
//	uint32 uMakeID = (uint32)readInt64FromString(strParam,' ') ;
//	uint32 uFee = (uint32)readInt64FromString(strParam,' ') ;
//	uint32 uCount = (uint32)readInt64FromString(strParam,' ') ;
//	uint32 uRemove = (uint32)readInt64FromString(strParam,' ') ;
//	uint32 uClearall = (uint32)readInt64FromString(strParam,' ') ;
//
//	PBC2GItemCommitEquipMakeOEMAsk pb;
//	if (uMakeID != 0)
//	{
//		PBEquipMakeOEM* pOEM = pb.mutable_oem();
//		if (pOEM)
//		{
//			pOEM->set_count(uCount);
//			pOEM->set_makeid(uMakeID);
//			pOEM->set_fee(uFee);
//		}
//		if (uRemove)
//		{
//			pb.set_isremoved(true);
//		}
//		else
//		{
//			pb.set_isremoved(false);
//		}
//		if (uClearall)
//		{
//			pb.set_isclearall(true);
//		}
//		else
//		{
//			pb.set_isclearall(false);
//		}
//	}
//	return m_clPlayer.getGameItemMgr().commitMakeEquipOemAsk(pb, uCode);
//}
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::MEVisit				(std::string&strParam,std::string&outMsg)
//{
//	uint8 uCode = 0;
//	uint32 uPlayerID = (uint32)readInt64FromString(strParam,' ') ;
//	return m_clPlayer.getGameItemMgr().visitMakeEquipOemAsk(uPlayerID, uCode);
//}
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::MEOem				(std::string&strParam,std::string&outMsg)
//{
//	uint8 uCode = 0;
//	uint32 uPlayerID = (uint32)readInt64FromString(strParam,' ') ;
//	uint32 uMakeID = (uint32)readInt64FromString(strParam,' ') ;
//	uint32 uMark = (uint32)readInt64FromString(strParam,' ') ;
//
//
//
//	CScenePlayer* pMaker = g_pGamePlayerMgr->findPlayer(uPlayerID);
//	if (!pMaker)
//	{
//		uCode = R_ResultItem_OEM_Not_Online;
//		m_clPlayer.getGameItemMgr().send_VisitMakeOEMAck(NULL,uCode);
//	}
//	else if (!pMaker->getGameItemMgr().preCheckMakeOEMSkilled(uMakeID, &m_clPlayer, uCode))
//	{
//		m_clPlayer.getGameItemMgr().send_VisitMakeOEMAck(NULL,uCode);
//	}
//	else if (!m_clPlayer.getGameItemMgr().equipMake(uMakeID, uMark, uCode))
//	{
//		uCode = R_ResultItem_Fail;
//		m_clPlayer.getGameItemMgr().send_VisitMakeOEMAck(NULL,uCode);
//	}
//	else
//	{//成功
//		//卖家得钱,扣除活力,买家扣掉钱
//		pMaker->getGameItemMgr().oemMakeSuccessAddSkilled(uMakeID, &m_clPlayer);
//		pMaker->getGameItemMgr().openMakeEquipOemDlgAsk(uCode);
//
//	}
//	return true;
//}
//
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::factioncreate(std::string&strParam,std::string&outMsg)
//{
//	readInt64FromString(strParam,' ') ;
//	CPlayerFaction* pPlayerFaction = (CPlayerFaction*)m_clPlayer.getPlayerFun(GameSystemType_Faction);
//	if (!pPlayerFaction )
//		return false;
//
//	PBC2GFactionCreate ask;
//	std::string FactionName = "Faction"+m_clPlayer.getName();
//	char Banner[10] = {0};
//	sprintf(Banner, "%d", m_clPlayer.getPlayerID() % 100);
//	ask.set_name(FactionName.c_str());
//	ask.set_worldid(m_clPlayer.getLogicWorldID());
//	ask.set_banner(Banner);
//	ask.set_declaration("you can you up!");
//	ask.set_playerid(m_clPlayer.getPlayerID());
//	ask.set_corps(Corps_north);
//	ask.set_creattime(getTime());
//
//	pPlayerFaction->FactionCreateGM(&ask);
//	return true;
//}
//
////-------------------------------------------------------------
////------------------------------
//
//bool CClientGMCommand::factionapply(std::string &strParam, std::string &outMsg)
//{
//	uint32 uID = (uint32)readInt64FromString(strParam,' ') ;
//	CPlayerFaction* pPlayerFaction = (CPlayerFaction*)m_clPlayer.getPlayerFun(GameSystemType_Faction);
//	if (!pPlayerFaction )
//		return false;
//	
//	// 判断是否已有帮会
//	if (0 != pPlayerFaction->getFactionID())
//		return false;
//
//	//判断离开cd
//	if (pPlayerFaction->checkCanJoinOrCreate())
//		return false;
//	
//	PBC2GFactionApply ask;
//	if (true)
//	{
//		ask.set_factionid(uID);
//		ask.set_isapply(true);
//		ask.set_worldid(m_clPlayer.getLogicWorldID());
//		PBFactionMemberBase *base = ask.mutable_base();
//		base->set_playerid(m_clPlayer.getPlayerID());
//		base->set_playername(m_clPlayer.getPlayerBase().szPlayerName);
//		base->set_level(m_clPlayer.getLevel());
//		base->set_job(m_clPlayer.getJob());
//		base->set_identity(m_clPlayer.getPlayerBase().uIdentity);
//		base->set_force(m_clPlayer.getFightPower());
//	}
//
//	CNetStream stStream;
//	stStream.write(m_clPlayer.getPlayerID());
//	stStream.SerializeToStream(ask);
//	return m_clPlayer.sendBufferToWorld(P_G2W_Protocol_Faction, C2S_Faction_Apply, stStream.getBuffer(), stStream.getPosition());
//}
//
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::setCT(std::string&strParam,std::string&outMsg)
//{
//	uint32 uCopymapID = (uint32)readInt64FromString(strParam,' ');
//	m_clPlayer.getPlayerClimbTower()->setJumpNextCopymapGM(uCopymapID);
//	return true;
//}
//
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::MANORopenall			(std::string&strParam,std::string&outmsg)
//{
//	PBC2GManorGetAllInfoAsk ask;
//	CNetStream stStream;
//	stStream.write(m_clPlayer.getPlayerID());
//	stStream.SerializeToStream(ask);
//	m_clPlayer.sendBufferToWorld(P_G2W_Protocol_Manor, C2S_Manor_Get_All_Info_Ask, stStream.getBuffer(), stStream.getPosition());
//	return true;
//}
//
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::MANORopenfight			(std::string&strParam,std::string&outmsg)
//{
//	PBC2GManorGetFightInfoAsk ask;
//	CNetStream stStream;
//	stStream.write(m_clPlayer.getPlayerID());
//	stStream.SerializeToStream(ask);
//	m_clPlayer.sendBufferToWorld(P_G2W_Protocol_Manor, C2S_Manor_Get_Fight_Info_Ask, stStream.getBuffer(), stStream.getPosition());
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::MANORapply			(std::string&strParam,std::string&outmsg)
//{
//	readInt64FromString(strParam,' ');
//	uint32 uIsCancle = (uint32)readInt64FromString(strParam,' ');
//
//	PBC2GManorApplyFightAsk ask;
//	if (uIsCancle > 0)
//	{
//		ask.set_iscancel(true);
//	}
//	else
//	{
//		ask.set_iscancel(false);
//	}
//	ask.set_manorid(1);
//	CNetStream stStream;
//	stStream.write(m_clPlayer.getPlayerID());
//	stStream.SerializeToStream(ask);
//	m_clPlayer.sendBufferToWorld(P_G2W_Protocol_Manor, C2S_Manor_Apply_Fight_Ask, stStream.getBuffer(), stStream.getPosition());
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::MANORenterlobby			(std::string&strParam,std::string&outmsg)
//{
//	uint32 uManorID = (uint32)readInt64FromString(strParam,' ');
//	PBC2GManorEnterLobbyAsk ask;
//	ask.set_manorid(uManorID);
//	CNetStream stStream;
//	stStream.write(m_clPlayer.getPlayerID());
//	stStream.SerializeToStream(ask);
//	m_clPlayer.sendBufferToWorld(P_G2W_Protocol_Manor, C2S_Manor_Enter_Lobby_Ask, stStream.getBuffer(), stStream.getPosition());
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::MANORmvpvote			(std::string&strParam,std::string&outmsg)
//{
//	//PBC2GManorGetAllInfoAsk ask;
//	//CNetStream stStream;
//	//stStream.write(m_clPlayer.getPlayerID());
//	//stStream.SerializeToStream(ask);
//	//m_clPlayer.sendBufferToWorld(P_G2W_Protocol_Manor, C2S_Manor_MVPVote_Ask, stStream.getBuffer(), stStream.getPosition());
//	return true;
//}
////------------------------------------------------------------------------------
////-------------------------------------
//bool	CClientGMCommand::MANORenterbf			(std::string&strParam,std::string&outmsg)
//{
//	uint32 uManorID = (uint32)readInt64FromString(strParam,' ');
//	uint32 uRegionID = (uint32)readInt64FromString(strParam,' ');
//
//
//	PBG2WManorBattleFieldEnterAsk pbAsk;
//	pbAsk.Clear();
//	pbAsk.set_manorid(uManorID);
//	pbAsk.set_regionid(uRegionID);
//	pbAsk.set_playerid(m_clPlayer.getPlayerID());
//	g_pServer->sendMsgToWorld(g_pServer->getLogicWorldID(),P_G2W_Protocol_Manor, G2W_Manor_BattleField_Enter_Ask, &pbAsk);
//
//	return true;
//}
//
//bool	CClientGMCommand::KSSQ			(std::string&strParam,std::string&outmsg)
//{
//	PBG2WManorSetActivityAsk pbAsk;
//	pbAsk.Clear();
//	pbAsk.set_setapply(true);
//	g_pServer->sendMsgToWorld(g_pServer->getLogicWorldID(),P_G2W_Protocol_Manor, G2W_Manor_GM_Set_Activity_Ask, &pbAsk);
//	return true;
//}
//bool	CClientGMCommand::GBSQ			(std::string&strParam,std::string&outmsg)
//{
//	PBG2WManorSetActivityAsk pbAsk;
//	pbAsk.Clear();
//	pbAsk.set_setapply(false);
//	g_pServer->sendMsgToWorld(g_pServer->getLogicWorldID(),P_G2W_Protocol_Manor, G2W_Manor_GM_Set_Activity_Ask, &pbAsk);
//	return true;
//}
//bool	CClientGMCommand::KSLDZ		(std::string&strParam,std::string&outmsg)
//{
//	PBG2WManorSetActivityAsk pbAsk;
//	pbAsk.Clear();
//	pbAsk.set_setfight(true);
//	g_pServer->sendMsgToWorld(g_pServer->getLogicWorldID(),P_G2W_Protocol_Manor, G2W_Manor_GM_Set_Activity_Ask, &pbAsk);
//	return true;
//}
//bool	CClientGMCommand::GBLDZ			(std::string&strParam,std::string&outmsg)
//{
//	PBG2WManorSetActivityAsk pbAsk;
//	pbAsk.Clear();
//	pbAsk.set_setfight(false);
//	g_pServer->sendMsgToWorld(g_pServer->getLogicWorldID(),P_G2W_Protocol_Manor, G2W_Manor_GM_Set_Activity_Ask, &pbAsk);
//	return true;
//}
//
//
//bool CClientGMCommand::stronger			(std::string&strParam,std::string&outmsg)
//{
//	uint32 ID = (uint32)readInt64FromString(strParam,' ');
//
//	CPlayerCommon *pCommon = m_clPlayer.getPlayerCommon();
//	uint8 uCode = 0;
//	m_clPlayer.calculateSubFightPower(uCode);
//	if (pCommon && !pCommon->strongerGetReWard(ID, uCode))
//		return false;
//		
//	return true;
//}
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::entersinglematch			(std::string&strParam,std::string&outmsg)
//{
//	uint8 uCode = 0;
//	return m_clPlayer.getPlayerSinglePvp()->enterMatchAsk(uCode);
//}
////-------------------------------------------------------------
////------------------------------
//bool CClientGMCommand::exitsinglematch			(std::string&strParam,std::string&outmsg)
//{
//	uint8 uCode = 0;
//	return m_clPlayer.getPlayerSinglePvp()->exitMatchAsk(uCode);
//}
//bool CClientGMCommand::singledayprize			(std::string&strParam,std::string&outmsg)
//{
//	uint32 uDayCount = (uint32)readInt64FromString(strParam,' ');
//	uint8 uCode = 0;
//	return m_clPlayer.getPlayerSinglePvp()->dayPrizeAsk(uDayCount,uCode);
//}