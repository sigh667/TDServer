/*----------------- socketStartup.h
*
* Copyright (C): 2011  Mokylin°§Mokyqi
* Author       : ’‘Œƒ‘¥
* Version      : V1.0
* Date         : 2012/3/12 15:26:28
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#ifdef WIN32
#include <winsock2.h>
#endif//WIN32
#include "basic/basicTypes.h"
/*************************************************************/
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus
/*************************************************************/
class CSocketStartup
{
private:
#ifdef WIN32
	WSADATA	m_wsaData;
	int32	m_nWSAInitResult;
#endif//WIN32
	bool	m_bDestructor;

public:
	CSocketStartup(bool bDestructor = false);
	virtual~CSocketStartup();
public:
	bool	startup();
	void	cleanup();
	bool	isStartup();
};
//****************************************************************
#ifdef __cplusplus
}
#endif//__cplusplus
