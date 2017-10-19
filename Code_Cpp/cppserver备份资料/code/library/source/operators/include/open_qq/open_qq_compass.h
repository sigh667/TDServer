/*----------------- open_qq_compass.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/8/1 13:44:43
*--------------------------------------------------------------
*罗盘系统
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
/*
_opuid:player_id
source:用户在进行登录或者注册等操作的外部来源
*/
//--- 登录(角色)
CResultParse*compass_report_login		(uint32 _worldid,uint64 _svrip,pc_str _openid,uint64 _userip,uint32 _opuid,uint64 _time,uint32 _level,pc_str _reserve_5,uint16 _pf,pc_str _source = NULL);
//--- 主动注册
CResultParse*compass_report_register	(uint32 _worldid,uint64 _svrip,pc_str _openid,uint64 _userip,uint32 _opuid,uint64 _time,pc_str _reserve_5,uint16 _pf,pc_str _source = NULL);
//--- 接受邀请注册
CResultParse*compass_report_accept		(uint32 _worldid,uint64 _svrip,pc_str _openid,uint64 _userip,uint32 _opuid,uint64 _time,pc_str _reserve_5,uint16 _pf,pc_str _source = NULL);
//--- 邀请他人注册
CResultParse*compass_report_invite		(uint32 _worldid,uint64 _svrip,pc_str _openid,uint64 _userip,uint32 _opuid,uint64 _time,uint32 _level,pc_str _reserve_5,uint16 _pf,pc_str _source = NULL);
//--- 支付消费
CResultParse*compass_report_consume		(uint32 _worldid,uint64 _svrip,pc_str _openid,uint64 _userip,uint32 _opuid,uint64 _time,uint32 _level,pc_str _reserve_5,uint16 _pf,int32 _modifyfee,uint32 _totalfee,uint16 _item_id,uint16 _item_count);
//--- 支付充值
CResultParse*compass_report_recharge	(uint32 _worldid,uint64 _svrip,pc_str _openid,uint64 _userip,uint32 _opuid,uint64 _time,uint32 _level,pc_str _reserve_5,uint16 _pf,int32 _modifyfee,uint32 _totalfee,uint16 _item_id,uint16 _item_count);
//--- 退出(角色)
CResultParse*compass_report_quit		(uint32 _worldid,uint64 _svrip,pc_str _openid,uint64 _userip,uint32 _opuid,uint64 _time,uint32 _level,uint32 _onlinetime,pc_str _reserve_5,uint16 _pf,pc_str _source = NULL);
//--- 通用 
CResultParse*compass_report				(uint32 _worldid,uint64 _svrip,uint64 _userip,uint64 _time,pc_str _reserve_5,uint16 _pf);
//--- 在线角色数量(5分钟上报一次)
CResultParse*compass_online_count		(uint32 _worldid,uint64 _svrip,uint64 _userip,uint64 _time,uint32 _count,uint16 _pf);
//--- 角色升级
CResultParse*compass_player_upgrade		(uint32 _worldid,uint64 _svrip,pc_str _openid,uint64 _userip,uint32 _opuid,uint64 _time,uint8 _level,uint64 _exp,uint32 _onlinetime,pc_str _reserve_5,uint16 _pf,pc_str _source = NULL);
