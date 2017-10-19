#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <thread>
#include "eventBase.h"
#include "blockingQueue.h"
#include <atomic>

/*
IThread  ��������
	�����ڲ���ʹ����Щ
	IThreadHandleEvent  ����ָ���¼��Ļ�������  
		CThreadBlockingQueue ����blockingqueue�ṩ���Ѻ�����
		CThreadLoopbuffLE     ����libevent�ṩ���Ѻ�����

	����ʹ�����������
	IThreadHandleByteArray  �����ַ����¼��Ļ�������

*/




class IThread
{
protected:
	int m_threadID{ 0 };
	std::thread* m_pThread;
public:
	IThread(int id):m_threadID(id)
	{
		m_pThread = nullptr;
	};
	virtual ~IThread() 
	{
		if (m_pThread)
		{
			delete m_pThread;
			m_pThread = nullptr;
		}
	};
	int getThreadID() { return m_threadID; }
public:
	static void * threadFunc(void * p)
	{
		IThread* pThis = static_cast<IThread*>(p);
		if (pThis)
		{
			pThis->run();
		}
		return pThis;

	};
	bool start()
	{
		m_pThread = new std::thread(IThread::threadFunc, this);
		if (m_pThread)
		{
			return true;
		}
		return false;
	}
	virtual bool initThread() { return true; }
	virtual bool stop() 
	{
		//�˳����߼���Ҫ���������Լ�ʵ�֡�
		if (m_pThread)
		{
			m_pThread->join();
		}
		return true; 
	}
	virtual void wait() 
	{
		if (m_pThread)
		{
			m_pThread->join();
		}
	}
	virtual void run() {}
	//һ���Ƕ�ʱ����,���м�ʱ��Ӧ�¼�����Ӧ�ô�ͬһ����ڽ���
	//�ڲ�Ҫ���ȴ����¼����У�Ҳ����byteArray����¼�
	//nMsNow�������жϣ����Ƿ���ĵ���timer��Ӧ��
	//����ڲ��ǵ���ʱ���ᣬ��ô�������Ҳ���Ժ���
	virtual void onRun(unsigned long long nMsNow) {};//�����ǵ�ǰʱ�䣬����Ϊ��λ
	//���������¼���������������߳������
	virtual void triggerNewEvent(){}

};

//��ѭ���ȴ��ź�������
class CThreadBlocking : public  IThread
{
public:
	CThreadBlocking(int id):IThread(id)
	{
	};

	void run() override
	{
		while (!m_bStop)
		{
			waitNotify();
			//todo ��ȡ����ʱ��
			onRun(10);
		}
	}
	void triggerNewEvent() override
	{
		signalNotify();
	}

	virtual void signalNotify()
	{
		m_cond.signalOnce();
	};
	virtual void waitNotify()
	{
		m_cond.waitFor(1);
	}
	virtual bool stop()
	{
		m_bStop = true;
		signalNotify();
		if (m_pThread)
		{
			m_pThread->join();
		}
		return true;
	}

private:
	CConditionVariable m_cond;
	bool m_bStop{ false };

};

//class IThreadHandleEvent :public IThread
//{
//public:
//	virtual void onHandleEvent(IEvent *&) {};//�����¼������򴥷�
//	virtual bool addEvent(IEvent* e) { return false; }
//};
//
//class IThreadHandleByteArray :public IThread
//{
//public:
//	virtual bool addEvent(CEventBAHead*) { return false; }
//};

//ͨ��blockingqueue����
//class CThreadBlockingQueue : public  IThreadHandleEvent
//{
//private :
//	CMPSCLoopBuff m_evQueue;
//	bool m_bStop{ false };
//	void  handleEvent(IEvent& e)
//	{
//		//todo  Ҫ�����¼�����������ģ�ͣ���ʱ�����������¼�����ͬ������ġ�
//	}
//public:
//	virtual bool initThread() { return false; }
//	virtual bool stop()
//	{
//		m_bStop = true;
//		//֪ͨ����
//		m_evQueue.signalNotify();
//		if (m_pThread)
//		{
//			m_pThread->join();
//		}
//		return true;
//	}
//	virtual void run() 
//	{
//		IEvent* p = nullptr;
//		m_evQueue.pop(p);
//		while (p && m_bStop == false)
//		{
//			//�߼�����������
//			handleEvent(p);
//			//todo ע��ʱ��
//			p->setGarbage();
//			p = nullptr;
//			m_evQueue.pop(p);
//		}
//	}
//	virtual bool addEvent(IEvent* e) 
//	{
//		if(e)
//		{
//			m_evQueue.push(e);
//		}
//	};
//
//};

