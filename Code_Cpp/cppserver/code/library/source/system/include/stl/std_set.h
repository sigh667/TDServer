/*----------------- std_set.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2012/7/12 16:50:19
*--------------------------------------------------------------
*�����ظ�����������Ҫ�ظ�ʹ��multiset,����Ҳ��ѯЧ����map�൱
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
	//--- �Ƿ����
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
lower_bound()	����������ڻ����ĳֵ�ĵ�һ��Ԫ�ص�����(�����Լ����)
upper_bound()	����ָ�����ĳ��ֵԪ�صĵ�����(�������Լ����)
equal_range()	���ؼ����������ֵ��ȵ�����������������

/////�Զ���ȽϺ���
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
