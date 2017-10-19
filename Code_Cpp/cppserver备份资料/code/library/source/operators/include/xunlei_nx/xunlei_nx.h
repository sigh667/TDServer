/*----------------- xunlei_nx.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2013/9/9 20:49:26
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/stringFunctions.h"
#include "encrypt/include/encrypts.h"
/*************************************************************/
namespace xunlei_nx
{
	std::string&get_call_key	();
	//--- ��ûص��˿�
	uint16		get_call_port	();
	//--- ��ʼ��
	bool		init			(pc_str _key,uint32 _timeout,uint16 _call_port);
	//--- ���ûص�key
	void		set_call_key	(pc_str _key);
	//--- ��¼��֤
	bool		login_verify	(url_parser&_param,std::string&_uname,int8&_adult,uint8*_client=NULL,std::string*_via=NULL,std::string*_adtag=NULL);

	//--- ����gm��½url��Ϣ
	bool		build_login_url	( pc_str _uname , uint16 uServerId ,uint64 uTime , std::string& url );
};
