/*----------------- deque_pool.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/7/25 15:48:12
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "stl/std_deque.h"
#include "stl/stl_list.h"
#include "threads/csLocker.h"
#include "poolVal.h"
/*************************************************************/
template<typename _Ty, typename _ValType = _Val_Init<_Ty> >
class deque_pool
{
protected:
	typedef stl_deque<_Ty*>				DEQUE;
	typedef stl_list<_Ty*>				LIST;

public:
	typedef	typename DEQUE::iterator	iterator;

public:
	CMutex_	m_mutex;		//线程安全互斥锁
	DEQUE	m_use_queue;	//使用队列
	LIST	m_free_list;	//释放队列

public:
	inline bool		empty		()const			{	return m_use_queue.empty();		}
	inline size_t	size		()const			{	return m_use_queue.size();		}
	inline CMutex_&	get_mutex	()				{	return m_mutex;					}
	inline iterator	begin		()				{	return m_use_queue.begin();		}
	inline iterator	end			()				{	return m_use_queue.end();		}

public:
	deque_pool();
	virtual~deque_pool();

protected:
	inline void	_build	();
	inline _Ty*	_malloc	();
	inline void	_free	(_Ty*_val);

public:
	inline void		destroy		();
	inline void		clear		();
	inline iterator	erase		(iterator&pos);
	inline void		pop_front	();
	inline void		pop_back	();
	inline _Ty*		data		(iterator&pos);
	inline _Ty*		push_front	();
	inline _Ty*		push_back	();
	inline _Ty*		front		();
	inline _Ty*		back		();
};
//-------------------------------------------------------------
//------------------------------ 
template<typename _Ty, typename _ValType>
deque_pool<_Ty,_ValType>::deque_pool()
{
}
//-------------------------------------------------------------
//------------------------------ 
template<typename _Ty, typename _ValType>
deque_pool<_Ty,_ValType>::~deque_pool()
{
	destroy();
}

//-------------------------------------------------------------
//------------------------------ 
template<typename _Ty, typename _ValType>
inline void	deque_pool<_Ty,_ValType>::_build	()
{
	try
	{
		//分配对像内存
		_Ty*_val = NULL;
		for (int i = 0;i < 1;i++)
		{
			_val = new _Ty;
			if(_val)
				m_free_list.push_back(_val);
		}
	}
	catch (...)
	{
	}
}
//-------------------------------------------------------------
//------------------------------ 
template<typename _Ty, typename _ValType>
inline _Ty*	deque_pool<_Ty,_ValType>::_malloc()
{
	_Ty*_val = NULL;
	try
	{
		if (m_free_list.empty())
			_build();

		if (!m_free_list.empty())
		{
			_val = m_free_list.front();
			m_free_list.pop_front();
		}

		if (NULL == _val)
			_val = new _Ty;
	}
	catch (...)
	{
	}

	return _val;
}
//-------------------------------------------------------------
//------------------------------ 
template<typename _Ty, typename _ValType>
inline void	deque_pool<_Ty,_ValType>::_free	(_Ty*_val)
{
	try
	{
		if(_val)
		{
			//_ValType::initialize(_val);
			m_free_list.push_front(_val);
		}
	}
	catch (...)
	{
	}
}

//-------------------------------------------------------------
//------------------------------ 
template<typename _Ty, typename _ValType>
inline void	deque_pool<_Ty,_ValType>::destroy()
{
	try
	{
		iterator _pos = m_use_queue.begin();
		iterator _end = m_use_queue.end();
		//销毁使用队列
		for ( ; _pos != _end; ++_pos)
		{
			safe_delete(*_pos);
		}

		//销毁释放队列
		for (auto iter = m_free_list.begin(); iter != m_free_list.end(); ++iter)
		{
			safe_delete(*iter);
		}
		m_free_list.clear();
	}
	catch (...)
	{
	}
}
//-------------------------------------------------------------
//------------------------------ 
template<typename _Ty, typename _ValType>
inline void	deque_pool<_Ty,_ValType>::clear	()
{
	try
	{
		//销毁使用队列
		iterator _pos = m_use_queue.begin();
		iterator _end = m_use_queue.end();
		for (;_pos != _end;++_pos)
			_free(*_pos);

		m_use_queue.clear();
	}
	catch (...)
	{
	}
}
//-------------------------------------------------------------
//------------------------------ 
template<typename _Ty, typename _ValType>
inline typename deque_pool<_Ty,_ValType>::iterator	deque_pool<_Ty,_ValType>::erase		(iterator&pos)
{
	try
	{
		if(pos != m_use_queue.end())
		{
			_free(*pos);
			pos = m_use_queue.erase(pos);
		}
	}
	catch (...)
	{
	}

	return pos;
}

//-------------------------------------------------------------
//------------------------------ 
template<typename _Ty, typename _ValType>
inline void	deque_pool<_Ty,_ValType>::pop_front	()
{
	try
	{
		if(!m_use_queue.empty())
		{
			_free(m_use_queue.front());
			m_use_queue.pop_front();
		}
	}
	catch (...)
	{
	}
}
//-------------------------------------------------------------
//------------------------------ 
template<typename _Ty, typename _ValType>
inline void	deque_pool<_Ty,_ValType>::pop_back	()
{
	try
	{
		if(!m_use_queue.empty())
		{
			_free(m_use_queue.back());
			m_use_queue.pop_back();
		}
	}
	catch (...)
	{
	}
}
//-------------------------------------------------------------
//------------------------------ 
template<typename _Ty, typename _ValType>
inline _Ty*	deque_pool<_Ty,_ValType>::data		(iterator&pos)
{
	try
	{
		if(pos != m_use_queue.end())
			return (*pos);
	}
	catch (...)
	{
	}

	return NULL;
}

//-------------------------------------------------------------
//------------------------------ 
template<typename _Ty, typename _ValType>
inline _Ty*	deque_pool<_Ty,_ValType>::push_front	()
{
	_Ty*_val = NULL;
	try
	{
		_val = _malloc();
		if(_val)
			m_use_queue.push_front(_val);
	}
	catch (...)
	{
	}
	_ValType::initialize(_val);
	return _val;
}
//-------------------------------------------------------------
//------------------------------ 
template<typename _Ty, typename _ValType>
inline _Ty*	deque_pool<_Ty,_ValType>::push_back	()
{
	_Ty*_val = NULL;
	try
	{
		_val = _malloc();
		if(_val)
			m_use_queue.push_back(_val);
	}
	catch (...)
	{
	}
	_ValType::initialize(_val);
	return _val;
}
//-------------------------------------------------------------
//------------------------------ 
template<typename _Ty, typename _ValType>
inline _Ty*	deque_pool<_Ty,_ValType>::front	()
{
	_Ty*_val = NULL;
	try
	{
		if(!m_use_queue.empty())
			_val = m_use_queue.front();
	}
	catch (...)
	{
	}

	return _val;
}
//-------------------------------------------------------------
//------------------------------ 
template<typename _Ty, typename _ValType>
inline _Ty*	deque_pool<_Ty,_ValType>::back	()
{
	_Ty*_val = NULL;
	try
	{
		if(!m_use_queue.empty())
			_val = m_use_queue.back();
	}
	catch (...)
	{
	}

	return _val;
}
