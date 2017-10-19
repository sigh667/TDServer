#include "logic/sceneThread.h"
#include "logic/scene.h"
#include "logic/server.h"
#include "logic/networkThread.h"
#include "logic/networkMsg.h"

void CSceneWorkerThread::waitNotify()
{
	if (m_pFather)
	{
		m_pFather->childNotify();
	}
}

void CSceneWorkerThread::onRun(unsigned long long nMsNow)
{
	//遍历所有场景，直到所有场景都执行过timer
	if (m_pFather)
	{
		CScene* p = m_pFather->pickSceneForChild(getThreadID(), nMsNow);
		while (p)
		{
			p->runInThread(nMsNow);
			p = nullptr;
			//每次都尽量取完所有的可执行场景。
			p = m_pFather->pickSceneForChild(getThreadID(), nMsNow);
		}
	}
};

