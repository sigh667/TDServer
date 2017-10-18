/*------------- urlEncode.h
* Copyright (C): 2011 Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.01
* Date         : 2011/8/22 10:01:14
*
*/ 
/***************************************************************
* url�ı��������
* ��ո���url������Ϊ20%
***************************************************************/
#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <map>
/*************************************************************/
//#############################################################
//############################## url���������
//#############################################################
namespace url_encode
{
	/*----->{ ���� }*/ 
	std::string&urlEncode(const std::string& str);
	/*----->{ ���� }*/ 
	std::string&urlDecode(const std::string& str);
};

//#############################################################
//############################## http����
//#############################################################
namespace http_content
{
	bool	get_request		(std::string&_request,const char*_host,unsigned short _port,const char*_url,const char*_content);
	bool	post_request	(std::string&_request,const char*_host,unsigned short _port,const char*_url,unsigned int _length);
	void	response_head	(std::string&_response,unsigned int _length,bool _utf8 = false);

	bool	is_http_message	(const char*_buffer,unsigned short _size);
	bool	is_http_complete(const char*_buffer,unsigned short _size);
};

//#############################################################
//############################## url����
//#############################################################
class url_parser	: public std::map<std::string,std::string>
{
protected:
	typedef std::map<std::string,std::string>	Parent;

public:
	//--- ����
	void				param_parse			(const char*_url);
	//--- ����
	void				param_parse			(const char*_url,unsigned int _len);
	//--- ɾ������
	void				param_del			(const char*_key);
	//--- ���Ӳ���
	void				param_add			(const char* _key,long long _data);
	//--- ���Ӳ���
	void				param_add			(const char* _key,unsigned long long _data);
	//--- ���Ӳ���
	void				param_add			(const char* _key,const char* _data,bool _must = false);
	//--- �����ַ���
	const char*			param_to_url_string	(const char* _prefix = NULL,bool _url_encode = true);

	//--- ��ò�������
	long long			get_param_long64	(const char*_key);
	unsigned long long	get_param_ulong64	(const char*_key);
	//--- ��ò����ַ���
	std::string*		get_param_string	(const char*_key);
	//--- ��ò�������
	const char*			get_param_content	(const char*_key,const char* _empty = NULL);
};
