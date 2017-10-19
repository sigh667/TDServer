/*----------------- std_set.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/7/12 16:50:19
*--------------------------------------------------------------
*有序不重复容器，如需要重复使用multiset,插入也查询效率与map相当
*------------------------------------------------------------*/
#pragma once
#include <set>
#include "basic/basicTypes.h"
/*************************************************************/
// #pragma pack(push,1)
//------------------------------------------------------
template<class _Kty/*,class _Pr = std::less<_Kty>,class _Alloc = std::allocator<_Kty>*/ >
class stl_set	: public std::set<_Kty/*,_Pr,_Alloc*/>
{
public:
	typedef std::set<_Kty/*,_Pr,_Alloc*/>	Parent;

public:
	//--- 是否存在
	inline bool	is_exist	(const _Kty& _key)const
	{
		typename Parent::const_iterator pos = Parent::find(_key);

		return (pos != Parent::end());
	}
};

/*
begin()
end()
rbegin()
rend()
erase()
insert()
find()
clear()
size()
count()
empty()
swap()
lower_bound()	返回批向大于或等于某值的第一个元素迭代器(包含自己向后)
upper_bound()	返回指向大于某个值元素的迭代器(不包含自己向后)
equal_range()	返回集合中与给定值相等的上下限两个迭代器

/////自定义比较函数
struct _struct
{
	int a;
	int b;
};
struct compare()
{
	bool operator()(const _struct&a,const _struct&b)const
	{
		return a.a < b.a;
	}
};
stl_set<type,compare>xx;
*/
//------------------------------------------------------
// #pragma pack(pop)
