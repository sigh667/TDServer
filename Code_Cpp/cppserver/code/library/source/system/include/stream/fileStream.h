/*------------- fileStream.h
* Copyright (C): 2011 Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.01
* Date         : 2011/8/19 11:48:49
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "streamBase.h"
/*************************************************************/
class CFileStream	: public CStreamBase
{
public:
	enum _enAccess
	{
		_Read		,/*��*/ 
		_Write		,/*д*/ 
		_ReadWrite	,/*��д*/ 
		_Append		,/*׷��*/ 
		Access_Max
	};

protected:
	static const char	m_gszAccess[Access_Max][5];

protected:
	FILE*	m_pFile;

public:
	inline bool		isOpen	()	{	return (m_pFile != NULL);	}
	inline FILE*	getFile	()	{	return m_pFile;				}

public:
	CFileStream();
	virtual~CFileStream();

public:
	/*----->{ ���ļ� }*/ 
	bool	open			(const char*pszFileName,_enAccess eAccess);
	/*----->{ ���ļ� }*/ 
	bool	open			(const char*pszFileName,const char* pszMode = "rb");
	/*----->{ �ر� }*/ 
	void	close			();
	//--- ���»�����
	void	flush			();

public:
	/*----->{ ����ļ����� }*/ 
	uint32	getFileLength	();
	/*----->{ ����ļ� }*/ 
	int		clear			();
	/*----->{ ƫ��(�ɹ�����0) }*/ 
	int		seek			(int32 _nOffset, int _Origin = SEEK_SET);

public:
	/*----->{ ���ļ�ͷƫ�� }*/ 
	inline int	seekBegin	(int32 _nOffset)					{	return seek(_nOffset,SEEK_SET);			}
	/*----->{ ���ļ�βƫ�� }*/ 
	inline int	seekEnd		(int32 _nOffset)					{	return seek(_nOffset,SEEK_END);			}
	/*----->{ ���ļ���ǰλ��ƫ�� }*/ 
	inline int	seekCurrent	(int32 _nOffset)					{	return seek(_nOffset,SEEK_CUR);			}

public:
	virtual uint32	getStreamSize	()							{	return getFileLength();					}
	virtual uint32	getPosition		();
	virtual bool	setPosition		(uint32 newPosition)		{	return (seekBegin(newPosition) == 0);	}
	virtual bool	isEof			();
	virtual bool	skipPosition	(int32 _Bytes)				{	return (seekCurrent(_Bytes) == 0);		}

protected:
	virtual bool	_read			(uint32 uBytes,void* outBuffer);
	virtual bool	_write			(uint32 uBytes,const void*inBuffer);

public:
	virtual uint32	fread			(uint32 uMaxBytes,void* outBuffer);
	virtual uint32	fwrite			(uint32 uMaxBytes,const void*inBuffer);
	virtual uint32	fprintf			(const char* pszFormat,...);
};
