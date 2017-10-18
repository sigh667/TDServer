/*----------------- std_deque.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/7/11 22:37:14
*--------------------------------------------------------------
*双端队列容器，采用分块的线性结果来存储数据,具有高效的删除首尾元素的函数
*-可采用数组或迭代器的方式进行遍历访问
*---
- push_back()
- push_front()
- iterator insert(iterator pos, const T& x)
- pop_front()	删除deque的第一个元素
- pop_back()	删除deque的最后一个元素
- swap(deque&)	deque的交换：
*************
vector deque 在使用迭代器遍历中删除对像时不可以使用已保存的end()
*------------------------------------------------------------*/
#pragma once
#include <deque>
#include "basic/basicTypes.h"
/*************************************************************/
// #pragma pack(push,1)
//------------------------------------------------------
template<class _Ty>
class stl_deque	: public std::deque<_Ty>
{
public:
	typedef std::deque<_Ty>	Parent;

public:
};
//------------------------------------------------------
// #pragma pack(pop)
