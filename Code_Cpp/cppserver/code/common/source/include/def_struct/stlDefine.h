/*------------- stlDefine.h
* Copyright (C): 2014 Mokylin·Mokyqi
* Author       : 陈亮
* Version      : V1.01
* Date         : 2014/18/11 20:00
*
*/ 
/***************************************************************
* 
***************************************************************/
#pragma once
#include "stl/stl_vector.h"
#include "stl/std_map.h"
#include "stl/std_set.h"
#include "stl/stl_list.h"
#include <string>
/*************************************************************/
//#############################################################
//############################## STL
//#############################################################
typedef stl_map<uint8,uint8>					MAP_U8_U8;
typedef stl_map<uint8,uint8>::iterator			ITER_MAP_U8_U8;
typedef stl_map<uint8,uint8>::const_iterator	CONST_ITER_MAP_U8_U8;
typedef stl_map<uint16,uint8>					MAP_U16_U8;
typedef stl_map<uint16,uint8>::iterator			ITER_MAP_U16_U8;
typedef stl_map<uint16,uint8>::const_iterator	CONST_ITER_MAP_U16_U8;
typedef stl_map<uint16,uint16>					MAP_U16_U16;
typedef stl_map<uint16,uint16>::iterator		ITER_MAP_U16_U16;
typedef stl_map<uint16,uint16>::const_iterator	CONST_ITER_MAP_U16_U16;
typedef stl_map<uint16,uint32>					MAP_U16_U32;
typedef stl_map<uint16,uint32>::iterator		ITER_MAP_U16_U32;
typedef stl_map<uint16,uint32>::const_iterator	CONST_ITER_MAP_U16_U32;
typedef stl_map<uint8,uint16>					MAP_U8_U16;
typedef stl_map<uint8,uint16>::iterator			ITER_MAP_U8_U16;
typedef stl_map<uint8,uint16>::const_iterator	CONST_ITER_MAP_U8_U16;
typedef stl_map<uint8,uint32>					MAP_U8_U32;
typedef stl_map<uint8,uint32>::iterator			ITER_MAP_U8_U32;
typedef stl_map<uint8,uint32>::const_iterator	CONST_ITER_MAP_U8_U32;
typedef stl_map<uint32,uint8>					MAP_U32_U8;
typedef stl_map<uint32,uint8>::iterator			ITER_MAP_U32_U8;
typedef stl_map<uint32,uint8>::const_iterator	CONST_ITER_MAP_U32_U8;
typedef stl_map<uint32,uint16>					MAP_U32_U16;
typedef stl_map<uint32,uint16>::iterator		ITER_MAP_U32_U16;
typedef stl_map<uint32,uint16>::const_iterator	CONST_ITER_MAP_U32_U16;
typedef stl_map<uint32,uint32>					MAP_U32_U32;
typedef stl_map<uint32,uint32>::iterator		ITER_MAP_U32_U32;
typedef stl_map<uint32,uint32>::const_iterator	CONST_ITER_MAP_U32_U32;
typedef stl_map<uint32,uint64>					MAP_U32_U64;
typedef stl_map<uint32,uint64>::iterator		ITER_MAP_U32_U64;
typedef stl_map<uint32,uint64>::const_iterator	CONST_ITER_MAP_U32_U64;
typedef stl_map<uint16,int32>					MAP_U16_I32;
typedef MAP_U16_I32::iterator					ITER_MAP_U16_I32;
typedef MAP_U16_I32::const_iterator				CONST_ITER_MAP_U16_I32;
typedef stl_map<uint16,int64>					MAP_U16_I64;
typedef MAP_U16_I64::iterator					ITER_MAP_U16_I64;
typedef MAP_U16_I64::const_iterator				CONST_ITER_MAP_U16_I64;
typedef stl_map<int32,int32>					MAP_I32_I32;
typedef MAP_I32_I32::iterator					ITER_MAP_I32_I32;
typedef MAP_I32_I32::const_iterator				CONST_ITER_MAP_I32_I32;
typedef stl_map<uint64,uint64>					MAP_U64_U64;
typedef MAP_U64_U64::iterator					ITER_MAP_U64_U64;
typedef MAP_U64_U64::const_iterator				CONST_ITER_MAP_U64_U64;
typedef stl_map<uint64,uint16>					MAP_U64_U16;
typedef MAP_U64_U16::iterator					ITER_MAP_U64_U16;
typedef MAP_U64_U16::const_iterator				CONST_ITER_MAP_U64_U16;
typedef stl_vector<uint16>						VEC_U16;
typedef stl_vector<uint16>::iterator			ITER_VEC_U16;
typedef stl_vector<uint16>::const_iterator		CONST_ITER_VEC_U16;
typedef stl_vector<uint8>						VEC_U8;
typedef stl_vector<uint8>::iterator				ITER_VEC_U8;
typedef stl_vector<uint8>::const_iterator		CONST_ITER_VEC_U8;
typedef stl_vector<uint32>						VEC_U32;
typedef stl_vector<uint32>::iterator			ITER_VEC_U32;
typedef stl_vector<uint32>::const_iterator		CONST_ITER_VEC_U32;
typedef stl_vector<uint64>						VEC_U64;
typedef stl_vector<uint64>::iterator			ITER_VEC_U64;
typedef stl_vector<uint64>::const_iterator		CONST_ITER_VEC_U64;

