/*------------- Crc32.h
* Copyright (C): 2011 Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.01
* Date         : 2011/8/22 9:51:28
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#ifdef WIN32
#include <windows.h>
#include <stdio.h>
/*************************************************************/
namespace crc_32
{
#ifndef _DWORD_DEFINED
#define _DWORD_DEFINED
	typedef unsigned long DWORD;
#endif// !_DWORD_DEFINED
#ifndef _QWORD_DEFINED
#define _QWORD_DEFINED
	typedef long long QWORD, *LPQWORD;
#endif// !_DWORD_DEFINED
#define MAKEQWORD(a, b)	((QWORD)( ((QWORD) ((DWORD) (a))) << 32 | ((DWORD) (b))))
#define LODWORD(l)		((DWORD)(l))
#define HIDWORD(l)		((DWORD)(((QWORD)(l) >> 32) & 0xFFFFFFFF))

	// Read 4K of data at a time (used in the C++ streams, Win32 I/O, and assembly functions)
#define MAX_READ_SIZE	4096		/*每次读取4K数据*/
	// Map a "view" size of 10MB (used in the filemap function)
#define MAX_VIEW_SIZE	10485760
	/**************************************************************
	* 说明 : 动态CRC值
	**************************************************************/
	class CCrc32Dynamic
	{
	protected:
		DWORD*	m_pdwCrc32Table;

	public:
		CCrc32Dynamic();
		virtual~CCrc32Dynamic();

		void			init					();
		void			free					();

		DWORD			stringCrc32			(LPCTSTR szString,	DWORD &dwCrc32)const;
		DWORD			fileCrc32Streams	(LPCTSTR szFilename,DWORD &dwCrc32)const;
		DWORD			fileCrc32Win32		(LPCTSTR szFilename,DWORD &dwCrc32)const;
		DWORD			fileCrc32Filemap	(LPCTSTR szFilename,DWORD &dwCrc32)const;
		DWORD			fileCrc32Assembly	(LPCTSTR szFilename,DWORD &dwCrc32)const;

	protected:
		static bool	getFileSizeQW		(const HANDLE hFile, QWORD &qwSize);
		inline void	calcCrc32			(const BYTE byte, DWORD &dwCrc32)const;
	};
	/**************************************************************
	* 说明 : 静态CRC值
	**************************************************************/
	class CCrc32Static
	{
	protected:
		static DWORD	s_arrdwCrc32Table[256];

	public:
		CCrc32Static()	{}
		virtual~CCrc32Static(){}

	protected:
		static bool			getFileSizeQW(const HANDLE hFile, QWORD &qwSize);
		static inline void	calcCrc32(const BYTE byte, DWORD &dwCrc32);

	public:
		static DWORD	stringCrc32			(LPCTSTR szString,	DWORD &dwCrc32);
		static DWORD	fileCrc32Streams	(LPCTSTR szFilename,DWORD &dwCrc32);
		static DWORD	fileCrc32Win32		(LPCTSTR szFilename,DWORD &dwCrc32);
		static DWORD	fileCrc32Filemap	(LPCTSTR szFilename,DWORD &dwCrc32);
		static DWORD	fileCrc32Assembly	(LPCTSTR szFilename,DWORD &dwCrc32);
		/*----->{ 文件剩余内容CRC值 }*/ 
		static DWORD	fileCrc32			(FILE* pFile,DWORD &dwCrc32);
	};
};/*crc_32*/
#endif//WIN32
