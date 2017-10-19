/*----------------- messageHandler.h
*
* Copyright (C): 2011  Mokylin°§Mokyqi
* Author       : ¡÷æ‘
* Version      : V1.0
* Date         : 2016/11/21 20:01:36
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
#include "log/log.h"
/*************************************************************/
template<typename TFunc>
class CMessageHandler
{
protected:
	int			m_iSize;
	TFunc		*m_pHandlers;

public:
	explicit	CMessageHandler(int size);
	virtual		~CMessageHandler();

public:
	inline virtual	bool		bind	(int id, TFunc pFunc);
	inline virtual const TFunc	handler	(int id) const;
};

template<typename TFunc>
CMessageHandler<TFunc> :: CMessageHandler(int size)
	: m_iSize(size)
{
	CHECK(size > 0);
	m_pHandlers = new TFunc[m_iSize];
	memset(m_pHandlers, 0, sizeof(TFunc)*m_iSize);
}

template<typename TFunc>
CMessageHandler<TFunc> :: ~CMessageHandler()
{
	safe_delete_array(m_pHandlers);
}

template<typename TFunc>
inline bool CMessageHandler<TFunc> :: bind	(int id, TFunc pFunc)
{
	CHECKF(id >= 0 && id < m_iSize && NULL == m_pHandlers[id]);
	m_pHandlers[id] = static_cast<TFunc>(pFunc);
	return true;
}

template<typename TFunc>
inline const TFunc		CMessageHandler<TFunc> :: handler	(int id) const
{
	CHECKN(id >= 0 && id < m_iSize);
	return m_pHandlers[id];
}