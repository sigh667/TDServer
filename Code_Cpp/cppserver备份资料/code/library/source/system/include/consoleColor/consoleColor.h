/*----------------- consoleColor.h
*
* Copyright (C): 2011  Mokylin°§Mokyqi
* Author       : ’‘Œƒ‘¥
* Version      : V1.0
* Date         : 2012/3/14 18:38:47
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
#ifdef WIN32
#include <windows.h>
#endif // WIN32
/*************************************************************/
class CConsoleColor
{
public:
	enum _emColor
	{
		color_read		,//∫Ï
		color_green		,//¬Ã
		color_yellow	,//ª∆
		color_blue		,//¿∂
		color_intensity	,//∏ﬂ¡¡
		color_max
	};
private:
#ifdef WIN32
	static HANDLE		g_hConsole;
	static WORD			g_fore_color[color_max];
	static WORD			g_back_color[color_max];
#else // WIN32
	static const char*	g_fore_color[color_max];
	static const char*	g_back_color[color_max];
#endif // WIN32

private:
#ifdef WIN32
	WORD		m_wColorSave;
#endif // WIN32

public:
	static bool	initialize	();
	static void	shutdown	();

public:
	CConsoleColor(uint8 _fore,uint8 _bank = 0);
	virtual~CConsoleColor();

public:
	void	setColor(uint8 _fore,uint8 _bank = 0);
};
