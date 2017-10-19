/*------------- basicTypes.h
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.01
* Date         : 2011/1/29 15:47:59
*
*/ 
/***************************************************************
* 定义各种基本类型
***************************************************************/
#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdexcept> 
#include <algorithm>
#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
#include <mswsock.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#else//WIN32
#include <netinet/in.h>//包含了<sys/socket.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>//系统类型定义
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>//close函数
#endif//WIN32

#if !defined(_WIN32) && !defined(_WIN64)
#include <stdint.h>
#include <inttypes.h>
#endif
/*************************************************************/
//------------------------------- Basic Types...
typedef signed char			int8;	///< Compiler independent Signed Char
typedef unsigned char		uint8;	///< Compiler independent Unsigned Char

typedef signed short		int16;	///< Compiler independent Signed 16-bit short
typedef unsigned short		uint16;	///< Compiler independent Unsigned 16-bit short

typedef unsigned long		ulong;///< Compiler independent Unsigned 32-bit integer

typedef float				float32;///< Compiler independent 32-bit float
typedef double				float64;///< Compiler independent 64-bit float

#if defined(_WIN32) || defined(_WIN64)
typedef signed __int32		int32;	///< Compiler independent Signed 32-bit integer
typedef unsigned __int32	uint32;	///< Compiler independent Unsigned 32-bit integer

typedef __int64				int64;	///Compiler independent Unsigned 64-bit integer
typedef unsigned __int64	uint64;	///Compiler independent Unsigned 64-bit integer

#else//WIN32

typedef int32_t				int32;	///< Compiler independent Signed 32-bit integer
typedef uint32_t			uint32;	///< Compiler independent Unsigned 32-bit integer

typedef long long			int64;
typedef unsigned long long	uint64;
#endif//WIN32

typedef char*				p_str;
typedef const char*			pc_str;
//------------------------------------------------------
typedef unsigned char		byte;
typedef unsigned char		BYTE;
typedef unsigned short		WORD;
typedef unsigned long		DWORD;

//------------------------------------------------------
//------------------------------- Type constants...
static const float32 Float_Pi			= float32(3.14159265358979323846);			///< Constant float PI
static const float32 Float_2Pi			= float32(2.0 * 3.14159265358979323846);	///< Constant float 2*PI
static const float32 Float_InversePi	= float32(1.0 / 3.14159265358979323846);	///< Constant float 1 / PI
static const float32 Float_HalfPi		= float32(0.5 * 3.14159265358979323846);	///< Constant float 1/2 * PI
static const float32 Float_2InversePi	= float32(2.0 / 3.14159265358979323846);	///< Constant float 2 / PI
static const float32 Float_Inverse2Pi	= float32(0.5 / 3.14159265358979323846);	///< Constant float 0.5 / PI

static const float32 Float_Sqrt2		= float32(1.41421356237309504880f);			///< Constant float sqrt(2)
static const float32 Float_SqrtHalf		= float32(0.7071067811865475244008443f);	///< Constant float sqrt(0.5)

static const int8	int8_MIN			= int8(-128);								///< Constant Min Limit int8
static const int8	int8_MAX			= int8(127);								///< Constant Max Limit int8
static const uint8	uint8_MAX			= uint8(255);								///< Constant Max Limit uint8

static const int16	int16_MIN			= int16(-32768);							///< Constant Min Limit int16
static const int16	int16_MAX			= int16(32767);								///< Constant Max Limit int16
static const uint16	uint16_MAX			= uint16(65535);							///< Constant Max Limit uint16

