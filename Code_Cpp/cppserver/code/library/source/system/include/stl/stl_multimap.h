/*----------------- stl_multimap.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2012/7/24 10:37:58
*--------------------------------------------------------------
*���ֵ����
*equal_range(key)���ʹ��_range.first=begin;_range.second=end;
*------------------------------------------------------------*/
#pragma once
#include <map>
#include "basic/basicTypes.h"
/*************************************************************/
template<class _Kty,class _Ty>
class stl_multimap	: public std::multimap<_Kty,_Ty>
{
private:
	typedef std::multimap<_Kty,_Ty>	Parent;

public:
	typedef std::pair<_Kty,_Ty>	Pair;

public:
#ifndef WIN32//linux
	typedef std::pair<typename Parent::iterator,bool>						_Pairib;
	typedef std::pair<typename Parent::iterator, typename Parent::iterator>	_Pairii;
#endif//WIN32

public:
	//--- ��������
	inline void	insert_	(const _Kty& _key,const _Ty&_ty)
	{
		Parent::insert(std::make_pair(_key,_ty));
	}
};
