/*----------------- 9388.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
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
	//--- ��ûص��˿�
	uint16		get_call_port	();
	//--- ��ʼ��
	bool		init			(pc_str _key,uint32 _timeout,uint16 _call_port);
	//--- ���ûص�key
	void		set_call_key	(pc_str _key);
	//--- ��¼��֤
	bool		login_verify	(url_parser&_param,std::string&_uname,std::string*_via=NULL);

	//--- ����gm��½url��Ϣ
	bool		build_login_url	( pc_str _uname , uint16 uServerId ,uint64 uTime , std::string& url );
};
