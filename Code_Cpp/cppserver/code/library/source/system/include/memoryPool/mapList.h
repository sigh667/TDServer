/*----------------- mapList.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/8/20 20:46:45
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/stl_list.h"
#include "stl/std_map.h"
#include "threads/csLocker.h"
/*************************************************************/
template <typename Key,typename T,int COUNT = 0>
class CMapList
{
private:
	typedef	stl_list<T*>					LIST;
	typedef	stl_map<Key,T*>					MAP;
	typedef	stl_map<T*,Key>					USE_POOL;
	typedef	stl_list<T*,T*>					LIST;
	typedef	typename MAP::iterator			USE_ITER;
	typedef	typename USE_POOL::iterator		USE_POOL_ITER;

public:
	typedef	typename LIST::iterator		iterator;
	CMutex_	m_csLock;

private:
	volatile int	m_nCount;
	volatile int	m_nMaxFree;
	MAP				m_UseMap;
	LIST			m_UseList;
	USE_POOL		m_UsePool;
	LIST			m_FreeList;

public:
	CMapList();
	virtual ~CMapList();

private:
	inline void	_build();
	inline void	_free	(T* p_t);

public:
	inline CMutex_&getLock()	{	return m_csLock;		}
	inline void	setMaxFree	(int nCount){	m_nMaxFree = nCount;	}
	inline void	setPoolCount(int nCount){	m_nCount = nCount;	}
	
public:
	void		initPool		();
	T*			allocate		(Key key);
	bool		release		(Key key);
	bool		release		(T* p_t);
	bool		release		(iterator pos);
	void		clear			();
	void		getHead		(iterator& pos);
	T*			getHead		();
	T*			getNext		(iterator& pos);
	T*			getData		(iterator pos);
	T*			find		(Key key);
	size_t		size			();
	void		moveToTail	(T* p_t);
	bool		isTail		(iterator pos);
};
//------------------------------------------------------
//------------------------------ 
template<typename Key,class T,int COUNT>
CMapList<Key,T,COUNT>::CMapList()
{
	m_nCount	= COUNT;
	m_nMaxFree	= COUNT;
}
//------------------------------------------------------ 
//------------------------------ 
template<typename Key,class T,int COUNT>
CMapList<Key,T,COUNT>::~CMapList()
{
	initPool();
}

//------------------------------------------------------ 
//------------------------------ 
template<typename Key,class T,int COUNT>
void	CMapList<Key,T,COUNT>::_build()
{
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
}
//------------------------------------------------------ 
//------------------------------ 
template<typename Key,class T,int COUNT>
void	CMapList<Key,T,COUNT>::_free(T* p_t)
{
	if (!p_t)
		return;

	if(m_nMaxFree < 0 || (int)m_FreeList.size() < m_nMaxFree)
		m_FreeList.push_front(p_t);
	else
	{
		safe_delete(p_t);
	}
}
//------------------------------------------------------ 
//------------------------------ 
template<typename Key,class T,int COUNT>
void	CMapList<Key,T,COUNT>::initPool()
{
	try
	{
		for (USE_ITER iter = m_UseMap.begin(); iter != m_UseMap.end(); ++iter)
		{
			safe_delete(iter->second);
		}
		m_UseMap.clear();
		m_UseList.clear();
		m_UsePool.clear();

		for (iterator iter = m_FreeList.begin(); iter != m_FreeList.end(); ++iter)
		{
			safe_delete(*iter);
		}
		m_FreeList.clear();
	}
	catch(...)
	{
		m_UseMap.clear();
		m_UseList.clear();
		m_FreeList.clear();
	}
}

