#ifndef __SCENE_THREAD_H__
#define __SCENE_THREAD_H__

#include <map>
#include <list>
#include <chrono>
#include "base/objectPool.h"
#include "base/threadMgr.h"
#include "base/eventBase.h"
#include "scene.h"

//单线程跑所有场景
class CSceneThreadSingle: public CThreadBlocking
{
	typedef CThreadBlocking BASECLASS;
public :
	CSceneThreadSingle(int id):BASECLASS(id) {};
	~CSceneThreadSingle() {};

public:
	//以下三个地方需要对map对象枷锁保护
	//主线程以外的地方访问,锁保护
	virtual void onAddNetMessage(CEventBAClient* e)
	{
		m_start = high_resolution_clock::now();

		if (e )
		{
			//需要并发枷锁
			lockAdd();
			auto iter = m_mpScene.find(e->m_nTargetSceneId);
			if (iter != m_mpScene.end())
			{
				iter->second->recordTestTime();
				//既然加了锁，内部就不用处理了
				e->writeTo(iter->second->getByteArray());
				//加了消息以后，会默认提高场景的时间片响应级别

			}
			unlockAdd();
			//
			triggerNewEvent();
		}
	}
	//增加，锁保护
	void addNewScene(CScene* p)
	{
		if (p)
		{
			//需要并发枷锁
			lockAdd();
			m_mpScene[p->getDynID()] = p;
			unlockAdd();
		}
	}
	//删除，锁保护
	void checkSceneGarbage()
	{
		std::list<CScene*> _stillRunning;
		lockAdd();
		auto iter = m_lstGarbageScene.begin();
		for (; iter != m_lstGarbageScene.end(); iter++)
		{
			if ((*iter)->getWorkerThreadID() == 0)
			{
				m_mpScene.erase((*iter)->getDynID());
				(*iter)->setGarbage();
			}
			else
			{
				_stillRunning.push_back(*iter);
			}
		}
		unlockAdd();
		m_lstGarbageScene.clear();
		//下一次删除回收
		m_lstGarbageScene = _stillRunning;
	}
	virtual void checkAndRunScene(unsigned long long nMsNow)
	{
		auto iter = m_mpScene.begin();
		for (; iter != m_mpScene.end(); iter++)
		{
			unsigned long long tt = iter->second->getNextTriggleTime();
			//if (tt == 0)
			//{
			//	//
			//	m_lstGarbageScene
			//}
			if (tt < nMsNow)
			{
				iter->second->printTestTime();
				iter->second->runInThread(nMsNow);
				iter->second->calcNextTriggleTime();
			}
		}
	}
	void onRun(unsigned long long nMsNow) override
	{
		//std::cout << " scene thread on timer" << std::endl;
		checkSceneGarbage();
		checkAndRunScene(nMsNow);
	};

	virtual bool initThread()
	{
		if (BASECLASS::initThread())
		{
			//增加测试场景
			CScene * pScene = new CScene(0xff);
			addNewScene(pScene);
			return true;
		}
		return false;
	}

protected:
	std::map<unsigned int, CScene*> m_mpScene;
	std::list<CScene*> m_lstGarbageScene;
public:
	void lockAdd()
	{//原子变量做的自旋锁
		while (m_addLock.test_and_set());
	};
	void unlockAdd()
	{
		m_addLock.clear();
	};
private:
	std::atomic_flag m_addLock{ ATOMIC_FLAG_INIT };
	high_resolution_clock::time_point m_start;

};

//多线程跑多场景
class CSceneThreadMultiple;
class CSceneWorkerThread :public CThreadBlocking
{
public:
	CSceneWorkerThread(CSceneThreadMultiple* f, int id) :CThreadBlocking(id),m_pFather(f)
	{

	}
	void waitNotify() override;
	void onRun(unsigned long long nMsNow) override;

private:
	CSceneThreadMultiple* m_pFather;
};

//采用actor模式的多场景多线程处理方式
class CSceneThreadMultiple : public CSceneThreadSingle
{
	typedef CSceneThreadSingle BASECLASS;
public:
	void childWait()
	{
		m_condChild.wait();

	}
	void childNotify()
	{
		m_condChild.signalOnce();
	}
	virtual void onTimer(unsigned long long nMsNow)
	{
		//给所有场景设置为可触发
	}
	bool addWorker()
	{
		CSceneWorkerThread* p = new CSceneWorkerThread(this, 1);
		if (p
			&& p->initThread()
			&& p->start())
		{
			m_lstWorker.push_back(p);
			return true;
		}
		return false;
	}

	void checkAndRunScene(unsigned long long nMsNow) override
	{
		//交给worker去处理
		//todo 重新整理所有的时间片，所有的场景要被走一遍
	}
	CScene* pickSceneForChild(unsigned int childThreadID, unsigned long long nMsNow)
	{
		//枷锁
		CScene* r = nullptr;
		lockChild();
		auto iter = m_mpScene.begin();
		for (; iter != m_mpScene.end(); iter++)
		{
			unsigned long long tt = iter->second->getNextTriggleTime();
			//if (tt == 0)
			//{
			//	//
			//	m_lstGarbageScene
			//}
			if (tt < nMsNow 
				&& iter->second->getWorkerThreadID() == 0)
			{
				r = iter->second;
				r->setWorkerThreadID(childThreadID);
				break;
			}
		}
		unlockChild();
		return r;
	}

private:
	CConditionVariable m_condChild;
	std::list<CSceneWorkerThread*> m_lstWorker;

public:
	void lockChild()
	{//原子变量做的自旋锁
		while (m_childLock.test_and_set());
	};
	void unlockChild()
	{
		m_childLock.clear();
	};
private:
	std::atomic_flag m_childLock{ ATOMIC_FLAG_INIT };

};


#endif