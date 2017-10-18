/*----------------- net_message.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/7/12 21:21:04
*--------------------------------------------------------------
*网络消息定义
*------------------------------------------------------------*/
#pragma once
#include "network/include/network.h"
#include "stream/bufferStream.h"
#include "msg_queue/message_queue.h"
/*************************************************************/
#pragma pack(push,1)
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ 网络消息节点
struct _stNetMessage 
{
	uint32		_apply_key;						//引用key
	tcp_context*_context;						//应用上下文
	uint16		_size;							//数据大小
	char		_buffer[CNetBuffer::BUFFER_MAX];//数据内容
	
	inline void		initialize()
	{
		memset(this, 0, sizeof(*this));
	}
	inline void		reset(uint32 _apply_key,tcp_context*_context,const char*_buffer,uint16 _size)
	{
		this->_context	= _context;
		this->_apply_key= _apply_key;
		this->_size		= _size;

		dMemcpy(this->_buffer,CNetBuffer::BUFFER_MAX,_buffer,_size);
	}
};
/*************************************************************/
#pragma pack(pop)
