/*----------------- memoryPool.h
*
* Copyright (C): 2011  Mokylin°§Mokyqi
* Author       : ’‘Œƒ‘¥
* Version      : V1.0
* Date         : 2011/8/20 20:44:47
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_set.h"
#include "stl/stl_list.h"
#include "memoryPool/mem_malloc.h"
#include "threads/csLocker.h"
#include "poolVal.h"
/*************************************************************/
template <typename T, typename _ValType = _Val_Init<T>, int COUNT = 0>
class CMemoryPool
{
private:
	typedef stl_set<T*> SET_LIST;
#ifdef _USE_NEDMALLOC
#else//_USE_NEDMALLOC
	typedef stl_list<T*>	LIST;
#endif//_USE_NEDMALLOC

public:
	typedef typename SET_LIST::iterator	iterator;

#ifdef _USE_NEDMALLOC
#else//_USE_NEDMALLOC
	typedef typename LIST::iterator	list_iterator;
#endif//_USE_NEDMALLOC

private:
	CMutex_				m_csLock;
	SET_LIST			m_UseList;

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
	CMemoryPool();
	virtual ~CMemoryPool();

public:
	inline CMutex_&getLock()	{	return m_csLock;	}
#ifdef _USE_NEDMALLOC
#else//_USE_NEDMALLOC
	inline void	setMaxFree	(int nCount){	m_nMaxFree = nCount;}
	inline void	setPoolCount(int nCount){	m_nCount = nCount;	}
#endif//_USE_NEDMALLOC

public:
	inline void		initPool();
	inline T*		_malloc	();
	inline void		_free	(T* p_t);

public:
	inline T*		allocate();
	inline void		release(T* p_t);
	inline void		release(iterator pos);
	inline void		clear();
	inline void		getHead(iterator& pos);
	inline void		getEnd(iterator& pos);
	inline T*		getNext(iterator& pos);
	inline T*		getData(iterator pos);
	inline size_t	size(); 
	inline bool		isTail(iterator pos);
	inline T*		being(T* p_t);
};

