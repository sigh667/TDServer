#ifndef __BLOCKING_QUEUE_H__
#define __BLOCKING_QUEUE_H__
#include <iostream>
#include <chrono>
#include <ctime>
#include <atomic>
#include <thread>
#include <mutex>          // std::mutex
#include <condition_variable> 
#include <chrono>
#include <list>
#include "eventBase.h"
#include "byteArray.h"
#include "logic/event.h"

//todo,数组改成单向循环链表，当队列满的时候，插入者负责增加新的链表节点
//基于loopbuff的blockingqueue，使用condition variable唤醒和等待

class CConditionVariable
{
public:
	void signaleAll()
	{
		std::unique_lock<std::mutex> lck(m_mtx);
		m_cv.notify_all();
	}
	void signalOnce()
	{
		std::unique_lock<std::mutex> lck(m_mtx);
		m_cv.notify_one();
	}
	void wait()
	{
		std::unique_lock<std::mutex> lck(m_mtx);
		m_cv.wait(lck);
	}
	void waitFor(int milliSeconds/*timer只做1秒以内的，milliseconds不能超过1000*/)
	{
		//按照实际需要的timer来做比如timer的间隔是20毫秒，如果timer本身逻辑已经用掉4毫秒，那么下次应该是16毫秒以后回调
		if (m_bFirstRun)
		{
			m_bFirstRun = false;
			m_triggleTime = std::chrono::system_clock::now();
		}
		std::chrono::system_clock::time_point _now = std::chrono::system_clock::now();
		while (m_triggleTime  <= _now)
		{
			m_triggleTime += std::chrono::milliseconds(milliSeconds);
		}
		std::unique_lock<std::mutex> lck(m_mtx);
		if (std::cv_status::timeout == m_cv.wait_until(lck, m_triggleTime))
		{
			//正常的timer
		}
		else
		{
			//有signal发生
		}	

		//先不加区分返回值
	}

private:
	std::mutex									m_mtx;
	std::condition_variable						m_cv;
	std::chrono::system_clock::time_point		m_triggleTime;
	bool										m_bFirstRun{ true };
};

class IBlockingQueue
{
public:
	virtual bool init()	{		return true;	}
	virtual void uninit()	{	}

	virtual void lockAdd() {};
	virtual void unlockAdd() {};
	virtual void signalNotify()	{	};
	virtual bool push(IEvent*e) { return false; };

	virtual void waitNotify() {	};
	virtual void lockPop() {};
	virtual void unlockPop() {};
	virtual bool pop(IEvent* & r) { return false; }
};

class CMPSCBlockingQueueByteArray 
{
public:
	//采用bytearray以后，就不需要使用事件了。
	//push pop的两个处理是伪的，需要特别注意
	//上层也不需要传一个真的指针下来
	//pop也不会真的传一个指针出去
	virtual bool push(CEventBAHead *e)
	{

		if (e )
		{
			lockAdd();
			e->writeTo(&m_byteArray);
			unlockAdd();
			signalNotify();
		}
	};
	virtual void signalNotify()
	{
		m_cond.signalOnce();
	};
	virtual void waitNotify()
	{
		m_cond.wait();
	}

public:
	void lockAdd()
	{//原子变量做的自旋锁
		while (m_addLock.test_and_set());
	};
	void unlockAdd()
	{
		m_addLock.clear();
	};
	CByteArray* getByteArray()
	{
		return &m_byteArray;
	}
private :
	CByteArray m_byteArray;
private :
	CConditionVariable m_cond;
	std::atomic_flag m_addLock{ ATOMIC_FLAG_INIT };

};


class CBlockingQueueLoopBuff:public IBlockingQueue
{
protected:
#define LOOPBUFFCOUNT 2000
	int                     m_IndexHead;
	int                     m_IndexTail;
	IEvent*    m_pLoopBuff[LOOPBUFFCOUNT];
	int getNext(int i)
	{
		return  (i + 1) % LOOPBUFFCOUNT;
	}
	CConditionVariable m_cond;
public:
	CBlockingQueueLoopBuff()
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
		m_cond.signalOnce();
	};
	virtual bool push(IEvent*e)
	{
		bool r = false;
		if (!e)
		{
			return r;
		}
		lockAdd();
		//检查是否可以插入，如果不可以返回。
		if (getNext(m_IndexTail) == m_IndexHead)
		{
			//不做任何事情;
			r = false;
		}
		else
		{
			//todo 这里的执行顺序需要用原子栅栏做一下保护
			//或者代码编译的时候不要做任何优化
			m_pLoopBuff[m_IndexTail] = e;
			m_IndexTail = getNext(m_IndexTail);
			// std::cout<<"add "<<e->m_count<< " new tail "<< m_IndexTail<<std::endl;
			r = true;
		}
		unlockAdd();
		if (r)
		{
			signalNotify();
		}
		return r;
	}

	virtual void waitNotify()
	{
		m_cond.wait();
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

//单消费者，多生产者，读写都不需要锁
class CSPSCLoopBuff : public CBlockingQueueLoopBuff
{
};

//由于我们没有actor式的线程池，一般都是多生产者单消费者，
class CMPSCLoopBuff : public CBlockingQueueLoopBuff
{
private :
	std::atomic_flag m_addLock{ ATOMIC_FLAG_INIT };
public :
	void lockAdd() override 
	{//原子变量做的自旋锁
		while (m_addLock.test_and_set());
	};
	void unlockAdd() override 
	{
		m_addLock.clear();
	};

};




#include <event2/event.h>
//单消费者，单生产者，读写都不需要锁，使用libevent做唤醒机制
//为了配合libevent线程本身就有的阻塞机制。
class CSPSCLELoopBuff : public CSPSCLoopBuff
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
	void rescheduleTimer()
	{
		if(m_pEvent)
		{
			event_del(m_pEvent);
			evtimer_add(m_pEvent, &m_tv);
		}
	}
	//上层需要指定回调函数
	bool init(event_base* base, int nCDSec, int nCDMicroSec,
		void(*cb)(evutil_socket_t, short, void *)
		, void * arg)
	{
		if (base)
		{
			m_tv.tv_sec = nCDSec;
			m_tv.tv_usec = nCDMicroSec;

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

class CMPSCLEByteArray : public CMPSCBlockingQueueByteArray
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
	void rescheduleTimer()
	{
		if (m_pEvent)
		{
			event_del(m_pEvent);
			evtimer_add(m_pEvent, &m_tv);
		}
	}
	//上层需要指定回调函数
	bool init(event_base* base, int nCDSec, int nCDMicroSec,
		void(*cb)(evutil_socket_t, short, void *)
		, void * arg)
	{
		if (base)
		{
			m_tv.tv_sec = nCDSec;
			m_tv.tv_usec = nCDMicroSec;

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


#endif // ! __BLOCKING_QUEUE_H__
