#ifndef __SCENE_H__
#define __SCENE_H__

#include <list>
#include <map>
#include "base/timeAxis.h"
#include "base/threadMgr.h"
#include "base/objectPool.h"
#include "base/byteArray.h"
#include "player.h"

//场景类，维护玩家对象池。还有其他对象
//只有常驻场景有这个需求，其他副本其实会释放。
class CScene : public CTimerAxis, public IRunnable
{
public:
	CScene(unsigned int dynID):m_dynID(dynID),CTimerAxis()
	{
		m_playerPool.initMemPool(100);
	}
public:
	void handleEvent();
	void handleTimerNode(unsigned long long tNow)
	{
		CTimerNode* pTop = top();
		while (pTop)
		{
			if (pTop->getNextTriggleTime()== 0)
			{
				//需要删除这个对象
				pTop = popNode();
				pTop->setGarbage();
				pTop = nullptr;
			}
			else if (pTop->getNextTriggleTime() < tNow)
			{
				//触发逻辑
				//todo
				runTopLogic();
				//重新计算时间片
				pTop->calcNextTriggleTime();
				pushNode(pTop);
			}
			pTop = top();
		}

	}

	//线程主要回调用，类似timer的定时处理，内部按照时间轴处理。
	bool runInThread(unsigned long long tNow) override
	{
		//要优先处理网络消息
		handleEvent();
		handleTimerNode(tNow);

		return true;
	}
	CPlayer* getPlayerByID(unsigned int nPlayerID)
	{
		auto iter = m_mpPlayer.find(nPlayerID);
		if (iter != m_mpPlayer.end())
		{
			return iter->second;
		}
		return nullptr;
	}
	bool addEventData(unsigned char* p, size_t s)
	{
		return m_byteArray.tryWriteData(p, s);
	}
	CByteArray* getByteArray()
	{
		return &m_byteArray;
	}
	void addPlayer()
	{
		//创建玩家的时候要放入时间轴
	}
	void addMonster()
	{
		//创建怪物的时候要放入时间轴
	}

	unsigned long long getNextTriggleTime() const override
	{
		if (m_byteArray.getDataLength() > 0)
		{
			//当前有事件要处理，则优先级最高
			//返回0另有意义，也许不要这样设计？
			return 1;
		}
		else
		{
			return CTimerAxis::getNextTriggleTime();
		}
	}

private :
	//玩家对象池
	CObjectMemPool<CPlayer>		m_playerPool;
	//玩家快速索引，也就是说一个玩家指针，存放在时间轴里，同时存放在map映射里，当释放的时候，需要特别关照。
	//从堆里释放，有两种做法，
	//		1 从原始容器vector删除，需要重建堆吗？？？
	//		2 修改元素的值，重新排序，走弹出堆的模式？？
	typedef std::map<unsigned int, CPlayer*> MAP_ID_2_PLAYER;
	MAP_ID_2_PLAYER				m_mpPlayer;
	CByteArray					m_byteArray;//直接缓存本场景的消息

public:
	void setWorkerThreadID(int id)	{ m_workerThreadID = id; }
	int  getWorkerThreadID()		{return m_workerThreadID;}
private:
	//如果不等于0，说明这个场景还被工作者线程占用，还不能释放。
	int m_workerThreadID{ 0 };
public:
	unsigned int getDynID() { return m_dynID; }
private:
	unsigned int m_dynID{ 0 };
public:
	void recordTestTime()
	{
		m_start = high_resolution_clock::now();
	}
	void printTestTime()
	{
		double d = duration_cast<duration<double, std::ratio<1, 1000>>>(high_resolution_clock::now() - m_start).count();
		//if (d > 50)
		{
			std::cout << getDynID ()<<  " scene run message elapse " << d
				<< std::endl;
		}
		recordTestTime();
	}
private:
	high_resolution_clock::time_point m_start;

};

#endif