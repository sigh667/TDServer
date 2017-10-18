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
		//���������������⡣������ͨ����Ϣ���ݹ�����handleEvent������
	}

	bool startServer()
	{
		//����������������������Ҫ���±���libevent���򿪶��̵߳�����
		//if (0 != evthread_use_pthreads())
		//{
		//	return -1;
		//}

		//����io�߳�
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


