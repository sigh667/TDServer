/*----------------- open_qq_user.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/8/1 13:56:02
*--------------------------------------------------------------
*用户信息
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
/*
//--- 获取登录用户的信息
@[__charset]&指定请求及响应的字符集，取值为【gbk】或utf-8（只有pf=qqgame或pf=3366时，可以输入该参数）。
@flag		&pf=qqgame时，必须输入该参数，指定需要获取QQGame中的哪些信息
			&&1：需要获取游戏昵称和性别；
			&&2：需要获取蓝钻等级；
			&&3：需要获取昵称和蓝钻等级；
			&&4：需要获取照片秀标志。
*/
CResultParse*user_get_info			(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip
									,pc_str _charset = NULL,uint32 _flag = 3);

/*
//--- 批量获取多个用户的基本信息，包括昵称、头像等
@_fopenids	&要获取数据的openid列表，中间以_隔开，每次最多100个。
*/
CResultParse*user_get_multi_info	(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip
									,pc_str _fopenids);
//--- 验证用户是否安装了应用
CResultParse*user_get_is_setup		(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip);
//--- 获取登录用户是否黄钻
CResultParse*user_get_is_vip		(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip);
//--- 获取登录用户所有VIP信息
CResultParse*user_get_total_vip_info(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip);

//--- 验证登录态&openkey续期
CResultParse*user_is_login			(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip);
/*
//--- 验证用户是否是从选区登录进入应用【续要与qq多次交互】
*多区多服时弹出第三方应用服务器时带_seqid
@_seqid	&登录的自增ID，通过该seqid可找到用户登录的唯一记录。
*/
CResultParse*user_is_area_login		(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip
									,pc_str _seqid);

//--- 批量获得好友的VIP信息
/*
@_fopenids = 需要获取信息的好友的openid列表，openid之间通过“_”分隔，每次最多可输入50个。
@_flags = 后期会支持更多业务的用户VIP信息查询。如果要查询多种VIP业务，通过“,”分隔。如果不输入该值，默认为全部查询。
*/
CResultParse*user_friends_vip_info	(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _fopenids,pc_str _flags = NULL);

//--- 用户所在用户群标签接口
/*
#返回customflag(uint32)
*/
CResultParse*user_get_app_flag		(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip);

//--- 清除用户标志位接口
/*
@_acttype=1：清除用户customflag中的用户群ID信息。3：清除用户高潜付费标识位。 
@_usergroupid=用户群ID，当acttype=1时需传入,get_app_flag返回customflag
*/
CResultParse*user_del_app_flag		(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip,uint32 _acttype,uint32 _usergroupid);

