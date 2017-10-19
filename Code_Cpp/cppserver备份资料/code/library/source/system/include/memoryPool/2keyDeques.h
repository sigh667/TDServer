/*----------------- 2keyDeques.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2011/8/20 20:52:45
*--------------------------------------------------------------
*双Key堆管操作,在申请时必须提供两个都未使用过的key,否则不能申请成功
*------------------------------------------------------------*/
#pragma once
#include "stl/std_map.h"
#include "stl/stl_list.h"
#include "threads/csLocker.h"
#include "poolVal.h"
/*************************************************************/
template<class Key1,class Key2,class T,typename _ValType = _Val_Init<T>,int COUNT = 1>
class C2KeyDeques
{
private:
	struct _Block
	{
		Key1	key1;
		Key2	key2;
		T		Data;
	};

private:
	typedef _Block							BLOCK;

private:
	typedef	stl_list<BLOCK*>				LIST;
	typedef	stl_map<Key1,BLOCK*>			MAP1;
	typedef	stl_map<Key2,BLOCK*>			MAP2;

public:
	typedef	typename LIST::iterator			iterator;
	typedef	typename MAP1::iterator			map1_iter;
	typedef	typename MAP2::iterator			map2_iter;

private:
	volatile	int		m_nCount;
	volatile	int		m_nMaxFree;
	LIST				m_UseList;
	MAP1				m_UseMap1;
	MAP2				m_UseMap2;
	LIST				m_FreeList;
	CMutex_	m_Lock;

public:
	explicit C2KeyDeques();
	virtual ~C2KeyDeques();

private:
	inline void	_build();
	inline void	_free	(BLOCK* p_t);

public:
	inline CMutex_&getLock()	{	return m_Lock;			}
	inline void	setMaxFree	(int nCount){	m_nMaxFree = nCount;	}
	inline void	setPoolCount(int nCount){	m_nCount = nCount;	}

public:
	void		initPool		();

public:
	T*			allocate	(const Key1& key1,const Key2& key2);
	bool		changeKey	(const Key2& oldKey,const Key2& newKey);
	bool		release		(const Key1& key1);
	bool		release		(const Key2& key2);
	bool		release		(const iterator& pos);
	void		clear			();
	void		getHead		(iterator& pos);
	T*			getNext		(iterator& pos);
	T*			getData		(const iterator& pos);
	T*			find		(const Key1& key1);
	T*			find		(const Key2& key2);
	bool		getPos		(const Key1& key1,iterator& pos);
	size_t	size			(); 
	bool		isTail		(const iterator& pos);
	void		moveToTail	(const Key1& key1);
	void		moveToTail	(const Key2& key2);
	/*----->{ 下界(包含自己向后) }*/ 
	void		lowerBound	(map1_iter& pos,Key1&key1);
	/*----->{ 上界(不包含自己向后) }*/ 
	void		upperBound	(map1_iter& pos,Key1&key1);
	T*			getNext		(map1_iter& pos);
	T*			getData		(const map1_iter& pos);
	bool		isTail		(const map1_iter& pos);
	/*----->{ 下界(包含自己向后) }*/ 
	void		lowerBound	(map2_iter& pos,Key2&key2);
	/*----->{ 上界(不包含自己向后) }*/ 
	void		upperBound	(map2_iter& pos,Key2&key2);
	T*			getNext		(map2_iter& pos);
	T*			getData		(const map2_iter& pos);
	bool		isTail		(const map2_iter& pos);
};

//------------------------------------------------------
//------------------------------ 
template<class Key1,class Key2,class T,typename _ValType,int COUNT>
C2KeyDeques<Key1,Key2,T,_ValType,COUNT>::C2KeyDeques()
{
	m_nCount	= COUNT;
	m_nMaxFree	= COUNT;
}

