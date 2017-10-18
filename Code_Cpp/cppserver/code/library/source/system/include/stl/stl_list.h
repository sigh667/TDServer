/*----------------- stl_list.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/7/11 22:27:34
*--------------------------------------------------------------
*尽量使用stl_deque替换,list效率弱
*------------------------------------------------------------*/
#pragma once
#include <list>
#include "basic/basicTypes.h"
/*************************************************************/
// #pragma pack(push,1)
//------------------------------------------------------
template<class _Ty>
class stl_list	: public std::list<_Ty>
{
public:
	typedef std::list<_Ty>	Parent;

public:
};
//------------------------------------------------------
// #pragma pack(pop)
