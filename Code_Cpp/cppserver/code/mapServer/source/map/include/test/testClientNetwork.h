#ifndef  __TEST_CLIENT_NETWORK_H__
#define __TEST_CLIENT_NETWORK_H__

#include "logic/networkThread.h"

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
			//���͵�½��Ϣ�������Լ����
		}
	}

	bool addConnector(const char* ip, short port) override;

};
#endif // ! __TEST_CLIENT_NETWORK_H__


