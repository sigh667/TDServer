/*----------------- mapPool.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/8/20 20:45:19
*--------------------------------------------------------------
*不排序，不频繁遍历时不使用
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "stl/stl_list.h"
#include "memoryPool/mem_malloc.h"
#include "threads/csLocker.h"
#include "poolVal.h"
/*************************************************************/
template <typename Key, typename T, typename _ValType = _Val_Init<T>, int COUNT = 0>
class CMapPool
{
private:
	typedef	stl_map<Key,T*>					USE_POOL;
#ifdef _USE_NEDMALLOC
#else//_USE_NEDMALLOC
	typedef stl_list<T*>					LIST;
#endif//_USE_NEDMALLOC

public:
	typedef	typename USE_POOL::iterator			iterator; 
	typedef	typename USE_POOL::const_iterator	const_iterator;
	/*反向迭代器*/ 
	typedef	typename USE_POOL::reverse_iterator	reverse_iterator;

#ifdef _USE_NEDMALLOC
#else//_USE_NEDMALLOC
	typedef typename LIST::iterator	list_iterator;
#endif//_USE_NEDMALLOC

private:
	CMutex_			m_csLock;
	USE_POOL		m_UseList;

#ifdef _USE_NEDMALLOC
public:
	mem_malloc_declaration();
#else//_USE_NEDMALLOC
private:
	volatile int	m_nCount;
	volatile int	m_nMaxFree;
	LIST			m_FreeList;
#endif//_USE_NEDMALLOC

public:
	CMapPool();
	virtual ~CMapPool();

private:
	inline void	_build	();
	inline T*	_malloc	();
	inline void	_free	(T* p_t);

public:
	inline CMutex_&			getLock		()						{	return m_csLock;				}
#ifdef _USE_NEDMALLOC
#else//_USE_NEDMALLOC
	inline void				setMaxFree	(int nCount)			{	m_nMaxFree = nCount;			}
	inline void				setPoolCount(int nCount)			{	m_nCount = nCount;				}
#endif//_USE_NEDMALLOC
	inline bool				empty		()const					{	return m_UseList.empty();		}
	inline bool				exist		(const Key& key)const	{	return m_UseList.is_exist(key);	}
	inline size_t			size		()const 				{	return m_UseList.size();		} 
	inline iterator			begin		()						{	return m_UseList.begin();		}
	inline iterator			end			()						{	return m_UseList.end();			}
	inline const_iterator	begin		()const					{	return m_UseList.begin();		}
	inline const_iterator	end			()const					{	return m_UseList.end();			}
	inline reverse_iterator	rbegin		()						{	return m_UseList.rbegin();		}
	inline reverse_iterator	rend		()						{	return m_UseList.rend();		}

public:
	void		initPool	();

public:
	T*			allocate	(const Key& key);
	bool		release		(const Key& key);
	bool		release		(iterator& pos);
	void		clear		();
	void		getHead		(iterator& pos);
	void		getEnd		(iterator& pos);
	void		getHead		(reverse_iterator& pos);
	T*			getHead		();
	/*----->{ 下界(包含自己向后) }*/ 
	void		lowerBound	(iterator& pos,Key&key);
	/*----->{ 上界(不包含自己向后) }*/ 
	void		upperBound	(iterator& pos,Key&key);
	T*			getNext		(iterator& pos);
	T*			getNext		(reverse_iterator& pos);
	bool		getKey		(iterator& pos,Key& key);
	bool		getKey		(reverse_iterator& pos,Key& key);
	T*			getData		(const iterator& pos);
	T*			getData		(const reverse_iterator& pos);
	T*			find		(const Key& key);
	const T*	find		(const Key& key) const;
	bool		is_exist	(const Key& key);
	bool		isTail		(const iterator& pos);
	bool		isTail		(const reverse_iterator& pos);
};
#ifdef _USE_NEDMALLOC
template <typename Key,typename T,typename _ValType,int COUNT>
mem_malloc	CMapPool<Key,T,_ValType,COUNT>::__malloc;
#endif//_USE_NEDMALLOC
//------------------------------------------------------
//------------------------------ 
template <typename Key,typename T,typename _ValType,int COUNT>
CMapPool<Key,T,_ValType,COUNT>::CMapPool()
{
#ifdef _USE_NEDMALLOC
#else//_USE_NEDMALLOC
	m_nCount	= COUNT;
	m_nMaxFree	= COUNT;
#endif//_USE_NEDMALLOC
}
//------------------------------------------------------ 
//------------------------------ 
template <typename Key,typename T,typename _ValType,int COUNT>
CMapPool<Key,T,_ValType,COUNT>::~CMapPool()
{
	initPool();
}
//------------------------------------------------------ 
//------------------------------ 
template <typename Key,typename T,typename _ValType,int COUNT>
void	CMapPool<Key,T,_ValType,COUNT>::_build()
{
#ifdef _USE_NEDMALLOC
#else//_USE_NEDMALLOC
	T*	lp_t = NULL;
	try
	{
		/*分配m_nCount个内存块*/ 
		for( int i = 0; i < m_nCount; i++ )
		{
			lp_t = new T;
			m_FreeList.push_back(lp_t);
		}/*end for(i)*/ 
	}
	catch (...)
	{
	}
#endif//_USE_NEDMALLOC
}
//-------------------------------------------------------------
//------------------------------ 
template <typename Key,typename T,typename _ValType,int COUNT>
inline T*	CMapPool<Key,T,_ValType,COUNT>::_malloc	()
{
	T* p_t = NULL;
	try
	{
#ifdef _USE_NEDMALLOC
		p_t = new T;
#else//_USE_NEDMALLOC
		if (m_FreeList.empty())
			_build();
		
		if (!m_FreeList.empty())
		{
			p_t = m_FreeList.front();
			m_FreeList.pop_front();
		}

		if (!p_t)
			p_t = new T;
#endif//_USE_NEDMALLOC
	}
	catch(...)
	{
	}
	return p_t;
}

