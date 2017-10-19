#include "logic/networkMsg.h"
#include "logic/server.h"
#include "logic/sceneThread.h"
#include "logic/event.h"

bool CTestEchoPeer::afterRecv()
{
	//std::cout << m_buff << std::endl;
	CEventBAClient e(m_buff, m_nRecvIndex, 0, 0xff, m_nSocket);
	g_pServer->m_pSThread->onAddNetMessage(&e);
	//返回消息
	//doSend(m_buff, m_nRecvIndex);
	CLEPeer::afterRecv();
	//尝试解包，只要满足长度就组包，并投递给相应的场景线程
	return true;
}