//------------------------------------------------------
//------------------------------ 
template<class Key1,class Key2,class T,typename _ValType,int COUNT>
C2KeyDeques<Key1,Key2,T,_ValType,COUNT>::~C2KeyDeques()
{
	initPool();
}

//------------------------------------------------------
//------------------------------ 
template<class Key1,class Key2,class T,typename _ValType,int COUNT>
void	C2KeyDeques<Key1,Key2,T,_ValType,COUNT>::_build()
{
	try
	{
		/*分配m_nCount个内存块*/ 
		for ( int i = 0; i < m_nCount; ++i )
		{
			m_FreeList.push_back(new BLOCK);
		}/*end for(i)*/ 
	}
	catch (...)
	{
	}
}

//------------------------------------------------------
//------------------------------ 
template<class Key1,class Key2,class T,typename _ValType,int COUNT>
void	C2KeyDeques<Key1,Key2,T,_ValType,COUNT>::_free(BLOCK* p_t)
{
	if (!p_t)
		return;

	if (m_nMaxFree < 0 || (int)m_FreeList.size() < m_nMaxFree)
	{
		//_ValType::initialize(&p_t->Data);
		m_FreeList.push_front(p_t);
	}
	else
	{
		safe_delete(p_t);
	}
}

//------------------------------------------------------
//------------------------------ 
template<class Key1,class Key2,class T,typename _ValType,int COUNT>
void	C2KeyDeques<Key1,Key2,T,_ValType,COUNT>::initPool()
{
	try
	{
		for (iterator iter = m_UseList.begin(); iter != m_UseList.end(); ++iter)
		{
			safe_delete(*iter);
		}
		m_UseList.clear();
		m_UseMap1.clear();
		m_UseMap2.clear();

		for (iterator iter = m_FreeList.begin(); iter != m_FreeList.end(); ++iter)
		{
			safe_delete(*iter);
		}
		m_FreeList.clear();
	}
	catch(...)
	{
		m_UseList.clear();
		m_UseMap1.clear();
		m_UseMap2.clear();
		m_FreeList.clear();
	}
}

