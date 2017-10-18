/*----------------- gamePlayerFun.cpp
*
* Copyright (C): 2014 Mokylin¡¤Mokyqi
* Author       : ÕÔÎÄÔ´
* Version      : V1.0
* Date         : 2014/20/10 11:23
*--------------------------------------------------------------
* 
*------------------------------------------------------------*/
#include "gameLogic/gamePlayerFun/gamePlayerFun.h"
#include "packet/clientPacket.h"
#include "gameLogic/sceneObject/scenePlayer.h"
#include "main/gameServer.h"
#include "log/log.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
CGamePlayerFun::CGamePlayerFun(CScenePlayer& clPlayer):m_clPlayer(clPlayer)
{
}

//-------------------------------------------------------------
//------------------------------ 
CGamePlayerFun::~CGamePlayerFun()
{
}
//-------------------------------------------------------------
//------------------------------ 
CScenePlayer&	CGamePlayerFun::getPlayer	()
{
	return m_clPlayer;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CGamePlayerFun::netPacket		(uint8 uProtocol,uint8 uEventID,CNetStream&clStream)
{
	MAP_HANDLER::iterator _pos = m_mapHandler.find(uProtocol);
	if ( _pos == m_mapHandler.end() )
		return false;

	_handleSubMsg handler = _pos->second;
	if ( !handler )
		return false;

	return (this->*handler)(uEventID,clStream);
}
//-------------------------------------------------------------
//------------------------------ 
void	CGamePlayerFun::registerHandler		(uint8 uProtocol,_handleSubMsg hHandler)
{
	if (m_mapHandler.find(uProtocol) != m_mapHandler.end())
	{
		const std::string& sFunType = _emGameSystemType_Name(getFunType());
		CLog::error("CGamePlayerFun [%s][%d] handler dup.", sFunType.c_str(), uProtocol);
	}
	m_mapHandler[uProtocol] = hHandler;
}