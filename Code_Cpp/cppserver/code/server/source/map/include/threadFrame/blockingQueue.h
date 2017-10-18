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

//todo,����ĳɵ���ѭ����������������ʱ�򣬲����߸��������µ�����ڵ�
//����loopbuff��blockingqueue��ʹ��condition variable���Ѻ͵ȴ�
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
		//����Ƿ���Բ��룬��������Է��ء�
		if (getNext(m_IndexTail) == m_IndexHead)
		{
			return false;
		}
		lockAdd();

		//todo �����ִ��˳����Ҫ��ԭ��դ����һ�±���
		//���ߴ�������ʱ��Ҫ���κ��Ż�
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
			//todo �����ִ��˳����Ҫ��ԭ��դ����һ�±���
			r = m_pLoopBuff[m_IndexHead];
			m_pLoopBuff[m_IndexHead] = NULL;
			m_IndexHead = getNext(m_IndexHead);
			// std::cout<<"pop "<<r->m_count<< " new head "<< m_IndexHead<<std::endl;

		}
		unlockPop();
		return true;
	}
};

//�������ߣ��������ߣ���д������Ҫ��
class CSPSCLoopBuff : public IBlockingQueueLoopBuff
{

};

#include <event2/event.h>
//�������ߣ��������ߣ���д������Ҫ����ʹ��libevent�����ѻ���
//Ϊ�����libevent�̱߳�����е��������ơ�
class CSPSCLELoopBuff : public IBlockingQueueLoopBuff
{
private:
	struct event * m_pEvent;//timer���¼�
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
	{//�ȴ��Ļ�����libevent����
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
	//		//����¼��Ļص�
	//		//std::cout << " delete p " << p->m_count << "  " << p->print() << std::endl;
	//		////�򵥵�ɾ��
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
	//	//���¼���timer
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
	//�ϲ���Ҫָ���ص�����
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



//�������ߣ��������ߣ�д��Ҫ��
class CMPSCLoopBuff : public IBlockingQueueLoopBuff
{
public:
	virtual void lockAdd() {};
	virtual void unlockAdd() {};

	virtual void signalNotify()
	{
	}
	//����������д��Ͷ������ 
	virtual bool push(IEvent*e)
	{
		if (!e)
		{
			return false;
		}
		//����Ƿ���Բ��룬��������Է��ء�
		if (getNext(m_IndexTail) == m_IndexHead)
		{
			return false;
		}
		std::unique_lock<std::mutex> lck(m_mtx);//ȡ���ٽ���
		m_pLoopBuff[m_IndexTail] = e;
		m_IndexTail = getNext(m_IndexTail);
		// std::cout<<"add "<<e->m_count<< " new tail "<< m_IndexTail<<std::endl;
		m_cv.notify_one();//����֪ͨ

		return true;
	}
};



#endif // ! __BLOCKING_QUEUE_H__
