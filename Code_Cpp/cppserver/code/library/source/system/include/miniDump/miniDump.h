/*------------- miniDump.h
* Copyright (C): 2011 Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.01
* Date         : 2011/7/8 17:11:27
*
*/ 
/***************************************************************
* dump��¼�쳣down����Ϣ
***************************************************************/
#pragma once
#ifdef WIN32
#include <iostream>
#include <windows.h>
/*************************************************************/
/*************************************************************/
//�÷�1
//�ڽ�������ʱ�������º���
//::SetUnhandledExceptionFilter(_dumpCatch);
/*************************************************************/
//------------------------------------------------------
//------------------------------ dump�쳣��ȡ
extern LONG WINAPI	_dumpCatch(LPEXCEPTION_POINTERS pExceptionInfo);
//�÷�1
//�ڽ�������ʱ�������º���
//::SetUnhandledExceptionFilter(_dumpCatch);
//------------------------------------------------------

//------------------------------------------------------
//------------------------------ �����쳣dump��Ϣ
extern LONG WINAPI	_createMiniDump(LPEXCEPTION_POINTERS pExceptionInfo);
//�÷�2(�ڲ��رս�������²����쳣dump�ļ�)
//__try
//{
// //����ʹ�ô������������������ṹ
//	...
//}
//__except(_createMiniDump(GetExceptionInformation()))
//{
//	...
//	//_exit(0);
//}
//------------------------------------------------------
#endif//WIN32