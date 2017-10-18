/*------------- mapVector.h
* Copyright (C): 2011 Mokylin��Mokyqi
* Author       : ����Դ
* Version      : V1.01
* Date         : 2011/7/5 16:04:35
*
*/ 
/***************************************************************
* map��vector�����б�
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
	/*���������*/ 
	typedef	typename USE_MAP::reverse_iterator	reverse_iterator;

private:
	USE_MAP				m_mapUse;
	USE_VECTOR			m_vecUse;

public:
	CMapVector(){}
	virtual ~CMapVector(){}

public:
	inline void				clear		();								///������ݳ�
	inline int				insert		(const Key& key,const T& t);	///��������
	inline void				erase		(const Key& key);				///�ͷ�����
	inline void				erase		(iterator& pos);				///�ͷ�����
	inline void				erase		(reverse_iterator& pos);		///�ͷ�����
	inline iterator			begin		();								///���mapͷ
	inline reverse_iterator	rbegin		();								///��÷���mapͷ
	inline iterator			end			();								///���mapβ
	inline reverse_iterator	rend		();								///��÷���mapβ
	inline iterator			find		(const Key& key);				///����map������
	inline void				lowerBound	(iterator& pos,Key&key);		///�½�(�����Լ����)
	inline void				upperBound	(iterator& pos,Key&key);		///�Ͻ�(�������Լ����)
	inline size_t			size		();								///��С

public:
	inline int				findIndex	(const iterator& pos);			///��õ�������vector����
	inline int				findIndex	(const reverse_iterator& pos);	///��õ�������vector����
	inline int				findIndex	(const Key& key);				///���Key��vector����
	inline T				at			(int index);					///�����������ֵ
};
//------------------------------------------------------
//------------------------------ ������ݳ�
template <typename Key,typename T>
inline void	CMapVector<Key,T>::clear()
{
	m_mapUse.clear();
	m_vecUse.clear();

}
//------------------------------------------------------
//------------------------------ ��������
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
//------------------------------ �ͷ�����
template <typename Key,typename T>
inline void	CMapVector<Key,T>::erase			(const Key& key)
{
	iterator pos = m_mapUse.find(key);
	if(pos == m_mapUse.end())

	int index = find(key);
	m_mapUse.erase(key);
}
//------------------------------------------------------
//------------------------------ �ͷ�����
template <typename Key,typename T>
inline void	CMapVector<Key,T>::erase			(iterator& pos)
{
	m_mapUse.erase(pos);
}
//------------------------------------------------------
//------------------------------ �ͷ�����
template <typename Key,typename T>
inline void	CMapVector<Key,T>::erase			(reverse_iterator& pos)
{
	m_mapUse.erase(pos);
}
//------------------------------------------------------
//------------------------------ ���mapͷ
template <typename Key,typename T>
inline iterator	CMapVector<Key,T>::begin			()
{
	return m_mapUse.begin();
}
//------------------------------------------------------
//------------------------------ ��÷���mapͷ
template <typename Key,typename T>
inline reverse_iterator	CMapVector<Key,T>::rbegin		()
{
	return m_mapUse.rbegin();
}
//------------------------------------------------------
//------------------------------ ���mapβ
template <typename Key,typename T>
inline iterator	CMapVector<Key,T>::end			()
{
	return m_mapUse.end();
}
//------------------------------------------------------
//------------------------------ ��÷���mapβ
template <typename Key,typename T>
inline reverse_iterator	CMapVector<Key,T>::rend			()
{
	return m_mapUse.rend();
}
//------------------------------------------------------
//------------------------------ ����map������
template <typename Key,typename T>
inline iterator	CMapVector<Key,T>::find			(const Key& key)
{
	return m_mapUse.find(key);
}
//------------------------------------------------------
//------------------------------ �½�(�����Լ����)
template <typename Key,typename T>
inline void	CMapVector<Key,T>::lowerBound	(iterator& pos,Key&key)
{
	pos = m_mapUse.lower_bound(key);
}
//------------------------------------------------------
//------------------------------ �Ͻ�(�������Լ����)
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
//------------------------------ ��С
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
//------------------------------ ��õ�������vector����
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
//------------------------------ ��õ�������vector����
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
//------------------------------ ���Key��vector����
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
//------------------------------ �����������ֵ
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
