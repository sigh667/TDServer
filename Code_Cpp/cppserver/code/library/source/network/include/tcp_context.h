/*----------------- tcp_context.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/3/26 19:06:34
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "socket.h"
#include "socketBuffer.h"
#include "tcp_thread.h"
#include "libevent.h"
/*************************************************************/
class tcp_network;
class tcp_thread;
class tcp_context	: public CSocketBuffer
{//状态有【链接中|已链接|断开中】
private:
	tcp_network*	_network;
	tcp_thread*		_thread;
	CSocket			_socket;

private:
	volatile bool	_connected;	//是否已完成链接
	volatile bool	_disconnect;//断开中
	volatile uint64	_connect_tick;//链接时间
	volatile uint64	_read_tick;	//读取时间
	volatile uint64	_write_tick;//写入时间

public:
	event			_ev_read;	//读取事件
	event			_ev_write;	//写入事件
	event			_ev_timeout;//超时事件

#ifdef _USE_OBJECT_POOL
	object_declaration(tcp_context);
#endif//_USE_OBJECT_POOL

public:
	friend class tcp_network;

public:
	inline bool			can_send		()const	{	return (_connected && !_disconnect && _socket.is_valid());	}
	inline bool			isConnected		()const	{	return _connected;											}
	inline uint64		getConnectTick	()const	{	return _connect_tick;										}
	inline void			setDisconnect	()		{	_disconnect = true;											}
	inline event_base*	get_base		()		{	return (_thread ? _thread->_base : NULL);					}

public:
	tcp_context	();
	virtual~tcp_context(){}
	void	initialize		(tcp_network* _network = NULL, tcp_thread*_thread = NULL);
	bool	send			(const char* pBuffer,int32 nSize);
	void	disconnect		();
	bool	getDataFluxSwitch();

public:
	ulong			remote_address	();
	const char*		remote_ip		();

private:
	void			clearEvent		();

public:
	virtual void	onConnect		();
	virtual void	onDisconnect	();
};
