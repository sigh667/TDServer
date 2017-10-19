/*----------------- std_map.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/4/16 17:44:23
*--------------------------------------------------------------
*排序存储数据,插入无hash_map快,最小时遍历比hash_map快
*------------------------------------------------------------*/
#pragma once
#include <map>
#include "basic/basicTypes.h"
/*************************************************************/
// #pragma pack(push,1)
//------------------------------------------------------
template<class _Kty,class _Ty>
class stl_map	: public std::map<_Kty,_Ty>	
{
private:
	typedef std::map<_Kty,_Ty>	Parent;

public:
	typedef std::pair<_Kty,_Ty>	Pair;

public:
#ifndef WIN32//linux
	typedef std::pair<typename Parent::iterator,bool>	_Pairib;
#endif//WIN32

public:
	inline typename Parent::iterator	erase_(typename Parent::iterator __it)
	{
#ifdef WIN32//windows
		return erase(__it);

#else//linux
		typename Parent::iterator erase_it = __it;
		++__it;
		Parent::erase(erase_it);
		return __it;
#endif//WIN32
	}

public:
	//--- 插入数据
	inline _Pairib insert_		(const _Kty& _key,const _Ty&_ty)
	{
		return Parent::insert(Pair(_key,_ty));

	}
	//--- 查找地址
	inline _Ty*	begin_			()
	{
		typename Parent::iterator pos = Parent::begin();
		return ((pos != Parent::end()) ? &(pos->second) : NULL);
	}
	inline const _Ty*	begin_	()	const
	{
		typename Parent::const_iterator pos = Parent::begin();
		return ((pos != Parent::end()) ? &(pos->second) : NULL);
	}
	//--- 查找地址
	inline _Ty*	find_			(const _Kty& _key)
	{
		typename Parent::iterator pos = Parent::find(_key);
		return ((pos != Parent::end()) ? &(pos->second) : NULL);
	}
	inline const _Ty*	find_	(const _Kty& _key)	const
	{
		typename Parent::const_iterator pos = Parent::find(_key);
		return ((pos != Parent::end()) ? &(pos->second) : NULL);
	}
	//--- 查找下一个地址
	inline _Ty*	find_next		(const _Kty& _key)
	{
		typename Parent::iterator pos = Parent::upper_bound(_key);//返回map中第一个大于key的迭代器指针

		return ((pos != Parent::end()) ? &(pos->second) : NULL);
	}
	//--- 返回map中第一个大于等于key的迭代器指针
	inline const _Ty*	find_lowerbound		(const _Kty& _key) const
	{
		typename Parent::const_iterator pos = Parent::lower_bound(_key);

		return ((pos != Parent::end()) ? &(pos->second) : NULL);
	}
	//--- 查找上一个地址
	inline _Ty*	find_prev		(const _Kty& _key,bool _self = false)
	{
		if ( Parent::empty() )
			return NULL;

		typename Parent::iterator _pos	= Parent::lower_bound(_key);//返回map中第一个大于或等于key的迭代器指针
		if ( _pos == Parent::begin() )
		{
			if(_pos->first > _key)
				return NULL;

			if(_self && _pos->first == _key)
				return &(_pos->second);

			return NULL;
		}

		if(_pos == Parent::end() || !_self || _pos->first != _key)
			--_pos;
		
		return &_pos->second;
	}
	//--- 是否存在
	inline bool	is_exist	(const _Kty& _key)	const
	{
		typename Parent::const_iterator pos = Parent::find(_key);

		return (pos != Parent::end());
	}

	//--- 是否包含b
	inline bool	is_contain	(Parent&_b) const
	{
		typename Parent::const_iterator _end_= Parent::end();
		typename Parent::const_iterator _pos = _b.begin();
		typename Parent::const_iterator _end = _b.end();
		for (;_pos != _end;++_pos)
		{
			if(Parent::find(_pos->first) == _end_)
				return false;
		}
		return true;
	}
	//--- 把B中所有数据追加容器中
	inline void		append	(Parent&_b)
	{
		typename Parent::iterator _pos = _b.begin();
		typename Parent::iterator _end = _b.end();
		for (;_pos != _end;++_pos)
			Parent::insert(Pair(_pos->first,_pos->second));
	}
	//--- map下界反向(包含自己向前)
	inline typename Parent::iterator rlower_bound	(const _Kty& _key)
	{
		typename Parent::iterator _begin= Parent::begin();
		typename Parent::iterator _end	= Parent::end();
		if(_begin == _end)//空容器
			return _end;

		typename Parent::iterator _pos	= Parent::lower_bound(_key);/*返回批向大于或等于某值的第一个元素迭代器(包含自己向后)*/ 
		if (_pos==_end)
			return _end;

		if(_pos->first == _key)//不是结尾且查到自己
			return _pos;

		return --_pos;
	}
	//-------------------------------------------------------------
	//------------------------------ 上界反向(不包含自己向前)
	inline typename Parent::iterator rupper_bound	(const _Kty& _key)
	{
		typename Parent::iterator _begin= Parent::begin();
		typename Parent::iterator _end	= Parent::end();
		if(_begin == _end )//空容器
			return _end;

		typename Parent::iterator _pos	= Parent::lower_bound(_key);/*返回批向大于某值的第一个元素迭代器(包含自己向后)*/ 
		if(_pos == _end)
			return _end;

		return --_pos;
	}
};

