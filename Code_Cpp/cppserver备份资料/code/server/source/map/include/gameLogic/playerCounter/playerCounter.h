/*------------- playerCounter.h
* Copyright (C): 2016  Mokylin¡¤Mokyqi
* Author       : ³ÂÌÚ
* Version      : V1.01
* Date         : 2016/03/07
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "gameLogic/gamePlayerFun/gamePlayerFun.h"
#include "protocol/PlayerStruct.pb.h"

/*************************************************************/
class CPlayerCounter	: public CGamePlayerFun
{
private:
	typedef	stl_map<uint16, uint64>		MAP_COUNTER;
	MAP_COUNTER		m_mapCounter;

public:
	void			updateCounter			(uint16 uType, int64 nCount);
	uint64			getCounter				(uint16 uType);
	bool			canCounter				(uint16 uType, uint64 uCount);

public:
	CPlayerCounter(CScenePlayer& clPlayer);
	virtual~CPlayerCounter();

public:
	virtual	void	initialize				();
	virtual	bool	loadFromProtobuf		(const PBPlayerData&pbPlayer);
	virtual	bool	saveToProtobuf			(PBPlayerData&pbPlayer);
	virtual	_emGameSystemType	getFunType	()const					{	return GameSystemType_Counter ; }

};
/*************************************************************/