//ͨ��libevent����������ע���Լ���timer
#include <event2/event.h>
class CThreadLE : public  IThread
{
	event_base*		m_pBase{ NULL };
	struct event *	m_pEvent {NULL};//timer���¼�
	struct timeval	m_tv { 0, 10000 };//Ĭ��100����
	bool			m_bNeedTimer{ false };
	bool			m_bExit{ false };
public:
	CThreadLE(int id):IThread(id)
	{
		m_pBase = NULL;
	}
	virtual ~CThreadLE()
	{
		if (m_pBase)
		{
			//delete m_pBase;
			//�ο��߳��˳���ʱ��event_base_free
			m_pBase = NULL;
		}
	}
public:
	bool setTimeElapse(int s, int us)
	{
		m_tv.tv_sec = s;
		m_tv.tv_usec = us;//�����
		return true;
	}
	void setNeedTimer(bool b) { m_bNeedTimer = b; }
	int getThreadID() { return m_threadID; }
	event_base* getEventBase()
	{
		return m_pBase;
	}

	virtual void onCallBack()
	{
		rescheduleTimer();
		//todo,��ȡ�µĵ���ʱ��
		onRun(1);
	}
	void rescheduleTimer()
	{
		if (m_pEvent)
		{
			//todo ����Ҫϸ��һ�£������ǰ���ѣ���ô�´�timerע��Ҫ��ʱ����ʱ��
			event_del(m_pEvent);
			evtimer_add(m_pEvent, &m_tv);
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
		p->onCallBack();
	}

public:
	bool initThread() override
	{
		m_pBase = event_base_new();
		if (m_pBase != NULL)
		{
			if (m_bNeedTimer)
			{
				m_pEvent = evtimer_new(m_pBase, CThreadLE::cb, this);
				return m_pEvent
					&& 0 == evtimer_add(m_pEvent, &m_tv);

			}
			else
			{
				return true;
			}
		} 
		return false;
	}

	bool stop() override
	{
		if (m_pBase)
		{
			m_bExit = true;
			struct timeval tv;
			tv.tv_sec = 1;//һ���Ժ��˳�
			tv.tv_usec = 0;
			event_base_loopexit(m_pBase, &tv);
		}
		return IThread::stop();
	}

	void run() override
	{
		while (!m_bExit)
		{
			if (-1 == event_base_loop(m_pBase, EVLOOP_ONCE))
			{
				//����
				break;
			}
		}
		event_base_free(m_pBase);
	}

	void triggerNewEvent() override
	{
		//��˵���̵߳�le�������ĺܴ��Ȳ�Ҫ������
		//if (m_pEvent)
		//{
		//	event_active(m_pEvent, EV_TIMEOUT, 0);
		//}
	}

};



//
//class CThreadBlockingQueueLE : public  IThreadHandleEvent
//{
//	CSPSCLELoopBuff m_evQueue;
//	event_base* m_pBase;
//public:
//	CThreadBlockingQueueLE() 
//	{
//		m_pBase = NULL;
//	}
//	virtual ~CThreadBlockingQueueLE()
//	{
//		if (m_pBase)
//		{
//			//delete m_pBase;
//			//�ο��߳��˳���ʱ��event_base_free
//			m_pBase = NULL;
//		}
//	}
//public:
//	event_base* getEventBase()
//	{
//		return m_pBase;
//	}
//	virtual void onHandleEvent(IEvent *&) {};//�����¼������򴥷�
//	virtual void onTimer(unsigned long long nMsNow) {};//�����ǵ�ǰʱ�䣬����Ϊ��λ
//
//	void onNewEventCallback()
//	{
//		IEvent* p = nullptr;
//		m_evQueue.pop(p);
//		while (p)
//		{
//			//�߼�����������
//			onHandleEvent(p);
//			//todo ע��ʱ��
//			p->setGarbage();
//			p = nullptr;
//			m_evQueue.pop(p);
//		}
//		//��Ҫ���timerʱ��
//		//todo ����ʱ������飬��Ҫÿ�ζ��������
//		onTimer(1);
//	}
//	static void cb(int sock, short which, void *arg)
//	{
//		if (!arg)
//		{
//			return;
//		}
//		CThreadBlockingQueueLE* p = (CThreadBlockingQueueLE*)arg;
//		if (!p)
//		{
//			return;
//		}
//
//		// if (!evtimer_pending(p->m_pEv, NULL)) 
//		//���¼���timer
//		p->m_evQueue.rescheduleTimer();
//		p->onNewEventCallback();
//	}
//
//public:
//	virtual bool initThread() 
//	{
//		//Ĭ��ʮ����ص�
//		m_pBase = event_base_new();
//		return m_pBase != NULL
//			&& m_evQueue.init(m_pBase, 0, 10000, CThreadBlockingQueueLE::cb, this);
//	}
//
//	virtual bool stop()
//	{
//		if (m_pBase)
//		{
//			struct timeval tv;
//			tv.tv_sec = 0;
//			tv.tv_usec = 1;
//			event_base_loopexit(m_pBase, &tv);
//		}
//		return IThread::stop();
//	}
//	virtual bool addEvent(IEvent* e)
//	{
//		if (e)
//		{
//			m_evQueue.push(e);
//			return true;
//		}
//		return false;
//	};
//
//	virtual void run() 
//	{
//		event_base_loop(m_pBase, 0);
//		event_base_free(m_pBase);
//	}
//};


class IRunnable
{
public :
	virtual bool runInThread(unsigned long long ) = 0;
};



#endif // !__THREAD_POOL__
