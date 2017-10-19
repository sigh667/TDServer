/*------------- clientGMCommand.h
* Copyright (C): 2014 Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.01
* Date         : 2014/22/10 9:21
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include <string>
/*************************************************************/
class CScenePlayer;
class PBGLAGLocatePlayerAck;
class CClientGMCommand
{
private:
	typedef bool (CClientGMCommand::*_cmdTextMsg)(std::string&strParam,std::string&outMsg);

	struct _stGMCommand
	{
		_cmdTextMsg func;	//函数指针
		std::string desc;	//指令说明

		_stGMCommand		()		{	reset();	}
		_stGMCommand		(_cmdTextMsg _func, const char* _desc)
		{	
			func = _func;
			desc = _desc;
		}

		void		reset	()
		{
			func = NULL;
			desc.clear();
		}
	};

	typedef stl_map<std::string, _stGMCommand>	MAP_CMD;

private:
	CScenePlayer&	m_clPlayer;
	MAP_CMD			m_mapCmd;

	void			addCMD				(const char* pCmd, _cmdTextMsg pFun, const char* pDesc);
public:
	CClientGMCommand(CScenePlayer& clPlayer);
	virtual~CClientGMCommand();
public:
	bool			handle_txt_cmd		(uint8 uEventID,CNetStream& clStream);

public:
	void			on_go_result		(PBGLAGLocatePlayerAck& ack);
	void			on_find_result		(PBGLAGLocatePlayerAck& ack);
public:
#define _declare_gm_handler(func)	bool func(std::string&strParam,std::string&outMsg)
	//_declare_gm_handler(help);
	//_declare_gm_handler(cleanday);
	//_declare_gm_handler(trap);
	//_declare_gm_handler(pathobj);
	//_declare_gm_handler(path);
	//_declare_gm_handler(ray);
	//_declare_gm_handler(move);
	//_declare_gm_handler(exp);
	//_declare_gm_handler(level);
	//_declare_gm_handler(promote);
	//_declare_gm_handler(fly);
	//_declare_gm_handler(flypos);
	//_declare_gm_handler(fly1);
	//_declare_gm_handler(baseinfo);
	//_declare_gm_handler(useskill);
	//_declare_gm_handler(addskill);
	//_declare_gm_handler(upgradeskill);
	//_declare_gm_handler(breakskill);
	//_declare_gm_handler(monster);
	//_declare_gm_handler(dartcar);
	//_declare_gm_handler(openmp);
	//_declare_gm_handler(closemp);
	//_declare_gm_handler(addbuff);
	//_declare_gm_handler(delbuff);
	//_declare_gm_handler(addattribute);
	//_declare_gm_handler(addexpend);
	//_declare_gm_handler(additem);
	//_declare_gm_handler(delitem);
	//_declare_gm_handler(sellitem);
	//_declare_gm_handler(equipitem);
	//_declare_gm_handler(moveitem);
	//_declare_gm_handler(buybackitem);
	//_declare_gm_handler(useitem);
	//_declare_gm_handler(splititem);
	//_declare_gm_handler(clearbag);
	//_declare_gm_handler(getposition);
	//_declare_gm_handler(clearmonster);
	//_declare_gm_handler(offlinedata);
	//_declare_gm_handler(sendmail);
	//_declare_gm_handler(gmmail);
	//_declare_gm_handler(questaccept);
	//_declare_gm_handler(questgiveup);
	//_declare_gm_handler(questcomplete);
	//_declare_gm_handler(questupdate);
	//_declare_gm_handler(questfire);
	//_declare_gm_handler(questauto);
	//_declare_gm_handler(questreset);
	//_declare_gm_handler(reloaddata);
	//_declare_gm_handler(copymapenter);
	//_declare_gm_handler(copymapcreate);
	//_declare_gm_handler(copymapexit);
	//_declare_gm_handler(copymapinvite);
	//_declare_gm_handler(copymapacceptinvite);
	//_declare_gm_handler(copymapteammemberinfo);
	//_declare_gm_handler(copymapclear);
	//_declare_gm_handler(copymapcomplete);
	//_declare_gm_handler(tradeapply);
	//_declare_gm_handler(tradeanswer);
	//_declare_gm_handler(tradeadditem);
	//_declare_gm_handler(tradedelitem);
	//_declare_gm_handler(tradesetmoney);
	//_declare_gm_handler(tradelock);
	//_declare_gm_handler(tradeconfirm);
	//_declare_gm_handler(tradecancel);
	//_declare_gm_handler(salemoneysale);
	//_declare_gm_handler(salemoneysalecancel);
	//_declare_gm_handler(salemoneylist);
	//_declare_gm_handler(auctionsale);
	//_declare_gm_handler(auctionsalecancel);
	//_declare_gm_handler(auctionbuy);
	//_declare_gm_handler(auctionlist);
	//_declare_gm_handler(auctionave);
	//_declare_gm_handler(toplistlist);
	//_declare_gm_handler(toplistplayer);
	//_declare_gm_handler(toplistupdate);
	//_declare_gm_handler(toplistrand);
	//_declare_gm_handler(toplist_resettime);
	//_declare_gm_handler(randpos);
	//_declare_gm_handler(call);
	//_declare_gm_handler(find);
	//_declare_gm_handler(go);
	//_declare_gm_handler(pkupdate);
	//_declare_gm_handler(friendadd);
	//_declare_gm_handler(frienddel);
	//_declare_gm_handler(friendanswer);
	//_declare_gm_handler(friendvalue);
	//_declare_gm_handler(zisha);
	//_declare_gm_handler(fuhuo);
	//_declare_gm_handler(splitbingfu);
	//_declare_gm_handler(upgradebingfu);
	//_declare_gm_handler(upgradewulue);
	//_declare_gm_handler(setbingfu);
	//_declare_gm_handler(wentaosetscheme);
	//_declare_gm_handler(wentaoqianxiu);
	//_declare_gm_handler(wentaotupo);
	//_declare_gm_handler(wentaoupgrate);
	//_declare_gm_handler(wentaosetbingfu);
	//_declare_gm_handler(wentaoreset);
	//_declare_gm_handler(wentaoactivescheme);
	//_declare_gm_handler(wentaoactivebingfa);
	//_declare_gm_handler(teaminvite);
	//_declare_gm_handler(teamacceptinvite);
	//_declare_gm_handler(teamapply);
	//_declare_gm_handler(teamacceptapply);
	//_declare_gm_handler(teamleave);
	//_declare_gm_handler(teamchangeleader);
	//_declare_gm_handler(teampublishask);
	//_declare_gm_handler(teampublishcancel);
	//_declare_gm_handler(teampublishinvite);
	//_declare_gm_handler(teampublishacceptinvite);
	//_declare_gm_handler(teampublishlist);
	//_declare_gm_handler(teamkick);
	//_declare_gm_handler(teamkickvote);
	//_declare_gm_handler(teamsetallot);
	//_declare_gm_handler(teamsetallotvote);
	//_declare_gm_handler(teamnearplayerlist);
	//_declare_gm_handler(teamnearteamlist);
	//_declare_gm_handler(dropitem);
	//_declare_gm_handler(dropiteminfo);
	//_declare_gm_handler(dropitemdice);
	//_declare_gm_handler(dropitemassign);
	//_declare_gm_handler(dropitempickup);
	//_declare_gm_handler(equipenhance);
	//_declare_gm_handler(equipforge);
	//_declare_gm_handler(equipinherit);
	//_declare_gm_handler(equipmake);
	//_declare_gm_handler(mount);
	//_declare_gm_handler(skillcmd);
	//_declare_gm_handler(broadcast);
	//_declare_gm_handler(httpstart);
	//_declare_gm_handler(httpstop);
	//_declare_gm_handler(httprequest);
	//_declare_gm_handler(httpresponse);
	//_declare_gm_handler(bosslist);
	//_declare_gm_handler(identity_open);
	//_declare_gm_handler(identity_learn);
	//_declare_gm_handler(identity_reset);
	//_declare_gm_handler(identity_upskill);
	//_declare_gm_handler(addliveblock);
	//_declare_gm_handler(callfollower);
	//_declare_gm_handler(clearskillcooltime);
	//_declare_gm_handler(clearskillexpend);
	//_declare_gm_handler(supply);
	//_declare_gm_handler(supplyextra);
	//_declare_gm_handler(save);
	//_declare_gm_handler(worldstarttime);
	//_declare_gm_handler(itemlike);
	//_declare_gm_handler(buy);
	//_declare_gm_handler(debugmsg);
	//_declare_gm_handler(wuxuenew);
	//_declare_gm_handler(wuxuedel);
	//_declare_gm_handler(wuxuechange);
	//_declare_gm_handler(wuxuelearnskill);
	//_declare_gm_handler(sysopen);
	//_declare_gm_handler(randscene);
	//_declare_gm_handler(callup);
	//_declare_gm_handler(acceptidentity);
	//_declare_gm_handler(hide);
	//_declare_gm_handler(factionbosspersonaltop);
	//_declare_gm_handler(factionbossfactiontop);
	//_declare_gm_handler(factionbossuniontop);
	//_declare_gm_handler(qinggong);
	//_declare_gm_handler(battleattrdetail);
	//_declare_gm_handler(setdtime);
	//_declare_gm_handler(getdtime);
	//_declare_gm_handler(resumedtime);
	//_declare_gm_handler(benew);
	//_declare_gm_handler(enterplane);
	//_declare_gm_handler(dazuo);
	//_declare_gm_handler(addtitle);
	//_declare_gm_handler(deltitle);
	//_declare_gm_handler(teamcreatebytargetrandom);
	//_declare_gm_handler(teamapplybytarget);
	//_declare_gm_handler(fasionadd);
	//_declare_gm_handler(fasiondel);
	//_declare_gm_handler(clientpbmsg);
	//_declare_gm_handler(setpkmode);
	//_declare_gm_handler(acceptteamtask);
	//_declare_gm_handler(enterteamtaskcopymap);
	//_declare_gm_handler(completeamtask); 
	//_declare_gm_handler(getitemsn);
	//_declare_gm_handler(addcontribution);
	//_declare_gm_handler(entermatch);
	//_declare_gm_handler(exitmatch);
	//_declare_gm_handler(enterready);
	//_declare_gm_handler(exitready);
	//_declare_gm_handler(openworldbossdlg);
	//_declare_gm_handler(runworldbossevent);
	//_declare_gm_handler(testworldbossbroadcast);
	//_declare_gm_handler(activityreset);
	//_declare_gm_handler(relive);
	//_declare_gm_handler(getdropfromwb);
	//_declare_gm_handler(freezewb);
	//_declare_gm_handler(teamconfirm);
	//_declare_gm_handler(setmakeskilled);
	//_declare_gm_handler(weallogin);
	//_declare_gm_handler(MEOpen);
	//_declare_gm_handler(MEOpenoem);
	//_declare_gm_handler(MECommitoem);
	//_declare_gm_handler(MEVisit);
	//_declare_gm_handler(MEOem);
	//_declare_gm_handler(factioncreate);
	//_declare_gm_handler(factionapply);
	//_declare_gm_handler(setCT);

	//_declare_gm_handler(MANORopenall);
	//_declare_gm_handler(MANORopenfight);
	//_declare_gm_handler(MANORapply);
	//_declare_gm_handler(MANORenterlobby);
	//_declare_gm_handler(MANORenterbf);
	//_declare_gm_handler(MANORmvpvote);
	//_declare_gm_handler(KSSQ);
	//_declare_gm_handler(GBSQ);
	//_declare_gm_handler(KSLDZ);
	//_declare_gm_handler(GBLDZ);


	//_declare_gm_handler(stronger);
	//_declare_gm_handler(entersinglematch);
	//_declare_gm_handler(exitsinglematch);
	//_declare_gm_handler(singledayprize);
#undef _declare_gm_handler
};
/*************************************************************/

