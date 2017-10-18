/*----------------- 37wan.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2013/7/8 17:00:44
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS

#include "basic/stringFunctions.h"
#include "encrypt/include/encrypts.h"
/*************************************************************/
namespace _37wan
{
	std::string&get_call_key	();
	//--- 获得回调端口
	uint16		get_call_port	();
	//--- 初始化
	bool		init			(pc_str _key,uint32 _timeout,uint16 _call_port);
	//--- 设置回调key
	void		set_call_key	(pc_str _key);

	//--- 登录验证
	bool		login_verify	(url_parser&_param,std::string&_uname,int8&_adult,uint8*_client=NULL,std::string*_via=NULL,std::string*_adtag=NULL);

	//--- 生成gm登陆url信息
	bool		build_login_url	(pc_str _uname , uint16 uServerId ,uint64 uTime , std::string& url);
};
