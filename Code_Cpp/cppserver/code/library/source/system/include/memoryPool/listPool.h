/*----------------- listPool.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/8/20 20:47:51
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "stl/stl_list.h"
#include "poolVal.h"
#include "threads/csLocker.h"
/*************************************************************/
template<typename T, typename _ValType = _Val_Init<T>, int COUNT = 0>
class CListPool
{
private:
	typedef	stl_list<T*>			LIST;
	typedef	stl_map<T*,T*>			MAP;
	typedef	typename MAP::iterator	MAP_ITER;

public:
	typedef	typename LIST::iterator	iterator;

private:
	volatile int		m_nCount;
	volatile int		m_nMaxFree;
	LIST				m_UseList;
	MAP					m_UsePool;
	LIST				m_FreeList;
	CMutex_	m_csLock;

public:
	CListPool();
	virtual ~CListPool();

private:
	inline void	_build();
	inline void	_free(T* p_t);

public:
	inline CMutex_&	getLock		()			{	return m_csLock;			}
	inline void		setMaxFree	(int nCount){	m_nMaxFree = nCount;		}
	inline void		setPoolCount(int nCount){	m_nCount = nCount;			}

public:
	inline bool		empty		()const		{	return m_UseList.empty();	}
	inline size_t	size		()const 	{	return m_UseList.size();	} 
	inline iterator	begin		()			{	return m_UseList.begin();	}
	inline iterator	end			()			{	return m_UseList.end();		}

public:
	void		initPool		();
	T*			allocate		();
	bool		release		(T* const p_t);
	bool		release		(iterator& pos);
	void		clear		();
	void		getHead		(iterator& pos);
	T*			getHead		();
	T*			getNext		(iterator& pos);
	T*			getData		(const iterator& pos);
	size_t	size			(); 
	bool		isTail		(const iterator& pos);
	void		moveToTail	(T* const p_t);
	void		moveToTail	(iterator& pos);
};
//------------------------------------------------------
//------------------------------ 
template<typename T, typename _ValType, int COUNT>
CListPool<T,_ValType,COUNT>::CListPool()
{
	m_nCount	= COUNT;
	m_nMaxFree	= COUNT;
}

//------------------------------------------------------
//------------------------------ 
template<typename T, typename _ValType, int COUNT>
CListPool<T,_ValType,COUNT>::~CListPool()
{
	initPool();
}

//------------------------------------------------------
//------------------------------ 
template<typename T, typename _ValType, int COUNT>
void	CListPool<T,_ValType,COUNT>::_build()
{
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
}

//------------------------------------------------------
//------------------------------ 
template<typename T, typename _ValType, int COUNT>
void	CListPool<T,_ValType,COUNT>::_free(T* p_t)
{
	if (!p_t)
		return;

	if(m_nMaxFree < 0 || (int)m_FreeList.size() < m_nMaxFree)
	{
		//_ValType::initialize(p_t);
		m_FreeList.push_front(p_t);
	}
	else
	{
		safe_delete(p_t);
	}
}

//------------------------------------------------------
//------------------------------ 
template<typename T, typename _ValType, int COUNT>
void	CListPool<T,_ValType,COUNT>::initPool()
{
	try
	{
		for(iterator iter = m_UseList.begin(); iter != m_UseList.end(); ++iter)
		{
			safe_delete(*iter);
		}
		m_UseList.clear();
		m_UsePool.clear();

		for (LIST::iterator iter = m_FreeList.begin(); iter != m_FreeList.end(); ++iter)
		{
			safe_delete(*iter);
		}
		m_FreeList.clear();
	}
	catch(...)
	{
		m_UseList.clear();
		m_UsePool.clear();
		m_FreeList.clear();
	}
}

//------------------------------------------------------
//------------------------------ 
template<typename T, typename _ValType, int COUNT>
T*		CListPool<T,_ValType,COUNT>::allocate()
{
	T*		lp_t = NULL;
	try
	{
		if (m_FreeList.size() <= 0)
			_build();

		LIST::const_iterator pos = m_FreeList.begin();
		if (pos != m_FreeList.end())
		{
			lp_t = *pos;
			m_FreeList.erase(pos);
		}
		else
			lp_t = new T;

		if (lp_t)
		{
			m_UsePool.insert_(lp_t,lp_t);
			m_UseList.push_back(lp_t);
		}
	}
	catch (...)
	{
		return NULL;
	}
	_ValType::initialize(lp_t);
	return lp_t;
}

