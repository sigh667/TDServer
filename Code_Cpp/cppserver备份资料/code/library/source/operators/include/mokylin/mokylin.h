/*----------------- mokylin.h
*
* Copyright (C): 2013  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2013/10/17 11:54:53
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS

#include "basic/stringFunctions.h"
#include "encrypt/include/encrypts.h"
/*************************************************************/
namespace mokylin
{
	std::string&get_app_id		();
	std::string&get_login_key	();
	std::string&get_call_key	();
	//--- ��ûص��˿�
	uint16		get_call_port	();
	//--- ��ʼ��
	bool		init			(pc_str _app_id,pc_str _login_key,uint32 _timeout,uint16 _call_port);
	//--- ���ûص�key
	void		set_call_key	(pc_str _call_key);

	//--- ��¼��֤
	bool		login_verify	(url_parser&_param,std::string&_uname,int8&_adult,uint8&_client,std::string*_via,std::string*_adtag);

	//--- ����gm��½url��Ϣ
	bool		build_login_url	(pc_str _uname,uint16 _serverid,uint8 _client,uint64 _time,std::string& url);
};
