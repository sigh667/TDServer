/*----------------- libevent.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/3/26 18:43:19
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "event2/event.h"
#include "event2/event_struct.h"
#include "event2/listener.h"
#include "event2/thread.h"
#ifdef WIN32
#include "iocp-internal.h"
#endif//WIN32
/*************************************************************/
#define EV_ALL	(EV_TIMEOUT | EV_READ | EV_WRITE | EV_SIGNAL | EV_PERSIST | EV_ET)
//-------------------------------------------------------------
//------------------------------ 根据系统创建IOCP/Epool事件
inline event_base* new_event_base()
{
	struct event_base *base = NULL;
	struct event_config *cfg = event_config_new();
	if (cfg)
	{
#ifdef _EVENT_HAVE_PTHREADS
		evthread_use_pthreads();
#endif//_EVENT_HAVE_PTHREADS
#ifdef _EVENT_HAVE_EPOLL
		event_config_set_flag(cfg,EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST);
#endif//_EVENT_HAVE_EPOLL
#ifdef WIN32
		evthread_use_windows_threads();
		event_config_set_flag(cfg,EVENT_BASE_FLAG_STARTUP_IOCP);
#endif//WIN32
		base = event_base_new_with_config(cfg);
		event_config_free(cfg);
	}
	return base;
}
//-------------------------------------------------------------
//------------------------------ 关闭事件
inline void	stop_event_base(event_base*base)
{
#ifdef WIN32
	if(base)
		event_base_stop_iocp(base);
#endif//WIN32
}