static const int32	int32_MIN			= int32(-2147483647 - 1);					///< Constant Min Limit int32
static const int32	int32_MAX			= int32(2147483647);						///< Constant Max Limit int32
static const uint32	uint32_MAX			= uint32(0xffffffff);						///< Constant Max Limit uint32
static const float32 FLOAT_EPSINON		= (float32)0.00001;									/// 浮点
#ifndef NULL
#define NULL	0
#endif//NULL
#define _IO_BUFFER_ 60 * 1024
#define _IO_1_KB	1024
//------------------------------------------------------
//------------------------------- min/max Operate...
//最小值
#ifndef MIN
#define MIN(a,b)				(std::min)(a,b)
#endif//MIN
//最大值
#ifndef MAX
#define MAX(a,b)				(std::max)(a,b)
#endif//MAX
//范围
#ifndef _RANGE
#define _RANGE(_v,_min,_max)	((_v > _max) ? _max : ((_v < _min) ? _min : _v))
#endif//_RANGE
//------------------------------------------------------
//------------------------------- Bit Operate...
#ifndef _BIT32
#define _BIT32(x) ((x < 32) ? (1 << (x)) : 0) 					///< Returns value with bit x set (2^x)
#endif//_BIT32

#ifndef _SET_BIT32
#define _SET_BIT32(mark,bit,s) (s ? (mark |= _BIT32(bit)) : (mark &=~_BIT32(bit)))
#endif//_SET_BIT32

#ifndef _CHECK_BIT
#define _CHECK_BIT(mark,bit)	((mark & bit) > 0)				///检测是否存在相同位
#endif//_CHECK_BIT
//------------------------------------------------------
//------------------------------- 64Bit Operate...
#ifndef _BIT64
#define _BIT64(x) uint64((x < 64) ? ((uint64)1 << (x)) : 0) 	///< Returns value with 64 bit x set (2^x)
#endif//_BIT64

#ifndef _SET_BIT64
#define _SET_BIT64(mark,bit,s) (s ? (mark |= _BIT64(bit)) : (mark &=~_BIT64(bit)))
#endif//_SET_BIT64

//------------------------------------------------------
//------------------------------- Array Bit Operate...
//------------------------------- 8 Bit Array
#ifndef _SET_BIT8ARRAY
#define _SET_BIT8ARRAY(arr,bit,s)	(s ? (arr[bit >> 3] |= uint8(1 << (bit & 0x7))) : (arr[bit >> 3] &= uint8(~(1 << (bit & 0x7)))))
#endif//_SET_BIT8ARRAY

#ifndef _CHECK_BIT8ARRAY
#define _CHECK_BIT8ARRAY(arr,bit)	((arr[bit >> 3] & uint8(1 << (bit & 0x7))) != 0)	///检测是否存在相同位
#endif//_CHECK_BIT8ARRAY
//------------------------------------------------------
//------------------------------- 
#define MAKE_UINT64(h,l)((uint64)( (((uint64)((uint32)(h) & 0xffffffff)) << 32) | ((uint32)((uint64)(l) & 0xffffffff)) ))
#define U64_H_U32(_v)	((uint32)(((uint64)(_v) >> 32) & 0xffffffff))
#define U64_L_U32(_v)	((uint32)((uint64)(_v) & 0xffffffff))

#define MAKE_UINT32(h,l)((uint32)( (((uint32)((uint16)(h) & 0xffff)) << 16) | ((uint16)((uint32)(l) & 0xffff)) ))
#define U32_H_U16(_v)	((uint16)(((uint32)(_v) >> 16) & 0xffff))
#define U32_L_U16(_v)	((uint16)((uint32)(_v) & 0xffff))
#define MAKE_UINT16(h,l)((uint16)( (((uint16)((uint8)(h) & 0xff)) << 8) | ((uint8)((uint16)(l) & 0xff)) ))
#define U16_H_U8(_v)	((uint8)(((uint16)(_v) >> 8) & 0xff))
#define U16_L_U8(_v)	((uint8)((uint16)(_v) & 0xff))
#define MAKEU16(a, b)	((WORD)(((BYTE)((DWORD)(a) & 0xff))		| ((WORD)((BYTE)((DWORD)(b) & 0xff))) << 8))
#define MAKEU32(a, b)	((LONG)(((WORD)((DWORD)(a) & 0xffff))	| ((DWORD)((WORD)((DWORD)(b) & 0xffff))) << 16))
#define LU16(l)			((WORD)((DWORD)(l) & 0xffff))
#define HU16(l)			((WORD)((DWORD)(l) >> 16))
#define LU8(w)			((BYTE)((DWORD)(w) & 0xff))
#define HU8(w)			((BYTE)((DWORD)(w) >> 8))

