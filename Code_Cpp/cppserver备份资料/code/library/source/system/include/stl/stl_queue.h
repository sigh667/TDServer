/*----------------- stl_queue.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/7/11 22:34:20
*--------------------------------------------------------------
*单向队列,先进先出
-量少时加入比deque快,量大时不如deque
*------------------------------------------------------------*/
#pragma once
#include <queue>
#include "basic/basicTypes.h"
/*************************************************************/
// #pragma pack(push,1)
//------------------------------------------------------
template<class _Ty>
class stl_queue	: public std::queue<_Ty>
{
public:
	typedef std::queue<_Ty>	Parent;

public:
};
//------------------------------------------------------
// #pragma pack(pop)
