/*----------------- open_qq_define.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/6/7 16:32:03
*--------------------------------------------------------------
*接口枚举定义
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
namespace open_qq
{
	//QQ平台ID
	enum _em_platform_id
	{
		_platform_qzone		= 1		,//空间
		_platform_pengyou	= 2		,//朋友
		_platform_qplus		= 4		,//Q+
		_platform_qqgame	= 10	,//QQGame
		_platform_3366		= 11	,//3366
		_platform_website	= 12	,//官网
		_platform_igame		= 16	,//游戏人生
		_platform_union		= 17	,//游戏联盟
		_platform_box		= 23	,//游戏盒子
		_platform_tgp		= 30085	,//游戏平台tgp
	};
	//QQ平台类型
	enum _em_platform_type
	{
		_platform_type_qzone	,//空间
		_platform_type_pengyou	,//朋友
		_platform_type_qplus	,//Q+
		_platform_type_qqgame	,//QQGame
		_platform_type_3366		,//3366
		_platform_type_website	,//官网
		_platform_type_igame	,//游戏人生
		_platform_type_union	,//游戏联盟
		_platform_type_box		,//游戏盒子
		_platform_type_tgp		,//游戏平台tgp

		_platform_type_max
	};

	//接口类型
	enum _em_type
	{
		_type_user		,//用户信息
		_type_relation	,//好友关系
		_type_page		,//平台基础功能
		_type_spread	,//推广
		_type_pay		,//支付/营销
		_type_csec		,//安全
		_type_compass	,//罗盘
		_type_union		,//联盟
		_type_message	,//消息

		_type_max
	};
	//用户信息
	enum _em_user
	{//"v3/user/"
		_user_get_info			,//获取用户信息
		_user_get_multi_info	,//批量获取用户信息
		_user_is_setup			,//验证用户是否安装了应用
		_user_is_vip			,//获取登录用户是否黄钻
		_user_total_vip_info	,//获得登录用户所有VIP信息
		_user_is_login			,//验证登录态&openkey续期
		_user_is_area_login		,//验证用户是否是从选区登录
		_user_friends_vip_info	,//批量获得好友的VIP信息
		_user_get_app_flag		,//用户所在用户群标签接口
		_user_del_app_flag		,//清除用户标志位接口

		_user_max
	};
	//好友关系
	enum _em_relation
	{//"v3/relation/"
		_relation_is_friend			,//验证是否平台好友
		_relation_get_app_friends	,//获取安装了应用的好友

		_relation_max
	};
	//平台基础功能
	enum _em_page
	{//"v3/page/"
		_page_is_fans			,//判断是否认证空间粉丝

		_page_max
	};
	//推广
	enum _em_spread
	{//"v3/spread/"
		_spread_verify_invkey	,//验证好友邀请的invkey

		//QQ提醒
		_spread_is_reminder_set	,//用户是否安装了“QQ提醒”应用，以及是否在“QQ提醒”中开启了提醒
		_spread_set_reminder	,//QQ提醒信息上报接口，可一次上报多条数据

		//QQGame专用
		_spread_set_feed		,//
		_spread_get_feed		,//
		_spread_set_request		,//
		_spread_get_request		,//
		_spread_get_limitation	,//

		_spread_max
	};
	//支付
	enum _em_pay
	{//"v3/pay/"
		//Q点直购道具
		_pay_buy_goods			,//获取交易token
		_pay_confirm_delivery	,//应用发货通知

		//其他支付相关
		_pay_get_pubacct_balance,//查询抵扣券余额
		_pay_get_balance		,//查询游戏币余额

		//营销
		_pay_get_token			,//获取领取道具的token
		_pay_has_task			,//查询是否有可执行任务

		_pay_max
	};

	//安全
	enum _em_csec
	{//"v3/csec/"
		_csec_captcha_get	,//拉取验证码图片
		_csec_captcha_verify,//校验图片验证码
		_csec_word_filter	,//过滤敏感词
		_csec_check_spam	,//垃圾信息检查
		_csec_punish_query	,//反外挂

		_csec_max
	};
	//罗盘
	enum _em_compass
	{//"/stat/"
		_compass_report_login		,//登录
		_compass_report_register	,//主动注册
		_compass_report_accept		,//接受邀请注册
		_compass_report_invite		,//邀请他人注册
		_compass_report_consume		,//支付消费
		_compass_report_recharge	,//支付充值
		_compass_report_quit		,//退出
		_compass_report				,//通用

		_compass_max
	};
	//联盟
	enum _em_union
	{
		_union_register		,//注册

		_union_max
	};
	//消息
	enum _em_message
	{
		//QQGame专用
		_message_send_notification	,//向玩家发送游戏通知

		_message_max
	};

	//#############################################################
	//############################## 返回定义
	//#############################################################
	//公共返回码
	enum _em_common_result
	{
		/*-20 <= ret <= -1: 接口调用不能通过接口代理机校验，需要开发者进行相应的处理*/
		_cr_param_invalid		= -1	,//请求参数无效,详情查看接口
		_cr_appid_not_exist		= -2	,//请求中的appid不存在
		_cr_api_not_right		= -3	,//无API访问权限
		_cr_ip_not_right		= -4	,//ip没有权限
		_cr_sig_fail			= -5	,//参数sig效验失败
		_cr_too_fast			= -6	,//访问频率超限
		_cr_protocol_error		= -7	,//协议不合法（要求必须为https协议的地方，使用了http协议）
		_cr_request_limited		= -8	,//请求受限，通常是安全审计没通过
		_cr_api_no_exist		= -9	,//api不存在

		/*ret <-50: 系统内部错误,联系OpenAPI支持人员，调查问题原因并获得解决方案。*/
		_cr_open_error			= -64	,//openid或者openkey不合法
		_cr_system_busy			= -65	,//系统繁忙导致的连接超时,概率低

		_cr_succeed				= 0		,//正确返回
		/*返回>0调用OpenAPI时发生错误，需要开发者进行相应的处理*/
		_cr_param_error			= 1		,//请求参数错误
		_cr_user_not_opened		= 2		,//用户没有开通对应的平台（朋友、空间、微博...）
		_cr_illegal_operate		= 1000	,//非法操作，通常是进行了不被授权的操作
		_cr_server_busy			= 1001	,//服务器繁忙，通常是后台出错
		_cr_not_login			= 1002	,//用户没有登录态，需要提示前端重新登录
		_cr_account_freeze		= 1003	,//帐户被冻结
		_cr_money_less			= 1004	,//帐户余额不足
		_cr_not_open_pengyou	= 1005	,//用户没有开通腾讯朋友，请先到http://www.pengyou.com/ 开通腾讯朋友
		_cr_not_open_qzone		= 1006	,//用户没有开通QQ空间，请先到http://qzone.qq.com/ 开通QQ空间

		//交易
		_cr_token_timeout		= 1059	,//TOKEN超时
		_cr_token_rollback		= 1060	,//订单已回滚
		_cr_token_commit		= 1060	,//订单已确认
		_cr_token_system_busy	= 1099	,//系统繁忙
		//>>>1100-1200(支付相关错误，预留码段，)
		_cr_cdkey_not_exist		= 1100	,//cdkey不存在
		_cr_cdkey_completed		= 1101	,//用户和cdkey不存在绑定关系，或礼品赠送完毕
		_cr_cdkey_limited		= 1102	,//参加活动受限
		//<<<1100-1200(支付相关错误，预留码段，)
		_cr_user_timeout		= 1300	,//多区选服页面登录验证失败，原因：用户登录记录不存在，或登录已超时

		_cr_max
	};
};
