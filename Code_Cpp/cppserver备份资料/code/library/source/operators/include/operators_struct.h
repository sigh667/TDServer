/*----------------- operators_struct.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2012/9/14 20:47:03
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
#include "basic/stringFunctions.h"
#include "basic/memoryFunctions.h"
#include "stream/streamBase.h"
/*************************************************************/
/*************************************************************/
struct _stRechargeBase
{
	uint16		uWorldID;	//������ID
	uint32		uPlayerID;	//��ɫID
	uint32		uMoneyPay;	//֧����
	uint32		uMoneyFree;	//��ѵ�
	uint32		uRmb;		//rmb
	uint64		uTime;		//ʱ��m

	inline bool	read(CStreamBase& stream)
	{
		return stream.read(uWorldID)&&
			stream.read(uPlayerID)&&
			stream.read(uMoneyPay)&&
			stream.read(uMoneyFree)&&
			stream.read(uRmb)&&
			stream.read(uTime);
	}
	inline bool	write(CStreamBase& stream)
	{
		return stream.write(uWorldID)&&
			stream.write(uPlayerID)&&
			stream.write(uMoneyPay)&&
			stream.write(uMoneyFree)&&
			stream.write(uRmb)&&
			stream.write(uTime);
	}

	_stRechargeBase&getRechargeBase()	{	return *this;					}
	inline void		initRechargeBase()	{	dMemset(this,0,sizeof(*this));	}
};
struct _stRecharge : _stRechargeBase
{
	std::string strAccount;	//�ʺ���
	std::string strSN;		//��ˮ��

	_stRecharge&getRecharge()	{	return *this;					}
	inline void	initRecharge()
	{
		initRechargeBase();
		strAccount.clear();
		strSN.clear();
	}
	inline bool	read	(CStreamBase&clStream)
	{
		if(!clStream.read(getRechargeBase()) || !clStream.readString(strAccount) || !clStream.readString(strSN))
			return false;

		return true;
	}
	inline bool	write	(CStreamBase&clStream)
	{
		if(!clStream.write(getRechargeBase()) || !clStream.writeString(strAccount) || !clStream.writeString(strSN))
			return false;

		return true;
	}
};
//****************************************************************