//------------------------------------------------------ 
//------------------------------ 
template <typename Key,typename T,typename _ValType,int COUNT>
void	CMapPool<Key,T,_ValType,COUNT>::_free(T* p_t)
{
	if (!p_t)
		return;

	try
	{
#ifdef _USE_NEDMALLOC
		safe_delete(p_t);
#else//_USE_NEDMALLOC
		if (m_nMaxFree > 0 && (int)m_FreeList.size() > m_nMaxFree)
		{
			safe_delete(p_t);
		}
		else
		{
			//_ValType::initialize(p_t);
			m_FreeList.push_front(p_t);
		}
#endif//_USE_NEDMALLOC
	}
	catch(...)
	{
	}
}

//------------------------------------------------------ 
//------------------------------ 
template <typename Key,typename T,typename _ValType,int COUNT>
void	CMapPool<Key,T,_ValType,COUNT>::initPool()
{
	iterator	pos;
	try
	{
		for(pos = m_UseList.begin();pos != m_UseList.end();++pos)
		{
			safe_delete(pos->second);
		}
#ifdef _USE_NEDMALLOC
#else//_USE_NEDMALLOC
		for (list_iterator iter = m_FreeList.begin(); iter != m_FreeList.end(); ++iter)
		{
			safe_delete(*iter);
		}
		m_FreeList.clear();
#endif//_USE_NEDMALLOC
	}
	catch(...)
	{
	}
	m_UseList.clear();
}

//------------------------------------------------------ 
//------------------------------ 
template <typename Key,typename T,typename _ValType,int COUNT>
T*		CMapPool<Key,T,_ValType,COUNT>::allocate(const Key& key)
{
	T*	lp_t = NULL;
	try
	{	
		/*避免重复Key再次分配*/ 
		iterator _pos = m_UseList.find(key);
		if (_pos != m_UseList.end())
		{
			lp_t = _pos->second;
		}
		else
		{
#ifdef _USE_NEDMALLOC
			lp_t = new T;
#else//_USE_NEDMALLOC
			lp_t = _malloc();
#endif//_USE_NEDMALLOC

			if (lp_t)
			{
				typename USE_POOL::_Pairib _Pair = m_UseList.insert_(key,lp_t);
				if (!_Pair.second)
				{
					//dPrintf("CMapPool::allocate()插入队列异常!");
					_free(lp_t);
					lp_t = NULL;
				}
			}
			/*
			if(lp_t)
				new (lp_t) T(); 
			*/
		}
	}
	catch (...)
	{
	}
	_ValType::initialize(lp_t);
	return lp_t;
}
//------------------------------------------------------ 
//------------------------------ 
template <typename Key,typename T,typename _ValType,int COUNT>
bool	CMapPool<Key,T,_ValType,COUNT>::release(const Key& key)
{
	try
	{
		iterator pos = m_UseList.find(key);
		if (pos != m_UseList.end())
		{
			_free(pos->second);
			m_UseList.erase(pos);

			return true;
		}
	}
	catch(...)
	{
	}

	return false;
}
//------------------------------------------------------ 
//------------------------------ 
template <typename Key,typename T,typename _ValType,int COUNT>
bool	CMapPool<Key,T,_ValType,COUNT>::release(iterator& pos)
{
	try
	{
		if (pos != m_UseList.end())
		{
			_free(pos->second);
			pos = m_UseList.erase_( pos );
			return true;
		}
	}
	catch (...)
	{
	}
	return false;
}

