/*----------------- keyDeques.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/8/20 20:49:38
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_deque.h"
#include "stl/stl_queue.h"
#include "stl/std_hash_map.h"
#include "threads/csLocker.h"
#include "poolVal.h"
/*************************************************************/
template<typename Key,typename T,typename _ValType = _Val_Init<T>, int COUNT = 0>
class CKeyDeques
{
private:
	struct _Block
	{
		Key	key;
		T	Data;
	};

private:
	typedef _Block							BLOCK;

private:
	typedef	stl_map<Key,BLOCK*>		KEY_POOL;
	typedef	stl_deque<BLOCK*>				DEQUE;
	typedef	stl_list<BLOCK*>				LIST;
	typedef	typename KEY_POOL::iterator		MAP_ITER;

public:
	typedef	typename DEQUE::iterator		iterator;

private:
	volatile int		m_nCount;
	volatile int		m_nMaxFree;
	DEQUE				m_UseList;
	KEY_POOL			m_UseMap;
	LIST				m_FreeList;
	CMutex_				m_csLock;

public:
	CKeyDeques();
	virtual ~CKeyDeques();

private:
	inline void		_build	();
	inline BLOCK*	_malloc	();
	inline void		_free	(BLOCK* p_t);

public:
	inline CMutex_&	getLock		()				{	return m_csLock;	}
	inline void		setMaxFree	(int nCount)	{	m_nMaxFree = nCount;}
	inline void		setPoolCount(int nCount)	{	m_nCount = nCount;	}
	inline bool		empty		()				{	return m_UseList.empty();		}
	inline iterator	begin		()				{	return m_UseList.begin();		}
	inline iterator	end			()				{	return m_UseList.end();			}

public:
	inline void	initPool		();

public:
	inline T*		push_back	(const Key&key);
	inline T*		push_front	(const Key&key);
	inline bool		release		(iterator&pos);
	inline bool		release		(const Key&key);
	inline void		clear		();
	inline void		getHead		(iterator&pos);
	inline void		pop_front	();
	inline void		pop_back	();
	inline T*		getData		(iterator&pos);
	inline T*		find		(const Key&key);
	inline size_t	size		(); 
	inline bool		isTail		(iterator&pos);
	inline void		moveToTail	(iterator&pos);	//不可以使用for或while否则会造成死循环
	inline void		moveToTail	(const Key&key);
	inline void		moveToFront	(const Key&key);
};

//------------------------------------------------------
//------------------------------ 
template<typename Key,typename T, typename _ValType, int COUNT>
CKeyDeques<Key,T,_ValType,COUNT>::CKeyDeques()
{
	m_nCount	= COUNT;
	m_nMaxFree	= COUNT;
}

//------------------------------------------------------
//------------------------------ 
template<typename Key,typename T, typename _ValType, int COUNT>
CKeyDeques<Key,T,_ValType,COUNT>::~CKeyDeques()
{
	initPool();
}

//------------------------------------------------------
//------------------------------ 
template<typename Key,typename T, typename _ValType, int COUNT>
inline void	CKeyDeques<Key,T,_ValType,COUNT>::_build()
{
	try
	{
		/*分配m_nCount个内存块*/ 
		for( int i = 0; i < m_nCount; i++ )
		{
			BLOCK*lp_t = new BLOCK;

			m_FreeList.push_back(lp_t);
		}/*end for(i)*/ 
	}
	catch (...)
	{
	}
}
//-------------------------------------------------------------
//------------------------------ 
template<typename Key,typename T,typename _ValType,int COUNT>
inline typename CKeyDeques<Key,T,_ValType,COUNT>::BLOCK*	CKeyDeques<Key,T,_ValType,COUNT>::_malloc	()
{
	BLOCK* p_t = NULL;
	try
	{
		if (m_FreeList.empty())
			_build();

		if (!m_FreeList.empty())
		{
			p_t = m_FreeList.front();
			m_FreeList.pop_front();
		}

		if (!p_t)
			p_t = new BLOCK;
	}
	catch(...)
	{
	}
	return p_t;
}
//------------------------------------------------------
//------------------------------ 
template<typename Key,typename T, typename _ValType, int COUNT>
inline void	CKeyDeques<Key,T,_ValType,COUNT>::_free(BLOCK* p_t)
{
	if (!p_t)
		return;

	try
	{
		if(m_nMaxFree > 0 && (int)m_FreeList.size() > m_nMaxFree)
		{
			safe_delete(p_t);
		}
		else
		{
			//_ValType::initialize(&p_t->Data);
			m_FreeList.push_front(p_t);
		}
	}
	catch (...)
	{
	}
}

