/*----------------- messageStruct.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2011/12/16 14:15:57
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "timer/dateTime.h"
#include "stream/streamBase.h"
/*************************************************************/
//#############################################################
enum _emChannelType_Max
{
	ChannelType_None,

	ChannelType_Max
};
//-------------------------------------------------------------
//------------------------------ ���Խṹ
struct _stBanSpeak
{
	uint16	uWorldID;
	uint32	uPlayerID;
	uint64	uEndTime;

	_stBanSpeak&getBanSpeak	()	{	return *this;					}
	inline void	initBanSpeak()	{	memset(this, 0, sizeof(*this));	}
};
//#############################################################
//#############################################################
//-------------------------------------------------------------
//------------------------------ �㲥��Ϣ
struct _stBroadcast
{
	enum
	{
		Msg_Max = 1024
	};
	uint32		uOrderID;	//�㲥ID
	uint16		uTimeout;	//�㲥���(����30��һ��)
	uint8		uPlace;		//�㲥λ��(_emBroadcastPlaceλ)
	std::string	strMsg;

	_stBroadcast&getBroadcast	()	{	return *this;						}
	inline void	initBroadcast	()
	{
		uOrderID= 0;
		uTimeout= 0;
		uPlace	= 0;
		strMsg.clear();
	}
	inline bool	readBroadcast(CStreamBase&clStream)
	{
		initBroadcast();
		uint16	uLength = 0;
		if(!clStream.read(uOrderID) || !clStream.read(uTimeout) || !clStream.read(uPlace) || !clStream.read(uLength))
			return false;

		if(uLength > Msg_Max)
			uLength = Msg_Max;

		static char szBuff[Msg_Max] = {0};
		if(!clStream.read(uLength,szBuff))
			return false;

		strMsg.append(szBuff,uLength);

		return true;
	}
	inline bool	writeBroadcast(CStreamBase&clStream)
	{
		uint16	uLength = (uint16)strMsg.length();
		if(uLength > Msg_Max)
			uLength = Msg_Max;

		if(!clStream.write(uOrderID) || !clStream.write(uTimeout) || !clStream.write(uPlace) || !clStream.write(uLength))
			return false;

		if(!clStream.write(uLength,strMsg.c_str()))
			return false;

		return true;
	}
};
//-------------------------------------------------------------
//------------------------------ 
struct _stBroadcastInfo	: _stBroadcast
{
	_stDateTime	stDataTime;	//����ʱ��

	_stBroadcastInfo&getBroadcastInfo	()	{	return *this;								}
	inline void		initBroadcastInfo	()	{	initBroadcast();stDataTime.initDataTime();	}
	inline bool		readBroadcastInfo(CStreamBase&clStream)
	{
		initBroadcastInfo();
		if(!readBroadcast(clStream) || !clStream.read(stDataTime))
			return false;

		return true;
	}
	inline bool	writeBroadcastInfo(CStreamBase&clStream)
	{
		if(!writeBroadcast(clStream) || !clStream.write(stDataTime))
			return false;

		return true;
	}
};

//-------------------------------------------------------------
//------------------------------ ���Խṹ��չ������һЩ���ݲ������ֶ�
struct _stBanSpeakEx : _stBanSpeak
{
	std::string strDescrible;				// �ı�˵��

	inline void	initBanSpeakEx()
	{
		strDescrible.clear();
		initBanSpeak();
	}
};
//-------------------------------------------------------------
//------------------------------ �㲥�ṹ��չ������һЩ���ݲ�����json�������ֶ�
struct _stBroadcastInfoEx	: _stBroadcastInfo
{
	uint8		uIsRemove;		//�Ƿ�ɾ��
	uint32		uWorldID;		//��ID
	std::string strPlace;		//�㲥λ��
	std::string	strDateTime;	//����ʱ��

	inline void	initBroadcastInfoEx()
	{
		uIsRemove	= 0;
		uWorldID	= 0;
		strPlace.clear();
		strDateTime.clear();
		initBroadcast();
	}
};
