/*----------------- 9388.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/9/13 13:57:00
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/stringFunctions.h"
#include "encrypt/include/encrypts.h"
/*************************************************************/
namespace _9388
{
	std::string&get_call_key	();
	//--- 获得回调端口
	uint16		get_call_port	();
	//--- 初始化
	bool		init			(pc_str _key,uint32 _timeout,uint16 _call_port);
	//--- 设置回调key
	void		set_call_key	(pc_str _key);
	//--- 登录验证
	bool		login_verify	(url_parser&_param,std::string&_uname,std::string*_via=NULL);

	//--- 生成gm登陆url信息
	bool		build_login_url	( pc_str _uname , uint16 uServerId ,uint64 uTime , std::string& url );
};
