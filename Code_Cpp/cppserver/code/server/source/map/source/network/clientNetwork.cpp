/*------------- clientNetwork.cpp
* Copyright (C): 2011  Mokylin¡¤Mokyqi
* Author       : ÕÔÎÄÔ´
* Version      : V1.01
* Date         : 2011/2/24 9:44:56
*
*/ 
/***************************************************************
* 
***************************************************************/
#include "operators.h"

#include "network/clientNetwork.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 
CNetPacket*	CClientNetwork::connect_new	(tcp_context* pContext)
{
	if(!pContext)
		return NULL;

	CClientPacket* _packet = m_poolPack.allocate(pContext);
	if(_packet)
	{
		_packet->initialize();
		_packet->bindContext(pContext);
		
		if(operators::is_operators_type(operators::_operators_open_qq) && open_qq::is_tgw())
			_packet->setConnectCheck(CNetPacket::_CC_Open_QQ);
	}

	return _packet;
}
