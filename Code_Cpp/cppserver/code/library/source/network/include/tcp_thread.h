/*----------------- tcp_thread.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/6/14 15:05:56
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "threads/thread.h"
#include "threads/condEvent.h"
#include "libevent.h"
/*************************************************************/
class tcp_network;
//--- tcp线程对像
class tcp_thread	: public CThread
{
public:
	uint64		_count;		//被引用次数
	CCondEvent*	_exit;		//退事件
	tcp_network*_network;	//归属对像

	event_base* _base;		//根基事件

public:
	inline void	enter_thread(){	_count++;			}
	inline void	exit_thread	(){	if(_count)_count--;	}

public:
	tcp_thread()
	{
		_count	= 0;
		_exit	= NULL;
		_network= NULL;
		_base	= NULL;
	}
};
