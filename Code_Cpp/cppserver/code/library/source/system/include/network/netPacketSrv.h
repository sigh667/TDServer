/*----------------- netPacketSrv.h
*
* Copyright (C): 2015  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2015/12/21 11:42:00
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "network/netPacket.h"
/*************************************************************/
class CNetPacketSrv	: public CNetPacket
{
protected:
	typedef CNetPacket Parent;

protected:
	_stSvrInfo	m_stSvrInfo;
public:
	typedef bool (CNetPacketSrv::*_HandleSrvMsg)			(_stPacketHead_Svr &pHead, CNetStream& clStream);
	typedef bool (CNetPacketSrv::*_HandleSrvSubMsg)			(_stPacketHead_Svr &pHead, CNetStream& clStream);
	typedef bool (CNetPacketSrv::*_HandlePlayerSrvSubMsg)	(_stPacketHead_Svr &pHead, CNetStream& clStream, uint32 playerid);

public:
	CNetPacketSrv();
	virtual ~CNetPacketSrv();

public:
	inline	_stSvrInfo&			getSvrInfo	()		{	return m_stSvrInfo; }
public:
	/*----->{ ��ʼ�� }*/ 
	virtual void		initialize			();

	bool				sendMsg				(_stSvrInfo& stFromSrvInfo,_stSvrInfo& stToSrvInfo,uint8 uMainProtocol, uint8 uProtocol, const ::google::protobuf::Message* msg, uint8 uEvent = 0, bool bCheck = true);
	bool				sendBuffer			(_stSvrInfo& stFromSrvInfo,_stSvrInfo& stToSrvInfo,uint8 uMainProtocol, uint8 uProtocol, void* pData, uint16 uSize, uint8 uEvent = 0, bool bCheck = true);
protected:
	/*--->[ �������ṹ���Ͱ�ͷ ]*/
	char*				buildPacketHeadsSrv	(_stSvrInfo& stFromSrvInfo,_stSvrInfo& stToSrvInfo,uint8 uMainProtocol,uint8 uProtocol,uint8 uEventFlag = 0);
protected:
	/*----->{ ���ݰ����� }*/ 
	virtual bool		onPacket			(_stPacketHead &pHead,CNetStream &clStream);
protected:
	/*----->{ ���ݰ����� }*/ 
	virtual bool		onPacketSrv			(_stPacketHead_Svr &pHead,CNetStream &clStream) = 0 ;
};
