/*----------------- open_qq_compass.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2012/8/1 13:44:43
*--------------------------------------------------------------
*����ϵͳ
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
/*
_opuid:player_id
source:�û��ڽ��е�¼����ע��Ȳ������ⲿ��Դ
*/
//--- ��¼(��ɫ)
CResultParse*compass_report_login		(uint32 _worldid,uint64 _svrip,pc_str _openid,uint64 _userip,uint32 _opuid,uint64 _time,uint32 _level,pc_str _reserve_5,uint16 _pf,pc_str _source = NULL);
//--- ����ע��
CResultParse*compass_report_register	(uint32 _worldid,uint64 _svrip,pc_str _openid,uint64 _userip,uint32 _opuid,uint64 _time,pc_str _reserve_5,uint16 _pf,pc_str _source = NULL);
//--- ��������ע��
CResultParse*compass_report_accept		(uint32 _worldid,uint64 _svrip,pc_str _openid,uint64 _userip,uint32 _opuid,uint64 _time,pc_str _reserve_5,uint16 _pf,pc_str _source = NULL);
//--- ��������ע��
CResultParse*compass_report_invite		(uint32 _worldid,uint64 _svrip,pc_str _openid,uint64 _userip,uint32 _opuid,uint64 _time,uint32 _level,pc_str _reserve_5,uint16 _pf,pc_str _source = NULL);
//--- ֧������
CResultParse*compass_report_consume		(uint32 _worldid,uint64 _svrip,pc_str _openid,uint64 _userip,uint32 _opuid,uint64 _time,uint32 _level,pc_str _reserve_5,uint16 _pf,int32 _modifyfee,uint32 _totalfee,uint16 _item_id,uint16 _item_count);
//--- ֧����ֵ
CResultParse*compass_report_recharge	(uint32 _worldid,uint64 _svrip,pc_str _openid,uint64 _userip,uint32 _opuid,uint64 _time,uint32 _level,pc_str _reserve_5,uint16 _pf,int32 _modifyfee,uint32 _totalfee,uint16 _item_id,uint16 _item_count);
//--- �˳�(��ɫ)
CResultParse*compass_report_quit		(uint32 _worldid,uint64 _svrip,pc_str _openid,uint64 _userip,uint32 _opuid,uint64 _time,uint32 _level,uint32 _onlinetime,pc_str _reserve_5,uint16 _pf,pc_str _source = NULL);
//--- ͨ�� 
CResultParse*compass_report				(uint32 _worldid,uint64 _svrip,uint64 _userip,uint64 _time,pc_str _reserve_5,uint16 _pf);
//--- ���߽�ɫ����(5�����ϱ�һ��)
CResultParse*compass_online_count		(uint32 _worldid,uint64 _svrip,uint64 _userip,uint64 _time,uint32 _count,uint16 _pf);
//--- ��ɫ����
CResultParse*compass_player_upgrade		(uint32 _worldid,uint64 _svrip,pc_str _openid,uint64 _userip,uint32 _opuid,uint64 _time,uint8 _level,uint64 _exp,uint32 _onlinetime,pc_str _reserve_5,uint16 _pf,pc_str _source = NULL);