/*(type *)0：把0地址当成type类型的指针。
((type *)0)->field：对应域的变量。
&((type *)0)->field:取该变量的地址，其实就等于该域相对于0地址的偏移量。
(size_t)&(((type *)0)->field)：将该地址（偏移量）转化为size_t型数据。*/
#define OFFSETOF(s,m)	offsetof(s,m)			///计算数据结构成员变量偏移值(struct,成员变量)

//#############################################################
//############################## 三角形数据
//#############################################################
#ifndef M_PI
#define M_PI		3.1415926535897932384626433
#endif//M_PI

#ifndef M_2PI
#define M_2PI		(M_PI * 2.0)
#endif//M_2PI

#ifndef M_PI_F
#define M_PI_F		3.1415926535897932384626433f
#endif//M_PI_F

#ifndef M_2PI_F
#define M_2PI_F	(M_PI_F * 2.0f)
#endif//M_2PI_F

//#############################################################
//############################## 常量
//#############################################################
//所有比率分母(万)
#define MAX_PROBABILITY				10000
#define FLOAT_MAX_PROBABILITY		10000.0f

#define READ_WRITE_STRUCT inline bool read(CStreamBase& stream) {return stream.read(sizeof(*this),this);}		\
						  inline bool write(CStreamBase& stream){return stream.write(sizeof(*this),this);}
#define CHECKF(A)			if (0 == (int)(A))	{	CLog::error("%s(%d):%s(%s)",__FILE__, __LINE__,__FUNCTION__,#A); return false;}
#define CHECKTR(A)			if (0 == (int)(A))	{	CLog::error("%s(%d):%s(%s)",__FILE__, __LINE__,__FUNCTION__,#A); return true;}
#define CHECKN(A)			if (0 == (int)(A))	{	CLog::error("%s(%d):%s(%s)",__FILE__, __LINE__,__FUNCTION__,#A); return NULL;}
#define CHECK0(A)			if (0 == (int)(A))	{	CLog::error("%s(%d):%s(%s)",__FILE__, __LINE__,__FUNCTION__,#A); return 0;}
#define CHECK(A)			if (0 == (int)(A))	{	CLog::error("%s(%d):%s(%s)",__FILE__, __LINE__,__FUNCTION__,#A); return;}
#define CHECKL(A)			if (0 == (int)(A))	{	CLog::error("%s(%d):%s(%s)",__FILE__, __LINE__,__FUNCTION__,#A);}
#define CHECKC(A)			if (0 == (int)(A))	{	CLog::error("%s(%d):%s(%s)",__FILE__, __LINE__,__FUNCTION__,#A); continue;}
#define CHECK_FIELD(A, B)	if (0 == (int)(A))	{	CLog::error("表名(%s)标签名(%s) 字段(%s) 行号(%d) 错误 (%s)" , getXlsName().c_str(),pSheet->Value(),B, pData->getUInt32("Row"), #A); return false;}
#define CHECK_T_EMPTY(A)	if (0 == (int)(A))	{	CLog::warn("标签名(%s) is empty.", pSheet->Value(), #A); return true;}
#define CHECK_FATAL(A, B)	if (0 == (int)(A))	{	throw std::runtime_error(B);	}

//#############################################################
//############################## 指针操作
//#############################################################
#define safe_delete(ptr)		do { if (ptr)	delete(ptr);	(ptr) = NULL;	} while (false)
#define safe_delete_array(ptr)	do { if (ptr)	delete[](ptr);	(ptr) = NULL;	} while (false)