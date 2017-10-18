/*----------------- uwan.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/4/23 15:54:37
*--------------------------------------------------------------
*动网接口
*------------------------------------------------------------*/
#pragma once
#include "basic/stringFunctions.h"
#include "encrypt/include/encrypts.h"
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS

/*************************************************************/
namespace uwan
{
	//--- 初始化
	bool	init			(uint32 _GameId,pc_str _key,uint32 _timeout);
	bool	set_host		(pc_str _host,uint32 _port = 80);

	//--- 登录验证
	bool	login_verify	(url_parser&_param,std::string&_uname,uint32&_userid,int8&_al);

	//--- 生成gm登陆url信息
	bool	build_login_url	( pc_str _uname , uint16 uServerId ,uint64 uTime , std::string& url );
};