//#############################################################
//############################## 自定义-双map
//#############################################################
template<class _Kty1,class _Kty2,class _Ty>
class double_map
{
public:
	typedef stl_map<_Kty2,_Ty>		_sub_map;	//子map
	typedef stl_map<_Kty1,_sub_map>	_main_map;	//主map

public:
	typedef typename _main_map::iterator		iterator;
	typedef typename _main_map::const_iterator	const_iterator;
	typedef typename _sub_map::iterator			sub_iterator;
	typedef typename _sub_map::const_iterator	sub_const_iterator;

private:
	_main_map	__main_map;

public:
	inline _main_map&		main_map()					{	return __main_map;				}
	inline void				clear	()					{	__main_map.clear();				}
	inline uint32			size	()	const			{	return __main_map.size();		}
	inline iterator			begin	()					{	return __main_map.begin();		}
	inline const_iterator	begin	()	const			{	return __main_map.begin();		}
	inline iterator			end		()					{	return __main_map.end();		}
	inline const_iterator	end		()	const			{	return __main_map.end();		}
	inline iterator			find	(const _Kty1&_k1)	{	return __main_map.find(_k1);	}
	inline const_iterator	find	(const _Kty1&_k1)	const	{	return __main_map.find(_k1);	}
	inline iterator			erase	(const _Kty1&_k1)	{	return __main_map.erase_(_k1);	}
	inline iterator			erase	(iterator&_pos)		{	return __main_map.erase_(_pos);	}

public:
	//--- 
	inline _Ty*	insert					(const _Kty1&_k1,const _Kty2&_k2,const _Ty&_k3)
	{
		typename _main_map::_Pairib _pai = __main_map.insert_(_k1,_sub_map());
		if(_pai.first == __main_map.end())
			return NULL;

		_sub_map&_sub						= (_pai.first)->second;
		typename _sub_map::_Pairib _s_pai	= _sub.insert_(_k2,_k3);
		if(_s_pai.first == _sub.end())
			return NULL;

		return &((_s_pai.first)->second);
	}
	//--- 
	inline void	erase					(const _Kty1&_k1,const _Kty2&_k2)
	{
		iterator _pos = __main_map.find(_k1);
		if(_pos == __main_map.end())
			return;

		_sub_map&_sub = _pos->second;

		_sub.erase(_k2);
		if(_sub.empty())
			__main_map.erase(_pos);
	}
	//--- 
	inline _sub_map*	find_			(const _Kty1&_k1)
	{
		iterator _pos = __main_map.find(_k1);
		if(_pos == __main_map.end())
			return NULL;

		return &(_pos->second);
	}
	//--- 
	inline const _sub_map*	find_(const _Kty1&_k1)	const
	{
		const_iterator _pos = __main_map.find(_k1);
		if(_pos == __main_map.end())
			return NULL;

		return &(_pos->second);
	}
	//--- 
	inline _Ty*			begin_			(const _Kty1&_k1)
	{
		iterator _pos = __main_map.find(_k1);
		if(_pos == __main_map.end())
			return NULL;

		_sub_map&_sub = _pos->second;
		return _sub.begin_();
	}
	//--- 
	inline _Ty*			find_			(const _Kty1&_k1,const _Kty2&_k2)
	{
		iterator _pos = __main_map.find(_k1);
		if(_pos == __main_map.end())
			return NULL;

		_sub_map&_sub = _pos->second;
		return _sub.find_(_k2);
	}
	//--- 
	inline const _Ty*	find_			(const _Kty1&_k1,const _Kty2&_k2)	const
	{
		const_iterator _pos = __main_map.find(_k1);
		if(_pos == __main_map.end())
			return NULL;

		const _sub_map&_sub = _pos->second;
		return _sub.find_(_k2);
	}
	//--- 查找下一个地址
	inline _Ty*			find_next		(const _Kty1&_k1,const _Kty2&_k2)
	{
		iterator _pos = __main_map.find(_k1);
		if(_pos == __main_map.end())
			return NULL;

		_sub_map&		_sub	= _pos->second;
		sub_iterator	s_pos	= _sub.upper_bound(_k2);

		return ((s_pos != _sub.end()) ? &(s_pos->second) : NULL);
	}
	//--- 查找上一个地址
	inline _Ty*			find_prev		(const _Kty1&_k1,const _Kty2&_k2,bool _self = false)
	{
		iterator _pos = __main_map.find(_k1);
		if(_pos == __main_map.end())
			return NULL;

		_sub_map&		_sub	= _pos->second;
		if(_sub.empty())
			return NULL;

		sub_iterator	s_pos	= _sub.lower_bound(_k2);
		if ( s_pos == _sub.begin() )
		{
			if(s_pos->first > _k2)
				return NULL;

			if(_self && s_pos->first == _k2)
				return &(s_pos->second);

			return NULL;
		}

		if(s_pos == _sub.end() || !_self || s_pos->first != _k2)
			--s_pos;

		return &s_pos->second;
	}
	//--- 大小
	inline uint32		sub_size		(const _Kty1&_k1)
	{
		iterator _pos = __main_map.find(_k1);
		if(_pos == __main_map.end())
			return 0;

		_sub_map&_sub = _pos->second;
		return _sub.size();
	}
	//--- 大小
	inline uint32		get_total_size	()
	{
		uint32 uSize = 0;
		iterator stpos = __main_map.begin();
		for (; stpos != __main_map.end(); ++stpos)
		{
			_sub_map& stSubmap = stpos->second;
			uSize += stSubmap.size();
		}

		return uSize;
	}
};

//------------------------------------------------------
// #pragma pack(pop)
