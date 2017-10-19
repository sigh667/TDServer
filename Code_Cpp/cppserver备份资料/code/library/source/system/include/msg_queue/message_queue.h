/*----------------- message_queue.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/7/12 20:58:24
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "memoryPool/queue_pool.h"
#include "msg_queue/net_message.h"
#include "msg_queue/safe_queue.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------
class message_queue : public pNc1_queue<_stNetMessage>
{
public:
	inline bool		push	(uint32 _apply_key,tcp_context*_context,const char*_buffer,uint16 _size);
};

//-------------------------------------------------------------
//------------------------------ 
inline bool	message_queue::push(uint32 _apply_key,tcp_context*_context,const char*_buffer,uint16 _size)
{
	try
	{
		if(!m_message[message_push])
			return false;

		//增加锁
		CCritLocker clLock(m_message[message_push]->get_mutex());
		if(m_queue_max && m_message[message_push]->size() > m_queue_max)
			return false;

		_stNetMessage*pMsg = m_message[message_push]->push();
		if(!pMsg)
			return false;

		pMsg->reset(_apply_key,_context,_buffer,_size);

		return true;
	}
	catch (...)
	{
	}
	return false;
}

//#############################################################
//##############################
//#############################################################
//-------------------------------------------------------------
//------------------------------ 消息队列处理
#define message_queue_process(_queue,_packet)									\
	{																			\
		_stNetMessage*pMsg = NULL;												\
		_queue.swap_queue();													\
		while(pMsg = _queue.front())											\
		{																		\
			if(pMsg)															\
			{																	\
				if(_packet)														\
				{																\
					(_packet)->set_apply_lock(true);							\
					(_packet)->handlePacket(pMsg->_buffer,pMsg->_size);			\
					(_packet)->set_apply_lock(false);							\
				}																\
			}																	\
			_queue.pop();														\
		}																		\
	}

//-------------------------------------------------------------
//------------------------------ 消息队列处理
#define message_queue_process_multi(_queue,_packet)								\
	{																			\
		_queue.swap_queue();													\
		do																		\
		{																		\
			_stNetMessage*pMsg = _queue.front();								\
			if(NULL != pMsg)													\
			{																	\
				CNetPacket*pPacket = _packet.getPacket(pMsg->_context,pMsg->_apply_key);\
				if(NULL != pPacket)												\
				{																\
					pPacket->handlePacket(pMsg->_buffer,pMsg->_size);			\
					pPacket->set_apply_lock(false);								\
				}																\
			}																	\
			_queue.pop();														\
		}while (!_queue.pop_empty());											\
	}

//-------------------------------------------------------------
//------------------------------ 定义函数
#define message_queue_get_packet(_packet)										\
	public:																		\
	virtual CNetPacket*	getPacket(tcp_context*_context,uint32 _key)				\
	{																			\
		CCritLocker clLock((_packet).getLock());								\
		CNetPacket* pPacket = (_packet).find(_context);							\
		if (pPacket && pPacket->get_apply_key() == _key)						\
		{																		\
			pPacket->set_apply_lock(true);										\
			return pPacket;														\
		}																		\
		return NULL;															\
	}

#define _USE_MESSAGE_QUEUE
