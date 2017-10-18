/*----------------- hashMapPool.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/8/20 20:50:56
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "stl/stl_list.h"
#include "memoryPool/mem_malloc.h"
#include "threads/csLocker.h"
#include "poolVal.h"
/*************************************************************/
template <typename Key, typename T, typename _ValType = _Val_Init<T>, int COUNT = 0>
class CHashMapPool
{
private:
	typedef	stl_map<Key,T*>			HASH_MAP;
#ifdef _USE_NEDMALLOC
#else//_USE_NEDMALLOC
	typedef stl_list<T*>					LIST;
#endif//_USE_NEDMALLOC

public:
	typedef	typename HASH_MAP::iterator		iterator;
#ifdef _USE_NEDMALLOC
#else//_USE_NEDMALLOC
	typedef typename LIST::iterator	list_iterator;
#endif//_USE_NEDMALLOC


private:
	CMutex_				m_csLock;
	HASH_MAP			m_UseList;

#ifdef _USE_NEDMALLOC
public:
	mem_malloc_declaration();
#else//_USE_NEDMALLOC
private:
	volatile int		m_nCount;
	volatile int		m_nMaxFree;
	LIST				m_FreeList;
#endif//_USE_NEDMALLOC

public:
	CHashMapPool();
	virtual ~CHashMapPool();

public:
	inline CMutex_&	getLock		()			{	return m_csLock;			}
#ifdef _USE_NEDMALLOC
#else//_USE_NEDMALLOC
	inline void		setMaxFree	(int nCount){	m_nMaxFree = nCount;		}
	inline void		setPoolCount(int nCount){	m_nCount = nCount;			}
#endif//_USE_NEDMALLOC
	inline iterator	begin		()			{	return m_UseList.begin();	}
	inline iterator	end			()			{	return m_UseList.end();		}
	inline bool		empty		()const		{	return m_UseList.empty();	}
	inline size_t	size		()const 	{	return m_UseList.size();	} 

private:
	inline void	_build	();
	inline T*	_malloc	();
	inline void	_free	(T* p_t);

public:
	void	initPool();

public:
	T*		allocate(const Key& key);
	bool	release	(const Key& key);
	bool	release	(iterator& pos);
	void	clear	();
	void	getHead	(iterator& pos);
	void	getEnd	(iterator& pos);
	T*		getHead	();
	T*		getNext	(iterator& pos);
	T*		getData	(const iterator& pos);
	T*		find	(const Key& key);
	bool	isTail	(const iterator& pos);
};
#ifdef _USE_NEDMALLOC
template <typename Key, typename T, typename _ValType, int COUNT>
mem_malloc	CHashMapPool<Key,T,_ValType,COUNT>::__malloc;
#endif//_USE_NEDMALLOC
//------------------------------------------------------
//------------------------------ 
template <typename Key, typename T, typename _ValType, int COUNT>
CHashMapPool<Key,T,_ValType,COUNT>::CHashMapPool()
{
#ifdef _USE_NEDMALLOC
#else//_USE_NEDMALLOC
	m_nCount	= COUNT;
	m_nMaxFree	= COUNT;
#endif//_USE_NEDMALLOC
}
//------------------------------------------------------ 
//------------------------------ 
template <typename Key, typename T, typename _ValType, int COUNT>
CHashMapPool<Key,T,_ValType,COUNT>::~CHashMapPool()
{
	initPool();
}
//------------------------------------------------------ 
//------------------------------ 
template <typename Key, typename T, typename _ValType, int COUNT>
inline void	CHashMapPool<Key,T,_ValType,COUNT>::_build()
{
#ifdef _USE_NEDMALLOC
#else//_USE_NEDMALLOC
	try
	{
		/*分配m_nCount个内存块*/ 
		for( int i = 0; i < m_nCount; i++ )
		{
			T*lp_t = new T;

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
template <typename Key, typename T, typename _ValType, int COUNT>
inline T*	CHashMapPool<Key,T,_ValType,COUNT>::_malloc	()
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
template <typename Key, typename T, typename _ValType, int COUNT>
void	CHashMapPool<Key,T,_ValType,COUNT>::_free(T* p_t)
{
	if (!p_t)
		return;

	try
	{
#ifdef _USE_NEDMALLOC
		safe_delete(p_t);
#else//_USE_NEDMALLOC
		if(m_nMaxFree > 0 && (int)m_FreeList.size() > m_nMaxFree)
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
template <typename Key, typename T, typename _ValType, int COUNT>
void	CHashMapPool<Key,T,_ValType,COUNT>::initPool()
{
	try
	{
		iterator	pos;
		for (pos = m_UseList.begin(); pos != m_UseList.end(); ++pos)
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
template <typename Key, typename T, typename _ValType, int COUNT>
T*		CHashMapPool<Key,T,_ValType,COUNT>::allocate(const Key& key)
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
		if (!lp_t)
		{
			lp_t = _malloc();
			if (lp_t)
				m_UseList.insert_(key,lp_t);
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
template <typename Key, typename T, typename _ValType, int COUNT>
bool	CHashMapPool<Key,T,_ValType,COUNT>::release(const Key& key)
{
	T*		lp_t	= NULL;
	bool	bRet	= true;
	iterator pos;
	try
	{
		pos = m_UseList.find(key);
		if (pos != m_UseList.end())
		{
			lp_t = pos->second;

			m_UseList.erase(pos);

			_free(lp_t);
		}
		else
			bRet = false;
	}
	catch(...)
	{
		return false;
	}

	return bRet;
}
//------------------------------------------------------ 
//------------------------------ 
template <typename Key, typename T, typename _ValType, int COUNT>
bool	CHashMapPool<Key,T,_ValType,COUNT>::release(iterator& pos)
{
	T*		lp_t = NULL;
	bool	bRet	= true;
	try
	{
		if (pos != m_UseList.end())
		{
			lp_t = pos->second;

			pos = m_UseList.erase_( pos );
			_free(lp_t);
		}
		else
			bRet = false;
	}
	catch (...)
	{
		return false;
	}
	return bRet;
}
//------------------------------------------------------ 
//------------------------------ 
template <typename Key, typename T, typename _ValType, int COUNT>
void	CHashMapPool<Key,T,_ValType,COUNT>::clear()
{
	T*			lp_t	= NULL;
	iterator	pos;
	try
	{
		pos = m_UseList.begin();
		for ( ; pos != m_UseList.end(); ++pos )
		{
			lp_t = pos->second;

			_free(lp_t);
		}
		m_UseList.clear();
	}
	catch (...)
	{
	}
}
//------------------------------------------------------ 
//------------------------------ 
template <typename Key, typename T, typename _ValType, int COUNT>
void	CHashMapPool<Key,T,_ValType,COUNT>::getHead(iterator& pos)
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
template <typename Key, typename T, typename _ValType, int COUNT>
void	CHashMapPool<Key,T,_ValType,COUNT>::getEnd(iterator& pos)
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
//------------------------------ 
template <typename Key, typename T, typename _ValType, int COUNT>
T*	CHashMapPool<Key,T,_ValType,COUNT>::getHead()
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
//------------------------------ 
template <typename Key, typename T, typename _ValType, int COUNT>
T*	CHashMapPool<Key,T,_ValType,COUNT>::getNext(iterator& pos)
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
	}

	return lp_t;
}
//------------------------------------------------------ 
//------------------------------ 
template <typename Key, typename T, typename _ValType, int COUNT>
T*	CHashMapPool<Key,T,_ValType,COUNT>::getData(const iterator& pos)
{
	T*	lp_t = NULL;
	try
	{
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
template <typename Key, typename T, typename _ValType, int COUNT>
T*	CHashMapPool<Key,T,_ValType,COUNT>::find(const Key& key)
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
template <typename Key, typename T, typename _ValType, int COUNT>
bool	CHashMapPool<Key,T,_ValType,COUNT>::isTail(const iterator& pos)
{
	bool bRes	=	true;

	try
	{
		if (pos != m_UseList.end())
			bRes = false;
	}
	catch(...)
	{
	}

	return bRes;
}
