/*----------------- open_qq.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2012/5/14 22:39:39
*--------------------------------------------------------------
* Ӧ�������ٵ���v3/user/get_info�ӿ�
* openid = string 32����ĸ����ʹ��char[64]
* openkey = string 32����ĸ����ʹ��char[64]
*
*****�ӿڵ���
//http://openapi.tencentyun.com/[api_name]
//http://119.147.19.43/[api_name]
*****
* sig = signature(ǩ��)
*****
*pf
pengyou
website
qqgame
3366
qzone
*****
platform = website
*------------------------------------------------------------*/
#pragma once
#include "basic/stringFunctions.h"
#include "basic/timeFunction.h"
#include "stl/std_hash_map.h"
#include "open_qq_define.h"
#include "encrypt/include/encrypts.h"
#include "operator_result.h"
/*************************************************************/
//#############################################################
//############################## �������ýӿ�
//#############################################################
namespace open_qq
{
	//--- ��ʼ������
	bool	init			(uint32 _appid,pc_str _appkey,pc_str _appname);
	//--- ����ƽ̨��֤�����������˿�
	bool	set_host		(pc_str _host,uint32 _port = 80,bool _tgw = false);
	//--- ��������ϵͳ
	bool	set_compass		(pc_str _host,uint32 _port = 80);
	//--- ���������ϱ�
	bool	set_union_report(pc_str _host,uint32 _port = 80);
	//--- ��������
	bool	set_consume		(bool _consume,uint32 _call_port,pc_str _host,uint32 _port);
	
	//--- �Ƿ����tgw���
	bool	is_tgw			();
	//--- �Ƿ�ʹ������
	bool	is_compass		();
	//--- �Ƿ������ϱ�
	bool	is_union_report	();
};
//#############################################################
//############################## ͨ�ýӿ�
//#############################################################
namespace open_qq
{
	extern uint32		m_appid;
	extern std::string	m_appkey;
	extern std::string	m_appname;
	extern bool			m_init_path;
	extern bool			m_init_path_url;
	//--- ��֤����
	extern std::string	m_host;
	extern uint32		m_port;
	//--- ����
	extern std::string	m_compass_host;
	extern uint32		m_compass_port;
	//--- �����ϱ�
	extern std::string	m_union_host;
	extern uint32		m_union_port;
	//--- �ص�����
	extern std::string	m_consume_host;
	extern uint32		m_consume_port;
	extern bool			m_consume;
	extern uint32		m_consume_call_port;

	uint32	get_appid			();
	uint16	get_platform_type_id (uint8 _type);
	pc_str	get_platform_type_str(uint8 _type);

	uint16	get_platform_id		(pc_str _platform);

	bool	is_consume			();
	uint32	get_consume_port	();

	//--- ��¼��֤
	bool	login_verify	(url_parser&_param,std::string&_openid,std::string&_openkey,std::string&_pfkey,std::string&_pf,std::string&_seqid,uint16&_pf_u,std::string*_via=NULL);

	//--- ����gm��½url��Ϣ
	bool	build_login_url	( pc_str _openid,pc_str _openkey , uint16 uServerId ,std::string& url );

	//--- ������Ϣͷ
	bool	build_common	(url_parser&_param,pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip = NULL,pc_str _format = "json");
	//--- ���ص�ǩ��
	bool	result_signature(std::string&_cmd,url_parser&_param);
};
//#############################################################
//############################## ����ָ��
//#############################################################
namespace open_qq
{
	class CResultParse;
	class CRequest	: public COperatorsResultMgr
	{
	public:
		virtual CResultParse*	newResultPacket() = 0;

	protected:
		CResultParse*send_request_url(uint8 _type,uint8 sub_type,url_parser&_sig_param,url_parser*_other_param = NULL,bool _sig = true);


	public:
		/*��¼ʱ�Դ�����
		@_openid	&��APPͨ�ŵ��û�key����ת��Ӧ����ҳ��URL�����ò����֡�
		@_openkey	&session key����ת��Ӧ����ҳ��URL�����ò�����
		@_pf		&��ת��Ӧ����ҳ��URL�����ò�������ʾӦ�õ���Դƽ̨��
		@_userip	&��ҵ�¼ʱ��ip
		*/

		//#############################################################
		//##############################�û���Ϣ
		//#############################################################
	public:
#include "open_qq_user.h"

