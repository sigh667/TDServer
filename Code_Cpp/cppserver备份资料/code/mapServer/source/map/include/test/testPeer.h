#ifndef __TEST_PEER_H__
#define __TEST_PEER_H__

#include <iostream>
#include <chrono>
#include "base/socketPeer.h"



class CTestPeerCompactSender : public CLEPeer
{
public:
	using CLEPeer::CLEPeer;

	bool afterRecv() override
	{
		//���Խ����ֻҪ���㳤�Ⱦ��������Ͷ�ݸ���Ӧ�ĳ����߳�
		long long  dt = duration_cast<duration<long long, std::ratio<1, 1000000>>>(high_resolution_clock::now().time_since_epoch()).count();
		//CByteArray ba;
		//ba.tryWriteU64Int(dt);
		//for (int i = 0; i < 5; ++i)
		//{
		//	doSend(ba.getBuff(), ba.getDataLength());
		//}
		//if (dt - m_dt > 50000)
		{
			std::cout<< socket() <<" message elapse ++++" <<dt - m_dt
				<<std::endl;

		}
		m_dt = dt;
		m_bSend = false;
		return CLEPeer::afterRecv();
	}
	void onConnected() override
	{
		doTestSend();
	}

	void doTestSend()
	{
		if (!m_bSend)
		{
			m_bSend = true;
			m_dt = duration_cast<duration<long long, std::ratio<1, 1000000>>>(high_resolution_clock::now().time_since_epoch()).count();
			CByteArray ba;
			ba.tryWriteU64Int(m_dt);
			doSend(ba.getBuff(), ba.getDataLength());
		}

	}

private:
	long long  m_dt;
	bool m_bSend{ false };

};


#endif