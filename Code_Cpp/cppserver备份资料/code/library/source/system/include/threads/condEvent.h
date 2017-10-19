/*----------------- condEvent.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2012/3/20 22:16:16
*--------------------------------------------------------------
*�����¼�
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
#ifdef WIN32
#include <windows.h>
#else//linux
#include <sys/types.h>//ϵͳ���Ͷ���
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
	//win32��Ϊ�¼����
	HANDLE				m_hEvent;

#else//linux
	//linux��ʹ�û������������������
	pthread_cond_t		m_pthread_cond;
	CMutex_				m_clLocker;
	bool				m_manual_reset;	//�ֶ���ʼ��
	bool				m_signaled;		//�Ƿ��Ѵ����ź�

#endif//WIN32

public:
	CCondEvent();
	~CCondEvent();

public:
	//--- �Ƿ���Ч
	bool	isValid		();
	//--- �����¼�
	bool	create_event(bool bManualReset,bool bInitialState);
	//--- �ر��¼�
	void	close_event	();
	//--- �����¼�
	void	set_event	();
	//--- ��λ�¼�
	void	reset_event	();
	//--- �ȴ��¼�[����false��ʾ��ʱ]
	bool	wait_event	(uint64 milliseconds);
};
