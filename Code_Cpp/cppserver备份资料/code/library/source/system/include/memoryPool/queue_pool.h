/*----------------- queue_pool.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 赵文源
* Version      : V1.0
* Date         : 2012/7/12 19:58:38
*--------------------------------------------------------------
*队列池
*------------------------------------------------------------*/
#pragma once
#include "stl/stl_queue.h"
#include "stl/stl_list.h"
#include "threads/csLocker.h"
#include "memoryPool/mem_malloc.h"
#include "poolVal.h"
/*************************************************************/
template<typename _Ty, typename _ValType = _Val_Init<_Ty> >
class queue_pool
{
private:
	enum
	{
		_block_max = 10000,//每次申请块最大尺寸
	};
protected:
	typedef stl_queue<_Ty*>		QUEUE;
	typedef stl_list<_Ty*>		LIST;

public:
	CMutex_	m_mutex;		//线程安全互斥锁
	int		m_block_size;	//每次申请块大小(默认1,最大10000)
	QUEUE	m_use_queue;	//使用队列
	LIST	m_free_list;	//释放队列

public:
	inline bool		empty			()const		{	return m_use_queue.empty();	}
	inline size_t	size			()const		{	return m_use_queue.size();	}
	inline void		set_block_size	(int _size)	{	m_block_size = _size;		}
	inline CMutex_&	get_mutex		()			{	return m_mutex;				}

public:
	queue_pool();
	virtual~queue_pool();

protected:
	inline void	_build	();
	inline _Ty*	_malloc	();
	inline void	_free	(_Ty*_val);

public:
	inline void	destroy	();
	inline void	clear	();
	inline void	pop		();
	inline _Ty*	push	();
	inline _Ty*	front	();
	inline _Ty*	back	();
};
//-------------------------------------------------------------
//------------------------------ 
template<typename _Ty, typename _ValType>
queue_pool<_Ty, _ValType>::queue_pool()
{
	m_block_size = 1;
}
//-------------------------------------------------------------
//------------------------------ 
template<typename _Ty, typename _ValType>
queue_pool<_Ty, _ValType>::~queue_pool()
{
	destroy();
}

//-------------------------------------------------------------
//------------------------------ 
template<typename _Ty, typename _ValType>
inline void queue_pool<_Ty, _ValType>::_build	()
{
	//调整尺寸
	if(m_block_size < 1)
		m_block_size = 1;

	if(m_block_size > _block_max)
		m_block_size = _block_max;

	try
	{
		//分配对像内存
		for (int i = 0;i < m_block_size;i++)
		{
			m_free_list.push_back(new _Ty);
		}
	}
	catch (...)
	{
	}
}
//-------------------------------------------------------------
//------------------------------ 
template<typename _Ty, typename _ValType>
inline _Ty* queue_pool<_Ty, _ValType>::_malloc()
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
inline void queue_pool<_Ty, _ValType>::_free	(_Ty*_val)
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
inline void queue_pool<_Ty, _ValType>::destroy()
{
	try
	{
		_Ty*_val = NULL;
		//销毁使用队列
		while(!m_use_queue.empty())
		{
			_val = m_use_queue.front();
			m_use_queue.pop();
			safe_delete(_val);
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
inline void queue_pool<_Ty, _ValType>::clear	()
{
	try
	{
		//销毁使用队列
		while(!m_use_queue.empty())
		{
			_free(m_use_queue.front());
			m_use_queue.pop();
		}
	}
	catch (...)
	{
	}
}
//-------------------------------------------------------------
//------------------------------ 
template<typename _Ty, typename _ValType>
inline void queue_pool<_Ty, _ValType>::pop	()
{
	try
	{
		if(!m_use_queue.empty())
		{
			_free(m_use_queue.front());
			m_use_queue.pop();
		}
	}
	catch (...)
	{
	}
}
//-------------------------------------------------------------
//------------------------------ 
template<typename _Ty, typename _ValType>
inline _Ty*	queue_pool<_Ty, _ValType>::push	()
{
	_Ty*_val = NULL;
	try
	{
		_val = _malloc();
		if(_val)
			m_use_queue.push(_val);
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
inline _Ty* queue_pool<_Ty, _ValType>::front	()
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
inline _Ty* queue_pool<_Ty, _ValType>::back	()
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