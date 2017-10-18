/*------------- Base64.h
* Copyright (C): 2011 Mokylin¡¤Mokyqi
* Author       : ÕÔÎÄÔ´
* Version      : V1.01
* Date         : 2011/8/22 9:50:47
*
*/ 
/***************************************************************
* ×Ö·û´®base64±àÂë
***************************************************************/
#pragma once
#include <string>
/*************************************************************/
class CBase64
{
public:
	static inline bool is_base64(unsigned char c)
	{
		return (isalnum(c) || (c == '+') || (c == '/'));
	};

	static	std::string base64_encode(const char*, unsigned int len);
	static	std::string base64_encode(std::string const& s);
	static	std::string base64_decode(const char*, unsigned int len);
	static	std::string base64_decode(std::string const& s);
};
