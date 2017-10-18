#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <thread>
#include "eventBase.h"
#include "blockingQueue.h"
#include <atomic>

/*
IThread  基本抽象
	倾向于不再使用这些
	IThreadHandleEvent  处理指针事件的基本类型  
		CThreadBlockingQueue 基于blockingqueue提供唤醒和阻塞
		CThreadLoopbuffLE     基于libevent提供唤醒和阻塞

	尽量使用下面的类型
	IThreadHandleByteArray  处理字符串事件的基本类型

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
		//退出的逻辑需要在子类里自己实现。
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
	//一个是定时触发,还有即时响应事件，都应该从同一个入口进入
	//内部要优先处理事件队列，也就是byteArray里的事件
	//nMsNow用来做判断，看是否真的调用timer响应，
	//如果内部是调用时间轴，那么这个参数也可以忽略
	virtual void onRun(unsigned long long nMsNow) {};//参数是当前时间，毫秒为单位
	//立即触发事件，这是在另外的线程里调用
	virtual void triggerNewEvent(){}

};

//主循环等待信号量触发
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
			//todo 获取本地时间
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
//	virtual void onHandleEvent(IEvent *&) {};//当有事件来，则触发
//	virtual bool addEvent(IEvent* e) { return false; }
//};
//
//class IThreadHandleByteArray :public IThread
//{
//public:
//	virtual bool addEvent(CEventBAHead*) { return false; }
//};

//通过blockingqueue阻塞
//class CThreadBlockingQueue : public  IThreadHandleEvent
//{
//private :
//	CMPSCLoopBuff m_evQueue;
//	bool m_bStop{ false };
//	void  handleEvent(IEvent& e)
//	{
//		//todo  要区分事件，按照这种模型，定时器和真正的事件是在同样处理的。
//	}
//public:
//	virtual bool initThread() { return false; }
//	virtual bool stop()
//	{
//		m_bStop = true;
//		//通知对象
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
//			//逻辑处理单个对象
//			handleEvent(p);
//			//todo 注意时序
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

//通过libevent驱动，可以注册自己的timer
#include <event2/event.h>
class CThreadLE : public  IThread
{
	event_base* m_pBase{ NULL };
	struct event * m_pEvent {NULL};//timer的事件
	struct timeval m_tv { 0, 10000 };//默认100毫秒
	bool			m_bNeedTimer{ false };
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
			//参考线程退出的时候event_base_free
			m_pBase = NULL;
		}
	}
public:
	bool setTimeElapse(int s, int us)
	{
		m_tv.tv_sec = s;
		m_tv.tv_usec = us;//⑩毫秒
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
		//todo,获取新的当下时间
		onRun(1);
	}
	void rescheduleTimer()
	{
		if (m_pEvent)
		{
			//todo 这里要细分一下，如果提前叫醒，那么下次timer注册要临时调整时间
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
			//if (m_bNeedTimer)
			{
				m_pEvent = evtimer_new(m_pBase, CThreadLE::cb, this);
				return m_pEvent
					&& 0 == evtimer_add(m_pEvent, &m_tv);

			}
			//else
			//{
			//	return true;
			//}
		} 
		return false;
	}

	bool stop() override
	{
		if (m_pBase)
		{
			struct timeval tv;
			tv.tv_sec = 0;
			tv.tv_usec = 1;
			event_base_loopexit(m_pBase, &tv);
		}
		return IThread::stop();
	}

	void run() override
	{
		event_base_loop(m_pBase, 0);
		event_base_free(m_pBase);
	}

	void triggerNewEvent() override
	{
		//听说跨线程的le性能消耗很大，先不要这样做
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
//			//参考线程退出的时候event_base_free
//			m_pBase = NULL;
//		}
//	}
//public:
//	event_base* getEventBase()
//	{
//		return m_pBase;
//	}
//	virtual void onHandleEvent(IEvent *&) {};//当有事件来，则触发
//	virtual void onTimer(unsigned long long nMsNow) {};//参数是当前时间，毫秒为单位
//
//	void onNewEventCallback()
//	{
//		IEvent* p = nullptr;
//		m_evQueue.pop(p);
//		while (p)
//		{
//			//逻辑处理单个对象
//			onHandleEvent(p);
//			//todo 注意时序
//			p->setGarbage();
//			p = nullptr;
//			m_evQueue.pop(p);
//		}
//		//还要检查timer时序
//		//todo 加上时间间隔检查，不要每次都调用这个
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
//		//重新加入timer
//		p->m_evQueue.rescheduleTimer();
//		p->onNewEventCallback();
//	}
//
//public:
//	virtual bool initThread() 
//	{
//		//默认十毫秒回调
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
