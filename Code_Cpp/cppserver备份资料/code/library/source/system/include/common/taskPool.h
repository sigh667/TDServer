/*----------------- taskPool.h
*
* Copyright (C): 2011  Mokylin·Mokyqi
* Author       : 林驹
* Version      : V1.0
* Date         : 2016/11/21 20:01:36
*--------------------------------------------------------------
*
*------------------------------------------------------------*/
#pragma once
#include "singleton/singleton.h"
#include "stl/stl_vector.h"
#include "dbMysql/include/dbMySql.h"
#include "msg_queue/safe_queue.h"
#include "threads/thread.h"
#include "threads/condEvent.h"
#include "threads/csLocker.h"
#include "stackChecker/stackChecker.h"
/*************************************************************/
template<typename T>
class ITaskQueue : public pNc1_queue<T>
{
protected:
	int32					m_id;			// 队列标识ID
	CThread					m_clThread;		// 线程
	CCondEvent				m_clStopEvent;	// 退出循环事件
	CMutex_					m_mutex;		// 线程安全互斥锁
	int32					m_timeout;		// 线程处理时间间隔

public:
	explicit ITaskQueue();
	virtual ~ITaskQueue();

	virtual bool	create(int32 id, int32 timeout = 100);
	virtual void	release();
	virtual void	stop(int32 timeout = 1000/*ms*/);
	virtual void	process(T *task);

protected:
	static	THREAD_RETURN	processThread(void* pParam);
};

template<typename T>
ITaskQueue<T>::ITaskQueue()
	: m_id(0)
{
}

template<typename T>
ITaskQueue<T>::~ITaskQueue()
{
	release();
}

template<typename T>
bool ITaskQueue<T>::create(int32 id, int32 timeout)
{
	CHECKF(id >= 0 && timeout >= 0);
	m_id = id;
	m_timeout = timeout;
	CHECKF(m_clStopEvent.create_event(true, false));
	m_clStopEvent.reset_event();
	CHECKF(m_clThread.create(ITaskQueue::processThread, this));
	return true;
}

template<typename T>
void ITaskQueue<T>::release()
{
	stop();
	pNc1_queue<T>::reset();
}

template<typename T>
void ITaskQueue<T>::stop(int32 timeout)
{
	if (m_clStopEvent.isValid())
	{
		m_clStopEvent.set_event();
		while (timeout > 0)
		{
			if (!m_clThread.is_runing())
				break;

			dSleep(10);
			timeout -= 10;
		}
		m_clThread.shutdown();
		m_clStopEvent.close_event();
	}
}

template<typename T>
void ITaskQueue<T>::process(T *task)
{
	if (NULL != task)
	{
		task->run();
	}
}

template<typename T>
THREAD_RETURN ITaskQueue<T>::processThread(void* pParam)
{
	INIT_STACK_CHECKER();
	ITaskQueue *lpThis	= (ITaskQueue*)pParam;
	if (!lpThis)
		return 0;

	while (!lpThis->m_clStopEvent.wait_event(lpThis->m_timeout))
	{
		lpThis->pNc1_queue<T>::swap_queue();
		T *pTask = NULL;
		while (pTask = lpThis->pNc1_queue<T>::front())
		{
			lpThis->process(pTask);
			lpThis->pNc1_queue<T>::pop();
		}	
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////
template<typename TQueue, typename T>
class ITaskPool : public stl_vector<TQueue>
{
public:
	explicit ITaskPool();
	virtual ~ITaskPool();

	virtual bool	create(int32 num, int32 timeout);
	virtual void	release();
};

template<typename TQueue, typename T>
ITaskPool<TQueue, T>::ITaskPool()
{
}

template<typename TQueue, typename T>
ITaskPool<TQueue, T>::~ITaskPool()
{
}

template<typename TQueue, typename T>
bool ITaskPool<TQueue, T>::create(int32 num, int32 timeout)
{
	CHECKF(num >= 0);
	resize(num);
	for (int32 i = 0; i < num; ++i)
	{
		(*this)[i].create(i, timeout);
	}
	return true;
}

template<typename TQueue, typename T>
void ITaskPool<TQueue, T>::release()
{
	for (size_t i = 0; i < size(); ++i)
	{
		(*this)[i].release();
	}
	clear();
}