//------------------------------------------------------ 
//------------------------------ 
template <typename Key,typename T,typename _ValType,int COUNT>
void	CMapPool<Key,T,_ValType,COUNT>::clear()
{
	try
	{
		T*		lp_t= NULL;
		iterator pos= m_UseList.begin();
		for ( ; pos != m_UseList.end(); ++pos )
		{
			lp_t = pos->second;
#ifdef _USE_NEDMALLOC
			safe_delete(lp_t);
#else//_USE_NEDMALLOC
			if(m_nMaxFree > 0 && m_FreeList.size() > (size_t)m_nMaxFree)
			{
				safe_delete(lp_t);
			}
			else
			{
				m_FreeList.push_back(lp_t);
			}
#endif//_USE_NEDMALLOC
		}
	}
	catch (...)
	{
	}
	m_UseList.clear();
}

//------------------------------------------------------ 
//------------------------------ 
template <typename Key,typename T,typename _ValType,int COUNT>
void	CMapPool<Key,T,_ValType,COUNT>::getHead(iterator& pos)
{
	try
	{
		pos = m_UseList.begin();
	}
	catch (...)
	{
	}
}
//------------------------------------------------------ 
//------------------------------ 
template <typename Key,typename T,typename _ValType,int COUNT>
void	CMapPool<Key,T,_ValType,COUNT>::getEnd(iterator& pos)
{
	try
	{
		pos = m_UseList.end();
	}
	catch (...)
	{
	}
}

//------------------------------------------------------ 
template <typename Key,typename T,typename _ValType,int COUNT>
void	CMapPool<Key,T,_ValType,COUNT>::getHead(reverse_iterator& pos)
{
	try
	{
		pos = m_UseList.rbegin();
	}
	catch (...)
	{
	}
}

//------------------------------------------------------ 
//------------------------------ 
template <typename Key,typename T,typename _ValType,int COUNT>
T*	CMapPool<Key,T,_ValType,COUNT>::getHead()
{
	T*	lp_t = NULL;
	try
	{
		iterator pos = m_UseList.begin();
		if(pos != m_UseList.end())
			lp_t = pos->second;
	}
	catch (...)
	{

	}

	return lp_t;
}
//------------------------------------------------------ 
/*----->{ 下界(包含自己向后) }*/ 
template <typename Key,typename T,typename _ValType,int COUNT>
void	CMapPool<Key,T,_ValType,COUNT>::lowerBound(iterator& pos,Key&key)
{
	try
	{
		pos = m_UseList.lower_bound(key);
	}
	catch (...)
	{

	}
}
//------------------------------------------------------ 
/*----->{ 上界(不包含自己向后) }*/ 
template <typename Key,typename T,typename _ValType,int COUNT>
void	CMapPool<Key,T,_ValType,COUNT>::upperBound(iterator& pos,Key&key)
{
	try
	{
		pos = m_UseList.upper_bound(key);
	}
	catch (...)
	{

	}
}

