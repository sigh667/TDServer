/*------------- bufferStream.h
* Copyright (C): 2011 Mokylin��Mokyqi
* Author       : ����Դ
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
	char*	m_pLocalBuffer;	// �ڲ�������ָ��
	char*	m_pBuffer;		// ������ָ��
	uint32	m_uBufferSize;	// ��������С 
	uint32	m_uBufferPos;	// ��������ǰλ�� 

public:
	explicit CBufferStream(char* pBuffer, uint32 uSize);
	explicit CBufferStream();
	virtual~CBufferStream();

public:
	void	initBufferStream		(char* pBuffer,uint32 uSize);
	void	initBufferStream		();

private:// ��ֹ����
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
