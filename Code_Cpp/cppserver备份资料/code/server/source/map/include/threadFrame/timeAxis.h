#ifndef __TIME_AXIS_H__
#define __TIME_AXIS_H__
#include <algorithm>    // std::make_heap, std::pop_heap, std::push_heap, std::sort_heap
#include <vector>       // std::vector



// ʱ��ڵ��װ��
class CTimerNode 
{
	unsigned long long 	m_uNextTriggleTime{0};
	int					m_nLeftLoopCount{0};
	int					m_nCD{0};
public:
	CTimerNode(unsigned long long n, int l, int cd):
		m_uNextTriggleTime(n)
		,m_nLeftLoopCount(l)
		,m_nCD(cd){}
	virtual ~CTimerNode(){}

public:
	unsigned long long getNextTriggleTime() const
	{
		//����0 �����˳�
		return m_uNextTriggleTime;
	}
	void calcNextTriggleTime()
	{
		if (m_nLeftLoopCount == 0)
		{
			m_uNextTriggleTime = 0;
		}
		else
		{
			m_uNextTriggleTime += m_nCD;
			if (m_nLeftLoopCount > 0)
			{
				m_nLeftLoopCount--;
			}
		}
	}
};


bool timeNodeCompareBigger(const CTimerNode* a, const CTimerNode* b)
{
	return a->getNextTriggleTime() > b->getNextTriggleTime();
}
class CTimerAxis
{
	std::vector<CTimerNode*> m_vcNodes;

public:
	CTimerAxis()
	{
		std::make_heap(m_vcNodes.begin(), m_vcNodes.end());
	}
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

	CTimerNode* top()
	{
		CTimerNode* r = nullptr;
		if (m_vcNodes.size()>0)
		{
			r = m_vcNodes.front();
		}
		return r;
	}
};


//class STimeElementSort
//{
//public:
//	bool  operator() ( const STimeElement* left, const STimeElement* right ) const
//	{
//		return left->m_U_UINT64 > right->m_U_UINT64;
//	}
//};
//
//typedef std::list< STimeElement*, std::SR_allocator<STimeElement*> >	TEList;
//
//// ʱ����
//class CTimeAxis
//{
//public:
//	CTimeAxis();
//	~CTimeAxis();
//
//	BOOL	HeartBeat( const U_UINT64& uTime );
//
//	// �����¼���nMilliseconds����֮����Ӧ
//	VOID	Insert( CTimeEvent* pEvent, UINT nMilliseconds );
//	// �����¼�����ʱ��ΪuANSITimen��Milliseconds����֮����Ӧ
//	VOID	Insert( CTimeEvent* pEvent, UINT uANSITime, UINT nMilliseconds );
//
//	VOID	Init( CSR_ScriptObj* pScriptObj );
//	VOID	Release();
//
//	bool			Delete( int eType );
//	STimeElement*	FindFirst( int eType );
//	VOID			FindAll( int eType, TEList& teList );
//
//private:
//	typedef _SARA::priority_queue_sara< STimeElement*, std::vector<STimeElement*>, STimeElementSort >	ElementHeap;
//	ElementHeap		m_Heap;
//
//	CSR_ScriptObj*	m_pParent;	// ʱ�������ڵ�����	�˶���һ����ȫ��ID����ֻ�ܱ���ָ��
//};

#endif // !__TIME_AXIS_H__

