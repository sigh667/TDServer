/*----------------- open_qq.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/5/14 22:39:39
*--------------------------------------------------------------
* 应用中至少调用v3/user/get_info接口
* openid = string 32个字母，可使用char[64]
* openkey = string 32个字母，可使用char[64]
*
*****接口调用
//http://openapi.tencentyun.com/[api_name]
//http://119.147.19.43/[api_name]
*****
* sig = signature(签名)
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
//############################## 启动配置接口
//#############################################################
namespace open_qq
{
	//--- 初始化配置
	bool	init			(uint32 _appid,pc_str _appkey,pc_str _appname);
	//--- 设置平台验证链接主机及端口
	bool	set_host		(pc_str _host,uint32 _port = 80,bool _tgw = false);
	//--- 开启罗盘系统
	bool	set_compass		(pc_str _host,uint32 _port = 80);
	//--- 设置联盟上报
	bool	set_union_report(pc_str _host,uint32 _port = 80);
	//--- 消费配置
	bool	set_consume		(bool _consume,uint32 _call_port,pc_str _host,uint32 _port);
	
	//--- 是否进行tgw检测
	bool	is_tgw			();
	//--- 是否使用罗盘
	bool	is_compass		();
	//--- 是否联盟上报
	bool	is_union_report	();
};
//#############################################################
//############################## 通用接口
//#############################################################
namespace open_qq
{
	extern uint32		m_appid;
	extern std::string	m_appkey;
	extern std::string	m_appname;
	extern bool			m_init_path;
	extern bool			m_init_path_url;
	//--- 验证主机
	extern std::string	m_host;
	extern uint32		m_port;
	//--- 罗盘
	extern std::string	m_compass_host;
	extern uint32		m_compass_port;
	//--- 联盟上报
	extern std::string	m_union_host;
	extern uint32		m_union_port;
	//--- 回调数据
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

	//--- 登录验证
	bool	login_verify	(url_parser&_param,std::string&_openid,std::string&_openkey,std::string&_pfkey,std::string&_pf,std::string&_seqid,uint16&_pf_u,std::string*_via=NULL);

	//--- 生成gm登陆url信息
	bool	build_login_url	( pc_str _openid,pc_str _openkey , uint16 uServerId ,std::string& url );

	//--- 公共信息头
	bool	build_common	(url_parser&_param,pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip = NULL,pc_str _format = "json");
	//--- 检测回调签名
	bool	result_signature(std::string&_cmd,url_parser&_param);
};
//#############################################################
//############################## 请求指令
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
		/*登录时自带参数
		@_openid	&与APP通信的用户key，跳转到应用首页后，URL后会带该参数字。
		@_openkey	&session key，跳转到应用首页后，URL后会带该参数。
		@_pf		&跳转到应用首页后，URL后会带该参数，表示应用的来源平台。
		@_userip	&玩家登录时的ip
		*/

		//#############################################################
		//##############################用户信息
		//#############################################################
	public:
#include "open_qq_user.h"

		//#############################################################
		//##############################好友关系
		//#############################################################
	public:
		/*
		//--- 验证是否平台好友
		@_fopenid	&待验证的好友QQ号码转化得到的ID
		@[_charset]	&指定请求及响应的字符集，取值为【gbk】或utf-8（只有pf=qqgame或pf=3366时，可以输入该参数）。
		*/
		CResultParse*relation_is_friend	(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip
										,pc_str _fopenid,pc_str _charset = NULL);
		/*【前端可自行获取】
		//--- 获取安装了应用的好友
		@_charset	&指定请求及响应的字符集，取值为【gbk】或utf-8（只有pf=qqgame或pf=3366时，可以输入该参数）。
		*/
		CResultParse*relation_get_app_friends(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip
										,pc_str _charset);

		//#############################################################
		//##############################平台基础功能
		//#############################################################
	public:
		/*
		//--- 验证登录用户是否为某个认证空间的粉丝
		@page_id	&表示认证空间的QQ号码。
		*/
		CResultParse*page_is_fans		(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip
										,pc_str _page_id);

		//#############################################################
		//##############################推广
		//#############################################################
	public:
#include "open_qq_spread.h"

		//#############################################################
		//##############################支付
		//#############################################################
	public:
#include "open_qq_pay.h"

		//#############################################################
		//##############################安全
		//#############################################################
	public:
		//--- 拉取验证码图片
		CResultParse*csec_captcha_get	(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip);
		/*
		//--- 校验图片验证码(为防止恶意注册/暴力破解/灌水等)
		@verifysig	&调用“获取验证码”接口（v3/csec/captcha_get）后的返回参数“filename”。
		@verifycode	&验证码图片中的待验证字符。
		*/
		CResultParse*csec_captcha_verify(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip
									,pc_str _verifysig,pc_str _verifycode);
		/*
		//--- 过滤敏感词
		@content	&待检查是否存在敏感词的文本，必须是utf-8编码，否则会导致敏感词不能被过滤。
		@msgid		&留言编号ID。最大长度64字节。务必保证msgid能唯一对应一条消息，该字段属于保留字段，用于后续程序扩展。
		@[opuid]	&操作用户的UID。UID为应用自身的帐号体系中用户的ID，没有则不传。{例如：用户A对B发起了留言，这里填A的uid}
		@[touid]	&被操作用户的uid。UID为应用自身的帐号体系中用户的ID，没有则不传。{例如：用户A对B发起了留言，这里填B的uid。}
		@[toopenid]	&被操作用户的openid。{例如：用户A对B发起了留言，这里填B的openid。}
		@[actionid]	&操作类型{6：表示留言； 7：表示留言回复； 8：表示其他。}
		*/
		CResultParse*csec_word_filter(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip
								,pc_str _content,pc_str _opuid/* = NULL*/,pc_str _touid/*=NULL*/,pc_str _toopenid/*=NULL*/
								,pc_str _msgid,pc_str actionid/*=NULL*/);
		/*
		//--- 垃圾信息检查【检查用户的发言、邮件、群组名等信息，对垃圾信息（例如广告、脏话）进行识别。】
		@ctype	&表示用户信息输入的途径
				&&1：表示聊天；
				&&2：表示邮件；
				&&3：表示游戏中的角色名；
				&&4：表示其他类型，如sns游戏中的留言板等。
		@content&待检查是否存在垃圾信息的文本。
		*/
		CResultParse*csec_check_spam(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip
								,pc_str _ctype,pc_str _content);

		//#############################################################
		//##############################罗盘
		//#############################################################
	public:
#include "open_qq_compass.h"

		//#############################################################
		//##############################联盟
		//#############################################################
	public:
#include "open_qq_union.h"
	};
};