//------------------------------------------------------
//------------------------------ 
template<class Key1,class Key2,class T,typename _ValType,int COUNT>
T*	C2KeyDeques<Key1,Key2,T,_ValType,COUNT>::allocate(const Key1& key1,const Key2& key2)
{
	BLOCK*	lpBlock1 = NULL;
	BLOCK*	lpBlock2 = NULL;
	try
	{
		/*避免重复Key再次分配*/ 
		map1_iter	pos1 = m_UseMap1.find(key1);
		if (pos1 != m_UseMap1.end())
			lpBlock1 = pos1->second;

		map2_iter	pos2 = m_UseMap2.find(key2);
		if (pos2 != m_UseMap2.end())
			lpBlock2 = pos2->second;

		if (lpBlock1 || lpBlock2)
			return NULL;

		if(lpBlock1)
			return &lpBlock1->Data;

		if (m_FreeList.size() <= 0)
			_build();

		if (!m_FreeList.empty())
		{
			lpBlock1 = m_FreeList.front();
			m_FreeList.pop_front();
		}

		if (NULL == lpBlock1)
			lpBlock1 = new BLOCK;

		lpBlock1->key1 = key1;
		lpBlock1->key2 = key2;

		m_UseMap1.insert_(lpBlock1->key1,lpBlock1);
		m_UseMap2.insert_(lpBlock1->key2,lpBlock1);
		m_UseList.push_back(lpBlock1);

		_ValType::initialize(lpBlock1->Data);
		return &(lpBlock1->Data);
	}
	catch (...)
	{
	}

	return NULL;
}
//------------------------------------------------------
//------------------------------ 
template<class Key1,class Key2,class T,typename _ValType,int COUNT>
bool	C2KeyDeques<Key1,Key2,T,_ValType,COUNT>::changeKey	(const Key2& oldKey,const Key2& newKey)
{
	try
	{
		/*新key是已否存在*/ 
		map2_iter pos2 = m_UseMap2.find(newKey);
		if (pos2 != m_UseMap2.end())
			return false;

		/*移除旧key插入新key*/ 
		pos2 = m_UseMap2.find(oldKey);
		if (pos2 != m_UseMap2.end())
		{
			BLOCK* lp_t = pos2->second;
			m_UseMap2.erase(pos2);

			lp_t->key2	= newKey;
			m_UseMap2.insert_(lp_t->key2,lp_t);
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
template<class Key1,class Key2,class T,typename _ValType,int COUNT>
bool	C2KeyDeques<Key1,Key2,T,_ValType,COUNT>::release(const Key1& key1)
{
	BLOCK*	lp_t	= NULL;
	bool	bRet	= true;
	map1_iter pos1;
	map2_iter pos2;
	try
	{
		pos1 = m_UseMap1.find(key1);
		if (pos1 != m_UseMap1.end())
		{
			lp_t = pos1->second;
			m_UseMap1.erase(pos1);

			pos2 = m_UseMap2.find(lp_t->key2);

			if (pos2 != m_UseMap2.end())
				m_UseMap2.erase(pos2);

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
template<class Key1,class Key2,class T,typename _ValType,int COUNT>
bool	C2KeyDeques<Key1,Key2,T,_ValType,COUNT>::release(const Key2& key2)
{
	BLOCK*	lp_t	= NULL;
	bool	bRet	= true;
	map1_iter pos1;
	map2_iter pos2;
	try
	{
		pos2 = m_UseMap2.find(key2);
		if (pos2 != m_UseMap2.end())
		{
			lp_t = pos2->second;
			m_UseMap2.erase(pos2);

			pos1 = m_UseMap1.find(lp_t->key1);

			if (pos1 != m_UseMap1.end())
				m_UseMap1.erase(pos1);

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
template<class Key1,class Key2,class T,typename _ValType,int COUNT>
bool	C2KeyDeques<Key1,Key2,T,_ValType,COUNT>::release(const iterator& pos)
{
	BLOCK*	lp_t = NULL;
	bool	bRet	= true;
	try
	{
		if (pos != m_UseList.end())
		{
			lp_t = (*pos);

			m_UseList.remove(lp_t);

			map1_iter pos1	= m_UseMap1.find(lp_t->key1);
			if (pos1 != m_UseMap1.end())
				m_UseMap1.erase(pos1);

			map2_iter pos2	= m_UseMap2.find(lp_t->key2);
			if (pos2 != m_UseMap2.end())
				m_UseMap2.erase(pos2);
			
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
template<class Key1,class Key2,class T,typename _ValType,int COUNT>
void	C2KeyDeques<Key1,Key2,T,_ValType,COUNT>::clear()
{
	iterator	pos;
	try
	{
		pos = m_UseList.begin();
		for ( ; pos != m_UseList.end(); ++pos )
		{
			_free(*pos);
		}
		m_UseList.clear();
		m_UseMap1.clear();
		m_UseMap2.clear();
	}
	catch (...)
	{
	}
}

//------------------------------------------------------
//------------------------------ 
template<class Key1,class Key2,class T,typename _ValType,int COUNT>
void	C2KeyDeques<Key1,Key2,T,_ValType,COUNT>::getHead(iterator& pos)
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
template<class Key1,class Key2,class T,typename _ValType,int COUNT>
T*		C2KeyDeques<Key1,Key2,T,_ValType,COUNT>::getNext(iterator& pos)
{
	T*		lp_t	= NULL;
	BLOCK*	lpBlock = NULL;
	try
	{
		if(pos != m_UseList.end())
		{
			lpBlock = (*pos);
			if (lpBlock)
				lp_t = &lpBlock->Data;

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
template<class Key1,class Key2,class T,typename _ValType,int COUNT>
T*	C2KeyDeques<Key1,Key2,T,_ValType,COUNT>::getData(const iterator& pos)
{
	T*		lp_t	= NULL;
	BLOCK*	lpBlock = NULL;
	try
	{
		if (pos != m_UseList.end())
		{
			lpBlock = (*pos);
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
template<class Key1,class Key2,class T,typename _ValType,int COUNT>
T*	C2KeyDeques<Key1,Key2,T,_ValType,COUNT>::find(const Key1& key1)
{
	T*		lp_t	= NULL;
	BLOCK*	lpBlock = NULL;
	map1_iter pos;
	try
	{
		pos = m_UseMap1.find(key1);
		if (pos != m_UseMap1.end())
		{
			lpBlock = pos->second;
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
template<class Key1,class Key2,class T,typename _ValType,int COUNT>
T*	C2KeyDeques<Key1,Key2,T,_ValType,COUNT>::find(const Key2& key2)
{
	T*		lp_t	= NULL;
	BLOCK*	lpBlock = NULL;
	map2_iter pos;
	try
	{
		pos = m_UseMap2.find(key2);
		if (pos != m_UseMap2.end())
		{
			lpBlock = pos->second;
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
template<class Key1,class Key2,class T,typename _ValType,int COUNT>
bool	C2KeyDeques<Key1,Key2,T,_ValType,COUNT>::getPos(const Key1& key1,iterator& pos)
{
	try
	{
		pos = m_UseMap1.find(key1);
		if (pos != m_UseMap2.end())
			return true;
	}
	catch (...)
	{
		return false;
	}

	return false;
}

//------------------------------------------------------
//------------------------------ 
template<class Key1,class Key2,class T,typename _ValType,int COUNT>
size_t	C2KeyDeques<Key1,Key2,T,_ValType,COUNT>::size()
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
template<class Key1,class Key2,class T,typename _ValType,int COUNT>
bool	C2KeyDeques<Key1,Key2,T,_ValType,COUNT>::isTail(const iterator& pos)
{
	bool bRes	=	false;

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
template<class Key1,class Key2,class T,typename _ValType,int COUNT>
void	C2KeyDeques<Key1,Key2,T,_ValType,COUNT>::moveToTail(const Key1& key1)
{
	map1_iter pos;
	BLOCK*	lpBlock = NULL;
	try
	{
		pos = m_UseMap1.find(key1);
		if (pos != m_UseMap1.end())
		{
			lpBlock	= pos->second;
			m_UseList.remove(lpBlock);
			m_UseList.push_back(lpBlock);
		}
	}
	catch(...)
	{
		return;
	}
}

//------------------------------------------------------
//------------------------------ 
template<class Key1,class Key2,class T,typename _ValType,int COUNT>
void	C2KeyDeques<Key1,Key2,T,_ValType,COUNT>::moveToTail(const Key2& key2)
{
	map2_iter pos;
	BLOCK*	lpBlock = NULL;
	try
	{
		pos = m_UseMap2.find(key2);
		if (pos != m_UseMap2.end())
		{
			lpBlock	= pos->second;
			m_UseList.remove(lpBlock);
			m_UseList.push_back(lpBlock);
		}
	}
	catch(...)
	{
		return;
	}
}

//------------------------------------------------------
//------------------------------ 下界(包含自己向后)
template<class Key1,class Key2,class T,typename _ValType,int COUNT>
void	C2KeyDeques<Key1,Key2,T,_ValType,COUNT>::lowerBound(map1_iter& pos,Key1&key1)
{
	try
	{
		pos = m_UseMap1.lower_bound(key1);
	}
	catch (...)
	{
	}
}
//------------------------------------------------------
//------------------------------ 上界(不包含自己向后)
template<class Key1,class Key2,class T,typename _ValType,int COUNT>
void	C2KeyDeques<Key1,Key2,T,_ValType,COUNT>::upperBound(map1_iter& pos,Key1&key1)
{
	try
	{
		pos = m_UseMap1.upper_bound(key1);
	}
	catch (...)
	{
	}
}

//------------------------------------------------------
//------------------------------ 
template<class Key1,class Key2,class T,typename _ValType,int COUNT>
T*		C2KeyDeques<Key1,Key2,T,_ValType,COUNT>::getNext(map1_iter& pos)
{
	T*		lp_t	= NULL;
	BLOCK*	lpBlock = NULL;
	try
	{
		if (pos != m_UseMap1.end())
		{
			lpBlock = pos->second;
			if (lpBlock)
				lp_t = &lpBlock->Data;

			++pos;
		}
	}
	catch(...)
	{
	}

	return lp_t;
}

//------------------------------------------------------
//------------------------------ 
template<class Key1,class Key2,class T,typename _ValType,int COUNT>
T*		C2KeyDeques<Key1,Key2,T,_ValType,COUNT>::getData(const map1_iter& pos)
{
	BLOCK*	lpBlock = NULL;
	try
	{
		if (pos != m_UseMap1.end())
			lpBlock = pos->second;

		if (lpBlock)
			return &lpBlock->Data;
	}
	catch(...)
	{
	}
	return NULL;
}

//------------------------------------------------------
//------------------------------ 
template<class Key1,class Key2,class T,typename _ValType,int COUNT>
bool	C2KeyDeques<Key1,Key2,T,_ValType,COUNT>::isTail(const map1_iter& pos)
{
	try
	{
		if (pos != m_UseMap1.end())
			return false;
	}
	catch(...)
	{
	}
	return true;
}

//------------------------------------------------------
//------------------------------ 下界(包含自己向后)
template<class Key1,class Key2,class T,typename _ValType,int COUNT>
void	C2KeyDeques<Key1,Key2,T,_ValType,COUNT>::lowerBound(map2_iter& pos,Key2&key2)
{
	try
	{
		pos = m_UseMap2.lower_bound(key2);
	}
	catch (...)
	{
	}
}
//------------------------------------------------------
//------------------------------ 上界(不包含自己向后)
template<class Key1,class Key2,class T,typename _ValType,int COUNT>
void	C2KeyDeques<Key1,Key2,T,_ValType,COUNT>::upperBound(map2_iter& pos,Key2&key2)
{
	try
	{
		pos = m_UseMap2.upper_bound(key2);
	}
	catch (...)
	{
	}
}

//------------------------------------------------------
//------------------------------ 
template<class Key1,class Key2,class T,typename _ValType,int COUNT>
T*		C2KeyDeques<Key1,Key2,T,_ValType,COUNT>::getNext(map2_iter& pos)
{
	T*		lp_t	= NULL;
	BLOCK*	lpBlock = NULL;
	try
	{
		if (pos != m_UseMap2.end())
		{
			lpBlock = pos->second;
			if (lpBlock)
				lp_t = &lpBlock->Data;

			++pos;
		}
	}
	catch(...)
	{
	}

	return lp_t;
}

//------------------------------------------------------
//------------------------------ 
template<class Key1,class Key2,class T,typename _ValType,int COUNT>
T*		C2KeyDeques<Key1,Key2,T,_ValType,COUNT>::getData(const map2_iter& pos)
{
	BLOCK*	lpBlock = NULL;
	try
	{
		if (pos != m_UseMap2.end())
			lpBlock = pos->second;

		if (lpBlock)
			return &lpBlock->Data;
	}
	catch(...)
	{
	}
	return NULL;
}

//------------------------------------------------------
//------------------------------ 
template<class Key1,class Key2,class T,typename _ValType,int COUNT>
bool	C2KeyDeques<Key1,Key2,T,_ValType,COUNT>::isTail(const map2_iter& pos)
{
	try
	{
		if (pos != m_UseMap2.end())
			return false;
	}
	catch(...)
	{
	}
	return true;
}