typedef stl_set<uint8>							SET_U8;
typedef stl_set<uint8>::iterator				ITER_SET_U8;
typedef stl_set<uint8>::const_iterator			CONST_ITER_SET_U8;
typedef stl_set<uint16>							SET_U16;
typedef stl_set<uint16>::iterator				ITER_SET_U16;
typedef stl_set<uint16>::const_iterator			CONST_ITER_SET_U16;
typedef stl_set<uint32>							SET_U32;
typedef stl_set<uint32>::iterator				ITER_SET_U32;
typedef stl_set<uint32>::const_iterator			CONST_ITER_SET_U32;
typedef stl_set<uint64>							SET_U64;
typedef stl_set<uint64>::iterator				ITER_SET_U64;
typedef stl_set<uint64>::const_iterator			CONST_ITER_SET_U64;

typedef stl_set<std::string>					SET_STRING;
typedef SET_STRING::iterator					ITER_SET_STRING;
typedef SET_STRING::const_iterator				CONST_ITER_SET_STRING;

typedef double_map<uint8,uint32,uint32>			DMAP_UINT8_32_32;
typedef stl_map<uint16,uint32>					MAP_NEED_EXPEND;
typedef stl_map<uint16,uint32>::iterator		ITER_MAP_NEED_EXPEND;
typedef stl_map<uint16,uint32>::const_iterator	CONST_ITER_MAP_NEED_EXPEND;
typedef double_map<uint32,uint16,uint8>			DMAP_NEED_ITEM;		//KEY1:道具ID KEY2:道具数量 key3:是否不绑定 0绑定
typedef double_map<uint32,uint16,uint8>::iterator		ITER_DMAP_NEED_ITEM;
typedef double_map<uint32,uint16,uint8>::const_iterator	CONST_ITER_DMAP_NEED_ITEM;
typedef double_map<uint32,uint16,uint16>				DMAP_NEED_BINGFU;		//KEY1:道具ID KEY2:兵符等级 key3:道具数量
typedef double_map<uint32,uint16,uint16>::iterator		ITER_DMAP_NEED_BINGFU;
typedef double_map<uint32,uint16,uint16>::const_iterator	CONST_ITER_DMAP_NEED_BINGFU;
typedef stl_map<uint32,uint16>					MAP_NEED_ITEM;		//KEY1:道具ID KEY2:道具数量 
typedef stl_map<uint32,uint16>::iterator		ITER_MAP_NEED_ITEM;
typedef stl_map<uint32,uint16>::const_iterator	CONST_ITER_MAP_NEED_ITEM;
typedef stl_list<uint8>					     	LIST_U8;