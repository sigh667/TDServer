/*------------- miniDump.h
* Copyright (C): 2011 Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.01
* Date         : 2011/7/8 17:11:27
*
*/ 
/***************************************************************
* dump记录异常down机信息
***************************************************************/
#pragma once
#ifdef WIN32
#include <iostream>
#include <windows.h>
/*************************************************************/
/*************************************************************/
//用法1
//在进程启动时调用如下函数
//::SetUnhandledExceptionFilter(_dumpCatch);
/*************************************************************/
//------------------------------------------------------
//------------------------------ dump异常获取
extern LONG WINAPI	_dumpCatch(LPEXCEPTION_POINTERS pExceptionInfo);
//用法1
//在进程启动时调用如下函数
//::SetUnhandledExceptionFilter(_dumpCatch);
//------------------------------------------------------

//------------------------------------------------------
//------------------------------ 创建异常dump信息
extern LONG WINAPI	_createMiniDump(LPEXCEPTION_POINTERS pExceptionInfo);
//用法2(在不关闭进程情况下产生异常dump文件)
//__try
//{
// //不能使用带参数且有晰构的类或结构
//	...
//}
//__except(_createMiniDump(GetExceptionInformation()))
//{
//	...
//	//_exit(0);
//}
//------------------------------------------------------
#endif//WIN32