#ifdef _USE_NEDMALLOC
template <typename T, typename _ValType, int COUNT>
mem_malloc	CMemoryPool<T,_ValType,COUNT>::__malloc;
#endif//_USE_NEDMALLOC
//------------------------------------------------------
//------------------------------ 
template <typename T, typename _ValType, int COUNT>
CMemoryPool<T,_ValType,COUNT>::CMemoryPool()
{
#ifdef _USE_NEDMALLOC
#else//_USE_NEDMALLOC
	m_nCount	= COUNT;
	m_nMaxFree	= COUNT;
#endif//_USE_NEDMALLOC
}
//------------------------------------------------------ 
//------------------------------ 
template <typename T, typename _ValType, int COUNT>
CMemoryPool<T,_ValType,COUNT>::~CMemoryPool()
{
	initPool();
}
//------------------------------------------------------ 
//------------------------------ 
template <typename T, typename _ValType, int COUNT>
inline void	CMemoryPool<T,_ValType,COUNT>::initPool()
{
	try
	{
		//CCritLocker lock(m_csLock);
		for (iterator pos = m_UseList.begin(); pos != m_UseList.end(); ++pos)
		{
			if (*pos)
				delete (*pos);
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
//-------------------------------------------------------------
//------------------------------ 
template <typename T, typename _ValType, int COUNT>
inline T*	CMemoryPool<T,_ValType,COUNT>::_malloc	()
{
	T* p_t = NULL;
	try
	{
#ifdef _USE_NEDMALLOC
		p_t = new T;
#else//_USE_NEDMALLOC
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
template <typename T, typename _ValType, int COUNT>
inline void	CMemoryPool<T,_ValType,COUNT>::_free(T* p_t)
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
template <typename T, typename _ValType, int COUNT>
inline T*		CMemoryPool<T,_ValType,COUNT>::allocate()
{
	T*	lp_t = NULL;
	try
	{
		lp_t = _malloc();
		if (lp_t)
			m_UseList.insert(lp_t);
	}
	catch (...)
	{
	}
	_ValType::initialize(lp_t);
	return lp_t;
}
//------------------------------------------------------ 
//------------------------------ 
template <typename T, typename _ValType, int COUNT>
inline void	CMemoryPool<T,_ValType,COUNT>::release(T* p_t)
{
	if (!p_t)
		return;

	try
	{
		//CCritLocker lock(m_csLock);
		iterator pos = m_UseList.find(p_t);
		if (pos == m_UseList.end())
			return;

		m_UseList.erase(pos);

		_free (p_t);
	}
	catch(...)
	{
	}
}

//------------------------------------------------------ 
//------------------------------ 
template <typename T, typename _ValType, int COUNT>
inline void	CMemoryPool<T,_ValType,COUNT>::release(iterator pos)
{
	T*	lp_t = NULL;
	try
	{
        //CCritLocker lock(m_csLock);
		if (pos != m_UseList.end())
		{
			lp_t = (*pos);
			m_UseList.erase( pos );

			_free (lp_t);
		}
	}
	catch (...)
	{
	}
}
//------------------------------------------------------ 
//------------------------------ 
template <typename T, typename _ValType, int COUNT>
inline void	CMemoryPool<T,_ValType,COUNT>::clear()
{
	T*	lp_t = NULL;
	iterator	pos;
	try
	{
        //CCritLocker lock(m_csLock);
		pos = m_UseList.begin();
		for ( ; pos != m_UseList.end(); ++pos )
		{
			lp_t = (*pos);

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
template <typename T, typename _ValType, int COUNT>
inline void	CMemoryPool<T,_ValType,COUNT>::getHead(iterator& pos)
{
	try
	{
        //CCritLocker lock(m_csLock);
		pos = m_UseList.begin();
	}
	catch (...)
	{
	}
}

//------------------------------------------------------ 
//------------------------------ 
template <typename T, typename _ValType, int COUNT>
inline void	CMemoryPool<T,_ValType,COUNT>::getEnd(iterator& pos)
{
	try
	{
		//CCritLocker lock(m_csLock);
		pos = m_UseList.end();
	}
	catch (...)
	{
	}
}

//------------------------------------------------------ 
//------------------------------ 
template <typename T, typename _ValType, int COUNT>
inline T*		CMemoryPool<T,_ValType,COUNT>::getNext(iterator& pos)
{
	T*	lp_t = NULL;
	try
	{
        //CCritLocker lock(m_csLock);
		if(pos != m_UseList.end())
		{
			lp_t = (*pos);
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
template <typename T, typename _ValType, int COUNT>
inline T*		CMemoryPool<T,_ValType,COUNT>::getData(iterator pos)
{
	T*	lp_t = NULL;
	try
	{
        //CCritLocker lock(m_csLock);
		if (pos != m_UseList.end())
		{
			lp_t = (*pos);
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
template <typename T, typename _ValType, int COUNT>
inline size_t	CMemoryPool<T,_ValType,COUNT>::size()
{
	size_t	sSize = 0;
	try
	{
        //CCritLocker lock(m_csLock);
		sSize = m_UseList.size();
	}
	catch(...)
	{
		return 0;
	}

	return sSize;

}

//------------------------------------------------------ 
//------------------------------ 
template <typename T, typename _ValType, int COUNT>
inline bool	CMemoryPool<T,_ValType,COUNT>::isTail(iterator pos)
{
	bool bRes	=	false;

	try
	{
        //CCritLocker lock(m_csLock);
		if (pos == m_UseList.end())
			bRes	=	true;
	}
	catch(...)
	{
		return true;
	}

	return bRes;
}

//------------------------------------------------------ 
//------------------------------ 
template <typename T, typename _ValType, int COUNT>
inline T*	CMemoryPool<T,_ValType,COUNT>::being(T* p_t)
{
	T*	lp_t = NULL;
	try
	{
        //CCritLocker lock(m_csLock);
		iterator pos = m_UseList.find(p_t);
		if (pos != m_UseList.end())
		{
			lp_t = (*pos);
		}
	}
	catch (...)
	{
		return NULL;
	}

	return lp_t;
}