//------------------------------------------------------ 
//------------------------------ 
template<typename Key,class T,int COUNT>
T*		CMapList<Key,T,COUNT>::allocate(Key key)
{
	T*	lp_t = NULL;
	try
	{
		/*避免重复Key再次分配*/ 
		USE_ITER	pos = m_UseMap.find(key);
		if (pos != m_UseMap.end())
		{
			lp_t = pos->second;
		}
		else
		{
			if (m_FreeList.size() <= 0)
				_build();

			LIST::iterator pos = m_FreeList.begin();
			if (pos != m_FreeList.end())
			{
				lp_t = pos->second;
				m_FreeList.erase(pos);
			}
			
			if (NULL == lp_t)
				lp_t = new T;

			m_UseList.push_back(lp_t);
			m_UseMap.insert_(key,lp_t);
			m_UsePool.insert_(lp_t,key);
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
template<typename Key,class T,int COUNT>
bool	CMapList<Key,T,COUNT>::release(Key key)
{
	T*		lp_t	= NULL;
	bool	bRet	= true;
	USE_ITER pos;
	try
	{
		pos = m_UseMap.find(key);
		if (pos != m_UseMap.end())
		{
			lp_t = pos->second;

			m_UseMap.erase(pos);
			m_UsePool.erase(lp_t);
			m_UseList.remove(lp_t);

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
template<typename Key,class T,int COUNT>
bool	CMapList<Key,T,COUNT>::release(T* p_t)
{
	bool	bRet	= true;
	USE_POOL_ITER pos;
	try
	{
		pos = m_UsePool.find(p_t);
		if (pos != m_UsePool.end())
		{
			m_UseMap.erase(pos->second);
			m_UsePool.erase(pos);
			m_UseList.remove(p_t);

			_free(p_t);
			p_t	= NULL;
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
template<typename Key,class T,int COUNT>
bool	CMapList<Key,T,COUNT>::release(iterator pos)
{
	T*		lp_t = NULL;
	bool	bRet	= true;
	try
	{
		if (pos != m_UseList.end())
		{
			lp_t = (*pos);

			m_UseList.erase( pos );

			USE_POOL_ITER Usepos = m_UsePool.find(lp_t);
			if (Usepos != m_UsePool.end())
			{
				m_UseMap.erase(Usepos->second);
				m_UsePool.erase(Usepos);
			}

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
template<typename Key,class T,int COUNT>
void	CMapList<Key,T,COUNT>::clear()
{
	iterator	pos;
	try
	{
		pos = m_UseList.begin();
		for ( ; pos != m_UseList.end(); ++pos )
			_free(*pos);

		m_UseMap.clear();
		m_UsePool.clear();
	}
	catch (...)
	{
	}
}

//------------------------------------------------------ 
//------------------------------ 
template<typename Key,class T,int COUNT>
void	CMapList<Key,T,COUNT>::getHead(iterator& pos)
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
template<typename Key,class T,int COUNT>
T*		CMapList<Key,T,COUNT>::getHead()
{
	T*	lp_t	= NULL;
	try
	{
		iterator	pos = m_UseList.begin();
		if (pos != m_UseList.end())
			lp_t	= (*pos);
	}
	catch (...)
	{
	}
	return lp_t;
}

//------------------------------------------------------ 
//------------------------------ 
template<typename Key,class T,int COUNT>
T*		CMapList<Key,T,COUNT>::getNext(iterator& pos)
{
	T*	lp_t = NULL;
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
template<typename Key,class T,int COUNT>
T*	CMapList<Key,T,COUNT>::getData(iterator pos)
{
	T*	lp_t = NULL;
	try
	{
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
template<typename Key,class T,int COUNT>
T*	CMapList<Key,T,COUNT>::find(Key key)
{
	T*	lp_t = NULL;
	USE_ITER pos;
	try
	{
		pos = m_UseMap.find(key);
		if (pos != m_UseMap.end())
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
template<typename Key,class T,int COUNT>
size_t	CMapList<Key,T,COUNT>::size()
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
template<typename Key,class T,int COUNT>
void	CMapList<Key,T,COUNT>::moveToTail(T* p_t)
{
	USE_POOL_ITER pos;
	try
	{
		pos = m_UsePool.find(p_t);
		if (pos != m_UsePool.end())
		{
			m_UseList.remove(p_t);
			m_UseList.push_back(p_t);
		}
	}
	catch(...)
	{
		return;
	}
}

//------------------------------------------------------ 
//------------------------------ 
template<typename Key,class T,int COUNT>
bool	CMapList<Key,T,COUNT>::isTail(iterator pos)
{
	bool bRes	=	false;
	try
	{
		if (pos == m_UseList.end())
			bRes	=	true;
	}
	catch(...)
	{
		return true;
	}

	return bRes;
}
