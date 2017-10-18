/*------------- udpNetwork.h
* Copyright (C): 2011 Mokylin��Mokyqi
* Author       : ����Դ
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
	CThread			m_clThread;			//�߳�
	CCondEvent		m_clKillEvent;		//�˳�ѭ���¼�

public:
	CUDPNetwork();
	virtual~CUDPNetwork();

public:
	/*----->{ �Ƿ������� }*/ 
	inline bool	isStartuped	()					{	return m_bStartuped;		}
	inline void	createThread(bool bCreate)		{	m_bCreateThread = bCreate;	}

public:
	/*----->{ ���� }*/ 
	bool	start	(uint16 usPort);
	/*----->{ ֹͣ }*/ 
	void	shutdown();
	/*----->{ ���� }*/ 
	bool	sendTo	(const void*pBuffer,uint32 uSize,const char* pToAddress,uint16 usToPort);
	/*----->{ ���� }*/ 
	bool	sendTo	(const void*pBuffer,uint32 uSize,ulong ulToAddr,uint16 usToPort);
	/*--->[ �ƽ����� ]*/ 
	bool	process	();

protected:
	/*----->{ ������� }*/ 
	virtual void	error		(const char* pString,...);
	/*----->{ ϵͳ������� }*/ 
	virtual void	lastError	(long _error,const char* pString,...);

protected:
	/*----->{ �����ص� }*/ 
	virtual void	onStartup	()																{}
	/*----->{ ֹͣ�ص� }*/ 
	virtual void	onShutdown	()																{}
	/*----->{ ���ջص� }*/ 
	virtual void	onReceive	(char*pBuffer,uint32 uSize,ulong ulFromAddr,uint16 usFromPort)	{}

protected:
	static THREAD_RETURN _networkThread(void* pParam);
};
/*************************************************************/
#ifdef __cplusplus
}
#endif
