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

//todo,����ĳɵ���ѭ����������������ʱ�򣬲����߸��������µ�����ڵ�
//����loopbuff��blockingqueue��ʹ��condition variable���Ѻ͵ȴ�

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
	void waitFor(int milliSeconds/*timerֻ��1�����ڵģ�milliseconds���ܳ���1000*/)
	{
		//����ʵ����Ҫ��timer��������timer�ļ����20���룬���timer�����߼��Ѿ��õ�4���룬��ô�´�Ӧ����16�����Ժ�ص�
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
			//������timer
		}
		else
		{
			//��signal����
		}	

		//�Ȳ������ַ���ֵ
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
	//����bytearray�Ժ󣬾Ͳ���Ҫʹ���¼��ˡ�
	//push pop������������α�ģ���Ҫ�ر�ע��
	//�ϲ�Ҳ����Ҫ��һ�����ָ������
	//popҲ������Ĵ�һ��ָ���ȥ
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
	{//ԭ�ӱ�������������
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
		//����Ƿ���Բ��룬��������Է��ء�
		if (getNext(m_IndexTail) == m_IndexHead)
		{
			//�����κ�����;
			r = false;
		}
		else
		{
			//todo �����ִ��˳����Ҫ��ԭ��դ����һ�±���
			//���ߴ�������ʱ��Ҫ���κ��Ż�
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
class CSPSCLoopBuff : public CBlockingQueueLoopBuff
{
};

//��������û��actorʽ���̳߳أ�һ�㶼�Ƕ������ߵ������ߣ�
class CMPSCLoopBuff : public CBlockingQueueLoopBuff
{
private :
	std::atomic_flag m_addLock{ ATOMIC_FLAG_INIT };
public :
	void lockAdd() override 
	{//ԭ�ӱ�������������
		while (m_addLock.test_and_set());
	};
	void unlockAdd() override 
	{
		m_addLock.clear();
	};

};




#include <event2/event.h>
//�������ߣ��������ߣ���д������Ҫ����ʹ��libevent�����ѻ���
//Ϊ�����libevent�̱߳�����е��������ơ�
class CSPSCLELoopBuff : public CSPSCLoopBuff
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
	void rescheduleTimer()
	{
		if(m_pEvent)
		{
			event_del(m_pEvent);
			evtimer_add(m_pEvent, &m_tv);
		}
	}
	//�ϲ���Ҫָ���ص�����
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
	void rescheduleTimer()
	{
		if (m_pEvent)
		{
			event_del(m_pEvent);
			evtimer_add(m_pEvent, &m_tv);
		}
	}
	//�ϲ���Ҫָ���ص�����
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
