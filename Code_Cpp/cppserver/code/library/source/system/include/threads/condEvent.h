/*----------------- condEvent.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/3/20 22:16:16
*--------------------------------------------------------------
*条件事件
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
#ifdef WIN32
#include <windows.h>
#else//linux
#include <sys/types.h>//系统类型定义
#include <signal.h>
#endif//WIN32
#include "csLocker.h"
/*************************************************************/
#ifndef INFINITE
#define INFINITE	0xFFFFFFFF
#endif//INFINITE
class CCondEvent
{
protected:
	bool				m_bCreated;
#ifdef WIN32
	//win32中为事件句柄
	HANDLE				m_hEvent;

#else//linux
	//linux中使用互斥锁与条件变量结合
	pthread_cond_t		m_pthread_cond;
	CMutex_				m_clLocker;
	bool				m_manual_reset;	//手动初始化
	bool				m_signaled;		//是否已触发信号

#endif//WIN32

public:
	CCondEvent();
	~CCondEvent();

public:
	//--- 是否有效
	bool	isValid		();
	//--- 创建事件
	bool	create_event(bool bManualReset,bool bInitialState);
	//--- 关闭事件
	void	close_event	();
	//--- 触发事件
	void	set_event	();
	//--- 复位事件
	void	reset_event	();
	//--- 等待事件[返回false标示超时]
	bool	wait_event	(uint64 milliseconds);
};
