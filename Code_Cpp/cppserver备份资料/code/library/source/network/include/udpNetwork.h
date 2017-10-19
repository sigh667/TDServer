/*------------- udpNetwork.h
* Copyright (C): 2011 Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.01
* Date         : 2011/7/15 14:28:54
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include <iostream>
#include "socket.h"
#include "socketStartup.h"
#include "threads/thread.h"
#include "threads/condEvent.h"
#include "platform/platform.h"
/*************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/*************************************************************/
#define UDP_SERVER_WORK_BUFFER_SIZE 2000
/*************************************************************/
//#############################################################
//##############################
//#############################################################
class CUDPNetwork
{
protected:
	uint16			m_usPort;
	bool			m_bStartuped;
	CSocket			m_clSocket;
	CSocketStartup	m_clNetStartup;

protected:
	bool			m_bCreateThread;
	CThread			m_clThread;			//线程
	CCondEvent		m_clKillEvent;		//退出循环事件

public:
	CUDPNetwork();
	virtual~CUDPNetwork();

public:
	/*----->{ 是否已启动 }*/ 
	inline bool	isStartuped	()					{	return m_bStartuped;		}
	inline void	createThread(bool bCreate)		{	m_bCreateThread = bCreate;	}

public:
	/*----->{ 启动 }*/ 
	bool	start	(uint16 usPort);
	/*----->{ 停止 }*/ 
	void	shutdown();
	/*----->{ 发送 }*/ 
	bool	sendTo	(const void*pBuffer,uint32 uSize,const char* pToAddress,uint16 usToPort);
	/*----->{ 发送 }*/ 
	bool	sendTo	(const void*pBuffer,uint32 uSize,ulong ulToAddr,uint16 usToPort);
	/*--->[ 推进处理 ]*/ 
	bool	process	();

protected:
	/*----->{ 错误输出 }*/ 
	virtual void	error		(const char* pString,...);
	/*----->{ 系统错误输出 }*/ 
	virtual void	lastError	(long _error,const char* pString,...);

protected:
	/*----->{ 启动回调 }*/ 
	virtual void	onStartup	()																{}
	/*----->{ 停止回调 }*/ 
	virtual void	onShutdown	()																{}
	/*----->{ 接收回调 }*/ 
	virtual void	onReceive	(char*pBuffer,uint32 uSize,ulong ulFromAddr,uint16 usFromPort)	{}

protected:
	static THREAD_RETURN _networkThread(void* pParam);
};
/*************************************************************/
#ifdef __cplusplus
}
#endif
