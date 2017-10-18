#ifndef __TEST_PEER_H__
#define __TEST_PEER_H__

#include <iostream>
#include <chrono>
#include "base/socketPeer.h"



class CTestPeerCompactSender : public CLEPeer
{
public:
	using CLEPeer::CLEPeer;

	virtual bool afterRecv()
	{
		//尝试解包，只要满足长度就组包，并投递给相应的场景线程
		long long  dt = duration_cast<duration<long long, std::ratio<1, 1000000>>>(high_resolution_clock::now().time_since_epoch()).count();
		CByteArray ba;
		ba.tryWriteU64Int(dt);
		for (int i = 0; i < 5; ++i)
		{
			doSend(ba.getBuff(), ba.getDataLength());
		}
		//if (dt - m_dt > 50000)
		{
			std::cout<< socket() <<" message elapse ++++" <<dt - m_dt
				<<std::endl;

		}
		m_dt = dt;
		return CLEPeer::afterRecv();
	}
	void onConnected() override
	{
		m_dt = duration_cast<duration<long long, std::ratio<1, 1000000>>>(high_resolution_clock::now().time_since_epoch()).count();
		CByteArray ba;
		ba.tryWriteU64Int(m_dt);
		doSend(ba.getBuff(), ba.getDataLength());
		//char * pHello = "hello from client ";
		//doSend((unsigned char*)pHello, strlen(pHello));

	}
private:
	long long  m_dt;

};


#endif