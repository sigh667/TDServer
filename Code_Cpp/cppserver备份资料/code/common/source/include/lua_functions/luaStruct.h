/*----------------- luaStruct.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/9/6 9:18:17
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "lua_tinker/luaTinker.h"
/*************************************************************/
// 注册一个protobuf的enum到lua虚拟机
#define _lua_pb_enum(L, _enum)													\
	do																			\
	{																			\
	lua_newtable(L);															\
	for (uint8 i = _enum##_MIN; i <= _enum##_MAX; ++i)							\
		{																		\
		const std::string &s = _enum##_Name((_enum)i);							\
		lua_pushlstring(pL, s.c_str(), s.size()/sizeof(char));					\
		lua_pushnumber(pL, i);													\
		lua_settable(pL, -3);													\
		}																		\
		lua_setglobal(pL, #_enum);												\
	} while (false)

extern void	defLuaStruct		(lua_State* pL);
