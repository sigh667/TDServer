/*----------------- daemonize.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2012/3/22 11:03:54
*--------------------------------------------------------------
* �ػ����̡�linuxר����
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
/*************************************************************/
//--- ������ʼ���ػ�����
/*
@_newWorkDir: ��Ҫ�ı乤��Ŀ¼,NULL���ı�
--�ź��ض���:NULLΪ���Դ��ź�;"/dev/null"����ò�Ҫʹ�á����͵����豸;"filePath/fileName"
@_out_dup	: �ն�����ض���
@_err_dup	: �ն˴����ض���
@_in_dup	: �ն������ض���
*/
extern int	daemonize(const char* _newWorkDir = NULL,const char*_out_dup = NULL,const char*_err_dup = NULL,const char*_in_dup = NULL);
