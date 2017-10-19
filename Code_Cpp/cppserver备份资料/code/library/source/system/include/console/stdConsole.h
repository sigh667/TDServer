/*------------- stdConsole.h
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.01
* Date         : 2011/3/4 15:35:32
*
*/ 
/***************************************************************
*
|ǰ��ɫ					|����ɫ					|��ɫ	|
|FOREGROUND_BLUE		|BACKGROUND_BLUE		|��ɫ	|
|FOREGROUND_GREEN		|BACKGROUND_GREEN		|��ɫ	|
|FOREGROUND_RED			|BACKGROUND_RED			|��ɫ	|
|FOREGROUND_INTENSITY	|BACKGROUND_INTENSITY	|����	|
|��ɫ(��|��):RED|GREEN
***************************************************************/
#pragma once
#include <stdarg.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#endif//WIN32
/*************************************************************/
class CStdConsole
{
protected:
	enum
	{
		Max_Record	= 20	,
		Max_History	= 20	,
		Max_Buffer	= 512	,
	};
protected:
#ifdef WIN32
	HANDLE			m_hInput;									/*������*/ 
	HANDLE			m_hOutput;									/*������*/ 
	HANDLE			m_hError;									/*������*/ 
	WORD			m_wOutputSaveAttributes;					/*����̨�������*/ 
	WORD			m_wOutputCurrAttributes;					/*����̨�������*/ 

private:
	DWORD			m_dwEventNumber;							/*�����¼�����*/ 
	INPUT_RECORD	m_arrInputRecord[Max_Record];				/*�����¼����(ÿ������ȡ20��)*/ 
#endif//WIN32

private:
	int				m_nHistoryIndex;							/*��ǰʹ������*/ 
	int				m_arrInputHistoryLength[Max_History];		/*������ʷ���ݳ�*/ 
	char			m_arrInputHistory[Max_History][Max_Buffer];	/*��ʷ��������*/ 

	int				m_nInputPos;								/*��������*/ 
	char			m_szInputBuffer[Max_Buffer];				/*���뻺��*/ 

	int				m_nOutPos;									/*�������*/ 
	char			m_szOutBuffer[Max_Buffer];					/*�������*/ 

public:
	CStdConsole();
	virtual~CStdConsole();

public:
	/*--->[ ��ʼ�� ]*/
	bool	initialize			();
	/*--->[ �ƽ����� ]*/
	void	process				();

public:
	/*--->[ ʹ��/�������̨ ]*/
	bool	enable				();
	/*--->[ ����/���ٿ���̨ ]*/
	bool	destroy				();
	/*--->[ ���ÿ���̨���� ]*/
	void	setTitle			(const char*pTitle);

protected:
	/*--->[ ����ص����� ]*/
	virtual void	callbackInput(int /*nLength*/,const char* /*pBuffer*/){}
};
