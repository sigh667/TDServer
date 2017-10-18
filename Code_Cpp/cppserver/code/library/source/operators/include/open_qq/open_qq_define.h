/*----------------- open_qq_define.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2012/6/7 16:32:03
*--------------------------------------------------------------
*�ӿ�ö�ٶ���
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
namespace open_qq
{
	//QQƽ̨ID
	enum _em_platform_id
	{
		_platform_qzone		= 1		,//�ռ�
		_platform_pengyou	= 2		,//����
		_platform_qplus		= 4		,//Q+
		_platform_qqgame	= 10	,//QQGame
		_platform_3366		= 11	,//3366
		_platform_website	= 12	,//����
		_platform_igame		= 16	,//��Ϸ����
		_platform_union		= 17	,//��Ϸ����
		_platform_box		= 23	,//��Ϸ����
		_platform_tgp		= 30085	,//��Ϸƽ̨tgp
	};
	//QQƽ̨����
	enum _em_platform_type
	{
		_platform_type_qzone	,//�ռ�
		_platform_type_pengyou	,//����
		_platform_type_qplus	,//Q+
		_platform_type_qqgame	,//QQGame
		_platform_type_3366		,//3366
		_platform_type_website	,//����
		_platform_type_igame	,//��Ϸ����
		_platform_type_union	,//��Ϸ����
		_platform_type_box		,//��Ϸ����
		_platform_type_tgp		,//��Ϸƽ̨tgp

		_platform_type_max
	};

	//�ӿ�����
	enum _em_type
	{
		_type_user		,//�û���Ϣ
		_type_relation	,//���ѹ�ϵ
		_type_page		,//ƽ̨��������
		_type_spread	,//�ƹ�
		_type_pay		,//֧��/Ӫ��
		_type_csec		,//��ȫ
		_type_compass	,//����
		_type_union		,//����
		_type_message	,//��Ϣ

		_type_max
	};
	//�û���Ϣ
	enum _em_user
	{//"v3/user/"
		_user_get_info			,//��ȡ�û���Ϣ
		_user_get_multi_info	,//������ȡ�û���Ϣ
		_user_is_setup			,//��֤�û��Ƿ�װ��Ӧ��
		_user_is_vip			,//��ȡ��¼�û��Ƿ����
		_user_total_vip_info	,//��õ�¼�û�����VIP��Ϣ
		_user_is_login			,//��֤��¼̬&openkey����
		_user_is_area_login		,//��֤�û��Ƿ��Ǵ�ѡ����¼
		_user_friends_vip_info	,//������ú��ѵ�VIP��Ϣ
		_user_get_app_flag		,//�û������û�Ⱥ��ǩ�ӿ�
		_user_del_app_flag		,//����û���־λ�ӿ�

		_user_max
	};
	//���ѹ�ϵ
	enum _em_relation
	{//"v3/relation/"
		_relation_is_friend			,//��֤�Ƿ�ƽ̨����
		_relation_get_app_friends	,//��ȡ��װ��Ӧ�õĺ���

		_relation_max
	};
	//ƽ̨��������
	enum _em_page
	{//"v3/page/"
		_page_is_fans			,//�ж��Ƿ���֤�ռ��˿

		_page_max
	};
	//�ƹ�
	enum _em_spread
	{//"v3/spread/"
		_spread_verify_invkey	,//��֤���������invkey

		//QQ����
		_spread_is_reminder_set	,//�û��Ƿ�װ�ˡ�QQ���ѡ�Ӧ�ã��Լ��Ƿ��ڡ�QQ���ѡ��п���������
		_spread_set_reminder	,//QQ������Ϣ�ϱ��ӿڣ���һ���ϱ���������

		//QQGameר��
		_spread_set_feed		,//
		_spread_get_feed		,//
		_spread_set_request		,//
		_spread_get_request		,//
		_spread_get_limitation	,//

		_spread_max
	};
	//֧��
	enum _em_pay
	{//"v3/pay/"
		//Q��ֱ������
		_pay_buy_goods			,//��ȡ����token
		_pay_confirm_delivery	,//Ӧ�÷���֪ͨ

		//����֧�����
		_pay_get_pubacct_balance,//��ѯ�ֿ�ȯ���
		_pay_get_balance		,//��ѯ��Ϸ�����

		//Ӫ��
		_pay_get_token			,//��ȡ��ȡ���ߵ�token
		_pay_has_task			,//��ѯ�Ƿ��п�ִ������

		_pay_max
	};

	//��ȫ
	enum _em_csec
	{//"v3/csec/"
		_csec_captcha_get	,//��ȡ��֤��ͼƬ
		_csec_captcha_verify,//У��ͼƬ��֤��
		_csec_word_filter	,//�������д�
		_csec_check_spam	,//������Ϣ���
		_csec_punish_query	,//�����

		_csec_max
	};
	//����
	enum _em_compass
	{//"/stat/"
		_compass_report_login		,//��¼
		_compass_report_register	,//����ע��
		_compass_report_accept		,//��������ע��
		_compass_report_invite		,//��������ע��
		_compass_report_consume		,//֧������
		_compass_report_recharge	,//֧����ֵ
		_compass_report_quit		,//�˳�
		_compass_report				,//ͨ��

		_compass_max
	};
	//����
	enum _em_union
	{
		_union_register		,//ע��

		_union_max
	};
	//��Ϣ
	enum _em_message
	{
		//QQGameר��
		_message_send_notification	,//����ҷ�����Ϸ֪ͨ

		_message_max
	};

	//#############################################################
	//############################## ���ض���
	//#############################################################
	//����������
	enum _em_common_result
	{
		/*-20 <= ret <= -1: �ӿڵ��ò���ͨ���ӿڴ����У�飬��Ҫ�����߽�����Ӧ�Ĵ���*/
		_cr_param_invalid		= -1	,//���������Ч,����鿴�ӿ�
		_cr_appid_not_exist		= -2	,//�����е�appid������
		_cr_api_not_right		= -3	,//��API����Ȩ��
		_cr_ip_not_right		= -4	,//ipû��Ȩ��
		_cr_sig_fail			= -5	,//����sigЧ��ʧ��
		_cr_too_fast			= -6	,//����Ƶ�ʳ���
		_cr_protocol_error		= -7	,//Э�鲻�Ϸ���Ҫ�����ΪhttpsЭ��ĵط���ʹ����httpЭ�飩
		_cr_request_limited		= -8	,//�������ޣ�ͨ���ǰ�ȫ���ûͨ��
		_cr_api_no_exist		= -9	,//api������

		/*ret <-50: ϵͳ�ڲ�����,��ϵOpenAPI֧����Ա����������ԭ�򲢻�ý��������*/
		_cr_open_error			= -64	,//openid����openkey���Ϸ�
		_cr_system_busy			= -65	,//ϵͳ��æ���µ����ӳ�ʱ,���ʵ�

		_cr_succeed				= 0		,//��ȷ����
		/*����>0����OpenAPIʱ����������Ҫ�����߽�����Ӧ�Ĵ���*/
		_cr_param_error			= 1		,//�����������
		_cr_user_not_opened		= 2		,//�û�û�п�ͨ��Ӧ��ƽ̨�����ѡ��ռ䡢΢��...��
		_cr_illegal_operate		= 1000	,//�Ƿ�������ͨ���ǽ����˲�����Ȩ�Ĳ���
		_cr_server_busy			= 1001	,//��������æ��ͨ���Ǻ�̨����
		_cr_not_login			= 1002	,//�û�û�е�¼̬����Ҫ��ʾǰ�����µ�¼
		_cr_account_freeze		= 1003	,//�ʻ�������
		_cr_money_less			= 1004	,//�ʻ�����
		_cr_not_open_pengyou	= 1005	,//�û�û�п�ͨ��Ѷ���ѣ����ȵ�http://www.pengyou.com/ ��ͨ��Ѷ����
		_cr_not_open_qzone		= 1006	,//�û�û�п�ͨQQ�ռ䣬���ȵ�http://qzone.qq.com/ ��ͨQQ�ռ�

		//����
		_cr_token_timeout		= 1059	,//TOKEN��ʱ
		_cr_token_rollback		= 1060	,//�����ѻع�
		_cr_token_commit		= 1060	,//������ȷ��
		_cr_token_system_busy	= 1099	,//ϵͳ��æ
		//>>>1100-1200(֧����ش���Ԥ����Σ�)
		_cr_cdkey_not_exist		= 1100	,//cdkey������
		_cr_cdkey_completed		= 1101	,//�û���cdkey�����ڰ󶨹�ϵ������Ʒ�������
		_cr_cdkey_limited		= 1102	,//�μӻ����
		//<<<1100-1200(֧����ش���Ԥ����Σ�)
		_cr_user_timeout		= 1300	,//����ѡ��ҳ���¼��֤ʧ�ܣ�ԭ���û���¼��¼�����ڣ����¼�ѳ�ʱ

		_cr_max
	};
};
