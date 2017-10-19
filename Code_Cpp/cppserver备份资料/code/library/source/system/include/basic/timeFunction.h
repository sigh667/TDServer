/*----------------- timeFunction.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2012/2/2 10:47:11
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
#include <time.h>
#include <string>
#ifdef WIN32
#include <winsock2.h>//δ�������綨���ͻ
#include <windows.h>
#else //WIN32
#include <sys/time.h>
#endif // WIN32
/*************************************************************/
//--- ���ϵͳ��������
extern uint32	getTickCount	();
//--- ���ϵͳ��������(64λ)
extern uint64	getTickCount64	();
//--- ���ϵͳ��������(64λ)����Ҫ�ֶ����¡�
extern uint64	getSystemTick64	();
//--- ����ϵͳ��������
extern uint64	setSystemTick64	();

//--- ���ϵͳ��ʱ��
extern uint64	getSecond		();
//--- ���ϵͳ����ʱ��
extern uint64	getMillisecond	();
//--- ���ϵͳ΢��ʱ��
extern uint64	getMicroseconds	();
//--- ���ʱ��ʱ��(����)
extern uint64	getClockTime	();

//--- ���ϵͳʱ��,����
extern uint32	getTime			();
//--- ���ϵͳʱ��
extern uint64	setSysTime		();
extern uint64	getSysTime		();
//--- ��ý��տ�ʼʱ��
extern uint64	getDayBeginTime	();
//--- ���ʱ������ڱ�����Ȼ�տ�ʼʱ��
extern uint64	getDayBeginTime	(const uint64& _time);
//--- ���ʱ������ڱ�����ȻСʱ��ʼʱ��
extern uint64	getHourBeginTime	(const uint64& _time);
//--- ���ʱ������ڱ�����Ȼ�ܿ�ʼʱ��
extern uint64	getWeekBeginTime	(const uint64& _time);

//--- ��ñ���ʱ��
extern int32	getLocalTime	(struct tm* _Tm,const uint64* _Time);
extern int32	getLocalTime	(struct tm* _Tm,uint64 _Time);

//--- ���ʱ�����Сʱ
extern int32	diffTimeHour	(uint64 _early,uint64 _late);
//--- ���ʱ���������
extern int32	diffTimeDay		(uint64 _early,uint64 _late);
//--- ���ʱ��������� ����_delay�ӳ�ʱ��
extern int32	diffTimeDay		(uint64 _early,uint64 _late,uint64 _delay);
//--- ���ʱ���������
extern int32	diffTimeWeek	(uint64 _early,uint64 _late);
//--- ���ʱ���������
extern int32	diffTimeMonth	(uint64 _early,uint64 _late);
//--- ʱ���ʽ��(YYYY-MM-DD HH:MM:SS)
extern bool		time_format		(uint64 _time, std::string & st);
//--- ʱ���ʽ��(YYYY-MM-DD HH:MM:SS)
extern uint64	time_format_to	(pc_str _time);
//--- ʱ���ʽ��(YYYYMMDDHHMMSS)
extern bool		time_format_str	(uint64 _time, std::string & st);
//--- ʱ���ʽ��(YYYYMMDDHHMMSS)
extern uint64	time_format_int	(pc_str _time);
//--- ˯��
extern void		dSleep			(uint32 millisecond);

#ifndef WIN32
struct SYSTEMTIME
{
	uint16 wYear;
	uint16 wMonth;
	uint16 wDayOfWeek;
	uint16 wDay;
	uint16 wHour;
	uint16 wMinute;
	uint16 wSecond;
	uint16 wMilliseconds;
};
#endif // WIN32
extern void	getLocalTime(SYSTEMTIME&systime);
