/*------------- player_inline.h
* Copyright (C): 2011  
* Author       : утндт╢
* Version      : V1.01
* Date         : 2011/3/23 13:32:35
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
inline void	CScenePlayer::setClientPacket	(CClientPacket*pPacket)
{
	m_pPacket		= pPacket;

	if(m_pPacket)
		resetGhost();
}
//-------------------------------------------------------------
//------------------------------ 
inline bool	CScenePlayer::testPlayerMark		(uint8 uMark)const
{
	return m_stPlayerBase.testPlayerMark(uMark);
}

//-------------------------------------------------------------
//------------------------------ 
inline void	CScenePlayer::setPlayerMark		(uint8 uMark,bool bSet)
{
	m_stPlayerBase.setPlayerMark(uMark,bSet);
}
//-------------------------------------------------------------
//------------------------------ 
inline const std::string CScenePlayer::getAnsiName()	const
{
	std::string name;
	_convertUTF8toANSI(m_stPlayerBase.szPlayerName, name);
	return name;
}