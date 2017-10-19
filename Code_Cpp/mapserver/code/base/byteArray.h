#ifndef __BYTE_ARRAY_H__
#define __BYTE_ARRAY_H__

#define BYTE_ARRAY_MAX_LENGTH (1024*32)
//用loopbuff存放消息缓存，包括写入和弹出功能
//提供spin lock，但是调用交给上层逻辑自己处理，内部不做假设，
#include <atomic>
#include "base/cppByteArray.h"
class CByteArray:public CPPByteArray
{
public:
	void writeLock()
	{//原子变量做的自旋锁
		while (m_writeLock.test_and_set());
	};
	void writeUnlock()
	{
		m_writeLock.clear();
	};
	void readLock()
	{//原子变量做的自旋锁
		while (m_readLock.test_and_set());
	};
	void readUnlock()
	{
		m_readLock.clear();
	};


private:
	std::atomic_flag m_writeLock{ ATOMIC_FLAG_INIT };
	std::atomic_flag m_readLock{ ATOMIC_FLAG_INIT };
};

#endif // !__BYTE_ARRAY_H__

