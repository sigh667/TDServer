#ifndef __SERVER_H__
#define __SERVER_H__

#include <list>
#include <iostream>
#include<event2/event.h>    
#include<event2/thread.h>

#include "base/objectPool.h"
#include "base/threadMgr.h"
#include "test/testClientNetwork.h"

class CTestServer : public CThreadLE
{
	typedef CThreadLE BASECLASS;

public:
	CTestServer() :BASECLASS(0) {};
	~CTestServer() {};

public:
	virtual bool initThread()
	{
		setNeedTimer(true);
		if (BASECLASS::initThread())
		{
			return startServer();
		}
		return false;
	}
	void onRun(unsigned long long nMsNow) override
	{
		if (m_ClientPeerCount ++ < 1000)
		{
			m_lstThreads.front()->addConnector("127.0.0.1", 19999);
		}
		
		//std::cout << "the server timer " << std::endl;
		//管理创建场景的问题。或者是通过消息传递过来在handleEvent里面做
	}

	bool startServer()
	{
		//根据配置启动场景对象，需要重新编译libevent，打开多线程的限制
		//if (0 != evthread_use_pthreads())
		//{
		//	return -1;
		//}

		//创建io线程
		m_pNThread = new CTestClientNetwork(2);
		if (!m_pNThread
			|| !m_pNThread->setTimeElapse(0, 100000)
			|| !m_pNThread->initThread()
			//|| !m_pNThread->addConnector("127.0.0.1", 19999)
			|| !m_pNThread->start())
		{
			return false;
		}
		m_lstThreads.push_back(m_pNThread);


		return true;
	}

public:
	CTestClientNetwork * m_pNThread{ nullptr };

private:
	std::list<CTestClientNetwork*> m_lstThreads;
	int m_ClientPeerCount{ 0 };
};
extern CTestServer * g_pServer;
#endif


