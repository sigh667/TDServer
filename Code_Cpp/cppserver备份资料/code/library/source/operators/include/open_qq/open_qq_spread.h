/*----------------- open_qq_spread.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2012/8/28 20:40:36
*--------------------------------------------------------------
*�ƹ�ӿ�
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
/*
//--- ��֤���������invkey{���ڡ�������Ѽ�������Ʒ���ȳ�����}
@invkey	&��Ҫ��֤�ĺ���������ܴ�����ת��Ӧ����ҳ��URL�����ò�����
@itime	&����ʱ�䡣��ת��Ӧ����ҳ��URL�����ò�����
@iopenid&���������ߵ�openid����ת��Ӧ����ҳ��URL�����ò�����
----���������ʱCanvasURL��Я������:invkey,itime,iopenid
*/
CResultParse*spread_verify_invkey	(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip
									,pc_str _invkey,uint64 _itime,pc_str _iopenid);

/*
QQ����
*/
//--- �Ƿ�װ��qq����
/*
@cmd = ��Ҫ��ȡ�Ĳ�ͬ״̬��
	0����ȡ�û��Ƿ�װ�ˡ�QQ���ѡ�Ӧ�ã�
	1����ȡ�û��Ƿ��ڡ�QQ���ѡ�Ӧ���п��������ѣ�
	2��ͬʱ��ȡ�û��Ƿ�װ�ˡ�QQ���ѡ�Ӧ�ã��Լ��Ƿ��ڡ�QQ���ѡ��п��������ѡ� 
*/
CResultParse*spread_is_reminder_set	(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _cmd = "2");
//--- �ϱ�qq����
/*
@beginx = ���ѿ�ʼʱ�䣨unixʱ�����ָ��UTCʱ��1970��1��1��00:00:00����ǰʱ�̵���������
@keyx = ��������ʱ��Ӧ����ĳ��Ŀ��ı�ʶ����Ӧ���Զ��壨��QQũ�������û�ĳ��صĹ�ʵ�����ˣ����ص�id��Ϊkey��ֵ����
@typex = ��������ʱ�����ѵ����ͣ���Ӧ���Զ��壬������20000���ϵ�������
@titlex = ���ѵľ������ݣ�����������30���������ڣ�������չʾ�Ŀ�ȣ�������20���������ڡ�
*x=������Ϣ���,��0��ʼ�ۼӣ�ÿ�λ��滻ԭ�ϱ�,(x��0-2)���3��
*/
CResultParse*spread_set_reminder	(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _beginx,pc_str _keyx,pc_str _typex,pc_str _titlex);


CResultParse*spread_set_feed		(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip);
CResultParse*spread_get_feed		(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip);
CResultParse*spread_set_request		(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip);
CResultParse*spread_get_request		(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip);
CResultParse*spread_get_limitation	(pc_str _openid,pc_str _openkey,pc_str _pf,pc_str _userip);
