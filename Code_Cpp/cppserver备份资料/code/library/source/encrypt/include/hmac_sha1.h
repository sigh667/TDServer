/*----------------- hmac_sha1.h
*
* Copyright (C): 2011  Mokylin°§Mokyqi
* Author       : ’‘Œƒ‘¥
* Version      : V1.0
* Date         : 2012/6/4 19:58:10
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
/*************************************************************/
namespace hmac_sha1
{
	unsigned int	hmac_sha1(const char*key,unsigned int key_length,const char*data,unsigned int data_length,char*output,unsigned int out_lentth);
};
