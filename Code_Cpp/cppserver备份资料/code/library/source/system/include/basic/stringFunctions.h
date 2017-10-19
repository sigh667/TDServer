/*------------- stringFunctions.h
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.01
* Date         : 2011/2/15 11:21:02
*
*/ 
/***************************************************************
* 字符串函数
***************************************************************/
#pragma once
#include "basic/basicTypes.h"
#include <memory.h>
#include <string>
#include <set>
#include <stdarg.h>
/*************************************************************/
extern void		set_locale(pc_str _locale);
extern pc_str	get_locale();
//------------------------------------------------------
//------------------------------ String Basic Functions
//--- 计算字符串长度
extern uint32	dStrlen(const char *s);

//--- 字符串链接
#ifdef WIN32
extern int32	dStrcat(char *dst,uint32 _size, const char *src);
#else // WIN32
extern char*	dStrcat(char *dst,uint32 _size, const char *src);
#endif // WIN32

//--- 字符串链接(_count:最多可链接数)
#ifdef WIN32
extern int32	dStrncat(char *dst,uint32 _size, const char *src,uint32 _count);
#else // WIN32
extern char*	dStrncat(char *dst,uint32 _size, const char *src,uint32 _count);
#endif // WIN32

//--- 字符串拷贝
#ifdef WIN32
extern int32	dStrcpy(char *dst,uint32 _size, const char *src);
#else // WIN32
extern char*	dStrcpy(char *dst,uint32 _size, const char *src);
#endif // WIN32

//--- 字符串拷贝(len:拷贝数)
#ifdef WIN32
extern int32	dStrncpy(char *dst,uint32 _size, const char *src, uint32 len);
#else // WIN32
extern char*	dStrncpy(char *dst,uint32 _size, const char *src, uint32 len);
#endif // WIN32

//--- 拷贝字符串
extern	int32	dStrcpyMax(char *dst,uint32 _size, const char *src, uint32 len);

//--- 从buf所指内存区域的前count个字节查找字符ch位置
extern void*	dMemchr(void *buf,char ch,uint32 count);

//--- 字符串中查找第一次出现c的位置
extern char*	dStrchr(char *s,char c);
extern pc_str 	dStrchr(const char *s,char c);

//--- 分解字符串为一组标记串
extern char*	dStrtok(char *s, const char *delim);

//--- 将字符串转换为小写形式
extern char*	dStrlwr(char *s);

//--- 将字符串转换为大写形式
extern char*	dStrupr(char *s);

//--- 是否宽字节
extern int32	ismbblead(char c);

//--- s1跟n，合并生成新字符串
extern void	formateStringInt(std::string& str,int32 n);

//--- 字符串比较(区分大小写)
extern int		dStrncmp(const char *s1,const char * s2,int n);

//--- 字符串比较(不区分大小写)
extern int		dStrnicmp(const char *s1,const char * s2,int n);

//--- 字符串查找第一次出现的位置(区分大小写)
extern char*	dStrstr(char *haystack, char *needle);

//--- 字符串查找第一次出现的位置(不区分大小写)
extern char*	dStristr(char *haystack, char *needle);

//--- 字符串格式化
extern int		dSprintf(char *string,size_t sizeInBytes,const char *format, ... );
extern int		dVsprintf(char *string,size_t sizeInBytes,const char *format,va_list _Args);
//--- 字符串打印
extern int		dPrintf(const char *format, ... );
extern int		dVprintf(const char *format,va_list _Args);
//------------------------------------------------------
//------------------------------ String Types
//--- 从右侧检测多字节是否合法
extern bool		testMultibyte(char* pStr);

//--- 去除指定字符
extern void		wipeOffChar(char* pStr,uint32 uLen,char cChar);

//--- 过滤全半角空格
extern void		filtrationBlank(char* pStr,uint32 uLen);

//--- 是否是数字字符串
extern bool		numeralString(const char* pStr,int32 nLen);

//--- 是否拥有字符
extern bool		haveFromString		(const std::string&strString,char c);
//--- 获得字符串段
extern void		getSubString		(std::string&strString,std::string&strSubString,char c = ';');
//--- 截断字符串
extern void		truncateString		(std::string&strString,char c = ';');
//--- 获得字符串段
extern void		truncateString		(std::string&strString,std::string&strSub,std::string::size_type&_off,std::string::size_type _count);
//--- 读取数字(返回0为未读取到)
extern int64	readInt64FromString	(std::string&strString,char c = ';');
//--- 读取日期时间(返回0为未读取到)
extern uint64	readDateTimeFromString(std::string&strString);
//--- 读取浮点(返回0为未读取到)
extern float64	readfloat64FromString(std::string&strString,char c = ';');
// -- 判断是否空格 (可用于中文字符串)
extern int chSpace					( int ch );
//--- 去除左边空白字符
extern std::string&	ltrim			(std::string&strString);
//--- 去除右边空白字符
extern std::string&	rtrim			(std::string&strString);
//--- 去除左右空白字符
extern std::string&	trim			(std::string&strString);
//--- 获得子字符串
extern bool		getSubString		(std::string&strString,std::string&strSubString,const char*pStrtok);
//-------------------------------------------------------------
//--- 是否拥有字符串charSet中任意字符 如果charSet为空，返回true
template<class T>
bool	haveAnyCharacter(const T &strString, const T &charSet)
{
	if (charSet.empty())
		return true;
	std::set<int>	setTarget;
	for (std::size_t i = 0; i < charSet.length(); ++i)
	{
		setTarget.insert(charSet[i]);
	}
	for (std::size_t i = 0; i < strString.length(); ++i)
	{
		if (setTarget.find(strString[i]) != setTarget.end())
		{
			return true;
		}
	}
	return false;
}
//-------------------------------------------------------------
//--- 是否拥有字符串charSet中的所有字符 如果charSet为空，返回true
template<class T>
bool	haveAllCharacter(const T &strString, const T &charSet)
{
	std::set<int>	setTarget;
	for (std::size_t i = 0; i < charSet.length(); ++i)
	{
		setTarget.insert(charSet[i]);
	}
	for (std::size_t i = 0; i < strString.length(); ++i)
	{
		setTarget.erase(strString[i]);
	}
	return setTarget.size() <= 0;
}
//------------------------------------------------------
//------------------------------ ANSI与UTF8的转换
//--- ANSI转换UTF8
extern bool	convertANSItoUTF8(char* pAnsiString,int32 nLen);
//--- UTF8转ANSI
extern bool	convertUTF8toANSI(char* pUtf8String,int32 nLen);
//--- ANSI转换UTF8
extern bool _convertANSItoUTF8(const std::string &strAnsi, std::string &strUtf8);
//--- UTF8转ANSI
extern bool _convertUTF8toANSI(const std::string &strUtf8, std::string &strAnsi);
//--- 转换为宽字节
extern std::wstring&convertToWString(const char *str);
//--- 宽字符到单字符转换
extern std::string&convertToCString(const std::wstring&str);
//--- 设置字符串的某一个位  uIndex从1开始
extern void		setStringBit	(std::string&strString,uint16 uIndex,bool _set);
//--- 获取字符串的某一个位  uIndex从1开始
extern bool		getStringBit	(std::string&strString,uint16 uIndex);