//------------------------------------------------------ 
//------------------------------ 
template <typename Key,typename T,typename _ValType,int COUNT>
T*		CMapPool<Key,T,_ValType,COUNT>::getNext(iterator& pos)
{
	T*	lp_t = NULL;
	try
	{
		if(pos != m_UseList.end())
		{
			lp_t = pos->second;
			++pos;
		}/*end if(pos)*/ 
	}
	catch (...)
	{
		return NULL;
	}

	return lp_t;
}
//------------------------------------------------------ 
//------------------------------ 
template <typename Key,typename T,typename _ValType,int COUNT>
T*		CMapPool<Key,T,_ValType,COUNT>::getNext(reverse_iterator& pos)
{
	T*	lp_t = NULL;
	try
	{
		if(pos != m_UseList.rend())
		{
			lp_t = pos->second;
			++pos;
		}/*end if(pos)*/ 
	}
	catch (...)
	{
		return NULL;
	}

	return lp_t;
}
//------------------------------------------------------ 
//------------------------------ 
template <typename Key,typename T,typename _ValType,int COUNT>
bool		CMapPool<Key,T,_ValType,COUNT>::getKey(iterator& pos,Key& key)
{
	try
	{
		if(pos != m_UseList.end())
		{
			key = pos->first;
			return true;
		}/*end if(pos)*/ 
	}
	catch (...)
	{
	}
	return false;
}
//------------------------------------------------------ 
//------------------------------ 
template <typename Key,typename T,typename _ValType,int COUNT>
bool		CMapPool<Key,T,_ValType,COUNT>::getKey(reverse_iterator& pos,Key& key)
{
	try
	{
		if(pos != m_UseList.rend())
		{
			key = pos->first;

			return true;
		}/*end if(pos)*/ 
	}
	catch (...)
	{
	}

	return false;
}

//------------------------------------------------------ 
//------------------------------ 
template <typename Key,typename T,typename _ValType,int COUNT>
T*	CMapPool<Key,T,_ValType,COUNT>::getData(const iterator& pos)
{
	T*	lp_t = NULL;
	try
	{
		if (pos != m_UseList.end())
		{
			lp_t = pos->second;
		}
	}
	catch (...)
	{
		return NULL;
	}

	return lp_t;
}
//------------------------------------------------------ 
//------------------------------ 
template <typename Key,typename T,typename _ValType,int COUNT>
T*	CMapPool<Key,T,_ValType,COUNT>::getData(const reverse_iterator& pos)
{
	T*	lp_t = NULL;
	try
	{
		if (pos != m_UseList.rend())
		{
			lp_t = pos->second;
		}
	}
	catch (...)
	{
		return NULL;
	}

	return lp_t;
}

//------------------------------------------------------ 
//------------------------------ 
template <typename Key,typename T,typename _ValType,int COUNT>
T*	CMapPool<Key,T,_ValType,COUNT>::find(const Key& key)
{
	T*	lp_t = NULL;
	iterator pos;
	try
	{
		pos = m_UseList.find(key);
		if (pos != m_UseList.end())
			lp_t = pos->second;
	}
	catch (...)
	{
	}

	return lp_t;
}

//------------------------------------------------------ 
//------------------------------ 
template <typename Key,typename T,typename _ValType,int COUNT>
const T*	CMapPool<Key,T,_ValType,COUNT>::find(const Key& key) const
{
	const T*	lp_t = NULL;
	const_iterator pos;
	try
	{
		pos = m_UseList.find(key);
		if (pos != m_UseList.end())
			lp_t = pos->second;
	}
	catch (...)
	{
	}

	return lp_t;
}

//------------------------------------------------------ 
//------------------------------ 
template <typename Key,typename T,typename _ValType,int COUNT>
bool	CMapPool<Key,T,_ValType,COUNT>::is_exist(const Key& key)
{
	bool bRes = false;
	try
	{
		bRes = m_UseList.is_exist(key);
		
	}
	catch (...)
	{
		return true;
	}

	return bRes;
}

//------------------------------------------------------ 
template <typename Key,typename T,typename _ValType,int COUNT>
bool	CMapPool<Key,T,_ValType,COUNT>::isTail(const iterator& pos)
{
	bool bRes = false;

	try
	{
		if (pos == m_UseList.end())
			bRes = true;
	}
	catch(...)
	{
		return true;
	}

	return bRes;
}

//------------------------------------------------------ 
//------------------------------ 
template <typename Key,typename T,typename _ValType,int COUNT>
bool	CMapPool<Key,T,_ValType,COUNT>::isTail(const reverse_iterator& pos)
{
	bool bRes = false;

	try
	{
		if (pos == m_UseList.rend())
			bRes = true;
	}
	catch(...)
	{
		return true;
	}

	return bRes;
}