//------------------------------------------------------
//------------------------------ 
template<typename T, typename _ValType, int COUNT>
bool	CListPool<T,_ValType,COUNT>::release(T* const p_t)
{
	try
	{
		m_UsePool.erase(p_t);
		m_UseList.remove(p_t);

		_free(p_t);
	}
	catch(...)
	{
		return false;
	}

	return true;
}

//------------------------------------------------------
//------------------------------ 
template<typename T, typename _ValType, int COUNT>
bool	CListPool<T,_ValType,COUNT>::release(iterator& pos)
{
	try
	{
		if (pos == m_UseList.end())
			return false;
		
		T*lp_t = (*pos);
		++pos;
		m_UseList.remove(lp_t);

		MAP_ITER _pos	= m_UsePool.find(lp_t);
		if (_pos != m_UsePool.end())
			m_UsePool.erase(_pos);

		_free(lp_t);
	}
	catch (...)
	{
		return false;
	}
	return true;
}

//------------------------------------------------------ 
//------------------------------ 
template<typename T, typename _ValType, int COUNT>
void	CListPool<T,_ValType,COUNT>::clear()
{
	try
	{
		iterator pos= m_UseList.begin();
		for ( ; pos != m_UseList.end(); ++pos )
		{
			_free((*pos));
		}
	}
	catch (...)
	{
	}
	m_UseList.clear();
	m_UsePool.clear();
}
//------------------------------------------------------
//------------------------------ 
template<typename T, typename _ValType, int COUNT>
void	CListPool<T,_ValType,COUNT>::getHead(iterator& pos)
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
template<typename T, typename _ValType, int COUNT>
T*	CListPool<T,_ValType,COUNT>::getHead()
{
	T*	lp_t	= NULL;
	try
	{
		iterator pos = m_UseList.begin();
		if (pos != m_UseList.end())
			lp_t = (*pos);
	}
	catch (...)
	{
	}

	return lp_t;
}

//------------------------------------------------------
//------------------------------ 
template<typename T, typename _ValType, int COUNT>
T*		CListPool<T,_ValType,COUNT>::getNext(iterator& pos)
{
	T*	lp_t	= NULL;
	try
	{
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
template<typename T, typename _ValType, int COUNT>
T*	CListPool<T,_ValType,COUNT>::getData(const iterator& pos)
{
	T*		lp_t	= NULL;
	try
	{
		if (pos != m_UseList.end())
			lp_t = (*pos);
	}
	catch (...)
	{
		return NULL;
	}

	return lp_t;
}

//------------------------------------------------------
//------------------------------ 
template<typename T, typename _ValType, int COUNT>
size_t	CListPool<T,_ValType,COUNT>::size()
{
	size_t	sSize = 0;
	try
	{
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
template<typename T, typename _ValType, int COUNT>
bool	CListPool<T,_ValType,COUNT>::isTail(const iterator& pos)
{
	try
	{
		if (pos == m_UseList.end())
			return true;
	}
	catch(...)
	{
		return true;
	}

	return false;
}

//------------------------------------------------------
//------------------------------ 
template<typename T, typename _ValType, int COUNT>
void	CListPool<T,_ValType,COUNT>::moveToTail(T* const p_t)
{
	try
	{
		MAP_ITER pos = m_UsePool.find(p_t);
		if (pos == m_UsePool.end())
			return;

		m_UseList.remove(p_t);
		m_UseList.push_back(p_t);
	}
	catch(...)
	{
		return;
	}
}

//------------------------------------------------------
//------------------------------ 
template<typename T, typename _ValType, int COUNT>
void	CListPool<T,_ValType,COUNT>::moveToTail(iterator& pos)
{
	try
	{
		if (pos == m_UseList.end())
			return false;

		T*lp_t	= (*pos);
		m_UseList.remove(lp_t);
		m_UseList.push_back(lp_t);
	}
	catch(...)
	{
		return;
	}
}
