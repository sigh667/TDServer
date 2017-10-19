/*----------------- stl_vector.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/7/11 22:31:06
*--------------------------------------------------------------
*矢量容器，使用连续内存，可使用qsort像数组一样排序
*------------------------------------------------------------*/
#pragma once
#include <vector>
#include "basic/basicTypes.h"
/*************************************************************/
// #pragma pack(push,1)
//------------------------------------------------------
template<class _Ty>
class stl_vector	: public std::vector<_Ty>
{
public:
	typedef std::vector<_Ty>	Parent;

public:
};
//------------------------------------------------------
// #pragma pack(pop)
