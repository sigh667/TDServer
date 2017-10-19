#include "logic/scene.h"
#include "logic/server.h"
#include "logic/networkThread.h"
#include "logic/networkMsg.h"
#include "logic/event.h"

void CScene::handleEvent()
{//处理消息队列
	if (m_byteArray.getDataLength() > 0)
	{

		CEventBAHead head;
		while (head.fakeReadFrom(&m_byteArray))
		{
			m_byteArray.tryMoveReadIndex(head.getHeadSize());
			CLEPeer* peer = g_pServer->m_pNThread->getPeer(head.m_nSocket);
			if (peer)
			{
				peer->doSend(m_byteArray.getBuff(), m_byteArray.getDataLength());
			}
			long long d = m_byteArray.tryReadU64Int();
			long long  dt = duration_cast<duration<long long, std::ratio<1, 1000000>>>(high_resolution_clock::now().time_since_epoch()).count();
			{
				std::cout << " handle event time elapse------" << dt - d<<std::endl;
			}
			m_byteArray.tryMoveReadIndex(head.getByteCount() - 8 - head.getHeadSize());

			//g_pServer->m_pNThread->getFirstPeer()->doSend(buff, s);
		}
	}
}
