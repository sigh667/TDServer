/*----------------- log.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/8/20 18:15:18
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include <iostream>
#include <ctime>
#include "basic/basicTypes.h"
#include "basic/timeFunction.h"
#include "threads/csLocker.h"
#include "stream/fileStream.h"
#include "consoleColor/consoleColor.h"
#include "threads/thread.h"
#include "threads/condEvent.h"
#include "singleton/singleton.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <list>

/*************************************************************/
class CLog
{
public:
	enum _e_LogType
	{
		LT_Log	,
		LT_Warn	,
		LT_Error,
		LT_Max
	};

private:
	static bool				m_bInitiate;
	static uint16			m_uPathID;
	static SYSTEMTIME		m_gSysTime[LT_Max];
	static char				m_szDirectory[256];
	static char				m_szBuffer[1024 * 8];
	static CMutex_			m_csLock;
	static CFileStream		m_clgFile[LT_Max];	/*文件*/ 
	static uint32			m_uMask;			// 日志采集标识

public:
	//--- 初始化
	static bool	initialize	(uint16 uPathID = 0, uint32 uMask = uint32(-1));
	//--- 关闭
	static void	shutdown	(); 
		
private:
	static void	createAFile	(uint8 ucType);
	static void	getDate		(char* pDate,const uint32& uSize);

private:
	static void	_print		(const char* pFormat,va_list&argptr);

public:
	static void setMaskFlag	(int ilogType, bool bSet);
	static void	setDirectory(const char* pDir, const char * pModName);
	static void	print		(const char* pFormat,...);
	static void	warn		(const char* pFormat,...);
	static void	error		(const char* pFormat,...);
	static void	lastError	(const char* pFormat,...);
};

//-------------------------------------------------------------
//------------------------------ 
class CTickLog
{
protected:
	char		m_szStrint[512];
	bool		m_bEnd;
	uint64		m_uBeginTick;
	uint64*		m_pCount;
	uint64*		m_pTime;

public:
	CTickLog(uint64*pCount,uint64*pTime,char*pszMsg = NULL,...);
	virtual~CTickLog();

public:
	void	onBegin	(char*pszMsg,va_list&argptr);
	void	begin	(char*pszMsg = NULL,...);
	void	end		();
};

#define FUNCTION_BEGIN_CHECK()	\
	/*static uint64 g_Check_Count= 0;\
	static uint64 g_Check_Time = 0;\
	CTickLog clFunctionLog(&g_Check_Count,&g_Check_Time,"FUNCTION_BEGIN_CHECK:%s(%d)",__FILE__,__LINE__);*/


class CThreadLog :_single_public(CThreadLog)
{

private:
	_single_friend(CThreadLog);


	struct SLogBuffer
	{
		int		type;
		char	strData[1024*8];
	};
	enum _e_LogType
	{
		LT_Log	,
		LT_Warn	,
		LT_Error	,
		LT_Max
	};
	typedef std::list<SLogBuffer* > LIST_LOGBUFFER;
	CMutex_					m_csLock;
	LIST_LOGBUFFER			m_lstLog[2];		//日志队列,
	LIST_LOGBUFFER*			m_pLstLog;
	uint32					m_uLogIndex;

	CThread					m_clThread;		//线程对像
	CCondEvent				m_clExitEvent;	//退出循环事件
	uint16					m_uPathID;
	SYSTEMTIME				m_gSysTime[LT_Max];
	char					m_szDirectory[256];
	CFileStream				m_clgFile[LT_Max];	/*文件*/ 
	bool					m_bClose;

public:
	CThreadLog();
	virtual ~CThreadLog()
	{

	};
private:
	SLogBuffer*	_build			(const char* pFormat,va_list&argptr);
	void        _build			(const char* pFormat,va_list&argptr,SLogBuffer& sSLB);
	void		_getDate		(char* pDate,const uint32& uSize);
	void		_tryCreateAFile	(uint8 ucType);

public:
	void	print_PF		(const char* pFormat,...);
	void	warn_PF		(const char* pFormat,...);
	void	error_PF		(const char* pFormat,...);

	void	print_P		(const char* pFormat,...);
	void	warn_P		(const char* pFormat,...);
	void	error_P		(const char* pFormat,...);

	void	print_F		(const char* pFormat,...);
	void	warn_F		(const char* pFormat,...);
	void	error_F		(const char* pFormat,...);

	void	lastError	(const char* pFormat,...);

	bool	startLogThread(uint16 uPathID=1);
	void	stopLogThread();

private:
	static THREAD_RETURN	__thread_proc_write_log(void* pParam);

};

