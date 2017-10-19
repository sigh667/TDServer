/*------------- gameMail.h
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ³ÂÌÚ
* Version      : V1.01
* Date         : 2016/05/04
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "gameLogic/gamePlayerFun/gamePlayerFun.h"
#include "protocol/PlayerStruct.pb.h"
#include "memoryPool/mapPool.h"
/*************************************************************/
class CScenePlayer;

class CPlayerMail	: public CGamePlayerFun
{
private:
	typedef CMapPool<uint32, PBMail, _Val_PB_Init<PBMail> >		MAP_MAIL;

	uint32			m_uMaxID;
	MAP_MAIL		m_mapMail;

public:
	static bool		sendMail				(PBMail& stMail);

public:
	virtual	void	initialize				();
	virtual	bool	loadFromProtobuf		(const PBPlayerData& pbPlayer);
	virtual	bool	saveToProtobuf			(PBPlayerData& pbPlayer);
	virtual	_emGameSystemType	getFunType	()const	{	return GameSystemType_Mail;	}
	virtual bool	handleOfflineData		(uint32 uSignType, CNetStream&clStream);
	
public:
	CPlayerMail(CScenePlayer& clPlayer);
	~CPlayerMail();

private:
	bool			addMail					(PBMail& stMail);
	bool			reward					(PBMail& stMail,uint8& uCode);

private:
	bool			handle_ReadMail_Ask		(uint8 uEventID, CNetStream& clStream);
	bool			handle_DeleteMail_Ask	(uint8 uEventID, CNetStream& clStream);
	bool			handle_RewardMail_Ask	(uint8 uEventID, CNetStream& clStream);
};
