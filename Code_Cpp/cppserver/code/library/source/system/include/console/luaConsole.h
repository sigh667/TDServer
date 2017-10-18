/*----------------- luaConsole.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
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
	int			m_iInputLength;					/*�����������ݳ�*/ 
	char		m_szInputBuffer[Max_Buffer];	/*������������*/ 

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
	/*--->[ ����ص����� ]*/
	void	callbackInput(int nLength,const char* pbuffer);

protected:
	void	inputProcess();
};