//------------------------------------------------------
//------------------------------ 
template<typename Key,typename T, typename _ValType, int COUNT>
inline void	CKeyDeques<Key,T,_ValType,COUNT>::initPool()
{
	try
	{
		for (iterator iter = m_UseList.begin(); iter != m_UseList.end(); ++iter)
		{
			safe_delete(*iter);
		}
		m_UseList.clear();
		m_UseMap.clear();

		for (auto iter = m_FreeList.begin(); iter != m_FreeList.end(); ++iter)
		{
			safe_delete(*iter);
		}
		m_FreeList.clear();
	}
	catch(...)
	{
		m_UseList.clear();
		m_UseMap.clear();
	}
}

//------------------------------------------------------
//------------------------------ 
template<typename Key,typename T, typename _ValType, int COUNT>
inline T*		CKeyDeques<Key,T,_ValType,COUNT>::push_back(const Key&key)
{
	T*		lp_t	= NULL;
	try
	{
		BLOCK*	lpBlock = NULL;
		/*避免重复Key再次分配*/ 
		MAP_ITER	pos = m_UseMap.find(key);
		if (pos != m_UseMap.end())
			lpBlock	= pos->second;

		if (lpBlock)
			lp_t = &lpBlock->Data;

		if(!lp_t)
		{
			lpBlock = _malloc();
			if (lpBlock)
			{
				lpBlock->key = key;

				m_UseMap.insert_(key,lpBlock);
				m_UseList.push_back(lpBlock);

				if (lpBlock)
					lp_t = &lpBlock->Data;
			}
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
template<typename Key,typename T, typename _ValType, int COUNT>
inline T*		CKeyDeques<Key,T,_ValType,COUNT>::push_front(const Key&key)
{
	T*		lp_t	= NULL;
	try
	{
		BLOCK*	lpBlock = NULL;
		/*避免重复Key再次分配*/ 
		MAP_ITER	pos = m_UseMap.find(key);
		if (pos != m_UseMap.end())
			lpBlock	= pos->second;

		if (lpBlock)
			lp_t = &lpBlock->Data;

		if(!lp_t)
		{
			lpBlock = _malloc();
			if (lpBlock)
			{
				lpBlock->key = key;

				m_UseMap.insert_(key,lpBlock);
				m_UseList.push_front(lpBlock);

				if (lpBlock)
					lp_t = &lpBlock->Data;
			}
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
template<typename Key,typename T, typename _ValType, int COUNT>
inline bool	CKeyDeques<Key,T,_ValType,COUNT>::release(iterator&pos)
{
	bool	bRet	= true;
	try
	{
		if (pos != m_UseList.end())
		{
			BLOCK*lp_t = (*pos);

			pos = m_UseList.erase(pos);
			MAP_ITER _pos	= m_UseMap.find(lp_t->key);
			if (_pos != m_UseMap.end())
				m_UseMap.erase(_pos);

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
template<typename Key,typename T, typename _ValType, int COUNT>
inline bool	CKeyDeques<Key,T,_ValType,COUNT>::release(const Key&key)
{
	try
	{
		MAP_ITER pos = m_UseMap.find(key);
		if (pos != m_UseMap.end())
		{
			BLOCK*lpBlock = pos->second;

			iterator _pos = m_UseList.begin();
			iterator _end = m_UseList.end();
			for(; _pos != _end; ++_pos)
			{
				if((*_pos) == lpBlock)
				{
					m_UseList.erase(_pos);
					break;
				}
			}

			_free(lpBlock);
			m_UseMap.erase(pos);
			return true;
		}
	}
	catch(...)
	{
		return false;
	}

	return false;
}

//------------------------------------------------------
//------------------------------ 
template<typename Key,typename T, typename _ValType, int COUNT>
inline void	CKeyDeques<Key,T,_ValType,COUNT>::clear()
{
	try
	{
		iterator pos = m_UseList.begin();
		for ( ; pos != m_UseList.end(); ++pos )
			_free(*pos);

		m_UseList.clear();
		m_UseMap.clear();
	}
	catch (...)
	{
	}
}

//------------------------------------------------------
//------------------------------ 
template<typename Key,typename T, typename _ValType, int COUNT>
inline void	CKeyDeques<Key,T,_ValType,COUNT>::getHead(iterator& pos)
{
	try
	{
		pos = m_UseList.begin();
	}
	catch (...)
	{
	}
}
//-------------------------------------------------------------
//------------------------------ 
template<typename Key,typename T, typename _ValType, int COUNT>
inline void	CKeyDeques<Key,T,_ValType,COUNT>::pop_front	()
{
	try
	{
		if(!m_UseList.empty())
		{
			BLOCK*	lp_t = m_UseList.front();
			if(lp_t)
				m_UseMap.erase(lp_t->key);

			m_UseList.pop_front();
			_free(lp_t);
		}
	}
	catch (...)
	{
	}
}

//-------------------------------------------------------------
//------------------------------ 
template<typename Key,typename T, typename _ValType, int COUNT>
inline void	CKeyDeques<Key,T,_ValType,COUNT>::pop_back	()
{
	try
	{
		if(!m_UseList.empty())
		{
			BLOCK*	lp_t = m_UseList.back();
			if(lp_t)
				m_UseMap.erase(lp_t->key);

			m_UseList.pop_back();
			_free(lp_t);
		}
	}
	catch (...)
	{
	}
}
//------------------------------------------------------
//------------------------------ 
template<typename Key,typename T, typename _ValType, int COUNT>
inline T*	CKeyDeques<Key,T,_ValType,COUNT>::getData(iterator&pos)
{
	T*		lp_t	= NULL;
	try
	{
		if (pos != m_UseList.end())
		{
			BLOCK*lpBlock = (*pos);
			if (lpBlock)
				lp_t = &lpBlock->Data;
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
template<typename Key,typename T, typename _ValType, int COUNT>
inline T*	CKeyDeques<Key,T,_ValType,COUNT>::find(const Key&key)
{
	T*		lp_t	= NULL;
	try
	{
		MAP_ITER pos = m_UseMap.find(key);
		if (pos != m_UseMap.end())
		{
			BLOCK*lpBlock = pos->second;
			if (lpBlock)
				lp_t = &lpBlock->Data;
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
template<typename Key,typename T, typename _ValType, int COUNT>
inline size_t	CKeyDeques<Key,T,_ValType,COUNT>::size()
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
template<typename Key,typename T, typename _ValType, int COUNT>
inline bool	CKeyDeques<Key,T,_ValType,COUNT>::isTail(iterator&pos)
{
	bool bRes	=	false;

	try
	{
		if (pos == m_UseList.end())
			bRes = true;
	}
	catch(...)
	{
	}

	return bRes;
}

//------------------------------------------------------
//------------------------------ 
template<typename Key,typename T, typename _ValType, int COUNT>
inline void	CKeyDeques<Key,T,_ValType,COUNT>::moveToTail(iterator&pos)
{
	try
	{
		if (pos == m_UseList.end())
			return;

		BLOCK*lpBlock = (*pos);
		pos		= m_UseList.erase(pos);

		m_UseList.push_back(lpBlock);
	}
	catch(...)
	{
	}
}

//------------------------------------------------------
//------------------------------ 
template<typename Key,typename T, typename _ValType, int COUNT>
inline void	CKeyDeques<Key,T,_ValType,COUNT>::moveToTail(const Key&key)
{
	try
	{
		MAP_ITER pos = m_UseMap.find(key);
		if (pos != m_UseMap.end())
		{
			BLOCK*lpBlock = pos->second;

			iterator _pos = m_UseList.begin();
			iterator _end = m_UseList.end();
			for(; _pos != _end; ++_pos)
			{
				if((*_pos) == lpBlock)
				{
					m_UseList.erase(_pos);
					break;
				}
			}

			m_UseList.push_back(lpBlock);
		}
	}
	catch(...)
	{
	}
}

//------------------------------------------------------
//------------------------------ 
template<typename Key,typename T, typename _ValType, int COUNT>
inline void	CKeyDeques<Key,T,_ValType,COUNT>::moveToFront(const Key&key)
{
	try
	{
		MAP_ITER pos = m_UseMap.find(key);
		if (pos != m_UseMap.end())
		{
			BLOCK*lpBlock = pos->second;

			iterator _pos = m_UseList.begin();
			iterator _end = m_UseList.end();
			for(; _pos != _end; ++_pos)
			{
				if((*_pos) == lpBlock)
				{
					m_UseList.erase(_pos);
					break;
				}
			}

			m_UseList.push_front(lpBlock);
		}
	}
	catch(...)
	{
	}
}