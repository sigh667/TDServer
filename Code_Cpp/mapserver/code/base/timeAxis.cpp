#include "base/timeAxis.h"

bool timeNodeCompareBigger(const CTimerNode* a, const CTimerNode* b)
{
	if (a && b)
	{
		return a->getNextTriggleTime() > b->getNextTriggleTime();
	}
	return false;
}

CTimerAxis::CTimerAxis() :CTimerNode(0, 0, 0)
{
	std::make_heap(m_vcNodes.begin(), m_vcNodes.end()
		, timeNodeCompareBigger);
}


