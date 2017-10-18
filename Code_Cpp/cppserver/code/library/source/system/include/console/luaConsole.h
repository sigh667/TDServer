/*----------------- luaConsole.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/9/5 19:41:25
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stdConsole.h"
#include "lua_tinker/luaTinker.h"
#include "threads/condEvent.h"
/*************************************************************/
class CLuaConsole	: public CStdConsole
{
protected:
	lua_State*	m_luaState;

protected:
	CCondEvent	m_clInputEvent;
	int			m_iInputLength;					/*输入内容数据长*/ 
	char		m_szInputBuffer[Max_Buffer];	/*输入内容数据*/ 

public:
	inline lua_State*	getLua()	{	return m_luaState;	}

public:
	CLuaConsole();
	virtual~CLuaConsole();

public:
	bool	luaRegister	();
	void	luaClose	();
	void	luaDoFile	(const char* pFileName);

protected:
	/*--->[ 输入回调处理 ]*/
	void	callbackInput(int nLength,const char* pbuffer);

protected:
	void	inputProcess();
};
