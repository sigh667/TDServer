/*----------------- sogou.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2013/8/27 18:00:52
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/stringFunctions.h"
#include "encrypt/include/encrypts.h"
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS

/*************************************************************/
namespace sogou
{
	extern uint32		m_uGameId;
	extern uint32		m_uTimeout;
	extern uint32		m_uCallPort;
	extern std::string	m_strKey;
	extern std::string	m_host;
	extern uint32		m_port;
	//------------------------------ 
	std::string&get_sogou_key	();
	//--- 获得回调端口
	uint16		get_call_port	();
	//--- 初始化
	bool		init			(uint32 _GameId,pc_str _key,uint32 _timeout,uint16 _call_port);
	//--- 设置链接主机地址(用于角色上报)
	bool		set_host		(pc_str _host,uint32 _port = 80);
	//--- 设置回调key
	void		set_call_key	(pc_str _key);

	//--- 登录验证
	bool		login_verify	(url_parser&_param,std::string&_uname,int8&_adult,uint8*_client=NULL,std::string*_via=NULL,std::string*_adtag=NULL);

	//--- 生成gm登陆url信息
	bool		build_login_url	( pc_str _uname , uint16 uServerId ,uint64 uTime , std::string& url );

//-------------------------------------------------------------
	//--- 签名
	bool		sign_auth		(url_parser&_param,uint64 uTime);
};

//#############################################################
//##############################
//#############################################################
namespace sogou
{
	//接口类型
	enum _em_type
	{
		_type_api		,//api

		_type_max
	};
	//接口子类型
	enum _em_api_type
	{
		_api_roleinfo	,//角色信息上报

		_api_max
	};
	//-------------------------------------------------------------
	pc_str		get_host		(uint8 _type);
	uint16		get_port		(uint8 _type);
	pc_str		get_request_path(uint8 _type,uint8 _sub_type);
};
