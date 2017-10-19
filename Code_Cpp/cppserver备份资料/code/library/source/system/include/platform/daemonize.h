/*----------------- daemonize.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/3/22 11:03:54
*--------------------------------------------------------------
* 守护进程【linux专属】
*------------------------------------------------------------*/
#pragma once
#include "basic/basicTypes.h"
/*************************************************************/
//--- 创建初始化守护进程
/*
@_newWorkDir: 需要改变工作目录,NULL不改变
--信号重定向:NULL为忽略此信号;"/dev/null"【最好不要使用】发送到空设备;"filePath/fileName"
@_out_dup	: 终端输出重定向
@_err_dup	: 终端错误重定向
@_in_dup	: 终端输入重定向
*/
extern int	daemonize(const char* _newWorkDir = NULL,const char*_out_dup = NULL,const char*_err_dup = NULL,const char*_in_dup = NULL);
