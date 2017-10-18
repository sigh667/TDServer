/*----------------- tcp_context.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
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
{//״̬�С�������|������|�Ͽ��С�
private:
	tcp_network*	_network;
	tcp_thread*		_thread;
	CSocket			_socket;

private:
	volatile bool	_connected;	//�Ƿ����������
	volatile bool	_disconnect;//�Ͽ���
	volatile uint64	_connect_tick;//����ʱ��
	volatile uint64	_read_tick;	//��ȡʱ��
	volatile uint64	_write_tick;//д��ʱ��

public:
	event			_ev_read;	//��ȡ�¼�
	event			_ev_write;	//д���¼�
	event			_ev_timeout;//��ʱ�¼�

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
