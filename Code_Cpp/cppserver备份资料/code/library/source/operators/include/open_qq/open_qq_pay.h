/*----------------- open_qq_pay.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2012/8/1 13:47:46
*--------------------------------------------------------------
*֧����ؽӿ�
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
/*
//--- ��ȡ����token
@pfkey		&��ת��Ӧ����ҳ��URL�����ò�������ƽֱ̨�Ӵ���Ӧ�ã�Ӧ��ԭ������ƽ̨���ɡ�
@[amt]		&��ʾ���鹺���������Ʒ���ܼ۸񣬼���Ʒ�ĵ���*������������Q��Ϊ��λ��1Q��=10Q�㣩��
			&&��������������ײ���Ʒ������봫��amt��amt��������Ʒ�ĵ���*��������֮�͡�
@ts			&linuxʱ�����
@payitem	&��Ʒ��Ϣ��
			&&1����ʹ��ID*price*num�ĸ�ʽ�����������ײ���Ʒ���á�;���ָ����ַ����в��ܰ���"|"�����ַ���
			&&��2��ID��ʾ��ƷID��price��ʾ���ۣ���Q��Ϊ��λ��1Q��=10Q�㣬���۵��ƶ�����ѭ���߶��۹淶����num��ʾ����Ĺ���������
			&&ʾ����
			&&���������ײͣ��ײ��а�����Ʒ1����Ʒ2����Ʒ1��IDΪG001������Ʒ�ĵ���Ϊ10Q�㣬��������Ϊ1����Ʒ2��IDΪG008������Ʒ�ĵ���Ϊ8Q�㣬��������Ϊ2����payitemΪ��G001*10*1;G008*8*2 ��
@[appmode]	&1��ʾ�û��������޸���Ʒ������2��ʾ�û�����ѡ������Ʒ��������Ĭ��Ϊ2�����������ײ�ʱ���������1��
@goodsmeta	&��Ʒ��Ϣ����ʽ�����ǡ�name*des�������������ײ�ʱҲֻ����1���������ƺ�1�����������������ײ͵����ƺ�������name��ʾ��Ʒ�����ƣ�des��ʾ��Ʒ��������Ϣ���û�������Ʒ��ȷ��֧��ҳ�棬����ʾ����Ʒ��Ϣ��
			&&���ȱ���<=256�ַ�������ʹ��utf8���롣��������ǰ���goodsmeta����base64 encode��
			&&Ŀǰgoodsmeta����76���ַ�������ӻس��ַ���
@goodsurl	&�û�������Ʒ��ȷ��֧��ҳ�潫��ʾ����ƷͼƬ������<=512�ַ�,ע��ͼƬ���Ϊ��116 * 116 px��
@[zoneid]	&����ID��Ʒ��ͼƬurl��
@manyouid	&���ƽ̨Ϊ����ƽ̨������������pfֵΪmanyou$siteid��������ת��Ӧ����ҳ��URL����manyouid����ʱ���ñ�֧���ӿ�ʱ������ش��ò�����
@[present]	&������ʶ�û��Ǹ��Լ�������Ʒ������������Ʒ�����ѡ���Ĭ��Ϊ0��
			&&1����ʾ�û�������Ʒ�����ѣ���������֧�������ڵ������ͳ�����
			&&0�򲻴�ֵ����ʾ�û����Լ�������Ʒ��
*/
CResultParse*pay_buy_goods			(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip
									,pc_str _pfkey,uint32 _amt,uint64 _ts,pc_str _payitem
									,pc_str _goodsmeta,pc_str _goodsurl,uint32 _zoneid,uint32 _appmode=2
									,uint32 _present = 0,pc_str _manyouid = "");

//--- Ӧ�÷���֪ͨ
CResultParse*pay_confirm_delivery	(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip
									,pc_str _pfkey,uint64 _ts,pc_str _payitem,pc_str _token_id,pc_str _billno,uint16 _zoneid
									,uint8 _providetype,uint8 _provide_errno,pc_str _provide_errmsg/* = NULL*/,uint32 _amt
									,uint32 _payamt_coins,uint32 _pubacct_payamt_coins);

//--- ��ѯ�ֿ�ȯ���
CResultParse*pay_get_pubacct_balance(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip
									,pc_str _pfkey,uint64 _ts = getTime());
/*
//--- ��ѯ��Ϸ�����
@_pfkey		&��ת��Ӧ����ҳ��URL�����ò���������ʾƽ̨����Ϣ���ܴ�������openid��openkey��pf��appid�����ɡ���
@[_zoneid]	&�������Ӧ�ñ��봫����ֶΣ���ʾ�������Ӧ��ĳһ��������ID��
@_ts		&linuxʱ�����
*/
CResultParse*pay_get_balance		(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip
									,pc_str _pfkey,uint32 _zoneid,uint64 _ts = getTime());
/*
//--- ��ȡ��ȡ���ߵ�token
@_pfkey		&��ת��Ӧ����ҳ��URL�����ò���������ʾƽ̨����Ϣ���ܴ�������openid��openkey��pf��appid�����ɡ���
@_tokentype	&token������
			&&1����ʾ��tokenΪÿ���������Ӫ����е����͵���/��Ʒ��token��
			&&1��2����ʾ��tokenΪ���������г�Ӫ����е���ȡ������token��
@_discountid&&��1������ǡ�ÿ��������͡�Ӫ���������discountid��ʾ���͵���/��Ʒ��Ӫ���ID��
			&&��2������ǡ������С�������discountid��ʾ�����Լ����õġ���ƷID����
@[_zoneid]	&�������Ӧ�ñ��봫����ֶΣ���ʾ�������Ӧ��ĳһ��������ID��
@_ts		&linuxʱ�����
*/
CResultParse*pay_get_token			(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip
									,pc_str _pfkey,uint32 _tokentype,pc_str _discountid,uint32 _zoneid,uint64 _ts = getTime());

/*
//--- ��ѯ�Ƿ��п�ִ������
@token = ��ȡ��������token���ǵ�����ȡҳ���Ψһ��ʶ��ͨ������v3/pay/get_token�ӿڻ�ȡ����token��Ч��Ϊ72Сʱ��
			���鿪���߽���ȡ����token���浽���أ���72Сʱ�ڿ�ֱ��ʹ�ã�����Ҫ���µ���v3/pay/get_token�ӿڻ�ȡ�� 
@awardid = ����Ӫ�����������������õĵ���ID��
			�������õ���ID����ڣ���¼op.open.qq.com�����������ģ�ѡ��1��Ӧ�ý�����ڡ�֧�����㡱tab�µġ������С��࣬ �ɿ������������ҡ���ť��������뼴�ɿ��������Լ����õĵ���ID��
*/
CResultParse*pay_has_task			(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip,pc_str _token,uint32 _awardid);
