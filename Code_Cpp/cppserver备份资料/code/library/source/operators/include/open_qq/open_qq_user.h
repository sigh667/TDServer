/*----------------- open_qq_user.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2012/8/1 13:56:02
*--------------------------------------------------------------
*�û���Ϣ
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
/*
//--- ��ȡ��¼�û�����Ϣ
@[__charset]&ָ��������Ӧ���ַ�����ȡֵΪ��gbk����utf-8��ֻ��pf=qqgame��pf=3366ʱ����������ò�������
@flag		&pf=qqgameʱ����������ò�����ָ����Ҫ��ȡQQGame�е���Щ��Ϣ
			&&1����Ҫ��ȡ��Ϸ�ǳƺ��Ա�
			&&2����Ҫ��ȡ����ȼ���
			&&3����Ҫ��ȡ�ǳƺ�����ȼ���
			&&4����Ҫ��ȡ��Ƭ���־��
*/
CResultParse*user_get_info			(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip
									,pc_str _charset = NULL,uint32 _flag = 3);

/*
//--- ������ȡ����û��Ļ�����Ϣ�������ǳơ�ͷ���
@_fopenids	&Ҫ��ȡ���ݵ�openid�б��м���_������ÿ�����100����
*/
CResultParse*user_get_multi_info	(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip
									,pc_str _fopenids);
//--- ��֤�û��Ƿ�װ��Ӧ��
CResultParse*user_get_is_setup		(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip);
//--- ��ȡ��¼�û��Ƿ����
CResultParse*user_get_is_vip		(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip);
//--- ��ȡ��¼�û�����VIP��Ϣ
CResultParse*user_get_total_vip_info(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip);

//--- ��֤��¼̬&openkey����
CResultParse*user_is_login			(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip);
/*
//--- ��֤�û��Ƿ��Ǵ�ѡ����¼����Ӧ�á���Ҫ��qq��ν�����
*�������ʱ����������Ӧ�÷�����ʱ��_seqid
@_seqid	&��¼������ID��ͨ����seqid���ҵ��û���¼��Ψһ��¼��
*/
CResultParse*user_is_area_login		(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip
									,pc_str _seqid);

//--- ������ú��ѵ�VIP��Ϣ
/*
@_fopenids = ��Ҫ��ȡ��Ϣ�ĺ��ѵ�openid�б�openid֮��ͨ����_���ָ���ÿ����������50����
@_flags = ���ڻ�֧�ָ���ҵ����û�VIP��Ϣ��ѯ�����Ҫ��ѯ����VIPҵ��ͨ����,���ָ�������������ֵ��Ĭ��Ϊȫ����ѯ��
*/
CResultParse*user_friends_vip_info	(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _fopenids,pc_str _flags = NULL);

//--- �û������û�Ⱥ��ǩ�ӿ�
/*
#����customflag(uint32)
*/
CResultParse*user_get_app_flag		(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip);

//--- ����û���־λ�ӿ�
/*
@_acttype=1������û�customflag�е��û�ȺID��Ϣ��3������û���Ǳ���ѱ�ʶλ�� 
@_usergroupid=�û�ȺID����acttype=1ʱ�贫��,get_app_flag����customflag
*/
CResultParse*user_del_app_flag		(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip,uint32 _acttype,uint32 _usergroupid);

