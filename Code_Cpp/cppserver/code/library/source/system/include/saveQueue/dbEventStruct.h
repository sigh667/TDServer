/*----------------- dbEventStruct.h
*
* Copyright (C): 2011  LiuLei
* Author       : ����
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
//------------------------------ �¼����ȼ�
enum _em_EventPri
{
	EventPri_Low	,/*��*/ 
	EventPri_Mid	,/*��*/ 
	EventPri_High	,/*��*/ 
	EventPri_Max
};
//------------------------------ �¼���ʶ
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
	uint8	uEventFlag;	//�¼����
	uint8	uEventPri;	//�¼����ȼ�
	uint32	uOrderID;	//����/����ID
	union
	{
		struct
		{
			uint8	uEvent;		//�¼�
			uint8	uSubEvent;	//���¼�
			uint32	uObjectID;	//����ID
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
//------------------------------ DB���ݴ����
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
