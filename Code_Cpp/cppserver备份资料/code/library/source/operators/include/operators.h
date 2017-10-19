/*----------------- operators.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/6/5 16:39:59
*--------------------------------------------------------------
*运营平台管理
*------------------------------------------------------------*/
#pragma once
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS

#include "lua_tinker/luaTinker.h"
#include "operator_result.h"
#include "operator_log.h"
#include "uwan/uwan.h"
#include "open_qq/open_qq.h"
#include "cookie/cookie.h"
#include "9388/9388.h"
#include "37wan/37wan.h"
#include "sogou/sogou.h"
#include "xunlei_nx/xunlei_nx.h"
#include "baidu/baidu.h"
#include "mokylin/mokylin.h"
/*************************************************************/
namespace operators
{
	enum
	{
		_operators_normal	,//无运营
		_operators_uwan		,//动网
		_operators_open_qq	,//qq
		_operators_cookie	,//临时登录
		_operators_9388		,//台湾酷玩
		_operators_37wan	,//37玩
		_operators_sogou	,//sogou
		_operators_xunlei_nx,//迅雷
		_operators_baidu	,//百度
		_operators_mokylin	,//接入平台

		_operators_max
	};
	extern operator_log	g_operator_log;

	void	def_lua_script		(lua_State* _lua);
	uint8	get_operators_type	();
	bool	need_start_network	();
	bool	is_operators_type	(uint8 _type);
	void	set_operators_type	(uint8 _type);
	void	set_log_path		(pc_str _path);
	void	set_server_info		(uint8 _type,uint16 _server_id);
	bool	load_operators		(lua_State* _lua);
};
//#############################################################
//############################## 平台链接应答处理 
//#############################################################
namespace operators
{
	//--- 获得回调端口
	uint16	get_call_port	();
	//--- 
	bool	start			(CMutex_*_mutex);
	void	shutdown		();
	//--- 
	bool	connect			(COperatorsResult*_result,uint16 uPort,pc_str pAddress = NULL);
};
