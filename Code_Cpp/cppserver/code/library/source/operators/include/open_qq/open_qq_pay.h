/*----------------- open_qq_pay.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/8/1 13:47:46
*--------------------------------------------------------------
*支付相关接口
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
/*
//--- 获取交易token
@pfkey		&跳转到应用首页后，URL后会带该参数。由平台直接传给应用，应用原样传给平台即可。
@[amt]		&表示建议购买的所有物品的总价格，即物品的单价*建议数量（以Q点为单位，1Q币=10Q点）。
			&&如果是批量购买套餐物品，则必须传入amt，amt是所有物品的单价*建议数量之和。
@ts			&linux时间戳。
@payitem	&物品信息。
			&&1）请使用ID*price*num的格式，批量购买套餐物品则用“;”分隔，字符串中不能包含"|"特殊字符。
			&&（2）ID表示物品ID，price表示单价（以Q点为单位，1Q币=10Q点，单价的制定需遵循道具定价规范），num表示建议的购买数量。
			&&示例：
			&&批量购买套餐，套餐中包含物品1和物品2。物品1的ID为G001，该物品的单价为10Q点，购买数量为1；物品2的ID为G008，该物品的单价为8Q点，购买数量为2，则payitem为：G001*10*1;G008*8*2 。
@[appmode]	&1表示用户不可以修改物品数量，2表示用户可以选择购买物品的数量。默认为2；批量购买套餐时，必须等于1。
@goodsmeta	&物品信息，格式必须是“name*des”，批量购买套餐时也只能有1个道具名称和1个描述，即给出该套餐的名称和描述。name表示物品的名称，des表示物品的描述信息。用户购买物品的确认支付页面，将显示该物品信息。
			&&长度必须<=256字符，必须使用utf8编码。发送请求前请对goodsmeta进行base64 encode；
			&&目前goodsmeta超过76个字符后不能添加回车字符。
@goodsurl	&用户购买物品的确认支付页面将显示该物品图片。长度<=512字符,注意图片规格为：116 * 116 px。
@[zoneid]	&大区ID物品的图片url。
@manyouid	&如果平台为漫游平台（即公共参数pf值为manyou$siteid），则跳转到应用首页后，URL后会带manyouid。此时调用本支付接口时，必须回传该参数。
@[present]	&用来标识用户是给自己购买物品，还是赠送物品给好友。【默认为0】
			&&1：表示用户赠送物品给好友，后续进行支付，用于道具赠送场景；
			&&0或不传值：表示用户给自己购买物品。
*/
CResultParse*pay_buy_goods			(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip
									,pc_str _pfkey,uint32 _amt,uint64 _ts,pc_str _payitem
									,pc_str _goodsmeta,pc_str _goodsurl,uint32 _zoneid,uint32 _appmode=2
									,uint32 _present = 0,pc_str _manyouid = "");

//--- 应用发货通知
CResultParse*pay_confirm_delivery	(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip
									,pc_str _pfkey,uint64 _ts,pc_str _payitem,pc_str _token_id,pc_str _billno,uint16 _zoneid
									,uint8 _providetype,uint8 _provide_errno,pc_str _provide_errmsg/* = NULL*/,uint32 _amt
									,uint32 _payamt_coins,uint32 _pubacct_payamt_coins);

//--- 查询抵扣券余额
CResultParse*pay_get_pubacct_balance(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip
									,pc_str _pfkey,uint64 _ts = getTime());
/*
//--- 查询游戏币余额
@_pfkey		&跳转到应用首页后，URL后会带该参数。【表示平台的信息加密串，根据openid，openkey，pf，appid等生成。】
@[_zoneid]	&多区多服应用必须传入该字段，表示多区多服应用某一个大区的ID号
@_ts		&linux时间戳。
*/
CResultParse*pay_get_balance		(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip
									,pc_str _pfkey,uint32 _zoneid,uint64 _ts = getTime());
/*
//--- 获取领取道具的token
@_pfkey		&跳转到应用首页后，URL后会带该参数。【表示平台的信息加密串，根据openid，openkey，pf，appid等生成。】
@_tokentype	&token的类型
			&&1：表示该token为每日礼包赠送营销活动中的赠送道具/物品的token。
			&&1：2：表示该token为任务买卖市场营销活动中的领取奖励的token。
@_discountid&&（1）如果是“每日礼包赠送”营销活动场景，discountid表示赠送道具/物品的营销活动ID。
			&&（2）如果是“任务集市”场景，discountid表示卖方自己设置的“商品ID”。
@[_zoneid]	&多区多服应用必须传入该字段，表示多区多服应用某一个大区的ID号
@_ts		&linux时间戳。
*/
CResultParse*pay_get_token			(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip
									,pc_str _pfkey,uint32 _tokentype,pc_str _discountid,uint32 _zoneid,uint64 _ts = getTime());

/*
//--- 查询是否有可执行任务
@token = 领取任务奖励的token，是弹出领取页面的唯一标识，通过调用v3/pay/get_token接口获取，该token有效期为72小时。
			建议开发者将获取到的token保存到本地，在72小时内可直接使用，不需要重新调用v3/pay/get_token接口获取。 
@awardid = 交叉营销任务集市中卖方设置的道具ID。
			卖方设置道具ID的入口：登录op.open.qq.com后进入管理中心，选择1个应用进入后，在“支付结算”tab下的“任务集市”类， 可看到“我是卖家”按钮，点击进入即可看到卖方自己设置的道具ID。
*/
CResultParse*pay_has_task			(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip,pc_str _token,uint32 _awardid);
