/*----------------- socketBuffer.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2012/3/12 15:32:38
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif//WIN32
#include "basic/basicTypes.h"
#include "basic/memoryFunctions.h"
#include "basic/stringFunctions.h"
#include "stl/stl_queue.h"
#include "stl/stl_list.h"
#include "memoryPool/memoryPool.h"
/*************************************************************/
//#define _NET_USE_TIME_WRITE
#pragma pack(push,1)
//--- buff
struct CNetBuffer
{
public:
	enum
	{
		BUFFER_MAX		= 60 * 1024		,
		COMPRESS_HEAD	= sizeof(uint16),
	};

#ifdef WIN32
	OVERLAPPED		ol;
	WSABUF			wsaBuffer;
	uint8			IOOperation;
#endif//WIN32
	uint16			uSize;									/*[ ���ݳ��� ]*/ 
	char			szBuffer[BUFFER_MAX + COMPRESS_HEAD];	/*[ ������(+ѹ��ͷ) ]*/ 

#ifdef _USE_OBJECT_POOL
	object_declaration(CNetBuffer);
#endif//_USE_OBJECT_POOL

public:
	inline void		initialize	()		{	memset(this,0,sizeof(*this));	}
#ifdef WIN32
	inline void		initOl		()		{	memset(&ol,0,sizeof(ol));		}
#endif//WIN32
	inline uint16	getIdleSize	()
	{
		if(uSize < BUFFER_MAX)
			return BUFFER_MAX - uSize;

		return 0;
	}
	//--- 
	inline char*	getIdelBuffer()
	{
		if(this->uSize >= BUFFER_MAX)
			return NULL;

		return (this->szBuffer + this->uSize);
	}

	//--- 
	inline uint16	write		(const void* pBuffer,uint32 uLen)
	{
		initialize();
		if (!pBuffer || uLen == 0)
			return 0;

		if (uLen > BUFFER_MAX)
			uSize	= BUFFER_MAX;
		else
			uSize	= (uint16)uLen;

		dMemcpy(szBuffer,BUFFER_MAX,pBuffer,uSize);

		return uSize;
	}
	//--- 
	inline uint16	append		(const void* pBuffer,uint32 uLen)
	{
		if (!pBuffer || uLen == 0 || uSize >= BUFFER_MAX)
			return 0;

		uint16 nIdle = BUFFER_MAX - uSize;
		if (uLen > nIdle)
			uLen = nIdle;

		dMemcpy(szBuffer + uSize,BUFFER_MAX - uSize,pBuffer,uLen);

		uSize += (uint16)uLen;
		return uLen;
	}
	//--- 
	inline void	popSize(uint16 uLen)
	{
		if(this->uSize <= 0 || uLen <= 0 || uLen > BUFFER_MAX)
			return;

		if (this->uSize > uLen)
			this->uSize -= uLen;
		else
			this->uSize = 0;
		if(this->uSize)
			dMemmove(this->szBuffer,BUFFER_MAX,this->szBuffer + uLen,this->uSize);
	}
};
#pragma pack(pop)
//#############################################################
//##############################
//#############################################################
class CSocketBuffer
{
public:
	typedef CMemoryPool<CNetBuffer>		POOL_BUFFER;
	typedef stl_queue<CNetBuffer*>		SEND_QUEUE;

public:
	uint8			m_uCompress;		//ѹ������(0~9)
	uint16			m_uCompressMin;		//��Сѹ��ֵ(>30�ֽ�)
	CNetBuffer		m_stCompress;		//ѹ��������
	CNetBuffer*		m_pSending;			//���ڷ��ͻ�����
	CNetBuffer		m_stRecvBuffer;		//���ջ�����
	SEND_QUEUE		m_sendQueue;		//���Ͷ���
	POOL_BUFFER		m_poolBuffer;		//����������

public:
	CMutex_			m_csLock;
#ifdef _USE_OBJECT_POOL
	CMutex_			m_csLock;
	object_declaration(CSocketBuffer);
#endif//_USE_OBJECT_POOL

public:
	inline void		setCompress		(uint8 _set)	{	m_uCompress = ((_set > 9) ? 9 : _set);		}
	inline void		setCompressMin	(uint16 _set)	{	m_uCompressMin = ((_set < 30) ? 30 : _set);	}
	inline bool		hasSend			()				{	return !(m_sendQueue.empty());				}
	inline uint32	sendCount		()				{	return m_sendQueue.size();					}

public:
	CSocketBuffer();
	virtual~CSocketBuffer();

public:
	void	initBuffer	();
	void	freeSend	();

public:
	/*-1:�ڲ�ʧ��;0=�ɹ�;1=����*/
	int8			sendBuffer		(int32 nSize,const char* pBuffer,uint16 _send_buffer_max);
	void			sendDone		(CNetBuffer*pBuffer);
	CNetBuffer*		getSendBuffer	();

public:
#ifdef _USE_OBJECT_POOL
	//mem_malloc_declaration();
#endif//_USE_OBJECT_POOL

public:
	//--- ���buff��
	CMutex_*		getBufferMutex	();
	//--- ����Buffer
	CNetBuffer*	mallocBuffer	();
	//--- �ͷ�buffer
	void		freeBuffer		(CNetBuffer*pBuffer);
};
