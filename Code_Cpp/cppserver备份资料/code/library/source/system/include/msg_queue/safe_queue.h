/*----------------- safe_queue.h
*
* Copyright (C): 2011  Mokylin��Mokyqi
* Author       : �־�
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
// �������� �������߶���
template<typename T>
class pNc1_queue
{
public:
	enum
	{
		message_pop	,//ȡ������
		message_push,//�������

		message_max
	};

public:
	typedef queue_pool<T>	msg_queue;

protected:
	uint32		m_queue_max;			//�����������
	msg_queue	m_instance[message_max];//��Ϣʵ��
	msg_queue*	m_message[message_max];	//��Ϣָ��

public:
	explicit pNc1_queue()	{	reset();	m_queue_max = 1000;	}
	virtual ~pNc1_queue()	{}

public:
	inline msg_queue*	get_pop	()				{	return m_message[message_pop];	}
	inline msg_queue*	get_push()				{	return m_message[message_push];	}

public:
	//--- ��λ(����,������߳�����д)
	inline void	reset		();
	//--- ������Ϣ����(����,������߳�����д)
	inline void	swap_queue	();

public:
	//--- ȡ����Ϣͷ(���̲߳���Ҫ����)
	inline T*			front	();
	//--- �Ƿ�Ϊ��
	inline bool				pop_empty();
	//--- ȡ����Ϣ(���̲߳���Ҫ����)
	inline void				pop		();

protected:
	//--- д����Ϣ(���߳���Ҫ����)
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
//------------------------------ ������Ϣ����
template<typename T>
inline void	pNc1_queue<T>::swap_queue()
{
	try
	{
		//�л�ʱ��Ҫ������������
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
//------------------------------ �Ƿ�Ϊ��
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
//		//������
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