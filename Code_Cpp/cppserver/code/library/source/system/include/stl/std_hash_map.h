/*----------------- std_hash_map.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.0
* Date         : 2012/2/24 15:58:14
*--------------------------------------------------------------
*stl hash_map��װ keyΪhash���㣬�����Ͱ�洢,�����ٶ�һ���map��
-�����ٶ���������map���
*------------------------------------------------------------*/
#pragma once
#ifdef WIN32 ///windows
//#include <hash_map>
#include <unordered_map>
#else	///linux
#include <ext/hash_map>
#endif//WIN32
#include "basic/basicTypes.h"

/*************************************************************/
// #pragma pack(push,1)
//------------------------------------------------------
template<class _Key,class _Tp>
class stl_hash_map	:
#ifdef WIN32//windows
	public std::unordered_map<_Key,_Tp>
#else//linux
	public __gnu_cxx::hash_map<_Key,_Tp>
#endif//WIN32
{
public:
#ifdef WIN32//windows
	typedef std::unordered_map<_Key,_Tp>				Parent;
	typedef std::pair<_Key,_Tp>						Pair;
#else//linux
	typedef __gnu_cxx::hash_map<_Key,_Tp>			Parent;
	typedef std::pair<typename __gnu_cxx::hash_map
						<_Key,_Tp>::iterator,bool>	_Pairib;
	typedef std::pair<_Key,_Tp>						Pair;
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
#endif//WIN32
		return __it;
	}

public:
	//--- ��������
	inline _Pairib insert_(const _Key& _key,const _Tp&_tp)
	{
		return Parent::insert(Pair(_key,_tp));

	}
	//--- hash_map��ѯ��ַ
	inline _Tp* find_	(const _Key& _key)
	{
		typename Parent::iterator _pos = Parent::find(_key);

		return ((_pos != Parent::end()) ? &(_pos->second) : NULL);
	}
	//--- hash_map��ѯ��ַ
	inline const _Tp* find_	(const _Key& _key)const
	{
		typename Parent::const_iterator _pos = Parent::find(_key);

		return ((_pos != Parent::end()) ? &(_pos->second) : NULL);
	}
	//--- �Ƿ����
	inline bool	is_exist	(const _Key& _key)const
	{
		typename Parent::const_iterator pos = Parent::find(_key);

		return (pos != Parent::end());
	}
	//--- �Ƿ����_B
	inline bool		is_contain	(Parent&_b)
	{
		typename Parent::iterator _end_= Parent::end();
		typename Parent::iterator _pos = _b.begin();
		typename Parent::iterator _end = _b.end();
		for (;_pos != _end;++_pos)
		{
			if(Parent::find(_pos->first) == _end_)
				return false;
		}
		return true;
	}

	//--- ��B����������׷�ӵ�������
	inline void		append	(Parent&_b)
	{
		typename Parent::iterator _pos = _b.begin();
		typename Parent::iterator _end = _b.end();
		for (;_pos != _end;++_pos)
			Parent::insert(Pair(_pos->first,_pos->second));
	}
};

//#############################################################
//############################## linux��hash����
//#############################################################
#ifndef WIN32
namespace __gnu_cxx  
{  
	template<>
	struct hash<int64>  
	{  
		size_t operator()(const int64& key) const  
		{  
			return (key >> 32) ^ key;
		}
	};

	template<>
	struct hash<uint64>  
	{  
		size_t operator()(const uint64& key) const  
		{  
			return (key >> 32) ^ key;
		}
	};

	template<class T>
	struct hash<T*>  
	{  
		size_t operator()(const T* key) const  
		{  
			return (size_t)key;
		}
	};

	template<>
	struct hash<std::string>  
	{  
		size_t operator()(const std::string& key) const  
		{  
			return __stl_hash_string(key.c_str());
		}
	};
}

//namespace std  
//{  
//	template<>
//	struct equal_to
//	{
//		bool operator()(const long long& lhs, const long long& rhs) const
//		{
//			return lhs == rhs;
//		}
//	};
//}
#endif//WIN32
//------------------------------------------------------
// #pragma pack(pop)
