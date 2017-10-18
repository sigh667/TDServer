/*----------------- netMsgHandler.h
*
* Copyright (C): 2011  Mokylin°§Mokyqi
* Author       : ¡÷æ‘
* Version      : V1.0
* Date         : 2016/11/21 20:01:36
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "common/messageHandler.h"
#include "network/netPacketSrv.h"
#include "network/netPacketClient.h"
/*************************************************************/
//--------------------------------------------------------------------------------------------------------
//						CNetMsgHandler
//--------------------------------------------------------------------------------------------------------
template<typename T>
class CNetMsgHandler : public CMessageHandler<bool (T::*)(_stPacketHead_Svr&, CNetStream&)>
{
protected:
	typedef		CMessageHandler<bool (T::*)		(_stPacketHead_Svr&, CNetStream&)>	Parent;
	typedef		bool (T::*_netMsgHandle)		(_stPacketHead_Svr &pHead, CNetStream& clStream);
	T			*m_pObj;
	inline virtual const _netMsgHandle		handler	(int id) const	{	return Parent::handler(id);	};

public:
	explicit	CNetMsgHandler(int size, T *pObj);
	inline virtual bool	onMessage	(_stPacketHead_Svr &pHead, CNetStream& clStream) const;
};

template<typename T>
CNetMsgHandler<T> :: CNetMsgHandler(int size, T *pObj)
	: Parent(size)
	, m_pObj(pObj)
{
	CHECK(NULL != m_pObj);
}

template<typename T>
inline bool		CNetMsgHandler<T> :: onMessage	(_stPacketHead_Svr &pHead, CNetStream& clStream) const
{
	const auto pFunc = CNetMsgHandler<T>::handler(pHead.uMainProtocol);
	CHECKF(NULL != m_pObj);
	if (NULL == pFunc)
	{
		CLog::error("onMessage fail. [%d-%d]",pHead.uMainProtocol, pHead.uProtocol);
		return false;
	}
	return (m_pObj->*pFunc)(pHead, clStream);
}


//--------------------------------------------------------------------------------------------------------
//						CNetSubMsgHandler
//--------------------------------------------------------------------------------------------------------
template<typename T>
class CNetSubMsgHandler : public CNetMsgHandler<T>
{
public:
	explicit	CNetSubMsgHandler(int size, T *pObj);
	inline virtual bool		onMessage	(_stPacketHead_Svr &pHead, CNetStream& clStream) const;
};

template<typename T>
CNetSubMsgHandler<T> :: CNetSubMsgHandler(int size, T *pObj)
	: CNetMsgHandler<T>(size, pObj)
{
}

template<typename T>
inline bool		CNetSubMsgHandler<T> :: onMessage	(_stPacketHead_Svr &pHead, CNetStream& clStream) const
{
	const auto pFunc = CNetSubMsgHandler<T>::handler(pHead.uProtocol);
	CHECKF(NULL != CNetSubMsgHandler<T>::m_pObj);
	if (NULL == pFunc)
	{
		CLog::error("onMessage fail. [%d-%d]",pHead.uMainProtocol, pHead.uProtocol);
		return false;
	}
	return (CNetSubMsgHandler<T>::m_pObj->*pFunc)(pHead, clStream);
}


//--------------------------------------------------------------------------------------------------------
//						CNetPlayerSubMsgHandler
//--------------------------------------------------------------------------------------------------------
template<typename T>
class CNetPlayerSubMsgHandler : public CMessageHandler<bool (T::*)(_stPacketHead_Svr&, CNetStream&, uint32)>
{
protected:
	typedef		CMessageHandler<bool (T::*)(_stPacketHead_Svr&, CNetStream&, uint32)>	Parent;
	typedef		bool (T::*_netMsgHandle)	(_stPacketHead_Svr &pHead, CNetStream& clStream, uint32 playerid);
	T		*m_pObj;
	inline virtual const _netMsgHandle		handler	(int id) const	{	return Parent::handler(id);	};

public:
	explicit	CNetPlayerSubMsgHandler(int size, T *pObj);
	inline virtual bool		onMessage	(_stPacketHead_Svr &pHead, CNetStream& clStream, uint32 playerid) const;
};

