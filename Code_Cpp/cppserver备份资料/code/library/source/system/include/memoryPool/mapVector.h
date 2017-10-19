/*------------- mapVector.h
* Copyright (C): 2011 Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.01
* Date         : 2011/7/5 16:04:35
*
*/ 
/***************************************************************
* map与vector符合列表
***************************************************************/
#pragma once
#include "stl/std_map.h"
#include "stl/stl_vector.h"
/*************************************************************/
template <typename Key,typename T>
class CMapVector
{
private:
	struct _stNode
	{
		T		data;
		int	index;

		_stNode(T t,int i = -1)
		{
			data = t;
			index = i;
		};
	};
private:
	typedef	stl_map<Key,_stNode*>				USE_MAP;
	typedef	stl_vector<_stNode*>				USE_VECTOR;

public:
	typedef	typename USE_MAP::iterator			iterator; 
	/*反向迭代器*/ 
	typedef	typename USE_MAP::reverse_iterator	reverse_iterator;

private:
	USE_MAP				m_mapUse;
	USE_VECTOR			m_vecUse;

public:
	CMapVector(){}
	virtual ~CMapVector(){}

public:
	inline void				clear		();								///清除数据池
	inline int				insert		(const Key& key,const T& t);	///分配数据
	inline void				erase		(const Key& key);				///释放数据
	inline void				erase		(iterator& pos);				///释放数据
	inline void				erase		(reverse_iterator& pos);		///释放数据
	inline iterator			begin		();								///获得map头
	inline reverse_iterator	rbegin		();								///获得反向map头
	inline iterator			end			();								///获得map尾
	inline reverse_iterator	rend		();								///获得反向map尾
	inline iterator			find		(const Key& key);				///查找map迭代器
	inline void				lowerBound	(iterator& pos,Key&key);		///下界(包含自己向后)
	inline void				upperBound	(iterator& pos,Key&key);		///上界(不包含自己向后)
	inline size_t			size		();								///大小

public:
	inline int				findIndex	(const iterator& pos);			///获得迭代器在vector索引
	inline int				findIndex	(const reverse_iterator& pos);	///获得迭代器在vector索引
	inline int				findIndex	(const Key& key);				///获得Key在vector索引
	inline T				at			(int index);					///获得向量索引值
};
//------------------------------------------------------
//------------------------------ 清除数据池
template <typename Key,typename T>
inline void	CMapVector<Key,T>::clear()
{
	m_mapUse.clear();
	m_vecUse.clear();

}
//------------------------------------------------------
//------------------------------ 分配数据
template <typename Key,typename T>
inline int	CMapVector<Key,T>::insert		(const Key& key,const T& t)
{
	USE_MAP::_Pairib pairib = m_mapUse.insert_(key,_stNode(t));
	if(!pairib.second)
	{
		if(pairib.first == m_mapUse.end())
			return -1;
		_stNode&stNode = (pairib.first)->second;

		return stNode.index;
	}
	_stNode&stNode = (pairib.first)->second;
	m_vecUse.push_back(stNode);

	stNode.index = (m_vecUse.size() - 1);

	return stNode.index;
}

//------------------------------------------------------
//------------------------------ 释放数据
template <typename Key,typename T>
inline void	CMapVector<Key,T>::erase			(const Key& key)
{
	iterator pos = m_mapUse.find(key);
	if(pos == m_mapUse.end())

	int index = find(key);
	m_mapUse.erase(key);
}
//------------------------------------------------------
//------------------------------ 释放数据
template <typename Key,typename T>
inline void	CMapVector<Key,T>::erase			(iterator& pos)
{
	m_mapUse.erase(pos);
}
//------------------------------------------------------
//------------------------------ 释放数据
template <typename Key,typename T>
inline void	CMapVector<Key,T>::erase			(reverse_iterator& pos)
{
	m_mapUse.erase(pos);
}
//------------------------------------------------------
//------------------------------ 获得map头
template <typename Key,typename T>
inline iterator	CMapVector<Key,T>::begin			()
{
	return m_mapUse.begin();
}
//------------------------------------------------------
//------------------------------ 获得反向map头
template <typename Key,typename T>
inline reverse_iterator	CMapVector<Key,T>::rbegin		()
{
	return m_mapUse.rbegin();
}
//------------------------------------------------------
//------------------------------ 获得map尾
template <typename Key,typename T>
inline iterator	CMapVector<Key,T>::end			()
{
	return m_mapUse.end();
}
//------------------------------------------------------
//------------------------------ 获得反向map尾
template <typename Key,typename T>
inline reverse_iterator	CMapVector<Key,T>::rend			()
{
	return m_mapUse.rend();
}
//------------------------------------------------------
//------------------------------ 查找map迭代器
template <typename Key,typename T>
inline iterator	CMapVector<Key,T>::find			(const Key& key)
{
	return m_mapUse.find(key);
}
//------------------------------------------------------
//------------------------------ 下界(包含自己向后)
template <typename Key,typename T>
inline void	CMapVector<Key,T>::lowerBound	(iterator& pos,Key&key)
{
	pos = m_mapUse.lower_bound(key);
}
//------------------------------------------------------
//------------------------------ 上界(不包含自己向后)
template <typename Key,typename T>
inline void	CMapVector<Key,T>::upperBound	(iterator& pos,Key&key)
{
	try
	{
		pos = m_mapUse.upper_bound(key);
	}
	catch (...)
	{
	}
}
//------------------------------------------------------
//------------------------------ 大小
template <typename Key,typename T>
inline size_t	CMapVector<Key,T>::size			()
{
	try
	{
		return m_mapUse.size();
	}
	catch (...)
	{
	}
	return 0;
}

//------------------------------------------------------
//------------------------------ 获得迭代器在vector索引
template <typename Key,typename T>
inline int	CMapVector<Key,T>::findIndex	(const iterator& pos)
{
	try
	{
		if(pos != m_mapUse.end())
			return (pos->second).index;
	}
	catch (...)
	{
	}
	return -1;
}
//------------------------------------------------------
//------------------------------ 获得迭代器在vector索引
template <typename Key,typename T>
inline int	CMapVector<Key,T>::findIndex	(const reverse_iterator& pos)
{
	try
	{
		if(pos != m_mapUse.rend())
			return (pos->second).index;
	}
	catch (...)
	{
	}
	return -1;
}
//------------------------------------------------------
//------------------------------ 获得Key在vector索引
template <typename Key,typename T>
inline int	CMapVector<Key,T>::findIndex	(const Key& key)
{
	try
	{
		iterator pos = m_mapUse.find(key);
		if(pos != m_mapUse.end())
			return (pos->second).index;
	}
	catch (...)
	{
	}
	return -1;
}
//------------------------------------------------------
//------------------------------ 获得向量索引值
template <typename Key,typename T>
inline T	CMapVector<Key,T>::at				(int index)
{
	try
	{
		if(m_vecUse.size() > index)
			return m_vecUse[index];
	}
	catch (...)
	{
	}
	return T;
}
