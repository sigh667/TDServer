/*----------------- safe_queue.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 林驹
* Version      : V1.0
* Date         : 2012/7/12 20:58:24
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "memoryPool/queue_pool.h"
/*************************************************************/
//-------------------------------------------------------------
//------------------------------
// 多生成者 单消费者队列
template<typename T>
class pNc1_queue
{
public:
	enum
	{
		message_pop	,//取出队列
		message_push,//加入队列

		message_max
	};

public:
	typedef queue_pool<T>	msg_queue;

protected:
	uint32		m_queue_max;			//队列最大数量
	msg_queue	m_instance[message_max];//消息实例
	msg_queue*	m_message[message_max];	//消息指针

public:
	explicit pNc1_queue()	{	reset();	m_queue_max = 1000;	}
	virtual ~pNc1_queue()	{}

public:
	inline msg_queue*	get_pop	()				{	return m_message[message_pop];	}
	inline msg_queue*	get_push()				{	return m_message[message_push];	}

public:
	//--- 复位(加锁,避免多线程正在写)
	inline void	reset		();
	//--- 交换消息队列(加锁,避免多线程正在写)
	inline void	swap_queue	();

public:
	//--- 取出消息头(单线程不需要加锁)
	inline T*			front	();
	//--- 是否为空
	inline bool				pop_empty();
	//--- 取出消息(单线程不需要加锁)
	inline void				pop		();

protected:
	//--- 写入消息(多线程需要加锁)
//	inline T*			push	();
};
//-------------------------------------------------------------
//------------------------------ 
template<typename T>
inline void	pNc1_queue<T>::reset()
{
	try
	{
		CCritLocker clLock_pop(m_instance[message_pop].get_mutex());
		CCritLocker clLock_push(m_instance[message_push].get_mutex());

		for (int i = 0;i < message_max;i++)
		{
			m_message[i] = &m_instance[i];
			m_instance[i].clear();
		}
	}
	catch (...)
	{
	}
}
//-------------------------------------------------------------
//------------------------------ 交换消息队列
template<typename T>
inline void	pNc1_queue<T>::swap_queue()
{
	try
	{
		//切换时需要对两个都加锁
		CCritLocker clLock_pop(m_instance[message_pop].get_mutex());
		CCritLocker clLock_push(m_instance[message_push].get_mutex());

		msg_queue* _temp			= m_message[message_pop	];
		m_message[message_pop	]	= m_message[message_push];
		m_message[message_push	]	= _temp;
	}
	catch (...)
	{
	}
}

//-------------------------------------------------------------
//------------------------------ 
template<typename T>
inline T*	pNc1_queue<T>::front()
{
	try
	{
		if(m_message[message_pop])
			return m_message[message_pop]->front();
	}
	catch (...)
	{
	}

	return NULL;
}
//-------------------------------------------------------------
//------------------------------ 
template<typename T>
inline void	pNc1_queue<T>::pop()
{
	try
	{
		if(m_message[message_pop])
			return m_message[message_pop]->pop();
	}
	catch (...)
	{
	}
}

//-------------------------------------------------------------
//------------------------------ 是否为空
template<typename T>
inline bool	pNc1_queue<T>::pop_empty	()
{
	try
	{
		if(m_message[message_pop])
			return m_message[message_pop]->empty();
	}
	catch (...)
	{
	}
	return true;
}

////-------------------------------------------------------------
////------------------------------ 
//template<typename T>
//inline T*	pNc1_queue<T>::push()
//{
//	try
//	{
//		if(!m_message[message_push])
//			return NULL;
//
//		//增加锁
//		CCritLocker clLock(m_message[message_push]->get_mutex());
//		if(m_queue_max && m_message[message_push]->size() > m_queue_max)
//			return NULL;
//
//		return m_message[message_push]->push();
//	}
//	catch (...)
//	{
//	}
//	return NULL;
//}