/*------------- stdConsole.h
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.01
* Date         : 2011/3/4 15:35:32
*
*/ 
/***************************************************************
*
|前景色					|背景色					|颜色	|
|FOREGROUND_BLUE		|BACKGROUND_BLUE		|蓝色	|
|FOREGROUND_GREEN		|BACKGROUND_GREEN		|绿色	|
|FOREGROUND_RED			|BACKGROUND_RED			|红色	|
|FOREGROUND_INTENSITY	|BACKGROUND_INTENSITY	|高亮	|
|黄色(红|绿):RED|GREEN
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
	HANDLE			m_hInput;									/*输入句柄*/ 
	HANDLE			m_hOutput;									/*输出句柄*/ 
	HANDLE			m_hError;									/*错误句柄*/ 
	WORD			m_wOutputSaveAttributes;					/*控制台输出属性*/ 
	WORD			m_wOutputCurrAttributes;					/*控制台输出属性*/ 

private:
	DWORD			m_dwEventNumber;							/*输入事件数量*/ 
	INPUT_RECORD	m_arrInputRecord[Max_Record];				/*输入记录数组(每次最多读取20条)*/ 
#endif//WIN32

private:
	int				m_nHistoryIndex;							/*当前使用索引*/ 
	int				m_arrInputHistoryLength[Max_History];		/*输入历史数据长*/ 
	char			m_arrInputHistory[Max_History][Max_Buffer];	/*历史输入数据*/ 

	int				m_nInputPos;								/*输入坐标*/ 
	char			m_szInputBuffer[Max_Buffer];				/*输入缓冲*/ 

	int				m_nOutPos;									/*输出坐标*/ 
	char			m_szOutBuffer[Max_Buffer];					/*输出缓冲*/ 

public:
	CStdConsole();
	virtual~CStdConsole();

public:
	/*--->[ 初始化 ]*/
	bool	initialize			();
	/*--->[ 推进处理 ]*/
	void	process				();

public:
	/*--->[ 使用/激活控制台 ]*/
	bool	enable				();
	/*--->[ 禁用/销毁控制台 ]*/
	bool	destroy				();
	/*--->[ 设置控制台标题 ]*/
	void	setTitle			(const char*pTitle);

protected:
	/*--->[ 输入回调处理 ]*/
	virtual void	callbackInput(int /*nLength*/,const char* /*pBuffer*/){}
};
