/*----------------- stl_vector.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2012/7/11 22:31:06
*--------------------------------------------------------------
*ʸ��������ʹ�������ڴ棬��ʹ��qsort������һ������
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
