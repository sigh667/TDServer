/*----------------- open_qq_result.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/6/7 16:36:07
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "operator_result.h"
#include "basic/stringFunctions.h"
#include "json_c/include/json.h"
#include "open_qq_define.h"
/*************************************************************/
namespace open_qq
{
	//#############################################################
	//##############################
	//#############################################################
	class CResultParse	: public COperatorsResult
	{
	public:
		typedef void (CResultParse::*_result_msg)(int32 _ret,pc_str _msg,json_object*_json);

	protected:
		uint8		m_request_type;
		uint8		m_request_sub_type;
		uint16		m_platform_id;

	private:
		_result_msg*m_result			[_type_max];

	private:
		_result_msg	m_result_user		[_user_max];
		_result_msg	m_result_relation	[_relation_max];
		_result_msg	m_result_page		[_page_max];
		_result_msg	m_result_spread		[_spread_max];
		_result_msg	m_result_pay		[_pay_max];
		_result_msg	m_result_csec		[_csec_max];
		_result_msg	m_result_compass	[_compass_max];
		_result_msg	m_result_union		[_union_max];

	public:
		inline void	set_platform_id	(uint16 _id)			{	m_platform_id = _id;								}
		inline void	set_request		(uint8 _type,uint8 _sub){	m_request_type = _type;m_request_sub_type = _sub;	}

	public:
		CResultParse();
		virtual~CResultParse(){}

	public:
		virtual void	onResult				(pc_str _content,uint32 _length);

	protected://用户信息
		virtual void	result_user_get_info			(int32 _ret,pc_str _msg,json_object*_json){}
		virtual void	result_user_multi_info			(int32 _ret,pc_str _msg,json_object*_json){}
		virtual void	result_user_is_setup			(int32 _ret,pc_str _msg,json_object*_json){}
		virtual void	result_user_is_vip				(int32 _ret,pc_str _msg,json_object*_json){}
		virtual void	result_user_total_vip_info		(int32 _ret,pc_str _msg,json_object*_json){}
		virtual void	result_user_is_login			(int32 _ret,pc_str _msg,json_object*_json){}
		virtual void	result_user_get_app_flag		(int32 _ret,pc_str _msg,json_object*_json){}
		virtual void	result_user_del_app_flag		(int32 _ret,pc_str _msg,json_object*_json){}

	protected://好友关系
		virtual void	result_relation_is_friend		(int32 _ret,pc_str _msg,json_object*_json){}
		virtual void	result_relation_get_app_friends	(int32 _ret,pc_str _msg,json_object*_json){}

	protected://平台基础功能
		virtual void	result_page_is_fans				(int32 _ret,pc_str _msg,json_object*_json){}

	protected://推广
		virtual void	result_spread_verify_invkey		(int32 _ret,pc_str _msg,json_object*_json){}
		//QQGame
		virtual void	result_spread_set_feed			(int32 _ret,pc_str _msg,json_object*_json){}
		virtual void	result_spread_get_feed			(int32 _ret,pc_str _msg,json_object*_json){}
		virtual void	result_spread_set_request		(int32 _ret,pc_str _msg,json_object*_json){}
		virtual void	result_spread_get_request		(int32 _ret,pc_str _msg,json_object*_json){}
		virtual void	result_spread_get_limitation	(int32 _ret,pc_str _msg,json_object*_json){}

	protected://支付
		virtual void	result_pay_buy_goods			(int32 _ret,pc_str _msg,json_object*_json){}
		virtual void	result_pay_confirm_delivery		(int32 _ret,pc_str _msg,json_object*_json){}
		virtual void	result_pay_get_pubacct_balance	(int32 _ret,pc_str _msg,json_object*_json){}
		virtual void	result_pay_get_balance			(int32 _ret,pc_str _msg,json_object*_json){}
		virtual void	result_pay_get_token			(int32 _ret,pc_str _msg,json_object*_json){}
		virtual void	result_pay_has_task				(int32 _ret,pc_str _msg,json_object*_json){}

	protected://安全
		virtual void	result_csec_captcha_get			(int32 _ret,pc_str _msg,json_object*_json){}
		virtual void	result_csec_captcha_verify		(int32 _ret,pc_str _msg,json_object*_json){}
		virtual void	result_csec_word_filter			(int32 _ret,pc_str _msg,json_object*_json){}
		virtual void	result_csec_check_spam			(int32 _ret,pc_str _msg,json_object*_json){}

	protected://罗盘系统
		virtual void	result_compass					(int32 _ret,pc_str _msg,json_object*_json){}

	protected://联盟
		virtual void	result_union					(int32 _ret,pc_str _msg,json_object*_json){}
	};
};
