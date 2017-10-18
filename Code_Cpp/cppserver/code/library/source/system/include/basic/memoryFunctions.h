/*------------- memoryFunctions.h
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.01
* Date         : 2011/1/29 17:49:50
*
*/ 
/***************************************************************
* 内存处理函数
***************************************************************/
#pragma once
#include <iostream>
#include <stdio.h>
#include "basic/basicTypes.h"
/*************************************************************/
//------------------------------------------------------
//--- 清除内存
extern void*	dMemset(void * _Dst,int _Val,size_t _Size);
//------------------------------------------------------
//--- 内存比较
extern int32	dMemcmp(const void * _Buf1,const void * _Buf2,size_t _Size);

#ifdef WIN32
//------------------------------------------------------
//--- 内存拷贝
extern int32	dMemcpy(void* _Dst,size_t _DstSize,const void* _Src,size_t _MaxCount);

//------------------------------------------------------
//--- 内存移动
extern int32	dMemmove(void* _Dst,size_t _DstSize,const void* _Src,size_t _MaxCount);
#else//WIN32
//------------------------------------------------------
//--- 内存拷贝
extern void*	dMemcpy(void* _Dst,size_t _DstSize,const void* _Src,size_t _MaxCount);

//------------------------------------------------------
//--- 内存移动
extern void*	dMemmove(void* _Dst,size_t _DstSize,const void* _Src,size_t _MaxCount);
#endif//WIN32
