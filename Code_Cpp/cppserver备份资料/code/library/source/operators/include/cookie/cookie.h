/*----------------- cookie.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2012/6/27 19:32:53
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "basic/stringFunctions.h"
#include "encrypt/include/encrypts.h"
/*************************************************************/
namespace cookie
{
	//--- ��ʼ��
	bool	init		(uint32 _timeout,pc_str _key);

	//--- ��¼��֤
	bool	login_verify(url_parser&_param,std::string&_uname);
};
