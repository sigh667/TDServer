/*------------- gameMail.cpp
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ³ÂÌÚ
* Version      : V1.01
* Date         : 2016/05/04
*
*/ 
/***************************************************************
* 
***************************************************************/
#include "gameLogic/gameMail/gameMail.h"
#include "basic/timeFunction.h"
#include "protocol/CASProtocol.pb.h"
#include "gameLogic/sceneObject/scenePlayer.h"
#include "gameLogin/gamePlayerMgr.h"
#include "def_struct/gameStruct.h"
#include "protocol/MsgResult.pb.h"
#include "protocol/GameDefine.pb.h"
#include "cfg_data/constantData/constantData.h"
/*************************************************************/

//-------------------------------------------------------------
//------------------------------ 
CPlayerMail::CPlayerMail(CScenePlayer& clPlayer) : CGamePlayerFun(clPlayer)
{
	initialize();

	registerHandler(C2S_Mail_Read		, (CGamePlayerFun::_handleSubMsg)&CPlayerMail::handle_ReadMail_Ask);
	registerHandler(C2S_Mail_Delete		, (CGamePlayerFun::_handleSubMsg)&CPlayerMail::handle_DeleteMail_Ask);
	registerHandler(C2S_Mail_Reward		, (CGamePlayerFun::_handleSubMsg)&CPlayerMail::handle_RewardMail_Ask);
}

//-------------------------------------------------------------
//------------------------------ 
CPlayerMail::~CPlayerMail()
{

}

//-------------------------------------------------------------
//------------------------------ 
void CPlayerMail::initialize()
{
	CGamePlayerFun::initialize();
	m_uMaxID		= 0;
	m_mapMail.clear();
}

