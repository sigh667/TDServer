/*----------------- operator_log.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/8/14 10:06:16
*--------------------------------------------------------------
*http日志
*日志格式
*时间 发送大小 发送内容接收大小 接收内容
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
#include "basic/timeFunction.h"
#include "stream/fileStream.h"
#include "timer/timers.h"
/*************************************************************/
class operator_log	: public CTimerObject
{
protected:
	char			m_szFilename[1024];
	char			m_szFileDir[256];
	tm				m_stSysTime;
	CFileStream		m_clFile;

public:
	operator_log();
	virtual~operator_log();

public:
	void	set_file_dir(pc_str _format,...);
	void	create_file	();
	void	close_file	();
	bool	write_log	(std::string&_send,std::string&_recv);

public:
	bool	open_file	(pc_str _file);
	bool	more_log	();

public:
	//--- 主要用来检测是否关闭文件已方便新创建文件
	virtual void	timerProcess();
};
