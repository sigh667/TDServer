/*----------------- stl_list.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2012/7/11 22:27:34
*--------------------------------------------------------------
*����ʹ��stl_deque�滻,listЧ����
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
