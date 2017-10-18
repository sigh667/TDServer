#ifndef __EVENT_BASE_H__
#define __EVENT_BASE_H__
#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>
#include <mutex>          // std::mutex
#include <condition_variable> 
#include <list>
#include "base/byteArray.h"
using namespace std::chrono;

class CEventMemcpy;
class IEvent : public IMemObject
{
private:
	////测试用------------------------
	int m_testCount;//测试用
	high_resolution_clock::time_point m_start;
	////测试用------------------------

public:
	IEvent()
	{
		m_start = high_resolution_clock::now();
	}
	virtual~IEvent() {};
	//测试用------------------------
	const double print()
	{
		return duration_cast<duration<double> >(high_resolution_clock::now() - m_start).count();
	}
	//测试用------------------------
public :
	virtual CEventMemcpy* asEventMemcpy() { return nullptr; }
};

class CEventMemcpy :public IEvent
{
public:
	CEventMemcpy(unsigned char * p, size_t s, int t) :
		m_pData(p)
		, m_nSize(s)
		, m_nTargetSceneId(t)
	{
	}
public :
	CEventMemcpy* asEventMemcpy() override { return this; }
	unsigned char*	m_pData{NULL};//不要管释放
	size_t			m_nSize{ 0 };
	int				m_nTargetSceneId{ 0 };
};

enum EINNERTYPE
{
	EVENT_INNERTYPE_MSG_FROM_CLIENT = 1,
	EVENT_INNERTYPE_MSG_FROM_WORLD = 2,
	EVENT_INNERTYPE_MSG_TIMMER_TRIGGER = 3,

};

class IEventByteArray
{
public:
	virtual bool	readFrom(CByteArray*) = 0;
	virtual bool	writeTo(CByteArray*) = 0;
	virtual size_t  getByteCount() = 0;

};



#endif // ! __EVENT_BASE_H__
