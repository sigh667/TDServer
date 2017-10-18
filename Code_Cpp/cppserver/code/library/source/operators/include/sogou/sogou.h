/*----------------- sogou.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
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
	//--- ��ûص��˿�
	uint16		get_call_port	();
	//--- ��ʼ��
	bool		init			(uint32 _GameId,pc_str _key,uint32 _timeout,uint16 _call_port);
	//--- ��������������ַ(���ڽ�ɫ�ϱ�)
	bool		set_host		(pc_str _host,uint32 _port = 80);
	//--- ���ûص�key
	void		set_call_key	(pc_str _key);

	//--- ��¼��֤
	bool		login_verify	(url_parser&_param,std::string&_uname,int8&_adult,uint8*_client=NULL,std::string*_via=NULL,std::string*_adtag=NULL);

	//--- ����gm��½url��Ϣ
	bool		build_login_url	( pc_str _uname , uint16 uServerId ,uint64 uTime , std::string& url );

//-------------------------------------------------------------
	//--- ǩ��
	bool		sign_auth		(url_parser&_param,uint64 uTime);
};

//#############################################################
//##############################
//#############################################################
namespace sogou
{
	//�ӿ�����
	enum _em_type
	{
		_type_api		,//api

		_type_max
	};
	//�ӿ�������
	enum _em_api_type
	{
		_api_roleinfo	,//��ɫ��Ϣ�ϱ�

		_api_max
	};
	//-------------------------------------------------------------
	pc_str		get_host		(uint8 _type);
	uint16		get_port		(uint8 _type);
	pc_str		get_request_path(uint8 _type,uint8 _sub_type);
};
