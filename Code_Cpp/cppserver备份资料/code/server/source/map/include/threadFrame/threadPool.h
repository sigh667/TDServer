#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <thread>
#include "eventBase.h"
#include "blockingQueue.h"
#include <atomic>
class IThreadHandleEvent
{
protected:

	std::thread* m_pThread;
public:
	IThreadHandleEvent()
	{
		m_pThread = nullptr;
	};
	virtual ~IThreadHandleEvent() 
	{
		if (m_pThread)
		{
			delete m_pThread;
			m_pThread = nullptr;
		}
	};
public:
	static void * threadFunc(void * p)
	{
		IThreadHandleEvent* pThis = static_cast<IThreadHandleEvent*>(p);
		if (pThis)
		{
			pThis->run();
		}
		return pThis;

	};
	bool start()
	{
		if (initThread())
		{
			m_pThread = new std::thread(IThreadHandleEvent::threadFunc, this);
			if (m_pThread)
			{
				return true;
			}
		}
		return false;
	}
	virtual bool initThread() { return false; }
	virtual bool stop() 
	{
		//�˳����߼���Ҫ���������Լ�ʵ�֡�
		if (m_pThread)
		{
			m_pThread->join();
		}
		return true; 
	}
	virtual void run() {}
	virtual bool addEvent(IEvent*) {};
	virtual void onHandleEvent(IEvent*) {};

};

//ͨ��blockingqueue��������������������actor
template<class EVENTQUEUE>class CThreadBlockingQueue : public  IThreadHandleEvent
{
private :
	EVENTQUEUE m_evQueue;
	bool m_bStop{ false };

public:
	virtual bool initThread() { return false; }
	virtual bool stop()
	{
		m_bStop = true;
		//֪ͨ����
		m_evQueue.signalNotify();
		if (m_pThread)
		{
			m_pThread->join();
		}
		return true;
	}
	virtual void run() 
	{
		IEvent* p = nullptr;
		m_evQueue.pop(p);
		while (p && m_bStop == false)
		{
			//�߼�����������
			onHandleEvent(p);
			//todo ע��ʱ��
			p->setGarbage();
			p = nullptr;
			m_evQueue.pop(p);
		}
	}
	virtual bool addEvent(IEvent* e) 
	{
		if(e)
		{
			m_evQueue.push(e);
		}
	};

};

//ͨ��libevent����������ע���Լ���timer
#include <event2/event.h>
class CThreadLE : public  IThreadHandleEvent
{
	CSPSCLELoopBuff m_evQueue;
	event_base* m_pBase;
public:
	CThreadLE() 
	{
		m_pBase = NULL;
	}
	virtual ~CThreadLE()
	{
		if (m_pBase)
		{
			delete m_pBase;
			m_pBase = NULL;
		}
	}
public:
	event_base* getEventBase()
	{
		return m_pBase;
	}

	void onNewEventCallback()
	{
		IEvent* p = nullptr;
		m_evQueue.pop(p);
		while (p)
		{
			//�߼�����������
			onHandleEvent(p);
			//todo ע��ʱ��
			p->setGarbage();
			p = nullptr;
			m_evQueue.pop(p);
		}
	}
	static void cb(int sock, short which, void *arg)
	{
		if (!arg)
		{
			return;
		}
		CThreadLE* p = (CThreadLE*)arg;
		if (!p)
		{
			return;
		}

		// if (!evtimer_pending(p->m_pEv, NULL)) 
		//���¼���timer
		p->m_evQueue.rescheduleTimer();
		p->onNewEventCallback();
	}

public:
	virtual bool initThread() 
	{
		m_pBase = event_base_new();
		return m_pBase != NULL
			&& m_evQueue.init(m_pBase, 0, 100, CThreadLE::cb, this);
	}

	virtual bool stop()
	{
		if (m_pBase)
		{
			struct timeval tv;
			tv.tv_sec = 0;
			tv.tv_usec = 1;
			event_base_loopexit(m_pBase, &tv);
		}
		return IThreadHandleEvent::stop();
	}
	virtual bool addEvent(IEvent* e)
	{
		if (e)
		{
			m_evQueue.push(e);
		}
	};

	virtual void run() 
	{
		event_base_loop(m_pBase, 0);
		event_base_free(m_pBase);
	}
};

//class CThreadManager
//{
//private:
//	IThreadHandleEvent* 
//};

#endif // !__THREAD_POOL__
