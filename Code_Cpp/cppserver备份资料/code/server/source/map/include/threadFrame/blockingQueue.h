#ifndef __BLOCKING_QUEUE_H__
#define __BLOCKING_QUEUE_H__
#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>
#include <mutex>          // std::mutex
#include <condition_variable> 
#include <list>
#include "eventBase.h"

//todo,数组改成单向循环链表，当队列满的时候，插入者负责增加新的链表节点
//基于loopbuff的blockingqueue，使用condition variable唤醒和等待
class IBlockingQueueLoopBuff
{
protected:
#define LOOPBUFFCOUNT 2000
	int                     m_IndexHead;
	int                     m_IndexTail;
	IEvent*    m_pLoopBuff[LOOPBUFFCOUNT];
	std::mutex              m_mtx;
	std::condition_variable m_cv;

	int getNext(int i)
	{
		return  (i + 1) % LOOPBUFFCOUNT;
	}
public:
	IBlockingQueueLoopBuff()
	{
		for (int i = 0; i<LOOPBUFFCOUNT; ++i)
		{
			m_pLoopBuff[i] = NULL;
		}
		m_IndexHead = 0;
		m_IndexTail = 0;

	}

public:
	virtual bool init()
	{
		return true;
	}
	virtual void uninit()
	{
	}

	virtual void lockAdd() {};
	virtual void unlockAdd() {};
	virtual void signalNotify()
	{
		std::unique_lock<std::mutex> lck(m_mtx);
		m_cv.notify_one();
	};
	virtual bool push(IEvent*e)
	{
		if (!e)
		{
			return false;
		}
		//检查是否可以插入，如果不可以返回。
		if (getNext(m_IndexTail) == m_IndexHead)
		{
			return false;
		}
		lockAdd();

		//todo 这里的执行顺序需要用原子栅栏做一下保护
		//或者代码编译的时候不要做任何优化
		m_pLoopBuff[m_IndexTail] = e;
		m_IndexTail = getNext(m_IndexTail);
		// std::cout<<"add "<<e->m_count<< " new tail "<< m_IndexTail<<std::endl;

		unlockAdd();
		signalNotify();
		return true;
	}

	virtual void waitNotify()
	{
		std::unique_lock<std::mutex> lck(m_mtx);
		m_cv.wait(lck);
	}
	virtual void lockPop() {};
	virtual void unlockPop() {};
	virtual bool pop(IEvent* & r)
	{
		if (m_pLoopBuff[m_IndexHead] == NULL)
			waitNotify();
		lockPop();
		if (m_pLoopBuff[m_IndexHead] != NULL)
		{
			//todo 这里的执行顺序需要用原子栅栏做一下保护
			r = m_pLoopBuff[m_IndexHead];
			m_pLoopBuff[m_IndexHead] = NULL;
			m_IndexHead = getNext(m_IndexHead);
			// std::cout<<"pop "<<r->m_count<< " new head "<< m_IndexHead<<std::endl;

		}
		unlockPop();
		return true;
	}
};

//单消费者，单生产者，读写都不需要锁
class CSPSCLoopBuff : public IBlockingQueueLoopBuff
{

};

#include <event2/event.h>
//单消费者，单生产者，读写都不需要锁，使用libevent做唤醒机制
//为了配合libevent线程本身就有的阻塞机制。
class CSPSCLELoopBuff : public IBlockingQueueLoopBuff
{
private:
	struct event * m_pEvent;//timer的事件
	struct timeval m_tv;

public:
	event * getEvent()
	{
		return m_pEvent;
	}
public:
	virtual void signalNotify()
	{
		if (m_pEvent)
		{
			event_active(m_pEvent, EV_TIMEOUT, 0);
		}

	}
	virtual void waitNotify()
	{//等待的机制由libevent负责
	}
	virtual bool init()
	{
		return false;
	}
	//bool onNewEventCallback()
	//{
	//	IEvent* p = nullptr;
	//	pop(p);
	//	while (p)
	//	{
	//		//针对事件的回调
	//		//std::cout << " delete p " << p->m_count << "  " << p->print() << std::endl;
	//		////简单的删除
	//		////delete p;
	//		//p = nullptr;
	//		pop(p);
	//	}
	//	return true;
	//}
	//static void cb(int sock, short which, void *arg)
	//{
	//	if (!arg)
	//	{
	//		return;
	//	}
	//	CSPSCLELoopBuff* p = (CSPSCLELoopBuff*)arg;
	//	if (!p)
	//	{
	//		return;
	//	}
	//	// if (!evtimer_pending(p->m_pEv, NULL)) 
	//	//重新加入timer
	//	if(p->m_pEvent)
	//	{
	//		event_del(p->m_pEvent);
	//		evtimer_add(p->m_pEvent, &p->m_tv);
	//		//p->onNewEventCallback();
	//	}
	//}
	void rescheduleTimer()
	{
		if(m_pEvent)
		{
			event_del(m_pEvent);
			evtimer_add(m_pEvent, &m_tv);
		}
	}
	//上层需要指定回调函数
	bool init(event_base* base, int nCDSec, int nCDUsec,
		void(*cb)(evutil_socket_t, short, void *)
		, void * arg)
	{
		if (base)
		{
			m_tv.tv_sec = nCDSec;
			m_tv.tv_usec = nCDUsec;

			m_pEvent = evtimer_new(base, cb, arg);

			if (m_pEvent
				&& 0 == evtimer_add(m_pEvent, &m_tv))
				return true;
		}
		return false;
	}
	virtual void uninit()
	{
		if (m_pEvent)
		{
			event_free(m_pEvent);
			m_pEvent = NULL;
		}
	}
};



//单消费者，多生产者，写需要锁
class CMPSCLoopBuff : public IBlockingQueueLoopBuff
{
public:
	virtual void lockAdd() {};
	virtual void unlockAdd() {};

	virtual void signalNotify()
	{
	}
	//将触发操作写在投递里面 
	virtual bool push(IEvent*e)
	{
		if (!e)
		{
			return false;
		}
		//检查是否可以插入，如果不可以返回。
		if (getNext(m_IndexTail) == m_IndexHead)
		{
			return false;
		}
		std::unique_lock<std::mutex> lck(m_mtx);//取得临界区
		m_pLoopBuff[m_IndexTail] = e;
		m_IndexTail = getNext(m_IndexTail);
		// std::cout<<"add "<<e->m_count<< " new tail "<< m_IndexTail<<std::endl;
		m_cv.notify_one();//触发通知

		return true;
	}
};



#endif // ! __BLOCKING_QUEUE_H__
