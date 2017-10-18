/*------------- playerCounter.cpp
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 陈腾
* Version      : V1.01
* Date         : 2016/03/07
*
*/ 
/***************************************************************
* 
***************************************************************/
#include "gameLogic/playerCounter/playerCounter.h"
#include "gameLogic/sceneObject/scenePlayer.h"

/*************************************************************/
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
CPlayerCounter::CPlayerCounter(CScenePlayer& clPlayer):CGamePlayerFun(clPlayer)
{

}

//-------------------------------------------------------------
//------------------------------ 
CPlayerCounter::~CPlayerCounter()
{
}
//-------------------------------------------------------------
//------------------------------ 
void	CPlayerCounter::initialize			()
{
	CGamePlayerFun::initialize();
	m_mapCounter.clear();
}
//-------------------------------------------------------------
//------------------------------ 
bool	CPlayerCounter::loadFromProtobuf	(const PBPlayerData&pbPlayer)
{
	const PBSystemCounter& pSystem = pbPlayer.playersystem().counter();
	for (int32 i = 0; i < pSystem.pbcounter_size(); ++i)
	{
		const PBCounter& counter = pSystem.pbcounter(i);
		m_mapCounter[counter.id()] = counter.count();
	}
	return true;
}
//-------------------------------------------------------------
//------------------------------ 
bool	CPlayerCounter::saveToProtobuf		(PBPlayerData&pbPlayer)
{
	PBSystemCounter* pSystem = pbPlayer.mutable_playersystem()->mutable_counter();
	if (!pSystem)
		return false;

	pSystem->clear_pbcounter();
	MAP_COUNTER::iterator _pos = m_mapCounter.begin();
	MAP_COUNTER::iterator _end = m_mapCounter.end();
	for (; _pos != _end; ++_pos)
	{
		PBCounter* pCounter = pSystem->add_pbcounter();
		if (!pCounter)
			continue;

		pCounter->set_id(_pos->first);
		pCounter->set_count(_pos->second);
	}
	return true;
}

//-------------------------------------------------------------
//------------------------------ 更新计数器
void CPlayerCounter::updateCounter(uint16 uType, int64 nCount)
{
	uint64 uCurCount = getCounter(uType);
	uint64 uNowCount = uCurCount + nCount;
	if (uCurCount + nCount < 0)
		uNowCount = 0;
	m_mapCounter[uType] = uNowCount;

	PBG2CCounter ack;
	PBCounter* pCounter = ack.add_counter();
	if (pCounter)
	{
		pCounter->set_id(uType);
		pCounter->set_count(uNowCount);
		m_clPlayer.sendMsg(P_S2C_Protocol_Counter,S2C_Counter_Update,&ack);
	}
}

//-------------------------------------------------------------
//------------------------------ 获取计数值
uint64 CPlayerCounter::getCounter(uint16 uType)
{
	MAP_COUNTER::iterator it = m_mapCounter.find(uType);
	if (it != m_mapCounter.end())
		return it->second;

	return 0;
}

//-------------------------------------------------------------
//------------------------------ 获取计数值
bool CPlayerCounter::canCounter	(uint16 uType, uint64 uCount)
{
	uint64 uCurCount = getCounter(uType);
	return uCurCount >= uCount;
}
