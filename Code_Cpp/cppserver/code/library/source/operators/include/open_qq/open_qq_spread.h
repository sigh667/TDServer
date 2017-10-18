/*----------------- open_qq_spread.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/8/28 20:40:36
*--------------------------------------------------------------
*推广接口
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
/*
//--- 验证好友邀请的invkey{用于“邀请好友即赠送礼品”等场景。}
@invkey	&需要验证的好友邀请加密串。跳转到应用首页后，URL后会带该参数。
@itime	&邀请时间。跳转到应用首页后，URL后会带该参数。
@iopenid&发起邀请者的openid。跳转到应用首页后，URL后会带该参数。
----被邀请进入时CanvasURL会携带参数:invkey,itime,iopenid
*/
CResultParse*spread_verify_invkey	(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip
									,pc_str _invkey,uint64 _itime,pc_str _iopenid);

/*
QQ提醒
*/
//--- 是否安装了qq提醒
/*
@cmd = 需要获取的不同状态。
	0：获取用户是否安装了“QQ提醒”应用；
	1：获取用户是否在“QQ提醒”应用中开启了提醒；
	2：同时获取用户是否安装了“QQ提醒”应用，以及是否在“QQ提醒”中开启了提醒。 
*/
CResultParse*spread_is_reminder_set	(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _cmd = "2");
//--- 上报qq提醒
/*
@beginx = 提醒开始时间（unix时间戳，指从UTC时间1970年1月1日00:00:00到当前时刻的秒数）。
@keyx = 发送提醒时，应用中某个目标的标识，由应用自定义（如QQ农场提醒用户某块地的果实成熟了，这块地的id即为key的值）。
@typex = 发送提醒时，提醒的类型，由应用自定义，必须是20000以上的整数。
@titlex = 提醒的具体内容，长度限制在30个汉字以内，受限于展示的宽度，建议在20个汉字以内。
*x=提醒信息序号,从0开始累加，每次会替换原上报,(x从0-2)最大3条
*/
CResultParse*spread_set_reminder	(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _beginx,pc_str _keyx,pc_str _typex,pc_str _titlex);


CResultParse*spread_set_feed		(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip);
CResultParse*spread_get_feed		(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip);
CResultParse*spread_set_request		(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip);
CResultParse*spread_get_request		(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip);
CResultParse*spread_get_limitation	(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip);
