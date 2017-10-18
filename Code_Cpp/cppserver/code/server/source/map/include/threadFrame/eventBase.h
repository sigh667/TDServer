#ifndef __EVENT_BASE_H__
#define __EVENT_BASE_H__
#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>
#include <mutex>          // std::mutex
#include <condition_variable> 
#include <list>

using namespace std::chrono;
class IEvent
{
private:
	bool m_isGarbage{false};//垃圾标志，统一释放，或者回收 
	////测试用------------------------
	int m_testCount;//测试用
	high_resolution_clock::time_point m_start;
	////测试用------------------------

public:
	void setGarbage() { m_isGarbage = true; };
	IEvent(int i)
	{
		m_isGarbage = false;
		m_testCount = i;
		m_start = high_resolution_clock::now();
	}
	virtual~IEvent() {};
	//测试用------------------------
	const double print()
	{
		return duration_cast<duration<double> >(high_resolution_clock::now() - m_start).count();
	}
	//测试用------------------------
};


class CEventPool
{
	std::list<IEvent*> m_lstPool;
};




#endif // ! __EVENT_BASE_H__
