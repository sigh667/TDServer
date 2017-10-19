#ifndef __TIME_AXIS_H__
#define __TIME_AXIS_H__
#include <algorithm>    // std::make_heap, std::pop_heap, std::push_heap, std::sort_heap
#include <vector>       // std::vector

#include "objectPool.h"


// 时间节点包装类
class CTimerNode :public IMemObject
{
	unsigned long long 	m_uMsNextTriggleTime{0};//单位 毫秒
	int					m_nLeftLoopCount{0};
	int					m_nMsCD{0};
public:
	CTimerNode(unsigned long long n, int l, int cd):
		m_uMsNextTriggleTime(n)
		,m_nLeftLoopCount(l)
		,m_nMsCD(cd){}
	virtual ~CTimerNode(){}

public:
	virtual unsigned long long getNextTriggleTime() const
	{
		//返回0 等于退出
		return m_uMsNextTriggleTime;
	}
	virtual void calcNextTriggleTime()
	{
		if (m_nLeftLoopCount == 0)
		{
			m_uMsNextTriggleTime = 0;
		}
		else
		{
			m_uMsNextTriggleTime += m_nMsCD;
			if (m_nLeftLoopCount > 0)
			{
				m_nLeftLoopCount--;
			}
		}
	}
	virtual bool runTopLogic() { return false; }
};

class CTimerAxis:public CTimerNode
{
	std::vector<CTimerNode*> m_vcNodes;

public:
	CTimerAxis();
	void pushNode(CTimerNode*p)
	{
		if (p)
		{
			m_vcNodes.push_back(p);
			std::push_heap(m_vcNodes.begin(), m_vcNodes.end());
		}
	}

	CTimerNode* popNode()
	{
		CTimerNode* r = nullptr;
		if (m_vcNodes.size()>0)
		{
			r = m_vcNodes.front();

			std::pop_heap(m_vcNodes.begin(), m_vcNodes.end());
			m_vcNodes.pop_back();
		}
		return r;
	}

	CTimerNode* top() const
	{
		CTimerNode* r = nullptr;
		if (m_vcNodes.size()>0)
		{
			r = m_vcNodes.front();
		}
		return r;
	}

	virtual unsigned long long getNextTriggleTime() const
	{
		//返回0xffff 等于退出
		CTimerNode* t = top();
		if (t)
		{
			return t->getNextTriggleTime();
		}
		return 0xffff;
	}
	virtual void calcNextTriggleTime()
	{
		CTimerNode* t = popNode();
		if (t)
		{
			t->calcNextTriggleTime();
			pushNode(t);
		}
	}
	virtual bool runTopLogic() 
	{
		CTimerNode* t = top();
		if (t)
		{
			t->runTopLogic();
			return true;
		}
		return false;
	}

};

#endif // !__TIME_AXIS_H__