//-------------------------------------------------------------
//------------------------------ 
bool CPlayerMail::loadFromProtobuf(const PBPlayerData& pbPlayer)
{
	const PBPlayerMail& pMailList = pbPlayer.playermail();

	for (int32 i = 0; i < pMailList.maillist_size(); ++i)
	{
		const PBMail& mail = pMailList.maillist(i);
		m_uMaxID = MAX(m_uMaxID, mail.mailid());
		
		// ¹ýÂËµô¹ýÆÚÓÊ¼þ
		uint32 nowTime	=	getTime();
		if (nowTime - mail.sendtime() > (uint32)(g_clConstantData.Game_MailExpirationTime * 24 * 3600)) 
		{
			continue;
		}
		PBMail* pMail = m_mapMail.allocate(mail.mailid());
		if (pMail)
			pMail->CopyFrom(mail);
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool CPlayerMail::saveToProtobuf(PBPlayerData& pbPlayer)
{
	PBPlayerMail* pMail = pbPlayer.mutable_playermail();
	if (pMail)
	{
		pMail->Clear();
		MAP_MAIL::iterator _pos = m_mapMail.begin();
		MAP_MAIL::iterator _end = m_mapMail.end();
		for (; _pos != _end; ++_pos)
		{
			PBMail* mail = _pos->second ? pMail->add_maillist() : NULL;
			if (mail && _pos->second)
				mail->CopyFrom(*_pos->second);
		}
	}

	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool CPlayerMail::handleOfflineData(uint32 uSignType, CNetStream&clStream)
{
	PBMail mail;
	if (!clStream.ParseFromStream(mail))
		return false;

	addMail(mail);
	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool CPlayerMail::addMail(PBMail& stMail)
{
	if ((uint32)m_mapMail.size()>= (uint32)g_clConstantData.Game_MailMax_Svr) // ¼ì²âÓÊÏäÊÇ·ñÒÑÂú
		return false;
	
	PBMail* pMail = m_mapMail.allocate(++m_uMaxID);
	if (!pMail)
		return false;
	
	pMail->CopyFrom(stMail);
	pMail->set_mailid(m_uMaxID);
	m_clPlayer.sendMsg(P_S2C_Protocol_Mail, S2C_Mail_Load, pMail);
	return  true;
}

//-------------------------------------------------------------
//------------------------------ 
bool CPlayerMail::sendMail(PBMail& stMail)
{
	if (stMail.sendtime() == 0)
		stMail.set_sendtime((uint64)getTime());

	stMail.set_readed(0);
	stMail.set_mailid(0);

	CScenePlayer* pPlayer = g_pGamePlayerMgr->findPlayer(stMail.playerid());
	if (!pPlayer)
		return CScenePlayer::offlineData(stMail.worldid(), stMail.playerid(), GameSystemType_Mail, 0, &stMail);

	CPlayerMail* pPlayerMail = (CPlayerMail*)pPlayer->getPlayerFun(GameSystemType_Mail);
	if (!pPlayerMail)
		return false;

	return pPlayerMail->addMail(stMail);
}
//-------------------------------------------------------------
//------------------------------ 
bool	CPlayerMail::reward					(PBMail& stMail,uint8& uCode)
{
	uCode = R_ResultMail_Fail;
	if ( m_clPlayer.getGameItemMgr().getBagSpare() < (uint32)stMail.item_size() )
	{
		uCode = R_ResultMail_BagFull;
		return false;
	}

	for ( int32 i = 0 ; i < (int32)stMail.money_size() ;++i )
	{
		const PBMoney& pbMoney = stMail.money(i);
		if ( !m_clPlayer.canUpdateMoney( (uint8)pbMoney.moneytype(), (int64)pbMoney.money() ) )
		{
			uCode = R_ResultMail_MoneyLimit;
			return false;
		}
	}

	for ( int32 i = 0 ; i < (int32)stMail.item_size() ;++i  )
	{
		bool bBind = _CHECK_BIT(stMail.item(i).flag(),_BIT32(ItemFlag_Binded));
		m_clPlayer.getGameItemMgr().addItem( stMail.item(i).itemid(),stMail.item(i).itemnum(),bBind, true,false, DoingType_Mail_Get);
	}
	stMail.clear_item();

	for ( int32 i = 0 ; i < (int32)stMail.money_size() ;++i )
	{
		const PBMoney& pbMoney = stMail.money(i);
		m_clPlayer.addMoney((uint8)pbMoney.moneytype(), (int64)pbMoney.money(), DoingType_Mail_Get);
	}
	stMail.clear_money();

	
	PBMailID ack;
	ack.set_mailid(stMail.mailid());
	m_clPlayer.sendMsg(P_S2C_Protocol_Mail,S2C_Mail_Reward,&ack);

	uCode = R_ResultMail_Succeed;
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool CPlayerMail::handle_ReadMail_Ask(uint8 uEventID, CNetStream& clStream)
{
	PBMailID ask;
	if (!clStream.ParseFromStream(ask))
		return false;

	PBMail* pMail = m_mapMail.find(ask.mailid());
	if (pMail)
		pMail->set_readed(1);

	m_clPlayer.sendMsg(P_S2C_Protocol_Mail,S2C_Mail_Read,&ask,R_ResultMail_Succeed);
	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool CPlayerMail::handle_DeleteMail_Ask(uint8 uEventID, CNetStream& clStream)
{
	PBMailID ask;
	if (!clStream.ParseFromStream(ask))
		return false;

	m_mapMail.release(ask.mailid());
	m_clPlayer.sendMsg(P_S2C_Protocol_Mail,S2C_Mail_Delete,&ask,R_ResultMail_Succeed);
	return true;
}

//-------------------------------------------------------------
//------------------------------ 
bool CPlayerMail::handle_RewardMail_Ask(uint8 uEventID, CNetStream& clStream)
{
	PBRewardMail ask;
	if (!clStream.ParseFromStream(ask))
		return false;

	uint8 uCode = R_ResultMail_Succeed;
	for (int32 i= 0; i < ask.mailid_size(); i++)
	{
		uint32 uMailID = ask.mailid(i);
		if (!uMailID )
			continue;

		PBMail* pMail = m_mapMail.find(uMailID);
		if (pMail)
			reward(*pMail,uCode);

		if ( uCode != R_ResultMail_Succeed )
		{
			PBMailID ack;
			ack.set_mailid(uMailID);
			m_clPlayer.sendMsg(P_S2C_Protocol_Mail,S2C_Mail_Reward,&ack,uCode);
			return false;
		}
	}
	return true;
}