template<typename T>
CNetPlayerSubMsgHandler<T> :: CNetPlayerSubMsgHandler(int size, T *pObj)
	: Parent(size)
	, m_pObj(pObj)
{
	CHECK(NULL != m_pObj);
}

template<typename T>
inline bool		CNetPlayerSubMsgHandler<T> :: onMessage	(_stPacketHead_Svr &pHead, CNetStream& clStream, uint32 playerid) const
{
	const auto pFunc = CNetPlayerSubMsgHandler<T>::handler(pHead.uProtocol);
	CHECKF(NULL != m_pObj);
	if (NULL == pFunc)
	{
		CLog::error("onMessage fail. [%d-%d] playerid[%d]",pHead.uMainProtocol, pHead.uProtocol, playerid);
		return false;
	}
	return (m_pObj->*pFunc)(pHead, clStream, playerid);
}


//--------------------------------------------------------------------------------------------------------
//						CClientMsgHandler
//--------------------------------------------------------------------------------------------------------
template<typename T>
class CClientMsgHandler : public CMessageHandler<bool (T::*)(_stPacketHead_Client&, CNetStream&)>
{
protected:
	typedef		CMessageHandler<bool (T::*)(_stPacketHead_Client&, CNetStream&)>	Parent;
	typedef		bool (T::*_netMsgHandle)	 (_stPacketHead_Client &pHead, CNetStream &clStream);
	T		*m_pObj;
	inline virtual const _netMsgHandle		handler	(int id) const	{	return Parent::handler(id);	};

public:
	explicit	CClientMsgHandler(int size, T *pObj);
	inline virtual bool		onMessage	(_stPacketHead_Client &pHead, CNetStream& clStream) const;
};

template<typename T>
CClientMsgHandler<T> :: CClientMsgHandler(int size, T *pObj)
	: Parent(size)
	, m_pObj(pObj)
{
	CHECK(NULL != m_pObj);
}

template<typename T>
inline bool		CClientMsgHandler<T> :: onMessage	(_stPacketHead_Client &pHead, CNetStream& clStream) const
{
	const auto pFunc = CClientMsgHandler<T>::handler(pHead.uMainProtocol);
	CHECKF(NULL != m_pObj);
	if (NULL == pFunc)
	{
		CLog::error("onClientMessage fail. [%d-%d]",pHead.uMainProtocol, pHead.uProtocol);
		return false;
	}
	return (m_pObj->*pFunc)(pHead, clStream);
}


//--------------------------------------------------------------------------------------------------------
//						CClientSubMsgHandler
//--------------------------------------------------------------------------------------------------------
template<typename T>
class CClientSubMsgHandler : public CClientMsgHandler<T>
{
public:
	explicit	CClientSubMsgHandler(int size, T *pObj);
	inline virtual bool		onMessage	(_stPacketHead_Client &pHead, CNetStream& clStream) const;
};

template<typename T>
CClientSubMsgHandler<T> :: CClientSubMsgHandler(int size, T *pObj)
	: CClientMsgHandler<T>(size, pObj)
{
}

template<typename T>
inline bool		CClientSubMsgHandler<T> :: onMessage	(_stPacketHead_Client &pHead, CNetStream& clStream) const
{
	const auto pFunc = CClientSubMsgHandler<T>::handler(pHead.uProtocol);
	CHECKF(NULL != CClientSubMsgHandler<T>::m_pObj);
	if (NULL == pFunc)
	{
		CLog::error("onClientMessage fail. [%d-%d]",pHead.uMainProtocol, pHead.uProtocol);
		return false;
	}
	return (CClientSubMsgHandler<T>::m_pObj->*pFunc)(pHead, clStream);
}

