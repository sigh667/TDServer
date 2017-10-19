/*------------- streamPool.h
* Copyright (C): 2014 Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.01
* Date         : 2014/4/6 16:53
*
*/ 
/***************************************************************
* 缓冲流列表
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
		char*	pBuffer;	/*缓冲区指针*/ 
		uint32	uBufferSize;/*缓冲区大小*/ 
		uint32	uBufferPos;	/*缓冲区当前位置*/ 
		void*	_base;		/*用于函数回调*/
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
	uint32					m_uBufferPos;	/*缓冲区当前位置*/ 
	uint32					m_uBufferSize;	/*缓冲区大小*/ 
	uint32					m_uPosIndex;	//当前缓冲区所在索引
	_stBuffer*				m_pBufferPos;	//当前缓冲区位置
	stl_vector<_stBuffer>	m_vecBuffer;	//缓冲区列表

public:
	explicit CBufferPool();
	virtual~CBufferPool();

public:
	inline bool		writeStream		(CBufferStream&clStream)	{	return _write(clStream.getPosition(),clStream.getBuffer());			}
	inline bool		writeStreamSpare(CBufferStream&clStream)	{	return _write(clStream.getSpareSize(),clStream.getSpareBuffer());	}

public:
	//--- 加入缓冲节点
	/*
	@_bas		写入数据时回调对象
	@_funPos	写入数据时回调通知缓冲区大小函数指针
	*/
	void			poolPush		(char* pBuffer,uint16 uSize,void*_bas=NULL,void(*_funPos)(uint32 _pos,void*_base)=NULL);
	//--- 池释放
	void			poolFree		();
	//--- 初始化位置
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
