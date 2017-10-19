/*----------------- net_message.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2012/7/12 21:21:04
*--------------------------------------------------------------
*������Ϣ����
*------------------------------------------------------------*/
#pragma once
#include "network/include/network.h"
#include "stream/bufferStream.h"
#include "msg_queue/message_queue.h"
/*************************************************************/
#pragma pack(push,1)
/*************************************************************/
//-------------------------------------------------------------
//------------------------------ ������Ϣ�ڵ�
struct _stNetMessage 
{
	uint32		_apply_key;						//����key
	tcp_context*_context;						//Ӧ��������
	uint16		_size;							//���ݴ�С
	char		_buffer[CNetBuffer::BUFFER_MAX];//��������
	
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
