#ifndef __SERVER_H__
#define __SERVER_H__

#include <list>
#include <iostream>
#include<event2/event.h>    
#include<event2/thread.h>

#include "base/objectPool.h"
#include "base/threadMgr.h"
#include "logic/networkThread.h"
#include "logic/sceneThread.h"

class CServer: public CThreadLE
{
	typedef CThreadLE BASECLASS;

public :
	CServer():BASECLASS(0) {};
	~CServer() {};

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
		m_pNThread = new CNetworkThread(2);
		if (!m_pNThread
			|| !m_pNThread->setTimeElapse(0,100)
			|| !m_pNThread->initThread()
			|| !m_pNThread->addListener("127.0.0.1", 19999)
			|| !m_pNThread->start())
		{
			return false;
		}
		m_lstThreads.push_back(m_pNThread);

		////创建场景线程
		m_pSThread = new CSceneThreadSingle(2);
		if (!m_pSThread
			|| !m_pSThread->initThread()
			|| !m_pSThread->start())
		{
			return false;
		}
		m_lstThreads.push_back(m_pSThread);

		return true;
	}

public:
	CNetworkThread * m_pNThread{ nullptr };
	CSceneThreadSingle * m_pSThread{ nullptr };

private:
	std::list<IThread*> m_lstThreads;
};
extern CServer * g_pServer;
#endif


