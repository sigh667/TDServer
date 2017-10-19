#ifndef  __TEST_CLIENT_NETWORK_H__
#define __TEST_CLIENT_NETWORK_H__

#include "logic/networkThread.h"
#include "test/testPeer.h"

class CTestClientNetwork :public CNetworkThread
{
	typedef CNetworkThread BASECLASS;
public:
	using CNetworkThread::CNetworkThread;


	bool initThread() override
	{
		setNeedTimer(true);
		return BASECLASS::initThread();
	}

	void addNewPeer(CLEPeer*p) override
	{
		if (p)
		{
			m_mpPeers[p->socket()] = p;
			//发送登陆消息，表明自己身份
		}
	}

	bool addConnector(const char* ip, short port) override;

	void onRun(unsigned long long nMsNow) override
	{
		auto iter = m_mpPeers.begin();
		for (; iter != m_mpPeers.end(); iter++)
		{
			CTestPeerCompactSender* p = (CTestPeerCompactSender*)iter->second;
			p->doTestSend();
		}
	};




};
#endif // ! __TEST_CLIENT_NETWORK_H__