		//#############################################################
		//##############################���ѹ�ϵ
		//#############################################################
	public:
		/*
		//--- ��֤�Ƿ�ƽ̨����
		@_fopenid	&����֤�ĺ���QQ����ת���õ���ID
		@[_charset]	&ָ��������Ӧ���ַ�����ȡֵΪ��gbk����utf-8��ֻ��pf=qqgame��pf=3366ʱ����������ò�������
		*/
		CResultParse*relation_is_friend	(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip
										,pc_str _fopenid,pc_str _charset = NULL);
		/*��ǰ�˿����л�ȡ��
		//--- ��ȡ��װ��Ӧ�õĺ���
		@_charset	&ָ��������Ӧ���ַ�����ȡֵΪ��gbk����utf-8��ֻ��pf=qqgame��pf=3366ʱ����������ò�������
		*/
		CResultParse*relation_get_app_friends(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip
										,pc_str _charset);

		//#############################################################
		//##############################ƽ̨��������
		//#############################################################
	public:
		/*
		//--- ��֤��¼�û��Ƿ�Ϊĳ����֤�ռ�ķ�˿
		@page_id	&��ʾ��֤�ռ��QQ���롣
		*/
		CResultParse*page_is_fans		(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip
										,pc_str _page_id);

		//#############################################################
		//##############################�ƹ�
		//#############################################################
	public:
#include "open_qq_spread.h"

		//#############################################################
		//##############################֧��
		//#############################################################
	public:
#include "open_qq_pay.h"

		//#############################################################
		//##############################��ȫ
		//#############################################################
	public:
		//--- ��ȡ��֤��ͼƬ
		CResultParse*csec_captcha_get	(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip);
		/*
		//--- У��ͼƬ��֤��(Ϊ��ֹ����ע��/�����ƽ�/��ˮ��)
		@verifysig	&���á���ȡ��֤�롱�ӿڣ�v3/csec/captcha_get����ķ��ز�����filename����
		@verifycode	&��֤��ͼƬ�еĴ���֤�ַ���
		*/
		CResultParse*csec_captcha_verify(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip
									,pc_str _verifysig,pc_str _verifycode);
		/*
		//--- �������д�
		@content	&������Ƿ�������дʵ��ı���������utf-8���룬����ᵼ�����дʲ��ܱ����ˡ�
		@msgid		&���Ա��ID����󳤶�64�ֽڡ���ر�֤msgid��Ψһ��Ӧһ����Ϣ�����ֶ����ڱ����ֶΣ����ں���������չ��
		@[opuid]	&�����û���UID��UIDΪӦ��������ʺ���ϵ���û���ID��û���򲻴���{���磺�û�A��B���������ԣ�������A��uid}
		@[touid]	&�������û���uid��UIDΪӦ��������ʺ���ϵ���û���ID��û���򲻴���{���磺�û�A��B���������ԣ�������B��uid��}
		@[toopenid]	&�������û���openid��{���磺�û�A��B���������ԣ�������B��openid��}
		@[actionid]	&��������{6����ʾ���ԣ� 7����ʾ���Իظ��� 8����ʾ������}
		*/
		CResultParse*csec_word_filter(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip
								,pc_str _content,pc_str _opuid/* = NULL*/,pc_str _touid/*=NULL*/,pc_str _toopenid/*=NULL*/
								,pc_str _msgid,pc_str actionid/*=NULL*/);
		/*
		//--- ������Ϣ��顾����û��ķ��ԡ��ʼ���Ⱥ��������Ϣ����������Ϣ�������桢�໰������ʶ�𡣡�
		@ctype	&��ʾ�û���Ϣ�����;��
				&&1����ʾ���죻
				&&2����ʾ�ʼ���
				&&3����ʾ��Ϸ�еĽ�ɫ����
				&&4����ʾ�������ͣ���sns��Ϸ�е����԰�ȡ�
		@content&������Ƿ����������Ϣ���ı���
		*/
		CResultParse*csec_check_spam(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip
								,pc_str _ctype,pc_str _content);

		//#############################################################
		//##############################����
		//#############################################################
	public:
#include "open_qq_compass.h"

		//#############################################################
		//##############################����
		//#############################################################
	public:
#include "open_qq_union.h"
	};
};
