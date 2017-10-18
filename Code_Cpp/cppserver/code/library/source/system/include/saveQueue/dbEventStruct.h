/*----------------- dbEventStruct.h
*
* Copyright (C): 2011  LiuLei
* Author       : 刘雷
* Version      : V1.0
* Date         : 2011/9/5 21:46:20
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/memoryFunctions.h"
#include "stream/streamBase.h"
/*************************************************************/
#pragma pack(push,1)
/*************************************************************/
//------------------------------------------------------
//------------------------------ 事件优先级
enum _em_EventPri
{
	EventPri_Low	,/*低*/ 
	EventPri_Mid	,/*中*/ 
	EventPri_High	,/*高*/ 
	EventPri_Max
};
//------------------------------ 事件标识
enum _em_EventFlag
{
	EventFlag_Read	,
	EventFlag_Write	,
	EventFlag_Max
};
//------------------------------------------------------
//------------------------------ 
struct _stDBEvent
{
	uint8	uEventFlag;	//事件标记
	uint8	uEventPri;	//事件优先级
	uint32	uOrderID;	//序列/操作ID
	union
	{
		struct
		{
			uint8	uEvent;		//事件
			uint8	uSubEvent;	//子事件
			uint32	uObjectID;	//对像ID
		}_EVENT_S;
		uint64	uEventID;
	}_ID_S;

	_stDBEvent()					{	initDBEvent();	}
	_stDBEvent&		getDBEvent	()	{	return *this;	}
	inline void		initDBEvent	()	{	memset(this,0,sizeof(*this));	}

	inline uint64	getEventID	()	{	return _ID_S.uEventID;			}
	inline uint8	getEvent	()	{	return _ID_S._EVENT_S.uEvent;	}
	inline uint8	getSubEvent	()	{	return _ID_S._EVENT_S.uSubEvent;}
	inline uint32	getObjectID	()	{	return _ID_S._EVENT_S.uObjectID;}

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
//------------------------------ DB数据处理块
struct _stDBBlock : _stDBEvent
{
	uint32	uSize;
	char	szData[_IO_BUFFER_];

	_stDBBlock()			{	initialize();					}
	void	initialize()	{	memset(this, 0, sizeof(*this));	}
	bool	write		(const void* pData,uint32 uLen)
	{
		if (!pData || uLen > sizeof(szData))
			return false;

		uSize	= uLen;
		dMemcpy(&szData,sizeof(szData),pData,uLen);
		return true;
	}
};
/*************************************************************/
#pragma pack(pop)
