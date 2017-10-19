/*------------- bufferStream.h
* Copyright (C): 2011 Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.01
* Date         : 2011/8/19 12:08:30
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "streamBase.h"
/*************************************************************/
class CBufferStream	: public CStreamBase
{
protected:
	char*	m_pLocalBuffer;	// 内部缓冲区指针
	char*	m_pBuffer;		// 缓冲区指针
	uint32	m_uBufferSize;	// 缓冲区大小 
	uint32	m_uBufferPos;	// 缓冲区当前位置 

public:
	explicit CBufferStream(char* pBuffer, uint32 uSize);
	explicit CBufferStream();
	virtual~CBufferStream();

public:
	void	initBufferStream		(char* pBuffer,uint32 uSize);
	void	initBufferStream		();

private:// 禁止拷贝
	CBufferStream(const CBufferStream &);
	CBufferStream &operator=(const CBufferStream &);
public:
	inline void		initPosition	()							{	m_uBufferPos = 0;															}
	inline char*	getBuffer		()							{	return m_pBuffer;															}
	inline char*	getSpareBuffer	()							{	return ((m_pBuffer && getSpareSize()) ? (m_pBuffer + getPosition()) : NULL);}
	inline bool		writeStream		(CBufferStream&clStream)	{	return _write(clStream.getPosition(),clStream.getBuffer());					}
	inline bool		writeStreamSpare(CBufferStream&clStream)	{	return _write(clStream.getSpareSize(),clStream.getSpareBuffer());			}

public:
	virtual uint32	getStreamSize	()							{	return m_uBufferSize;														}
	virtual uint32	getPosition		()							{	return m_uBufferPos;														}
	virtual bool	setPosition		(uint32 newPosition);

protected:
	virtual bool	_read			(uint32 uBytes,void* outBuffer);
	virtual bool	_write			(uint32 uBytes,const void*inBuffer);

public:
	virtual uint32	fprintf			(const char* pszFormat,...);
};
