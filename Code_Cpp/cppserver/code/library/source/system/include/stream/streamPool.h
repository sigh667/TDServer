/*------------- streamPool.h
* Copyright (C): 2014 Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.01
* Date         : 2014/4/6 16:53
*
*/ 
/***************************************************************
* �������б�
***************************************************************/
#pragma once
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS

#include "bufferStream.h"
/*************************************************************/
class CBufferPool	: public CStreamBase
{
private:
	struct _stBuffer
	{
		char*	pBuffer;	/*������ָ��*/ 
		uint32	uBufferSize;/*��������С*/ 
		uint32	uBufferPos;	/*��������ǰλ��*/ 
		void*	_base;		/*���ں����ص�*/
		void(*_funPos)(uint32 _pos,void*_base);

		_stBuffer(char*_buffer,uint32 _size,void*_base,void(*_funPos)(uint32 _pos,void*_base))
		{
			this->pBuffer		= _buffer;
			this->uBufferSize	= _size;
			this->uBufferPos	= 0;
			this->_base			= _base;
			this->_funPos		= _funPos;
		}
	};

private:
	uint32					m_uBufferPos;	/*��������ǰλ��*/ 
	uint32					m_uBufferSize;	/*��������С*/ 
	uint32					m_uPosIndex;	//��ǰ��������������
	_stBuffer*				m_pBufferPos;	//��ǰ������λ��
	stl_vector<_stBuffer>	m_vecBuffer;	//�������б�

public:
	explicit CBufferPool();
	virtual~CBufferPool();

public:
	inline bool		writeStream		(CBufferStream&clStream)	{	return _write(clStream.getPosition(),clStream.getBuffer());			}
	inline bool		writeStreamSpare(CBufferStream&clStream)	{	return _write(clStream.getSpareSize(),clStream.getSpareBuffer());	}

public:
	//--- ���뻺��ڵ�
	/*
	@_bas		д������ʱ�ص�����
	@_funPos	д������ʱ�ص�֪ͨ��������С����ָ��
	*/
	void			poolPush		(char* pBuffer,uint16 uSize,void*_bas=NULL,void(*_funPos)(uint32 _pos,void*_base)=NULL);
	//--- ���ͷ�
	void			poolFree		();
	//--- ��ʼ��λ��
	void			initPosition	();

public:
	virtual uint32	getStreamSize	()							{	return m_uBufferSize;												}
	virtual uint32	getPosition		()							{	return m_uBufferPos;												}
	virtual bool	setPosition		(uint32 newPosition);
	virtual bool	skipPosition	(int32 _Bytes);

protected:
	virtual bool	_read			(uint32 uBytes,void* outBuffer);
	virtual bool	_write			(uint32 uBytes,const void*inBuffer);

private:
	virtual bool	_nextBuffer		();
	virtual	void	_onSetPos		(_stBuffer*pBuffer);
};
