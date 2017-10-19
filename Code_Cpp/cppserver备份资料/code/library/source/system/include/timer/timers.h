/*------------- timers.h
* Copyright (C): 2011 Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.01
* Date         : 2011/8/19 17:06:14
*
*/ 
/***************************************************************
* ��ʱ��
***************************************************************/
#pragma once
#include "libevent/libevent.h"
#include "basic/basicTypes.h"
#include "threads/csLocker.h"
#include "threads/thread.h"
#include "threads/condEvent.h"
#include "stl/std_set.h"
/*************************************************************/
#define _USE_LIBEVENT_TIMEOUT
//------------------------------------------------------
//------------------------------ ��ʱ������
class CTimer;
class CTimerObject
{
protected:
	CTimer*	m_timer;

protected:
	uint64	m_timeout;	//��ʱʱ��
	uint64	m_last_time;//���ʱ��
	uint64	m_last_tick;

#ifdef _USE_LIBEVENT_TIMEOUT
	event	m_ev_timeout;//��ʱ�¼�
#endif//_USE_LIBEVENT_TIMEOUT
	
public:
	friend class CTimer;

public:
	inline	uint64	getLastTime()const 	{	return m_last_time;	}

public:
	CTimerObject();
	virtual~CTimerObject(){};

public:
	virtual void	timerProcess(){};
	virtual void	timerProcess(uint64 uTime,uint64 uTick,uint64 uSecond){};
};

//------------------------------------------------------
//------------------------------ ��ʱ��
class CTimer
{
protected:
	typedef stl_set<CTimerObject*>	OBJECT_LIST;

protected:
	CThread			m_thread;		//��ʱ���߳�
	CCondEvent		m_exit_event;

protected:
#ifdef _USE_LIBEVENT_TIMEOUT
	event_base*		m_event_base;
#endif//_USE_LIBEVENT_TIMEOUT

protected:
	bool			m_shutdown;
	CMutex_			m_mutex;
	OBJECT_LIST		m_object_list;

public:
	inline CMutex_*	get_timer_locket()	{	return &m_mutex;	}

public:
	CTimer();
	virtual~CTimer();

public:
	bool	start_timer	();
	void	stop_timer	();

public:
	bool	add_object	(CTimerObject *_object,long _tick, bool _need_persist=true);
	void	del_object	(CTimerObject *_object);
	void	clear_object();

protected:
	bool	loop_break	();
	void	loop_event	();

private:
	static void				time_object		(CTimerObject* _object,uint64 _time,uint64 _tick);

public:
	static void				on_timeout		(evutil_socket_t fd,short events,void * arg);
	static THREAD_RETURN	_timer_thread_	(void* _param);
};
