/*----------------- registerStruct.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/9/6 18:00:51
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "registerDefine.h"
#include "basic/memoryFunctions.h"
#include "stream/streamBase.h"
#include "network/netDefine.h"
/*************************************************************/
#pragma pack(push,1)
/*************************************************************/
struct _stRegEvent
{
	union
	{
		struct
		{
			uint32	uObjectID;	//对像ID
			uint8	uEvent;		//事件
			uint8	uSubEvent;	//子事件
		}_EVENT_S;
		uint64	uEventID;
	}_ID_S;

	_stRegEvent()									{	initEvent();	}
	_stRegEvent&	getRegEvent	()					{	return *this;	}
	inline void		initEvent	()					{	memset(this,0,sizeof(*this));		}

	inline uint64	getEventID	()					{	return _ID_S.uEventID;				}
	inline uint8	getEvent	()					{	return _ID_S._EVENT_S.uEvent;		}
	inline uint8	getSubEvent	()					{	return _ID_S._EVENT_S.uSubEvent;	}
	inline uint32	getObjectID	()					{	return _ID_S._EVENT_S.uObjectID;	}

	inline void		setEvent	(uint8 uEvent)		{	_ID_S._EVENT_S.uEvent	= uEvent;	}
	inline void		setSubEvent	(uint8 uSubEvent)	{	_ID_S._EVENT_S.uSubEvent= uSubEvent;}
	inline void		setObjectID	(uint32 uObjectID)	{	_ID_S._EVENT_S.uObjectID= uObjectID;}

	inline bool		read			(CStreamBase& stream)
	{
		return stream.read(sizeof(*this),this);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return stream.write(sizeof(*this),this);
	}
};

//------------------------------------------------------
//------------------------------ 注册数据处理块
struct _stRegBlock : _stRegEvent
{
	uint32	uSize;
	char	szData[_IO_BUFFER_];

	_stRegBlock()									{	initialize();					}
	void	initialize()							{	memset(this, 0, sizeof(*this));	}
	bool	write		(const void* pData,uint32 uLen)
	{
		if (!pData || uLen > sizeof(szData))
			return false;

		uSize	= uLen;
		dMemcpy(&szData,sizeof(szData),pData,uLen);
		return true;
	}

	inline bool		read			(CStreamBase& stream)
	{
		return stream.read(sizeof(*this),this);
	}
	inline bool		write			(CStreamBase& stream)
	{
		return stream.write(sizeof(*this),this);
	}
};
//****************************************************************
#pragma pack(pop)
