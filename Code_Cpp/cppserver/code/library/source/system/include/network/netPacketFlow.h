/*----------------- netPacketFlow.h
*
* Copyright (C): 2016  Mokylin·Mokyqi
* Author       : 方声辉
* Version      : V1.0
* Date         : 2016/12/19 16:51:24
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "singleton/singleton.h"
#include "stl/std_map.h"

/*************************************************************/

class CNetPacketFlow :_single_public(CNetPacketFlow)
{
public:
	CNetPacketFlow();
	~CNetPacketFlow(){};

	inline uint64	getAllPacketFlowIn()	{	return m_AllPacketFlowIn;	}
	inline uint64   getAllPacketFlowOut()	{ return m_AllPacketFlowOut;	}

	uint64	findPacketFlowIn(uint8 mainID, uint8 subID); 
	uint64	findPacketFlowOut(uint8 mainID, uint8 subID);

	uint64	updataPacketFlowIn(uint8 mainID, uint8 subID, uint64 val);
	uint64	updataPacketFlowOut(uint8 mainID, uint8 subID, uint64 val);
private:
	typedef double_map<uint8,uint8,uint64>		MAIN_SUB_COUNT;

	MAIN_SUB_COUNT  m_packetFlowIn;			// 协议进入数据流量统计
	MAIN_SUB_COUNT  m_packetFlowOut;		// 协议出去数据流量统计

	uint64  m_AllPacketFlowIn;	  
	uint64  m_AllPacketFlowOut;
};

/*************************************************************/
extern CNetPacketFlow* g_packet_flow;