/*----------------- operators.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2012/6/5 16:39:59
*--------------------------------------------------------------
*��Ӫƽ̨����
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
		_operators_normal	,//����Ӫ
		_operators_uwan		,//����
		_operators_open_qq	,//qq
		_operators_cookie	,//��ʱ��¼
		_operators_9388		,//̨�����
		_operators_37wan	,//37��
		_operators_sogou	,//sogou
		_operators_xunlei_nx,//Ѹ��
		_operators_baidu	,//�ٶ�
		_operators_mokylin	,//����ƽ̨

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
//############################## ƽ̨����Ӧ���� 
//#############################################################
namespace operators
{
	//--- ��ûص��˿�
	uint16	get_call_port	();
	//--- 
	bool	start			(CMutex_*_mutex);
	void	shutdown		();
	//--- 
	bool	connect			(COperatorsResult*_result,uint16 uPort,pc_str pAddress = NULL);
};
